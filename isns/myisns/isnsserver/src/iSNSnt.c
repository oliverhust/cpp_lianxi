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

#include "iSNSnt.h"
#include "iSNS.h"
#include "iSNScomm.h"
#include "iSNSdb.h"
#include "iSNSmsg.h"
#include "iSNSqueue.h"
#include "iSNSfsm.h"
#include "iSNScomm.h"
#include "iSNSbuffer.h"
#include "iSNSipc.h"
#include "iSNSparse.h"
#include "getarg.h"

WSADATA g_WsaData;

extern int sns_ready;
extern int sns_local_priority;
extern int sns_local_version;

int pauseFlag = TRUE;
int snsServiceFlag = FALSE;
int loadFlag = FALSE;

void NTDebugOn (void);
void SNSReqTimeoutHdlr (void);

void
taskDelay (int time)
{
   Sleep (time);
}

int
sysClkRateGet (void)
{
   return (1);
}

#define _SECOND 10000000
LARGE_INTEGER l;
LARGE_INTEGER *
ConvertTime (int sec, LARGE_INTEGER * pl)
{
   __int64 qwDueTime;
   qwDueTime = -sec * _SECOND;

   /* Copy the relative time into a LARGE_INTEGER. */
   pl->LowPart = (DWORD) (qwDueTime & 0xFFFFFFFF);
   pl->HighPart = (LONG) (qwDueTime >> 32);
   return (pl);
}

int
wdStart (void *sns_request_timer, int next_timeout,
         void *nptr1, void *nptr2)
{
   int results;
   int timeout;
   timeout = next_timeout;
   if (timeout < 1)
      timeout = 1;
   results =
      SetWaitableTimer ((void *) sns_request_timer, ConvertTime (timeout, &l),
                        0, NULL, NULL, FALSE);
   return (!results);
}

void
wdCancel (void *timer)
{
   if (timer)
      CancelWaitableTimer ((void *) timer);
}

