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
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/netdb.h>
#include <sys/basetype.h>
#include <sys/error.h>
#include <sys/l3vpn.h>
#include <sys/dns.h>
#include <sys/list.h>
#include <sys/assert.h>
#include <sys/epoll.h>
#include <sys/in.h>

#include "../../include/iscsi_basetype.h"
#include "../../include/iscsi_com.h"
#include "../../include/iscsi_main.h"
#include "../../include/iscsi_event.h"
#include "../../include/iscsi_util.h"

#include "iSNS.h"
#include "iSNStypes.h"
#include "iSNScomm.h"
#include "iSNSmsg.h"
#include "iSNSparse.h"
#include "iSNSdebug.h"
#include "iSNSrcv.h"
#include "iSNSEpoll.h"
#include "iSNSevent.h"
#include "errno.h"

int sns_comm_debug = 0;
int sns_comm_msg_filter = 0;

char  snsp_subnet[16];
char  snsp_ip[16]="0.0.0.0";
char  snsp_bip[16]=SNS_BROADCAST_ADDR;
int   snsp_port;
char  sns_if_name[10] = {SNS_IF_NAME};

extern pthread_mutex_t sns_status_mutex;
extern pthread_cond_t sns_status_cond;

extern int sns_status;

extern ULONG ISCSI_ISNS_TrcvEpollCallBack(IN UINT uiEvent, IN VOID *pHandle);

int ifAddrGet(char *ifname, char *ip)
{
    if (strlen (snsp_ip) != 0) {
        strcpy(ip, snsp_ip);
    }
    else
    {
        return (ERROR);
    }

    return(SUCCESS);
}

int ifBroadcastGet(char *ifname, char *ip)
{
   strcpy(ip, snsp_bip);
   return(SUCCESS);
}

/* NOTE: none of these are IP_ADDR 16 compliant */
int     sns_comm_support        = SNS_LAYER_2;
int     sns_comm_support_set    = SNS_LAYER_2;

int     sns3 = 0;
int     sns_comm_main_port      = SNS_UDP_PORT;
int     sns_comm_main_port_set  = SNS_UDP_PORT;
int     sns_comm_mcast_port     = SNS_UDP_PORT + 1;
int     sns_comm_mcast_grp      = 0;
int     sns_comm_mcast_grp_set  = 0;

/* Variable to enable Hacks */
int enableHack=0;
/*
 * Socket descriptor used by the SOIP service entity to communicate
 * with a remote SOIP service entity
 */
SOCKET sd;
SOCKET mc_sd;
SOCKET isns_sd;

/* Socket address of the ISNS */
struct sockaddr_in  isns_sock;

/*
 * Socket address of the SOIP entity
 */
struct sockaddr_in  local_sock;

/*
 * Socket address of the server
 */
struct sockaddr_in  server_sock;

/*
 * Socket address of the service client/server multicast group
 */
struct sockaddr_in  mc_sock;

void pipe_handler(int n)
{
  __LOG_INFO ("SIGPIPE received:%i",n);
}

/*
 * Function Name: SNSCommInit
 *
 * Synopsis:      int SNSCommInit(entity);
 *
 * Arguments:     entity - type of SOIP service entity for
 *                which communication services need to be
 *                initialized.
 *
 * Description:   Creates a UDP socket, binds it to the SoIP
 *                port and joins the IP multicast group based
 *                on the type of SOIP entity.
 *
 * Return value:  SUCCESS (0)
 *                ERROR (-1)
 *
 */
