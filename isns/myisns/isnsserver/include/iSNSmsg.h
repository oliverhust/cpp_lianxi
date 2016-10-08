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


#ifndef _SNSMsg_h_
#define _SNSMsg_h_

#include "iSNSipc.h"
#include "iSNStypes.h"

#define SNS_MAX_ETH_PACKET_SIZE 1440
#define SNS_OLD_MAX_PACKET_SIZE (SNS_MSG_PAYLOAD_SIZE + sizeof(SNS_Msg_Hdr))
#define SNS_MSG_PAYLOAD_SIZE  8*1024

typedef struct sns_msg_hdr {

      uint16_t    version;
      uint16_t    type;
      uint16_t    msg_len;
      uint16_t    flags;
      uint16_t    xid;
      uint16_t    sequence;
           
} SNS_Msg_Hdr, ISNS_Msg_Hdr;

/*
 * Definition of sns_msg_hdr flags
 */
#define SOIP_ENTITY_PRIMARY_SERVER     1
#define SOIP_ENTITY_SECONDARY_SERVER   2
#define SOIP_ENTITY_CLIENT             4
#define SOIP_OPERATION_FAILURE         8

/*
 * Definition of SoIP service message types.
 */
typedef enum {

     /* USED INTERNALLY FOR THE STATE MACHINE */

      START_FSM_MSG_TYPE = 0xF150,
      DISCOVERY,
      CLAIM,
      REQ_TIMEOUT_EVENT,
      FSM_TIMEOUT_EVENT,
      TRANS_FROM_PRIMARY,
      VERIFY_COMM_REQ,
      VERIFY_COMM_RES,
      TRANS_TO_WFCOMM,        
      ESI_TIMEOUT_EVENT,

      END_FSM_MSG_TYPE

} SNS_Msg_Type;

/*
 * Heartbeat message
 */
typedef struct sns_hb_payload {
   IP_Address  ip_addr;  
   uint16_t    udp_port;
   uint16_t    tcp_port;

   uint32_t    interval;        /* heartbeat interval */
   uint32_t    counter;         /* incremental hb counter */

} SNS_Hb, ISNS_Hb;

/*
 * Register Device Attribute message
 */
typedef struct sns_req_payload {

      ISNS_Key  key;

} SNS_Req;

typedef struct SNS_reg_callback_req {
      union {
          void (*rfunc) (uint32_t xid, uint32_t status);
          void (*qfunc) (uint32_t xid, uint32_t status,
                         ISNS_ATTR_VALS_CB *attr_vals_sptr);
      } func_val;

} SNS_Reg_Callback_Req;

/*
 * SOIP service agent message
 */

typedef struct ISNS_msg {

     SNS_Msg_Hdr  hdr;
     union {
          uint32_t errorCode;
          uint32_t error_code;

          /* maximum size of a message */
          uint8_t payload_data[SNS_MSG_PAYLOAD_SIZE];

          /* heartbeat payload */
          SNS_Hb heartbeat;

          /* sns attr request payload */
          SNS_Req sns_req;

          /* callback payload */
          SNS_Reg_Callback_Req rcb_req;

     } payload;
} __attribute__((packed)) ISNS_Msg;




/*
 * iSNS message descriptor block contains control information 
 * pertaining to the message.
 */
typedef struct SNS_msg_cb {

    /*
     * Sender ID
     */
     ISNS_Entity   sender;

    /*
     * Address information associated with the sender
     */
     ISNS_Socket_Ep    sock;

    /*
     * Queue pointers used for enqueuing requests that 
     * require a response from the peer SOIP service
     * entity
     */
     struct ISNS_msg_descp  *next;
     struct ISNS_msg_descp  *prev;

    /*
     * Timeout value (in seconds) associated with an outstanding
     * request.
     */
     long double    resp_timeout;

    /*
     * Number of times this message is to be retransmitted
     * Every time there is response timeout for this message
     * the retransmit count is decremented and the message is
     * retransmitted. When the count reaches zero the message
     * is discarded
     */
     int    ttl;
     /*
      * This is used to indicate the messages are in the 
      * registration queue prior to the server being online.
      */
     int    init_rmsg;
     /*
      * The task identifier. This is used to map an attribute
      * registration request with its callback function.
      */
     int    task_id;
     /*
      * Identifies the status of the transaction.
      */
     int    xid_status;     

     /* Callback function */
     union {
        void (*reg_func) (uint32_t xid, uint32_t status);
        void (*query_func) (uint32_t xid, uint32_t status, ISNS_ATTR_VALS_CB *avcb_ptr);
     } callback_func;

} SNS_Msg_Cb, ISNS_Msg_Cb;



/*
 * Message descriptor used for staging a received message along
 * with address information associated with the sender of the
 * message
 */
typedef struct ISNS_msg_descp {

   /*
    * Control block
    */
    SNS_Msg_Cb  cb;


   /*
    * iSNS message
    */
    ISNS_Msg    msg;


} ISNS_Msg_Descp;

#endif
