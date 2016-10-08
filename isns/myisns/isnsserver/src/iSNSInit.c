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


/*
 * This file contains source code for initializing 
 * iSNS task.
 *
 */

#include "iSNS.h"
#include "iSNScomm.h"
#include "iSNSdb.h"
#include "iSNSmsg.h"
#include "iSNSqueue.h"
#include "iSNSfsm.h"
#include "iSNSipc.h"
#include "iSNSbuffer.h"
#include "iSNSreg.h"
#include "iSNSdebug.h"

#ifndef SNS_LINUX
HANDLE trcv_pid;
#endif

char sns_errmsg[256];

STATUS
SNSInit (ISNS_Entity sns_role)
{

    /*
     * The Process ID of the UDP receive task.
     */

     if (CreateIPCEndPoint(SNS_EP) == ERROR)
     {
        sprintf (sns_errmsg, "%s:%d - Error creating SNS_EP IPC endpoint\n",
                 __FILE__,__LINE__);
        __LOG_ERROR("Error creating SNS_EP IPC endpoint");
        taskDelete(taskIdSelf());
     }
    /*
     * Create a message queue for sending messages
     * to other software entities in the switch.
     */
     if (CreateIPCEndPoint(DEVICE_MGMT_EP) == ERROR)
     {
        __LOG_ERROR("Error creating DEVICE_MGMT_EP IPC endpoint");
        taskDelete(taskIdSelf());
     }
     /*
      * Initialize database envirnoment and resources
      */
     if (ISNSdbOpen() == ERROR)
     {
         __LOG_ERROR("Error in opening iSNS DB file");
         taskDelete(taskIdSelf());
     }

     /*
      * Setup socket for talking to SoIP entities.
      */
      if (SNSCommInit(sns_role) == -1) 
      {
         __LOG_ERROR ("Error in Initialing socket communication");
         /*
          * clean up and bail out
          */
          ISNSdbClose();
          taskDelete(taskIdSelf());
      }

     /* 
      * Setup the queue for staging iSNS requests that require
      * a response
      */
      if (SNSQInit(SNS_TRANSACTION_QUEUE) == ERROR)
      {
          __LOG_ERROR("Error initing queue IPC msgs");
          ISNSdbClose();
          taskDelete(taskIdSelf());
      }

      /*
       * Start the procefor receiving UDP messages.
       *
       */
#ifdef SNS_LINUX
         if ((taskSpawn("tsns_rx", 67, 0, 8192, SNSReceiveMain,
                                  0, 0, 0, 0, 0, 0, 0, 0, 0, 0)) != 0)
#else
       if ((trcv_pid = (void *)taskSpawn("tsns_rx", 67, 0, 8192, SNSReceiveMain,
                                        0, 0, 0, 0, 0, 0, 0, 0, 0, 0)) == NULL)
#endif
       {
           __LOG_ERROR("Error spawning iSNS UDP receive task");
           ISNSdbClose();
           taskDelete(taskIdSelf());
       }


      /*
       * Start the heartbeat interval timer.
       */
       ISNSInitDBTables();
       Create_Default_DD();
       strcpy(isns_control_node, CONTROL_NODE);

       if ( SNSStartFSM() == ERROR )
       {
           __LOG_ERROR("Unable to Start iSNS FSM");
           ISNSdbClose();
           taskDelete(taskIdSelf());
       }

       return (OK);
}
