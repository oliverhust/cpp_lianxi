/**************************************************************************
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
 * This file contains source code for registering objects with the
 * iSNS database.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/netdb.h>
#include <sys/basetype.h>
#include <sys/error.h>
#include <sys/l3vpn.h>
#include <sys/dns.h>
#include <sys/list.h>
#include <sys/assert.h>
#include <sys/epoll.h>
#include <sys/in.h>

#include "../../include/iscsi_com.h"
#include "../../include/iscsi_event.h"
#include "../../include/iscsi_util.h"

#include "../../include/iscsi_basetype.h"
#include "../../include/iscsi_packet.h"
#include "../../include/iscsi_main.h"

#include "iSNStypes.h"
#include "iSNS.h"
#include "iSNSmsg.h"
#include "iSNSList.h"
#include "iSNSdb.h"
#include "iSNSobjects.h"
#include "iSNSdb.h"
#include "iSNSreg.h"
#include "iSNSquery.h"
#include "iSNSscn.h"
#include "iSNSresponse.h"
#include "iSNScomm.h"
#include "iSNSbuffer.h"
#include "iSNStbl.h"
#include "iSNSparse.h"
#include "iSNSdebug.h"

/* Global */
extern int isns_dereg_debug;;
extern uint32_t isns_esi_interval;
extern uint32_t *dlist_node; /*[MAX_DD_PER_LIST];*/
extern ISNS_LIST scn_callback_list;
extern ISNS_Msg *p_scn_msg_buffer;
extern ISNS_Msg *p_scn_all_msg_buffer;
extern ISNS_Msg_Descp *p_scn_md;
extern ISNS_Msg_Descp *p_rspMd;
extern dbStats iSNS_stats;

/*
 * static common variables for gdbm fetches
 * entry is for common registrations and queries
 * scn_entry is for state change notifications
 * opn_entry is for access control
 */
static SOIP_DB_Entry entry;     /* top level fetches ONLY */
static SOIP_DB_Entry entry2;    /* secondary fetches */
static SOIP_DB_Entry entry3;
static SOIP_DB_Entry entry4;
static SOIP_DB_Entry entry5;

/*********************************************************************
_SNSdbRemoveAttr

This will call the more specific remove procedures.

*********************************************************************/
int
ISNSdbRemoveAttr ( ISNS_Msg_Descp * p_md, ISNS_Msg * p_rspmsg)
{
   ISNS_Attr      *p_attr;
   int            status;
   int            i;
   int            deregEnitityFlag;
   int            deregFCNodeFlag; 
   int            deregFCPortFlag; 
   int            deregPortalFlag; 
   int            deregISCSIFlag;
   ISNS_Attr      *attr_indx[SNS_MAX_ATTRS];
   ISNS_Attr      *key_indx[SNS_MAX_ATTRS];

   ISNS_Attr      *src_attr;

   ISNSParseMsg( &p_md->msg, attr_indx, key_indx, &src_attr );

   /* Search the operation Attributes */
   deregEnitityFlag = FALSE;
   deregFCNodeFlag = FALSE;
   deregFCPortFlag = FALSE;
   deregISCSIFlag = FALSE;
   deregPortalFlag = FALSE;

   for (i = 0; i < MAX_PARSE_ATTRS && attr_indx[i]; i++)
   {
      p_attr = (ISNS_Attr *)(attr_indx[i]);
      switch (p_attr->tag)
      {
      case ISNS_ENTITY_PERIOD:
      case ISNS_ENTITY_ID:
      case ISNS_ENTITY_TYPE:
      case ISNS_PROT_VER:
      case ISNS_MGMT_IP:
      case ISNS_ESI_INTERVAL:
      case ISNS_ESI_PORT:
      case ISNS_TIMESTAMP:
      case ISNS_ENTITY_CERT:
         deregEnitityFlag = TRUE;
         break;

      case ISNS_PORTAL_IP:
      case ISNS_PORTAL_PORT:
      case ISNS_PORTAL_SYM_NAME:
         deregPortalFlag = TRUE;
         break;

      case ISNS_NODE_NAME:
      case ISNS_NODE_SYM_NAME:
      case ISNS_FC_NODE_IP:
      case ISNS_FC_NODE_IPA:
      case ISNS_FC_NODE_CERT:
         deregFCNodeFlag = TRUE;
         break;

      case ISNS_PORT_NAME:
      case ISNS_PORT_ID:
      case ISNS_PORT_TYPE:
      case ISNS_PORT_SYM_NAME:
      case ISNS_FABRIC_PORT_NAME:
      case ISNS_FC_HARD_ADDR:
      case ISNS_FC_PORT_IP:
      case ISNS_FC_COS:
      case ISNS_FC4_TYPE:
      case ISNS_FC4_DESC:
      case ISNS_FC4_FEATURE:
         deregFCPortFlag = TRUE;
         break;

      case ISNS_ISCSI_NODE_ID:
      case ISNS_ISCSI_TYPE:
      case ISNS_ISCSI_ALIAS:
         deregISCSIFlag = TRUE;
         break;

      default:
         break;
      }
   }

   status = ISNS_NO_ERR;
   if (deregEnitityFlag)
      status = SNSdbRemoveAttrEntity (attr_indx, src_attr, p_md, &p_rspMd->msg);
   if (deregPortalFlag && status == ISNS_NO_ERR)
      status = SNSdbRemoveAttrPortal (attr_indx, src_attr, p_md, &p_rspMd->msg);
   if (deregFCNodeFlag && status == ISNS_NO_ERR)
      status = SNSdbRemoveAttrNode (attr_indx, src_attr, p_md, &p_rspMd->msg);
   if (deregFCPortFlag && status == ISNS_NO_ERR)
      status = SNSdbRemoveAttrPort (attr_indx, src_attr, p_md, &p_rspMd->msg);
   if (deregISCSIFlag && status == ISNS_NO_ERR)
      status = SNSdbRemoveAttrISCSI (attr_indx, src_attr, p_md, &p_rspMd->msg);

   return status;
}

/*********************************************************************
_SNSdbRemoveAttr_DDS_Entry

Removes a DDS entry from the database.
*********************************************************************/
int
SNSdbRemoveAttrDDS_Entry (int id, ISNS_Attr * src_attr,
                              ISNS_Msg * p_rspmsg)
{
   SOIP_Dds          *p_dds;
   SOIP_DDS_Key      dds_key;
   int               isActiveFlag;
   int               rval;
   int               count;
   ISNS_LIST_NODE    *pnode;
   SOIP_DD_Key       dd_key;
   SOIP_Dd           *p_dd;
   ISNS_LIST_NODE    *pnode2;
   int               updated_dd_list_size;
   int               memberRemovedFlag;

   __DEBUG (isns_dereg_debug &1, RemoveAttrDSS_Entry);

   dds_key.id = id;
   rval = read_DDSObject(id, &p_dds, &entry);
   if (rval != SUCCESS)
      return (rval);

   /* Remove DDS from DD's DDS List */
   memberRemovedFlag = FALSE;
   memset(dlist_node, 0, MAX_DD_PER_LIST*sizeof(uint32_t));
   updated_dd_list_size = 0;

   pnode = NULL;
   while ( (pnode=GetNextNode(&p_dds->dd_list, pnode)) )
   {
      dd_key.id = *(uint32_t *)GetNodeData(pnode);
      rval = read_DDObject(dd_key.id, &p_dd, &entry2);
      if (rval != SUCCESS)
         return (rval);

      isActiveFlag = IsDDActive( dd_key.id, p_dds->id );

      if (p_dd->activeFlag && !isActiveFlag)
      {
         dlist_node[updated_dd_list_size]=p_dd->id;
         updated_dd_list_size++;
      }

      if ((pnode2=FindNode(&p_dd->dds_list, (char *)&p_dds->id, sizeof(p_dds->id))))
      {
         RemoveNode(&p_dd->dds_list, pnode2);
      }

      if ( !isActiveFlag )
         p_dd->activeFlag = FALSE;

      memberRemovedFlag = TRUE;

      /* Store Entry */
      write_DDObject(dd_key.id,entry2);
   }

   for ( count = 0; count<updated_dd_list_size; count++ )
   {
      dd_key.id = dlist_node[ count ];
      rval = read_DDObject(dd_key.id, &p_dd, &entry2);
      if (rval == SUCCESS)
        change_dd_member_active_flag( p_dd );
   }

   DeleteList(&p_dds->dd_list);

   if ( delete_DDSObject (dds_key.id) != SUCCESS)
   {
      __DEBUG (isns_dereg_debug & 1, (DDS Removal failed, %u), dds_key.id);
      return (ISNS_UNKNOWN_ERR);
   }
   iSNS_stats.num_dds--;

   /* Is DD Still active */

   return (ISNS_NO_ERR);
}

