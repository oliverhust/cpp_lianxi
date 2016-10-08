#if 0
***********************************************************************
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
#endif

#ifndef _ISNSDEFS_H_
#define _ISNSDEFS_H_

#define ISNS_VER_SIZE               (4)
#define ISNS_DD_FEATURE_BITMAP_SIZE (4)
#define ISNS_ISCSI_TYPE_SIZE       (4)
#define ISNS_COMPANY_OUI_SIZE      (4)
#define ISNS_VENDOR_ID_SIZE        (256)
#define ISNS_NODE_ACTIVE_SIZE      (4)
#define ISNS_DD_ACTIVE_SIZE        (4)
#define ISNS_ENTITY_IDX_SIZE       (4)
#define ISNS_ISCSI_IDX_SIZE        (4)
#define ISNS_PORTAL_IDX_SIZE       (4)
#define ISNS_DD_ISCSI_MEMBER_IDX_SIZE (4)
#define ISNS_ENTITY_PERIOD_SIZE    (4)
#define ISNS_ERROR_CODE_SIZE       (4)
#define ISNS_DDS_STATUS_SIZE       (4)
#define ISNS_DD_ID_SIZE            (4)
#define ISNS_DDS_ID_SIZE           (4)
#define ISNS_IP_SIZE               (16)
#define ISNS_TIMESTAMP_SIZE        (8)
#define ISNS_ESI_INTERVAL_SIZE     (4)
#define ISNS_PORTAL_IP_SIZE        (16)
#define ISNS_PORTAL_PORT_SIZE      (4)
#define ISNS_PORTAL_GROUP_IP_SIZE  (16)
#define ISNS_PORTAL_GROUP_PORT_SIZE (4)
#define ISNS_ESI_PORT_SIZE         (4)
#define PORTAL_NAME_SIZE           (ISNS_PORTAL_PORT_SIZE+ISNS_PORTAL_IP_SIZE)

#define ISNS_DELIMITER_SIZE        (0)
#define ISNS_SCN_BITMAP_SIZE       (4)
#define ISNS_ENTITY_TYPE_SIZE      (4)
#define ISNS_PORT_SIZE             (4)
#define ISNS_PORT_TYPE_SIZE        (4)
#define ISNS_FC_NODE_IPA_SIZE      (8)
#define ISNS_FC_NODE_IP_SIZE       (16)
#define ISNS_PORT_NAME_SIZE        (8)
#define ISNS_IFCP_NODE_NAME_SIZE   (8)
#define ISNS_FC_NODE_NAME_SIZE     (8)
#define ISNS_FABRIC_PORT_NAME_SIZE (8)
#define ISNS_FC_HARD_ADDR_SIZE     (4)
#define ISNS_FC_PORT_IP_SIZE       (16)
#define ISNS_FC_COS_SIZE           (4)
#define ISNS_FC4_TYPE_SIZE         (32)
#define ISNS_FC4_FEATURE_SIZE      (128)
#define ISNS_PORT_ID_SIZE          (4)
#define ISNS_NODE_NAME_SIZE        (8)
#define ISNS_ISCSI_NODE_TYPE_SIZE  (4)
#define PORTAL_SYM_NAME_SIZE       (256)
#define MAX_ENTITY_ID_SIZE         (256)
#define MAX_ISCSI_NODE_ID_SIZE     (256)
#define MAX_ISCSI_ALIAS_SIZE       (256)
#define MAX_SAN_SYM_NAME_SIZE      (256)
#define FC_FEATURE_SIZE            (128)
#define FC_DESCRIPTOR_SIZE         (256)
#define ISNS_SCN_BITMAP_SIZE       (4)
#define ISNS_PORTAL_GROUP_TAG_SIZE (4)

#define DD_SET_ID_SIZE             (4)
#define DD_SET_STATUS_SIZE         (4)
#define DD_ID_SIZE                 (4)
#define DD_IFCP_MEMBER_SIZE        (8)

#define DDS_SYM_NAME_SIZE           (256)
#define DD_SYM_NAME_SIZE            (256)
#define DD_MAX_DD_MEMBER_SIZE       (256)

#define ISNS_DETAIL_SCN             (0x80000000)
#define ISNS_SCN_DETAILED           (0x80000000)

/* New scn types */
#define ISNS_SCN_ALL                (0x1F)
#define ISNS_SCN_MEMBER_ADDED       (0x1)
#define ISNS_SCN_MEMBER_REMOVED     (0x2)
#define ISNS_SCN_OBJ_ADDED          (0x4)
#define ISNS_SCN_OBJ_REMOVED        (0x8)
#define ISNS_SCN_OBJ_UPDATED        (0x10)

