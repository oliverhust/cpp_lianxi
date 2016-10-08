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

#include "isns.h"

#ifndef SNS_LINUX
#include <io.h>
#include <winsock2.h>
#include <Ws2tcpip.h>
#else
#define Sleep(a) sleep(a)
#endif

#include <time.h>
#include "util.h"
#include "comm.h"
#include "parse.h"
#include <fcntl.h>
#include <errno.h>
#include <poll.h>
#include <netdb.h>

MSG_CB msg_q[MSG_Q_SIZE]; /* TCP msg Q */
int msgInitFlag = FALSE;
int msgCurrentIndex = 0;
int msgCurrentHead = 0;

#ifndef SNS_LINUX
WSADATA g_WsaData;
#endif

SOCKET fd;

struct sockaddr_in their_addr;
struct sockaddr_in my_addr;


int tcpFlag;   /* TRUE if using TCP */
int isns_port; /* iSNS port */

extern int parserFlag;
extern char *optarg;
extern int enableESIFlag;
extern char p_ip[256];
char multicast_addr[20];
struct addrinfo hints, *res;

/***********************************************************************/
/* Initializes communications */
/***********************************************************************/
int
InitComm( int hb_flag, /* Set to non-zero if using heartbeat to find iSNS */
          int l3_hb_flag,
          int t_flag  /* Set to non-zero if using TCP */)
{
   tcpFlag=t_flag;
   int flags = 0;
   int rc = 0;
   int timeout = 5;   // # seconds
   struct pollfd fds;
   char port[16];

#ifndef SNS_LINUX
   /* Start up the winsock proprietary Stuff */
   if (WSAStartup (MAKEWORD (0x02, 0x00), &g_WsaData) == SOCKET_ERROR)
   {
      exit(-1);
   }
#endif

   if (hb_flag)
   {
      if (-1==HeartBeatListener())
         exit(-1);
   }
   else if (l3_hb_flag)
   {
      if (-1==L3_HeartBeatListener())
          exit(-1);
   }

   memset(&hints,0,sizeof(struct addrinfo));
   hints.ai_family = AF_UNSPEC;
   hints.ai_socktype = SOCK_STREAM;
   sprintf(port,"%i",isns_port);
   rc = getaddrinfo(p_ip,port,&hints,&res);
   if (rc != 0)
   {
      printf("Failed to resolve host:%i %s\n",rc,strerror(errno));
      return rc;
   }

   if (tcpFlag)
   {
      /* TCP Mode */
      if ((fd = socket (AF_INET, SOCK_STREAM, 0)) < 0)
      {
         perror ("TCP Fatal Error while calling socket");
         exit(-1);
      }

      /* setup socket to nonblocking so we can do a timeout on the connect call */
      flags = fcntl(fd, F_GETFL,0);
      fcntl (fd, F_SETFL, flags | O_NONBLOCK);

      printf ("Connecting to %s...\n", p_ip);
      rc = connect (fd, res->ai_addr, res->ai_addrlen);
      if (rc < 0)
      {
        if (!(errno == EINTR || errno == EINPROGRESS || errno == EWOULDBLOCK))
        {
            printf ("TCP Fatal Error connecting: %s\n",strerror(errno));
            exit(-1);
        }

        /* because we have nonblocking connection, we have to poll to find out when */
        /* the connection actually occurs.  If it can't connect, then a timeout occurs */      
        fds.fd = fd;
        fds.events = 0XFFFF; //POLLOUT;
        for (;;) {
            fds.revents = 0;
            rc = poll(&fds, 1, timeout * 5000);
            if (rc < 0)
            {
               printf("errno is:%i %s\n",errno,strerror(errno));
               if (!(errno == EINTR || errno == EAGAIN))
                 break;
            }
            else if (rc == 0)
            {
                printf ("TCP Fatal Error Timeout occured trying to Connect\n");
                exit(-1);
            }
            else
               if (fds.revents & POLLERR)
         {
                   printf ("TCP error on connection\n");
                   exit(-1);
                 }
               printf("We have a connection:0x%x\n",fds.revents);
               break;
        }
      }

      /* set socket back to blocking */
      fcntl (fd, F_SETFL, flags );

//      printf("Spawn TCPReceiveMain\n");

      /* Spawn TCP Recv Thread*/
      {
#ifdef SNS_LINUX
         pthread_t junk;
         if (0 != pthread_create (&junk, NULL, (void*) (TCPReceiveMain) , NULL))
            printf ("\n\nThread Creation Failed!\n\n");
#else
         DWORD junk;
         CreateThread ((LPSECURITY_ATTRIBUTES)0, 0, (LPTHREAD_START_ROUTINE) TCPReceiveMain, NULL, 0, &junk);
         Sleep (10);
#endif
      }

   }
   else
   {
      /* UDP Mode */
      if ((fd = socket (AF_INET, SOCK_DGRAM, 0)) < 0)
      {
         perror ("UDP Fatal Error while calling socket");
         exit(-1);
      }

      rc = connect (fd, res->ai_addr, res->ai_addrlen);
      if (rc < 0)
      {
            printf ("UDP Fatal Error: Bind: %s\n",strerror(errno));
            exit(-1);
      }

      /* setup socket to nonblocking so we can do a timeout on the connect call */
      flags = fcntl(fd, F_GETFL,0);
      fcntl (fd, F_SETFL, flags | O_NONBLOCK);
   }
   //freeaddrinfo(res);
   return (0);
}

