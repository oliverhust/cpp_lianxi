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
 * This file contains source code for implementing the
 * main processing loop for the SoIP service task.
 *
 */
#ifdef SNS_LINUX
#include <sched.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include "iSNSLinux.h"
#endif

#include "iSNS.h"
#include "iSNScomm.h"
#include "iSNSmsg.h"
#include "iSNSqueue.h"
#include "iSNSfsm.h"
#include "iSNSbuffer.h"
#include "iSNSipc.h"
#include "iSNSparse.h"
#include "iSNStbl.h"
#include "iSNSdb.h"
#include "iSNSreg.h"
#include "iSNSquery.h"
#include "iSNSesi.h"
#include "iSNSscn.h"
#include "iSNSdebug.h"

ISNS_Entity isns_role = 0;

/*
 * flags to control debug trace output of iSNS
 */
int isns_pdu_debug = 0;
int isns_main_debug = 0;         /* SNSMain misc debugging */
int sns_hb_debug = 0;           /* heartbeat debugging */
int sns_cb_debug = 0;           /* callback debugging */
int sns_scn_debug = 0;          /* state change debugging */
int sns_rsync_debug = 0;        /* resync debugging. 1-brief. 2-detail */
int sns_hb_counter = 0;
int sns_cb_msg_filter = 0;
int sns_scn_msg_filter = 0xf;   /* init non-zero since 0 is FLOGI */
int sns_request_retries = SNS_REQUEST_RETRIES;
int sns_request_timeout = 0;

extern int sns_fsm_timeout;

/*
 * Timer used for retransmitting iSNS requests and for timing out 
 * requests awaiting a response.
 */
#ifdef SNS_LINUX
pthread_mutex_t sns_request_timer;
pthread_cond_t request_cond = PTHREAD_COND_INITIALIZER;
extern pthread_mutex_t sns_fsm_timer;
extern pthread_mutex_t sns_resync_timer;
extern pthread_mutex_t sns_esi_timer;  
#else
DWORD junk;
DWORD request_id;
void *sns_request_timer;
#endif

/*
 * the current main message descriptor being processed
 */
static ISNS_Msg_Descp main_md;

/*
 * client request msg
 */
static ISNS_Msg_Descp query_md;

/*
 * Local function prototypes.
 */
static void regCallback(ISNS_Msg_Descp* p_md);
static void SNSProcessRequest (ISNS_Msg_Descp *);
void SNSReqTimeoutHdlr (void);
int SNSExecCallback (ISNS_Msg_Descp *, int status);
void SNSCheckTxQueue ();
void SNSCheckInitTxQueue (ISNS_Msg_Descp * p_md);
int TCP_RecvMain();
static int ISNSdbExec( ISNS_Msg_Descp *p_md, ISNS_Msg_Descp *p_rsp_md);
extern void ISNSNotReady ();

/********************************************************************
 * Function Name:    SNSMain
 *
 * Synopsis:         void SNSMain();
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
 ********************************************************************/