/*********************************************************************
_SNSdbRemoveAttrDDS_Member 

  Removes a DD from a DDS.
*********************************************************************/
int
SNSdbRemoveAttrDDS_Member (int id, ISNS_Attr **attr_indx, ISNS_Attr * src_attr,
                               ISNS_Msg * p_rspmsg)
{
   int            kk;
   int            rval;
   ISNS_Key       *key;
   SOIP_Dds       *p_dds;
   SOIP_DD_Key    dds_key;
   ISNS_LIST_NODE *pnode;
   SOIP_DD_Key    dd_key;
   SOIP_Dd        *p_dd;
   int            updated_dd_list_size;
   int            memberRemovedFlag;


   __DEBUG (isns_dereg_debug &1, RemoveAttrDDS_Member);

   dds_key.id = id;
   rval = read_DDSObject(dds_key.id, &p_dds, &entry);
   if (rval != SUCCESS)
      return (rval);

   /* Remove DDS from DD's DDS List */
   memberRemovedFlag = FALSE;
   memset(dlist_node, 0, MAX_DD_PER_LIST*sizeof(uint32_t));
   updated_dd_list_size = 0;
   for (kk = 0; kk < MAX_PARSE_ATTRS && attr_indx[kk]; kk++)
   {
      key = (ISNS_Key *)(attr_indx[kk]);
      if (key->tag == ISNS_DD_ID)
      {
         if (ISNS_NO_ERR != Remove_dd_from_dds (p_dds, key->val.dd_id))
         {
            return (ISNS_UNKNOWN_ERR);
         }

         dd_key.id = key->val.dd_id;
         rval = read_DDObject(dd_key.id, &p_dd, &entry2);
         if (rval != SUCCESS)
              return (rval);

         pnode=NULL;
         if ((pnode=FindNode(&p_dd->dds_list, (char *)&p_dds->id, sizeof(p_dds->id))))
         {

            RemoveNode(&p_dd->dds_list, pnode);
         }

         memberRemovedFlag = TRUE;

         if (p_dd->activeFlag && !IsDDActive( p_dd->id, p_dds->id ))
         {
            dlist_node[updated_dd_list_size] = p_dd->id;
            p_dd->activeFlag = FALSE;
         }

         /* Store Entry */
         write_DDObject(dd_key.id,entry2);
      }
   }

   for (kk = 0; kk <updated_dd_list_size; kk++)
   {
      if (!dlist_node[ kk ])
         break;

      dd_key.id = dlist_node[ kk ];
      rval = read_DDObject(dd_key.id, &p_dd, &entry2);
      if (rval == SUCCESS)
         change_dd_member_active_flag( p_dd );
   }

   /* Store Entry */
   rval = write_DDSObject(id, entry);
   return (ISNS_NO_ERR);
}

/*********************************************************************
_SNSdbRemoveAttrDDS

  Removes a DDS from the database.
*********************************************************************/
int
ISNSdbRemoveAttrDDS ( ISNS_Msg_Descp * p_md, ISNS_Msg * p_rspmsg )
{
   int            rval;
   ISNS_Key       *key;
   int            ddKeyIndex;
   int            keyRemovedFlag;
   ISNS_Attr      *src_attr;
   ISNS_Attr      *attr_indx[SNS_MAX_ATTRS];
   ISNS_Attr      *key_indx[SNS_MAX_ATTRS];

   __DEBUG (isns_dereg_debug & 1, "Entering RemoveAttrDDS()\n");

   ISNSParseMsg(&p_md->msg, attr_indx, key_indx, &src_attr);

   rval = Check_Authorization (src_attr);
   if (rval != SUCCESS)
   {
      return (ISNS_AUTH_FAILED_ERR);
   }

   ddKeyIndex = ISNSFindTag (0, ISNS_DDS_ID, key_indx);
   keyRemovedFlag = FALSE;
   while (ddKeyIndex != -1)
   {
      key = (ISNS_Key *)(key_indx[ddKeyIndex]);

      if (attr_indx[0] != NULL)
      {
         if (ISNS_NO_ERR !=
             (rval =
              SNSdbRemoveAttrDDS_Member (key->val.dd_id, attr_indx,
                                             (ISNS_Attr *)src_attr, p_rspmsg)))
            return (rval);
      }
      else
         if (ISNS_NO_ERR !=
             (rval =
              SNSdbRemoveAttrDDS_Entry (key->val.dd_id, (ISNS_Attr *)src_attr,
                                            p_rspmsg)))
      {
         return (rval);
      }

      keyRemovedFlag = TRUE;
      ddKeyIndex = ISNSFindTag (ddKeyIndex + 1, ISNS_DDS_ID, key_indx);
   }

   if (keyRemovedFlag != TRUE)
   {
      __DEBUG (isns_dereg_debug & 1, (No DDS deregistered));
      return (ISNS_UNKNOWN_ERR);
   }

   return (ISNS_NO_ERR);
}

/*********************************************************************
_SNSdbRemoveAttrDD

Removes one or more DD from the database.
*********************************************************************/
int
ISNSdbRemoveAttrDD ( ISNS_Msg_Descp * p_md, ISNS_Msg * p_rspmsg )
{
   int         rval;
   ISNS_Key    *key;
   int         flag;
   int         ddKeyIndex;
   int         keynum;
   ISNS_Attr   *src_attr;
   ISNS_Attr   *attr_indx[SNS_MAX_ATTRS];
   ISNS_Attr   *key_indx[SNS_MAX_ATTRS];

   __DEBUG (isns_dereg_debug & 1, "Entering RemoveAttrDD()\n");

   ISNSParseMsg( &p_md->msg, attr_indx, key_indx, &src_attr );

   rval = Check_Authorization (src_attr);
   if (rval != SUCCESS)
   {
      return (ISNS_AUTH_FAILED_ERR);
   }

   keynum = isns_ldap_checkKeyNum(key_indx);
   if (1 < keynum)
      return (ISNS_MSG_FMT_ERR);


   ddKeyIndex = ISNSFindTag (0, ISNS_DD_ID, key_indx);
   flag = FALSE;
   while (ddKeyIndex != -1)
   {
      key = (ISNS_Key *)(key_indx[ddKeyIndex]);

      if(0 == key->len)
        return ISNS_MSG_FMT_ERR;

      if (attr_indx[0] != NULL)
      {
         /* Remove DD MEMBER */
         if (ISNS_NO_ERR !=
             (rval =
              SNSdbRemoveDD_Member (key->val.dd_id, attr_indx,
                                        (ISNS_Attr *)src_attr, p_rspmsg)))
         {
            return (rval);
         }
      }
      else if (ISNS_NO_ERR !=
               (rval =
                SNSdbRemoveDD_Entry (key->val.dd_id, (ISNS_Attr *)src_attr,
                                         p_rspmsg)))
      {
         if (ISNS_NO_SUCH_ENTRY_ERR == rval)
             return (ISNS_NO_ERR);
         else
             return rval;
      }

      flag = TRUE;

      /* Next round */
      ddKeyIndex = ISNSFindTag (ddKeyIndex + 1, ISNS_DD_ID, key_indx);
   }

   if (FALSE == flag)
   {
      __DEBUG (isns_dereg_debug & 1, (Key not found));
      return (ISNS_MSG_FMT_ERR);
   }

   return (ISNS_NO_ERR);
}