/***********************************************************************/
/* Used to store PDU which comes on a TCP session */
/***********************************************************************/
int
AddMsg (void *p_msg, int size)
{
   int i;
   if (FALSE == msgInitFlag)
   {
      for (i = 0; i < MSG_Q_SIZE; i++)
      {
         msg_q[i].p_msg = NULL;
      }

      msgCurrentIndex = 0;
      msgCurrentHead = 0;
      msgInitFlag = TRUE;
   }
   msg_q[msgCurrentIndex].p_msg = (void *)malloc (size);
   msg_q[msgCurrentIndex].size = size;
   memcpy (msg_q[msgCurrentIndex].p_msg, p_msg, size);

   msgCurrentIndex = (msgCurrentIndex + 1) % MSG_Q_SIZE;

   return (0);
}
/***********************************************************************/
/* Sends a msg to the iSNS without waiting for a response */
/***********************************************************************/
int
ISNSJustSendCmd (ISNS_CMD * cmd)
{
   int e;
   int len = cmd->hdr.len + sizeof (ISNS_HDR);

   cmd->hdr.len = htons (cmd->hdr.len);
   e = SendPDU (cmd, len);

   if (parserFlag)
      printf("PDU sent-->\n");
   DumpHex (cmd, e);
   if (e < 0)
   {
      printf ("Error Sending.\n");
      return (-1);
   }

   return (0);
}

/***********************************************************************/
/* Called to receive a PDU from the iSNS */
/***********************************************************************/
int
RcvPDU (ISNS_CMD * cmd, int size)
{
   int e;
   int rcvSize;
   socklen_t len;

   if (tcpFlag)
   {
      /* Using TCP */
      rcvSize = 0;
      while ((e = TCPGetMsg (cmd, size)) == -1)
      {
         /* Loops until we receive a msg */
         usleep(100);
      }

   }
   else
   {
      /* Using UDP */
      len = sizeof (their_addr);
      e = recvfrom (fd, (char *)cmd, size, 0, res->ai_addr, &res->ai_addrlen);

      if (e < 0)
      {
         printf ("Error Receiving.\n");
         return (e);
      }
   }

   return (e);
}

/***********************************************************************/
/* Sends a PDU.  This function will actually call send() or sento(). */
/***********************************************************************/
int
SendPDU (ISNS_CMD * cmd, int len)
{
   int e;
   fd_set read_set;
   struct timeval timeout;
   int nb;


   if (tcpFlag)
   {
      /* Using TCP */
      e = send (fd, (char *)cmd, len, 0);
      if (e < 0)
      {
         printf ("Error Sending TCP request.\n");
         return (-1);
      }

   }
   else
   {
      e = sendto (fd, (char *) cmd, len, 0, res->ai_addr, res->ai_addrlen);
      if (e < 0)
      {
         printf ("Error Sending UDP request.\n");
         return (e);
      }

      /* setup a timer to check for results */
      FD_ZERO(&read_set);
      FD_SET(fd, &read_set);
      timeout.tv_sec = 5;   // seconds
      timeout.tv_usec = 0;  // microseconds

      nb = select(fd+1, &read_set, NULL, NULL, &timeout);
      if (nb < 0 )
          printf ("Error setting up socket\n");

      if (nb == 0)
      {
        printf ("Unable to connect - timedout\n");
        exit (-1);  
      }

   }
   return (e);
}

