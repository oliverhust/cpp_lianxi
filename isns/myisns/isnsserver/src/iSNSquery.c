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
 * This file contains source code for managing the
 * iSNS database. 
 *
 */
#include "iSNS.h"
#include "iSNSmsg.h"
#include "iSNSList.h"
#include "iSNSdb.h"
#include "iSNSobjects.h"
#include "iSNSquery.h"
#include "iSNSresponse.h"
#include "iSNSreg.h"
#include "iSNStypes.h"
#include "iSNScomm.h"
#include "iSNSbuffer.h"
#include "iSNStbl.h"
#include "iSNSparse.h"
#include "iSNSdebug.h"

/* Global */
extern int isns_query_debug;
extern uint32_t *dlist_src; /*[MAX_DD_PER_LIST];*/
extern uint32_t *dlist_node; /*[MAX_DD_PER_LIST];*/
extern SOIP_Dd_Member *ddmem; /* [100]; */
extern ISNS_Msg_Descp *p_rspMd;

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

/* local prototypes */
static int
SNSdbGetAttrNode (ISNS_Attr **attr_indx, ISNS_Attr **key_indx, ISNS_Msg_Descp * md,
                  ISNS_Msg * p_rspmsg);

static int
SNSdbGetAttrPort (ISNS_Attr **attr_indx, ISNS_Attr **key_indx, ISNS_Attr * src_attr,
                  ISNS_Msg_Descp * md, ISNS_Msg * p_rspmsg);

static int
SNSdbGetAttrPortal (ISNS_Attr **attr_indx, ISNS_Attr **key_indx, ISNS_Attr * src_attr,
                    ISNS_Msg_Descp * md, ISNS_Msg * p_rsp_msg);

static int
SNSdbGetAttrDDEntry (int id, ISNS_Attr **attr_indx, ISNS_Msg * p_msg);

int
ISNSGetNextAttr( ISNS_Msg_Descp *p_md, ISNS_Msg * p_rspmsg );

static int
SNSdbGetAttrDD (ISNS_Attr **attr_indx, ISNS_Attr **key_indx, ISNS_Attr * src_attr, ISNS_Msg_Descp * md,
                ISNS_Msg * p_rspmsg);

static int
SNSdbGetAttrDDSEntry (int id, ISNS_Attr **attr_indx, ISNS_Msg * p_msg);

static int
SNSdbGetAttrDDS (ISNS_Attr **attr_indx, ISNS_Attr **key_indx, ISNS_Attr * src_attr, 
                 ISNS_Msg_Descp * md, ISNS_Msg * p_rspmsg);

static int
ISNSdbGetAttrISCSI (ISNS_Attr **attr_indx, ISNS_Attr **key_indx, ISNS_Attr * src_attr,
                   ISNS_Msg_Descp * md, ISNS_Msg * p_rspmsg);

static int
SNSdbGetAttrEntity (ISNS_Attr **attr_indx, ISNS_Attr **key_indx, ISNS_Attr * src_attr,
                    ISNS_Msg_Descp * md, ISNS_Msg * p_rspmsg);

/*********************************************************************
_SNSdbGetAttr 

Calls the more specific get routines.
*********************************************************************/
int
ISNSdbGetAttr ( ISNS_Msg_Descp *p_md, ISNS_Msg * p_rspmsg )
{
   int         status = ISNS_UNKNOWN_ERR;
   ISNS_Key    *key;
   ISNS_Attr   *src_attr;
   ISNS_Attr   *attr_indx[SNS_MAX_ATTRS];
   ISNS_Attr   *key_indx[SNS_MAX_ATTRS];

   ISNSParseMsg( &p_md->msg, attr_indx, key_indx, &src_attr );
   key = (ISNS_Key *)(key_indx[0]);
   if (key == NULL)
   {
      __DEBUG (isns_query_debug & 1, (No attr in query msg));
      return ( ISNS_MSG_FMT_ERR );
   }
   switch ( key->tag )
   {
      case ISNS_ENTITY_ID:
      case ISNS_ENTITY_TYPE:
      case ISNS_MGMT_IP:
      case ISNS_PROT_VER:
      case ISNS_TIMESTAMP:
      case ISNS_ENTITY_CERT:
      case ISNS_ENTITY_PERIOD:
      case ISNS_ENTITY_IDX:
         /* Search entity table */
         status = SNSdbGetAttrEntity ( attr_indx, key_indx, 
                                       (ISNS_Attr *)src_attr, NULL, p_rspmsg);
         break;

      case ISNS_ESI_PORT:
      case ISNS_ESI_INTERVAL:
      case ISNS_PORTAL_IP:
      case ISNS_PORTAL_PORT:
      case ISNS_PORTAL_SYM_NAME:
      case ISNS_PORTAL_IDX:
         status = SNSdbGetAttrPortal ( attr_indx, key_indx, 
                                       (ISNS_Attr *)src_attr, NULL, p_rspmsg );
         break;

      case ISNS_NODE_NAME:
      case ISNS_NODE_SYM_NAME:
      case ISNS_FC_NODE_IP:
      case ISNS_FC_NODE_IPA:
      case ISNS_FC_NODE_CERT:
         status = SNSdbGetAttrNode ( attr_indx, key_indx, NULL, p_rspmsg );
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
         status = SNSdbGetAttrPort ( attr_indx, key_indx, 
                                     (ISNS_Attr *)src_attr, NULL, p_rspmsg );
         break;
      case ISNS_ISCSI_TYPE:
      case ISNS_ISCSI_NODE_ID:
      case ISNS_ISCSI_ALIAS:
      case ISNS_ISCSI_SCN_BITMAP:
      case ISNS_ISCSI_IDX:
         status = ISNSdbGetAttrISCSI ( attr_indx, key_indx, 
                                       (ISNS_Attr *)src_attr, NULL, p_rspmsg );
         break;

      case ISNS_DDS_ID:
      case ISNS_DDS_SYM_NAME:
      case ISNS_DDS_STATUS:
         status = SNSdbGetAttrDDS ( attr_indx, key_indx, (ISNS_Attr *)src_attr, NULL, p_rspmsg );
         break;

      case ISNS_DD_ID:
      case ISNS_DD_SYM_NAME:
      case ISNS_DD_ISCSI_MEMBER:
      case ISNS_DD_IFCP_MEMBER:
      case ISNS_DD_FEATURE_BITMAP:
      case ISNS_DD_ACTIVE:
         status = SNSdbGetAttrDD ( attr_indx, key_indx, (ISNS_Attr *)src_attr, NULL, p_rspmsg );
         break;

      default:
         break;
   }

   return ( status );
}

/*********************************************************************
_SNSdbGetAttrEntity

Gets one or more entity attributes.  The attributes will be appended
to p_rspmsg.
*********************************************************************/
static int
SNSdbGetAttrEntity (ISNS_Attr **attr_indx, ISNS_Attr **key_indx, ISNS_Attr * src_attr,
                    ISNS_Msg_Descp * md, ISNS_Msg * p_rspmsg)
{
   int ii;
   int entityIdxFlag;
   int entityKeyFlag;
   int entityTypeKeyFlag;
   int portalIpKeyFlag;
   int portalPortKeyFlag;
   int iSCSINodeKeyFlag;
   int iSCSINodeIdxKeyFlag;
   int iFCPNodeKeyFlag;
   int foundFlag;
   ISNS_Key *key;
   ISNS_Attr *attr;  /* query attr, response attr */
   SOIP_Entity *p_entity;
   SOIP_Portal *p_portal;
   SOIP_Ifcp *p_port;
   SOIP_Portal_Key db_portal;
   int rval;

   foundFlag = FALSE;

   entityIdxFlag = -1;
   entityKeyFlag = -1;
   entityTypeKeyFlag = -1;
   portalIpKeyFlag = -1;
   portalPortKeyFlag = -1;
   iSCSINodeKeyFlag = -1;
   iFCPNodeKeyFlag = -1;
   iSCSINodeIdxKeyFlag = -1;
   
   __DEBUG (isns_query_debug &1,(GetAttrEntity));

   ISNSTouchEntity( src_attr );

   for (ii = 0; ii < MAX_PARSE_ATTRS && key_indx[ii]; ii++)
   {
      key = (ISNS_Key *)(key_indx[ii]);
      switch (key->tag)
      {
         case ISNS_ENTITY_ID:
            if (key->len > 0)
               entityKeyFlag = ii;
            break;
         case ISNS_ENTITY_IDX:
            entityIdxFlag = ii;
            break;
         case ISNS_ENTITY_TYPE:
            entityTypeKeyFlag = ii;
            break;
         case ISNS_PORT_NAME:
            iFCPNodeKeyFlag = ii;
            break;
         case ISNS_ISCSI_NODE_ID:
            iSCSINodeKeyFlag = ii;
            break;
         case ISNS_ISCSI_IDX:
            iSCSINodeIdxKeyFlag = ii;
            break;
         case ISNS_PORTAL_IP:
            portalIpKeyFlag = ii;
            break;
         case ISNS_PORTAL_PORT:
            portalPortKeyFlag = ii;
            break;

         default:
            break;
      }
   }

   if ( entityKeyFlag != -1 )
   {
      entityKeyFlag = ISNSFindTag (0, ISNS_ENTITY_ID, key_indx);

      while (entityKeyFlag != -1)
      {
         /* Entity ID found in key */
         SOIP_Entity_Id db_entity_id;
         key = (ISNS_Key *)(key_indx[entityKeyFlag]);
         __ISNS_COPY (db_entity_id.id, sizeof(db_entity_id.id), &key->val, key->len);

         if (ISNS_NO_ERR != read_EntityObject (db_entity_id.id, &p_entity, &entry))
         {
            return (ISNS_INVALID_QUERY_ERR);
         }

         rval = Check_Permission (src_attr, ENTITY_ID_KEY, p_entity);
         if (rval == SUCCESS)
         { 
           foundFlag = TRUE;
           ISNSdbProcessEntityOpAttr (p_rspmsg, (ISNS_Attr **)attr_indx, p_entity);
         }
         entityKeyFlag = ISNSFindTag (entityKeyFlag + 1, ISNS_ENTITY_ID, key_indx);
      }
   }
   else if ( entityIdxFlag != -1 )
   {
      entityKeyFlag = ISNSFindTag (0, ISNS_ENTITY_IDX, key_indx);

      while (entityKeyFlag != -1)
      {
         void *ptr;

         /* Entity ID found in key */
         key = (ISNS_Key *)(key_indx[entityKeyFlag]);
         __DEBUG (isns_query_debug &1,(Find Entity Index:%i),key->val.index);

         rval = read_EntityidxObject(key->val.index, &ptr, &entry3);
         if (rval != SUCCESS)
            return rval;

         /* Fetch entity from hash */
         if (ISNS_NO_ERR != read_EntityObject ((char *)ptr, &p_entity, &entry ))
         {
            return (ISNS_INVALID_QUERY_ERR);
         }

         rval = Check_Permission (src_attr, ENTITY_ID_KEY, p_entity);
         if (rval == SUCCESS)
         { 
           foundFlag = TRUE;
           ISNSdbProcessEntityOpAttr (p_rspmsg, (ISNS_Attr **)attr_indx, p_entity);
         }
         entityKeyFlag = ISNSFindTag (entityKeyFlag + 1, ISNS_ENTITY_IDX, key_indx);
      }
   }
   else if ((portalIpKeyFlag != -1) || (portalPortKeyFlag != -1))
   {
      if ((portalIpKeyFlag == -1) || (portalPortKeyFlag == -1))
      {
         __LOG_ERROR ("***Located only one required portal key.");
         return (ISNS_MSG_FMT_ERR);
      }

      attr = (ISNS_Attr *) key_indx[portalPortKeyFlag];
      __ISNS_COPY (db_portal.ip_addr.v, sizeof(db_portal.ip_addr.v), &attr->val, attr->len);

      attr = (ISNS_Attr *) key_indx[portalIpKeyFlag];
      db_portal.ip_port=attr->val.ip_port;

      rval = read_PortalObject(&db_portal, &p_portal, &entry2);
      if (rval != SUCCESS)
         return rval;

      /* Fetch entity from hash */
      if (ISNS_NO_ERR !=
             read_EntityObject (p_portal->entity_id.id, &p_entity, &entry))
      {
            return (ISNS_INVALID_QUERY_ERR);
      }

      rval = Check_Permission (src_attr, ENTITY_ID_KEY, p_entity);
      if (rval == SUCCESS)
      { 
           foundFlag = TRUE;
           ISNSdbProcessEntityOpAttr (p_rspmsg, (ISNS_Attr **)attr_indx, p_entity);
      }
   }
   else if (iFCPNodeKeyFlag != -1)
   {
      /* Fetch iFCP Node */
      key = (ISNS_Key *) key_indx[iFCPNodeKeyFlag];
      
      rval = read_FCPortObject((char *)&key->val, &p_port, &entry);
      if (rval != SUCCESS)
         return rval;

      /* Fetch entity from hash */
      if (ISNS_NO_ERR !=
          read_EntityObject (p_port->entity_id.id, &p_entity, &entry))
      {
         return (ISNS_NO_SUCH_ENTRY_ERR);
      }

      rval = Check_Permission (src_attr, ENTITY_ID_KEY, p_entity);
      if (rval == SUCCESS)
      { 
         foundFlag = TRUE;
         ISNSdbProcessEntityOpAttr (p_rspmsg, (ISNS_Attr **)attr_indx, p_entity);
      }

   }
   else if (iSCSINodeKeyFlag != -1 || iSCSINodeIdxKeyFlag != -1)
   {
      SOIP_ISCSI_Node_Id db_node_name;
      SOIP_Iscsi *p_node;
      void *ptr;

      memset (&db_node_name, 0, sizeof (db_node_name));
      if (iSCSINodeIdxKeyFlag != -1)
      {
         key = (ISNS_Key *)(key_indx[iSCSINodeIdxKeyFlag]);
         rval = read_ISCSIidxObject(key->val.index, &ptr, &entry3);
         if (rval != SUCCESS)
            return rval;

         strncpy ((char *) db_node_name.v, (char *) ptr, sizeof(db_node_name.v));

      }
      else
      {

         /* Fetch iSCSI Node */
         key = (ISNS_Key *)(key_indx[iSCSINodeKeyFlag]);
         memset (&db_node_name, 0, sizeof (db_node_name));
         __ISNS_COPY (db_node_name.v, sizeof(db_node_name.v), &key->val, key->len);

      }

      rval = read_ISCSIObject(db_node_name.v, &p_node, &entry);
      if (rval != SUCCESS)
        return rval;

      rval = read_EntityObject (p_node->entity_id.id, &p_entity, &entry);
      if (rval != SUCCESS)
        return rval;

      rval = Check_Permission (src_attr, ENTITY_ID_KEY, p_entity);
      if (rval == SUCCESS)
      { 
        foundFlag = TRUE;
        ISNSdbProcessEntityOpAttr (p_rspmsg, (ISNS_Attr **)attr_indx, p_entity);
      }

   }
   else
   {
      /* Return all entity */
      ISNS_DBKey key = { 0 };
      key.tag = ENTITY_ID_KEY;

      while (SNSdbGetNextOfKey (&key) == SUCCESS)
      {
         if (ISNS_NO_ERR != read_EntityObject (key.val.entity_key.id, &p_entity, &entry))
         {
            return (ISNS_NO_SUCH_ENTRY_ERR);
         }

         rval = Check_Permission (src_attr, ENTITY_ID_KEY, p_entity);
         if (rval == SUCCESS)
         {
           foundFlag = TRUE;
           ISNSdbProcessEntityOpAttr (p_rspmsg, (ISNS_Attr **)attr_indx, p_entity);
         }
      }
   }

   if (foundFlag)
      return (ISNS_NO_ERR);
   else
      return (ISNS_NO_SUCH_ENTRY_ERR);
}