int
SNSCommInit (ISNS_Entity entity)
{
   char            ip_addr[INET_ADDR_LEN+1];
   int             bcast_on;
   int             not_needed = 1;

    /*
     * Check the entity type, Is it a client or server ?
     */
     if ((entity != ISNS_CLIENT) && (entity != ISNS_SECONDARY_SERVER))
        return (ERROR);

    /*
     * Set initialization parameters
     *
     */
    sns_comm_support    = sns_comm_support_set;
    sns_comm_main_port  = sns_comm_main_port_set;
    sns_comm_mcast_port = sns_comm_main_port_set + 1;
    sns_comm_mcast_grp  = sns_comm_mcast_grp_set;

    if ((sns_comm_support == SNS_LAYER_3) &&
        (!IN_CLASSD(sns_comm_mcast_grp)))
       sns_comm_mcast_grp = inet_addr(SNS_SCGRP_IPMC_ADDR);

    /*
     * create UDP socket.
     */
     if ((sd = socket (AF_INET, SOCK_DGRAM, 0)) < 0)
     {
        __LOG_ERROR("\nSNSCommInit:socket");
        perror("\nSNSCommInit:socket");
        return (-1);
     }

     /* Allow reuse of addresses */
     if (setsockopt (sd, SOL_SOCKET, SO_REUSEADDR, &not_needed, sizeof(not_needed)) != 0)
     {
      __LOG_ERROR ("Reuse address");
      perror ("Reuse address");
      return (-1);
     }

     if ((mc_sd = socket (AF_INET, SOCK_DGRAM, 0)) < 0)
     {
        __LOG_ERROR("\nSNSCommInit:socket");
        perror("\nSNSCommInit:socket");
        return (-1);
     }

     /* Allow reuse of addresses */
    if (setsockopt (mc_sd, SOL_SOCKET, SO_REUSEADDR, &not_needed, sizeof(not_needed)) != 0)
    {
       __LOG_ERROR ("\nReuse address");
       perror ("\nReuse address");
       return (-1);
    }

     bzero ((char *) &local_sock, sizeof (struct sockaddr_in));
     bzero ((char *) &server_sock, sizeof (struct sockaddr_in));
     bzero ((char *) &mc_sock, sizeof (struct sockaddr_in));

     local_sock.sin_family   = AF_INET;
     server_sock.sin_family  = AF_INET;
     mc_sock.sin_family      = AF_INET;

     local_sock.sin_port   = htons ((short)sns_comm_main_port);
     server_sock.sin_port  = htons ((short)sns_comm_main_port);
     mc_sock.sin_port      = htons ((short)sns_comm_mcast_port);

     local_sock.sin_addr.s_addr = SNSGetIfAddr();

     /* Allow reuse of addresses */
     if (setsockopt (sd, SOL_SOCKET, SO_REUSEADDR, &not_needed, sizeof(not_needed)) < 0)
     {
      __LOG_ERROR ("Reuse address");
      perror ("Reuse address");
      return (-1);
     }

    /*
     * Bind it to the well-known SoIP UDP port
     */
     if (0 != bind (sd,
                (struct sockaddr *)&local_sock,
                sizeof (struct sockaddr)))
     {
          close(sd);
          __LOG_ERROR ("\nSNSCommInit: bind");
          perror ("\nSNSCommInit: bind");
          return (-1);
     }

     mc_sock.sin_addr.s_addr = inet_addr(ip_addr);

     /* SNSGetIfBroadcastAddr();  */
     /* htonl (INADDR_BROADCAST); */

      /* Allow reuse of addresses */
      if (setsockopt (mc_sd, SOL_SOCKET, SO_REUSEADDR, &not_needed, sizeof(not_needed)) < 0)
      {
          __LOG_ERROR ("Reuse address");
          perror ("Reuse address");
          return (-1);
      }

     if (bind (mc_sd, (struct sockaddr *)&mc_sock,
                sizeof (struct sockaddr)) != 0 )
     {
          close (sd);
          close (mc_sd);
          __LOG_ERROR ("\nSNSCommInit: bind");
          perror ("\nSNSCommInit: bind");
          return (-1);
     }

     ifBroadcastGet(sns_if_name, ip_addr);
     mc_sock.sin_addr.s_addr = inet_addr(ip_addr);
     /*
      * Enable broadcast traffic on the iSNS socket.
      * This is a temporary workaround since Tornado 1.0 does not
      * support IP multicasting.
      */
      bcast_on = 1;
      if (setsockopt (sd, SOL_SOCKET, SO_BROADCAST,
                     (char *) &bcast_on, sizeof (int)) == -1 )
      {
          close (sd);
          __LOG_ERROR ("\nSNSCommInit: setsockopt SO_BROADCAST:");
          perror ("\nSNSCommInit: setsockopt SO_BROADCAST:");
          return (-1);
      }

     signal (SIGPIPE, pipe_handler);

     return (SUCCESS);
}

