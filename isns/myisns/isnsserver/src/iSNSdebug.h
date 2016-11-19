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
 * This header file contains definitions for enabling
 * and disabling debug tracing of iSNS code.
 *
 */

#ifndef _SNSdebug_h_
#define _SNSdebug_h_

#include <syslog.h>

#define LO_DETAIL 1
#define HI_DETAIL 2

#define ERR_MODULE_SNS       0x40000000    /* Name Server */

#define SNS_IPC_ERR          0x0001
#define SNS_BPOOL_ERR        0x0002
#define SNS_DBOPEN_ERR       0x0003
#define SNS_COMM_ERR         0x0004
#define SNS_QINIT_ERR        0x0005
#define SNS_SPAWN_ERR        0x0006
#define SNS_FSM_ERR          0x0007
#define SNS_ROLE_ERR         0x0008
#define SNS_WDTIMER_ERR      0x0009

#define SNS_BUFFER_WARN      0x0100
        /* p1 is buffer size */
#define SNS_SYNCATTR_WARN    0x0200
        /* p1 is response status of sync request */
#define SNS_SYNCFAIL_WARN    0x0300
#define SNS_SYNCMSG_WARN     0x0400
        /* p1 is unexpected msg_type */
#define SNS_GETATTR_WARN     0x0500
        /* p1 is tag, p2 is msg id, p3 is xid */
#define SNS_MSGRETRY_WARN    0x0600
        /* p1 is msg id, p2 is xid */
#define SNS_VERSION_WARN     0x0700
        /* p1 is current version, p2 is other version */

#define SNS_DEBUG_WARN        0x1
#define SNS_DEBUG_ERROR       0x2
#define SNS_DEBUG_FATAL       0x4
#define SNS_DEBUG_INFO        0x8


extern char sns_errmsg[];

extern int daemon_state;

/*
 * Function declaration.
 */
void ISNSDisplay_iFCPNode (SOIP_Ifcp *p_node);
void ISNSDisplay_FCNode (SOIP_Fc_Node *p_node);
void ISNSDisplay_ISCSINode (SOIP_Iscsi *p_node, int detail);
void ISNSDisplay_Entity (SOIP_Entity *p_entity, int detail);
void ISNSDisplay_DDS( SOIP_Dds *p_dds, int detail );
void ISNSDisplay_DD( SOIP_Dd *p_dd, int detail );

void ISNSDisplayMsg        (ISNS_Msg *msg);
void ISNSDisplay_Hdr_Info  (ISNS_Msg *msg); 
void ISNSDisplay_DbData(ISNS_Key *p_key, SOIP_DB_Entry *p_entry, int detail);
void ISNSDisplay_Port_Name (SOIP_Port_Name *p_port, char *p_name);
void ISNSDisplay_Node_Name (SOIP_Node_Name *p_node);
void ISNSDisplay_Port_Id   (SOIP_Port_Id *p_id);
void ISNSDisplay_IPAddress (IP_Address *ip_addr);
void ISNSDisplay_Port      (SOIP_Ifcp *p_port);
void ISNSDisplay_Node      (SOIP_Node *p_node);
void ISNSDisplay_IPAddr    (IP_Address *ip_addr);
void ISNSDisplay_Port_List (SOIP_Port_List *p_list);
void ISNSDisplay_Port_TypeList (SOIP_Port_List *p_list);
void ISNSDisplay_FC4TypeList   (SOIP_Port_List *p_list);
void ISNSDisplay_FC4Type (void *fc4_types, uint32_t attr_len);
void ISNSDisplay_Zone_Tag  (SOIP_Zone *p_zone);
void ISNSDisplay_Zone_Priority (uint32_t *zone_priority);
void ISNSDisplay_Zone      (uint32_t *zone_tag);
void ISNSDisplay_HardAddr  (SOIP_Hard_Addr *hard_addr);
void ISNSDisplay_NodeIPA   (SOIP_Node_IPA *node_ipa);
void ISNSDisplay_Node_SymName (char *sym_name);
void ISNSDisplay_Port_SymName (char *sym_name);
void ISNSDisplay_Port_Type (SOIP_Port_Type *p_type);
void ISNSDisplay_Port_Cos (SOIP_COS *p_cos);
void ISNSDisplay_Portal(SOIP_Portal *p_portal, int level);

void ipaddrchar(void * ipaddrhex, void * ipaddrstr);
char *FuncIDText (uint16_t funcid);
char *isnsTagText (uint32_t tagid);
char *errorText(int errorCode);
void ParsePDU (char *buffer);

#ifndef SNS_DEBUG
#define SNS_DEBUG
#endif


#ifdef SNS_DEBUG

#define __DEBUG(level, format, ...) ((level) ? isns_log(LOG_DEBUG,"%s:%d\t" #format "\n", __FILE__,__LINE__, ##__VA_ARGS__ ) : 0)

#else

#define __DEBUG(level, format, ...)

#endif


#define __LOG_INFO(format, ...) isns_log(LOG_INFO,"%s:%d\t" format "\n", __FILE__,__LINE__,##__VA_ARGS__)
#define __LOG_ERROR(format, ...) isns_log(LOG_ERR,"%s:%d\t" format "\n", __FILE__,__LINE__,##__VA_ARGS__)
#define __LOG_WARNING(format, ...) isns_log(LOG_WARNING,"%s:%d\t" format "\n", __FILE__,__LINE__,##__VA_ARGS__)

#define ISNS_ERROR __LOG_ERROR
#define ISNS_WARNING __LOG_WARNING
#define ISNS_INFO __LOG_INFO

void isns_log(int level, char *format, ...);

#define __ISNS_COPY(a,b,c,d) if (b<d) __LOG_WARNING("dst:%i < src:%i",b,d); isns_copy(a,b,c,d)

int isns_copy (void *dst, int dst_size, void *src, int src_size);

#endif
