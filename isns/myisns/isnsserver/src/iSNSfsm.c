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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/netdb.h>
#include <sys/basetype.h>
#include <sys/error.h>
#include <sys/l3vpn.h>
#include <sys/dns.h>
#include <sys/list.h>
#include <sys/assert.h>
#include <sys/epoll.h>
#include <sys/in.h>

#include "../../include/iscsi_com.h"
#include "../../include/iscsi_event.h"
#include "../../include/iscsi_util.h"

#include "../../include/iscsi_basetype.h"
#include "../../include/iscsi_packet.h"
#include "../../include/iscsi_main.h"

#include "iSNStypes.h"
#include "iSNSfsm.h"
#include "iSNS.h"
#include "iSNSdb.h"
#include "iSNScomm.h"
#include "iSNSqueue.h"
#include "iSNSbuffer.h"
#include "iSNSipc.h"
#include "iSNStbl.h"
#include "iSNSdebug.h"

/* Globals */
extern int sns_fsm_debug;   /* state machine debugging */
extern int sns_bcast_debug;     /* broadcast debugging (except hb, scn) */
extern int sns_hb_debug;
extern int sns_comm_support;

/*
 * multicast outgoing message common buffer
 */
static ISNS_Msg mc_msg;

/*
  User Callback Routine when ever
  iSNS Readyness(sic) changes
*/
static void *notifyUser = 0;

static int firstTime = 1;

#if 0
void  
SNSFSMTimeoutHdlr(void);
void 
SNSESIStartTimer(int timeout);
#endif

static void 
SNSFSMStartTimer(int timeout);


static void
SNSProcessClaim(ISNS_Msg_Descp *);

static void
SNSSendClaim(void);

static void
SNSSendDiscovery(void);

static void
SNSSendCommReq(void);

static void
SNSSendHeartbeat(void);

void
SNSResyncTimeoutHdlr(void);

static void
SNSInitiateTrans2Primary(void);

static void
SNSInvalidState(void);

void
SNSReady();

void
ISNSNotReady();

/*
 * iSNS Server FSM
 */
