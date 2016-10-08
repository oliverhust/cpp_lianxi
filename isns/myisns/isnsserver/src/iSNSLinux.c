/***********************************************************************
 Copyright (c) 2001, Nishan Systems, Inc.
 All rights reserved.
 
 Redistribution and use in source and binary forms, with or without 
 modification, are permitted provided that the following conditions are 
 met:
 
 - Redistributions of source code must retain the above copyright notice, 
   this list of conditions and the following disclaimer. 
 
 - Redistributions in binary form must reproduce the above copyright 
   notice, this list of conditions and the following disclaimer in the 
   documentation and/or other materials provided with the distribution. 
 
 - Neither the name of the Nishan Systems, Inc. nor the names of its 
   contributors may be used to endorse or promote products derived from 
   this software without specific prior written permission. 
 
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
 IMPLIED WARRANTIES OF MERCHANTABILITY, NON-INFRINGEMENT AND FITNESS FOR A 
 PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL NISHAN SYSTEMS, INC. 
 OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
 PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; 
 OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
 WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR 
 OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
 ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************/
#include <semaphore.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

#include "iSNSLinux.h"
#include "iSNS.h"
#include "iSNScomm.h"
#include "iSNSdb.h"
#include "iSNSmsg.h"
#include "iSNSqueue.h"
#include "iSNSfsm.h"
#include "iSNScomm.h"
#include "iSNSbuffer.h"
#include "iSNSipc.h"
#include "iSNSdebug.h"

#ifndef SNS_LINUX
#define SNS_LINUX
#endif

extern int sns_ready;
extern int sns_local_priority;
extern int sns_local_version;
extern int sns_heartbeat_interval;
extern int sns_esi_interval;
extern int sns_request_timeout;
extern int sns_comm_main_port_set;

/* Linux */
extern pthread_cond_t esi_cond;
extern pthread_cond_t fsm_cond;
extern pthread_cond_t resync_cond;
extern pthread_mutex_t sns_request_timer;
extern pthread_cond_t request_cond;
extern pthread_mutex_t sns_resync_timer;
extern pthread_mutex_t sns_fsm_timer;
extern pthread_mutex_t sns_esi_timer;

/* log */
int daemon_state = TRUE;

/* debug */
extern int sns_bcast_debug;
extern int sns_cb_debug;
extern int sns_fsm_debug;
extern int sns_hb_debug;
extern int isns_main_debug;
extern int sns_recv_debug;
extern int sns_rsync_debug;
extern int sns_scn_debug;
extern int sns_comm_debug;
extern int isns_db_debug;
extern int isns_list_debug;
extern int isns_parse_debug;
extern int sns_tcp_debug;
extern int isns_query_debug;
extern int isns_reg_debug;
extern int isns_dereg_debug;
extern int isns_esi_debug;

int pauseFlag = FALSE;
int snsServiceFlag = FALSE;

extern void SNSReqTimeoutHdlr(void);


HANDLE init_ipc_rcv (void);
void DebugOn (char *);

void
taskDelay (int time)
{
   sleep (time);
}

int
sysClkRateGet (void)
{
   return (1);
}

void
taskDelete (pthread_t task)
{
   if (task == 0)
      return;

   if ((void *)task != NULL)
   {
     pthread_cancel (task);
   }
}

void
taskSuspend (void)
{
   return;
}

int
taskIdSelf (void)
{
   return (0);
}

/* Linux */
int
LinuxTaskSpawn (char *taskName, void *funcPtr)
{
  int result;
  pthread_t athread;
  result =  (pthread_create (&athread, NULL, (void*) funcPtr, NULL));
  if (result != 0)
    perror ("taskSpawn");
  return (result);
}

