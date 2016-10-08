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
 * This file contains definitions of data structures
 * and macros used for implementing queueing of 
 * SoIP service requests that require a response from
 * a peer SoIP service entity.
 *
 */

#ifndef _SNSqueue_h_
#define _SNSqueue_h_


#include "iSNSmsg.h"

/*
 * iSNS message descriptor queue names
 */
typedef enum {

    SNS_FSM_QUEUE,
    SNS_TRANSACTION_QUEUE,
    ISNS_CALLBACK_QUEUE,
    SNS_ZONE_ACCESS_QUEUE,
    SNS_PENDING_MSG_QUEUE,

    SNS_NUM_QUEUES

} SNS_Queue_Name;

/*
 * Queue descriptor for staging iSNS requests that are
 * awaiting a response from another SOIP service entity
 * This queue maintains a list of pointers to all iSNS
 * requests for which a response is outstanding.
 */
struct SNS_Msg_Descp_Q {

     ISNS_Msg_Descp  *head;
     ISNS_Msg_Descp  *tail;

};

/*
 * iSNS queue management routines
 */
extern int
SNSQInit (SNS_Queue_Name);

extern void
ISNSEnqReq (SNS_Queue_Name, ISNS_Msg_Descp *);

extern ISNS_Msg_Descp *
ISNSDeqReq (SNS_Queue_Name, uint16_t, int);

ISNS_Msg_Descp *
ISNSQHead (SNS_Queue_Name name);

ISNS_Msg_Descp *
SNSGetReq (SNS_Queue_Name name, int xid, int tid);

#endif