struct SNS_fsm sns_server_fsm[SNS_NUM_FSM_STATES][SNS_NUM_FSM_EVENTS] = {


/* SNS_INIT */       { {SNS_WFCOMM,   NULL       }, 
                       {SNS_INIT,     NULL                      }, 
                       {SNS_INIT,     NULL                      },
                       {SNS_INIT,     NULL                      },
                       {SNS_INIT,     NULL                      },
                       {SNS_WFDACK,   SNSSendDiscovery          },
                       {SNS_INIT,     NULL                      },
                       {SNS_INIT,     NULL                      },
                       {SNS_INIT,     NULL                      },
                       {SNS_INIT,     NULL                      },
                       {SNS_INIT,     NULL                      },
                       {SNS_INIT,     NULL                      } },


/* SNS_WFHB */       { {SNS_WFHB,     NULL       },
                       {SNS_WFHB,     NULL                      },
                       {SNS_WFHB,     NULL                      },
                       {SNS_ELECTION, SNSProcessClaim           },
                       {SNS_WFHB,     NULL                      },
                       {SNS_WFDACK,   SNSSendDiscovery          },
                       {SNS_WFCOMM,   SNSSendCommReq            },
                       {SNS_WFHB,     NULL                      },
                       {SNS_WFHB,     NULL                      },
                       {SNS_WFHB,     NULL                      },
                       {SNS_WFHB,     NULL                      },
                       {SNS_WFHB,     NULL                      } },


/* SNS_WFCOMM */     { {SNS_WFCOMM,   NULL       },
                       {SNS_WFCOMM,   NULL                      },
                       {SNS_WFHB,     NULL            },
                       {SNS_WFCOMM,   NULL                      },
                       {SNS_WFCOMM,   NULL                      },
                       {SNS_WFDACK,   SNSSendDiscovery          },
                       {SNS_WFCOMM,   NULL                      },
                       {SNS_WFCOMM,   NULL                      },
                       {SNS_WFCOMM,   NULL                      },
                       {SNS_WFCOMM,   NULL                      },
                       {SNS_WFCOMM,   NULL                      },
                       {SNS_WFCOMM,   NULL                      } },


/* SNS_WFDACK */     { {SNS_WFCOMM,   NULL       },
                       {SNS_WFDACK,   NULL                      },
                       {SNS_WFDACK,   NULL                      },
                       {SNS_ELECTION, SNSProcessClaim           },
                       {SNS_WFDACK,   NULL                      },
                       {SNS_ELECTION, SNSSendClaim              }, 
                       {SNS_WFDACK,   NULL                      },
                       {SNS_WFDACK,   NULL                      },
                       {SNS_WFDACK,   NULL                      },
                       {SNS_WFDACK,   NULL                      },
                       {SNS_WFDACK,   NULL                      },
                       {SNS_WFDACK,   NULL                      } },



/* SNS_ELECTION */   { {SNS_WFCOMM,   NULL       },
                       {SNS_ELECTION, NULL                      },
                       {SNS_ELECTION, NULL                      },
                       {SNS_ELECTION, SNSProcessClaim           },
                       {SNS_ELECTION, NULL                      },
                       {SNS_PRIMARY,  SNSInitiateTrans2Primary  },
                       {SNS_ELECTION, NULL                      },
                       {SNS_ELECTION, NULL                      },
                       {SNS_ELECTION, NULL                      },
                       {SNS_ELECTION, NULL                      },
                       {SNS_ELECTION, NULL                      },
                       {SNS_ELECTION, NULL                      } },



/* SNS_PRIMARY  */   { {SNS_PRIMARY,  NULL       },
                       {SNS_PRIMARY,  NULL            },
                       {SNS_PRIMARY,  NULL                      },
                       {SNS_PRIMARY,  SNSSendHeartbeat          },
                       {SNS_PRIMARY,  SNSSendHeartbeat          },
                       {SNS_PRIMARY,  SNSSendHeartbeat          }, 
                       {SNS_WFCOMM,   NULL                      },
                       {SNS_PRIMARY,  NULL                      },
                       {SNS_PRIMARY,  NULL                      },
                       {SNS_PRIMARY,  NULL                      },
                       {SNS_PRIMARY,  NULL                      },
                       {SNS_PRIMARY,  NULL                      } },



/* SNS_FSM_INVALID */{ {SNS_FSM_INVALID, SNSInvalidState              },
                       {SNS_FSM_INVALID, SNSInvalidState              },
                       {SNS_FSM_INVALID, SNSInvalidState              },
                       {SNS_FSM_INVALID, SNSInvalidState              },
                       {SNS_FSM_INVALID, SNSInvalidState              },
                       {SNS_FSM_INVALID, SNSInvalidState              },
                       {SNS_FSM_INVALID, SNSInvalidState              },
                       {SNS_FSM_INVALID, SNSInvalidState              },
                       {SNS_FSM_INVALID, SNSInvalidState              },
                       {SNS_FSM_INVALID, SNSInvalidState              },
                       {SNS_FSM_INVALID, SNSInvalidState              },
                       {SNS_FSM_INVALID, SNSInvalidState              } }

};

/* string representation of the states */
char sns_state_string[SNS_NUM_FSM_STATES][16] = 
{ {"SNS_INIT"}, {"SNS_WFHB"}, {"SNS_WFCOMM"}, {"SNS_WFDACK"}, 
  {"SNS_ELECTION"}, {"SNS_PRIMARY"}, {"SNS_FSM_INVALID"} };


extern int sns_esi_interval;
int sns_heartbeat_interval = SNS_HEARTBEAT_INTERVAL;
int sns_heartbeat_timeout  = SNS_HEARTBEAT_WAITTIME;
int sns_heartbeat_counter  = 0;
int sns_discovery_counter  = 0;

/*
 * current state of the iSNS state machine
 */
int        sns_fsm_tree_wait = TRUE;
int        sns_ready = FALSE;
SNS_State  sns_state;

int        sns_primary_version = 0;
int        sns_primary_priority = 0;
int        sns_local_version = SNS_VERSION;
int        sns_local_priority = 5;

/*
 * persistant but scope-limited variables (static)
 */
static ISNS_Entity sns_init_role;

/*
 * Timers for scheduling FSM events and resync events
 */
int      sns_fsm_timeout = 0;


pthread_mutex_t sns_fsm_timer;
pthread_mutex_t sns_resync_timer;
pthread_mutex_t sns_esi_timer;
/* Create AND initialize the conditions */
pthread_cond_t fsm_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t resync_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t esi_cond = PTHREAD_COND_INITIALIZER;
int fsmtimeout_pid = 0;
int esitimeout_pid = 0;
int sns_resync_pid = 0;




