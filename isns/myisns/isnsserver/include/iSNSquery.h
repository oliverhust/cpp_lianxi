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
 * This file contains definitions related to iSNSreg 
 *
 */

#ifndef _SNSquery_h_
#define _SNSquery_h_

extern int
SNSdbGetAttr (ISNS_Attr **attr_indx, ISNS_Attr **key_indx,
              ISNS_Attr *src_attr, ISNS_Msg_Descp * md);

extern ISNS_Msg_Descp *
SNSdbGetPorts(ISNS_Attr **attr_indx, ISNS_Msg_Descp *);

extern int
SNSdbGetPortName(IP_Address *ip, SOIP_Port_Id *id, SOIP_Port_Name *pn);

int
ISNS_GetDD_ID_from_sym ( char *p_sym );

int
ISNS_GetDDS_ID_from_sym ( char *p_sym );

int
ISNSGetNextEntityPortal( ISNS_Msg_Descp *p_md, ISNS_Msg * p_rspmsg );

int
ISNSGetNextDDIscsiMember( ISNS_Msg_Descp *p_md, ISNS_Msg * p_rspmsg );

int
ISNSGetNextDDSMember( ISNS_Msg_Descp *p_md, ISNS_Msg * p_rspmsg );

int
ISNSGetNextAttr( ISNS_Msg_Descp *p_md, ISNS_Msg * p_rspmsg );

int
IsDDActive(int dd_id, int exclude_dds_id);

int 
IsDDSActive(int dds_id);

int
Get_Active_DD_List (ISNS_LIST *dd_list, ISNS_DD_LIST p_dlist, int flag);

int
ISNSdbProcessISCSIOpAttr (ISNS_Msg * p_msg, ISNS_Attr ** attr_indx,
                          SOIP_Iscsi * p_node);

int
ISNSdbProcessFCNodeOpAttr (ISNS_Msg * p_msg, ISNS_Attr ** attr_indx,
                          SOIP_Fc_Node * p_node);

int
Get_Active_DD_List_From_ISCSI_Node (char *nodename, ISNS_DD_LIST p_dlist,
                                    int flag);

int
Append_Nodes_From_DD_List (ISNS_DD_LIST p_dlist,
                           SOIP_Dd_Member p_ddmem[], uint32_t type);

int
Get_Active_DD_List_From_IFCP_Node (char *nodename, ISNS_DD_LIST p_dlist,
                                   int flag);


int
Get_Active_DD_List_From_ENTITY_Node (char *p_entity_name,
                                     SOIP_Entity * p_entity,
                                     ISNS_DD_LIST p_list, int flag);

int
SNSProcessPortalOpAttr (ISNS_Msg * p_msg, ISNS_Attr ** attr_indx,
                        SOIP_Portal * p_portal);

int
ISNSdbProcessPortOpAttr (ISNS_Msg * p_msg, ISNS_Attr ** attr_indx,
                         SOIP_Ifcp * p_port);

int
ISNSdbProcessEntityOpAttr (ISNS_Msg * p_msg, ISNS_Attr ** attr_indx,
                           SOIP_Entity *p_entity);

int
ISNSdbProcessDDSOpAttr(ISNS_Msg *p_msg, ISNS_Attr **attr_indx, 
               SOIP_Dds *p_dds);

int
ISNSdbProcessDDOpAttr(ISNS_Msg *p_msg, ISNS_Attr **attr_indx, 
               SOIP_Dd *p_dd);

int
ISNSGetNextEntityPortal( ISNS_Msg_Descp *p_md, ISNS_Msg * p_rspmsg );

int
ISNSGetNextDDIscsiMember( ISNS_Msg_Descp *p_md, ISNS_Msg * p_rspmsg );

int
ISNSGetNextDDSMember( ISNS_Msg_Descp *p_md, ISNS_Msg * p_rspmsg );

int
ISNSdbGetAttr ( ISNS_Msg_Descp * p_md, ISNS_Msg * p_rspmsg );

int
Get_Active_DD_List_From_IFCP_Node (char *nodename, ISNS_DD_LIST p_dlist,
                                   int flag);

int
SNSGet_Active_DD_List_From_Src (ISNS_Attr * src_attr,
                                ISNS_DD_LIST p_dlist_src, int flag);

int
Get_Active_DD_List_From_ENTITY_Node (char *p_entity_name,
                                     SOIP_Entity * p_entity,
                                     ISNS_DD_LIST p_list, int flag);

int
Check_Permission (ISNS_Attr *src_attr, int rectype, void *ptr);

#endif