/*********************************************************************
_SNSdbRemoveAttrEntityEntry

Removes an entity from the database.
*********************************************************************/
int
ISNSdbRemoveAttrEntityEntry ( char *p_entity_id, ISNS_Attr * src_attr,
                            ISNS_Msg * p_rspmsg )
{
   SOIP_Entity       *p_entity;
   int               rval;
   ISNS_LIST_NODE    *pnode;

   __DEBUG ( isns_dereg_debug & 1, (Deregistering Entity %s),p_entity_id);

   /* read entity from database */
   rval = read_EntityObject(p_entity_id, &p_entity, &entry);
   if (rval != SUCCESS)
     return rval;

   /* Remove Portals */
   __DEBUG (isns_dereg_debug &1,Remove iportal_list);
   pnode = NULL;
   while ((pnode = GetNextNode(&p_entity->iportal_list, pnode)))
   {
      rval = SNSRemovePortalEntry (GetNodeData(pnode), NULL, p_rspmsg);
   }
   DeleteList( &p_entity->iportal_list );

   /* Remove nodes */
   if ( p_entity->eid_type == ENTITY_TYPE_IFCP )
   {
      /* Remove IFCP Nodes */
      __DEBUG (isns_dereg_debug &1,Remove ifcp_node_list);
      pnode = NULL;
      while ((pnode = GetNextNode(&p_entity->ifcp_node_list, pnode)))
      {
         rval = SNSRemoveIFCPNodeEntry (GetNodeData(pnode), NULL, p_rspmsg);
      }
      DeleteList( &p_entity->ifcp_node_list );
   }
   else if (p_entity->eid_type == ENTITY_TYPE_ISCSI)
   {
      /* Remove ISCSI Nodes */  
      __DEBUG (isns_dereg_debug &1,Remove iscsi_node_list);
      pnode = NULL;
      while ((pnode = GetNextNode(&p_entity->iscsi_node_list, pnode)))
      {
            __DEBUG (isns_dereg_debug &1, Call SNSRemoveISCSINodeEntry);
            rval = SNSRemoveISCSINodeEntry (GetNodeData(pnode), NULL, p_rspmsg);
            if (rval)
               break;
      }
      DeleteList( &p_entity->iscsi_node_list );
   }
   else
   {
      __DEBUG (isns_dereg_debug & 1, (Entity Type invalid %u), p_entity->eid_type);
   }

   RemoveCert (&p_entity->ptr_cert);

   /* Remove Entity In Entity Index Database */
   delete_EntityidxObject(p_entity->entity_index);

   /* Remove Entity */
   rval = delete_EntityObject(p_entity_id);

   iSNS_stats.num_entity--;

   __DEBUG (isns_dereg_debug &1,(Prepare response for: %s),p_entity_id);

   ISNSAppendKey (p_rspmsg, ISNS_ENTITY_ID, PAD4 (strlen (p_entity_id)),
                  p_entity_id, 0);

   return ( rval );
}

/*********************************************************************
_SNSRemoveIFCPNodeEntry

Removes an iFCP entry from the database.
*********************************************************************/
int
SNSRemoveIFCPNodeEntry (char *ifcp_node_key, ISNS_Attr * src_attr,
                        ISNS_Msg * p_rspmsg)
{
   int rval;
   char buffer[PORT_NAME_SIZE];
   SOIP_DB_Node_Name db_node_name;
   SOIP_Entity *p_entity;
   SOIP_Fc_Node *p_fc_node;
   SOIP_Ifcp *p_ifcp_node;
   SOIP_Dd *p_dd;
   SOIP_DD_Key dd_key;
   ISNS_LIST_NODE *pnode;

   __DEBUG (isns_dereg_debug & 1, (Deregistering IFCP WWPN));

   /*
      Validate the src attr has permission to delete the node 
      If the src node is an iscsi node, verify that it is the same
      as the iscsi node.
      If the src is an entity verify that iscsi node belongs to the entity
   */

   memset (buffer, 0, PORT_NAME_SIZE);
   __ISNS_COPY (buffer, sizeof(buffer), ifcp_node_key, PORT_NAME_SIZE);

   rval = read_FCPortObject((char *)buffer, &p_ifcp_node, &entry2);
   if (rval != SUCCESS)
     return rval;

   /* Remove from all DD */
   pnode=NULL;
   while ((pnode=GetNextNode(&p_ifcp_node->dd_id_list, pnode)))
   {
      dd_key.id = *(uint32_t *) GetNodeData(pnode);
      rval = read_DDObject(dd_key.id, &p_dd, &entry5);
      if (rval != SUCCESS)
        return rval;

      if (ISNS_NO_ERR != Disable_DD_Member (p_dd, p_ifcp_node->port_name.v,
                             ISNS_DD_IFCP_MEMBER))
         return (ISNS_UNKNOWN_ERR);

      /* Store Entry */
      rval = write_DDObject(dd_key.id,entry5);
   }

   /* Remove from the FC4 Types port list. */

   /* Remove Node Name */
   {
      char zero[ISNS_NODE_NAME_SIZE];

      memset (zero, 0, sizeof (zero));
      if (memcmp (p_ifcp_node->node_name.v, zero, ISNS_NODE_NAME_SIZE))
      {
         /* Valid Node Name */

         /* FC Node's IFCP Node List */
         memset (&db_node_name, 0, sizeof (db_node_name));
         __ISNS_COPY (db_node_name.v, sizeof(db_node_name.v), p_ifcp_node->node_name.v, ISNS_NODE_NAME_SIZE);
         db_node_name.v[8] = (char)SOIP_DB_Node_Id;
         read_FCNodeObject(db_node_name.v, &p_fc_node, &entry3);
         if (rval != SUCCESS)
            return rval;

         /* Remove IFCP Node from FC Node's List */
         if ((pnode=FindNode(&p_fc_node->port_list, ifcp_node_key, PORT_NAME_SIZE)))
         {
            RemoveNode(&p_fc_node->port_list, pnode);
         }
         
         rval = write_FCNodeObject(db_node_name.v, entry3);

         if (IsEmptyList(&p_fc_node->port_list))
         {
            SNSRemoveFCNodeEntry (p_fc_node->node_name.v, NULL);
         }
      }
   }

   RemoveCert (&p_ifcp_node->ptr_cert);

   /* Remove Port from entity's List */
   rval = read_EntityObject(p_ifcp_node->entity_id.id, &p_entity, &entry4);
   if (rval != SUCCESS)
     return rval;

   pnode=NULL;
   if ((pnode=FindNode(&p_entity->ifcp_node_list, ifcp_node_key, PORT_NAME_SIZE)))
   {
         RemoveNode(&p_entity->ifcp_node_list, pnode);
   }

   rval = write_EntityObject(p_ifcp_node->entity_id.id, entry4);

   if (IsEmptyList(&p_entity->ifcp_node_list) && src_attr != NULL)
   {
      int more_esi_portals;
      SOIP_Portal *p_portal;

      /* Anymore Portals w/ ESI */
      more_esi_portals = FALSE;
      pnode=NULL;
      while ((pnode=GetNextNode(&p_entity->iportal_list, pnode)))
      {
         /* Fetch Portal */
         rval = read_PortalObject(GetNodeData(pnode), &p_portal, &entry3);
         if (rval != SUCCESS)
           return rval;

         if (p_portal->esi_interval != 0)
         {
            more_esi_portals = TRUE;
            break;
         }
      }

      if (!more_esi_portals)
      {
         /* remove entity */
         ISNSdbRemoveAttrEntityEntry (p_entity->eid.id, NULL, NULL);
      }
   }

   /* Delete the iFCP Node */
   DeleteList(&p_ifcp_node->dd_id_list);

   iSNS_stats.num_ifcp--;

   rval = delete_FCPortObject(buffer);
   if (rval != SUCCESS)
   {
      __LOG_ERROR ("ERROR: IFCP NODE Removal failed.");
      return (ERROR);
   }

   __DEBUG (isns_dereg_debug &1,(Prepare response for: %s),buffer);

   ISNSAppendKey (p_rspmsg, ISNS_PORT_NAME, ISNS_IFCP_NODE_NAME_SIZE, buffer,
                  0);
   return (ISNS_NO_ERR);
}