/*
 * The Process ID of the Resync task.
 */
DWORD tresync_id;
HANDLE tresync_pid=0;

/*
 * List of ip addresses (sockets, really) during resync
 */
ISNS_ATTR_VALS_CB    sync_ip_list = {0};

int
SNSStartFSM(void)
{
    sns_state = SNS_INIT;
    sns_init_role = isns_role;

   /*
    * Let them know we are still in setup.
    * Delay to let communication interface start up
    */
    ISNSNotReady();
    __DEBUG (sns_fsm_debug & 4, (iSNS starting...));
//sns_ready = TRUE;
//isns_role = ISNS_PRIMARY_SERVER;

   /*
    * Initialize the queue for staging FSM messages
    * that require a response
    */
    if (SNSQInit(SNS_FSM_QUEUE) == ERROR)
       return (ERROR);

   /*
    * Activate timeout handlers.
    */
    {
        pthread_t junk;
        
        if (0 != pthread_create (&junk, NULL, (void*)SNSFSMTimeoutThread, NULL))
        perror ("Error creating thread for FSM Timeout");
        
        if (0 != pthread_create (&junk, NULL, (void*)SNSESITimeoutThread, NULL))
        perror ("Error creating thread for ESI Timeout");
        
        if (0 != pthread_create (&junk, NULL, (void*)SNSResyncTimeoutThread, NULL))
        perror ("Error creating thread for iSNS Resync");
     }



   SNSFSMStartTimer(sns_heartbeat_interval * sysClkRateGet());
   SNSESIStartTimer(sns_esi_interval * sysClkRateGet());

   return(SUCCESS);
}



void
SNSProcFSM(SNS_FSM_Event event, void *arg)
{
      SNS_State sns_prev_state;
      char dot_not_local[INET_ADDR_LEN];

      if (event > SNS_NUM_FSM_EVENTS)
         return;

      sns_prev_state = sns_state;
       if (sns_server_fsm[sns_state][event].action != NULL)
       {
          sns_server_fsm[sns_state][event].action(arg);
          sns_state = sns_server_fsm[sns_state][event].next_state;
       }
       if (sns_state == SNS_PRIMARY)
          isns_role = ISNS_PRIMARY_SERVER;
       else
          isns_role = sns_init_role;

      if (sns_prev_state != sns_state)
      {
         inet_ntoa_b(*(struct in_addr *)SNSGetLocalAddr(), dot_not_local);
         __DEBUG(sns_fsm_debug & 2, (%s - state change %s -> %s), 
                 dot_not_local, sns_state_string[sns_prev_state], 
                 sns_state_string[sns_state]);
      }
}


void
SNSESIStartTimer (int timeout)
{
return;
}

/*
 * Function Name:    SNSFSMStartTimer 
 *
 * Synopsis:         void SNSFSMStartTimer(int timeout);
 *
 * Description:      Arm the heartbeat timer.
 *
 */
static void
SNSFSMStartTimer (int timeout)
{
return;
}

void
SNSFSMTimeoutHdlr(void)
{
     char   fsm_timeout_md[sizeof(SNS_Msg_Cb) + sizeof(SNS_Msg_Hdr)];
     STATUS status = 0;

     sns_fsm_timeout = 1;
     bzero ((char *) fsm_timeout_md, sizeof (fsm_timeout_md));

     /*
      * Send a FSM timeout message to the iSNS core task
     */
     ((ISNS_Msg_Descp *)fsm_timeout_md)->msg.hdr.type = 
                       FSM_TIMEOUT_EVENT;
     ((ISNS_Msg_Descp *)fsm_timeout_md)->msg.hdr.msg_len = 0;
 
     status = SendIPCMessage (SNS_EP, (void *)fsm_timeout_md, 
                     sizeof(fsm_timeout_md), IPC_NO_WAIT);
}

void
SNSESITimeoutHdlr(void)
{
   char   esi_timeout_md[sizeof(SNS_Msg_Cb) + sizeof(SNS_Msg_Hdr)];
   STATUS status = 0;

    bzero ((char *) esi_timeout_md, sizeof (esi_timeout_md));
    /*
     * Send a ESI timeout message to the iSNS core task
     */
     ((ISNS_Msg_Descp *)esi_timeout_md)->msg.hdr.type = 
                       ESI_TIMEOUT_EVENT;
     ((ISNS_Msg_Descp *)esi_timeout_md)->msg.hdr.msg_len = 0;
 
     status = SendIPCMessage (SNS_EP, (void *)esi_timeout_md, 
                     sizeof(esi_timeout_md), IPC_NO_WAIT);     
}