#define ISNS_PROT_VER_SIZE          (4)
/* ISCSI TYPES */
#define ISNS_ISCSI_TYPE_TARGET      (0x1)
#define ISNS_ISCSI_TYPE_INITIATOR   (0x2)
#define ISNS_ISCSI_TYPE_CONTROL     (0x4)
#define ISNS_ISCSI_TYPE_REMOTE      (0x8)

#define ISNS_DD_FEATURE_BOOT_LIST_BIT (0x1)

/* ISNS FUNCTION IDs */
typedef enum {

   /* Request */
   ISNS_START_VALID_REQ_FUNC_ID=1,
   ISNS_REG_DEV_ATTR_REQ=ISNS_START_VALID_REQ_FUNC_ID,   /* 1 */
   ISNS_DEV_ATTR_QRY_REQ,
   ISNS_DEV_GET_NXT_REQ,
   ISNS_DEREG_DEV_REQ,
   ISNS_SCN_REG_REQ,                         /* 5 */
   ISNS_SCN_DEREG_REQ,
   ISNS_SCN_EVENT,
   ISNS_SCN,
   ISNS_REG_DD_REQ,
   ISNS_DEREG_DD_REQ,
   ISNS_REG_DDS_REQ,
   ISNS_DEREG_DDS_REQ,
   ISNS_ESI=0xD,
   ISNS_HEART_BEAT,
   ISNS_REQ_SW_ID_REQ=0x11,                          
   ISNS_REL_SW_ID_REQ,
   ISNS_GET_SW_ID_REQ,
   ISNS_DDS_GET_NXT_MEMBER_REQ,
   ISNS_DD_GET_NXT_ISCSI_MEMBER_REQ,
   ISNS_ENTITY_GET_NXT_PORTAL_REQ,

   ISNS_END_VALID_REQ_FUNC_ID,

   /* Response */
   ISNS_START_VALID_RES_FUNC_ID=0x8001,
   ISNS_REG_DEV_ATTR_RES=ISNS_START_VALID_RES_FUNC_ID, /* 0x8001 */
   ISNS_DEV_ATTR_QRY_RES,
   ISNS_DEV_GET_NXT_RES,
   ISNS_DEREG_DEV_RES,
   ISNS_SCN_REG_RES, 
   ISNS_SCN_DEREG_RES,
   ISNS_SCN_EVENT_RES,
   ISNS_SCN_RES,
   ISNS_REG_DD_RES,
   ISNS_DEREG_DD_RES,
   ISNS_REG_DDS_RES,
   ISNS_DEREG_DDS_RES,
   ISNS_ESI_RSP=0x800D,
   ISNS_REQ_SW_ID_RES=0x8011,                          
   ISNS_REL_SW_ID_RES,
   ISNS_GET_SW_ID_RES,
   ISNS_DDS_GET_NXT_MEMBER_RES,
   ISNS_DD_GET_NXT_ISCSI_MEMBER_RES,
   ISNS_ENTITY_GET_NXT_PORTAL_RES,

   ISNS_END_VALID_RES_FUNC_ID,

} ISNS_FUNC_ID;