/*********************************************************************
_SNSdbGetAttrDDSEntry 

Retrieves the DDS entry from the database.
*********************************************************************/
static int
SNSdbGetAttrDDSEntry (int id, ISNS_Attr **attr_indx, ISNS_Msg * p_msg)
{
   SOIP_Dds *p_dds;
   int rval;

   rval = read_DDSObject(id, &p_dds, &entry);
   if (rval != SUCCESS)
      return rval;

   ISNSdbProcessDDSOpAttr(p_msg, (ISNS_Attr **) attr_indx, p_dds);
   return (ISNS_NO_ERR);
}

/*********************************************************************
_SNSdbGetAttrDDS

Determines what needs to be done to return the DDS attributes.
*********************************************************************/
static int
SNSdbGetAttrDDS (ISNS_Attr **attr_indx, ISNS_Attr **key_indx, ISNS_Attr * src_attr, 
                 ISNS_Msg_Descp * md, ISNS_Msg * p_rspmsg)
{
   int ii;
   int rval;
   int ddsKeyIndex;
   int ddKeyIndex;
   int iscsiKeyIndex;
   int ifcpKeyIndex;
   int ddsSymIndex;
   int found;
   ISNS_Key *key;
   ISNS_Attr *p_attr;
   SOIP_Dds *p_dds;

   rval = Check_Authorization (src_attr);
   if (rval != SUCCESS)
      return (ISNS_AUTH_FAILED_ERR);

   ISNSTouchEntity( src_attr );

   found = FALSE;

   ddsKeyIndex = -1;
   ddKeyIndex = -1;
   iscsiKeyIndex = -1;
   ifcpKeyIndex = -1;
   ddsSymIndex = -1;

   for (ii = 0; ii < MAX_PARSE_ATTRS && key_indx[ii]; ii++)
   {
      key = (ISNS_Key *)(key_indx[ii]);
      switch (key->tag)
      {
         case ISNS_ISCSI_NODE_ID:
            iscsiKeyIndex = ii;
            break;
         case ISNS_PORT_NAME:
            ifcpKeyIndex = ii;
            break;
         case ISNS_DD_ID:
            if (key->val.dd_id)
               ddKeyIndex = ii;
            break;
         case ISNS_DDS_ID:
            if (key->val.dds_id)
               ddsKeyIndex = ii;
            break;
         case ISNS_DDS_SYM_NAME:
            ddsSymIndex = ii;
            break;
         default:
            break;
      }
   }

   if (ddsKeyIndex != -1)
   {
      key = (ISNS_Key *)(key_indx[ddsKeyIndex]);
      rval = SNSdbGetAttrDDSEntry (*(uint32_t *) & key->val,
                  attr_indx, p_rspmsg);

      if (!rval)
         found = TRUE;
   }
   else if (ddKeyIndex != -1)
   {
      __DEBUG(isns_query_debug & 1, (Not implemented));
   }
   else if (ifcpKeyIndex != -1)
   {
      __DEBUG (isns_query_debug & 1, (Not implemented));
   }
   else if (iscsiKeyIndex != -1)
   {
      __DEBUG (isns_query_debug & 1, (Not implemented));
   }
   else
   {
      /* retrieve all DDS */

      ISNS_DBKey key = { 0 };
      key.tag = DDS_ID_KEY;

      while (SNSdbGetNextOfKey (&key) == SUCCESS)
      {
         if (ddsSymIndex!=-1)
         {
            rval = read_DDSObject(key.val.dds_key.id, &p_dds, &entry);
            if (rval != SUCCESS)
                return rval;

            p_attr = (ISNS_Attr *)attr_indx[ddsSymIndex];

            if (strcmp(p_dds->sym_name, (char *)&p_attr->val))
               continue;
         }
         if (ERROR ==
             SNSdbGetAttrDDSEntry (key.val.dd_key.id, attr_indx, p_rspmsg))
            return (ERROR);
         found = TRUE;
      }
   }

   if (found != TRUE)
   {
      return (ISNS_NO_SUCH_ENTRY_ERR);
   }

   return (ISNS_NO_ERR);
}


/*********************************************************************
_SNSdbGetAttrDDEntry 

Retrieves a DD entry from the database.
*********************************************************************/
static int
SNSdbGetAttrDDEntry (int id, ISNS_Attr **attr_indx, ISNS_Msg * p_msg)
{
   SOIP_Dd *p_dd;
   int rval;

   __DEBUG (isns_query_debug &1, SNSdbGetAttrDDEntry);

   rval = read_DDObject(id, &p_dd, &entry);

   if (rval == SUCCESS)
      ISNSdbProcessDDOpAttr( p_msg, (ISNS_Attr **) attr_indx, p_dd );
   
   return ( rval );
}

/*********************************************************************
_SNSdbGetAttrDD 

Determines what to do to retrieve a DD's attributes
*********************************************************************/
static int
SNSdbGetAttrDD (ISNS_Attr **attr_indx, ISNS_Attr **key_indx, ISNS_Attr * src_attr, ISNS_Msg_Descp * md,
                ISNS_Msg * p_rspmsg)
{
   int ddKeyIndex;
   int nodeKeyIndex;
   int entityKeyIndex;
   int ifcpKeyIndex;
   int ddsKeyIndex;
   int found;
   ISNS_Key *key;
   SOIP_Dds *p_dds;
   int ii;
   int rval;
   SOIP_DD_Key dds_key;
   SOIP_Iscsi *p_node;
   ISNS_LIST_NODE *pnode;

   __DEBUG (isns_query_debug &1, SNSdbGetAttrDD);

   rval = Check_Authorization (src_attr);
   if (rval != SUCCESS)
      return (ISNS_AUTH_FAILED_ERR);

   ISNSTouchEntity( src_attr );

   nodeKeyIndex = -1;
   entityKeyIndex = -1;
   ifcpKeyIndex = -1;
   ddKeyIndex = -1;
   ddsKeyIndex = -1;

   found = FALSE;
   for (ii = 0; ii < MAX_PARSE_ATTRS && key_indx[ii]; ii++)
   {
      key = (ISNS_Key *)(key_indx[ii]);
      switch ( key->tag )
      {
         case ISNS_ISCSI_NODE_ID:
            nodeKeyIndex = ii;
            break;
         case ISNS_ENTITY_ID:
            entityKeyIndex = ii;
            break;
         case ISNS_PORT_NAME:
            ifcpKeyIndex = ii;
            break;
         case ISNS_DD_ID:
            if ( key->val.dd_id )
               ddKeyIndex = ii;
            break;
         case ISNS_DDS_ID:
            if ( key->val.dds_id )
               ddsKeyIndex = ii;
            break;
         default:
            /* Error condition */
            break;
      }
   }

   if (ddKeyIndex != -1)
   {
      key = (ISNS_Key *)(key_indx[ddKeyIndex]);
      if (SUCCESS !=
          (rval =
           SNSdbGetAttrDDEntry (*(uint32_t *) & key->val, attr_indx,
                                p_rspmsg)))
         return (rval);
      found = TRUE;
   }
   else if (ddsKeyIndex != -1)
   {
      key = (ISNS_Key *)(key_indx[ddsKeyIndex]);
      dds_key.id = key->val.dd_id;
      rval = read_DDSObject(dds_key.id, &p_dds, &entry3);
      if (rval != SUCCESS)
        return rval;

      pnode=NULL;
      while ((pnode=GetNextNode(&p_dds->dd_list, pnode)))
      {
            if (SUCCESS !=
                (rval =
                 SNSdbGetAttrDDEntry (*(uint32_t *)GetNodeData(pnode), attr_indx,
                                      p_rspmsg)))
               return (rval);
            found = TRUE;
      }

   }
   else if (nodeKeyIndex != -1)
   {
      key = (ISNS_Key *)(key_indx[nodeKeyIndex]);

      rval = read_ISCSIObject((char *)&key->val, &p_node, &entry3);
      if (rval != SUCCESS)
         return rval;

      pnode=NULL;
      while ((pnode=GetNextNode(&p_node->dd_id_list, pnode)))
      {
            if (SUCCESS !=
                (rval =
                 SNSdbGetAttrDDEntry (*(uint32_t *)GetNodeData(pnode), attr_indx,
                                      p_rspmsg)))
               return (rval);
            found = TRUE;
      }
   }
   else
   {
      /* retrieve all DD */
      ISNS_DBKey key = { 0 };
      key.tag = DD_ID_KEY;

      while (SNSdbGetNextOfKey (&key) == SUCCESS)
      {
         if (SUCCESS !=
             (rval =
              SNSdbGetAttrDDEntry (key.val.dd_key.id, attr_indx, p_rspmsg)))
            return (rval);
         found = TRUE;
      }
   }

   if (found != TRUE)
   {
      return (ISNS_NO_SUCH_ENTRY_ERR);
   }

   return (ISNS_NO_ERR);
}

/*********************************************************************
*********************************************************************/
static int
SNSdbGetAttrNode (ISNS_Attr **attr_indx, ISNS_Attr **key_indx, ISNS_Msg_Descp * md,
                  ISNS_Msg * p_rspmsg)
{
   int ii;
   ISNS_Key *key;
   SOIP_Fc_Node *p_node;
   SOIP_Ifcp *p_port;
   SOIP_DB_Node_Name db_node_name;
   SOIP_Port_Name db_port_name;
   char *p_node_name = NULL;
   int nodeKeyFlag, portKeyFlag;
   int rval;

   /* Scan the keys */
   nodeKeyFlag = 0;
   portKeyFlag = 0;

   for (ii = 0; ii < MAX_PARSE_ATTRS && key_indx[ii]; ii++)
   {
      key = (ISNS_Key *)(key_indx[ii]);
      switch (key->tag)
      {
      case ISNS_PORT_NAME:
         portKeyFlag = TRUE;
         break;
      case ISNS_NODE_NAME:
         p_node_name = (char *)&key->val;
         nodeKeyFlag = TRUE;
         break;
      default:
         /* Error condition */
         break;

      }
   }

   /* Order is important */
   if (nodeKeyFlag)
   {
      /* Fetch the node */
      memset (&db_node_name, 0, sizeof (db_node_name));
      __ISNS_COPY (db_node_name.v, sizeof(db_node_name.v), p_node_name, ISNS_NODE_NAME_SIZE);
      db_node_name.v[8] = (char)SOIP_DB_Node_Id;

      rval = read_FCNodeObject(db_node_name.v, &p_node, &entry);
      if (rval != SUCCESS)
        return rval;

      ISNSdbProcessFCNodeOpAttr (p_rspmsg, (ISNS_Attr **)attr_indx, p_node);
   }
   else if (portKeyFlag)
   {
      /* Fetch the port */
      memset (&db_port_name, 0, sizeof (db_port_name));
      key = (ISNS_Key *)(key_indx[ii]);
      __ISNS_COPY (db_port_name.v, sizeof(db_port_name.v), &key->val, key->len);

      rval = read_FCPortObject(db_port_name.v, &p_port, &entry);
      if (rval != SUCCESS)
        return rval;

      key = (ISNS_Key *)(key_indx[nodeKeyFlag]);

      if (!memcmp (p_port->node_name.v, &key->val, ISNS_NODE_NAME_SIZE))
      {
         /* Match Found */

         /* Fetch the Node */
         memset (&db_node_name, 0, sizeof (db_node_name));
         __ISNS_COPY (db_node_name.v, sizeof(db_node_name.v), p_port->node_name.v, ISNS_NODE_NAME_SIZE);
         db_node_name.v[8] = (char)SOIP_DB_Node_Id;

         rval = read_FCNodeObject(db_node_name.v, &p_node, &entry);
         if (rval != SUCCESS)
            return rval;

         ISNSdbProcessFCNodeOpAttr (p_rspmsg, (ISNS_Attr **)attr_indx, p_node);
      }
   }
   else
   {
      /* Search the node table */
      ISNS_DBKey key = { 0 };
      key.tag = NODE_NAME_KEY;
      
      while (SNSdbGetNextOfKey (&key) == SUCCESS)
      {
         memset (&db_node_name, 0, sizeof (db_node_name));
         __ISNS_COPY (db_node_name.v, sizeof(db_node_name.v), key.val.node_key.v, ISNS_NODE_NAME_SIZE);
         db_node_name.v[8] = (char)SOIP_DB_Node_Id;

         rval = read_FCNodeObject(db_node_name.v, &p_node, &entry);
         if (rval != SUCCESS)
            return rval;

         ISNSDisplay_FCNode (p_node);
         ISNSdbProcessFCNodeOpAttr (p_rspmsg, (ISNS_Attr **)attr_indx, p_node);

      }
   }

   return ISNS_NO_ERR;
}