char Usage[] = "\
Usage: isnss [options] -i IP -b BROADCAST IP\n\
NOTES: IP-Addresses must be in IP dotted notation.\n\
Options:\n\
   [-a addr]   IP-Address for SNSP traffic.\n\
   [-b addr]   Broadcast Address for SNSP traffic.\n\
   [-p port]   primary port for SNSP traffic.\n\
   [-d switch] debug options: parse,db,list,reg,fsm,esi,scn,all \n\
   [-f]        Foreground - no daemon.\n\
   [-?]        Help.\n";


int
main (int argc, char **argv)
{
   int rc;
   pid_t pid;
   int c;
   char sfilename[256] = { 0 };
   ISNS_Entity role = ISNS_SECONDARY_SERVER;

   /* Don't forget to init the rest */
   while (optind != argc)
   {
      c = getopt (argc, argv, "fa:b:p:c:d:");

      switch (c)
      {
      case EOF:
         optarg = argv[optind];
         optind++;
         break;

      case 'a':
         strcpy (snsp_ip, optarg);
         break;

      case 'b':
         strcpy (snsp_bip, optarg);
         break;

      case 'c':
         strcpy (sfilename, optarg);
         break;

      case 'p':
         sns_comm_main_port_set = atoi (optarg);
         break;

      case 'd':
         DebugOn(optarg);
         break;
      case 'f':
         daemon_state = FALSE;
         break;
      case '?':
      default:
         fprintf(stderr,Usage);
         exit (0);
      }
   }

   rc = mkdir("/var/isns",0777);
   rc = chdir("/var/isns");

   if (daemon_state == TRUE)
   {
     pid = fork();
     if (pid < 0)
     {
        __LOG_ERROR("starting daemon failed\n");
        exit(1);
     }
     else if (pid)
     {
        exit(0);
     }
     else
     {
       int fd_pid;
       char buf[64];
       char *pid_file = "/var/run/isnss.pid";
       int rc;

       fd_pid = open(pid_file, O_WRONLY | O_CREAT, 0644);

       if (lockf(fd_pid, F_TLOCK, 0) < 0)
       {
          __LOG_ERROR("unable to lock pid file already is use\n");
          __LOG_INFO("isnss daemon not started\n");
          exit (1);
       }

       rc = ftruncate(fd_pid, 0);
       sprintf(buf,"%d\n",getpid());
       rc = write(fd_pid, buf, strlen(buf));
       __LOG_INFO ("isns daemon started as pid:%i",getpid());

       setsid();
     }
   }

   SNSMain (role);

   /* Never should hit this. */

   exit (-1);
}


/*********************************************************
  SNSReqTimeoutThread
*********************************************************/
int
SNSReqTimeoutThread ()
{
   struct timeval now;
   struct timespec timeout;
   int rc;
    
   __DEBUG ( isns_main_debug & 16,"SNSReqTimeoutHdlr Started.");

   while (1)
   {
    pthread_mutex_lock(&sns_request_timer);
    gettimeofday(&now, NULL);
    timeout.tv_sec = now.tv_sec + sns_request_timeout;
    timeout.tv_nsec = now.tv_usec * 1000;
    rc = pthread_cond_timedwait (&request_cond,&sns_request_timer,&timeout);
        if (rc != ETIMEDOUT)
        perror ("Error in Timeout Request in SNSReqTimeoutThread");

    pthread_mutex_unlock(&sns_request_timer);
    SNSReqTimeoutHdlr ();

      if (pauseFlag)
    return (0);
   }

   return (0);
}

/***********************************************************
SSNSFSMTimeoutThread
***********************************************************/
/* Linux */

int
SNSFSMTimeoutThread ()
{
   struct timeval now;
   struct timespec timeout;
   int rc;
    
   __DEBUG (isns_main_debug & 1666666,"SNSFSMTimeoutHdlr thread started.");
   
   while (1)
   {
      pthread_mutex_lock(&sns_fsm_timer);
      gettimeofday(&now, NULL);
        
      timeout.tv_sec = now.tv_sec + sns_heartbeat_interval;
      timeout.tv_nsec = now.tv_usec * 1000;

      rc = pthread_cond_timedwait(&fsm_cond, &sns_fsm_timer, &timeout);
      if (rc != ETIMEDOUT)
        perror ("Error in Timeout Request in SNSFSMTimeoutThread");
        
      SNSFSMTimeoutHdlr ();

      pthread_mutex_unlock(&sns_fsm_timer);

      if (pauseFlag)
         return (0);
   }

   return (0);
}