/*********************************************************************
Removes one or more portals from the database.  This will loop through
all the keys and call helper functions.
*********************************************************************/
int
SNSdbRemoveAttrPortal (ISNS_Attr **key_indx, ISNS_Attr *src_attr, ISNS_Msg_Descp * p_md,
                       ISNS_Msg * rspmsg)
{
   int               foundFlag;
   SOIP_Portal_Key   db_portal;
   int               ii;
   int               portalIpIndex;
   int               portalPortIndex;
   ISNS_Key          *key;
   int               saveIndex;
   int               rval;


   __DEBUG (isns_dereg_debug &1, SSNSdbRemoveAttrPortal);

   foundFlag = FALSE;
   saveIndex = 0;
   portalIpIndex = -1;
   portalPortIndex = -1;
   while (1)
   {
      for (ii = saveIndex; ii < SNS_MAX_ATTRS && key_indx[ii]
           && (portalIpIndex == -1 || portalPortIndex == -1); ii++)
      {
         key = (ISNS_Key *)(key_indx[ii]);
         switch (key->tag)
         {
         case ISNS_PORTAL_IP:
            /* Check to see if we already had an IP */
            if (portalIpIndex != -1)
            {
               __LOG_ERROR ("ERROR: PORTAL_IP tag found twice.");
               return (ERROR);
            }
            portalIpIndex = ii;
            break;
         case ISNS_PORTAL_PORT:
            /* Check to see if we already had a PORT */
            if (portalPortIndex != -1)
            {
               __LOG_ERROR ("ERROR: PORTAL_PORT tag found twice.");
               return (ERROR);
            }
            portalPortIndex = ii;
            break;
         default:
            break;
         }
      }

      saveIndex = ii;

      if (portalIpIndex == -1 || portalPortIndex == -1)
      {
         break;
      }

      /* Call Remove Entry */
      key = (ISNS_Key *)(key_indx[portalIpIndex]);
      __ISNS_COPY (db_portal.ip_addr.v, sizeof(db_portal.ip_addr.v), &key->val, ISNS_PORTAL_IP_SIZE);

      key = (ISNS_Key *)(key_indx[portalPortIndex]);
      db_portal.ip_port=key->val.ip_port;

      if (SUCCESS !=
          (rval = SNSRemovePortalEntry (&db_portal, src_attr, rspmsg)))
         return (rval);

      foundFlag = TRUE;

      portalIpIndex = -1;
      portalPortIndex = -1;
   }

   if (!foundFlag)
   {
      __LOG_ERROR ("ERROR: Missing Keys.");
      return (ERROR);
   }

   return (ISNS_NO_ERR);
}

/*********************************************************************
Removes one or more iFCP nodes from the database.  This will loop
through all the keys and call helper functions.
*********************************************************************/
int
SNSdbRemoveAttrPort (ISNS_Attr **key_indx, ISNS_Attr *src_attr, ISNS_Msg_Descp * p_md,
                     ISNS_Msg * rspmsg)
{
   ISNS_Key *key;
   int iFCPNodeKeyIndex;
   int rval;

   iFCPNodeKeyIndex = ISNSFindTag (0, ISNS_PORT_NAME, key_indx);
   if (iFCPNodeKeyIndex == -1)
   {
      __LOG_ERROR ("ERROR: iFCP Node key not found.");
      return (ERROR);
   }

   while (iFCPNodeKeyIndex != -1)
   {
      key = (ISNS_Key *)(key_indx[iFCPNodeKeyIndex]);
      if (SUCCESS !=
          (rval = SNSRemoveIFCPNodeEntry ((char *)&key->val, (ISNS_Attr *)src_attr, rspmsg)))
         return (rval);

      iFCPNodeKeyIndex =
         ISNSFindTag (iFCPNodeKeyIndex + 1, ISNS_PORT_NAME, key_indx);
   }

   return (ISNS_NO_ERR);
}

/*********************************************************************
Removes one or more iSCSI Nodes.  This will loop through all the iSCSI
node keys and call helper functions which will do all the brute work.
*********************************************************************/
int
SNSdbRemoveAttrISCSI (ISNS_Attr **key_indx, ISNS_Attr *src_attr, ISNS_Msg_Descp * p_md,
                      ISNS_Msg * rspmsg)
{
   ISNS_Key *key;
   int nodeKeyIndex;
   int rval;

   nodeKeyIndex = ISNSFindTag (0, ISNS_ISCSI_NODE_ID, key_indx);
   if (nodeKeyIndex == -1)
   {
      __DEBUG (isns_dereg_debug & 1, (Entity Key not found));
      return (ISNS_MSG_FMT_ERR);
   }

   while (nodeKeyIndex != -1)
   {
      key = (ISNS_Key *)(key_indx[nodeKeyIndex]);
      if (SUCCESS !=
          (rval = SNSRemoveISCSINodeEntry ((char *)&key->val, (ISNS_Attr *)src_attr, rspmsg)))
         return (rval);

      nodeKeyIndex =
         ISNSFindTag (nodeKeyIndex + 1, ISNS_ISCSI_NODE_ID, key_indx);
   }

   return (ISNS_NO_ERR);
}

/*********************************************************************
Removes one or more FC Node from the database.  The brute work is done
in helper functions.
*********************************************************************/
int
SNSdbRemoveAttrNode (ISNS_Attr **key_indx, ISNS_Attr *src_attr, ISNS_Msg_Descp * p_md, ISNS_Msg * rspmsg)
{
   ISNS_Key *key;
   int FCNodeKeyIndex;
   int rval;

   FCNodeKeyIndex = ISNSFindTag (0, ISNS_NODE_NAME, key_indx);
   if (FCNodeKeyIndex == -1)
   {
      __DEBUG (isns_dereg_debug & 1, (Entity Key not found));
      return (ISNS_MSG_FMT_ERR);
   }

   while (FCNodeKeyIndex != -1)
   {
      key = (ISNS_Key *)(key_indx[FCNodeKeyIndex]);
      if (SUCCESS != (rval = SNSRemoveFCNodeEntry ((char *)&key->val, rspmsg)))
         return (rval);

      FCNodeKeyIndex =
         ISNSFindTag (FCNodeKeyIndex + 1, ISNS_NODE_NAME, key_indx);
   }

   return (ISNS_NO_ERR);
}