/*********************************************************************
*********************************************************************/
static int
SNSdbGetAttrPortal (ISNS_Attr **attr_indx, ISNS_Attr **key_indx, ISNS_Attr * src_attr,
                    ISNS_Msg_Descp * md, ISNS_Msg * p_rsp_msg)
{
   int                  ii;
   ISNS_Key             *key = NULL;
   ISNS_Attr            *attr;
   SOIP_Portal          *p_portal;
   SOIP_Entity          *p_entity;
   SOIP_Ifcp            *p_port;
   SOIP_Portal_Key      db_portal;
   int                  entityKeyFlag;
   int                  portalIpKeyFlag;
   int                  portalPortKeyFlag;
   int                  portalIdxKeyFlag;
   int                  iSCSINodeIdKeyFlag;
   int                  portNameKeyFlag;
   int                  foundFlag;
   ISNS_LIST_NODE       *pnode;
   char                 *p_portal_name;
   ISNS_DBKey           key2 = { 0 };
   int                  rval;


   __DEBUG (isns_query_debug &1, SNSdbGetAttrPortal);

   foundFlag = FALSE;

   /* Scan the keys */
   entityKeyFlag = -1;
   portalIpKeyFlag = -1;
   portalPortKeyFlag = -1;
   iSCSINodeIdKeyFlag = -1;
   portNameKeyFlag = -1;
   portalIdxKeyFlag = -1;

   for (ii = 0; ii < MAX_PARSE_ATTRS && key_indx[ii]; ii++)
   {
      key = (ISNS_Key *)(key_indx[ii]);
      switch (key->tag)
      {
      case ISNS_ENTITY_ID:
         entityKeyFlag = ii;
         break;
      case ISNS_PORTAL_IP:
         if (key->len > 0)
            portalIpKeyFlag = ii;
         break;
      case ISNS_PORTAL_PORT:
         portalPortKeyFlag = ii;
         break;
      case ISNS_ISCSI_NODE_ID:
         iSCSINodeIdKeyFlag = ii;
         break;
      case ISNS_PORT_NAME:
         portNameKeyFlag = ii;
         break;
      case ISNS_PORTAL_IDX:
         portalIdxKeyFlag = ii;
         break;
      default:
         break;

      }
   }

   if (portalPortKeyFlag != -1 || portalIpKeyFlag != -1)
   {
      /* Fetch Portal */
      if (portalPortKeyFlag == -1 || portalIpKeyFlag == -1)
      {
         __LOG_ERROR ("***Located only one required portal key.");
         return (ISNS_MSG_FMT_ERR);
      }

      attr = (ISNS_Attr *) key_indx[portalPortKeyFlag];
      db_portal.ip_port=attr->val.ip_port;

      attr = (ISNS_Attr *) key_indx[portalIpKeyFlag];
      __ISNS_COPY (db_portal.ip_addr.v, sizeof(db_portal.ip_addr.v), &attr->val, attr->len);

      rval = read_PortalObject(&db_portal, &p_portal, &entry2);
      __DEBUG (isns_query_debug &1,Read Portal from database rval:%i,rval);
      if (rval != SUCCESS)
         return rval;

      rval = Check_Permission (src_attr, PORTAL_ID_KEY, p_portal);
      if (rval == SUCCESS)
      {
        SNSProcessPortalOpAttr (p_rsp_msg, (ISNS_Attr **)attr_indx, p_portal);
        foundFlag = TRUE;
      }
   }
   else if (portalIdxKeyFlag != -1)
   {
      void *ptr;
      /* Fetch Portal */

      attr = (ISNS_Attr *) key_indx[portalIdxKeyFlag];
      rval = read_PortalidxObject(attr->val.index, &ptr, &entry3);
      if (rval != SUCCESS)
          return rval;

      __ISNS_COPY (&db_portal, sizeof(db_portal), ptr, sizeof(db_portal));

      rval = read_PortalObject(&db_portal, &p_portal, &entry2);
      if (rval != SUCCESS)
         return rval;

      rval = Check_Permission (src_attr, PORTAL_ID_KEY, p_portal);
      if (rval == SUCCESS)
      {
        SNSProcessPortalOpAttr (p_rsp_msg, (ISNS_Attr **)attr_indx, p_portal);
        foundFlag = TRUE;
      }
   }
   else if (entityKeyFlag != -1)
   {
      /* Fetch entity from hash */
      if (ISNS_NO_ERR != read_EntityObject ((char *)&key->val, &p_entity, &entry))
      {
         return (ISNS_NO_SUCH_ENTRY_ERR);
      }

      /* Cycle through all portal */
      pnode=NULL;
      while ((pnode=GetNextNode(&p_entity->iportal_list, pnode)))
      {
            rval = read_PortalObject(GetNodeData(pnode), &p_portal, &entry2);
            if (rval != SUCCESS)
              return rval;

            rval = Check_Permission (src_attr, PORTAL_ID_KEY, p_portal);
            if (rval == SUCCESS)
            {
              SNSProcessPortalOpAttr (p_rsp_msg, (ISNS_Attr **)attr_indx, p_portal);
              foundFlag = TRUE;
            }
      }                         /* end for */
   }
   else if (portNameKeyFlag != -1)
   {
      /* Fetch iFCP Node */
      key = (ISNS_Key *)(key_indx[portNameKeyFlag]);

      rval = read_FCPortObject(key->val.port_name.v, &p_port, &entry);
      if (rval != SUCCESS)
        return rval;

      /* Fetch Entity */
      if (ISNS_NO_ERR !=
          read_EntityObject (p_port->entity_id.id, &p_entity, &entry))
      {
         return (ERROR);
      }

      /* Cycle through all portal */
      pnode=NULL;
      while ((pnode=GetNextNode(&p_entity->iportal_list, pnode)))
      {
            rval = read_PortalObject(GetNodeData(pnode), &p_portal, &entry2);
            if (rval != SUCCESS)
               return rval;

            rval = Check_Permission (src_attr, PORTAL_ID_KEY, p_portal);
            if (rval == SUCCESS)
            {
              SNSProcessPortalOpAttr (p_rsp_msg, (ISNS_Attr **)attr_indx, p_portal);
              foundFlag = TRUE;
            }
      }                         /* end for */

   }
   else if (iSCSINodeIdKeyFlag != -1)
   {
      SOIP_ISCSI_Node_Id db_node_name;
      SOIP_Iscsi *p_node;

      /* Fetch iSCSI Node */
      key = (ISNS_Key *)(key_indx[iSCSINodeIdKeyFlag]);
      memset (&db_node_name, 0, sizeof (db_node_name));
      __ISNS_COPY (db_node_name.v, sizeof(db_node_name.v), &key->val, key->len);

      rval = read_ISCSIObject(db_node_name.v, &p_node, &entry);
      if (rval != SUCCESS)
        return rval;
  
      rval = Check_Permission (src_attr, ISCSI_ID_KEY, p_node);
      if (rval != SUCCESS)
      {
            __DEBUG(isns_query_debug & 1, "***ERROR Invalid dd");
            return (ISNS_NO_SUCH_ENTRY_ERR);
      }

      rval = read_EntityObject (p_node->entity_id.id, &p_entity, &entry);
      if (rval != SUCCESS)
        return rval;

      /* Cycle through all portal */
      pnode=NULL;
      while ((pnode=GetNextNode(&p_entity->iportal_list, pnode)))
      {
            rval = read_PortalObject(GetNodeData(pnode), &p_portal, &entry2);
            if (rval != SUCCESS)
                return rval;

            SNSProcessPortalOpAttr (p_rsp_msg, (ISNS_Attr **)attr_indx, p_portal);
            foundFlag = TRUE;
      }                         /* end for */
   }
   else
   {
      /* No Key */
      SOIP_Portal_Key db_portal_key;
      p_portal_name = NULL;
      key2.tag = PORTAL_ID_KEY;
      key2.len = 0;

      while (SNSdbGetNextOfKey(&key2) == SUCCESS)
      {
         __ISNS_COPY(&db_portal_key, sizeof(SOIP_Portal_Key), &key2.val.portal_key, sizeof(SOIP_Portal_Key));
         __DEBUG (isns_query_debug &1,read next portal db entry);
         rval = read_PortalObject(&db_portal_key, &p_portal, &entry2);
         if (rval != SUCCESS)
            return rval;

         rval = Check_Permission (src_attr, PORTAL_ID_KEY, p_portal);
         if (rval == SUCCESS)
         {
           SNSProcessPortalOpAttr (p_rsp_msg, (ISNS_Attr **)attr_indx, p_portal);
           foundFlag = TRUE;
         }
      }
   }

   if (!foundFlag)
      return (ISNS_NO_SUCH_ENTRY_ERR);

   return (ISNS_NO_ERR);
}

/*********************************************************************
_SNSdbGetAttrPort

Gets the attributes for an iFCP node query.
*********************************************************************/
static int
SNSdbGetAttrPort (ISNS_Attr **attr_indx, ISNS_Attr **key_indx, ISNS_Attr * src_attr,
                  ISNS_Msg_Descp * md, ISNS_Msg * p_rspmsg)
{
   int ii;
   ISNS_Key *key;
   ISNS_Attr *attr;
   SOIP_Ifcp *p_port;
   SOIP_Entity *p_entity;
   SOIP_Portal *p_portal;
   SOIP_Portal_Key db_portal;
   int entityKeyFlag;
   int portKeyFlag;
   int fc4TypesKeyFlag;
   int portalIpKeyFlag;
   int portalPortKeyFlag;
   int nodeKeyFlag;
   int foundFlag;
   ISNS_LIST_NODE *pnode;
   int rval;
   SOIP_DB_Node_Name db_node_name;
   SOIP_Fc_Node *p_node;


   foundFlag = FALSE;

   /* Scan the keys */
   entityKeyFlag = -1;
   portKeyFlag = -1;
   fc4TypesKeyFlag = -1;
   portalIpKeyFlag = -1;
   portalPortKeyFlag = -1;
   nodeKeyFlag = -1;

   for (ii = 0; ii < MAX_PARSE_ATTRS && key_indx[ii]; ii++)
   {
      key = (ISNS_Key *)(key_indx[ii]);
      switch (key->tag)
      {
      case ISNS_ENTITY_ID:
         entityKeyFlag = ii;
         break;
      case ISNS_PORT_NAME:
         portKeyFlag = ii;
         break;
      case ISNS_FC4_TYPE:
         fc4TypesKeyFlag = ii;
         break;
      case ISNS_PORTAL_IP:
         portalIpKeyFlag = ii;
         break;
      case ISNS_PORTAL_PORT:
         portalPortKeyFlag = ii;
         break;
      case ISNS_NODE_NAME:
         nodeKeyFlag = ii;
         break;
      default:
         break;
      }
   }

   if (portKeyFlag != -1)
   {
      portKeyFlag = ISNSFindTag (0, ISNS_PORT_NAME, key_indx);
      do
      {
         key = (ISNS_Key *) key_indx[portKeyFlag];

         rval = read_FCPortObject(key->val.port_name.v, &p_port, &entry);
         if (rval != SUCCESS)
            return rval;

         rval = Check_Permission (src_attr, PORT_NAME_KEY, p_port);
         if (rval == SUCCESS)
         {
           ISNSdbProcessPortOpAttr (p_rspmsg, (ISNS_Attr **)attr_indx, p_port);
           foundFlag = TRUE;
         }
         portKeyFlag = ISNSFindTag (portKeyFlag + 1, ISNS_PORT_NAME, key_indx);
      }
      while (portKeyFlag != -1);

   }
   else if (nodeKeyFlag != -1)
   {
      /* Fetch the node */
      key = (ISNS_Key *)(key_indx[nodeKeyFlag]);
      memset (&db_node_name, 0, sizeof (db_node_name));
      __ISNS_COPY (db_node_name.v, sizeof(db_node_name.v), &key->val, ISNS_NODE_NAME_SIZE);
      db_node_name.v[8] = (char)SOIP_DB_Node_Id;

      rval = read_FCNodeObject(db_node_name.v, &p_node, &entry);
      if (rval != SUCCESS)
        return rval;

      pnode=NULL;
      while ((pnode=GetNextNode(&p_node->port_list, pnode)))
      {
            char *pname=GetNodeData(pnode);

            rval = read_FCPortObject(pname, &p_port, &entry2);
            if ( rval != SUCCESS)
              return rval;

            rval = Check_Permission (src_attr, PORT_NAME_KEY, p_port);
            if (rval == SUCCESS)
            {
              ISNSdbProcessPortOpAttr (p_rspmsg, (ISNS_Attr **)attr_indx, p_port);
              foundFlag = TRUE;
            }
      }
   }
   else if (entityKeyFlag != -1)
   {
      SOIP_Entity_Key db_entity_id={{0}};

      key = (ISNS_Key *)(key_indx[entityKeyFlag]);
      strncpy ( db_entity_id.id, (char *)&key->val, sizeof(db_entity_id.id) );

      rval = read_EntityObject (db_entity_id.id, &p_entity, &entry);
      if (rval != SUCCESS)
        return rval;

      /* Scan the entity's port list */
      pnode=NULL;
      while ((pnode=GetNextNode(&p_entity->iportal_list, pnode)))
      {
         /* Fetch the Port Object */
         rval = read_FCPortObject(GetNodeData(pnode), &p_port, &entry2);
         if (rval != SUCCESS)
           return rval;

         rval = Check_Permission (src_attr, PORT_NAME_KEY, p_port);
         if (rval == SUCCESS)
         {
           ISNSdbProcessPortOpAttr (p_rspmsg, (ISNS_Attr **)attr_indx, p_port);
           foundFlag = TRUE;
         }
      }

   }
   else if ((portalIpKeyFlag != -1) || (portalPortKeyFlag != -1))
   {
      if ((portalIpKeyFlag == -1) || (portalPortKeyFlag == -1))
      {
         __LOG_ERROR ("***Located only one required portal key.");
         return (ISNS_INVALID_QUERY_ERR);
      }

      attr = (ISNS_Attr *) key_indx[portalPortKeyFlag];
      __ISNS_COPY (db_portal.ip_addr.v, sizeof(db_portal.ip_addr.v), &attr->val, attr->len);

      attr = (ISNS_Attr *) key_indx[portalIpKeyFlag];
      db_portal.ip_port=attr->val.ip_port;

      rval = read_PortalObject(&db_portal, &p_portal, &entry2);
      if (rval != SUCCESS)
        return rval;
      else
      {
         rval = read_EntityObject(p_portal->entity_id.id, &p_entity, &entry);
         if (rval != SUCCESS)
           return rval;

         /* Scan the entity's port list */
         pnode=NULL;
         while ((pnode=GetNextNode(&p_entity->ifcp_node_list, pnode)))
         {
            rval = read_FCPortObject(GetNodeData(pnode), &p_port, &entry2);
            if (rval != SUCCESS)
               return rval;

            rval = Check_Permission (src_attr, PORT_NAME_KEY, p_port);
            if (rval == SUCCESS)
            {
              ISNSdbProcessPortOpAttr (p_rspmsg, (ISNS_Attr **)attr_indx, p_port);
              foundFlag = TRUE;
            }
         }
      }
   }
   else if (fc4TypesKeyFlag != -1)
   {
      __DEBUG (isns_query_debug & 1, (Not Implemented));
      return (ERROR);
   }
   else
   {
      /* Return all Ports */
      ISNS_DBKey key = { 0 };
      key.tag = PORT_NAME_KEY;

      while (SNSdbGetNextOfKey (&key) == SUCCESS)
      {
         rval = read_FCPortObject(key.val.port_key.v, &p_port, &entry);
         if (rval != SUCCESS)
            return rval;

         ISNSDisplay_iFCPNode (p_port);
         ISNSdbProcessPortOpAttr (p_rspmsg, (ISNS_Attr **)attr_indx, p_port);
         foundFlag = TRUE;
      }
   }

   if (!foundFlag)
      return (ISNS_NO_SUCH_ENTRY_ERR);

   return ISNS_NO_ERR;
}

