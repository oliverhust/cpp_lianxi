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
 * This file contains source code for receiving UDP messages
 * from a SOIP service entity. The messages received over 
 * the socket are queued and later processed by the SOIP
 * service agent task.
 *
 */
#include "iSNS.h"
#include "iSNSmsg.h"
#include "iSNSfsm.h"
#include "iSNScomm.h"
#include "iSNSipc.h"

#include "iSNSdebug.h"

extern int isns_main_debug;

int sns_recv_debug = 0;
int sns_recv_msg_filter  = 0;

/* support fragmentated buffers */
int awaiting_fragment = 0;
int frag_len = 0;
ISNS_Msg_Descp frag_md;

/* ReceiveMain statistics */
int sns_recv_num_dropped = 0;
int sns_recv_num_pri_res = 0;
int sns_recv_num_cli_fwd = 0;

#define ARRAY_DEPTH  20
#define ARRAY_WIDTH  12

#ifdef SNS_LINUX
unsigned int sns_recv_msg_array[ARRAY_DEPTH][2 + ARRAY_WIDTH];
#else
int sns_recv_msg_array[ARRAY_DEPTH][2 + ARRAY_WIDTH];
#endif

         
static ISNS_Msg_Descp md;
static void SNS_SaveRecentMsg(ISNS_Msg_Descp *p_md);

/*
 * Function Name:    SNSReceiveMain
 *
 * Synopsis:         int SNSReceiveMain(int sd);
 *
 * Parameters:       sd = descriptor of receive socket
 *  
 * Description:      SNSMain is the entry-point of the SoIP 
 *                   service task. 
 *
 * Return value:     If task is successfully initialized, 
 *                   SNSMain() will not return. If an error 
 *                   occurs during initialization SNSMain() 
 *                   returns ERROR (-1).
 *
 * 
 */
#ifdef SNS_LINUX
int
#else
DWORD WINAPI
#endif
SNSReceiveMain (LPVOID lparam)
{
     static int first_warn = TRUE;
     ISNS_Msg    *p_msg = NULL;

     memset((char *)&frag_md, 0, sizeof(ISNS_Msg_Descp));
             
     __DEBUG (isns_main_debug & 1, "Starting SNSReceiveMain thread.");
     /*
      * Wait for a message to arrive on the socket.
      * Forward the received message to the iSNS core
      * task for processing.
      */
      for (;;)
      {
         extern int pauseFlag;
         if (pauseFlag)
            return (-1);

          if (( SNSGetMsg (&md)) != ERROR )
          {
               p_msg = &md.msg;
               SNS_SaveRecentMsg(&md);

               /* Check the flags */

               /* If First PDU set */
               if (p_msg->hdr.flags & ISNS_FLAG_FIRST_PDU)
               {
                   if (awaiting_fragment)
                   {
                      /* got another while waiting: drop previous message */
                      sns_recv_num_dropped++;
                      __LOG_ERROR("dropped frame %d", sns_recv_num_dropped);
                      awaiting_fragment = 0;
                      frag_len = 0;
                   }

                   /* save header */
                   memcpy((char *)&frag_md, (char *)&md, 
                          sizeof(SNS_Msg_Cb) + sizeof(SNS_Msg_Hdr));
               }
               else
               {
                   /* xid and version must match prev */
                   if ((frag_md.msg.hdr.xid != p_msg->hdr.xid) ||
                       (frag_md.msg.hdr.version != p_msg->hdr.version))
                   {
                      sns_recv_num_dropped++;

                      __LOG_ERROR("dropped frame %d", sns_recv_num_dropped);
                      awaiting_fragment = 0;
                      frag_len = 0;
                      memset((char *)&frag_md, 0, 
                             sizeof(SNS_Msg_Cb) + sizeof(SNS_Msg_Hdr));
                      continue;
                   }
               }

                /* copy into buffer */
                memcpy((char *)&frag_md.msg.payload + frag_len, 
                       (char *)&p_msg->payload, p_msg->hdr.msg_len);
                awaiting_fragment++;
                frag_len += p_msg->hdr.msg_len;

                if (!(p_msg->hdr.flags & ISNS_FLAG_FIRST_PDU))
                   continue;

                /* got all fragments, copy back into msg buffer */
                memcpy((char *)&md, (char *)&frag_md, 
                       frag_len + sizeof(SNS_Msg_Cb) + sizeof(SNS_Msg_Hdr));
                md.msg.hdr.msg_len = frag_len;

                /* clear all frag variables */
                memset((char *)&frag_md, 0,
                       frag_len + sizeof(SNS_Msg_Cb) + sizeof(SNS_Msg_Hdr));
                awaiting_fragment = 0;
                frag_len = 0;

                if ((p_msg->hdr.version > SNS_VERSION) && (first_warn))
                {
                   /*
                    * SNS_WARN(SNS_VERSION_WARN, 
                    *          "version mismatch (current, new)\n",
                    *          SNS_VERSION, p_msg->hdr.version, 0);
                    */
                   __LOG_WARNING("NOTE: Newer version (v %d) of iSNS detected in the",
                          p_msg->hdr.version);
                   __LOG_WARNING("network.  Current version is v %d.", 
                          (int)SNS_VERSION);
                   first_warn = FALSE;
                }

                md.cb.sender = ISNS_REMOTE;
                if (SendIPCMessage(SNS_EP, (void *)&md, sizeof(SNS_Msg_Cb) + 
                                   sizeof(SNS_Msg_Hdr) + p_msg->hdr.msg_len, 
                                   IPC_MSG_PRI_HI) == ERROR)
                {
                   __LOG_ERROR ("%s %d: Error forwarding fsm message (xid %d)", 
                              __FILE__, __LINE__, p_msg->hdr.xid);
                }
               } 
               else if (isns_role != ISNS_PRIMARY_SERVER)
               {
                   if (SendIPCMessage(SNS_EP, (void *)&md, 
                        sizeof(SNS_Msg_Cb) + sizeof(SNS_Msg_Hdr) + 
                                      p_msg->hdr.msg_len, 0) == ERROR)
                   {
                      __LOG_ERROR ("%s %d: Error forwarding res message (xid %d)",
                              __FILE__, __LINE__, p_msg->hdr.xid);
                   }
                  else 
                  {
                      sns_recv_num_cli_fwd++;
                      sns_recv_num_dropped++;
                      __LOG_ERROR("dropped frame %d", sns_recv_num_dropped);
                  }
               }
               else if (isns_role == ISNS_PRIMARY_SERVER)
               {
                   if (SendIPCMessage(SNS_EP, (void *)&md, 
                       sizeof(SNS_Msg_Cb) + sizeof(SNS_Msg_Hdr) + 
                       p_msg->hdr.msg_len, 0) == ERROR)
                   {
                      __LOG_ERROR ("%s %d: Error forwarding op message (xid %d)", 
                              __FILE__, __LINE__, p_msg->hdr.xid);
                   } 
                   else 
                   {
                     sns_recv_num_dropped++;
                     __LOG_ERROR("dropped frame %d", sns_recv_num_dropped);
                   }
       }
       else
       {
          __LOG_ERROR ("Socket Error");
               taskDelay(1*sysClkRateGet());
           }
      }

      return (-1);
}

