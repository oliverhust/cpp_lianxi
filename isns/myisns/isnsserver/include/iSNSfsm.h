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

#ifndef _SNSfsm_h_
#define _SNSfsm_h_

#include <sys/types.h>

#include "iSNSmsg.h"

typedef enum {

    SNS_INIT, 
    SNS_WFHB,
    SNS_WFCOMM,
    SNS_WFDACK,
    SNS_ELECTION,  
    SNS_PRIMARY,            
    SNS_FSM_INVALID,
    SNS_NUM_FSM_STATES

} SNS_State;


typedef enum {

    SNS_HEARTBEAT_RCVD,
    SNS_COMM_REQ_RCVD,
    SNS_COMM_ACK_RCVD,
    SNS_CLAIM_RCVD,
    SNS_DISCOVERY_RCVD,
    SNS_HEARTBEAT_TIMEOUT,
    SNS_TRANS_TO_WFCOMM,
    SNS_RESYNC_START,
    SNS_RESYNC_START_RES,
    SNS_RESYNC_END,
    SNS_RESYNC_END_RES,
    SNS_SYNC_TIMEOUT,

    SNS_NUM_FSM_EVENTS

} SNS_FSM_Event;


struct SNS_fsm {

    SNS_State  next_state;
    void       (*action)();
};

extern int  
SNSStartFSM(void);
void  
SNSFSMTimeoutHdlr(void);
void 
SNSESIStartTimer(int timeout);

void SNSESITimeoutHdlr(void);

extern void 
SNSProcFSM(SNS_FSM_Event, void *);

extern void
SNSSetNotifyUser(void *ptr);

#endif