/*
 * Function Name: SNSGetMsg
 *
 * Synopsis:      int SNSGetMsg(p_md);
 *
 * Arguments:     p_md - address of message descriptor block that will
 *                       be used for storing the received message and
 *                       sender address information.
 *
 * Description:   Receive a message from a remote SOIP service entity.
 *
 * Return value:
 *
 */
int
SNSReceiveMain(void)
{
  /*
   * Wait for a message to arrive on the socket.
   * Forward the received message to the service
   * agent task for processing.
   */

   ISCSI_UTL_EpollAddFd(ISCSI_ISNS_EpollHandleGet(), sd, EPOLLIN, SNSReceiveCallBack);

   ISCSI_UTL_EpollAddFd(ISCSI_ISNS_EpollHandleGet(), mc_sd, EPOLLIN, SNSReceiveCallBack);

   return (SUCCESS);
}

int
ISNSSendMsg(ISNS_Msg_Descp *p_md)
{
   ISNS_Attr      * attr_indx[SNS_MAX_ATTRS];
   ISNS_Attr      * key_indx[SNS_MAX_ATTRS];
   int rval;

   __DEBUG (sns_comm_debug &1,ISNSSendMsg);

   p_md->msg.hdr.flags |= ISNS_FLAG_SND_SERVER;

   ISNSParseMsg(&p_md->msg, attr_indx, key_indx, NULL);

   if (p_md->cb.sock.socketType==ISNS_SOCKET_TYPE_UDP)
   {
      /* UDP */
      rval = SNSSendMsg_UDP(p_md);
   }
   else if (p_md->cb.sock.socketType==ISNS_SOCKET_TYPE_TCP)
   {
      /* TCP */
      rval = SNSSendMsg_TCP(p_md);
   }
   else
   {
      /* UDP */
      rval = SNSSendMsg_UDP(p_md);
      __LOG_WARNING ("WARNING: Wrong Socket Type.");

      return (rval);
   }

   return (rval);
}

int
SNSSendMsg_TCP(ISNS_Msg_Descp *p_md)
{
   int  s_len;
   int  len;
   int  dest_version;
   char dot_not_addr[INET_ADDR_LEN];

   __DEBUG (sns_comm_debug &1,ISNSSendMsg_TCP);

   SNSConvertPayloadHTON(&p_md->msg);

   len = sizeof (SNS_Msg_Hdr) + p_md->msg.hdr.msg_len;
   inet_ntoa_b (p_md->cb.sock.addr.sin_addr, dot_not_addr);
   dest_version = p_md->msg.hdr.version;

   p_md->msg.hdr.type     = htons (p_md->msg.hdr.type);
   p_md->msg.hdr.xid      = htons (p_md->msg.hdr.xid);
   p_md->msg.hdr.version  = htons (SNS_VERSION);
   p_md->msg.hdr.sequence = htons (p_md->msg.hdr.sequence);
   p_md->msg.hdr.msg_len  = htons (p_md->msg.hdr.msg_len);

   p_md->msg.hdr.flags |= ISNS_FLAG_LAST_PDU | ISNS_FLAG_FIRST_PDU;
   p_md->msg.hdr.flags = htons(p_md->msg.hdr.flags);

   s_len = send(p_md->cb.sock.sockfd, (char *)&p_md->msg, len, 0);
   if (s_len<=0)
   {
      __LOG_INFO ("send() error=%d %s",s_len, strerror(errno) );
      return (ERROR);
   }

   __DEBUG (sns_comm_debug &1,ISNSSendMsg_TCP finished);
   return (SUCCESS);
}