/*********************************************************************
This is called during a query for iSCSI attributes.
*********************************************************************/
static int
ISNSdbGetAttrISCSI (ISNS_Attr **attr_indx, ISNS_Attr **key_indx, ISNS_Attr * src_attr,
                   ISNS_Msg_Descp * md, ISNS_Msg * p_rspmsg)
{
   int                foundFlag;
   int                ii;
   int                nodeKeyIndex;
   int                nodeTypeKeyIndex;
   int                nodeIdxKeyIndex;
   int                entityKeyIndex;
   int                portalIpKeyIndex;
   int                portalPortKeyIndex;
   SOIP_Entity        *p_entity;
   ISNS_Key           *key=NULL;
   SOIP_Iscsi         *p_node;
   SOIP_ISCSI_Node_Id db_node_name;
   ISNS_LIST_NODE     *pnode;
   int                rval;
   int            rc;

   __DEBUG (isns_query_debug &1,(Attribute Query - ISCSI:%s),(char *)&src_attr->val);

   foundFlag = FALSE;

   ISNSTouchEntity( src_attr );

   /* Scan the keys */
   nodeKeyIndex = -1;
   nodeTypeKeyIndex = -1;
   entityKeyIndex = -1;
   portalIpKeyIndex = -1;
   portalPortKeyIndex = -1;
   nodeIdxKeyIndex = -1;

   for (ii = 0; ii < MAX_PARSE_ATTRS && key_indx[ ii ]; ii++)
   {
      key = (ISNS_Key *)(key_indx[ ii ]);
      switch (key->tag)
      {
      case ISNS_ISCSI_NODE_ID:
         if (key->len > 0)
            nodeKeyIndex = ii;
         break;
      case ISNS_ISCSI_TYPE:
         nodeTypeKeyIndex = ii;
         break;
      case ISNS_ENTITY_ID:
         entityKeyIndex = ii;
         break;
      case ISNS_PORTAL_IP:
         portalIpKeyIndex = ii;
         break;
      case ISNS_PORTAL_PORT:
         portalPortKeyIndex = ii;
         break;
      case ISNS_ISCSI_IDX:
         nodeIdxKeyIndex = ii;
         break;
      default:
         /* Error condition */
         break;

      }
   }

   /* Order is important */
   if (nodeKeyIndex != -1)
   {
      __DEBUG (isns_query_debug &1,nodeKeyIndex);
      nodeKeyIndex = ISNSFindTag (0, ISNS_ISCSI_NODE_ID, key_indx);

      while (nodeKeyIndex != -1)
      {
         key = (ISNS_Key *)(key_indx[ nodeKeyIndex ]);

         /* Fetch the node */
         memset (&db_node_name, 0, sizeof (db_node_name));
         __ISNS_COPY (db_node_name.v, sizeof(db_node_name.v), &key->val, key->len);

         rval = read_ISCSIObject(db_node_name.v, &p_node, &entry);
         if (rval != SUCCESS)
           return rval;

         rval = Check_Permission (src_attr, ISCSI_ID_KEY, p_node);
         if (rval == SUCCESS)
         { 
           __DEBUG (isns_query_debug &1,call ISNSdbProcessICSCIOpAttr);
           ISNSdbProcessISCSIOpAttr (p_rspmsg, (ISNS_Attr **)attr_indx, p_node);
           foundFlag = TRUE;
         }

         nodeKeyIndex = ISNSFindTag (nodeKeyIndex + 1, ISNS_ISCSI_NODE_ID, key_indx);
      }

   }
   else if (nodeIdxKeyIndex != -1)
   {
      nodeIdxKeyIndex = ISNSFindTag (0, ISNS_ISCSI_IDX, key_indx);

      while (nodeIdxKeyIndex != -1)
      {
         void *ptr;
         key = (ISNS_Key *)(key_indx[nodeIdxKeyIndex]);
         rval = read_ISCSIidxObject(key->val.index, &ptr, &entry3);
         if (rval != SUCCESS)
          return rval;

         /* Fetch the node */
         memset (&db_node_name, 0, sizeof (db_node_name));
         strncpy ((char *) db_node_name.v, ptr, sizeof(db_node_name.v) );

         rval = read_ISCSIObject(db_node_name.v, &p_node, &entry);
         if (rval != SUCCESS)
           return rval;

         rval = Check_Permission (src_attr, ISCSI_ID_KEY, p_node);
         if (rval == SUCCESS)
         { 
            ISNSdbProcessISCSIOpAttr (p_rspmsg, (ISNS_Attr **)attr_indx, p_node);
            foundFlag = TRUE;
         }
         nodeIdxKeyIndex = ISNSFindTag (nodeIdxKeyIndex + 1, ISNS_ISCSI_IDX, key_indx);
      }

   }
   else if (entityKeyIndex != -1)
   {

      int rval;
      key = (ISNS_Key *) (key_indx[entityKeyIndex]);

      rval = read_EntityObject((char *)&key->val, &p_entity, &entry);

      if (rval)
         return (rval);

      /* Check to make sure this is an iSCSI Entity */
      if (p_entity->eid_type != ENTITY_TYPE_ISCSI)
      {
         __LOG_ERROR ("***ERROR: Expected an iSCSI Entity.");
         return (ERROR);
      }

      /* Scan the entity's iSCSI List */
      pnode=NULL;
      while ((pnode=GetNextNode(&p_entity->iscsi_node_list, pnode)))
      {
         /* Fetch the iSCSI Node */
         rval = read_ISCSIObject(GetNodeData(pnode), &p_node, &entry2);
         if (rval != SUCCESS)
           return rval;

         rval = Check_Permission (src_attr, ISCSI_ID_KEY, p_node);
         if (rval == SUCCESS)
         { 
           ISNSdbProcessISCSIOpAttr (p_rspmsg, (ISNS_Attr **)attr_indx, p_node);
           foundFlag = TRUE;
         }
      }

   }
   else if (portalPortKeyIndex != -1 || portalIpKeyIndex != -1)
   {
      SOIP_Portal_Key db_portal;
      SOIP_Portal * p_portal;

      __DEBUG (isns_query_debug &1,(Search for Portal));

      memset (&db_portal, 0, sizeof (db_portal));

      if (portalPortKeyIndex == -1 || portalIpKeyIndex == -1)
      {
         __LOG_ERROR ("***ERROR: Didn't have all the Portal Keys.");
         return (ERROR);
      }

      /* Get Portal */
      key = (ISNS_Key *)(key_indx[portalPortKeyIndex]);
      db_portal.ip_port=key->val.ip_port;

      key = (ISNS_Key *) key_indx[portalIpKeyIndex];
      __ISNS_COPY (db_portal.ip_addr.v, sizeof(db_portal.ip_addr.v), key->val.ip_addr.v, key->len);

      rval = read_PortalObject(&db_portal, &p_portal, &entry);
      if (rval != SUCCESS)
         return rval;
      else
      {
        rval = read_EntityObject(p_portal->entity_id.id, &p_entity, &entry);
        if (rval != SUCCESS)
               return rval;

        /* Scan the entity's iSCSI List */
        pnode=NULL;
        while ((pnode=GetNextNode(&p_entity->iscsi_node_list, pnode)))
        {
               /* Fetch the iSCSI Node */
               rval = read_ISCSIObject(GetNodeData(pnode), &p_node, &entry2);
               if (rval != SUCCESS)
                 return rval;

               rval = Check_Permission (src_attr, ISCSI_ID_KEY, p_node);
               if (rval == SUCCESS)
               { 
                 ISNSdbProcessISCSIOpAttr (p_rspmsg, (ISNS_Attr **)attr_indx, p_node);
                 foundFlag = TRUE;
               }
         }
      }
   }

   else
   {
      /* Search the node table */
      ISNS_DBKey qkey = { 0 };

      __DEBUG (isns_query_debug &1,search node table);
      qkey.tag = ISCSI_ID_KEY;

      while (SNSdbGetNextOfKey (&qkey) == SUCCESS)
      {
         rval = read_ISCSIObject(qkey.val.iscsi_key.v, &p_node, &entry);
         if (rval != SUCCESS)
            __LOG_ERROR ("Error in iscsi node database for key:%s",qkey.val.iscsi_key.v);

         if (nodeTypeKeyIndex != -1)
         {
            key = (ISNS_Key *)(key_indx[nodeTypeKeyIndex]);
            __DEBUG(isns_query_debug &1, key->val.node_type:%i p_node->type:%i,key->val.node_type,p_node->type);
            
            if (!(*(uint32_t *) &key->val & p_node->type))
               continue;
         }

         rval = Check_Permission (src_attr, ISCSI_ID_KEY, p_node);
         if (rval == SUCCESS)
         { 
            /* if the first found entry then add the key attr */
            if (!foundFlag)
            {
              if (nodeTypeKeyIndex != -1)
              {
                ISNSAppendKey(p_rspmsg, ISNS_ISCSI_TYPE, key->len, (char *)&key->val, 0);
                ISNSAppendKey(p_rspmsg, ISNS_DELIMITER, 0, NULL, 0);
              }
            }
           ISNSdbProcessISCSIOpAttr (p_rspmsg, (ISNS_Attr **)attr_indx, p_node);
           foundFlag = TRUE;
         }
      }
   }
   
   rc = ISNS_NO_ERR;
   if (!foundFlag)
      rc = ISNS_NO_SUCH_ENTRY_ERR;

   __DEBUG (isns_query_debug &1,return from GetAttrISCSI rc:%i,rc);

   return rc;
}