/*********************************************************************
Removes one or more entities from the database.
*********************************************************************/
int
SNSdbRemoveAttrEntity (ISNS_Attr **key_indx, ISNS_Attr *src_attr, ISNS_Msg_Descp * p_md,
                       ISNS_Msg * rspmsg)
{
   ISNS_Key *key;
   int rval;
   int entityKeyIndex;
   char buffer[MAX_ENTITY_ID_SIZE] = { 0 };

   rval = ERROR;
   entityKeyIndex = ISNSFindTag (0, ISNS_ENTITY_ID, key_indx);
   if (entityKeyIndex == -1)
   {
      __DEBUG (isns_dereg_debug & 1, (Entity Key not found));
      return (ISNS_MSG_FMT_ERR);
   }

   while (entityKeyIndex != -1)
   {
      key = (ISNS_Key *)(key_indx[entityKeyIndex]);
      __ISNS_COPY (buffer, sizeof(buffer), &key->val, key->len);
      rval = ISNSdbRemoveAttrEntityEntry (buffer, src_attr, rspmsg);
      if (rval != ISNS_NO_ERR)
         return (rval);
      entityKeyIndex =
         ISNSFindTag (entityKeyIndex + 1, ISNS_ENTITY_ID, key_indx);
   }
   return (rval);
}

/*********************************************************************
Removes an iSCSI Node from the database.
*********************************************************************/
int
SNSRemoveISCSINodeEntry (char *iscsi_node_key, ISNS_Attr * src_attr,
                         ISNS_Msg * p_rspmsg)
{
   int               rval;   
   int               scn_bitmap;
   SOIP_Iscsi        *p_node;
   SOIP_Entity       *p_entity;
   SOIP_Dd           *p_dd;
   SOIP_DD_Key       dd_key;
   ISNS_LIST_NODE    *pnode;

   __DEBUG (isns_dereg_debug & 1, (Deregistering iSCSI Node:%s),iscsi_node_key);

   memset(p_scn_all_msg_buffer, 0, sizeof(ISNS_Msg));
   p_scn_all_msg_buffer->hdr.type = ISNS_SCN;

   scn_bitmap = 0;
   rval = read_ISCSIObject(iscsi_node_key, &p_node, &entry2);
   if (rval != SUCCESS)
    return rval;

   /*
      Validate the src attr has permission to delete the node 
      If the src node is an iscsi node, verify that it is the same
      as the iscsi node.
      If the src is an entity verify that iscsi node belongs to the entity
   */

   RemoveCert (&p_node->ptr_cert);

   /* Remove ISCSI Node from DD */

   pnode=NULL;
   while ((pnode=GetNextNode(&p_node->dd_id_list, pnode)))
   {
      dd_key.id = *(uint32_t *)GetNodeData(pnode);
      rval = read_DDObject(dd_key.id, &p_dd, &entry5);
      if (rval == SUCCESS)
      {
        Disable_DD_Member (p_dd, p_node->id.v, ISNS_DD_ISCSI_MEMBER);

        /* Store Entry */
        write_DDObject(dd_key.id, entry5);
      }
   }

   /* read entity from database */
   rval = read_EntityObject(p_node->entity_id.id, &p_entity, &entry4);
   if (rval != SUCCESS)
     return ERROR;

   /* Remove ISCSI Node from ENTITY */
   if ((pnode = FindNode(&p_entity->iscsi_node_list, iscsi_node_key, PAD4(strlen(iscsi_node_key)))))
   {
      RemoveNode(&p_entity->iscsi_node_list, pnode);
   }

   /* update entity in database by doing a write */
   rval = write_EntityObject(p_node->entity_id.id, entry4);

   /* 如果没有DD引用则把NAME--INDEX关系删了 */
   SOIP_ISCSI_Index stIscsiIndex;
   if(SUCCESS == read_ISCSIidxObjectByName(p_node->id.v, &stIscsiIndex) &&
      0 == stIscsiIndex.iDdRefCount)
   {
        delete_ISCSIidxObject(p_node->iscsi_index);
   }

   RemoveCert (&p_node->ptr_cert);
   DeleteList(&p_node->dd_id_list);

   ISNSAppendKey( p_scn_all_msg_buffer, ISNS_ISCSI_NODE_ID, PAD4(strlen(iscsi_node_key)),iscsi_node_key, 0);

   send_iscsi_scn_to_members ( NULL, iscsi_node_key, p_scn_all_msg_buffer, ISNS_SCN_OBJ_REMOVED );

   rval = delete_ISCSIObject(iscsi_node_key);
   if (rval != SUCCESS)
   {
      __DEBUG (isns_dereg_debug & 1, (ISCSI NODE Removal failed %s), iscsi_node_key);
      return (ERROR);
   }

   iSNS_stats.num_iscsi--;

   return ( ISNS_NO_ERR );
}

/*********************************************************************
Removes an FC Node entry from the database.
*********************************************************************/
int
SNSRemoveFCNodeEntry (char *p_key, ISNS_Msg * p_rspmsg)
{
   int rval;
   SOIP_Fc_Node *p_fc_node;
   SOIP_DB_Node_Name db_node_name;
   ISNS_LIST_NODE *pnode;
   ISNS_DBKey read_key;

   __DEBUG (isns_dereg_debug & 1, (Deregistering FC Node));

   memset (&db_node_name, 0, sizeof (db_node_name));

   __ISNS_COPY (db_node_name.v, sizeof(db_node_name.v), p_key, ISNS_NODE_NAME_SIZE);
   db_node_name.v[8] = (char)SOIP_DB_Node_Id;

   rval = read_FCNodeObject(db_node_name.v, &p_fc_node, &entry4);
   if (rval != SUCCESS)
     return rval;

   pnode=NULL;
   while ((pnode=GetNextNode(&p_fc_node->port_list, pnode)))
   {
      strcpy(read_key.val.node_key.v,GetNodeData(pnode));
      rval = read_FCNodeObject(GetNodeData(pnode), &p_fc_node, &entry5);
      if (rval != SUCCESS)
         return rval;

      memset (p_fc_node->node_name.v, 0, ISNS_NODE_NAME_SIZE);
      rval = write_FCNodeObject(db_node_name.v, entry5);
   }

   __ISNS_COPY (db_node_name.v, sizeof(db_node_name.v), p_key, ISNS_NODE_NAME_SIZE);
   db_node_name.v[8] = (char)SOIP_DB_Node_Id;

   delete_FCNodeObject(db_node_name.v);

   __DEBUG (isns_dereg_debug &1,(Prepare response for: %s),p_key);

   ISNSAppendKey (p_rspmsg, ISNS_NODE_NAME, ISNS_NODE_NAME_SIZE, p_key, 0);
   return (ISNS_NO_ERR);
}