int
SNSMain (ISNS_Entity role)
{
   int rc;
   pthread_t athread;
   ISNS_Msg_Descp *p_md;

   __LOG_INFO ("started IETF iSNS Open Source, v%s.", ISNS_VERSION);
   isns_role = ISNS_SECONDARY_SERVER;

   if (pthread_create (&athread, NULL, (void*) TCP_RecvMain, NULL) != 0)
        perror("Creating TCP_RecvMain");

   if (SNSInit (role) == ERROR)
        taskDelete (taskIdSelf ());

#ifdef SNS_LINUX
   {
        int res;
    res = pthread_mutex_init (&sns_request_timer, NULL);
    /* Create AND initialize the request condition */
    if (res  != 0)
    {
        __LOG_ERROR ("Mutex initialization failed!");
        exit (-1);
    }
    res = pthread_mutex_init (&sns_fsm_timer, NULL);
    if (res  != 0)
    {
        __LOG_ERROR ("Mutex initialization failed!");
        exit (-1);
    }
    res = pthread_mutex_init (&sns_esi_timer, NULL);
    if (res  != 0)
    {
        __LOG_ERROR ("Mutex initialization failed!");
        exit (-1);
    }
    res = pthread_mutex_init (&sns_resync_timer, NULL);
    if (res  != 0)
    {
        __LOG_ERROR ("Mutex initialization failed!");
        exit (-1);
    }

    pthread_t hdle;
    rc =(int)pthread_create (&hdle, NULL, (void *)SNSReqTimeoutThread, NULL);
    if (rc != 0){
        __LOG_ERROR ("Thread creation failed.");
        exit (-1);
    }   
   }
#else
   {
      /*
       * Create the sns request timer
      */
      sns_request_timer = CreateWaitableTimer (NULL, FALSE, NULL);
      request_pid = CreateThread (0, 0, SNSReqTimeoutThread, &junk, 0, &request_id);
   }
#endif

   /*
    * Wait for a message to arrive.
    */
   p_md = &main_md;

   for (;;)
   {
      if (SUCCESS == ReceiveIPCMessage (SNS_EP, (void *) p_md, sizeof (ISNS_Msg_Descp), 0))
      {
            SNSConvertPayloadNTOH (p_md);
            SNSProcessRequest (p_md);
      }

      if (sns_request_timeout)
         SNSCheckTxQueue ();

      if (sns_fsm_timeout)
         SNSCheckInitTxQueue (p_md);
   }

   return (SUCCESS);
}                               /* End SNSMain */