/***********************************************************************/
/* This will send a PDU and wait for a rsp.  The rsp will be ignored.  */
/***********************************************************************/
int
ISNSSendCmd (ISNS_CMD * cmd)
{
   int e;
   char buffer[MAX_PAYLOAD_SIZE];
   int len = cmd->hdr.len + sizeof (ISNS_HDR);
   int errorCode;

   cmd->hdr.len = htons (cmd->hdr.len);
   e = SendPDU (cmd, len);
   if (e < 0)
   {
      printf ("Error Sending.\n");
      return (e);
   }
   if (parserFlag)
     printf("PDU sent-->\n");

   DumpHex (cmd, len);
   len = sizeof (their_addr);
   e = RcvPDU ((struct cmd *)buffer, sizeof (buffer));
   if (e < 0)
   {
      printf ("Error Receiving.\n");
   }
   
   if (parserFlag)
      printf("PDU rcv-->\n");

   DumpHex (buffer, e);

   {
      char *ptr = (char *) buffer + sizeof (ISNS_HDR);
      errorCode = ntohl (*(uint32_t *) ptr);
   }
   if (errorCode != 0)
   {
      printf ("***WARNING: iSNS returned an error, error=%#x, \"%s\"\n", errorCode, errorText(errorCode));
   }
   else
   {
      ISNS_HDR *p_cmd;

      p_cmd = (struct isns_hdr *)buffer;

      /* Convert fields */
      p_cmd->flags = ntohs (p_cmd->flags);
      p_cmd->func_id = ntohs (p_cmd->func_id);
      p_cmd->len = ntohs (p_cmd->len);
      p_cmd->seq = ntohs (p_cmd->seq);
      p_cmd->version = ntohs (p_cmd->version);
      p_cmd->xid = ntohs (p_cmd->xid);
   }

   return (errorCode);
}

/***********************************************************************/
/* This will send a PDU and wait for a rsp.  The rsp will be returned. */
/***********************************************************************/
int
ISNSSendCmd2 (ISNS_CMD * cmd, char *rcvBuffer, int rcvSize)
{
   int e;
   int len = cmd->hdr.len + sizeof (ISNS_HDR);
   int errorCode;

   cmd->hdr.len = htons (cmd->hdr.len);
   e = SendPDU (cmd, len);
   if (e < 0)
   {
      printf ("Error Sending.\n");
      return e;
   }

   if (parserFlag)
      printf("PDU sent-->\n");
   DumpHex (cmd, len);
   len = sizeof (their_addr);
   e = RcvPDU ((struct cmd *)rcvBuffer, rcvSize);

   if (e < 0)
   {
      printf ("Error Receiving.\n");
   }

   if (parserFlag)
      printf("PDU rcv-->\n");

   DumpHex (rcvBuffer, e);

   {
      char *ptr = (char *) rcvBuffer + sizeof (ISNS_HDR);
      errorCode = ntohl (*(uint32_t *) ptr);
   }
   if (errorCode != 0)
   {
      printf ("***WARNING: iSNS returned an error, error=%#x, \"%s\"\n", errorCode, errorText(errorCode));
   }
   else
   {
      ISNS_HDR *p_cmd;

      p_cmd = (struct isns_hdr *)rcvBuffer;

      /* Convert fields */
      p_cmd->flags = ntohs (p_cmd->flags);
      p_cmd->func_id = ntohs (p_cmd->func_id);
      p_cmd->len = ntohs (p_cmd->len);
      p_cmd->seq = ntohs (p_cmd->seq);
      p_cmd->version = ntohs (p_cmd->version);
      p_cmd->xid = ntohs (p_cmd->xid);
   }

   return (errorCode);
}

/***********************************************************************/
/* This will listen for a heartbeat and using the heartbeat message to
   initialize some variables. */
