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
 * This file contains IPC message service source code.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/basetype.h>
#include <sys/error.h>
#include <sys/list.h>
#include <sys/epoll.h>
#include "iSNStypes.h"
#include "iSNSmsg.h"
#include "iSNSipc.h"
#include "iSNSdebug.h"
#include "iSNSparse.h"

extern void SNSProcessRequest (ISNS_Msg_Descp *);

/*
 * IPC endpoints used by the switch software entities
 */
IPC_EP  ipc_ep[NUM_IPC_EP_NAMES+1];

extern ISNS_Msg_Descp main_md;

MSG_Q_ID  SNSDevMgtQueue;
MSG_Q_ID  SNSEpQueue;

/*
 * Function Name: CreateIPCEndPoint
 *
 * Synopsis:      int CreateIPCEndPoint (ep_name)
 *
 * Arguments:     ep_name - End point name
 *
 * Description:   create a message queue and bind it to
 *                the named entity.
 *
 * Return value:  0 (0) or -1 (-1)
 *
 */
int
CreateIPCEndPoint (IPC_EP_Name ep_name)
{
   int num_buffers;
   struct sockaddr_in my_addr;

   if (!(VALID_EP_NAME (ep_name)))
      return (ERROR);

   num_buffers = ep_name==DEVICE_MGMT_EP?MAX_DEVICE_MGMT_EP_BUFFERS:MAX_SNS_EP_BUFFERS;
   my_addr.sin_family = AF_INET;
   my_addr.sin_port = htons ((short) (ep_name==DEVICE_MGMT_EP?6005:6006));
   my_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

   ipc_ep[ep_name].their_addr.sin_family = AF_INET;
   ipc_ep[ep_name].their_addr.sin_port = htons ((short) (ep_name==DEVICE_MGMT_EP?6005:6006));
   ipc_ep[ep_name].their_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

   ipc_ep[ep_name].s = socket(AF_INET, SOCK_DGRAM, 0);

    if (ipc_ep[ep_name].s < 0)
    {
       __LOG_ERROR ("Error: While calling socket.");
       return (ERROR);
    }

   if (bind ( ipc_ep[ep_name].s, (struct sockaddr *) &my_addr, 
              sizeof (my_addr)) < 0)
   {
      __LOG_ERROR ("Fatal Error while Binding.");
      return (0);
   }
      return(SUCCESS);
}



/*
 * Function Name: SendIPCMessage
 *
 * Synopsis:      int SendIPCMessage(ep_name, p_msg, size, pri)
 *
 * Arguments:     ep_name - End point name
 *                p_msg   - pointer to outbound message
 *                size    - message size in bytes
 *                pri     - message priority
 *
 * Description:   Send a message to a switch software entity.
 *
 * Return value:  0 (0) or -1 (-1)
 *
 */
int
SendIPCMessage (IPC_EP_Name ep_name, void *p_msg, int msg_len, int options)
{
   int     cnt;

   if (!(VALID_EP_NAME (ep_name)))
      return (ERROR);
  
   if (ipc_ep[ep_name].s == 0)
      return (ERROR);

   cnt = sendto(ipc_ep[ep_name].s, p_msg, msg_len, 0, 
      (struct sockaddr * )&ipc_ep[ep_name].their_addr, sizeof(ipc_ep[ep_name].their_addr));

   if (cnt < 0)
   {
      __LOG_ERROR ("Error: sendto.");
      //pthread_exit((void *)0);
   }
   return (SUCCESS); 
}



/*
 * Function Name: ReceiveIPCMessage
 *
 * Synopsis:      int ReceiveIPCMessage(ep_name, p_msg, max_len, timeout)
 *
 * Arguments:     ep_name - receive message from the named EP.
 *                p_msg   - place holder for received message
 *                max_len - maximum size of expected message
 *                timeout - timeout for message arrival
 *
 * Description:   Receive a message from a switch software entity.
 *
 * Return value:  SUCCESS (0) or ERROR (-1)
 *
 */
ULONG ReceiveIPCMessage (IN UINT uiEvent, IN VOID *pHandle)
{
   int cnt;
   int len;
   int iFd = (INT)(LONG)pHandle;

   if (0 == (uiEvent & (UINT)EPOLLIN))
   {
       return ERROR_FAILED;
   }

   len=sizeof(ipc_ep[SNS_EP].their_addr);
   cnt = recvfrom(iFd, &main_md, sizeof (ISNS_Msg_Descp), 0,
      (struct sockaddr * )&ipc_ep[SNS_EP].their_addr, (socklen_t *)&len);

   if (cnt < 0)
   {
      __LOG_ERROR ("Receive error.");
      pthread_exit((void *)0);
   }

   SNSConvertPayloadNTOH (&main_md);
   SNSProcessRequest (&main_md);

   return(SUCCESS);
}


/*
 * Function Name: DestroyIPCEndPoint
 *
 * Synopsis:      int DestroyIPCEndPoint(ep_name)
 *
 * Arguments:     ep_name - name of EP to remove.
 *
 * Description:   Release resources associated with an IPC endpoint.
 *
 * Return value:  0 (0) or -1 (-1)
 *
 */
int
DestroyIPCEndPoint (IPC_EP_Name ep_name)
{
   return (SUCCESS);
}