/********************************************************************
 * Function Name: SNSProcessRequest
 *
 * Synopsis:      void SNSProcessRequest(p_md);
 *
 * Arguments:     p_md - pointer to the received iSNS message descriptor.
 *
 * Description:   Validate and process a received iSNS message.
 *
 *
********************************************************************/
static void
SNSProcessRequest (ISNS_Msg_Descp * p_md)
{

   struct in_addr source_ip;
   struct in_addr null_addr = { 0 };
   char dot_not_addr[INET_ADDR_LEN];
   ISNS_Msg_Descp *tcb_md;
   ISNS_FUNC_ID msg_type;
   ISNS_Msg *msg;
   int errorCode;
   int timeout;
   ISNS_Msg_Descp  *unparsed_md = NULL;
   ISNS_Msg_Descp  *p_req_md;

   /* make a copy to forward to the primary */
   if (isns_role != ISNS_PRIMARY_SERVER)
   {
     unparsed_md = (ISNS_Msg_Descp *) ISNSAllocBuffer(sizeof(ISNS_Msg_Cb) + 
                    sizeof(ISNS_Msg_Hdr) + p_md->msg.hdr.msg_len);
     if (unparsed_md == NULL)
     {
        __LOG_WARNING("ProcessRequest out of buffs (size:%i)",
                 (int)(sizeof(ISNS_Msg_Cb) + sizeof(ISNS_Msg_Hdr) + 
                 p_md->msg.hdr.msg_len));
        return;
     }
     memcpy(unparsed_md, p_md, sizeof(ISNS_Msg_Cb) + 
            sizeof(ISNS_Msg_Hdr) + p_md->msg.hdr.msg_len);
   }

   msg = &p_md->msg;
   msg_type = msg->hdr.type;
   tcb_md = NULL;

   source_ip.s_addr = p_md->cb.sock.addr.sin_addr.s_addr;
   inet_ntoa_b (source_ip, dot_not_addr);

   __DEBUG(isns_main_debug &1,"ProcessRequest type:0x%x-%s",msg_type,FuncIDText(msg_type));

   switch (msg_type)
   {
   case ESI_TIMEOUT_EVENT:
      /*ISNS_SendESI (p_md);*/
      ISNS_SendESI ();
      break;
   case ISNS_ESI_RSP:
      ISNS_ProcessESI (p_md);
      break;
   case ISNS_SCN_RES:
      __LOG_INFO ("Received SCN response.");
      break;
   case FSM_TIMEOUT_EVENT:
      if (sns_fsm_timeout)
      {
         SNSCheckInitTxQueue (p_md);

         ISNS_CheckEntities ();
      }
      break;

   case REQ_TIMEOUT_EVENT:
      {
         __DEBUG (isns_main_debug & 32,"Received a REQ_TIMEOUT_EVENT");
         /* SNSCheckTxQueue (); */
      }
      break;
   case ISNS_HEART_BEAT:
      break;
   case ISNS_REG_DDS_REQ:
   case ISNS_REG_DD_REQ:
   case ISNS_SCN_EVENT:
   case ISNS_REG_DEV_ATTR_REQ:
   case ISNS_DEREG_DEV_REQ:
   case ISNS_DEREG_DD_REQ:
   case ISNS_DEREG_DDS_REQ:
   case ISNS_SCN_DEREG_REQ:
   case ISNS_SCN_REG_REQ:
   case ISNS_DEV_GET_NXT_REQ:
   case ISNS_DDS_GET_NXT_MEMBER_REQ:
   case ISNS_DD_GET_NXT_ISCSI_MEMBER_REQ:
   case ISNS_ENTITY_GET_NXT_PORTAL_REQ:
   case ISNS_DEV_ATTR_QRY_REQ:
   {
        extern struct sockaddr_in server_sock;

        __DEBUG (isns_main_debug & 16, (Received %s), FuncIDText((short)msg_type));
        if (isns_pdu_debug & 1)
        {
           ParsePDU((char *)&p_md->msg);
        }
        /* register callback functions for local operations */
        if (p_md->cb.sender == DEVICE_MGMT_EP)
            regCallback(p_md);

        memset(&query_md, 0, sizeof(query_md));

        errorCode = SUCCESS;
        if (isns_role == ISNS_PRIMARY_SERVER)
        {
           errorCode = ISNSdbExec(p_md, &query_md);
        }

         if (errorCode == SUCCESS)
         {
            __DEBUG (isns_main_debug & 16, (%s), 
                     "Attribute Registered in Local iSNS Database");

            if ( msg_type == ISNS_SCN_REG_REQ && 
                 p_md->cb.sender == DEVICE_MGMT_EP )
            {
               ISNS_RegisterSCNCallback( p_md );
            }
            /*
             * If running as a client then register the attributes with
             * the primary server.
             */
             if (isns_role == ISNS_CLIENT || 
                 isns_role == ISNS_SECONDARY_SERVER)
             {
                 inet_ntoa_b (server_sock.sin_addr, dot_not_addr);
                 __DEBUG(isns_main_debug & 16, (%s, %s 0x%xu), 
                         "The server socket is: ", dot_not_addr, 
                         (uint32_t)server_sock.sin_addr.s_addr);
                            
                 if ( server_sock.sin_addr.s_addr == 0x0 )
                 {
                     p_md->cb.init_rmsg = 1;
                     __DEBUG (isns_main_debug & 16, "(%s %d)", 
                              "Set the init reg msg", 
                              p_md->cb.init_rmsg); 
                 }
                /*
                 * Enqueue the request pending a response, start a 
                 * response timer and forward the request to the server.
                 */
                 __DEBUG (isns_main_debug & 16, "(%s %d)", 
                          "Forwarding reg request: XID is: ",
                          msg->hdr.xid);
                 /*
                  * set initial msg control block time-to-live
                  */
                 unparsed_md->cb.ttl = sns_request_retries;
                 ISNSEnqReq ( SNS_TRANSACTION_QUEUE, unparsed_md );
                 if ( ISNSSendMsg2Server( &unparsed_md->msg ) == SUCCESS ) {
                     __DEBUG (isns_main_debug & 16, "(%s %d)", 
                              "Message forwarded : XID is: ",
                              unparsed_md->msg.hdr.xid);
#ifndef SNS_LINUX
                     wdCancel(sns_request_timer);
#endif

#ifdef SNS_LINUX
                     timeout = (int)((ISNSQHead(SNS_TRANSACTION_QUEUE))->cb.resp_timeout - time ((time_t*) 0) );
#else
                     timeout = (int)((ISNSQHead(SNS_TRANSACTION_QUEUE))->cb.resp_timeout - tickGet());
#endif
                     __DEBUG (isns_main_debug & 16, "(%s %d)", 
                              "Device registration timeout value", 
                              timeout);
                     if (timeout < 1)
                        timeout = 1;
#ifndef SNS_LINUX
                     wdStart(sns_request_timer, 
                             timeout,
                             SNSReqTimeoutHdlr,
                             NULL);
#endif
                 }
                 else {
                   /* Error Case while forwarding to primary */
                   ISNSCommUpdate(&null_addr);
                   ISNSNotReady();
                   /*
                    * send a response back to the originator 
                    * of the request.
                    */
                    msg->hdr.type  = 0x8000 | msg_type;
                    msg->hdr.flags = 0;
                    msg->hdr.msg_len = 4;
                    msg->payload.errorCode = ISNS_UNKNOWN_ERR;

                    ISNSDeqReq (SNS_TRANSACTION_QUEUE, msg->hdr.xid, 0);
                    SNSExecCallback( p_md, ISNS_UNKNOWN_ERR );
                    if ( unparsed_md )
                       ISNSFreeBuffer( (char*)unparsed_md );
                 }                   
                 return;
             }
         }
         else
         {
            /* Error registering */
            __DEBUG (isns_main_debug & 16, (Error: %s), FuncIDText((short)msg_type));
         }

        /*
         * Send a response back to the originator of the request
         */
         __DEBUG (isns_main_debug &16,Send response back to originator);
         msg->hdr.type  = 0x8000 | msg_type;
         msg->hdr.flags = 0;

         /* Copy the response message back */
         memcpy (&p_md->msg.payload, &query_md.msg.payload,
                 query_md.msg.hdr.msg_len);
         p_md->msg.hdr.msg_len = query_md.msg.hdr.msg_len==0 ? 4:query_md.msg.hdr.msg_len;
         p_md->msg.hdr.flags = 0;
         p_md->msg.payload.errorCode = errorCode;

         if (p_md->cb.sender == DEVICE_MGMT_EP)
         {
            SNSExecCallback(p_md, errorCode);
         }
         else
         {
            /*
             * Send the registration response back to the client.
             */
            __DEBUG (isns_main_debug &16,Call ISNSSendMsg);
            ISNSSendMsg(p_md);
         }

         if (unparsed_md)
            ISNSFreeBuffer((char*)unparsed_md);
         __DEBUG (isns_main_debug & 0x400, (Process Request Complete));
   }
   break;

   case ISNS_SCN:
   {
       __DEBUG (isns_main_debug & 16, (Received %s), FuncIDText((short)msg_type));
       
       errorCode = ISNSdbExec(p_md, &query_md);

       /* Send back a response */
       memcpy(&p_md->msg.payload, &query_md.msg.payload, query_md.msg.hdr.msg_len);
       p_md->msg.hdr.msg_len = query_md.msg.hdr.msg_len;
       p_md->msg.hdr.type = ISNS_SCN_RES;
       p_md->msg.payload.errorCode = errorCode;

       ISNSSendMsg(p_md);

       break;
   }
   case ISNS_REG_DEV_ATTR_RES:
   case ISNS_DEV_ATTR_QRY_RES:
   case ISNS_DDS_GET_NXT_MEMBER_RES:
   case ISNS_DD_GET_NXT_ISCSI_MEMBER_RES:
   case ISNS_ENTITY_GET_NXT_PORTAL_RES:
   case ISNS_DEV_GET_NXT_RES:
   case ISNS_DEREG_DEV_RES:
   case ISNS_SCN_REG_RES: 
   case ISNS_SCN_DEREG_RES:
   case ISNS_SCN_EVENT_RES:
   case ISNS_REG_DD_RES:
   case ISNS_REG_DDS_RES:
   case ISNS_DEREG_DD_RES:
   case ISNS_DEREG_DDS_RES:
   {
       __DEBUG (isns_main_debug & 16, (Received %s), FuncIDText((short)msg_type));
       __DEBUG (isns_main_debug & 16, (Received iSNS response for xid %d), 
                msg->hdr.xid);
      /*
       * Remove the corresponding request from the request queue
       */
       p_req_md = ISNSDeqReq (SNS_TRANSACTION_QUEUE, msg->hdr.xid, 0);

       if (p_req_md != NULL)
       {
           __DEBUG (isns_main_debug & 16, (%s %d qid %d), 
                    "Received transaction msg id : ", msg->hdr.xid,
                    p_req_md->msg.hdr.xid);

           /* 
            * Check if this is and async or sync operation. Send back the
            * appropriate response.
            */
           if ((tcb_md = ISNSQHead(ISNS_CALLBACK_QUEUE)) != NULL)
           {
               if (msg->payload.errorCode == ISNS_NO_ERR)
               {
                   SNSExecCallback (p_md, SUCCESS);
               }
               else
               {
                   SNSExecCallback (p_md, msg->payload.errorCode);
               }
           }
          /*
           * Free up buffer resources being used by the queued request
           */
           ISNSFreeBuffer((char *)p_req_md);
       }
      /*
       * release the response message buffer
       */
       if (unparsed_md)
          ISNSFreeBuffer((char*)unparsed_md);
   }
   break;

   default:
      __LOG_WARNING("Received unknown message type %#x from %s", msg_type, dot_not_addr);

      break;

   }                            /* switch - msg_type */
}


