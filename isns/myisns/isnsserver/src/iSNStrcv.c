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
#include <signal.h>
#include <sys/poll.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/basetype.h>
#include <sys/error.h>
#include <sys/epoll.h>
#include <sys/list.h>
#include <sys/in.h>
#include <sys/assert.h>
#include <sys/eventfd.h>

#include "../../include/iscsi_com.h"
#include "../../include/iscsi_util.h"
#include "../../include/iscsi_event.h"

#include "../../include/iscsi_basetype.h"
#include "../../include/iscsi_packet.h"
#include "../../include/iscsi_main.h"

#include "iSNStypes.h"
#include "iSNS.h"
#include "iSNSmsg.h"
#include "iSNSfsm.h"
#include "iSNScomm.h"
#include "iSNSipc.h"
#include "iSNSdebug.h"
#include "iSNSEpoll.h"
#include "iSNSevent.h"

#define SOCKET_TIMEOUT_VALUE 120*1000

pthread_mutex_t sns_status_mutex;
pthread_cond_t sns_status_cond = PTHREAD_COND_INITIALIZER;

int sns_status = DISABLE;
/******************************************/

int
TCP_RecvThread(LPVOID lparam);

typedef struct _tcp_cb
{
   SOCKET sock;
} TCP_CB;

#define MAX_BUFFER         (2048)

/******************************************/

extern int sns_ready;
extern int sns_comm_main_port;
int sns_tcp_debug=0;

ULONG iscsi_isns_RecvCreate(IN UINT uiEvent, IN VOID *pHandle);
ULONG ISCSI_ISNS_TrcvEpollCallBack(IN UINT uiEvent, IN VOID *pHandle);

/*****************************************************************************
    Func Name: iscsi_isns_RecvCreate
 Date Created: 2016-10-10
       Author: lichao@dian
  Description: 接收连接回调函数
        Input: IN UINT uiEvent
               IN VOID *pHandle
       Output:
       Return: ULONG
      Caution:
-----------------------------------------------------------------------------
   Modification History
   DATE        NAME             DESCRIPTION
-----------------------------------------------------------------------------

******************************************************************************/
ULONG iscsi_isns_RecvCreate(IN UINT uiEvent, IN VOID *pHandle)
{
    INT iRecvSock = ISCSI_FD_INIT;
    ULONG ulRet;
    socklen_t addrlen;
    INT iFd = (INT)(LONG)pHandle;
    struct sockaddr_in their_addr;

    if (0 == (uiEvent & (UINT)EPOLLIN))
    {
        return ERROR_FAILED;
    }

    /* 调用accept建立socket连接 */
    iRecvSock = accept(iFd, (struct sockaddr *) &their_addr,&addrlen);
    if (iRecvSock < 0)
    {
        return ERROR_FAILED;
    }

    ulRet = ISCSI_UTL_RecvFdAdd(ISCSI_ISNS_RecvFdListGet(), iRecvSock,
                                ISCSI_ISNS_TrcvEpollCallBack);
    if (ERROR_SUCCESS != ulRet)
    {
        close(iRecvSock);
    }

    return ulRet;
}

int
ISNS_RecvMain(void)

{
   SOCKET sock;
   int not_needed = 1;
   ISCSI_EPOLL_CALLBACK_PF pfEpollProcess = NULL;
   int i;
   int rc;
   int iEvNum;
   struct epoll_event astEvents[ISCSI_EPOLL_MAX_EVENT_NUM];
   struct sockaddr_in my_addr;

   __DEBUG(sns_tcp_debug & 1, (TCP_RecvMain: Starting));

   if(ERROR_SUCCESS != ISCSI_ISNS_EpollInit())
   {
        perror("Failed to Create Epoll");
   }

   my_addr.sin_family = AF_INET;
   my_addr.sin_port = htons ((short) sns_comm_main_port);
   my_addr.sin_addr.s_addr = INADDR_ANY;

   sock = socket(AF_INET, SOCK_STREAM, 0);

   /* Allow reuse of addresses */
   if (setsockopt (sock, SOL_SOCKET, SO_REUSEADDR, &not_needed, sizeof(not_needed)) < 0)
   {
      perror ("Reuse address");
      return (-1);
   }


   rc = bind (sock, (struct sockaddr *) &my_addr, sizeof (my_addr));
   if (rc < 0)
   {
    close (sock);
    {
           __LOG_ERROR ("Fatal Error while Binding:%i - %s.",rc,strerror(errno));
           return (0);
    }
   }

   rc = listen(sock,100);
   if (rc < 0)
   {
      __LOG_ERROR ("Fatal Error while doing a Listen:%i - %s",rc,strerror(errno));
      return (0);
   }

   /*创建保存接收数据的FD的队列*/
   ISCSI_UTL_FdListInit(ISCSI_ISNS_EpollHandleGet(), ISCSI_ISNS_RecvFdListGet());

   SNSReceiveMain();

   ISCSI_UTL_EpollAddFd(ISCSI_ISNS_EpollHandleGet(), sock, EPOLLIN, iscsi_isns_RecvCreate);
   for (;;)
   {
       if (sns_status == DISABLE)
       {
            pthread_mutex_lock(&sns_status_mutex);
        	rc = pthread_cond_wait (&sns_status_cond,&sns_status_mutex);
            if (rc != 0)
    		perror ("Error in disabele status");

	        pthread_mutex_unlock(&sns_status_mutex);

            continue;
       }

       memset(astEvents, 0, sizeof(astEvents));
       iEvNum = epoll_wait(ISCSI_ISNS_EpollHandleGet(), astEvents,
                           ISCSI_EPOLL_MAX_EVENT_NUM, SOCKET_TIMEOUT_VALUE);

       if ((iEvNum < 0) && (EINTR != errno))
       {
          perror("Failed to wait");
          break;
       }

       for (i = 0; i < iEvNum; i++)
       {
           pfEpollProcess = (ISCSI_EPOLL_CALLBACK_PF)(ULONG)(astEvents[i].callback);

           if(NULL != pfEpollProcess)
           {
                (VOID)pfEpollProcess(astEvents[i].events, astEvents[i].data.ptr);
           }
       }
   }

   ISCSI_ISNS_EpollDeInit();

   return(ERROR);
}