/*********************************************************************
Removes a portal entry from the database.
*********************************************************************/
int
SNSRemovePortalEntry (SOIP_Portal_Key *portal_key, ISNS_Attr * src_attr,
                      ISNS_Msg * p_rspmsg)
{
   SOIP_Portal       *p_portal;
   SOIP_Entity       *p_entity;
   int               rval;
   ISNS_LIST_NODE    *pnode;
   ISNS_LIST_NODE    *pnode2;
   PORTAL_LIST_ENTRY portalListEntry;
   char              *p_iscsi_name;

   __DEBUG (isns_dereg_debug & 1, (Deregistering Portal));

   memset(p_scn_all_msg_buffer, 0, sizeof(ISNS_Msg));
   p_scn_all_msg_buffer->hdr.type = ISNS_SCN;

   rval = read_PortalObject(portal_key, &p_portal, &entry2);
   if (rval != SUCCESS)
   {
    __DEBUG(isns_dereg_debug &1, Portal not found:%i,rval);
    return rval;
   }
   /*
      Validate the src attr has permission to delete the node 
      If the src node is an iscsi node, verify that it is the same
      as the iscsi node.
      If the src is an entity verify that iscsi node belongs to the entity
   */

   __DEBUG (isns_dereg_debug &1,(Remove Cert));

   RemoveCert (&p_portal->ptr_cert);

   delete_PortalidxObject(p_portal->portal_index);

   iSNS_stats.num_portals--;

   rval = read_EntityObject(p_portal->entity_id.id, &p_entity, &entry4);
   if (rval != SUCCESS)
      return rval;

   /* Remove Portals */
   __ISNS_COPY (&portalListEntry.portal_ip_port, sizeof(portalListEntry.portal_ip_port), portal_key, 
             sizeof(portalListEntry.portal_ip_port));
   portalListEntry.portal_idx = p_portal->portal_index;

   pnode = NULL;
   if ((pnode=FindNode(&p_entity->iportal_list, (char *)&portalListEntry, sizeof (portalListEntry))))
   {
         RemoveNode(&p_entity->iportal_list, pnode);
   }

   rval = write_EntityObject(p_portal->entity_id.id, entry4);

   /* Send detailed SCN */
   pnode2 = NULL;
   while ( (pnode2 = GetNextNode( &p_entity->iscsi_node_list, pnode2)) )
   {
         p_iscsi_name = (char *) GetNodeData( pnode2 );
         send_iscsi_scn_to_members ( NULL, p_iscsi_name, p_scn_all_msg_buffer, ISNS_SCN_OBJ_REMOVED );
   }

   rval = delete_PortalObject(portal_key);
   if ( rval != SUCCESS)
      __LOG_ERROR ("ERROR: Portal DEREG Failed.");

   return (rval);
}

/*********************************************************************
_Remove_dd_from_dds

Remove a DD from the DDS's DD List.
*********************************************************************/
int
Remove_dd_from_dds ( SOIP_Dds * p_dds, int id )
{
   ISNS_LIST_NODE *pnode;

   if ((pnode=FindNode(&p_dds->dd_list, (char *)&id, sizeof(id))))
   {
      RemoveNode(&p_dds->dd_list, pnode);
   }

   return (ISNS_NO_ERR);
}

/*********************************************************************
_SNSdbRemoveDD_Entry

PURPOSE:
This will remove a DD ID from the database.

PARAMETER(S):
1. DD ID.
2. SRC ATTR.

RETURN:
Error Code.
*********************************************************************/
int
SNSdbRemoveDD_Entry ( int id, ISNS_Attr * src_attr, ISNS_Msg * p_rspmsg )
{
   int            rval;
   SOIP_Dd        *p_dd;
   SOIP_DD_Key    dd_key;
   SOIP_Dd_Member *p_member;
   ISNS_LIST_NODE *pnode;
   int            memberRemovedFlag;
   SOIP_Dds       *p_dds;
   SOIP_DD_Key     dds_key;
   ISNS_LIST_NODE *pnode2;
   SOIP_ISCSI_Index stIscsiIndex;

   dd_key.id = id;
   rval = read_DDObject(dd_key.id, &p_dd, &entry);
   if (rval != SUCCESS)
      return rval;

   /* Remove DD Members */
   pnode = NULL;
   memberRemovedFlag = FALSE;
   while ( (pnode = GetNextNode(&p_dd->member_list, NULL)) )
   {
      p_member=(SOIP_Dd_Member *) GetNodeData(pnode);

      if ( p_member->type != 0 && 
          p_member->status != ISNS_DD_MEMBER_DISABLE )
      {
         char member_name[256];
         memset (member_name, 0, sizeof (member_name));
         switch (p_member->type)
         {
            case ISNS_DD_ISCSI_MEMBER:
               strncpy (member_name, p_member->node_id, sizeof(member_name));
               if (ISNS_NO_ERR !=
                   Remove_DD_from_ISCSI_Node (p_dd,
                                              p_member->node_id))
               {
                  return ( ISNS_UNKNOWN_ERR );
               }
               memberRemovedFlag = TRUE;
               break;
            case ISNS_DD_IFCP_MEMBER:
               __ISNS_COPY (member_name, sizeof(member_name), p_member->node_id, ISNS_PORT_NAME_SIZE);
               if (ISNS_NO_ERR !=
                   Remove_DD_from_IFCP_Node (p_dd->id, p_member->node_id))
               {
                  return ( ISNS_UNKNOWN_ERR );
               }
               memberRemovedFlag = TRUE;
               break;
            default:
               __LOG_ERROR ("Error, dd member type=%u incorrect.", p_member->type);
   /*          return (ISNS_MSG_FMT_ERR); */
               break;
         }
      }

      /* ISCSI NAME DD引用计数-- */
      if(SUCCESS == read_ISCSIidxObjectByName(p_member->node_id, &stIscsiIndex))
      {
         stIscsiIndex.iDdRefCount--;
         SOIP_Iscsi *pstIscsi;
         SOIP_DB_Entry stEntry;
         if(0 == stIscsiIndex.iDdRefCount &&
            ISNS_NO_SUCH_ENTRY_ERR == read_ISCSIObject(p_member->node_id, &pstIscsi, &stEntry))
         {
            delete_ISCSIidxObject(stIscsiIndex.uiIndex);
         }
         else
         {
            write_ISCSIidxObject(&stIscsiIndex);
         }
      }

      RemoveNode( &p_dd->member_list, pnode );
   }

   pnode = NULL;
   while ( (pnode = GetNextNode( &p_dd->dds_list, pnode )) )
   {
      dds_key.id = *(uint32_t *) GetNodeData(pnode);
      rval = read_DDSObject(dds_key.id, &p_dds, &entry2);
      if (rval != SUCCESS)
        return rval;

      if ((pnode2 = FindNode(&p_dds->dd_list, (char *)&p_dd->id, sizeof(p_dd->id))))
      {
         RemoveNode(&p_dds->dd_list, pnode2);
      }

      /* Store Entry */
      rval = write_DDSObject(dds_key.id, entry2);
   }

   DeleteList( &p_dd->member_list );
   DeleteList( &p_dd->dds_list );

   if ( delete_DDObject(dd_key.id) != SUCCESS )
   {
      __DEBUG ( isns_dereg_debug & 1, (DD Removal failed %u), dd_key.id );
      return ( ERROR );
   }
   iSNS_stats.num_dd--;

   return ( ISNS_NO_ERR );
}