/********************************************************************
********************************************************************/
void
SNSReqTimeoutHdlr (void)
{
   sns_request_timeout = 1;
}

/********************************************************************
 * Function Name:    SNSExecCallback
 *
 * Synopsis:         void SNSExecCallback();
 *
 * Description:      SNSExecCallback executes a users callback.
 *
 * Return value:     SUCCESS or ERROR.
 * 
********************************************************************/
int
SNSExecCallback (ISNS_Msg_Descp * p_md, int status)
{
   uint32_t xid;
   ISNS_Msg_Descp *tcb_md;
   int rval;
   int display_debug;
   ISNS_ATTR_VALS_CB *attr_vals_cb;

   /*
    * Exec the appropriate callbacks.
    */
   xid = 0;
   rval = ERROR;
   tcb_md = NULL;
   attr_vals_cb = NULL;

   if (p_md != NULL)
   {
      xid = p_md->msg.hdr.xid;
      tcb_md = ISNSDeqReq (ISNS_CALLBACK_QUEUE, (uint16_t)xid, 0);
      /*
       * Validate the callback.
       */

      display_debug = 0;
      if (sns_cb_debug == 1)
         display_debug = (sns_cb_msg_filter != p_md->msg.hdr.type);
      else if ((sns_cb_debug == 2) || (sns_cb_debug == 3))
         display_debug = (sns_cb_msg_filter == p_md->msg.hdr.type);

      switch (p_md->msg.hdr.type)
      {
      default:
         {
            /* 
             * These are requests that never went to the primary,
             * so free the callback buffer.
             */
          __DEBUG (sns_cb_debug, (Registration response:Unknown message));
            if (tcb_md != NULL)
               rval = SUCCESS;
         }
      }
   }
   if (rval == SUCCESS)
   {
      tcb_md->msg.payload.rcb_req.func_val.qfunc = NULL;
      ISNSFreeBuffer ((char *) tcb_md);
      if (attr_vals_cb != NULL)
      {
         ISNSFreeBuffer ((char *) attr_vals_cb->attr_val_sptr);
         ISNSFreeBuffer ((char *) attr_vals_cb);
      }
   }
   return (rval);
}