/*********************************************************************
Implements the GetNextAttr iSNS Command.
*********************************************************************/
int
ISNSGetNextAttr( ISNS_Msg_Descp *p_md, ISNS_Msg * p_rspmsg )
{
   int             ii;
   int             found;
   int             nodeKeyIndex;
   int             entityKeyIndex;
   int             portalIpKeyIndex;
   int             portalPortKeyIndex;
   int             entityIdxKeyIndex;
   int             portalIdxKeyIndex;
   int             iscsiIdxKeyIndex;
   int             portKeyIndex;
   int             ddKeyIndex;
   int             ddsKeyIndex;
   ISNS_DBKey      qkey = { 0 };
   ISNS_Key        *key;
   ISNS_Attr       *src_attr;
   ISNS_Attr       *attr_indx[SNS_MAX_ATTRS];
   ISNS_Attr       *key_indx[SNS_MAX_ATTRS];
   SOIP_Iscsi      *p_node;
   int             rval;
   SOIP_Dd         *p_dd;
   SOIP_Dds        *p_dds;

   __DEBUG (isns_query_debug &1, (ISNSGetNextAttr));

   ISNSParseMsg( &p_md->msg, attr_indx, key_indx,&src_attr );

   /* Scan the keys */
   nodeKeyIndex = -1;
   entityKeyIndex = -1;
   portalIpKeyIndex = -1;
   portalPortKeyIndex = -1;
   portKeyIndex = -1;
   iscsiIdxKeyIndex = -1;
   portalIdxKeyIndex = -1;
   entityIdxKeyIndex = -1;
   ddKeyIndex = -1;
   ddsKeyIndex = -1;

   for (ii = 0; ii < MAX_PARSE_ATTRS && key_indx[ii]; ii++)
   {
      key = (ISNS_Key *)(key_indx[ii]);
      __DEBUG (isns_query_debug &1,(keytag:%i (%s)),key->tag,isnsTagText(key->tag));
      switch (key->tag)
      {
         case ISNS_ISCSI_NODE_ID:
            nodeKeyIndex = ii;
            break;
         case ISNS_ENTITY_ID:
            entityKeyIndex = ii;
            break;
         case ISNS_PORT_NAME:
            portKeyIndex = ii;
            break;
         case ISNS_PORTAL_IP:
            portalIpKeyIndex = ii;
            break;
         case ISNS_PORTAL_PORT:
            portalPortKeyIndex = ii;
            break;
         case ISNS_ISCSI_IDX:
            iscsiIdxKeyIndex = ii;
            break;
         case ISNS_ENTITY_IDX:
            entityIdxKeyIndex = ii;
            break;
         case ISNS_PORTAL_IDX:
            portalIdxKeyIndex = ii;
            break;
         case ISNS_DD_ID:
            ddKeyIndex = ii;
            break;
         case ISNS_DDS_ID:
            ddsKeyIndex = ii;
            break;
         default:
            /* Error condition */
            break;
      }
   }

   if ( ddKeyIndex != -1 )
   {
      memset(&qkey, 0, sizeof(qkey));
      qkey.tag = DD_ID_KEY;

      key = (ISNS_Key *)(key_indx[ ddKeyIndex ]);

      if ( key->len && key->val.dd_id)
      {
         /* Key is being provided */
         qkey.val.dd_key.id = key->val.dd_id;
         qkey.len = DD_KEY_SIZE;
      }

      found=FALSE;
      while (!found)
      {
         if (SNSdbGetNextOfKey (&qkey) != SUCCESS)
         {
            return (ISNS_NO_SUCH_ENTRY_ERR);
         }

         rval = read_DDObject(qkey.val.dd_key.id, &p_dd, &entry);
         if (rval != SUCCESS)
         {
           __LOG_ERROR("Error in dd database not found id:%i",qkey.val.dd_key.id);
           return rval;
         }

         ISNSAppendKey (p_rspmsg, ISNS_DD_ID, ISNS_DD_ID_SIZE, NULL, p_dd->id);
         ISNSAppendKey (p_rspmsg, 0, 0, NULL, 0);
         ISNSdbProcessDDOpAttr (p_rspmsg, (ISNS_Attr **)attr_indx, p_dd);

         found=TRUE;
      }
   }
   else if ( ddsKeyIndex != -1 )
   {

      memset(&qkey, 0, sizeof(qkey));
      qkey.tag = DDS_ID_KEY;

      key = (ISNS_Key *)(key_indx[ ddsKeyIndex ]);

      if ( key->len && key->val.dds_id)
      {
         /* Key is being provided */
         qkey.val.dds_key.id = key->val.dds_id;
         qkey.len = DDS_KEY_SIZE;
      }

      found=FALSE;
      while (!found)
      {
         if (SNSdbGetNextOfKey (&qkey) != SUCCESS)
         {
            return (ISNS_NO_SUCH_ENTRY_ERR);
         }

         rval = read_DDSObject(qkey.val.dds_key.id, &p_dds, &entry);
         if (rval != SUCCESS)
         {
            __LOG_ERROR("Error dds database missing record:%i",qkey.val.dds_key.id);
            return rval;
         }

         ISNSAppendKey (p_rspmsg, ISNS_DDS_ID,ISNS_DDS_ID_SIZE, NULL, p_dds->id);
         ISNSAppendKey (p_rspmsg, 0, 0, NULL, 0);
         ISNSdbProcessDDSOpAttr (p_rspmsg, (ISNS_Attr **)attr_indx, p_dds);

         found=TRUE;
      }
   }
   else if ( nodeKeyIndex != -1 || iscsiIdxKeyIndex!=-1 )
   {
      __DEBUG (isns_query_debug &1,process nodeKeyIndex);
      memset(&qkey, 0, sizeof(qkey));
      qkey.tag = ISCSI_ID_KEY;

      if ( nodeKeyIndex !=-1 )
      {
         __DEBUG (isns_query_debug &1,(nodeKeyIndex:%i),nodeKeyIndex);
         key=(ISNS_Key *)(key_indx[nodeKeyIndex]);
         __DEBUG (isns_query_debug &1,key length:%i,key->len);
         __DEBUG (isns_query_debug &1,key:%s,(char *)&key->val);
         if (key->len)
         {
            qkey.len = strlen((char *)&key->val);
            __ISNS_COPY (&qkey.val, sizeof(qkey.val), &key->val, key->len);
         }
      }
      else if (iscsiIdxKeyIndex != -1)
      {
         void *ptr;
         
         key=(ISNS_Key *)(key_indx[iscsiIdxKeyIndex]);

         if (key->len && key->val.index)
         {
            rval = read_ISCSIidxObject(key->val.index, &ptr, &entry3);
            if (rval != SUCCESS)
             return rval;

            qkey.len = strlen(ptr);
            __ISNS_COPY (&qkey.val, sizeof (qkey.val), ptr, qkey.len);
         }
      }

      found=FALSE;
      while (!found)
      {
         __DEBUG (isns_query_debug &1, (GetNextOfKey:%s),(char *)&qkey.val);
         if (SNSdbGetNextOfKey (&qkey) != SUCCESS)
         {
            return (ISNS_NO_SUCH_ENTRY_ERR);
         }
         rval = read_ISCSIObject(qkey.val.iscsi_key.v, &p_node, &entry);
         if (rval != SUCCESS)
         {
            __LOG_ERROR ("ERROR reading Key: %s",qkey.val.iscsi_key.v);
            return rval;
         }

         rval = Check_Permission (src_attr, ISCSI_ID_KEY, p_node);
         if (rval != SUCCESS)
         {
               continue;
         }
         
         ISNSAppendKey (p_rspmsg, ISNS_ISCSI_NODE_ID,
                         PAD4 (strlen ((char *)&qkey.val)), (char *)&qkey.val, 0);

         ISNSAppendKey (p_rspmsg, 0, 0, NULL, 0);
         ISNSdbProcessISCSIOpAttr (p_rspmsg, (ISNS_Attr **)attr_indx, p_node);
         found=TRUE;
      }
   }
   else if (entityKeyIndex!=-1 || entityIdxKeyIndex!=-1)
   {
      SOIP_Entity *p_entity;

      memset(&qkey, 0, sizeof(qkey));
      qkey.tag = ENTITY_ID_KEY;

      if (entityKeyIndex !=-1)
      {
         key=(ISNS_Key *)(key_indx[entityKeyIndex]);

         if (key->len)
         {
            /* Key is being provided */
            strncpy(qkey.val.entity_key.id, (char *)&key->val, sizeof(qkey.val.entity_key.id) );
            qkey.len = PAD4(strlen(qkey.val.entity_key.id));
         }
      }
      else if (entityIdxKeyIndex != -1)
      {
         void *ptr;

         key = (ISNS_Key *)(key_indx[entityIdxKeyIndex]);

         if (key->len && key->val.index)
         {
            rval = read_EntityidxObject(key->val.index, &ptr, &entry3);
            if (rval != SUCCESS)
              return rval;

            strncpy(qkey.val.entity_key.id, (char *)ptr, sizeof(qkey.val.entity_key.id) );
            qkey.len = PAD4(strlen(qkey.val.entity_key.id));
         }
      }

      found=FALSE;
      while (!found)
      {
         if (SNSdbGetNextOfKey (&qkey) != SUCCESS)
         {
            return (ISNS_NO_SUCH_ENTRY_ERR);
         }
         rval = read_EntityObject(qkey.val.entity_key.id, &p_entity, &entry);

         if (rval)
            return (rval);

         rval = Check_Permission (src_attr, ENTITY_ID_KEY, p_entity);
         if (rval != SUCCESS)
         {
               continue;
         }

         ISNSAppendKey (p_rspmsg, ISNS_ENTITY_ID,
                         PAD4 (strlen (p_entity->eid.id)), p_entity->eid.id, 0);

         ISNSAppendKey (p_rspmsg, 0, 0, NULL, 0);
         ISNSdbProcessEntityOpAttr (p_rspmsg, (ISNS_Attr **)attr_indx, p_entity);

         found=TRUE;
      }
   }
   else if (portKeyIndex!=-1)
   {
      SOIP_Ifcp *p_port;
      key=(ISNS_Key *)(key_indx[portKeyIndex]);
      qkey.tag = PORT_NAME_KEY;
      qkey.len = key->len;
      __ISNS_COPY (&qkey.val, sizeof(qkey.val), &key->val, key->len);

      found=FALSE;
      while (!found)
      {
         if (SNSdbGetNextOfKey (&qkey) != SUCCESS)
         {
            return (ISNS_NO_SUCH_ENTRY_ERR);
         }
         rval = read_FCPortObject(qkey.val.port_key.v, &p_port, &entry);
         if (rval != SUCCESS)
         {
           __DEBUG (isns_query_debug &1,(Error reading getnext));
           return 0;
         }

         rval = Check_Permission (src_attr, PORT_NAME_KEY, p_port);
         if (rval != SUCCESS)
         {
               continue;
         }

         ISNSAppendKey (p_rspmsg, ISNS_PORT_NAME,
                         ISNS_PORT_NAME_SIZE, (char *)&qkey.val, 0);

         ISNSAppendKey (p_rspmsg, 0, 0, NULL, 0);
         ISNSdbProcessPortOpAttr (p_rspmsg, (ISNS_Attr **)attr_indx, p_port);

         found=TRUE;
      }
   }
   else if ((portalPortKeyIndex!=-1 && portalIpKeyIndex!=-1) || portalIdxKeyIndex != -1)
   {
      SOIP_Portal * p_portal;
      SOIP_Portal_Key db_portal;

      memset(&db_portal, 0, sizeof(db_portal));

      qkey.tag = PORTAL_ID_KEY;
      qkey.len = 0;

      if (portalIdxKeyIndex != -1)
      {
         void *ptr;

         key = (ISNS_Key *)(key_indx[portalIdxKeyIndex]);

         if (key->len && key->val.index)
         {
            rval = read_PortalidxObject(key->val.index, &ptr, &entry3);
            if (rval != SUCCESS)
              return rval;

            __ISNS_COPY (&qkey.val, sizeof(qkey.val), ptr, sizeof(db_portal));
            qkey.len = sizeof(SOIP_Portal_Key);
         }
      }
      else
      {
         key = (ISNS_Key *)(key_indx[portalPortKeyIndex]);
         db_portal.ip_port=key->val.ip_port;
         qkey.len += key->len;

         key = (ISNS_Key *)(key_indx[portalIpKeyIndex]);
         __ISNS_COPY (db_portal.ip_addr.v, sizeof(db_portal.ip_addr.v), key->val.ip_addr.v, key->len);
         qkey.len += key->len;

         __ISNS_COPY (&qkey.val, sizeof(qkey.val), &db_portal, sizeof (SOIP_Portal_Key));
      }

      found=FALSE;
      while (!found)
      {
         if (SNSdbGetNextOfKey (&qkey) != SUCCESS)
         {
            return (ISNS_NO_SUCH_ENTRY_ERR);
         }

         __ISNS_COPY (&db_portal, sizeof(SOIP_DB_Portal), &qkey.val.portal_key, sizeof(SOIP_DB_Portal));
         rval = read_PortalObject(&db_portal, &p_portal, &entry);
         if (rval != SUCCESS)
         {
           __DEBUG (isns_query_debug &1,(getnextkey error));
         }

         rval = Check_Permission (src_attr, PORTAL_ID_KEY, p_portal);
         if (rval != SUCCESS)
         {
               continue;
         }

         ISNSAppendKey (p_rspmsg, ISNS_PORTAL_IP,ISNS_PORTAL_IP_SIZE,(char *)&qkey.val, 0);
         ISNSAppendKey (p_rspmsg, ISNS_PORTAL_PORT,
                         ISNS_PORTAL_PORT_SIZE, (char *)&qkey.val + ISNS_PORTAL_IP_SIZE, 0);

         ISNSAppendKey (p_rspmsg, 0, 0, NULL, 0);
         SNSProcessPortalOpAttr (p_rspmsg, (ISNS_Attr **)attr_indx, p_portal);

         found=TRUE;
      }
   }
   else
   {
      /* Error Condition */
      return (ISNS_MSG_FMT_ERR);
   }

   if (!found)
      return (ISNS_NO_SUCH_ENTRY_ERR);

   return (ISNS_NO_ERR);
}

/********************************************************************
********************************************************************/

int
ISNSGetNextDDSMember ( ISNS_Msg_Descp *p_md, ISNS_Msg * p_rspmsg )
{
   int            ddsKeyIndex;
   int            ddKeyIndex;
   ISNS_Attr      *p_attr;
   SOIP_Dds       *p_dds;
   SOIP_DD_Key    dds_key;
   ISNS_LIST_NODE *plnode;
   ISNS_Attr       *src_attr;
   ISNS_Attr       *attr_indx[SNS_MAX_ATTRS];
   ISNS_Attr       *key_indx[SNS_MAX_ATTRS];
   int             rval;

   ISNSParseMsg( &p_md->msg, attr_indx, key_indx, &src_attr );
   ddsKeyIndex = ISNSFindTag(0, ISNS_DDS_ID, key_indx);
   ddKeyIndex = ISNSFindTag(0, ISNS_DD_ID, key_indx);

   if (ddsKeyIndex == -1)
   {
      return (ISNS_MSG_FMT_ERR);
   }

   p_attr=(ISNS_Attr *)key_indx[ddsKeyIndex];

   dds_key.id = p_attr->val.dds_id;
   rval = read_DDSObject(dds_key.id, &p_dds, &entry);
   if (rval != SUCCESS)
     return rval;

   /* Append Keys */

   p_attr = NULL;
   if ( ddKeyIndex != -1 )
   {
      p_attr = (ISNS_Attr *)key_indx[ ddKeyIndex ];
   }

   plnode = NULL;
   if (p_attr && p_attr->val.dd_id != 0)
   {
      plnode = FindNode( &p_dds->dd_list, (char *)&p_attr->val.dd_id, sizeof(uint32_t) );

      if (!plnode)
         return (ISNS_INVALID_QUERY_ERR);
   }

   plnode = GetNextNode(&p_dds->dd_list, plnode);
   if ( plnode )
   {
      /* Append Attribute */
      ISNSAppendAttr(p_rspmsg, ISNS_DD_ID, ISNS_DD_ID_SIZE, 
         NULL, *(uint32_t *)GetNodeData(plnode));
   }

   return ( plnode?ISNS_NO_ERR:ISNS_NO_SUCH_ENTRY_ERR );
}

int
ISNSGetNextEntityPortal( ISNS_Msg_Descp *p_md, ISNS_Msg * p_rspmsg )
{
   int            portalKeyIndex;
   int            entityKeyIndex;
   ISNS_Attr      *p_attr;
   ISNS_LIST_NODE *plnode;
   ISNS_Attr      *src_attr;
   ISNS_Attr      *attr_indx[SNS_MAX_ATTRS];
   ISNS_Attr      *key_indx[SNS_MAX_ATTRS];
   int            foundFlag;
   void           *ptr;
   SOIP_Entity    *p_entity;
   PORTAL_LIST_ENTRY *p_portalListEntry;
   int            rval;

   ISNSParseMsg( &p_md->msg, attr_indx, key_indx, &src_attr );
   entityKeyIndex = ISNSFindTag(0, ISNS_ENTITY_IDX, key_indx);
   portalKeyIndex = ISNSFindTag(0, ISNS_PORTAL_IDX, key_indx);

   if (entityKeyIndex == -1)
   {
      return (ISNS_MSG_FMT_ERR);
   }

   /* Entity ID found in key */
   p_attr = (ISNS_Attr *)(key_indx[entityKeyIndex]);
   rval = read_EntityidxObject(p_attr->val.index, &ptr, &entry3);
   if (rval != SUCCESS)
       return rval;

   /* Fetch entity from hash */
   if (ISNS_NO_ERR != read_EntityObject ( ptr, &p_entity, &entry ))
   {
      return (ISNS_INVALID_QUERY_ERR);
   }

   p_attr = NULL;
   if ( portalKeyIndex != -1 )
   {
      p_attr = (ISNS_Attr *)key_indx[ portalKeyIndex ];
   }

   plnode = NULL;
   if (p_attr && p_attr->val.index != 0)
   {
      foundFlag = FALSE;
      while ( (plnode = GetNextNode(&p_entity->iportal_list, plnode)) )
      {
         p_portalListEntry = (PORTAL_LIST_ENTRY *)GetNodeData(plnode);

         if ( p_portalListEntry->portal_idx == p_attr->val.index)
         {
            foundFlag = TRUE;
            break;
         }
      }

      if ( !foundFlag )
         return (ISNS_INVALID_QUERY_ERR);
   }

   plnode = GetNextNode(&p_entity->iportal_list, plnode);
   if ( plnode )
   {
      /* Append Attribute */
      p_portalListEntry = (PORTAL_LIST_ENTRY *)GetNodeData(plnode);
      ISNSAppendAttr( p_rspmsg, ISNS_PORTAL_IDX, ISNS_PORTAL_IDX_SIZE, 
                      NULL, p_portalListEntry->portal_idx );
   }

   return ( plnode?ISNS_NO_ERR:ISNS_NO_SUCH_ENTRY_ERR );
}


/********************************************************************
Gets the next DD iSCSI Member.
********************************************************************/
int
ISNSGetNextDDIscsiMember( ISNS_Msg_Descp *p_md, ISNS_Msg * p_rspmsg )
{
   int            ddKeyIndex;
   int            iscsiKeyIndex;
   ISNS_Attr      *p_attr;
   SOIP_Dd        *p_dd;
   ISNS_LIST_NODE *plnode;
   ISNS_Attr      *src_attr;
   ISNS_Attr      *attr_indx[SNS_MAX_ATTRS];
   ISNS_Attr      *key_indx[SNS_MAX_ATTRS];
   SOIP_Dd_Member *p_member;
   int            foundFlag;
   int            rval;

   ISNSParseMsg( &p_md->msg, attr_indx, key_indx, &src_attr );
   ddKeyIndex = ISNSFindTag(0, ISNS_DD_ID, key_indx);
   iscsiKeyIndex = ISNSFindTag(0, ISNS_DD_ISCSI_MEMBER_IDX, key_indx);
   if (ddKeyIndex == -1)
   {
      return (ISNS_MSG_FMT_ERR);
   }

   p_attr=(ISNS_Attr *)key_indx[ddKeyIndex];

   rval = read_DDObject(p_attr->val.dd_id, &p_dd, &entry);
   if (rval != SUCCESS)
     return rval;

   /* Append Keys */

   p_attr = NULL;
   if ( iscsiKeyIndex != -1 )
   {
      p_attr = (ISNS_Attr *)key_indx[ iscsiKeyIndex ];
   }

   plnode = NULL;
   if (p_attr && p_attr->val.index != 0)
   {
      foundFlag = FALSE;
      while ( (plnode = GetNextNode(&p_dd->member_list, plnode)) )
      {
         p_member = (SOIP_Dd_Member *)GetNodeData(plnode);

         if ( p_member->type != ISNS_DD_ISCSI_MEMBER )
            continue;

         if ( p_member->node_idx == p_attr->val.index )
         {
            foundFlag = TRUE;
            break;
         }
      }

      if ( !foundFlag )
         return (ISNS_INVALID_QUERY_ERR);

   }

   plnode = GetNextNode(&p_dd->member_list, plnode);
   if ( plnode )
   {
      /* Append Attribute */
      p_member = (SOIP_Dd_Member *)GetNodeData(plnode);
      ISNSAppendAttr(p_rspmsg, ISNS_DD_ISCSI_MEMBER_IDX, ISNS_ISCSI_IDX_SIZE, 
         NULL, p_member->node_idx);
   }

   return ( plnode?ISNS_NO_ERR:ISNS_NO_SUCH_ENTRY_ERR );
}