static void 
SNS_SaveRecentMsg(ISNS_Msg_Descp *p_md)
{
   static int index = 0;
   int  display_debug;
   char dot_not_addr[INET_ADDR_LEN];

   inet_ntoa_b(p_md->cb.sock.addr.sin_addr, dot_not_addr);
   display_debug = 0;
   if (sns_recv_debug == 1)
      display_debug = (p_md->msg.hdr.type != sns_recv_msg_filter);
   else if (sns_recv_debug == 2)
      display_debug = (p_md->msg.hdr.type == sns_recv_msg_filter);

   __DEBUG (display_debug, "recv msg %d, len %d, xid %d from %s",
            p_md->msg.hdr.type, p_md->msg.hdr.msg_len, 
            p_md->msg.hdr.xid, dot_not_addr);

   if ((display_debug == 0) ||
       (p_md->msg.hdr.type == sns_recv_msg_filter))

      return;

#ifdef SNS_LINUX
   sns_recv_msg_array[index][0] = (long double)time ((time_t*) 0);
#else
   sns_recv_msg_array[index][0] = tickGet();
#endif

   sns_recv_msg_array[index][1] = 
     (int)p_md->cb.sock.addr.sin_addr.s_addr;
   memcpy(&sns_recv_msg_array[index][2], &p_md->msg, 
          ARRAY_WIDTH*sizeof(int));
   index++;
   index = index % ARRAY_DEPTH;
}

void SNSReceiveMain_dump(int num_display)
{
   int ii, jj;

   __LOG_INFO("SNSReceiveMain num dropped %d", sns_recv_num_dropped);
   __LOG_INFO("SNSReceiveMain num pri res %d", sns_recv_num_pri_res);
   __LOG_INFO("SNSReceiveMain num cli fwd %d", sns_recv_num_cli_fwd);

   __LOG_INFO("Msgs received");
   if ((num_display < 0) || (num_display > ARRAY_DEPTH))
      num_display = ARRAY_DEPTH;
   for (ii = 0; ii < num_display; ii++)
   {
      __LOG_INFO("time %08x, src %08x", sns_recv_msg_array[ii][0],
             sns_recv_msg_array[ii][1]);
      for (jj = 0; jj < ARRAY_WIDTH; jj++)
      {
         __LOG_INFO("%08x ", sns_recv_msg_array[ii][2 + jj]);
         if (jj == 7)
            __LOG_INFO("\n\t");
      }
   }
}

int
sns_recv_debug_help()
{
   __LOG_INFO("sns_recv_debug represents the debugging level for the iSNS");
   __LOG_INFO("  socket receive messages.  Use in conjuction with sns_recv_msg_filter");
   __LOG_INFO("  There is also SNSReceiveMain_dump x, which shows dropped");
   __LOG_INFO("  msg stats and the last x (up to %d) msg headers received",
          ARRAY_DEPTH);
   __LOG_INFO("  when 1 : displays received msgs (except msg filter)");
   __LOG_INFO("  when 2 : displays only msg filter msgs");

   return (sns_recv_debug);
}