/********************************************************************
********************************************************************/
void
SNSCheckTxQueue ()
{
   ISNS_Msg_Descp *p_md, *tcb_md;
   int timeout = 1;
   uint32_t xid;

   sns_request_timeout = 0;
   /*
    * Release all the message that may have been held in the tx queue.
    */
   while ((p_md = ISNSQHead (SNS_TRANSACTION_QUEUE)) != NULL)

   {

      if (p_md->cb.ttl > 0)
      {

         /* retry sending all timeout out msgs */
#ifdef SNS_LINUX
         while ((p_md) && ((unsigned) p_md->cb.resp_timeout < (long 
double)time ((time_t*) 0) ))
#else
         while ((p_md) && ((unsigned) p_md->cb.resp_timeout < tickGet ()))
#endif
         {

            __DEBUG (isns_main_debug & 32, (%s % d % d),
                     "iSNS retrying to forward msg, xid",
                     p_md->msg.hdr.type, p_md->msg.hdr.xid);
            if (ISNSSendMsg2Server (&p_md->msg) == SUCCESS)
#ifdef SNS_LINUX
               p_md->cb.resp_timeout = (long double)time ((time_t*) 0) + 
timeout;
#else
               p_md->cb.resp_timeout = tickGet () + timeout;
#endif
            /* decrement ttl */
            p_md->cb.ttl--;

            p_md = p_md->cb.next;

         }

         if (timeout < 1)
            timeout = 1;

         return;
      }

      /*
       * Remove the message from the request queue.
       */
      tcb_md = ISNSQHead (ISNS_CALLBACK_QUEUE);
      xid = p_md->msg.hdr.xid;
      p_md = ISNSDeqReq (SNS_TRANSACTION_QUEUE, (uint16_t)xid, 0);
      __LOG_WARNING ("retry timeout (msg id, xid)",
                p_md->msg.hdr.type, p_md->msg.hdr.xid);

      /*
       * Send a response back to the originator of the request
       */
      p_md->msg.hdr.type = SNSParseTransOpcode (p_md->msg.hdr.type);
      p_md->msg.hdr.flags = SOIP_OPERATION_FAILURE;

      __DEBUG (isns_main_debug & 32, (%s %d), "Checking the transaction: ",
               xid);
      /* 
       * Check if this is an async or sync operation. Send back the
       * appropriate response.
       */
      if (tcb_md != NULL)
      {
         SNSExecCallback (p_md, ERROR);
      }
      /* release the message buffer after sending response */
      ISNSFreeBuffer ((char *) p_md);
   }

}