static void
SNSProcessClaim(ISNS_Msg_Descp *p_md)
{
  SNSFSMStartTimer(sns_heartbeat_timeout * sysClkRateGet());
}


static void
SNSSendClaim(void)
{
  /*
   * start the heartbeat timer
   */
   SNSFSMStartTimer(sns_heartbeat_interval * sysClkRateGet());
}

static void
SNSSendDiscovery(void)
{
   ISNSNotReady();

  /*
   * start the heartbeat timer
   */
   SNSFSMStartTimer(sns_heartbeat_interval * sysClkRateGet());

   sns_discovery_counter++;
}

static void SNSSendCommReq(void)
{
   SNSFSMStartTimer(sns_heartbeat_timeout * sysClkRateGet());
}

extern int sns_comm_main_port;

static void
SNSSendHeartbeat(void)
{
   int ip;
   char dot_not_local[INET_ADDR_LEN];
   char dot_not_bcast[INET_ADDR_LEN];

   bzero ((char *)&mc_msg, sizeof(SNS_Msg_Hdr) + sizeof(SNS_Hb));

   mc_msg.hdr.type = ISNS_HEART_BEAT;
   memset(mc_msg.payload.heartbeat.ip_addr.v, 0, sizeof(mc_msg.payload.heartbeat.ip_addr.v));
   ip=*(uint32_t *)SNSGetLocalAddr();
   *(uint32_t *)&mc_msg.payload.heartbeat.ip_addr.v[12]=ip;
   mc_msg.payload.heartbeat.udp_port = sns_comm_main_port;
   mc_msg.payload.heartbeat.tcp_port = sns_comm_main_port;
   mc_msg.payload.heartbeat.interval = sns_heartbeat_interval;
   mc_msg.payload.heartbeat.counter  = sns_heartbeat_counter;
   mc_msg.hdr.msg_len = sizeof (SNS_Hb);

   inet_ntoa_b(*(struct in_addr *)SNSGetLocalAddr(), dot_not_local);
   inet_ntoa_b(*(struct in_addr *)SNSGetMCgrpAddr(), dot_not_bcast);
   __DEBUG(sns_hb_debug & 1, (%s - HEARTBEAT broadcast to %s. %d), 
           dot_not_local, dot_not_bcast, sns_heartbeat_counter);

   sns_heartbeat_counter++;
   SNSSendMsg2MCgrp (&mc_msg);

   SNSFSMStartTimer(sns_heartbeat_interval * sysClkRateGet());
   
}

static void
SNSInitiateTrans2Primary(void)
{
   char dot_not_local[INET_ADDR_LEN];
   char trapstr[48];

   sns_primary_version = sns_local_version;
   sns_primary_priority = sns_local_priority;
   sns_heartbeat_counter = 0;
   /* now primary, so set server_sock to own (local) address */
   ISNSCommUpdate(SNSGetLocalAddr ());

   /* get that heartbeat going out */
   SNSFSMStartTimer(sns_heartbeat_interval * sysClkRateGet());

   if (sns_ready == FALSE)
      SNSReady();

   /* delete any outstanding resync task */
   if (tresync_pid)
      taskDelete((pthread_t)tresync_pid);

   inet_ntoa_b(*(struct in_addr *)SNSGetLocalAddr(), dot_not_local);
   sprintf(trapstr, "%s elected primary\n", dot_not_local);
}

static void
SNSInvalidState (void)
{
  taskSuspend();
}

void SNSSetNotifyUser(void *ptr) 
{
  notifyUser = ptr;
}

void
SNSReady()
{
  sns_ready = TRUE;

  __DEBUG (sns_fsm_debug & 1, (iSNS ready));
  if (notifyUser == 0)
    return;

  if (firstTime == 1) {
    (*(void (*)())notifyUser)(SNS_FIRST_UP);
    firstTime = 0;
  } else {
    (*(void (*)())notifyUser)(SNS_UP);
  }
}

void
ISNSNotReady()
{
   if (sns_ready)
   {
     sns_ready = FALSE;
     __DEBUG (sns_fsm_debug & 1, (iSNS not ready));

     if (notifyUser)
       (*(void (*)())notifyUser)(SNS_DOWN);
   }
}