/***********************************************************************/
int
HeartBeatListener (void)
{
   SNS_Hb *hb_ptr;
   SOCKET fd;
   struct sockaddr_in their_addr;
   struct sockaddr_in my_addr;
   ISNS_CMD cmd;
   int e;
   socklen_t len;

   if ((fd = socket (AF_INET, SOCK_DGRAM, 0)) < 0)
   {
      perror ("Fatal Error while calling socket");
      return (-1);
   }

   /* setup for the bind */
   my_addr.sin_family = AF_INET;
   my_addr.sin_port = htons ((short) ISNS_HEARTBEAT_PORT);
   my_addr.sin_addr.s_addr = INADDR_ANY;

   /* Do a Bind */
   if (bind (fd, (struct sockaddr *) &my_addr, sizeof (my_addr)) < 0)
   {
      printf ("Fatal Error while Binding using heartbeat.\n");
      return (-1);
   }


   len = sizeof (their_addr);

   printf ("Waiting for a heartbeat...\n");
   while (1)
   {
      len = sizeof (their_addr);
      e = recvfrom (fd, (char *)&cmd, sizeof (cmd) - sizeof (ISNS_HDR), 0,
                    (struct sockaddr *) &their_addr, &len);

      if (e < 0)
      {
         printf ("***ERROR: recvfrom().\n");
      }
      if (ntohs (cmd.hdr.func_id) != ISNS_HEART_BEAT)
         continue;

      printf ("RCVd: Heart beat-->");
      DumpHex (&cmd, e);

      hb_ptr = (struct sns_hb_payload *)((char *) &cmd + sizeof (ISNS_HDR));

      hb_ptr->counter = ntohl (hb_ptr->counter);
      hb_ptr->interval = ntohl (hb_ptr->interval);
      hb_ptr->tcp_port = ntohs (hb_ptr->tcp_port);
      hb_ptr->udp_port = ntohs (hb_ptr->udp_port);

      isns_port = hb_ptr->udp_port;

      {
         struct in_addr ip;
         ip.s_addr = *(uint32_t *) ((char *) hb_ptr->ip_ptr + 12);
         printf ("SNS IP: %s.\n", inet_ntoa (ip));
         strcpy (p_ip, inet_ntoa (ip));
      }
      printf ("Heartbeat counter: %u.\n", hb_ptr->counter);
      printf ("Heartbeat interval: %u.\n", hb_ptr->interval);
      printf ("Heartbeat tcp_port: %u.\n", hb_ptr->tcp_port);
      printf ("Heartbeat udp_port: %u.\n", hb_ptr->udp_port);

      break;
   }

   return (0);
}