/********************************************************************
********************************************************************/
void
SNSCheckInitTxQueue (ISNS_Msg_Descp * p_md)
{
   SNS_FSM_Event event;

   sns_fsm_timeout = 0;

   event = SNS_HEARTBEAT_TIMEOUT;
   SNSProcFSM (event, NULL);
}

/********************************************************************
********************************************************************/
static void regCallback(ISNS_Msg_Descp* p_md)
{
    ISNS_Msg_Descp   *reg_callback;
    /*
     * This message is small, so we only need to allocate
     * from the smaller buffer pool
     */
    reg_callback = (ISNS_Msg_Descp*)ISNSAllocBuffer(sizeof(ISNS_Msg_Descp));
    if (reg_callback == NULL)
    {
       __LOG_WARNING("regCallback out of buffs (size%i)",
                (int)sizeof(ISNS_Msg_Descp) );
       return;
    }
    memcpy(&(reg_callback->cb), &(p_md->cb), sizeof(p_md->cb));
    reg_callback->msg.hdr.xid     = p_md->msg.hdr.xid;
    reg_callback->msg.hdr.type    = p_md->msg.hdr.type;
    reg_callback->msg.hdr.msg_len = (uint16_t) sizeof (p_md->cb.callback_func);
    reg_callback->msg.hdr.flags   = 0;
    reg_callback->msg.payload.rcb_req.func_val.rfunc = p_md->cb.callback_func.reg_func; 
    ISNSEnqReq (ISNS_CALLBACK_QUEUE, reg_callback);
}

