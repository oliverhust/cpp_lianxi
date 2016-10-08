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

#ifndef _SNSreg_h_
#define _SNSreg_h_

#include <time.h>
#include "iSNSmsg.h"

extern int
SNSdbAddAttr (char **attr_indx, char **key_indx, ISNS_Msg_Descp * p_md);

extern int
SNSDeRegisterSCN (char **attr_indx, char **key_indx, ISNS_Attr * src_attr,
                  ISNS_Msg * rspmsg);

extern STATUS
SNSRemoveAllPortsFromNode(SOIP_Node_Name nn);

extern void
ISNS_CheckEntities (void);

int ISNS_RegisterSCNCallback(ISNS_Msg_Descp* p_md);

int ISNSTouchEntity(ISNS_Attr* src_attr);

int
change_dd_member_active_flag( SOIP_Dd *p_dd );

int
ISNS_DeRegisterSCN ( ISNS_Msg_Descp * p_md, ISNS_Msg * rspmsg);

int
ISNS_RegisterSCN (ISNS_Msg_Descp * p_md, ISNS_Msg * rspmsg);

int
ISNSdbRemoveAttr (ISNS_Msg_Descp * p_md, ISNS_Msg * rspmsg);

int
ISNSdbRemoveAttrDD ( ISNS_Msg_Descp * p_md, ISNS_Msg * p_rspmsg );

int
ISNSdbRemoveAttrDDS ( ISNS_Msg_Descp * p_md, ISNS_Msg * p_rspmsg );

int
ISNSAddSCNEntry(uint32_t type, char *nodename, uint32_t bitmap, int sockfd);

int
ISNSRemoveSCNEntry(uint32_t type, char *nodename);

int
Copy_DD_List(ISNS_LIST *dd_list, ISNS_DD_LIST p_dlist);

int
SNSRemovePortalEntry (SOIP_Portal_Key *portal_key, ISNS_Attr * src_attr,
                      ISNS_Msg * p_rspmsg);

int
ISNSdbAddAttr (ISNS_Msg_Descp * p_md, ISNS_Msg *rspMsg);

int
ISNSdbAddAttrPortal (ISNS_Attr **attr_indx, ISNS_Attr **key_indx, ISNS_Msg_Descp * p_md,
                    ISNS_Msg * p_rspmsg);

int
ISNSdbAddAttrPortalGroup (ISNS_Attr **attr_indx, ISNS_Attr **key_indx, ISNS_Msg_Descp * p_md, ISNS_Msg * p_rspmsg);

int
SNSdbAddAttrPort (ISNS_Attr **attr_indx, ISNS_Attr **key_indx, ISNS_Msg_Descp * p_md,
                    ISNS_Msg * p_rspmsg);
                    
int
SNSdbRemoveAttrPort (ISNS_Attr **key_indx, ISNS_Attr *src_attr, ISNS_Msg_Descp * p_md,
                     ISNS_Msg * rspmsg);

int
AddCert (void **ptr, int *size, void *p_cert, int cert_size);

int
RemoveCert (void **ptr);

int
SNSRemoveIFCPNodeEntry (char *ifcp_node_key, ISNS_Attr * src_attr,
                        ISNS_Msg * p_rspmsg);

int
Activate_DD_Membership (uint32_t type, void *p_node);

int
SNSRemoveFCNodeEntry (char *p_key, ISNS_Msg * p_rspmsg);


int
Check_Overlap_DD (ISNS_DD_LIST p_dlist_src, ISNS_DD_LIST p_dlist_node);


int
SNSRemoveISCSINodeEntry (char *iscsi_node_key, ISNS_Attr * src_attr,
                         ISNS_Msg * p_rspmsg);

int
ISNSdbAddAttrNode (ISNS_Attr **attr_indx, ISNS_Attr **key_indx, ISNS_Msg_Descp * p_md,
                   ISNS_Msg * p_rspmsg);