/*
 * Function Name: SNSSendMsg
 *
 * Synopsis:      SNSSendMsg(p_md);
 *
 * Arguments:     p_md - pointer to message descriptor block. The iSNS
 *                       message associaed with this descriptor is
 *                       transmitted to the address contained in the
 *                       descriptor block.
 *
 * Description:   Send a iSNS message to a remote SOIP service entity
 *
 * Return value:  SUCCESS (0) OR ERROR (-1)
 *
 */
int
SNSSendMsg_UDP(ISNS_Msg_Descp *p_md)
{

     int  status;
     int  len, msg_len, payload_len, count;
     int  dest_version;
     int  display_debug;
     char dot_not_addr[INET_ADDR_LEN];

     SNSConvertPayloadHTON(&p_md->msg);

     len = sizeof (SNS_Msg_Hdr) + p_md->msg.hdr.msg_len;
     inet_ntoa_b (p_md->cb.sock.addr.sin_addr, dot_not_addr);
     dest_version = p_md->msg.hdr.version;

     p_md->msg.hdr.type     = htons (p_md->msg.hdr.type);
      p_md->msg.hdr.xid      = htons (p_md->msg.hdr.xid);
      p_md->msg.hdr.version  = htons (SNS_VERSION);
      p_md->msg.hdr.sequence = htons (p_md->msg.hdr.sequence);
      p_md->msg.hdr.msg_len  = htons (p_md->msg.hdr.msg_len);

     display_debug = 0;
     if (sns_comm_debug == 1)
        display_debug = (p_md->msg.hdr.type != sns_comm_msg_filter);
     else if (sns_comm_debug == 2)
        display_debug = (p_md->msg.hdr.type == sns_comm_msg_filter);

     status = SUCCESS;
     if (len > SNS_MAX_ETH_PACKET_SIZE)
     {
        count = 0;
        p_md->msg.hdr.flags |= ISNS_FLAG_FIRST_PDU;
        while ((status != ERROR) && (len > (int)sizeof(SNS_Msg_Hdr)))
        {
           msg_len = MIN(len, SNS_MAX_ETH_PACKET_SIZE);
           if (msg_len == len)
              p_md->msg.hdr.flags |= ISNS_FLAG_LAST_PDU;

           p_md->msg.hdr.flags = htons(p_md->msg.hdr.flags);
           payload_len = msg_len - sizeof(SNS_Msg_Hdr);
           p_md->msg.hdr.msg_len = htons ((short)payload_len);
           status = sendto(sd, (void *)&p_md->msg, msg_len, 0,
                           (struct sockaddr *)&p_md->cb.sock.addr,
                           p_md->cb.sock.len);
           __DEBUG (display_debug,
                    (iSNS Sending msg id %d, len %d, xid %d to %s),
                    p_md->msg.hdr.type, p_md->msg.hdr.msg_len,
                    p_md->msg.hdr.xid, dot_not_addr);
           count++;
           p_md->msg.hdr.sequence = htons ((short)count);
           len -= payload_len;
           memcpy(&p_md->msg.payload, (char *)&p_md->msg.payload +
                  payload_len, len - sizeof(SNS_Msg_Hdr));
        }
     }
     else
     {
        if (len > (int)SNS_OLD_MAX_PACKET_SIZE)
           p_md->msg.hdr.msg_len  = htons (SNS_OLD_MAX_PACKET_SIZE);

        p_md->msg.hdr.flags |= (ISNS_FLAG_LAST_PDU | ISNS_FLAG_FIRST_PDU);
        p_md->msg.hdr.flags = htons(p_md->msg.hdr.flags);
        status = sendto(sd, (void *)&p_md->msg, len, 0,
                        (struct sockaddr *)&p_md->cb.sock.addr,
                        p_md->cb.sock.len);
        __DEBUG (display_debug,
                 (iSNS Sending msg id %#x, len %d, xid %d to %s),
                 ntohs(p_md->msg.hdr.type), ntohs(p_md->msg.hdr.msg_len),
                 ntohs(p_md->msg.hdr.xid), dot_not_addr);
        if (status != ERROR)
           status = SUCCESS;
     }

     return (status);
}


/*
 * Function Name: SNSGetServerAddr
 *
 * Synopsis:      SNSGetServerAddr();
 *
 * Description:   Get the socket address of the server.
 *
 * Return value:  SUCCESS (0) OR ERROR (-1)
 *
 */
uint32_t
SNSGetServerAddr(void)
{
   return (server_sock.sin_addr.s_addr);
}



/*
 * Function Name: SNSCommUpdate
 *
 * Synopsis:      SNSCommUpdate(server_addr);
 *
 * Description:   update the socket address of the server.
 *
 * Return value:  SUCCESS (0) OR ERROR (-1)
 *
 */
void
ISNSCommUpdate (struct in_addr  *server_addr)
{
   if (server_sock.sin_addr.s_addr != server_addr->s_addr)
      server_sock.sin_addr.s_addr = server_addr->s_addr;
}





/*
 * Function Name: SNSGetLocalAddr
 *
 * Synopsis:      SNSGetLocalAddr();
 *
 * Description:   Get the Local socket address.
 *
 * Return value:  pointer to the local socket address structure.
 *
 */
struct in_addr *
SNSGetLocalAddr(void)
{
   return(&local_sock.sin_addr);
}



/*
 * Function Name: SNSGetMCgrpAddr
 *
 * Synopsis:      SNSGetMCgrpAddr();
 *
 * Description:   Get the multicast group (*misnomer*--actually broadcast)
 *                socket address.
 *
 * Return value:  pointer to the multicast group socket address structure.
 *
 */
struct in_addr *
SNSGetMCgrpAddr(void)
{
  return(&mc_sock.sin_addr);
}


/*
 * Function Name: SNSSendMsg2Server
 *
 * Synopsis:      SNSSendMsg2MCServer (p_msg);
 *
 * Arguments:     p_msg  - pointer to message that will be sent
 *                         to the iSNS server
 *
 * Description:   Send an iSNS message to the SOIP service server
 *
 * Return value:  SUCCESS (0) OR ERROR (-1)
 *
 */
int
ISNSSendMsg2Server (ISNS_Msg *p_msg)
{

    int  status;
    int  len;
    int  display_debug;
    char dot_not_addr[INET_ADDR_LEN];

    if (server_sock.sin_addr.s_addr == 0)
       return ERROR;

    inet_ntoa_b (server_sock.sin_addr, dot_not_addr);
    len = sizeof (SNS_Msg_Hdr) + p_msg->hdr.msg_len;

    SNSConvertPayloadHTON(p_msg);

    if (p_msg->hdr.type<999)
    {
       p_msg->hdr.type     = htons (p_msg->hdr.type);
       p_msg->hdr.xid      = htons (p_msg->hdr.xid);
       p_msg->hdr.msg_len  = htons (p_msg->hdr.msg_len);
       p_msg->hdr.flags    = htons (p_msg->hdr.flags);
       p_msg->hdr.version  = htons (SNS_VERSION);
       p_msg->hdr.sequence = htons (p_msg->hdr.sequence);
    }
    else
    {
       __LOG_WARNING("Warning: type is already in network byte order.");
    }

    len = sizeof (SNS_Msg_Hdr) + ntohs(p_msg->hdr.msg_len);
    display_debug = 0;
    if (sns_comm_debug == 1)
       display_debug = (p_msg->hdr.type != sns_comm_msg_filter);
    else if (sns_comm_debug == 2)
       display_debug = (p_msg->hdr.type == sns_comm_msg_filter);

    if ( sendto(sd, (void *)p_msg, len, 0, (struct sockaddr *)&server_sock,
                sizeof(struct sockaddr_in)) == ERROR)
    {
        __DEBUG (sns_comm_debug, (Error in Sending message to %s),
                 dot_not_addr);
        status = ERROR;
    }
    else
    {
        __DEBUG (display_debug, (Sending message to %s), dot_not_addr);
        status = SUCCESS;
    }

   return (status);

}



/*
 * Function Name: SNSSendMsg2MCgrp
 *
 * Synopsis:      SNSSendMsg2MCgrp (p_msg);
 *
 * Arguments:     p_msg  - pointer to message that will be multicast
 *                         to the SOIP service server/client group
 *
 * Description:   Send an iSNS message to the SOIP service server/client
 *                multicast group.
 *
 * Return value:  SUCCESS (0) OR ERROR (-1)
 *
 */
int
SNSSendMsg2MCgrp (ISNS_Msg *p_msg)
{

    int  status;
    int  len;
    int  display_debug;
    struct sockaddr_in  mc_dest_sock;
    char dot_not_addr[INET_ADDR_LEN];

    len          = sizeof (SNS_Msg_Hdr) + p_msg->hdr.msg_len;
    SNSConvertPayloadHTON(p_msg);

    p_msg->hdr.type     = htons (p_msg->hdr.type);
    p_msg->hdr.xid      = htons (p_msg->hdr.xid);
    p_msg->hdr.msg_len  = htons (p_msg->hdr.msg_len);
    p_msg->hdr.flags    = htons ((short)(p_msg->hdr.flags | ISNS_FLAG_SND_SERVER
                                 | ISNS_FLAG_LAST_PDU | ISNS_FLAG_FIRST_PDU));
    p_msg->hdr.version  = htons (SNS_VERSION);
    p_msg->hdr.sequence = htons (p_msg->hdr.sequence);

    status = SUCCESS;
    if (sns_comm_support == SNS_LAYER_2)
    {
       inet_ntoa_b (mc_sock.sin_addr, dot_not_addr);
       if ( sendto(sd, (void *)p_msg, len,
                   0, (struct sockaddr *)&mc_sock,
                   sizeof(struct sockaddr_in)) == ERROR)
          status = ERROR;
    }
    else /* SNS_LAYER_3 */
    {
       bzero ((char *) &mc_dest_sock, sizeof (struct sockaddr_in));
       mc_dest_sock.sin_family      = AF_INET;
       mc_dest_sock.sin_port        = htons((short)sns_comm_mcast_port);
       mc_dest_sock.sin_addr.s_addr = htonl(sns_comm_mcast_grp);
       inet_ntoa_b (mc_dest_sock.sin_addr, dot_not_addr);

       if ( sendto(mc_sd, (void *)p_msg, len,
                   0, (struct sockaddr *)&mc_dest_sock,
                   sizeof(struct sockaddr_in)) == ERROR)
          status = ERROR;
   }

    display_debug = 0;
    if (sns_comm_debug == 3)
       display_debug = (p_msg->hdr.type != sns_comm_msg_filter);

    __DEBUG (display_debug,
             (iSNS Sending bcast/mcast msg id %d, len %d, xid %d to %s),
             p_msg->hdr.type, p_msg->hdr.msg_len,
             p_msg->hdr.xid, dot_not_addr);

    return (status);

}



/*
 * Function Name: SNSGetIfAddr
 *
 * Synopsis:      SNSGetIfAddr();
 *
 * Description:   Obtain the IP address associated with the NIC being
 *                used by a SOIP service entity.
 *
 * Return value:  32 bit IPv4 address in network byte order.
 *
 */
int
SNSGetIfAddr(void)
{
   char  ip_addr[16];
   int ip;

   if (ifAddrGet(sns_if_name, ip_addr) == ERROR)
   {
      //return(INADDR_ANY);
      ip = SNSGetIPAddress();
      return ip;
   }
   else
      return(inet_addr(ip_addr));
}

/*
 * Function Name: SNSGetIfBroadcastAddr
 *
 * Synopsis:      SNSGetIfBroadcastAddr();
 *
 * Description:   Obtain the Broadcat IP address associated with the NIC being
 *                used by a SOIP service entity.
 *
 * Return value:  32 bit IPv4 address in network byte order.
 *
 */
int
SNSGetIfBroadcastAddr(void)
{
   char  ip_addr[16];

   if (ifBroadcastGet(sns_if_name, ip_addr) == ERROR)
      return(htonl(INADDR_ANY));
   else
      return(inet_addr(ip_addr));
}

/*
 * Function Name: SNSGetMgmtAddr
 *
 * Synopsis:      SNSGetMgmtAddr();
 *
 * Description:   Obtain the IP address associated with the
 *                switch management interface.
 *
 * Return value:  32 bit IPv4 address in network byte order.
 *
 */
int
SNSGetMgmtAddr(void)
{
   char ip_addr[INET_ADDR_LEN+1];
   if (ifAddrGet(SNS_IFMGMT_NAME, ip_addr) == ERROR)
      return(INADDR_ANY);
   else
      return(inet_addr(ip_addr));
}

int
SNSGetIPAddress()
{
   char hostname[256];
   struct hostent *host;
   struct in_addr h_addr;

   gethostname(hostname,sizeof(hostname));
   host = gethostbyname(hostname);
   h_addr.s_addr= *(uint32_t *)(host->h_addr_list[0]);
   return h_addr.s_addr;
}

/*********************************************************************
     Func Name : ISCSI_ADJ_ResetNotify
  Date Created : 2010/10/20
        Author : chaojunxian  07617
   Description : 异步通知邻居线程reset
         Input : IN UINT uiSysIndex          系统索引
                 IN USHORT usMtIndex         拓扑索引
                 IN ISCSI_RESET_NOTIFY_TYPE_E enNotifyType     Reset通知类型
                 IN ISCSI_RESET_TRIGGER_TYPE_E enResetReason   Reset触发原因
        Output : NONE
        Return : ERROR_FAILED         失败
                 ERROR_SUCCESS        成功
       Caution : 系统事件模块调用接口
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
ULONG ISCSI_ISNS_ResetNotify(IN USHORT usMtIndex,
                          IN ISCSI_RESET_NOTIFY_TYPE_E enNotifyType,
                          IN ISCSI_RESET_TRIGGER_TYPE_E enResetReason)
{
    RWSTQ_HEAD_S *pstAdjQue = ISCSI_ISNS_ThreadEventMsgQueGet();
    ISCSI_ISNS_EVENT_RESET_MSG_S *pstMsg = NULL;

    IGNORE_PARAM(usMtIndex);
    IGNORE_PARAM(enResetReason);

    ULONG ulSize = sizeof(ISCSI_ISNS_EVENT_RESET_MSG_S);

    pstMsg = (ISCSI_ISNS_EVENT_RESET_MSG_S *) malloc(ulSize);
    if(NULL == pstMsg)
    {
        return ERROR_FAILED;
    }
    memset(pstMsg, 0, ulSize);
    pstMsg->stIsMsgHdr.uiMsgType = enNotifyType;
    pstMsg->stIsMsgHdr.uiSubMsgType = enNotifyType;
    pstMsg->usMtIndex = usMtIndex;
    pstMsg->uiResetReason = enResetReason;

    /*向线程消息队列添加消息并发送信号，通知邻居线程*/
    ISCSI_EVT_ThreadQueueMsgAdd(ISCSI_ISNS_EventFdGet(), pstAdjQue, pstMsg);

    return ERROR_SUCCESS;
}