void
taskDelete (int task)
{
   if (task == 0)
      return;

   if ((void *)task != NULL)
   {
      TerminateThread ((void *)task, 0);
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

void *
NTtaskSpawn (char *taskName, void *funcPtr)
{
   void *result;
   DWORD junk;
   result = NULL;
   result = CreateThread (0, 0, funcPtr, &junk, 0, &junk);
   return (result);
}

DWORD WINAPI
NTtimertaskWrapper (LPVOID lparam)
{
   /* Run Task */
   return (0);
}
HANDLE init_ipc_rcv (void);

char Usage[] = "\
Usage: isnss [options] -i IP -b BROADCAST IP\n\
   NOTES: IP-Addresses must be in IP dotted notation.\n\
Options:\n\
   [-a addr]  IP-Address for SNSP traffic.\n\
   [-b addr]  Broadcast Address for SNSP traffic.\n\
   [-p port]  primary port for SNSP traffic.\n\
   [-d]       Full debug options on.\n\
   [-?]       Help.\n\
\n\
";

DWORD WINAPI TCP_RecvMain (LPVOID l);

extern int sns_comm_main_port_set;

int
main (int argc, char **argv)
{
   DWORD junk;
   int c;
   char sfilename[256] = { 0 };
   ISNS_Entity role = ISNS_SECONDARY_SERVER;

   memset (snsp_ip, 0, sizeof (snsp_ip));

   if (argc < 3)
   {
      __LOG_ERROR (Usage);
      exit (0);
   }

   /* Don't forget to init the rest */
   while (optind != argc)
   {
      c = getopt (argc, argv, "da:b:p:c:");

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
         NTDebugOn();
         break;

      case '?':
      default:
         __LOG_ERROR (Usage);
         exit (0);
      }
   }

   if (strlen (snsp_ip) == 0 )
   {
      __LOG_ERROR (Usage);
      exit (0);
   }


   pauseFlag = FALSE;

   while (pauseFlag)
      Sleep (10);

   if (WSAStartup (MAKEWORD (0x02, 0x00), &g_WsaData) == SOCKET_ERROR)
   {
      return (0);
   }

   CreateThread (0, 0, TCP_RecvMain, &junk, 0, &junk);
   SNSMain (role);
 
   /* Never should hit this. */
   WSACleanup ();

   exit (-1);
}

extern void *sns_request_timer;
DWORD WINAPI
SNSReqTimeoutThread (LPVOID lparam)
{
   DWORD results;
   __LOG_INFO ("SNSReqTimeoutHdlr Started.\n");
   while (1)
   {
      results = WaitForSingleObject ((HANDLE)sns_request_timer, INFINITE);
      SNSReqTimeoutHdlr ();

      if (pauseFlag)
         return (0);
   }

   return (0);
}

extern int sns_fsm_timer;
DWORD WINAPI
SNSFSMTimeoutThread (LPVOID lparam)
{
   DWORD results;
   __LOG_INFO ("SNSFSMTimeoutHdlr thread started.\n");
   while (1)
   {
      results = WaitForSingleObject ((HANDLE)sns_fsm_timer, INFINITE);
      SNSFSMTimeoutHdlr ();

      if (pauseFlag)
         return (0);
   }

   return (0);
}

extern int sns_esi_timer;
DWORD WINAPI
SNSESITimeoutThread (LPVOID lparam)
{
   DWORD results;
   __LOG_INFO ("SNSESITimeoutHdlr thread started.\n");
   while (1)
   {
      results = WaitForSingleObject ((HANDLE)sns_esi_timer, INFINITE);
      SNSESITimeoutHdlr ();

      if (pauseFlag)
         return (0);
   }

   return (0);
}

extern int sns_resync_timer;
DWORD WINAPI
SNSResyncTimeoutThread (LPVOID lparam)
{
   DWORD results;
   while (1)
   {
      results = WaitForSingleObject ((HANDLE)sns_resync_timer, INFINITE);

      if (pauseFlag)
         return (0);
   }

   return (0);
}

void
SNSTrap (void)
{
}
/***********************************************/

void
NTDebugOn (void)
{
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
}

int 
NTReadFromFile(int *lenPtr, void *ptr)
{
   HANDLE hFile; 
   int bResult;

   hFile = CreateFile("iSNS.DAT",        // open ONE.TXT 
          GENERIC_READ,                 // open for reading 
          0,                            // do not share 
          NULL,                         // no security 
          OPEN_EXISTING,                // existing file only 
          FILE_ATTRIBUTE_NORMAL,        // normal file 
          NULL);                        // no attr. template 

   if (hFile == INVALID_HANDLE_VALUE) 
   { 
      __LOG_ERROR ("iSNS: Unable to read data file.\n");
      CloseHandle(hFile);
      return(ERROR);
   }
 
   bResult = ReadFile(hFile, ptr, 1024*10, lenPtr, 
       NULL) ; 

   if (!bResult)
   {
      __LOG_ERROR("iSNS: Unable to read data file.\n");
      CloseHandle(hFile);
      return(ERROR);
   }

   CloseHandle(hFile);
   return(SUCCESS);
}

void 
NTWriteToFile(int len, void *ptr)
{
   HANDLE hFile; 
   int bResult;
   int nBytesWrote;

   hFile = CreateFile("iSNS.DAT",              // create MYFILE.TXT 
                GENERIC_WRITE,                // open for writing 
                0,                            // do not share 
                0,                            // no security 
                CREATE_ALWAYS,                   // overwrite existing 
                0,                            // asynchronous I/O 
                0);                           // no attr. template 

   if (hFile == INVALID_HANDLE_VALUE) 
   { 
      __LOG_INFO ("iSNS: Unable to write data file.\n");
      CloseHandle(hFile);
      return;
   }
 
   // attempt an asynchronous read operation 
   bResult = WriteFile(hFile, ptr, len, &nBytesWrote, 
       NULL) ; 

   if (!bResult)
   {
      __LOG_ERROR ("iSNS: Unable to write data file.\n");
      CloseHandle(hFile);
      return;
   }

   if (nBytesWrote!=len)
   {
      __LOG_ERROR ("iSNS: Error writing data file.\n");
      CloseHandle(hFile);
      return;
   }

   CloseHandle(hFile);
   return;
}