/* ISNS Tags */
typedef enum {
   ISNS_DELIMITER=0,
   ISNS_START_VALID_TAG=1,

   /* ENTITY tags */
   ISNS_ENTITY_ID=ISNS_START_VALID_TAG,       /* 1 */
   ISNS_ENTITY_TYPE,
   ISNS_MGMT_IP,
   ISNS_TIMESTAMP,
   ISNS_PROT_VER,
   ISNS_ENTITY_PERIOD,                        /* REGISTRATION PERIOD */
   ISNS_ENTITY_IDX,
   ISNS_ENTITY_NEXT_IDX,
   ISNS_ENTITY_ISAKMP=11,
   ISNS_ENTITY_CERT,

   /* PORT tags */
   ISNS_PORTAL_IP=16,                         /* 16 */
   ISNS_PORTAL_PORT,
   ISNS_PORTAL_SYM_NAME,
   ISNS_ESI_INTERVAL,
   ISNS_ESI_PORT,
   ISNS_PORTAL_GROUP,                        /* tag was removed from RFC spec */
   ISNS_PORTAL_IDX=22,
   ISNS_SCN_PORT,
   ISNS_PORTAL_NEXT_IDX,
   ISNS_PORTAL_SECURITY_BITMAP=27,
   ISNS_PORTAL_CERT=31,

   /* ISCSI node tags */
   ISNS_ISCSI_NODE_ID=32,                     /* 32 */
   ISNS_ISCSI_TYPE,
   ISNS_ISCSI_ALIAS,
   ISNS_ISCSI_SCN_BITMAP,
   ISNS_ISCSI_IDX,
   ISNS_WWNN_TOKEN,
   ISNS_ISCSI_NEXT_IDX,
   ISNS_ISCSI_CERT=40,

   /* PORTAL_GROUP tags */
   ISNS_PORTAL_GROUP_ISCSI_NAME=48,
   ISNS_PORTAL_GROUP_IP,
   ISNS_PORTAL_GROUP_PORT,
   ISNS_PORTAL_GROUP_TAG,
   ISNS_PORTAL_GROUP_IDX,
   ISNS_PORTAL_GROUP_NEXT_IDX,

   /* FC */
   ISNS_PORT_NAME=64,                         /* 64 */
   ISNS_PORT_ID,
   ISNS_PORT_TYPE,
   ISNS_PORT_SYM_NAME,
   ISNS_FABRIC_PORT_NAME,
   ISNS_FC_HARD_ADDR,
   ISNS_FC_PORT_IP,
   ISNS_FC_COS,
   ISNS_FC4_TYPE,
   ISNS_FC4_DESC,
   ISNS_FC4_FEATURE,
   ISNS_IFCP_SCN_BITMAP,
   ISNS_IFCP_NODE_CERT=80,
   iSNS_FC4_TYPE_QUERY_KEY=95,
   ISNS_NODE_NAME,
   ISNS_NODE_SYM_NAME,
   ISNS_FC_NODE_IP,
   ISNS_FC_NODE_IPA,
   ISNS_FC_NODE_CERT,

   /* Server specific tags */
   ISNS_VENDOR_ID = 131,
   ISNS_VENDOR_REV,
   ISNS_PRIMARY_VER,
   ISNS_PRIMARY_IP,
   ISNS_PRIMARY_TCP_PORT,
   ISNS_PRIMARY_UDP_PORT,
   ISNS_PRIMARY_MGMT_IP,

   ISNS_COMPANY_OUI = 256,

   /* Nishan vendor specific tags */
   ISNS_SCN_CALLBACK = 257,
   ISNS_DD_ACTIVE,
   ISNS_NODE_ACTIVE,
   ISNS_END_VALID_TAG,

   /* DDS tags */
   ISNS_DDS_ID=2049,
   ISNS_DDS_SYM_NAME,
   ISNS_DDS_STATUS,

   /* DD tags */
   ISNS_DD_ID=2065,
   ISNS_DD_SYM_NAME,
   ISNS_DD_ISCSI_MEMBER_IDX,
   ISNS_DD_ISCSI_MEMBER,
   ISNS_DD_IFCP_MEMBER,
   ISNS_DD_PORTAL_MEMBER_IDX,
   ISNS_DD_PORTAL_IP_ADDR,
   ISNS_DD_PORTAL_TCPUDP,
   ISNS_DD_FEATURE_BITMAP=2078,
   ISNS_DD_NEXT_ID

} ISNS_TAG;

/* ISNS Entity types */
typedef enum {
   NO_PROTOCOL=1,
   ENTITY_TYPE_ISCSI,
   ENTITY_TYPE_IFCP,
} ISNS_ENITITY_TYPES;

/* ISNS Flags */
#define ISNS_FLAG_FIRST_PDU    (0x400)
#define ISNS_FLAG_LAST_PDU     (0x800)
#define ISNS_FLAG_REPLACE_REG  (0x1000)
#define ISNS_FLAG_AUTH         (0x2000)
#define ISNS_FLAG_SND_SERVER   (0x4000)
#define ISNS_FLAG_SND_CLIENT   (0x8000)

/* ISNS Error Codes */
typedef enum {
   ISNS_NO_ERR=0,
   ISNS_UNKNOWN_ERR,
   ISNS_MSG_FMT_ERR,
   ISNS_INVALID_REG_ERR,
   ISNS_ESI_TOO_SHORT,
   ISNS_INVALID_QUERY_ERR,
   ISNS_AUTH_UNKNOWN_ERR,
   ISNS_AUTH_ABSENT_ERR,
   ISNS_AUTH_FAILED_ERR,
   ISNS_NO_SUCH_ENTRY_ERR,
   ISNS_VER_NOT_SUPPORTED_ERR,
   ISNS_INT_BUS_ERR,
   ISNS_BUSY_NOW_ERR,
   ISNS_OPTION_NOT_UNDERSTOOD_ERR,
   ISNS_INVALID_REG_UPD_ERR,
   ISNS_MSG_NOT_SUPPORTED_ERR,
   ISNS_SCN_EVENT_REJECTED_ERR,
   ISNS_SCN_REG_REJECTED_ERR,
   ISNS_SW_ID_NOT_AVAIL,
   ISNS_SW_ID_NOT_ALLOC,
} ISNS_ERROR_CODES;

#define ISNS_SIZEOF_TAG ( sizeof(uint32_t) + sizeof(uint32_t) )

#ifndef PAD4
#define PAD4(a)   ( (a % 4) ? (4 - (a % 4) ) + a: (a+4) )
#endif

#undef SUCCESS
#define SUCCESS  0

#undef ERROR
#define ERROR    1

#ifndef MIN
#define MIN(a,b)   ((a) <= (b) ? (a) : (b))
#endif

#endif