int
ISNSdbProcessDDSOpAttr (ISNS_Msg * p_msg, ISNS_Attr ** attr_indx,
                           SOIP_Dds *p_dds)
{
   int ii;
   ISNS_Attr *attr;
   ISNS_LIST_NODE *pnode;
   int dd_id_Flag = -1;
   int dd_sym_Flag = -1;
   SOIP_Dd *p_dd;
   SOIP_DB_Entry lentry;
   int rval;

   dd_id_Flag = ISNSFindTag (0, ISNS_DD_ID, attr_indx);
   dd_sym_Flag = ISNSFindTag (0, ISNS_DDS_SYM_NAME, attr_indx);

   for (ii = 0; (ii < SNS_MAX_ATTRS) && (attr_indx[ii]); ii++)
   {
      attr = (ISNS_Attr *) attr_indx[ii];
      switch (attr->tag)
      {
      case ISNS_DDS_ID:
         ISNSAppendAttr (p_msg, ISNS_DDS_ID, ISNS_DDS_ID_SIZE, NULL,
                         p_dds->id);
         break;
      case ISNS_DDS_SYM_NAME:
         ISNSAppendAttr (p_msg, ISNS_DDS_SYM_NAME,
                         PAD4 (strlen (p_dds->sym_name)), p_dds->sym_name, 0);
         break;
      case ISNS_DDS_STATUS:
         ISNSAppendAttr (p_msg, ISNS_DDS_STATUS, ISNS_DDS_STATUS_SIZE,
                         NULL, p_dds->status);
         break;
      case ISNS_DD_SYM_NAME:
        pnode=NULL;
        while ((pnode=GetNextNode(&p_dds->dd_list, pnode)))
        {
           if (dd_id_Flag != -1 )
              ISNSAppendAttr (p_msg, ISNS_DD_ID, ISNS_DD_ID_SIZE, NULL, *(uint32_t *)GetNodeData(pnode));
           rval = read_DDObject(*(uint32_t *)GetNodeData(pnode), &p_dd, &lentry);
           if (rval == SUCCESS)
           {
             ISNSAppendAttr (p_msg, ISNS_DD_SYM_NAME,
                         PAD4 (strlen (p_dd->sym_name)), p_dd->sym_name, 0);
           }
         }
         break;
       case ISNS_DD_ID:
         pnode=NULL;
         if (dd_sym_Flag == -1)
         {
           while ((pnode=GetNextNode(&p_dds->dd_list, pnode)))
           {
             ISNSAppendAttr (p_msg, ISNS_DD_ID, ISNS_DD_ID_SIZE, NULL, *(uint32_t *)GetNodeData(pnode));
           }
         }
         break;
       default:
         break;
      }
   }

   return (0);
}

int
ISNSdbProcessDDOpAttr (ISNS_Msg * p_msg, ISNS_Attr ** attr_indx,
                           SOIP_Dd *p_dd)
{
   int ii;
   ISNS_Attr *attr;
   ISNS_LIST_NODE *pnode;
   SOIP_Dd_Member *p_member;
   SOIP_DB_Entry lentry;
   int index_Flag = -1;
   int rval;
   SOIP_Dds *p_dds;

   for (ii = 0; (ii < SNS_MAX_ATTRS) && (attr_indx[ii]); ii++)
   {
      attr = (ISNS_Attr *) attr_indx[ii];
      switch (attr->tag)
      {
      case ISNS_DD_ID:
         ISNSAppendAttr (p_msg, ISNS_DD_ID, ISNS_DD_ID_SIZE, NULL, p_dd->id);
         break;
      case ISNS_DD_SYM_NAME:
         ISNSAppendAttr (p_msg, ISNS_DD_SYM_NAME,
                         PAD4 (strlen (p_dd->sym_name)), p_dd->sym_name, 0);
         break;
      case ISNS_DD_FEATURE_BITMAP:
         ISNSAppendAttr (p_msg, ISNS_DD_FEATURE_BITMAP,
                         ISNS_DD_FEATURE_BITMAP_SIZE, NULL, p_dd->feature);
         break;
      case ISNS_DDS_ID:
         pnode=NULL;
         while ((pnode=GetNextNode(&p_dd->dds_list, pnode)))
         {
               ISNSAppendAttr (p_msg, ISNS_DDS_ID, ISNS_DDS_ID_SIZE, NULL,
                               *(uint32_t *)GetNodeData(pnode));
         }
         break;
      case ISNS_DDS_SYM_NAME:
         pnode=NULL;
         while ((pnode=GetNextNode(&p_dd->dds_list, pnode)))
         {
               rval = read_DDSObject(*(uint32_t *)GetNodeData(pnode), &p_dds, &lentry);
               if (rval == SUCCESS)
               {
                 ISNSAppendAttr (p_msg, ISNS_DDS_SYM_NAME, PAD4(strlen(p_dds->sym_name)),p_dds->sym_name,0);
               }
         }
         break;
      case ISNS_DD_ISCSI_MEMBER:
         pnode=NULL;
         while ((pnode=GetNextNode(&p_dd->member_list, pnode)))
         {
           p_member=(SOIP_Dd_Member *)GetNodeData(pnode);
           if (p_member->type == ISNS_DD_ISCSI_MEMBER)
           {
               ISNSAppendAttr (p_msg, ISNS_DD_ISCSI_MEMBER,
                               PAD4 (strlen (p_member->node_id)),
                               p_member->node_id, 0);
               index_Flag = ISNSFindTag (0, ISNS_DD_ISCSI_MEMBER_IDX, attr_indx);
               if (index_Flag != -1 )
                 if (p_member->type == ISNS_DD_ISCSI_MEMBER)
                   ISNSAppendAttr (p_msg, ISNS_DD_ISCSI_MEMBER_IDX,
                               ISNS_DD_ISCSI_MEMBER_IDX_SIZE,
                               NULL, p_member->node_idx);
           }
         }
         break;
      case ISNS_DD_IFCP_MEMBER:
         pnode=NULL;
         while ((pnode=GetNextNode(&p_dd->member_list, pnode)))
         {
            p_member=(SOIP_Dd_Member *)GetNodeData(pnode);
            if (p_member->type == ISNS_DD_IFCP_MEMBER)
            {
               ISNSAppendAttr (p_msg, ISNS_DD_IFCP_MEMBER,
                               ISNS_PORT_NAME_SIZE, p_member->node_id,
                               0);
               index_Flag = ISNSFindTag (0, ISNS_DD_ISCSI_MEMBER_IDX, attr_indx);
               if (index_Flag != -1 )
                 if (p_member->type == ISNS_DD_ISCSI_MEMBER)
                   ISNSAppendAttr (p_msg, ISNS_DD_ISCSI_MEMBER_IDX,
                               ISNS_DD_ISCSI_MEMBER_IDX_SIZE,
                               NULL, p_member->node_idx);
            }
         }
         break;
      default:
         break;
      }
   }

   return (0);
}

/*********************************************************************
Appends iSCSI operational attributes to the msg.
*********************************************************************/
int
ISNSdbProcessISCSIOpAttr (ISNS_Msg * p_msg, ISNS_Attr ** attr_indx,
                          SOIP_Iscsi * p_node)
{
   int            ii, jj;
   int            fetchEntityFlag;
   int            fetchPortalFlag;
   ISNS_Attr      *q_attr;
   ISNS_LIST_NODE *pnode;
   SOIP_Entity    *p_entity;
   SOIP_Portal    *p_portal;
   SOIP_Portal_Group    *p_portal_group = NULL;
   SOIP_Portal_Group_Key key_portal_group;
   int            rval;

   p_portal = NULL;
   p_entity = NULL;

   fetchEntityFlag = FALSE;
   fetchPortalFlag = FALSE;

   /* Look for Any Portal or Entity attributes */
   for (ii = 0; (ii < SNS_MAX_ATTRS) && (attr_indx[ii]); ii++)
   {
      q_attr = (ISNS_Attr *) attr_indx[ii];

      switch (q_attr->tag)
      {
         case ISNS_ENTITY_TYPE:
         case ISNS_MGMT_IP:
         case ISNS_TIMESTAMP:
         case ISNS_PROT_VER:
         case ISNS_ENTITY_CERT:
         case ISNS_ENTITY_PERIOD:
            fetchEntityFlag = TRUE;
            break;

         case ISNS_PORTAL_IP:
         case ISNS_PORTAL_PORT:
         case ISNS_PORTAL_SYM_NAME:
         case ISNS_ESI_INTERVAL:
         case ISNS_ESI_PORT:
         case ISNS_PORTAL_CERT:
         case ISNS_PORTAL_GROUP_TAG:
         case ISNS_PORTAL_GROUP_IP:
         case ISNS_PORTAL_GROUP_PORT:
         case ISNS_PORTAL_GROUP_ISCSI_NAME:
            fetchPortalFlag = TRUE;
            break;
         default:
            break;
      }
   }

   /* Fetch Entity */
   if ( fetchEntityFlag || fetchPortalFlag )
   {
         if ( SUCCESS != read_EntityObject ( p_node->entity_id.id, &p_entity, &entry4 ) )
         return ( ISNS_UNKNOWN_ERR );
   }

   for ( ii = 0; ii < SNS_MAX_ATTRS && attr_indx[ ii ]; ii++ )
   {
      q_attr = (ISNS_Attr *) attr_indx[ ii ];

      __DEBUG (isns_query_debug &1, process attribute for iscsi and entity objects (%s),isnsTagText(q_attr->tag));
      switch ( q_attr->tag )
      {
      case ISNS_ISCSI_NODE_ID:
         __DEBUG (isns_query_debug &1,ISCSI NODE ID %s,p_node->id.v);
         ISNSAppendAttr ( p_msg, ISNS_ISCSI_NODE_ID,
                          PAD4 (strlen (p_node->id.v)), p_node->id.v, 0 );
         break;

      case ISNS_ISCSI_ALIAS:
         ISNSAppendAttr ( p_msg, ISNS_ISCSI_ALIAS,
                          PAD4 (strlen (p_node->alias)), p_node->alias, 0 );
         break;

      case ISNS_ISCSI_TYPE:
         ISNSAppendAttr ( p_msg, ISNS_ISCSI_TYPE, ISNS_ISCSI_TYPE_SIZE,
                          NULL, p_node->type );
         break;

      case ISNS_NODE_ACTIVE:
         ISNSAppendAttr ( p_msg, ISNS_NODE_ACTIVE, ISNS_NODE_ACTIVE_SIZE,
                          NULL, p_node->activeFlag );
         break;

      case ISNS_DD_ID:
         pnode = NULL;
         while ( ( pnode = GetNextNode(&p_node->dd_id_list, pnode) ) ) 
         {
            ISNSAppendAttr ( p_msg, ISNS_DD_ID, ISNS_DD_ID_SIZE, NULL,
                             *(uint32_t *)GetNodeData( pnode ) );
         }
         break;

      case ISNS_ISCSI_CERT:
         if ( p_node->ptr_cert )
         {
            ISNSAppendAttr ( p_msg, ISNS_ISCSI_CERT, p_node->cert_size,
                             p_node->ptr_cert, 0 );
         }
         break;

      case ISNS_ENTITY_ID:
         ISNSAppendAttr ( p_msg, ISNS_ENTITY_ID, PAD4(strlen(p_node->entity_id.id)),
                          p_node->entity_id.id, 0 );
         break;

      case ISNS_MGMT_IP:
         ISNSAppendAttr ( (struct ISNS_msg *) p_msg, ISNS_MGMT_IP, ISNS_IP_SIZE,
                          p_entity->mgmt_ip_addr.v, 0 );
         break;
      case ISNS_TIMESTAMP:
         ISNSAppendAttr ( p_msg, ISNS_TIMESTAMP,
                          ISNS_TIMESTAMP_SIZE, NULL,
                          p_entity->timestamp.t_time );
         break;
      case ISNS_ENTITY_TYPE:
         ISNSAppendAttr ( p_msg, ISNS_ENTITY_TYPE,
                          ISNS_ENTITY_TYPE_SIZE, NULL, p_entity->eid_type );
         break;
      case ISNS_ENTITY_PERIOD:
         ISNSAppendAttr ( p_msg, ISNS_ENTITY_PERIOD,
                          ISNS_ENTITY_PERIOD_SIZE, NULL, p_entity->period );
         break;
      case ISNS_PROT_VER:
         ISNSAppendAttr ( p_msg, ISNS_PROT_VER,
                          ISNS_PROT_VER_SIZE, (char *)&p_entity->prot_ver, 0 );
         break;
      case ISNS_ENTITY_CERT:
         if (p_entity->ptr_cert)
         {
            ISNSAppendAttr ( p_msg, ISNS_ENTITY_CERT,
                             p_entity->cert_size, p_entity->ptr_cert, 0 );
         }
         break;
      case ISNS_ENTITY_IDX:
         ISNSAppendAttr ( p_msg, ISNS_ENTITY_IDX, ISNS_ENTITY_IDX_SIZE, 
                          NULL, p_entity->entity_index );
         break;
      case ISNS_ISCSI_IDX:
         ISNSAppendAttr ( p_msg, ISNS_ISCSI_IDX, ISNS_ISCSI_IDX_SIZE, 
                          NULL, p_node->iscsi_index );
         break;
      case ISNS_ISCSI_SCN_BITMAP:
         ISNSAppendAttr ( p_msg, ISNS_ISCSI_SCN_BITMAP, ISNS_SCN_BITMAP_SIZE, 
                          NULL, p_node->scn_bitmap);
         break;
      default:
         break;
      }
   }

   /* Fetch Portal */
   if ( fetchPortalFlag )
   {
      __DEBUG (isns_query_debug &1, (fetchPortalFlag));
      /* Cycle through all portals */
      pnode = NULL;
      while ( (pnode = GetNextNode(&p_entity->iportal_list, pnode)) )
      {
         __DEBUG (isns_query_debug &1, Get portal record );
         rval = read_PortalObject(GetNodeData(pnode), &p_portal, &entry5);
         if (rval != SUCCESS )
           return rval;

         __DEBUG (isns_query_debug &1, Get portal group record );
         memset(&key_portal_group, 0, sizeof(SOIP_Portal_Group_Key));
         key_portal_group.ip_port = p_portal->ip_port;
         __ISNS_COPY (key_portal_group.ip_addr.v, sizeof(key_portal_group.ip_addr.v), p_portal->ip_addr.v,ISNS_PORTAL_IP_SIZE);
         __ISNS_COPY (key_portal_group.id.v, sizeof(key_portal_group.id.v), p_node->id.v,strlen(p_node->id.v));

         rval = read_PortalGroupObject(&key_portal_group, &p_portal_group, &entry4);
         if (rval != SUCCESS )
         {
           __DEBUG (isns_query_debug &1, portal group record not found );
           p_portal_group = NULL;
         }

         for (jj = 0; (jj < SNS_MAX_ATTRS) && (attr_indx[jj]); jj++)
         {
            q_attr = (ISNS_Attr *) attr_indx[jj];
            __DEBUG (isns_query_debug &1, process attribute for portal objects (%s),isnsTagText(q_attr->tag));

            switch ( q_attr->tag )
            {
               /* Located in the Portal object */
               case ISNS_PORTAL_PORT:
                  ISNSAppendAttr (p_msg, ISNS_PORTAL_PORT, ISNS_PORTAL_PORT_SIZE,
                                  NULL, p_portal->ip_port);
                  break;
               case ISNS_PORTAL_IP:
                  __DEBUG (isns_query_debug &1,Add ISNS_PORTAL_IP);
                  ISNSAppendAttr (p_msg, ISNS_PORTAL_IP, ISNS_PORTAL_IP_SIZE,
                                  p_portal->ip_addr.v, 0);
                  break;
               case ISNS_PORTAL_SYM_NAME:
                  ISNSAppendAttr (p_msg, ISNS_PORTAL_SYM_NAME,
                                  PAD4 (strlen (p_portal->sym_name)),
                                  p_portal->sym_name, 0);
                  break;
               case ISNS_ESI_PORT:
                  ISNSAppendAttr (p_msg, ISNS_ESI_PORT, ISNS_ESI_PORT_SIZE,
                                  NULL, p_portal->esi_port);
                  break;
               case ISNS_ESI_INTERVAL:
                  ISNSAppendAttr (p_msg, ISNS_ESI_INTERVAL, ISNS_ESI_INTERVAL_SIZE,
                                  NULL, p_portal->esi_interval);
                  break;
               case ISNS_PORTAL_CERT:
                  if (p_portal->ptr_cert)
                  {
                     ISNSAppendAttr (p_msg, ISNS_ISCSI_CERT, p_portal->cert_size,
                                     p_portal->ptr_cert, 0);
                  }
                  break;
               case ISNS_PORTAL_IDX:
                  ISNSAppendAttr (p_msg, ISNS_PORTAL_IDX, ISNS_PORTAL_IDX_SIZE,
                                  NULL, p_portal->portal_index);
                  break;
               case ISNS_PORTAL_GROUP_PORT:
                  if (p_portal_group)
                  {
                    __DEBUG (isns_query_debug &1,ISNS_PORTAL_GROUP_PORT:%i,p_portal_group->ip_port);
                    ISNSAppendAttr (p_msg, ISNS_PORTAL_GROUP_PORT, ISNS_PORTAL_GROUP_PORT_SIZE,
                                  NULL, p_portal_group->ip_port);
                  }
                  else
                  {
                    //since portal group doesn't exist use portal instead - workaround for open-iscsiinitiator
                    __DEBUG (isns_query_debug &1,ISNS_PORTAL_GROUP_PORT:%i,p_portal->ip_port);
                    ISNSAppendAttr (p_msg, ISNS_PORTAL_GROUP_PORT, ISNS_PORTAL_GROUP_PORT_SIZE,
                                  NULL, p_portal->ip_port);
                  }
                  break;
               case ISNS_PORTAL_GROUP_IP:
                  if (p_portal_group)
                  {
                    __DEBUG (isns_query_debug &1,Add ISNS_PORTAL_GROUP_IP);
                    ISNSAppendAttr (p_msg, ISNS_PORTAL_GROUP_IP, ISNS_PORTAL_GROUP_IP_SIZE,
                                  p_portal_group->ip_addr.v, 0);
                  }
                  else
                  {
                    //since portal group doesn't exist use portal instead - workaround for open-iscsiinitiator
                    __DEBUG (isns_query_debug &1,Add ISNS_PORTAL_GROUP_IP);
                    ISNSAppendAttr (p_msg, ISNS_PORTAL_GROUP_IP, ISNS_PORTAL_GROUP_IP_SIZE,
                                  p_portal->ip_addr.v, 0);
                  }
                  break;
               case ISNS_PORTAL_GROUP_ISCSI_NAME:
                  ISNSAppendAttr (p_msg, ISNS_PORTAL_GROUP_ISCSI_NAME,
                                PAD4 (strlen (p_node->id.v)),
                                p_node->id.v, 0);
                  break;
               case ISNS_PORTAL_GROUP_TAG:
                  if (p_portal_group)
                  {
                    __DEBUG (isns_query_debug &1,Add ISNS_PORTAL_GROUP_TAG from portal group object);
                   ISNSAppendAttr (p_msg, ISNS_PORTAL_GROUP_TAG, ISNS_PORTAL_GROUP_TAG_SIZE,
                                 NULL, p_portal_group->portal_tag);
                  }
                  else
                  {
                    __DEBUG (isns_query_debug &1,Add ISNS_PORTAL_GROUP_TAG from default tag in portal object);
                   ISNSAppendAttr (p_msg, ISNS_PORTAL_GROUP_TAG, ISNS_PORTAL_GROUP_TAG_SIZE,
                                 NULL, p_portal->default_portal_tag);
                  }
                  break;
               default:
                  break;
            }                         /* end for */
         }
      }                         /* end for */
   }

   return ( SUCCESS );
}