/*********************************************************
SNSESITimeoutThread
*********************************************************/

/* Linux */
int
SNSESITimeoutThread ()
{
   struct timeval now;
   struct timespec timeout;
   int rc;
   
   __DEBUG (isns_main_debug & 16,"SNSESITimeoutHdlr thread started.");
   
   while (1)
   {
      pthread_mutex_lock(&sns_esi_timer);
      gettimeofday(&now, NULL);
      timeout.tv_sec = now.tv_sec + sns_esi_interval;
      timeout.tv_nsec = now.tv_usec * 1000;
      rc = pthread_cond_timedwait(&esi_cond, &sns_esi_timer, &timeout);
      if (rc != ETIMEDOUT)
        perror ("Error in Timeout Request in SNSESITimeoutThread");

      SNSESITimeoutHdlr ();
      pthread_mutex_unlock(&sns_esi_timer);
      if (pauseFlag)
       return (0);
   }
   return (0);
}

/***********************************************************
SNSResyncTimeoutThread
***********************************************************/
int
SNSResyncTimeoutThread ()
{
   struct timeval now;
   struct timespec timeout;
   int rc;

   while (1)
   {
      pthread_mutex_lock(&sns_resync_timer);
      gettimeofday(&now, NULL);
      timeout.tv_sec = now.tv_sec + 60; //sns_resync_interval;
      timeout.tv_nsec = now.tv_usec * 1000;
      rc = pthread_cond_timedwait(&resync_cond, &sns_resync_timer, &timeout);
      if (rc != ETIMEDOUT)
    perror ("Error in Timeout Request in SNSResyncTimeoutThread");

      pthread_mutex_unlock(&sns_resync_timer);
      if (pauseFlag)
    return (0);
   }

   return (0);
}

void
DebugOn (char * type)
{

   if (!strcmp(type,"parse"))
   {
     isns_parse_debug = -1;
     isns_main_debug = -1;
     sns_comm_debug = -1;
     sns_tcp_debug = -1;
   }
   else if (!strcmp(type,"tcp"))
   {
     sns_comm_debug = -1;
     sns_tcp_debug = -1;
   }
   else if (!strcmp(type,"db"))
   {
     isns_db_debug = -1;
   }
   else if (!strcmp(type,"list"))
   {
     isns_list_debug = -1;
   }
   else if (!strcmp(type,"query"))
   {
     isns_query_debug = -1;
   }
   else if (!strcmp(type,"reg"))
   {
     isns_reg_debug = -1;
     isns_dereg_debug = -1;
   }
   else if (!strcmp(type,"fsm"))
   {
     sns_fsm_debug = -1;
     sns_hb_debug = -1;
     sns_cb_debug = -1;
   }
   else if (!strcmp(type,"esi"))
   {
     isns_esi_debug = -1;
   }
   else if (!strcmp(type,"scn"))
   {
     sns_scn_debug = -1;
   }
   else if (!strcmp(type,"all"))
   {
     sns_bcast_debug = -1;
     sns_cb_debug = -1;
     sns_comm_debug = -1;
     isns_db_debug = -1;
     sns_fsm_debug = -1;
     sns_hb_debug = -1;
     isns_main_debug = -1;
     sns_recv_debug = -1;
     sns_rsync_debug = -1;
     sns_scn_debug = -1;
     isns_parse_debug = -1;
     sns_tcp_debug = -1;
     isns_list_debug = -1;
     isns_query_debug = -1;
     isns_reg_debug = -1;
     isns_dereg_debug = -1;
     isns_esi_debug = -1;
   }
}