/*********************************************************************
_SNSdbRemoveDD_Member

Removes a member from a DD.
*********************************************************************/
int
SNSdbRemoveDD_Member (int id, ISNS_Attr **attr_indx, ISNS_Attr * src_attr,
                          ISNS_Msg * p_rspmsg)
{
   int ii, rval;
   ISNS_Attr *p_attr;
   SOIP_Dd *p_dd;
   SOIP_DD_Key dd_key;
   int removeMemberFlag;

   __DEBUG (isns_dereg_debug & 1, "Entering RemoveDD_Member()\n");

   dd_key.id = id;
   rval = read_DDObject(dd_key.id, &p_dd, &entry);
   if (rval !=SUCCESS)
      return rval;

   /* Prepare the SCN */
   memset(p_scn_all_msg_buffer, 0, sizeof(ISNS_Msg));
   p_scn_all_msg_buffer->hdr.type = ISNS_SCN;

   /* Remove the DD_ID Members */
   removeMemberFlag = FALSE;
   for (ii = 0; ii < SNS_MAX_ATTRS && attr_indx[ii]; ii++)
   {
      p_attr = (ISNS_Attr *)(attr_indx[ii]);
      switch (p_attr->tag)
      {
      case ISNS_ISCSI_IDX:
      case ISNS_DD_ISCSI_MEMBER_IDX:
      {
         SOIP_DB_Entry entry1;
         SOIP_DB_Entry *p_entry = &entry1;
         void *ptr;

         if(p_attr->len != 0)
         {
           //如果内存中没找到指定index对应的node，则返回错误
           rval = read_ISCSIidxObject(*(uint32_t *)&p_attr->val, &ptr, p_entry);
           if (rval != SUCCESS)
           {
             ISNSAppendAttr (p_rspmsg, ISNS_DD_ISCSI_MEMBER_IDX, ISNS_DD_ISCSI_MEMBER_IDX_SIZE, NULL, *(uint32_t *)&p_attr->val);
             return (ISNS_NO_SUCH_ENTRY_ERR);
           }
         }
         //接下来的处理过程与移除iSCSI Name一样
         memset(p_attr, 0, sizeof(ISNS_Attr));
         p_attr->tag = ISNS_DD_ISCSI_MEMBER;
         p_attr->len = sizeof((char *)&p_entry->data.iscsi_idx.v);
         strlcpy(p_attr->val.node_name.v, (char *)&p_entry->data.iscsi_idx.v, p_attr->len);
      }
      case ISNS_ISCSI_NODE_ID:
      case ISNS_DD_ISCSI_MEMBER:
         Remove_DD_from_ISCSI_Node (p_dd, (char *)&p_attr->val);
         Remove_DD_Member (p_dd, (char *)&p_attr->val, ISNS_DD_ISCSI_MEMBER);

         ISNSAppendKey( p_scn_all_msg_buffer, ISNS_DD_ID, ISNS_DD_ID_SIZE, NULL, p_dd->id);
         ISNSAppendKey( p_scn_all_msg_buffer, ISNS_DD_ISCSI_MEMBER, 
                        PAD4(strlen((char *)&p_attr->val)), (char *)&p_attr->val, 0);
         ISNSAppendKey( p_scn_all_msg_buffer, ISNS_ISCSI_SCN_BITMAP, ISNS_SCN_BITMAP_SIZE, NULL, 
                        ISNS_SCN_MEMBER_REMOVED);
         removeMemberFlag = TRUE;
         send_iscsi_scn_to_members ( NULL,(char *)&p_attr->val, p_scn_all_msg_buffer, ISNS_SCN_OBJ_REMOVED );

         break;
      case ISNS_DD_IFCP_MEMBER:
         if (ISNS_NO_ERR != Remove_DD_from_IFCP_Node (p_dd->id, (char *)&p_attr->val))
         {
            return (ERROR);
         }
         Remove_DD_Member (p_dd, (char *)&p_attr->val, ISNS_DD_IFCP_MEMBER);
         ISNSAppendKey( p_scn_all_msg_buffer, ISNS_DD_ID, ISNS_DD_ID_SIZE, NULL, p_dd->id);
         ISNSAppendKey( p_scn_all_msg_buffer, ISNS_DD_IFCP_MEMBER, 
                        PAD4(strlen((char *)&p_attr->val)), (char *)&p_attr->val, 0);
         ISNSAppendKey( p_scn_all_msg_buffer, ISNS_IFCP_SCN_BITMAP, ISNS_SCN_BITMAP_SIZE, NULL, 
                        ISNS_SCN_MEMBER_REMOVED);
         send_iscsi_scn_to_members ( NULL, (char *)&p_attr->val, p_scn_all_msg_buffer, ISNS_SCN_OBJ_REMOVED );
         removeMemberFlag = TRUE;
         break;
      default:
         return (ERROR);
         break;
      }
   }

   /* Store Entry */
   write_DDObject(dd_key.id, entry);

   return ( ISNS_NO_ERR );
}

/*********************************************************************
_Remove_DD_Member 

Removes a member from a DD.

Returns ISNS_NO_ERR if successful.
*********************************************************************/
int
Remove_DD_Member (SOIP_Dd *p_dd, char *ptr, uint32_t type)
{
   ISNS_LIST_NODE *pnode;
   SOIP_Dd_Member *p_member;
   SOIP_ISCSI_Index stIscsiIndex;

   pnode=NULL;
   while ((pnode=GetNextNode(&p_dd->member_list, pnode)))
   {
      p_member=(SOIP_Dd_Member *)GetNodeData(pnode);

      if (p_member->type!=type)
         /* Type doesn't match */
         continue;

      if (p_member->type==ISNS_DD_IFCP_MEMBER &&
         memcmp(p_member->node_id, ptr, PORT_NAME_SIZE))
         /* Comparing port name doesn't match */
         continue;

      if (strcmp(p_member->node_id, ptr))
         /* Comparing FCIP or iSCSI names don't match */
         continue;

      RemoveNode(&p_dd->member_list, pnode);
      /* ISCSI NAME DD引用计数-- */
      if(SUCCESS == read_ISCSIidxObjectByName(ptr, &stIscsiIndex))
      {
         stIscsiIndex.iDdRefCount--;
         SOIP_Iscsi *pstIscsi;
         SOIP_DB_Entry stEntry;
         if(0 == stIscsiIndex.iDdRefCount &&
            ISNS_NO_SUCH_ENTRY_ERR == read_ISCSIObject(ptr, &pstIscsi, &stEntry))
         {
            delete_ISCSIidxObject(stIscsiIndex.uiIndex);
         }
         else
         {
            write_ISCSIidxObject(&stIscsiIndex);
         }
      }
      break;
   }

   return (ISNS_NO_ERR);
}

/*********************************************************************
_Remove_DD_from_ISCSI_Node

Remove the DD from an iSCSI Node.
*********************************************************************/
int
Remove_DD_from_ISCSI_Node (SOIP_Dd *p_dd, char *p_node_name)
{
   SOIP_Iscsi       *p_node;
   SOIP_DB_Entry    lentry;
   SOIP_DB_Entry    lentry2;
   ISNS_LIST_NODE   *p_lnode;
   SOIP_Dd          *p_dd2;
   int              activeFlag;
   SOIP_DD_Key      dd_key;
   int              rval;

   rval = read_ISCSIObject(p_node_name, &p_node, &lentry);
   if (rval != SUCCESS)
     return rval;

   if ((p_lnode=FindNode(&p_node->dd_id_list, (char *)&p_dd->id, sizeof(p_dd->id))))
   {
      RemoveNode(&p_node->dd_id_list, p_lnode);

      /* Scan all DD, check for at least one active DD */
      p_lnode = NULL;
      activeFlag = FALSE;
      while ( p_node->activeFlag && (p_lnode = GetNextNode(&p_node->dd_id_list, p_lnode)) )
      {
         uint32_t id;
         id = *(uint32_t *)GetNodeData(p_lnode);

         if (id == p_dd->id)
            continue;

         dd_key.id = id;
         rval = read_DDObject(dd_key.id, &p_dd2, &lentry2);
         if (rval == SUCCESS)
           if (p_dd2->activeFlag)
           {
            activeFlag = TRUE;
           }
      }

      p_node->activeFlag = activeFlag;
   }

   write_ISCSIObject(p_node_name, lentry);

   return (ISNS_NO_ERR);
}