ULONG ISCSI_ISNS_TrcvEpollCallBack(IN UINT uiEvent, IN VOID *pHandle)
{
   int startIndex;
   int endIndex;
   int pending;
   int msg_size;
   int i;
   char buffer[MAX_BUFFER];
   SNS_Msg_Hdr *p_msg;
   ISNS_Msg_Descp md;
   int sockfd = (INT)(LONG)pHandle;

   startIndex=0;
   endIndex=0;

   if (0 != (uiEvent & EPOLLIN))
   {
       i = recv(sockfd, &buffer[endIndex], sizeof(buffer)-endIndex, 0);

       if (i == 0)
       {
          __DEBUG(sns_tcp_debug & 1, (recv no data - client closed connection));
          ISCSI_UTL_RecvFdRemove(ISCSI_ISNS_RecvFdListGet(), sockfd);
          close(sockfd);
          return ERROR_FAILED;
       }
       else if (i < 0)
       {
          __LOG_ERROR("recv error we will close connection:%i - %s",i,strerror(errno));
          ISCSI_UTL_RecvFdRemove(ISCSI_ISNS_RecvFdListGet(), sockfd);
          close(sockfd);
          return ERROR_FAILED;
       }

       endIndex+=i;
       pending=endIndex-startIndex;
       /* Parse Msg */

       __DEBUG(sns_tcp_debug &1,"recvsize:%i pending:%i",i,pending);

       while ( pending >= sizeof(SNS_Msg_Hdr) )
       {
          p_msg=(struct sns_msg_hdr *)&buffer[startIndex];
          msg_size=sizeof(SNS_Msg_Hdr)+ntohs(p_msg->msg_len);

          if (  pending < msg_size)
          {
             __DEBUG(sns_tcp_debug &1,"pending:%i is less than msg_size:%i",pending,msg_size);
             break;
             return ERROR_FAILED;
          }

          p_msg->type     =  ntohs (p_msg->type);
          p_msg->xid      =  ntohs (p_msg->xid);
          p_msg->msg_len  =  ntohs (p_msg->msg_len);
          p_msg->flags    =  ntohs (p_msg->flags);
          p_msg->version  =  ntohs (p_msg->version);
          p_msg->sequence =  ntohs (p_msg->sequence);

          md.cb.sender = ISNS_REMOTE;

          /* Store the type and socket */
          md.cb.sock.sockfd=sockfd;
          md.cb.sock.socketType=ISNS_SOCKET_TYPE_TCP;

          memcpy(&md.msg, p_msg, msg_size);

          __DEBUG(sns_tcp_debug &1, (ISNSPDU rcv size %d), msg_size);
          while (!sns_ready)
          {
             sleep(1);
          }
          __DEBUG(sns_tcp_debug &1,SendIPCMessage msg_len:%i,p_msg->msg_len);
          SendIPCMessage(SNS_EP, (void *)&md,
             sizeof(SNS_Msg_Cb) + sizeof(SNS_Msg_Hdr) + p_msg->msg_len, 0);

          pending-=msg_size;
          startIndex+=msg_size;
       }
       if (pending==0)
       {
          startIndex=0;
          endIndex=0;
       }
       else
       {
          __DEBUG(sns_tcp_debug &1,??? pending non-zero:%i,pending);
         /* copy buffer from StartIndex begining of buffer with size of pending */
         memcpy(buffer, &buffer[startIndex], pending);

         /* set endIndex=pending set startIndex = 0 */
         endIndex = pending;
         startIndex = 0;
       }
   }

   return ERROR_SUCCESS;
}