/*********************************************************************
*********************************************************************/
int
SNSProcessPortalOpAttr ( ISNS_Msg * p_msg, ISNS_Attr ** attr_indx,
                         SOIP_Portal * p_portal )
{
   int            ii;
   ISNS_Attr      *q_attr;

   for (ii = 0; ii < MAX_PARSE_ATTRS && attr_indx[ii]; ii++)
   {
      q_attr = attr_indx[ii];
      switch (q_attr->tag)
      {
         case ISNS_ENTITY_ID:
            ISNSAppendAttr ( p_msg, ISNS_ENTITY_ID,
                                PAD4 (strlen (p_portal->entity_id.id)),
                                p_portal->entity_id.id, 0 );
            break;
         case ISNS_PORTAL_PORT:
            ISNSAppendAttr ( p_msg, ISNS_PORTAL_PORT, ISNS_PORTAL_PORT_SIZE,
                             NULL, p_portal->ip_port );
            break;
         case ISNS_PORTAL_IP:
            ISNSAppendAttr ( p_msg, ISNS_PORTAL_IP, ISNS_PORTAL_IP_SIZE,
                             p_portal->ip_addr.v, 0 );
            break;
         case ISNS_PORTAL_SYM_NAME:
            ISNSAppendAttr ( p_msg, ISNS_PORTAL_SYM_NAME,
                             PAD4 (strlen (p_portal->sym_name)),
                             p_portal->sym_name, 0 );
            break;
         case ISNS_ESI_PORT:
            ISNSAppendAttr ( p_msg, ISNS_ESI_PORT, ISNS_ESI_PORT_SIZE,
                             NULL, p_portal->esi_port );
            break;
         case ISNS_ESI_INTERVAL:
            ISNSAppendAttr ( p_msg, ISNS_ESI_INTERVAL, ISNS_ESI_INTERVAL_SIZE,
                             NULL, p_portal->esi_interval );
            break;
         case ISNS_PORTAL_CERT:
            if ( p_portal->ptr_cert )
            {
               ISNSAppendAttr ( p_msg, ISNS_ISCSI_CERT, p_portal->cert_size,
                                p_portal->ptr_cert, 0 );
            }
            break;
         case ISNS_ENTITY_IDX:
            ISNSAppendAttr ( p_msg, ISNS_ENTITY_IDX, ISNS_ENTITY_IDX_SIZE,
                                NULL, p_portal->entity_index );
            break;
         case ISNS_PORTAL_IDX:
            ISNSAppendAttr ( p_msg, ISNS_PORTAL_IDX, ISNS_PORTAL_IDX_SIZE,
                             NULL, p_portal->portal_index );
            break;
         default:
            break;
      }
   }

   return ( ISNS_NO_ERR );
}

/*********************************************************************
Appends the opperational attributes for an entity.
*********************************************************************/
int
ISNSdbProcessEntityOpAttr (ISNS_Msg * p_msg, ISNS_Attr ** attr_indx,
                           SOIP_Entity *p_entity)
{
   int ii;
   ISNS_Attr *q_attr;

   __DEBUG (isns_query_debug &1, ISNSdbProcessEntityOpAttr);

   for (ii = 0; ii < MAX_PARSE_ATTRS && attr_indx[ii]; ii++)
   {
      q_attr = attr_indx[ii];
      switch (q_attr->tag)
      {
      case ISNS_ENTITY_ID:
         ISNSAppendAttr (p_msg, ISNS_ENTITY_ID,
                         PAD4 (strlen (p_entity->eid.id)),
                         p_entity->eid.id, 0);
         break;
      case ISNS_MGMT_IP:
         ISNSAppendAttr (p_msg, ISNS_MGMT_IP, ISNS_IP_SIZE,
                         p_entity->mgmt_ip_addr.v, 0);
         break;
      case ISNS_TIMESTAMP:
         ISNSAppendAttr (p_msg, ISNS_TIMESTAMP,
                         ISNS_TIMESTAMP_SIZE, NULL,
                         p_entity->timestamp.t_time);
         break;
      case ISNS_ENTITY_TYPE:
         ISNSAppendAttr (p_msg, ISNS_ENTITY_TYPE,
                         ISNS_ENTITY_TYPE_SIZE, NULL, p_entity->eid_type);
         break;
      case ISNS_PROT_VER:
         ISNSAppendAttr (p_msg, ISNS_PROT_VER,
                         ISNS_PROT_VER_SIZE, (char *)&p_entity->prot_ver, 0);
         break;
         /* The following are not currently implemented. */
      case ISNS_ENTITY_PERIOD:
         ISNSAppendAttr (p_msg, ISNS_ENTITY_PERIOD,
                         ISNS_ENTITY_PERIOD_SIZE, NULL, p_entity->period);
         break;
      case ISNS_ENTITY_CERT:
         if (p_entity->ptr_cert)
         {
            ISNSAppendAttr (p_msg, ISNS_ENTITY_CERT,
                            p_entity->cert_size, p_entity->ptr_cert, 0);
         }
         break;
      case ISNS_ENTITY_IDX:
         ISNSAppendAttr ( p_msg, ISNS_ENTITY_IDX,
                          ISNS_ENTITY_IDX_SIZE, NULL, p_entity->entity_index);
         break;
      default:
         break;
      }
   }

   return (ISNS_NO_ERR);
}

/*********************************************************************
Appends the operational attributes for an iFCP node.
*********************************************************************/
int
ISNSdbProcessPortOpAttr (ISNS_Msg * p_msg, ISNS_Attr ** attr_indx,
                         SOIP_Ifcp * p_port)
{
   int ii;
   ISNS_Attr *q_attr;
   ISNS_LIST_NODE *pnode;

   for (ii = 0; (ii < SNS_MAX_ATTRS) && (attr_indx[ii]); ii++)
   {
      q_attr = (ISNS_Attr *) attr_indx[ii];
      switch (q_attr->tag)
      {
      case ISNS_PORT_NAME:
         ISNSAppendAttr (p_msg, ISNS_PORT_NAME,
                         PORT_NAME_SIZE, p_port->port_name.v, 0);
         break;
      case ISNS_PORT_ID:
         ISNSAppendAttr (p_msg, ISNS_PORT_ID,
                         ISNS_PORT_ID_SIZE, (char *)&p_port->id, 0);
         break;
      case ISNS_PORT_TYPE:
         ISNSAppendAttr (p_msg, ISNS_PORT_TYPE,
                         ISNS_PORT_TYPE_SIZE, NULL, p_port->type);
         break;
      case ISNS_PORT_SYM_NAME:
         ISNSAppendAttr (p_msg, ISNS_PORT_SYM_NAME,
                         PAD4 (strlen (p_port->sym_name)), p_port->sym_name,
                         0);
         break;
      case ISNS_FABRIC_PORT_NAME:
         ISNSAppendAttr (p_msg, ISNS_FABRIC_PORT_NAME,
                         ISNS_FABRIC_PORT_NAME_SIZE,
                         p_port->fabric_port_name.v, 0);
         break;
      case ISNS_FC_HARD_ADDR:
         ISNSAppendAttr (p_msg, ISNS_FC_HARD_ADDR,
                         ISNS_FC_HARD_ADDR_SIZE, p_port->hard_addr.v, 0);
         break;
      case ISNS_FC_PORT_IP:
         ISNSAppendAttr (p_msg, ISNS_FC_PORT_IP,
                         ISNS_FC_PORT_IP_SIZE, p_port->ip_addr.v, 0);
         break;
      case ISNS_FC_COS:
         ISNSAppendAttr (p_msg, ISNS_FC_COS,
                         ISNS_FC_COS_SIZE, NULL, p_port->cos);
         break;
      case ISNS_FC4_TYPE:
         ISNSAppendAttr (p_msg, ISNS_FC4_TYPE,
                         ISNS_FC4_TYPE_SIZE, (char *)&p_port->fc4_types.bitmap, 0);
         break;
      case ISNS_FC4_DESC:
         ISNSAppendAttr (p_msg, ISNS_FC4_DESC,
                         PAD4 (strlen ((char *)&p_port->fc_descr)), (char *)&p_port->fc_descr.v,
                         FC_DESCRIPTOR_SIZE);
         break;
      case ISNS_FC4_FEATURE:
         ISNSAppendAttr (p_msg, ISNS_FC4_FEATURE,
                         ISNS_FC4_FEATURE_SIZE, (char *)&p_port->fc_feature.v,
                         FC_FEATURE_SIZE);
         break;
      case ISNS_DD_ID:
         pnode=NULL;
         while ((pnode=GetNextNode(&p_port->dd_id_list, pnode)))
         {
               ISNSAppendAttr (p_msg, ISNS_DD_ID, ISNS_DD_ID_SIZE, NULL,
                               *(uint32_t *)GetNodeData(pnode));
         }
         break;
      case ISNS_IFCP_NODE_CERT:
         if (p_port->ptr_cert)
         {
            ISNSAppendAttr (p_msg, ISNS_IFCP_NODE_CERT,
                            p_port->cert_size, p_port->ptr_cert, 0);
         }
         break;

      default:
         break;
      }
   }

   return (ISNS_NO_ERR);
}