/*********************************************************************
_Remove_DD_from_IFCP_Node 

Removes the DD from the iFCP's DD list.
*********************************************************************/
int
Remove_DD_from_IFCP_Node (int id, char *p_node_name)
{
   SOIP_Ifcp      *p_node;
   SOIP_DB_Entry  lentry;
   ISNS_LIST_NODE *p_lnode;
   int            rval;

   rval = read_FCPortObject(p_node_name, &p_node, &lentry);
   if (rval != SUCCESS)
     return rval;

   if ((p_lnode=FindNode(&p_node->dd_id_list, (char *)&id, sizeof(id))))
   {
      RemoveNode(&p_node->dd_id_list, p_lnode);
   }

   write_FCPortObject(p_node_name, lentry);
   return (ISNS_NO_ERR);
}

/*********************************************************************
_Disable_DD_Member 
Disables a DD member.  This gets called when a node is deleted.
*********************************************************************/
int
Disable_DD_Member (SOIP_Dd * p_dd, char *ptr, uint32_t type)
{
   ISNS_LIST_NODE *pnode;
   SOIP_Dd_Member *p_member;

   __DEBUG (isns_dereg_debug &1, Disable_DD_Member);

   pnode=NULL;
   while ((pnode=GetNextNode(&p_dd->member_list, pnode)))
   {
      p_member=(SOIP_Dd_Member *)GetNodeData(pnode);

      if (p_member->type != type)
      {
         __DEBUG (isns_dereg_debug &1, p_member->type:%i not equal to type:%i,p_member->type,type);
         continue;
      }

      if (p_member->type == ISNS_DD_ISCSI_MEMBER &&
          0 != strcmp (p_member->node_id, ptr))
      {
         __DEBUG (isns_dereg_debug &1, p_member->type not equal to type);
         continue;
      }

      if (p_member->type == ISNS_DD_IFCP_MEMBER &&
          0 != memcmp (p_member->node_id, ptr, ISNS_PORT_NAME_SIZE))
      {
         continue;
      }

      /* Match found */
      p_member->status = ISNS_DD_MEMBER_DISABLE;
      return (ISNS_NO_ERR);
   }

   __DEBUG (isns_dereg_debug &1, Disable_DD_Member return error);
   return (ERROR);
}

/*********************************************************************
Deregisters a SCN.
*********************************************************************/
int
ISNS_DeRegisterSCN ( ISNS_Msg_Descp * p_md, ISNS_Msg * rspmsg )
{
   int                  ii;
   int                  iscsiKeyIndex;
   int                  entityKeyIndex;
   int                  ifcpKeyIndex;
   int                  rval;
   ISNS_Key             *key;
   SOIP_Iscsi           *p_node;
   SOIP_ISCSI_Node_Id   db_node_name;
   SOIP_Entity          *p_entity;
   SOIP_Ifcp            *p_ifcp_node;
   ISNS_Attr            *src_attr;
   ISNS_Attr            *attr_indx[SNS_MAX_ATTRS];
   ISNS_Attr            *key_indx[SNS_MAX_ATTRS];

   rval = ISNS_UNKNOWN_ERR;

   ISNSParseMsg( &p_md->msg, attr_indx, key_indx, &src_attr );

   /* Scan the keys */
   iscsiKeyIndex = -1;
   ifcpKeyIndex = -1;
   entityKeyIndex = -1;

   for (ii = 0; ii < MAX_PARSE_ATTRS && key_indx[ii]; ii++)
   {
      key = (ISNS_Key *)(key_indx[ii]);
      switch (key->tag)
      {
      case ISNS_ISCSI_NODE_ID:
         iscsiKeyIndex = ii;
         break;
      case ISNS_ENTITY_ID:
         entityKeyIndex = ii;
         break;
      case ISNS_PORT_NAME:
         ifcpKeyIndex = ii;
         break;
      default:
         /* Error condition */
         break;
      }
   }

   if (iscsiKeyIndex != -1)
   {
      key = (ISNS_Key *)(key_indx[iscsiKeyIndex]);

      /* Fetch the node */
      memset (&db_node_name, 0, sizeof (db_node_name));
      __ISNS_COPY (db_node_name.v, sizeof(db_node_name.v), &key->val, key->len);

      rval = read_ISCSIObject(db_node_name.v, &p_node, &entry);
      if (rval != SUCCESS)
        return rval;

      p_node->scn_bitmap=0;
      p_node->scn_sockfd = 0;

      /* Store the node */
      write_ISCSIObject(db_node_name.v, entry);
   }
   else if (ifcpKeyIndex != -1)
   {
      key = (ISNS_Key *)(key_indx[ifcpKeyIndex]);

      rval = read_FCPortObject((char *)&key->val, &p_ifcp_node, &entry);
      if (rval != SUCCESS)
       return rval;

      p_ifcp_node->scn_bitmap=0;
      p_ifcp_node->scn_sockfd = 0;

      /* Store the node */
      rval = write_FCPortObject((char *)&key->val, entry);

      return (ISNS_NO_ERR);
   }
   else if (entityKeyIndex != -1)
   {
      key = (ISNS_Key *)(key_indx[entityKeyIndex]);

      /* Fetch entity from hash */
      if (ISNS_NO_ERR != read_EntityObject ((char *) &key->val, &p_entity, &entry))
      {
         return (ISNS_NO_SUCH_ENTRY_ERR);
      }

      p_entity->scn_bitmap=0;
      p_entity->scn_sockfd = 0;

      /* Store the node */
      write_EntityObject(p_entity->eid.id, entry);

      return (ISNS_NO_ERR);
   }
   else
   {
      __DEBUG (isns_dereg_debug & 1, (No valid key in SCN request));
      return (ISNS_MSG_FMT_ERR);
   }

   return (ISNS_NO_ERR);
}

/*********************************************************************
_RemoveCert 

Deallocates the cert space.
*********************************************************************/
int
RemoveCert ( void **ptr )
{
   if (*ptr)
   {
      ISNSFreeBuffer (*ptr);
      *ptr = NULL;
   }

   return (SUCCESS);
}

/**************************************************
Cycles through the entities.  If the entity's
period has expired, deregister.
**************************************************/
void
ISNS_CheckEntities (void)
{
   ISNS_DBKey  key = { 0 };
   SOIP_Entity *p_entity;
   time_t      t;
   int         secs;

   /* Get first entity id */
   key.tag = ENTITY_ID_KEY;
   key.len = 0;

   /* Get Time */
   secs = time (&t);

   while (SNSdbGetNextOfKey (&key) == SUCCESS)
   {
      /* Fetch entity from hash */

      if (ISNS_NO_ERR != read_EntityObject (key.val.entity_key.id, &p_entity, &entry))
      {
         return;
      }

      if (p_entity->period == 0)
         continue;

      if (secs<(int)p_entity->period+(int)p_entity->timestamp.t_time)
         continue;

      /* Period has expired, deregister */
      ISNSdbRemoveAttrEntityEntry (p_entity->eid.id, NULL, NULL);
   }
}