int
SNSdbAddAttrEntity (ISNS_Attr **attr_indx, ISNS_Attr **key_indx,
                    ISNS_Msg_Descp * p_md, ISNS_Msg * p_rspmsg);

int
Remove_DD_from_ISCSI_Node (SOIP_Dd *p_dd, char *p_node_name);

int
Add_dds_to_dd (SOIP_Dds *p_dds, int dd_id);

int
Remove_DD_from_IFCP_Node (int id, char *p_node_name);

int
Add_DD_to_ISCSI_Node (SOIP_Dd *p_dd, char *p_node_name);

int
Add_DD_Member (SOIP_Dd * p_dd, char *ptr, int p_len, uint32_t type, uint32_t status, uint32_t index);

int
Remove_DD_Member (SOIP_Dd *p_dd, char *ptr, uint32_t type);

int
Add_DD_to_IFCP_Node (int id, char *p_node_name);

int 
Remove_dd_from_dds (SOIP_Dds * p_dds, int id);

int
ISNSdbAddAttrISCSINode (ISNS_Attr **attr_indx, ISNS_Attr **key_indx,
                       ISNS_Msg_Descp * p_md, ISNS_Msg * p_rspmsg);

int
SNSdbRemoveAttrEntity (ISNS_Attr **key_indx, ISNS_Attr *src_attr, ISNS_Msg_Descp * md,
                       ISNS_Msg * rspmsg);

int
ISNSdbRemoveAttrEntityEntry (char *p_entity_id, ISNS_Attr * src_attr,
                            ISNS_Msg * p_rspmsg);


int
ISNSdbAddAttrDDS ( ISNS_Msg_Descp * p_md, ISNS_Msg * p_rspmsg );

int
ISNSdbAddAttrDD ( ISNS_Msg_Descp * p_md, ISNS_Msg * p_rspmsg );

int
ISNSdbProcessDDOpAttr(ISNS_Msg *p_msg, ISNS_Attr **attr_indx, 
               SOIP_Dd *p_dd);

int ISNS_GetNewDDS_ID (void);
int ISNS_GetNewDD_ID (void);

int Check_Unique_DDS_Sym (SOIP_Dds * p_dds, char *p_sym_name);
int Check_Unique_DD_Sym (SOIP_Dd * p_dd, char *p_sym_name);
uint32_t ISNSGetNewEntityIdx(void);
uint32_t ISNSGetNewISCSIIdx(void);
uint32_t ISNSGetNewPortalIdx(void);
int SNSdbRemoveAttrDDS_Entry(int id, ISNS_Attr *src_attr, ISNS_Msg *p_rspmsg);
int SNSdbRemoveDD_Entry(int id, ISNS_Attr *src_attr, ISNS_Msg *p_rspmsg);

int
SNSdbRemoveDD_Member (int id, ISNS_Attr **attr_indx, ISNS_Attr * src_attr,
                          ISNS_Msg * p_rspmsg);
int
Disable_DD_Member (SOIP_Dd * p_dd, char *ptr, uint32_t type);

int
SNSdbAddAttrPort (ISNS_Attr **attr_indx, ISNS_Attr **key_indx, ISNS_Msg_Descp * p_md,
                  ISNS_Msg * rspmsg);


int
SNSdbRemoveAttrISCSI (ISNS_Attr **key_indx, ISNS_Attr *src_attr, ISNS_Msg_Descp * p_md,
                      ISNS_Msg * rspmsg);

int
SNSdbRemoveAttrNode (ISNS_Attr **key_indx, ISNS_Attr *src_attr, ISNS_Msg_Descp * p_md, ISNS_Msg * rspmsg);

int
SNSdbRemoveAttrPortal (ISNS_Attr **key_indx, ISNS_Attr *src_attr, ISNS_Msg_Descp * p_md,
                       ISNS_Msg * rspmsg);

int
Append_DD_to_List (ISNS_DD_LIST p_dlist, uint32_t id);

int
Create_Default_DD();

int
Check_Authorization (ISNS_Attr *src_attr);

#endif