/********************************************************************
********************************************************************/
static int
ISNSdbExec( ISNS_Msg_Descp *p_md, ISNS_Msg_Descp *p_rsp_md)
{
   int errorCode = ISNS_UNKNOWN_ERR;

   if (!p_md)
      return ( ISNS_UNKNOWN_ERR );

   switch ( p_md->msg.hdr.type )
   {
      case ISNS_SCN_REG_REQ:
      {
          errorCode = ISNS_RegisterSCN (p_md, &p_rsp_md->msg);
          break;
      }
      case ISNS_SCN_DEREG_REQ:
      {
          errorCode = ISNS_DeRegisterSCN (p_md, &p_rsp_md->msg);
          break;
      }
      case ISNS_REG_DDS_REQ:
      {
          errorCode = ISNSdbAddAttrDDS (p_md, &p_rsp_md->msg);
          break;
      }
      case ISNS_REG_DD_REQ:
      {
          errorCode = ISNSdbAddAttrDD ( p_md, &p_rsp_md->msg );
          break;
      }
      case ISNS_SCN_EVENT:
      {
          errorCode = ISNS_Process_SCN_Event ( &p_md->msg );
          break;
      }
      case ISNS_REG_DEV_ATTR_REQ:
      {
         errorCode = ISNSdbAddAttr (p_md, &p_rsp_md->msg);
         break;
      }
      case ISNS_DEREG_DEV_REQ:
      {
         errorCode = ISNSdbRemoveAttr( p_md, &p_rsp_md->msg);
         break;
      }
      case ISNS_DEREG_DDS_REQ:
      {
         errorCode = ISNSdbRemoveAttrDDS( p_md, &p_rsp_md->msg );
         break;

      }
      case ISNS_DEREG_DD_REQ:
      {
         errorCode = ISNSdbRemoveAttrDD ( p_md, &p_rsp_md->msg );
         break;
      }
      case ISNS_SCN:
      {
          errorCode = ISNS_Process_SCN(p_md, &p_rsp_md->msg);
          break;
      }
      case ISNS_DEV_ATTR_QRY_REQ:
      {
         errorCode = ISNSdbGetAttr ( p_md, &p_rsp_md->msg);
         break;
      }
      case ISNS_DEV_GET_NXT_REQ:
      {
         errorCode = ISNSGetNextAttr ( p_md, &p_rsp_md->msg);
         break;
      }
      case ISNS_DDS_GET_NXT_MEMBER_REQ:
      {
         errorCode = ISNSGetNextDDSMember ( p_md, &p_rsp_md->msg);
         break;
      }
      case ISNS_DD_GET_NXT_ISCSI_MEMBER_REQ:
      {
         errorCode = ISNSGetNextDDIscsiMember ( p_md, &p_rsp_md->msg );
         break;
      }
      case ISNS_ENTITY_GET_NXT_PORTAL_REQ:
      {
         errorCode = ISNSGetNextEntityPortal ( p_md, &p_rsp_md->msg );
         break;
      }
      case ISNS_ESI_RSP:
      {
         errorCode = ISNS_ProcessESI( p_md );
         break;
      }
      default:
      {
         __DEBUG (1, (Error Unknown msg %d), p_md->msg.hdr.type);
         break;
      }
   }

   return ( errorCode );
}