int
L3_HeartBeatListener (void)
{
   SNS_Hb *hb_ptr;
   SOCKET fd;
   struct sockaddr_in their_addr;
   struct sockaddr_in my_addr;
   ISNS_CMD cmd;
   int e;
   socklen_t len;
   struct ip_mreq stMreq;
   int iRet;

   if ((fd = socket (AF_INET, SOCK_DGRAM, 0)) < 0)
   {
      perror ("Fatal Error while calling socket");
      return (-1);
   }

   /* setup for the bind */
   my_addr.sin_family = AF_INET;
   my_addr.sin_port = htons ((short) ISNS_HEARTBEAT_PORT);
   my_addr.sin_addr.s_addr = INADDR_ANY;

   /* Do a Bind */
   if (bind (fd, (struct sockaddr *) &my_addr, sizeof (my_addr)) < 0)
   {
      printf ("Fatal Error while Binding.\n");
      return (-1);
   }

   /* join the multicast group */
   stMreq.imr_multiaddr.s_addr = inet_addr(multicast_addr);
   stMreq.imr_interface.s_addr = INADDR_ANY;

   iRet = setsockopt(fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, 
     (char *)&stMreq, sizeof(stMreq));

   if (iRet < 0) {
     printf ("setsockopt() IP_ADD_MEMBERSHIP failed, Err: %d\n",
#ifdef SNS_LINUX
    0);
#else
        WSAGetLastError());
#endif
  } 

   len = sizeof (their_addr);

   printf ("Waiting for a heartbeat...\n");
   while (1)
   {
      e = recvfrom (fd, (char *)&cmd, sizeof (cmd) - sizeof (ISNS_HDR), 0,
                    (struct sockaddr *) &their_addr, &len);

      if (e < 0)
      {
         printf ("***ERROR: recvfrom().\n");
      }
      if (ntohs (cmd.hdr.func_id) != ISNS_HEART_BEAT)
         continue;

      printf ("RCVd: Heart beat-->");
      DumpHex (&cmd, e);

      hb_ptr = (struct sns_hb_payload *)((char *) &cmd + sizeof (ISNS_HDR));

      hb_ptr->counter = ntohl (hb_ptr->counter);
      hb_ptr->interval = ntohl (hb_ptr->interval);
      hb_ptr->tcp_port = ntohs (hb_ptr->tcp_port);
      hb_ptr->udp_port = ntohs (hb_ptr->udp_port);

      isns_port = hb_ptr->udp_port;

      {
         struct in_addr ip;
         ip.s_addr = *(uint32_t *) ((char *) hb_ptr->ip_ptr + 12);
         printf ("SNS IP: %s.\n", inet_ntoa (ip));
         strcpy (p_ip, inet_ntoa (ip));
      }
      printf ("Heartbeat counter: %u.\n", hb_ptr->counter);
      printf ("Heartbeat interval: %u.\n", hb_ptr->interval);
      printf ("Heartbeat tcp_port: %u.\n", hb_ptr->tcp_port);
      printf ("Heartbeat udp_port: %u.\n", hb_ptr->udp_port);

      break;
   }

   return (0);
}
/***********************************************************************/
/* Thread used for listening and responding to UDP ESI/SCN messages. */
/***********************************************************************/
#ifdef SNS_LINUX
int
#else
DWORD WINAPI
#endif
ESIListener (DWORD lparam)
{
   char eid[256];
   char buffer[1048];
   char rbuffer[1048];
   LISTENER_CB_P p_cb = (LISTENER_CB_P)&lparam;
   int ip = p_cb->ip;
   int port = p_cb->port;
   int e;
   socklen_t len;
   struct sockaddr_in their_addr;
   struct sockaddr_in my_addr;
   ISNS_Attr *p_attr;
   SOCKET fd;
   ISNS_CMD *p_cmd;
   ISNS_CMD *p_rcmd;

   printf ("ESI Listener thread started for EID=%s.\n", p_cb->eid);

   strcpy (eid, p_cb->eid);
   free (p_cb);

   if ((fd = socket (AF_INET, SOCK_DGRAM, 0)) < 0)
   {
      perror ("Fatal Error while calling socket");
      return (0);
   }

   /* setup for the bind */
   my_addr.sin_family = AF_INET;
   my_addr.sin_port = htons ((short) port);
   my_addr.sin_addr.s_addr = INADDR_ANY;

   /* Do a Bind */
   if (bind (fd, (struct sockaddr *) &my_addr, sizeof (my_addr)) < 0)
   {
      printf ("Fatal Error while Binding.\n");
      return (0);
   }

   /* Setup Variables, Addresses, Etc. */
   their_addr.sin_family = AF_INET; /* host byte order */
   their_addr.sin_port = htons ((short) isns_port);   /* short, network byte order */
   their_addr.sin_addr.s_addr = ip;

   len = sizeof (their_addr);

   p_cmd = (struct cmd *)buffer;
   p_attr = (struct ISNS_attr *)((char *) buffer + sizeof (ISNS_HDR));
   while (1)
   {
      /* Listen for ESI */
      e = recvfrom (fd, buffer, sizeof (buffer), 0,
                    (struct sockaddr *) &their_addr, &len);

      if (e < 0)
      {
         printf ("Error Receiving.\n");
         return (-1);
      }

      printf ("RCV ESI for %s via UDP-->",(char *)&p_attr->val);
      DumpHex (buffer, e);

      p_cmd->hdr.func_id = ntohs (p_cmd->hdr.func_id);

      /* Verify Msg */
      if (p_cmd->hdr.func_id == ISNS_SCN)
      {
         printf ("***RCV: SCN.\n");
         Process_SCN (p_cmd, e);
         continue;
      }

      if (p_cmd->hdr.func_id != ISNS_ESI)
      {
         printf ("***ERROR: Expected ESI, recv %#x.\n", p_cmd->hdr.func_id);
         continue;
      }

      /* Send response */
      memset (rbuffer, 0, sizeof (rbuffer));
      ISNSCreateHdr (ISNS_ESI_RSP, (struct cmd *)rbuffer, sizeof (rbuffer), 0);
      memcpy ((char *) rbuffer + sizeof (ISNS_HDR) + 4,
              buffer + sizeof (ISNS_HDR), ntohs (p_cmd->hdr.len));
      p_rcmd = (struct cmd *)rbuffer;
      p_rcmd->hdr.len = ntohs (p_cmd->hdr.len) + 4;
      p_rcmd->hdr.xid = p_cmd->hdr.xid;

      ParseESI (buffer);

      if (FALSE == enableESIFlag)
      {
         printf ("ESI Response is disabled.\n");
         continue;
      }

      printf ("esi rsp--->");
      ISNSJustSendCmd ((struct cmd *)rbuffer);
   }
}
/***********************************************************************/
/* Sends an ESI response message. */
/***********************************************************************/
void
SendESIRsp (void)
{
   char cmdLine[256];
   ISNS_CMD cmd;

   memset (cmdLine, 0, sizeof (cmdLine));

   printf ("Sending ESI Response.\n");
   ISNSCreateHdr (ISNS_REG_DEV_ATTR_REQ, &cmd, sizeof (cmd), 0);

   /* Get an entity ID */
   if (0 == GetInput(cmdLine, "Entity ID", NULL, sizeof(cmdLine)))
   {
      printf ("***ERROR: Cmd Line required.\n");
      exit (-1);
   }

   ISNSAppendAttr (&cmd, ISNS_ENTITY_ID, PAD4 (strlen (cmdLine)), cmdLine, 0);

   ISNSJustSendCmd (&cmd);
}
/***********************************************************************/
/* Thread used to receive ALL TCP messages */
/***********************************************************************/
#ifdef SNS_LINUX
int
#else
DWORD WINAPI
#endif
TCPReceiveMain (DWORD lparam)
{
   int e;
   char buffer[MAX_PAYLOAD_SIZE];
   ISNS_CMD *p_cmd;
   int msg_size;
   int pending;
   int startIndex;
   int endIndex;

   startIndex = 0;
   endIndex = 0;
   while (1)
   {
      e = recv (fd, &buffer[endIndex], sizeof (buffer), 0);
      if (e < 0)
      {
         printf ("***ERROR: recv().\n");
         exit(-1);
      }
      else if (e == 0)
      {
         printf("\nConnection closed by isns server\n");
         exit(-1);
      }     

      endIndex += e;
      pending = endIndex - startIndex;

      /* Check to see if we received all the message. */
      while (pending > sizeof (ISNS_HDR))
      {
         p_cmd = (struct cmd *)&buffer[startIndex];
         msg_size = ntohs (p_cmd->hdr.len) + sizeof (ISNS_HDR);

         if (pending < msg_size)
            break;

         switch (ntohs (p_cmd->hdr.func_id))
         {
         case ISNS_ESI:
            printf ("Rcv ESI via TCP-->\n");
            DumpHex (p_cmd, msg_size);
            Send_ESI_Response (p_cmd, msg_size);
            break;
         case ISNS_SCN:
            printf ("Rcv SCN via TCP-->\n");
            Process_SCN (p_cmd, msg_size);
            break;
         default:
            AddMsg (p_cmd, msg_size);
            break;
         }
         startIndex += msg_size;
         pending -= msg_size;
      }

      if (pending == 0)
      {
         startIndex = 0;
         endIndex = 0;
      }
   }
}
/***********************************************************************/
/* Used to retrieve messages from the TCP message Q. */
/***********************************************************************/
int
TCPGetMsg (void *buffer, int b_size)
{
   int size;
   if (NULL == (msg_q[msgCurrentHead].p_msg))
      return (-1);

   size = msg_q[msgCurrentHead].size;
   memcpy (buffer, msg_q[msgCurrentHead].p_msg, msg_q[msgCurrentHead].size);
   free (msg_q[msgCurrentHead].p_msg);
   msg_q[msgCurrentHead].p_msg = NULL;

   msgCurrentHead = (msgCurrentHead + 1) % MSG_Q_SIZE;

   return (size);
}




