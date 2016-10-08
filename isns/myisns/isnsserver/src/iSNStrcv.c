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

#include "iSNS.h"
#include "iSNSmsg.h"
#include "iSNSfsm.h"
#include "iSNScomm.h"
#include "iSNSipc.h"
#include "iSNSdebug.h"

#define SOCKET_TIMEOUT_VALUE 120*1000

/******************************************/

#ifdef SNS_LINUX
int
#else
DWORD WINAPI
#endif
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

#ifdef SNS_LINUX
int
TCP_RecvMain(void)
#else
DWORD WINAPI
TCP_RecvMain(LPVOID lp)
#endif
{
   SOCKET sock;
   SOCKET sockfd;
   int not_needed = 1;
   socklen_t addrlen;
   int rc;
   int threadcount = 0;
   pthread_attr_t attr;
   
   struct sockaddr_in my_addr, their_addr;

   __DEBUG(sns_tcp_debug & 1, (TCP_RecvMain: Starting));

   my_addr.sin_family = AF_INET;
   my_addr.sin_port = htons ((short) sns_comm_main_port);
   my_addr.sin_addr.s_addr = INADDR_ANY;

   sock = socket(AF_INET, SOCK_STREAM, 0);

#ifdef SNS_LINUX
   /* Allow reuse of addresses */
   if (setsockopt (sock, SOL_SOCKET, SO_REUSEADDR, &not_needed, sizeof(not_needed)) < 0)
   {
      perror ("Reuse address");
      return (-1);
   }
#endif
  
   rc = bind (sock, (struct sockaddr *) &my_addr, sizeof (my_addr));
   if (rc < 0)
   {
    close (sock);
    {
           __LOG_ERROR ("Fatal Error while Binding:%i - %s.",rc,strerror(errno));
           return (0);
    }
   }

   addrlen = sizeof (their_addr);

   rc = listen(sock,100);
   if (rc < 0)
   {
      __LOG_ERROR ("Fatal Error while doing a Listen:%i - %s",rc,strerror(errno));
      return (0);
   }

   while (1)
   {
      sockfd = accept (sock, (struct sockaddr *) &their_addr,&addrlen);
      if (sockfd < 0)
      {
         __LOG_ERROR ("Fatal Error while doing an Accept sockfd:%i - %s",sockfd,strerror(errno));
         break;
      }
      else
      {
#ifdef SNS_LINUX
         pthread_t ignore;
#else
         DWORD ignore;
#endif
         TCP_CB *cb_p;
         cb_p=malloc(sizeof(TCP_CB));
         cb_p->sock=sockfd;

         __DEBUG(sns_tcp_debug & 1,"accepted a TCP connection request thread count:%i",threadcount);

#ifdef SNS_LINUX
         pthread_attr_init(&attr);
         pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
     rc = pthread_create (&ignore, &attr, (void*) TCP_RecvThread, (LPVOID)cb_p);
         if (rc != 0)
         {
           __LOG_ERROR("Error creating thread:%i - %s",rc,strerror(rc));
         }
     threadcount++;
#else
         CreateThread(0, 0, TCP_RecvThread, (LPVOID)cb_p, 0, &ignore);
#endif
      }
   }

   __LOG_INFO("TCP_RecvMain: Finished.");
   return(ERROR);
}

/******************************************/
#ifdef SNS_LINUX
int
#else
DWORD WINAPI
#endif
TCP_RecvThread(LPVOID lparam)
{
   TCP_CB *cb_p=(TCP_CB *)lparam;
   SOCKET sockfd;
   char buffer[MAX_BUFFER];
   int startIndex;
   int endIndex;
   int pending;
   int msg_size;
   int i;
   struct pollfd ufds;
   int rc;

   SNS_Msg_Hdr *p_msg;
   ISNS_Msg_Descp md;

   __DEBUG(sns_tcp_debug & 1, (TCP_RecvThread: Starting a thread on socket:%i),cb_p->sock);

   sockfd=cb_p->sock;
   free(cb_p);

   startIndex=0;
   endIndex=0;
   while (1)
   {
      ufds.fd = sockfd;
      ufds.events = POLLIN;
      ufds.revents = 0;
      rc = poll(&ufds, 1, SOCKET_TIMEOUT_VALUE);
      if (rc == 0)
      {
         /* close connection if no activity */
         __DEBUG(sns_tcp_debug &1, (connection timed out and closed:%i),sockfd);
         close(sockfd);
         break;
      }
      else if (rc < 0)
      {
         __LOG_ERROR("Error on connection:%i - errno:%i %s",sockfd,errno,strerror(errno));
         close(sockfd);
         break;
      }

      i = recv(sockfd, &buffer[endIndex], sizeof(buffer)-endIndex, 0);

      if (i == 0)
      {
         __DEBUG(sns_tcp_debug & 1, (recv no data - client closed connection));
         close(sockfd);
         break;
      }
      else if (i < 0)
      {
         __LOG_ERROR("recv error we will close connection:%i - %s",i,strerror(errno));
         close(sockfd);
         break;
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

   __DEBUG(sns_tcp_debug & 1, (TCP_RecvThread: Socket:%i Exiting thread),sockfd);
   /* thread terminates here */
   pthread_exit(&rc);
   __LOG_ERROR("pthread_exit failed");
   return (SUCCESS);
}