/*********************************************************************
_ISNSdbProcessFCNodeOpAttr

This will append all the FC Node's attributes to the msg.
*********************************************************************/
int
ISNSdbProcessFCNodeOpAttr (ISNS_Msg * p_msg, ISNS_Attr ** attr_indx,
                           SOIP_Fc_Node * p_node)
{
   int ii;
   ISNS_Attr *q_attr;
   int attr_len;
   ISNS_LIST_NODE *pnode;

   for (ii = 0; (ii < SNS_MAX_ATTRS) && (attr_indx[ii]); ii++)
   {
      q_attr = (ISNS_Attr *) attr_indx[ii];
      switch (q_attr->tag)
      {
      case ISNS_NODE_NAME:
         ISNSAppendAttr (p_msg, ISNS_NODE_NAME, ISNS_NODE_NAME_SIZE,
                         p_node->node_name.v, 0);
         break;

      case ISNS_NODE_SYM_NAME:
         ISNSAppendAttr (p_msg, ISNS_NODE_SYM_NAME,
                         PAD4 (strlen (p_node->sym_name)), p_node->sym_name,
                         0);
         break;

      case ISNS_FC_NODE_IPA:
         ISNSAppendAttr (p_msg, ISNS_FC_NODE_IPA, ISNS_FC_NODE_IPA_SIZE,
                         p_node->node_ipa.v, 0);
         break;

      case ISNS_FC_NODE_IP:
         ISNSAppendAttr (p_msg, ISNS_FC_NODE_IP, ISNS_FC_NODE_IP_SIZE,
                         p_node->ip_addr.v, 0);
         break;

      case ISNS_PORT_NAME:
         attr_len = PORT_NAME_SIZE;
         pnode=NULL;
         while ((pnode=GetNextNode(&p_node->port_list, pnode)))
         {
            ISNSAppendAttr (p_msg, ISNS_PORT_NAME, ISNS_PORT_NAME_SIZE,
                            GetNodeData(pnode), 0);
         }
         break;
      case ISNS_FC_NODE_CERT:
         ISNSAppendAttr (p_msg, ISNS_FC_NODE_CERT,
                         p_node->cert_size, p_node->ptr_cert, 0);
         break;
      default:
         break;
      }
   }

   return (SUCCESS);
}

/********************************************************************
********************************************************************/
int 
IsDDSActive(int dds_id)
{
   SOIP_Dds *p_dds;
   SOIP_DB_Entry lentry;
   int rval;

   rval = read_DDSObject(dds_id, &p_dds, &lentry);
   if (rval != SUCCESS)
     return rval;

   return (p_dds->status);
}

/********************************************************************
********************************************************************/
int
IsDDActive(int dd_id, int exclude_dds_id)
{
   SOIP_Dd        *p_dd;
   SOIP_DB_Entry  lentry;
   int            rval;
   ISNS_LIST_NODE *pnode;

   if (SUCCESS != read_DDObject (dd_id, &p_dd, &lentry))
      return (FALSE);

   pnode = NULL;
   rval = FALSE;
   while ((pnode=GetNextNode(&p_dd->dds_list, pnode)))
   {
      int dds_id;

      dds_id = *(uint32_t *)GetNodeData(pnode);
      if (exclude_dds_id && dds_id == exclude_dds_id)
         continue;

      if (IsDDSActive(dds_id))
      {
         rval = TRUE;
         break;
      }
   }

   return (rval);
}

/*********************************************************************
FLAG should be set to 0 if you only want the active DD, else set it
to 1 and it will return ALL DD.
*********************************************************************/
int
Get_Active_DD_List (ISNS_LIST *dd_list, ISNS_DD_LIST p_dlist, int flag)
{
   SOIP_Dd        *p_dd;
   SOIP_DD_Key    dd_key;
   SOIP_DB_Entry  lentry2;
   ISNS_LIST_NODE *pnode;
   int            rval;

   pnode = NULL;

   __DEBUG (isns_query_debug &1,(Get_Active_DD_list));

   while ( (pnode=GetNextNode(dd_list, pnode)) )
   {
      __DEBUG (isns_query_debug &1,(Get_Active_DD_List - node data));
      /* First, look at the Node's Local DD_LIST. */
      if (flag)
      {
         Append_DD_to_List (p_dlist, *(uint32_t *)GetNodeData(pnode));
         continue;
      }

      dd_key.id = *(uint32_t *)GetNodeData(pnode);
      rval = read_DDObject(dd_key.id, &p_dd, &lentry2);
      if (rval != SUCCESS)
         return rval;

      if (p_dd->activeFlag || flag)
         Append_DD_to_List (p_dlist, dd_key.id);
   }

   return (ISNS_NO_ERR);
}

/*********************************************************************
Gets a DD list from an iFCP node.  flag=0 for active DD, else flag=1
for all DD.
*********************************************************************/
int
Get_Active_DD_List_From_IFCP_Node (char *nodename, ISNS_DD_LIST p_dlist,
                                   int flag)
{
   SOIP_Ifcp *p_node;
   SOIP_DB_Entry lentry;
   int rval;

   rval = read_FCPortObject(nodename, &p_node, &lentry);
   if (rval != SUCCESS)
     return rval;

   Get_Active_DD_List (&p_node->dd_id_list, p_dlist, flag);

   return (ISNS_NO_ERR);
}

int
Get_Active_DD_List_From_ENTITY_Node (char *p_entity_name,
                                     SOIP_Entity * p_entity,
                                     ISNS_DD_LIST p_list, int flag)
{
   int rval = ISNS_NO_ERR;
   SOIP_DB_Entry lentry;
   ISNS_LIST_NODE *pnode;

   if (p_entity == NULL)
   {
      /* Fetch entity from hash */
      if (ISNS_NO_ERR != read_EntityObject (p_entity_name, &p_entity, &lentry))
      {
         return (ERROR);
      }
   }

   if (p_entity->eid_type == ENTITY_TYPE_ISCSI)
   {
      /* For each iSCSI Node */
      pnode=NULL;
      while ((pnode=GetNextNode(&p_entity->iscsi_node_list, pnode)))
      {
            Get_Active_DD_List_From_ISCSI_Node (GetNodeData(pnode),
                                                p_list, flag);
      }
   }
   else if (p_entity->eid_type == ENTITY_TYPE_IFCP)
   {
      /* For each iFCP Node */
      pnode=NULL;
      while ((pnode=GetNextNode(&p_entity->ifcp_node_list, pnode)))
      {
         /*Append DD */
         Get_Active_DD_List_From_IFCP_Node (GetNodeData(pnode),
                                            p_list, flag);
      }
   }
   else
   {
      rval = ERROR;
   }

   return (rval);
}

/*********************************************************************
Gets the DD from an iSCSI node.  flag=0 for active DD, else flag=1 for
all DD.
*********************************************************************/
int
Get_Active_DD_List_From_ISCSI_Node (char *nodename, ISNS_DD_LIST p_dlist,
                                    int flag)
{
   SOIP_Iscsi    *p_node;
   SOIP_DB_Entry lentry;
   int           rval;

   __DEBUG(isns_query_debug & 1, (Get_Active_DD_List_From_ISCSI_Node nodename=%s),nodename);

   rval = read_ISCSIObject(nodename, &p_node, &lentry);
   if (rval != SUCCESS)
     return rval;

   Get_Active_DD_List (&p_node->dd_id_list, p_dlist, flag);

   return (ISNS_NO_ERR);
}

/*********************************************************************
Appends the active DD from a node.
*********************************************************************/
int
SNSGet_Active_DD_List_From_Src (ISNS_Attr * src_attr,
                                ISNS_DD_LIST p_dlist_src, int flag)
{
   int rval;

   /* Validate the src attr */
   if (src_attr == NULL || src_attr->tag == 0)
   {
      /* Do nothing */
   }
   else if (src_attr->tag == ISNS_ISCSI_NODE_ID)
   {
      /* This will retrieve all the ACTIVE DD_ID for the src. */
      if (SUCCESS !=
          (rval=Get_Active_DD_List_From_ISCSI_Node ((char *)&src_attr->val, p_dlist_src,
                                              flag)))
         return (rval);
   }
   else if (src_attr->tag == ISNS_PORT_NAME)
   {
      if (SUCCESS !=
          (rval=Get_Active_DD_List_From_IFCP_Node ((char *)&src_attr->val, p_dlist_src,
                                             flag)))
         return (rval);
   }
   else if (src_attr->tag == ENTITY_ID_KEY)
   {
      if (SUCCESS !=
          (rval=Get_Active_DD_List_From_ENTITY_Node ((char *)&src_attr->val, NULL,
                                               p_dlist_src, flag)))
         return (rval);
   }
   else
   {
      __LOG_ERROR ("***ERROR: Invalid SOURCE Tag.");
      return (ISNS_MSG_FMT_ERR);
   }

   return (SUCCESS);
}

int
ISNSTouchEntity( ISNS_Attr *src_attr )
{
   SOIP_DB_Entry        lentry;
   SOIP_DB_Entry        lentry2;
   SOIP_Entity          *p_entity;
   SOIP_ISCSI_Node_Id   db_node_name;
   SOIP_Iscsi           *p_node;
   time_t               t;
   int                  rval;

   if (!src_attr || src_attr->tag == 0)
      return (SUCCESS);

   if ( src_attr->tag == ISNS_ISCSI_NODE_ID )
   {
      memset ( &db_node_name, 0, sizeof (db_node_name) );
      strncpy ( (char *) db_node_name.v, (char *)&src_attr->val, sizeof(db_node_name.v) );

      rval = read_ISCSIObject(db_node_name.v, &p_node, &lentry);
      if (rval != SUCCESS)
        return (ERROR);

      rval = read_EntityObject (p_node->entity_id.id, &p_entity, &lentry2);
      if (rval != SUCCESS)
         return ( ERROR );

      /* Update Time Stamp */
      p_entity->timestamp.t_time = time (&t);
      write_EntityObject(p_node->entity_id.id, lentry2);
   }
   else
   {
      __LOG_INFO ("src_attr->tag is not a iSCSI node and should be");
      return (ERROR);
   }

   return ( SUCCESS );
}

/*********************************************************************
Returns TRUE if the src and node dd list overlap.
*********************************************************************/
int
Check_Overlap_DD (ISNS_DD_LIST p_dlist_src, ISNS_DD_LIST p_dlist_node)
{
   int ii, jj;

   for (ii = 0; ii < MAX_DD_PER_LIST; ii++)
   {
      if (p_dlist_src[ii]==0)
         break;

      for (jj = 0; jj < MAX_DD_PER_LIST; jj++)
      {
         if (p_dlist_node[jj]==0)
            break;

         if (p_dlist_src[ii] == p_dlist_node[jj])
            return (TRUE);
      }
   }

   return (FALSE);
}

/*********************************************************************
*********************************************************************/
int
Check_Permission (ISNS_Attr *src_attr, int rectype, void * ptr)
{
   int rval;
   ISNS_LIST_NODE * pnode;
   SOIP_Dds * p_dds;
   SOIP_Dd *p_dd;
   SOIP_Entity *p_entity;
   SOIP_Portal *p_portal;

   __DEBUG (isns_query_debug &1,(Check_Permission src_node_name=%s),src_attr->val.node_name.v);

   /* check if control node */
   if ( 0 == strcmp (src_attr->val.node_name.v, isns_control_node) )
     return SUCCESS;
 
   memset (dlist_src, 0, MAX_DD_PER_LIST*sizeof(uint32_t));
   rval = SNSGet_Active_DD_List_From_Src (src_attr, dlist_src, 0);
   if (rval !=SUCCESS)
   {
      return (ISNS_AUTH_FAILED_ERR);
   }

   switch (rectype)
   {
     case ISCSI_ID_KEY:
        if (src_attr->tag)
        {
          SOIP_Iscsi * pnode;
          pnode = (SOIP_Iscsi *)ptr;
          memset (dlist_node, 0, MAX_DD_PER_LIST*sizeof(uint32_t));

          __DEBUG (isns_query_debug &1, call Get_Active_DD_List);
          Get_Active_DD_List (&pnode->dd_id_list, dlist_node, 0);

          if (FALSE == Check_Overlap_DD (dlist_src, dlist_node))
          {
              __DEBUG(isns_query_debug & 1,"src attribute not found in a Discovery Domain");
              return (ISNS_AUTH_FAILED_ERR);
          }
        }
        break;
     case ENTITY_ID_KEY:
          if (src_attr->tag)
          {
            p_entity = ptr;

            memset (dlist_node, 0, MAX_DD_PER_LIST*sizeof(uint32_t));
            rval = Get_Active_DD_List_From_ENTITY_Node (NULL, p_entity,dlist_node,0);
            if (ISNS_NO_ERR != rval)
            {
               return (ISNS_INVALID_QUERY_ERR);
            }
            if (FALSE == Check_Overlap_DD (dlist_src, dlist_node))
            {
              __DEBUG(isns_query_debug & 1,"src attribute not found in a Discovery Domain");
              return (ISNS_AUTH_FAILED_ERR);
            }
          }
          break;
      case PORTAL_ID_KEY:
         if (src_attr->tag)
         {
           p_portal = ptr;

           Get_Active_DD_List_From_ENTITY_Node (p_portal->entity_id.id, NULL,
                                                     dlist_node, 0);

           if (FALSE == Check_Overlap_DD (dlist_src, dlist_node))
           {
              __DEBUG(isns_query_debug & 1,"src attribute not found in a Discovery Domain");
              return (ISNS_AUTH_FAILED_ERR);
           }
         }
         break;
     case PORT_NAME_KEY:
          if (src_attr->tag)
          {
            SOIP_Ifcp *p_port;
            p_port = (SOIP_Ifcp *)ptr;

            memset (dlist_node, 0, MAX_DD_PER_LIST*sizeof(uint32_t));

            __DEBUG (isns_query_debug &1, Get_Active_DD_List);
            Get_Active_DD_List (&p_port->dd_id_list, dlist_node, 0);

            if (FALSE == Check_Overlap_DD (dlist_src, dlist_node))
            {
              __DEBUG(isns_query_debug & 1,"src attribute not found in a Discovery Domain");
              return (ISNS_AUTH_FAILED_ERR);
            }
          }
          break;
      case DDS_ID_KEY:
         {
            p_dds = ptr;
            int jj;

            pnode=NULL;
            while ((pnode=GetNextNode(&p_dds->dd_list, pnode)))
            {
               for (jj = 0; jj < MAX_DD_PER_LIST && dlist_src[jj]!=0; jj++)
               {
                 if (dlist_src[jj]==*(uint32_t *)GetNodeData(pnode))
                 {
                   return (SUCCESS);
                   break;
                 }
               }
             }
          }
          break;
 
      case DD_ID_KEY:
          {
            p_dd = ptr;
            int ii;

            for (ii = 0; ii < MAX_DD_PER_LIST && dlist_src[ii]!=0; ii++)
            {
               if (dlist_src[ii] == p_dd->id)
               {
                 return (SUCCESS);
                 break;
               }
            }
          }
          break;
      default:
          return (ISNS_AUTH_FAILED_ERR);
          break;
   }

   return SUCCESS;
}
