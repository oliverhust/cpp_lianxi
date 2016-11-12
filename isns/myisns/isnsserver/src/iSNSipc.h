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
 * This file contains definitions of data structures, macros 
 * and function prototypes used to support a message queue
 * based IPC service
 *
 */

#ifndef _IPCmessage_h_
#define _IPCmessage_h_

#define MAX_SNS_EP_BUFFERS            256
#define MAX_DEVICE_MGMT_EP_BUFFERS    20


/*
 * End Point Names of switch software entities
 */
typedef enum {

    FIRST_EP_NAME = 0,

    DEVICE_MGMT_EP = FIRST_EP_NAME,
    SNS_EP,

    LAST_EP_NAME = SNS_EP,

    NUM_IPC_EP_NAMES = LAST_EP_NAME + 1

} IPC_EP_Name;

/*
 * Message queue based IPC endpoint used for communicating with
 * a local switch software entity.
 */
typedef struct ipc_ep {
   SOCKET s;
   struct sockaddr_in their_addr;
} IPC_EP;
    

#if 0
#define VALID_EP_NAME(x)  (((x < FIRST_EP_NAME) && (x > LAST_EP_NAME)) ? 0 : 1)
#endif

#define VALID_EP_NAME(x)   (1)

#define IPC_NO_WAIT        1
#define IPC_MSG_PRI_HI     2

/*-----------------------------------------------------------
 * Function prototypes
 *-----------------------------------------------------------
 */
extern int
SendIPCMessage (IPC_EP_Name, void *, int, int);

extern ULONG ReceiveIPCMessage (IN UINT uiEvent, IN VOID *pHandle);

extern int
CreateIPCEndPoint (IPC_EP_Name);

extern int
DestroyIPCEndPoint (IPC_EP_Name);

extern void
InitIPCEndPoints (void);

#endif

