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
 * This file contains source code for registering objects with the
 * iSNS database. 
 *
 */
#include "iSNS.h"
#include "iSNStypes.h"
#include "iSNSmsg.h"
#include "iSNSList.h"
#include "iSNSdb.h"
#include "iSNSobjects.h"
#include "iSNSreg.h"
#include "iSNSscn.h"
#include "iSNSquery.h"
#include "iSNSresponse.h"
#include "iSNScomm.h"
#include "iSNSbuffer.h"
#include "iSNStbl.h"
#include "iSNSparse.h"
#include "iSNSdebug.h"

/* Global */
extern int isns_reg_debug;
extern int sns_esi_interval;
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

int Check_if_member_of_DD (char * id);
static int Add_ISCSINode_Default_DD (SOIP_Iscsi *p_node);

/*********************************************************************
_SNSdbAddAttr 

Calls the more specific add routines.
*********************************************************************/
int
ISNSdbAddAttr (ISNS_Msg_Descp * p_md, ISNS_Msg *rspMsg)
{
   ISNS_Attr   *p_attr;
   int         status;
   int         i;
   int         regEnitityFlag;
   int         regFCNodeFlag;
   int         regFCPortFlag;
   int         regPortalFlag;
   int         regISCSIFlag;
   int         regPortalGroupFlag;
   int         regFlag;
   ISNS_Attr   *attr_indx[SNS_MAX_ATTRS];
   ISNS_Attr   *key_indx[SNS_MAX_ATTRS];

   ISNS_Attr      *src_attr;

   /* Search the operation Attributes */
   regFlag = FALSE;
   regEnitityFlag = 0;
   regFCNodeFlag = 0;
   regFCPortFlag = 0;
   regPortalFlag = 0;
   regISCSIFlag = 0;
   regPortalGroupFlag = 0;

   ISNSParseMsg( &p_md->msg, attr_indx, key_indx, &src_attr );

   /* Profile which registration needs to be done */
   for (i = 0; i < MAX_PARSE_ATTRS && attr_indx[ i ]; i++)
   {
      p_attr = (ISNS_Attr *)(attr_indx[ i ]);
      switch ( p_attr->tag )
      {
         case ISNS_ENTITY_ID:
         case ISNS_ENTITY_TYPE:
         case ISNS_MGMT_IP:
         case ISNS_PROT_VER:
         case ISNS_TIMESTAMP:
         case ISNS_ENTITY_CERT:
         case ISNS_ENTITY_PERIOD:
            regEnitityFlag = TRUE;
            break;

         case ISNS_PORTAL_IP:
         case ISNS_PORTAL_PORT:
         case ISNS_PORTAL_SYM_NAME:
         case ISNS_PORTAL_CERT:
         case ISNS_ESI_INTERVAL:
         case ISNS_ESI_PORT:
            regPortalFlag = TRUE;
            break;

         case ISNS_NODE_NAME:
         case ISNS_NODE_SYM_NAME:
         case ISNS_FC_NODE_IP:
         case ISNS_FC_NODE_IPA:
         case ISNS_FC_NODE_CERT:
            regFCNodeFlag = TRUE;
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
            regFCPortFlag = TRUE;
            break;

         case ISNS_ISCSI_NODE_ID:
         case ISNS_ISCSI_ALIAS:
         case ISNS_ISCSI_TYPE:
         case ISNS_ISCSI_CERT:
            regISCSIFlag = TRUE;
            break;
 
         case ISNS_PORTAL_GROUP_ISCSI_NAME:
         case ISNS_PORTAL_GROUP_TAG:
         case ISNS_PORTAL_GROUP_PORT:
         case ISNS_PORTAL_GROUP_IP:
            regPortalGroupFlag = TRUE;
            break;
         default:
            break;
      }
   }

   regPortalGroupFlag = (regISCSIFlag && regPortalFlag) || regPortalGroupFlag;

   __DEBUG (isns_reg_debug &1,regISCSIFlag:%i,regISCSIFlag);
   __DEBUG (isns_reg_debug &1,regPortalFlag:%i,regPortalFlag);
   __DEBUG (isns_reg_debug &1,regEntityFlag%i,regEnitityFlag);
   __DEBUG (isns_reg_debug &1,regPortalGroupFlag%i,regPortalGroupFlag);

   status = ISNS_NO_ERR;
   /* Register Entity */
   if (regEnitityFlag)
   {
      regFlag=TRUE;
      status = SNSdbAddAttrEntity (attr_indx, key_indx, p_md, rspMsg);
   }

   if (regFCPortFlag && status == ISNS_NO_ERR)
   {
      regFlag=TRUE;
      status = SNSdbAddAttrPort (attr_indx, key_indx, p_md, rspMsg);
   }
   /* Note the else */
   else if (regFCNodeFlag && status == ISNS_NO_ERR)
   {
      regFlag=TRUE;
      status = ISNSdbAddAttrNode (attr_indx, key_indx, p_md, rspMsg);
   }

   if (regISCSIFlag && status == ISNS_NO_ERR)
   {
      regFlag=TRUE;
      status = ISNSdbAddAttrISCSINode (attr_indx, key_indx, p_md, rspMsg);
   }

   if (regPortalFlag && status == ISNS_NO_ERR)
   {
      regFlag=TRUE;
      status = ISNSdbAddAttrPortal (attr_indx, key_indx, p_md, rspMsg);
   }

   if (regPortalGroupFlag && status == ISNS_NO_ERR)
   {
      regFlag=TRUE;
      status = ISNSdbAddAttrPortalGroup (attr_indx, key_indx, p_md, rspMsg);
   }

   if (!regFlag)
       status=ISNS_INVALID_REG_ERR;

   return status;
}

/*********************************************************************
_ISNSdbAddAttrPortal

Adds one or more portals to the database.  The keys are appended to the
rsp message.
*********************************************************************/
int
ISNSdbAddAttrPortal (ISNS_Attr **attr_indx, ISNS_Attr **key_indx, ISNS_Msg_Descp * p_md,
                    ISNS_Msg * p_rspmsg)
{
   ISNS_Key          *key = NULL;
   ISNS_Attr         *attr = NULL;
   SOIP_Portal       *p_portal;
   SOIP_Entity       *p_entity;
   SOIP_Portal_Key   db_portal;
   int               ii;
   int               rval;
   int               portalIpIndex;
   int               portalPortIndex;
   int               saveIndex;
   int               stopFlag;
   int               newPortalRegFlag;
   int               eidKeyIndex;
   int               updateESIflag;
   int               foundFlag;
   int               updateFlag;
   SOIP_Entity_Key   db_entity_id = {{0}};
   ISNS_LIST_NODE    *pnode;
   ISNS_LIST_NODE    *pnode2;
   PORTAL_LIST_ENTRY newPortalEntry;
   char              *p_iscsi_name;
   SOIP_DB_Portal    *p_idx;
   
   __DEBUG (isns_reg_debug & 1, (Registering Portal));

   updateESIflag = FALSE;
   newPortalRegFlag = FALSE;
   portalPortIndex = -1;
   portalIpIndex = -1;

   for (ii = 0; ii < SNS_MAX_ATTRS && key_indx[ ii ]
        && (portalIpIndex == -1 || portalPortIndex == -1); ii++)
   {
      key = (ISNS_Key *)( key_indx[ ii ] );
      switch ( key->tag )
      {
      case ISNS_PORTAL_IP:
         /* Check to see if we already had an IP */
         if (portalIpIndex != -1)
         {
            __DEBUG (isns_reg_debug & 1, (PORTAL_IP tag found twice));
            return ( ISNS_INVALID_REG_ERR );
         }
         portalIpIndex = ii;
         break;
      case ISNS_PORTAL_PORT:
         /* Check to see if we already had a PORT */
         if (portalPortIndex != -1)
         {
            __DEBUG (isns_reg_debug & 1, (PORTAL_PORT tag found twice));
            return ( ISNS_INVALID_REG_ERR );
         }
         portalPortIndex = ii;
         break;
      default:
         break;
      }
   }

   if (portalIpIndex == -1 && portalPortIndex == -1)
   {
      /* No keys */
      newPortalRegFlag = TRUE;
   }
   else if (portalIpIndex == -1 || portalPortIndex == -1)
   {
      /* One or the other key */
      __DEBUG (isns_reg_debug & 1, (Portal Reg: Missing keys));
      return ( ISNS_INVALID_REG_ERR );
   }

   saveIndex = 0;
   /* Loop for every portal IP and portal PORT */
   while ( saveIndex < SNS_MAX_ATTRS )
   {
      if ( newPortalRegFlag )
      {
         /* Search for the tag portal IP and portal PORT. */
         /* These are going to be used as the key. */
         portalIpIndex = -1;
         portalPortIndex = -1;

         for ( ii = saveIndex; ii < SNS_MAX_ATTRS && attr_indx[ii]
               && (portalIpIndex == -1 || portalPortIndex == -1); ii++ )
         {
            attr = (ISNS_Attr *) attr_indx[ ii ];
            switch ( attr->tag )
            {
               case ISNS_PORTAL_IP:
                  /* Check to see if we already had an IP */
                  if (portalIpIndex != -1)
                  {
                     __DEBUG (isns_reg_debug & 1, (PORTAL_IP tag found twice));
                     return ( ISNS_INVALID_REG_ERR );
                  }
                  portalIpIndex = ii;
                  break;
               case ISNS_PORTAL_PORT:
                  /* Check to see if we already had a PORT */
                  if (portalPortIndex != -1)
                  {
                     __DEBUG (isns_reg_debug & 1, (PORTAL_PORT tag found twice));
                     return ( ISNS_INVALID_REG_ERR );
                  }
                  portalPortIndex = ii;
                  break;
               default:
                  break;
            }
         }

         /* Check to see if we have both indices */
         if ( portalIpIndex == -1 || portalPortIndex == -1 )
         {
            __DEBUG (isns_reg_debug & 1, (PORTAL REG, MISSING KEYS));
            return ( ISNS_INVALID_REG_ERR );
         }

         __DEBUG (isns_reg_debug &1,(Portal ip_port:%i),attr->val.ip_port);
         attr = (ISNS_Attr *) attr_indx[ portalPortIndex ];
         db_portal.ip_port = attr->val.ip_port;

         attr = (ISNS_Attr *) attr_indx[ portalIpIndex ];
         __ISNS_COPY (db_portal.ip_addr.v, sizeof(db_portal.ip_addr.v), &attr->val, ISNS_PORTAL_IP_SIZE);

         saveIndex = ii;
      }
      else
      {
         __DEBUG (isns_reg_debug &1,(Portal ip_port:%i),key->val.ip_port);
         /* Use Keys to fetch the portal Object */
         key = (ISNS_Key *) key_indx[ portalPortIndex ];
         db_portal.ip_port = key->val.ip_port;

         __DEBUG (isns_reg_debug &1,(Portal ip_addr len:%i),key->len);
         key = (ISNS_Key *) key_indx[ portalIpIndex ];
         __ISNS_COPY (db_portal.ip_addr.v, sizeof(db_portal.ip_addr.v), &key->val, key->len);
         saveIndex = 0;
      }

      __DEBUG (isns_reg_debug &1,(read portal:%s),db_portal.ip_addr.v);
      rval = read_PortalObject(&db_portal, &p_portal, &entry2);
      if ( (rval == SUCCESS) && (p_md->msg.hdr.flags & ISNS_FLAG_REPLACE_REG) )
      {
         __DEBUG (isns_reg_debug & 1,Deregistering Portal);
         SNSRemovePortalEntry (&db_portal, NULL, NULL);
         rval = ERROR;
      }

      /* Create new entry */
      if ( rval != SUCCESS )
      {
         __DEBUG (isns_reg_debug &1,create new portal entry);

         if ( FALSE == newPortalRegFlag )
         {
            __DEBUG (isns_reg_debug & 1, (Unable to retrieve portal entry));
            return ( ISNS_INVALID_REG_ERR );
         }
         /* create a new node element */
         memset (&entry2, 0, sizeof (SOIP_DB_Entry));

         attr = (ISNS_Attr *) attr_indx[portalIpIndex];
         __ISNS_COPY (p_portal->ip_addr.v, sizeof(p_portal->ip_addr.v), attr->val.ip_addr.v, attr->len);

         attr = (ISNS_Attr *) attr_indx[portalPortIndex];
         p_portal->ip_port = attr->val.ip_port;
         __DEBUG (isns_reg_debug & 1, (Obtain Portal ip_port:%i),p_portal->ip_port);
         p_portal->portal_index = ISNSGetNewPortalIdx();

     p_portal->default_portal_tag = 1;  //DEFAULT portal_tag 

         memset (&entry3, 0, sizeof(SOIP_DB_Entry));
         p_idx = &entry3.data.portal_idx;
         __ISNS_COPY (p_idx, sizeof(entry3.data.portal_idx), &db_portal,sizeof(SOIP_DB_Portal));
         rval = write_PortalidxObject(p_portal->portal_index, entry3);

         /* Update Entity's portal Bitmap */
         eidKeyIndex = ISNSFindTag (0, ISNS_ENTITY_ID, key_indx);

         if (eidKeyIndex == -1)
         {
            attr = ISNSFindKeyInResponseMsg (ISNS_ENTITY_ID, p_rspmsg);
         }
         else
         {
            attr = (ISNS_Attr *)(key_indx[eidKeyIndex]);
         }

         /* Fetch entity */
         do
         {
            __DEBUG (isns_reg_debug &1,Fetch entity);
            strncpy(db_entity_id.id, (char *)&attr->val, sizeof(db_entity_id.id));
            rval = read_EntityObject(db_entity_id.id, &p_entity, &entry);
            if ( rval != SUCCESS)
            {
               __DEBUG (isns_reg_debug & 1, Error Fetching Entity);
               return (ERROR);
            }

            foundFlag = FALSE;
            pnode = NULL;
            __DEBUG (isns_reg_debug &1,(Find iportal_list in entity));
            while ((pnode=GetNextNode(&p_entity->iportal_list, pnode)))
            {
               void *ptr;
               ptr = GetNodeData(pnode);
               if (0 == memcmp(ptr, &db_portal, sizeof(db_portal)))
               {
                  __DEBUG (isns_reg_debug &1, (set portal foundFlag));
                  foundFlag = TRUE;
                  break;
               }
            }

            if (!foundFlag)
            {
               __ISNS_COPY (&newPortalEntry.portal_ip_port, sizeof(newPortalEntry.portal_ip_port), &db_portal, sizeof(db_portal));
               newPortalEntry.portal_idx = p_portal->portal_index;
               __DEBUG (isns_reg_debug &1, (AddNode - iportal));
               AddNode(&p_entity->iportal_list, (char *)&newPortalEntry, sizeof(newPortalEntry));
            }

        strcpy(p_portal->entity_id.id,p_entity->eid.id);
            __DEBUG (isns_reg_debug &1, p_portal->entity_id.id is:%s,p_portal->entity_id.id);
            p_portal->entity_index = p_entity->entity_index;

            if (eidKeyIndex!=-1)
               eidKeyIndex = ISNSFindTag (eidKeyIndex+1, ISNS_ENTITY_ID, key_indx);

            if (eidKeyIndex!=-1)
               attr = (ISNS_Attr *)(key_indx[eidKeyIndex]);

            /* Store Entity Entry */
            __DEBUG (isns_reg_debug &1,Store Entity);
            rval = write_EntityObject(db_entity_id.id, entry);

         } while ( eidKeyIndex!=-1 );

         newPortalRegFlag = TRUE;
      }
      else
      {
         eidKeyIndex = 0;

         /* Any entities in the key */
         while (-1 != (eidKeyIndex = ISNSFindTag (eidKeyIndex, ISNS_ENTITY_ID, key_indx)))
         {
            attr = (ISNS_Attr *)(key_indx[ eidKeyIndex ]);
            strncpy(db_entity_id.id, (char *)&attr->val, sizeof(db_entity_id.id));
            if ( read_EntityObject (db_entity_id.id, &p_entity, &entry) )
            {
               return (ISNS_INVALID_REG_ERR);
            }

            foundFlag = FALSE;
            pnode = NULL;
            while ( (pnode = GetNextNode(&p_entity->iportal_list, pnode)) )
            {
               void *ptr;
               ptr = GetNodeData( pnode );
               if (0 == memcmp(ptr, &db_portal, sizeof(db_portal)))
               {
                  foundFlag = TRUE;
                  break;
               }
            }

            if ( !foundFlag )
            {
               /* Portal is not already in entity */
               __ISNS_COPY (&newPortalEntry.portal_ip_port, sizeof(newPortalEntry.portal_ip_port), &db_portal, sizeof(db_portal));
               newPortalEntry.portal_idx = p_portal->portal_index;
               AddNode(&p_entity->iportal_list, (char *)&newPortalEntry, sizeof(newPortalEntry));
               strcpy(p_portal->entity_id.id,p_entity->eid.id);

               /* Store Entity Entry */
               rval = write_EntityObject(db_entity_id.id,entry);
            }

            eidKeyIndex++;
         }
      }

      stopFlag = FALSE;
      updateFlag = FALSE;
      memset(p_scn_all_msg_buffer, 0, sizeof(ISNS_Msg));
      for ( ii = saveIndex; (ii < SNS_MAX_ATTRS) && (attr_indx[ ii ]); ii++ )
      {
         attr = (ISNS_Attr *) attr_indx[ ii ];
         switch ( attr->tag )
         {
         case ISNS_PORTAL_SYM_NAME:
            if (strncmp(p_portal->sym_name, (char *)&attr->val, MIN(PORTAL_SYM_NAME_SIZE, attr->len)))
            {
               memset (p_portal->sym_name, 0, attr->len);
               __ISNS_COPY (p_portal->sym_name, sizeof(p_portal->sym_name), &attr->val, attr->len);

               ISNSAppendKey( p_scn_all_msg_buffer, ISNS_PORTAL_SYM_NAME, 
                              PAD4(strlen(p_portal->sym_name)), p_portal->sym_name, 0 );

               updateFlag = TRUE;
            }
            break;
         case ISNS_PORTAL_GROUP_TAG:
          p_portal->default_portal_tag = attr->val.portal_group;
              break;
         case ISNS_PORTAL_IP:
         case ISNS_PORTAL_PORT:
            saveIndex = ii;
            stopFlag = TRUE;
            break;
         case ISNS_ESI_INTERVAL:
            p_portal->esi_interval = *(uint32_t *) & attr->val;

            if (p_portal->esi_interval != 0
                && p_portal->esi_interval < sns_esi_interval)
            {
               __DEBUG (isns_reg_debug & 1, (ESI Interval too small %u < %u),
                        p_portal->esi_interval, sns_esi_interval);

               p_portal->esi_interval = sns_esi_interval;
            }
            updateESIflag = TRUE;

            break;
         case ISNS_ESI_PORT:
            p_portal->esi_port = *(uint32_t *) & attr->val & 0xFFFF;
            updateESIflag = TRUE;
            break;
         case ISNS_PORTAL_CERT:
            AddCert (&p_portal->ptr_cert, (int *)&p_portal->cert_size,
                     &attr->val, attr->len);
            updateFlag = TRUE;
            break;
         case ISNS_ISCSI_TYPE:
            p_portal->node_type=p_portal->node_type | *(uint32_t *)&attr->val;
            updateFlag = TRUE;
         default:
            break;
         }

         if (stopFlag)
            break;
      }

      if ( updateESIflag )
      {
         /* Update ESI */
         if (p_portal->esi_port == 0 && p_portal->esi_interval != 0 &&
             p_md->cb.sock.socketType != ISNS_SOCKET_TYPE_TCP)
         {
            __DEBUG (isns_reg_debug & 1, (ERROR:Attempting a TCP ESI without a TCP
                                        session));
            return (ISNS_INVALID_REG_ERR);
         }
         if (p_portal->esi_port == 0 && p_portal->esi_interval != 0 &&
                   p_md->cb.sock.socketType == ISNS_SOCKET_TYPE_TCP)
         {
            p_portal->esi_sockfd=p_md->cb.sock.sockfd;
         }
      }

      __DEBUG(isns_reg_debug &1, Entity id:%s,p_portal->entity_id.id);

      /* Send Out Detailed SCN */
      if (newPortalRegFlag || updateFlag)
      {
            if ( read_EntityObject ((char *)p_portal->entity_id.id, &p_entity, &entry) )
               return ( ERROR );

            pnode2 = NULL;
            while ( (pnode2 = GetNextNode( &p_entity->iscsi_node_list, pnode2)) )
            {
               p_iscsi_name = (char *) GetNodeData( pnode2 );
               send_iscsi_scn_to_members ( NULL, p_iscsi_name, p_scn_all_msg_buffer, ISNS_SCN_OBJ_ADDED );
            }
      }

      if ( newPortalRegFlag )
         iSNS_stats.num_portals++;

      /* Store the portal */
      __DEBUG (isns_reg_debug &1,Store Portal:%s,db_portal.ip_addr.v);
      rval = write_PortalObject(&db_portal, entry2);

      /* insert the key(s) */
      if (p_rspmsg != NULL)
      {
         ISNSAppendKey (p_rspmsg, ISNS_PORTAL_IP, ISNS_PORTAL_IP_SIZE,
                        p_portal->ip_addr.v, 0);
         ISNSAppendKey (p_rspmsg, ISNS_PORTAL_PORT, ISNS_PORTAL_PORT_SIZE, 0,
                        p_portal->ip_port);
      }

      /* Loop on the following conditions. */
      attr = (ISNS_Attr *)(attr_indx[saveIndex]);

      if (!newPortalRegFlag)
         break;

      if (!attr_indx[saveIndex])
         break;

      newPortalRegFlag = TRUE;
      updateESIflag = FALSE;
      if ((attr->tag == ISNS_PORTAL_IP) || (attr->tag == ISNS_PORTAL_PORT))
         continue;
      else
         break;

      __DEBUG (isns_reg_debug &1,end of loop);
   }
   
   __DEBUG (isns_reg_debug &1,return);
   return (ISNS_NO_ERR);
}

/*********************************************************************
_ISNSdbAddAttrPortalGroup

Adds a portal group to the database.  The keys are appended to the
rsp message.
*********************************************************************/
int
ISNSdbAddAttrPortalGroup (ISNS_Attr **attr_indx, ISNS_Attr **key_indx, ISNS_Msg_Descp * p_md,
                    ISNS_Msg * p_rspmsg)
{
   ISNS_Key          *key = NULL;
   ISNS_Attr         *attr = NULL;
   SOIP_Portal_Group *p_portal_group = NULL;
   SOIP_Portal_Group_Key   db_portal_group;
   int               ii;
   int               rval;
   int               portalIpIndex;
   int               portalPortIndex;
   int               iscsiIndex;
   int               saveIndex;
   int               stopFlag = FALSE;
   int               portal_tag = 1;
   int               newPortalGroupRegFlag;
   
   __DEBUG (isns_reg_debug & 1, (Registering Portal Group));

   newPortalGroupRegFlag = FALSE;
   portalPortIndex = -1;
   portalIpIndex = -1;
   iscsiIndex = -1;

   memset(&db_portal_group,0,sizeof(SOIP_Portal_Group_Key));

   for (ii = 0; ii < SNS_MAX_ATTRS && key_indx[ ii ]
        && (portalIpIndex == -1 || portalPortIndex == -1 || iscsiIndex == -1); ii++)
   {
      key = (ISNS_Key *)( key_indx[ ii ] );
      switch ( key->tag )
      {
      case ISNS_PORTAL_GROUP_IP:
         /* Check to see if we already had an IP */
         if (portalIpIndex != -1)
         {
            __DEBUG (isns_reg_debug & 1, (PORTAL_IP tag found twice));
            return ( ISNS_INVALID_REG_ERR );
         }
         portalIpIndex = ii;
         break;
      case ISNS_PORTAL_GROUP_PORT:
         /* Check to see if we already had a PORT */
         if (portalPortIndex != -1)
         {
            __DEBUG (isns_reg_debug & 1, (PORTAL_PORT tag found twice));
            return ( ISNS_INVALID_REG_ERR );
         }
         portalPortIndex = ii;
         break;
      case ISNS_ISCSI_NODE_ID:
         /* Check to see if we already have an iscsi name */
         if (iscsiIndex != -1)
         {
            __DEBUG (isns_reg_debug & 1, (ISCSI Name tag found twice));
            return ( ISNS_INVALID_REG_ERR );
         }
         iscsiIndex = ii;
         break;
      default:
         break;
      }
   }

   if (portalIpIndex == -1 && portalPortIndex == -1)
   {
      /* No keys */
      newPortalGroupRegFlag = TRUE;
   }
   else if (portalIpIndex == -1 || portalPortIndex == -1)
   {
      /* One or the other key */
      __DEBUG (isns_reg_debug & 1, (Portal Reg: Missing keys in Key Attributes));
      return ( ISNS_INVALID_REG_ERR );
   }

   saveIndex = 0;
   /* Loop for every portal IP and portal PORT */
   while ( saveIndex < SNS_MAX_ATTRS )
   {
      if ( newPortalGroupRegFlag )
      {
         /* Search for the tag portal IP and portal PORT. */
         /* These are going to be used as the key. */
         portalIpIndex = -1;
         portalPortIndex = -1;

         stopFlag=FALSE;
         for (ii=saveIndex; (ii<SNS_MAX_ATTRS) && attr_indx[ii] && !stopFlag; ii++)
         {
            attr = (ISNS_Attr *) attr_indx[ ii ];
            switch ( attr->tag )
            {
               case ISNS_PORTAL_IP:
                  /* Check to see if we already had an IP */
                  if (portalIpIndex != -1)
                     stopFlag=TRUE;
                  else
                     portalIpIndex = ii;
                  break;
               case ISNS_PORTAL_PORT:
                  /* Check to see if we already had a PORT */
                  if (portalPortIndex != -1)
                     stopFlag=TRUE;
                  else
                     portalPortIndex = ii;
                  break;
               case ISNS_ISCSI_NODE_ID:
                  if (iscsiIndex != -1)
                     stopFlag=TRUE;
                  else
                     iscsiIndex = ii;
                  break;
               case ISNS_PORTAL_GROUP:
               case ISNS_PORTAL_GROUP_TAG:
                  portal_tag = attr->val.portal_group;
                  __DEBUG (isns_reg_debug &1,(Portal Group tag:%i),portal_tag);
                  break;
               default:
                  break;
            }
         }

         /* Check to see if we have all three indices */
         if ( portalIpIndex == -1 || portalPortIndex == -1 || iscsiIndex == -1)
         {
            __DEBUG (isns_reg_debug & 1, (Portal Group Registration, MISSING KEYS));
            return ( ISNS_INVALID_REG_ERR );
         }

         attr = (ISNS_Attr *) attr_indx[ portalPortIndex ];
         db_portal_group.ip_port = attr->val.ip_port;
         __DEBUG (isns_reg_debug &1,(Portal Group ip_port:%i),attr->val.ip_port);

         attr = (ISNS_Attr *) attr_indx[ portalIpIndex ];
         __ISNS_COPY (db_portal_group.ip_addr.v, sizeof(db_portal_group.ip_addr.v), &attr->val, ISNS_PORTAL_IP_SIZE);
         __DEBUG (isns_reg_debug &1,(Portal Group ip_addr:%s),db_portal_group.ip_addr.v);

         attr = (ISNS_Attr *) attr_indx[ iscsiIndex ];
         __ISNS_COPY (db_portal_group.id.v, sizeof(db_portal_group.id.v), &attr->val, attr->len);
         __DEBUG (isns_reg_debug &1,(Portal Group iscsi_name:%s),db_portal_group.id.v);

         saveIndex = ii;
      }
      else
      {
         /* Use Keys to fetch the portal Object */
         key = (ISNS_Key *) key_indx[ portalPortIndex ];
         db_portal_group.ip_port = key->val.ip_port;
         __DEBUG (isns_reg_debug &1,(Portal Group ip_port:%i),key->val.ip_port);

         key = (ISNS_Key *) key_indx[ portalIpIndex ];
         __ISNS_COPY (db_portal_group.ip_addr.v, sizeof(db_portal_group.ip_addr.v), &key->val, key->len);
         __DEBUG (isns_reg_debug &1,(Portal ip_addr len:%i),key->len);

         attr = (ISNS_Attr *) key_indx[ iscsiIndex ];
         __ISNS_COPY (db_portal_group.id.v, sizeof(db_portal_group.id.v), &attr->val, attr->len);
         __DEBUG (isns_reg_debug &1,(Portal Group iscsi_name:%s),db_portal_group.id.v);

         saveIndex = 0;
      
         /* Find portal_tag value */
         stopFlag = FALSE;
         for ( ii = 0; (ii < SNS_MAX_ATTRS) && attr_indx[ii] && !stopFlag; ii++ )
         {
           attr = (ISNS_Attr *) attr_indx[ ii ];
           switch ( attr->tag )
           {
           case ISNS_PORTAL_GROUP:
           case ISNS_PORTAL_GROUP_TAG:
            portal_tag = attr->val.portal_group;
            __DEBUG (isns_reg_debug &1,Portal Group tag:%i,p_portal_group->portal_tag);
            stopFlag = TRUE;
            break;
           default:
            break;
           }

           if (stopFlag)
            break;
         }
      }

      __DEBUG (isns_reg_debug &1,(read portal:%s),db_portal_group.id.v);

      rval = read_PortalGroupObject(&db_portal_group, &p_portal_group, &entry2);
      if ( (rval == SUCCESS) && (p_md->msg.hdr.flags & ISNS_FLAG_REPLACE_REG) )
      {
         __DEBUG (isns_reg_debug & 1,Deregistering PortalGroup);
         rval = delete_PortalGroupObject(&db_portal_group);
         rval = ERROR;
      }

      /* Create new entry */
      if ( rval != SUCCESS )
      {
         __DEBUG (isns_reg_debug &1,create new portal group entry);

         memset (&entry2, 0, sizeof (SOIP_DB_Entry));
         attr = (ISNS_Attr *) attr_indx[portalIpIndex];
         __ISNS_COPY (p_portal_group->ip_addr.v, sizeof(p_portal_group->ip_addr.v), attr->val.ip_addr.v, attr->len);

         attr = (ISNS_Attr *) attr_indx[portalPortIndex];
         p_portal_group->ip_port = attr->val.ip_port;

         attr = (ISNS_Attr *) attr_indx[iscsiIndex];
         __ISNS_COPY (p_portal_group->id.v, sizeof(p_portal_group->id.v), attr->val.node_name.v, attr->len);

         newPortalGroupRegFlag = TRUE;
      }

      p_portal_group->portal_tag = portal_tag;

      __DEBUG (isns_reg_debug &1,Store Portal Group:%s,db_portal_group.id.v);
      rval = write_PortalGroupObject(&db_portal_group, entry2);

      if ( newPortalGroupRegFlag )
         iSNS_stats.num_portal_groups++;

      if (p_rspmsg != NULL)
      {
         ISNSAppendKey (p_rspmsg, ISNS_PORTAL_GROUP_IP, ISNS_PORTAL_IP_SIZE,
                        p_portal_group->ip_addr.v, 0);
         ISNSAppendKey (p_rspmsg, ISNS_PORTAL_GROUP_PORT, ISNS_PORTAL_PORT_SIZE, 0,
                        p_portal_group->ip_port);
         ISNSAppendKey (p_rspmsg, ISNS_PORTAL_GROUP_TAG, ISNS_PORTAL_GROUP_TAG_SIZE, 0,
                        p_portal_group->portal_tag);
      }

      /* Loop on the following conditions. */
      attr = (ISNS_Attr *)(attr_indx[saveIndex]);

      if (!newPortalGroupRegFlag)
         break;

      if (!attr_indx[saveIndex])
         break;

      newPortalGroupRegFlag = FALSE;
      if ((attr->tag == ISNS_PORTAL_IP) || (attr->tag == ISNS_PORTAL_PORT))
         continue;
      else
         break;

      __DEBUG (isns_reg_debug &1,end of loop);
   }
   
   __DEBUG (isns_reg_debug &1, End of AddAttrPortalGroup);
   return (ISNS_NO_ERR);
}

/*********************************************************************
_SNSdbAddAttrDDS 

Adds one or more DDS(s) to the database.
*********************************************************************/
int
ISNSdbAddAttrDDS ( ISNS_Msg_Descp * p_md, ISNS_Msg * p_rspmsg )
{
   int              ii;
   int              rval;
   int              saveIndex;
   int              stopFlag;
   int              newRegFlag;
   int              ddKeyIndex;
   int              ddsSymIndex;
   int              ddAttrIndex;
   int              status_changed_flag;
   int              updateFlag;
   int              newMemberFlag;
   uint32_t         newState;
   int              dd_id;
   int              new_dd_id;
   uint32_t         count;
   uint32_t         new_scn_bitmap;
   uint32_t         updated_dd_array_size;
   ISNS_Key         *key;
   ISNS_Attr        *attr;
   ISNS_Key         newKey;
   SOIP_Dds         *p_dds;
   SOIP_Dd          *p_dd;
   SOIP_DD_Key      dds_key;
   ISNS_LIST_NODE   *pnode;
   ISNS_Attr        *src_attr;
   ISNS_Attr        *attr_indx[SNS_MAX_ATTRS];
   ISNS_Attr        *key_indx[SNS_MAX_ATTRS];
   int              symIndex;
   char             *p_sym;

   __DEBUG (isns_reg_debug & 1, (Registering DDS));

   ISNSParseMsg( &p_md->msg, attr_indx, key_indx, &src_attr );

   rval = Check_Authorization (src_attr);
   if (rval != SUCCESS)
     return (ISNS_AUTH_FAILED_ERR);

   /* Search the keys for DD_Set. */
   ddKeyIndex = ISNSFindTag (0, ISNS_DDS_ID, key_indx);
   if (ddKeyIndex != -1)
   {
      key = (ISNS_Key *)(key_indx[ddKeyIndex]);
      if (key->len == 0)
      {
         /* Reset flag */
         ddKeyIndex = -1;
      }
   }

   saveIndex = 0;
   while (saveIndex < SNS_MAX_ATTRS)
   {
      newRegFlag = FALSE;
      if (ddKeyIndex == -1)
      {
         /* DD_Set Tag not found in keys */

         /* Search the op_attr */
         ddAttrIndex = ISNSFindTag (saveIndex, ISNS_DDS_ID, attr_indx);
         if (ddAttrIndex != -1)
         {
            key = (ISNS_Key *)(attr_indx[ddAttrIndex]);
            saveIndex = ddAttrIndex + 1;
            if (key->len == 0)
            {
               /* Reset flag */
               ddAttrIndex = -1;
            }
         }
      }

      if ( ddKeyIndex != -1 )
      {
         /* DD_Set was found in the key */
         /* Use it to fetch */
         key = (ISNS_Key *)( key_indx[ ddKeyIndex ] );
      }
      else if (ddAttrIndex != -1)
      {
         /* DD_Set was found in the op attr */
         /* Use it to fetch */
         key = (ISNS_Key *)( attr_indx[ ddAttrIndex ] );

      }
      else
      {
         /* Generate a Unique DDS_ID */
         newKey.len = 4;
         symIndex = ISNSFindTag( saveIndex, ISNS_DDS_SYM_NAME, attr_indx );
         *(uint32_t *) & newKey.val = 0;
         if (symIndex != -1)
         {
            attr = (ISNS_Attr *)attr_indx[symIndex];
            p_sym = (char *)&attr->val;
            *(uint32_t *) & newKey.val = ISNS_GetDDS_ID_from_sym( p_sym );
         }

         if (!*(uint32_t *) & newKey.val)
            *(uint32_t *) & newKey.val = ISNS_GetNewDDS_ID ();

         key = &newKey;
      }

      dds_key.id = key->val.dds_id;
      if ( dds_key.id == 0 )
      {
         __DEBUG( isns_reg_debug, (DDS ID of zero is reserved) );
         return( ISNS_INVALID_REG_ERR );
      }

      rval = read_DDSObject(dds_key.id, &p_dds, &entry);
      if ( (rval == SUCCESS) && (p_md->msg.hdr.flags & ISNS_FLAG_REPLACE_REG) )
      {
         __DEBUG(isns_reg_debug & 1, "Deregistering DDS.\n");
         SNSdbRemoveAttrDDS_Entry (dds_key.id, NULL, NULL);
         rval = ERROR;
      }

      if ( rval != SUCCESS )
      {
         /* Create a new DDS element */
         memset (&entry, 0, sizeof (SOIP_DB_Entry));
         p_dds->id = *(uint32_t *) & key->val;

         if (-1 == (ddsSymIndex = ISNSFindTag(saveIndex, ISNS_DDS_SYM_NAME, attr_indx)))
         {
            return (ISNS_INVALID_REG_ERR);
         }
         else
         {
            attr = (ISNS_Attr *)attr_indx[ddsSymIndex];
            if (!strlen((char *)&attr->val))
            {
               return (ISNS_INVALID_REG_ERR);
            }
         }

         InitList(DDS_DD_LIST, p_dds);

         newRegFlag = TRUE;

      }

      new_scn_bitmap = 0;
      newMemberFlag = FALSE;
      updateFlag = FALSE;
      memset(p_scn_all_msg_buffer, 0, sizeof(ISNS_Msg));
      p_scn_all_msg_buffer->hdr.type = ISNS_SCN;
      ISNSAppendKey(p_scn_all_msg_buffer, ISNS_DDS_ID, ISNS_DDS_ID_SIZE, NULL, p_dds->id);

      stopFlag = FALSE;
      status_changed_flag = FALSE;

      __DEBUG (isns_reg_debug &1,Process the message attributes);

      for (ii = saveIndex;
           (ii < SNS_MAX_ATTRS) && (attr_indx[ii]) && !stopFlag; ii++)
      {
         attr = (ISNS_Attr *) attr_indx[ii];
         switch (attr->tag)
         {
            case ISNS_DDS_ID:
               stopFlag = TRUE;
               saveIndex = ii;
               break;

            case ISNS_DDS_SYM_NAME:
               if (FALSE == Check_Unique_DDS_Sym (p_dds, (char *)&attr->val))
               {
                  __DEBUG (isns_reg_debug & 1, (DDS sym name not unique:%s), (char *)&attr->val);
                  rval = ISNS_INVALID_REG_ERR;

                  if (newRegFlag)
                     DeleteList(&p_dds->dd_list);

                  return (rval);
               }
               strncpy (p_dds->sym_name, (char *)&attr->val, sizeof(p_dds->sym_name));
               break;

            case ISNS_DDS_STATUS:
               if (p_dds->status != *(uint32_t *)&attr->val)
               {
                  p_dds->status=*(uint32_t *)&attr->val;
                  status_changed_flag = TRUE;
                  updateFlag = TRUE;
                  ISNSAppendKey( p_scn_all_msg_buffer, ISNS_DDS_STATUS, ISNS_DDS_STATUS_SIZE, 
                                 NULL, p_dds->status );
               }
               break;

            case ISNS_DD_ID:
               new_dd_id = *(uint32_t *) & attr->val;
               if (ISNS_NO_ERR != Add_dds_to_dd (p_dds, new_dd_id))
               {
                  rval = ISNS_UNKNOWN_ERR;

                  if (newRegFlag)
                     DeleteList(&p_dds->dd_list);

                  return (rval);
               }
               ISNSAppendKey( p_scn_all_msg_buffer, ISNS_DD_ID, ISNS_DD_ID_SIZE, NULL, new_dd_id);
               newMemberFlag = TRUE;

               break;
            default:
               break;
         }
      }
      if (newRegFlag && !strlen(p_dds->sym_name))
      {
         __DEBUG(isns_reg_debug & 1, (Symbolic name missing));
         DeleteList(&p_dds->dd_list);
         return (ISNS_INVALID_REG_ERR);
      }
      /* Store Entry */
      rval = write_DDSObject(dds_key.id, entry);
      if ( newRegFlag )
         iSNS_stats.num_dds++;

      ISNSAppendKey ( p_rspmsg, ISNS_DDS_ID, ISNS_DDS_ID_SIZE, NULL,
                     p_dds->id );

      if ( status_changed_flag )
      {
         /* Status changed */
         updated_dd_array_size = 0;
         memset(dlist_node, 0, MAX_DD_PER_LIST*sizeof(uint32_t));
         pnode = NULL;

         while ( (pnode = GetNextNode ( &p_dds->dd_list, pnode ) ) )
         {
            dd_id = *(uint32_t *) GetNodeData (pnode);

            /* Change status for all DD */
            rval = read_DDObject(dd_id, &p_dd, &entry);
            if (rval != SUCCESS)
               return rval;

            newState = IsDDActive ( dd_id, 0 );

            if ( newState != p_dd->activeFlag)
            {
               p_dd->activeFlag = newState;

               dlist_node[ updated_dd_array_size ] = p_dd->id;
               updated_dd_array_size++;

               /* Store Entry */
               write_DDObject(dd_id, entry);
            }
         }

         /* Change status for all DD's members */
         for (count = 0; count<updated_dd_array_size; count++)
         {
            if (!dlist_node[ count ])
               break;

            rval = read_DDObject(dlist_node[count], &p_dd, &entry);
            if (rval != SUCCESS)
              return rval;

            /* Change all members to new state */
            change_dd_member_active_flag( p_dd );
         }
      }

      attr = (ISNS_Attr *)(attr_indx[ saveIndex ]);
      ddKeyIndex = -1;
      if (attr && attr->tag == ISNS_DDS_ID)
         continue;
      else
         break;
   }

   return (ISNS_NO_ERR);
}

/*********************************************************************
_SNSdbAddAttrDD

Adds one or more DD to the database.
*********************************************************************/
int
ISNSdbAddAttrDD ( ISNS_Msg_Descp *p_md, ISNS_Msg * p_rspmsg )
{
   int         ii;
   int         rval;
   int         ddKeyIndex;
   int         ddAttrIndex;
   int         ddSymIndex;
   int         saveIndex;
   int         stopFlag;
   int         newRegFlag;
   int         updateFlag;
   int         newMemberFlag;
   int         s;
   uint32_t    new_scn_bitmap;
   ISNS_Key    *key;
   ISNS_Attr   *attr;
   ISNS_Key    newKey;
   SOIP_Dd     *p_dd;
   SOIP_DD_Key dd_key;
   ISNS_Attr   *src_attr;
   ISNS_Attr   *attr_indx[SNS_MAX_ATTRS];
   ISNS_Attr   *key_indx[SNS_MAX_ATTRS];
   char        *p_sym;
   int         symIndex;
   uint32_t    iscsi_idx;

   __DEBUG (isns_reg_debug & 1, (Registering DD));
   ISNSParseMsg(&p_md->msg, attr_indx, key_indx, &src_attr);

   rval = Check_Authorization (src_attr);
   if (rval != SUCCESS)
     return (ISNS_AUTH_FAILED_ERR);

   /* Search the keys for DD_Set. */
   ddKeyIndex = ISNSFindTag (0, ISNS_DD_ID, key_indx);
   if (ddKeyIndex != -1)
   {
      key = (ISNS_Key *)(key_indx[ddKeyIndex]);
      if (key->len == 0)
      {
         ddKeyIndex = -1;
      }
   }

   saveIndex = 0;
   while (saveIndex < SNS_MAX_ATTRS)
   {
      newRegFlag = FALSE;
      if (ddKeyIndex == -1)
      {
         /* DD_ID Tag not found in keys */

         /* Search the op_attr */
         ddAttrIndex = ISNSFindTag (saveIndex, ISNS_DD_ID, attr_indx);
         if (ddAttrIndex != -1)
         {
            key = (ISNS_Key *)(attr_indx[ddAttrIndex]);
            saveIndex = ddAttrIndex + 1;
            if (key->len == 0)
            {
               ddAttrIndex = -1;
            }
         }
      }

      if (ddKeyIndex != -1)
      {
         /* DD_ID was found in the key */
         /* Use it to fetch */
         key = (ISNS_Key *)(key_indx[ddKeyIndex]);
      }
      else if (ddAttrIndex != -1)
      {
         /* DD_ID was found in the op attr */
         /* Use it to fetch */
         key = (ISNS_Key *)(attr_indx[ddAttrIndex]);

      }
      else
      {
         /* Generate a Unique DD_ID, or retrieve the id of a dd 
            with the same symbolic name. */
         newKey.len = 4;

         /* First try to see if a DD_ID is associated with this
            symbolic name */
         symIndex = ISNSFindTag(saveIndex, ISNS_DD_SYM_NAME, attr_indx);
         p_sym = NULL;
         if (symIndex != -1)
         {
            attr = (ISNS_Attr *)attr_indx[ symIndex ];
            p_sym = (char *)&attr->val;
            *(uint32_t *) &newKey.val = ISNS_GetDD_ID_from_sym( p_sym );
         }

         /* Get a new DD_ID */
         if (!*(uint32_t *)&newKey.val)
            *(uint32_t *) & newKey.val = ISNS_GetNewDD_ID ();

         key = &newKey;
      }

      dd_key.id = *(uint32_t *) &key->val;
      rval = read_DDObject(dd_key.id, &p_dd, &entry);
      if ( (rval == SUCCESS) && (p_md->msg.hdr.flags & ISNS_FLAG_REPLACE_REG) )
      {
         SNSdbRemoveDD_Entry ( dd_key.id, NULL, NULL );
         rval = ERROR;
      }

      if ( rval != SUCCESS)
      {
         __DEBUG (isns_reg_debug &1,(Create a new DD element));
         /* Create a new DD element */
         memset ( &entry, 0, sizeof (SOIP_DB_Entry) );
         p_dd->id = *(uint32_t *) & key->val;

         if (-1 == (ddSymIndex = ISNSFindTag(saveIndex, ISNS_DD_SYM_NAME, attr_indx)))
         {
            return (ISNS_INVALID_REG_ERR);
         }
         else
         {
            attr = (ISNS_Attr *)attr_indx[ddSymIndex];
            if (!strlen((char *)&attr->val))
            {
               return (ISNS_INVALID_REG_ERR);
            }
         }

         InitList(DD_DDS_LIST, p_dd );
         InitList(DD_MEMBER_LIST, p_dd );

         newRegFlag = TRUE;
      }

      __DEBUG (isns_reg_debug &1, p_dd dds_dds_list id:%i,p_dd->dds_list.list_id);
      __DEBUG (isns_reg_debug &1, p_dd member_list id:%i,p_dd->member_list.list_id);

      /* Prep the SCN message */
      updateFlag = FALSE;
      newMemberFlag = FALSE;
      new_scn_bitmap = 0;
      memset( p_scn_all_msg_buffer, 0, sizeof(ISNS_Msg));
      p_scn_all_msg_buffer->hdr.type = ISNS_SCN;
      ISNSAppendKey(p_scn_all_msg_buffer, ISNS_DD_ID, ISNS_DD_ID_SIZE, NULL, p_dd->id);

      stopFlag = FALSE;
      newMemberFlag = FALSE;
      iscsi_idx = 0;
      for (ii = saveIndex;
           (ii < SNS_MAX_ATTRS) && (attr_indx[ ii ]) && !stopFlag; ii++)
      {
         attr = (ISNS_Attr *) attr_indx[ii];
         switch (attr->tag)
         {
            case ISNS_DD_ID:
            {
               saveIndex = ii;
               stopFlag = TRUE;
               break;
            }
            case ISNS_DD_FEATURE_BITMAP:
            {
               p_dd->feature = *(uint32_t *)&attr->val;
               break;
            }
            case ISNS_DD_SYM_NAME:
            {
               if (memcmp(p_dd->sym_name, (char *)&attr->val, attr->len ))
               {
                  if ( FALSE == Check_Unique_DD_Sym ( p_dd, (char *)&attr->val ) )
                  {
                     __DEBUG ( isns_reg_debug & 1,Error DD Symbolic name not unique);
                     return ( ISNS_INVALID_REG_ERR );
                  }
                  strncpy ( p_dd->sym_name, (char *)&attr->val, sizeof(p_dd->sym_name) );

                  ISNSAppendKey( p_scn_all_msg_buffer, ISNS_DD_SYM_NAME, 
                                 attr->len, (char *)&attr->val, 0 );

                  updateFlag = TRUE;
               }
               break;
            }
            case ISNS_DD_ISCSI_MEMBER_IDX:
            {
               iscsi_idx = *(uint32_t *)&attr->val;
               break;
            }
            case ISNS_ISCSI_NODE_ID:
            case ISNS_DD_ISCSI_MEMBER:
            {
               s = Add_DD_to_ISCSI_Node ( p_dd, (char *)&attr->val );

               Add_DD_Member ( p_dd, (char *) &attr->val, 
                  PAD4( strlen((char *)&attr->val) ),  ISNS_DD_ISCSI_MEMBER,
                  s == ISNS_NO_ERR?ISNS_DD_MEMBER_ENABLE:ISNS_DD_MEMBER_DISABLE, iscsi_idx);

               ISNSAppendKey( p_scn_all_msg_buffer, ISNS_DD_ISCSI_MEMBER, attr->len, (char *)&attr->val, 0);
               send_iscsi_scn_to_members (NULL, (char *)&attr->val, p_scn_all_msg_buffer, ISNS_SCN_OBJ_ADDED);

               newMemberFlag = TRUE;
               iscsi_idx = 0;
               break;
            }
            case ISNS_PORT_NAME:
            case ISNS_DD_IFCP_MEMBER:
            {
               s = Add_DD_to_IFCP_Node (p_dd->id, (char *)&attr->val);

               Add_DD_Member (p_dd, (char *) &attr->val, PORT_NAME_SIZE, ISNS_DD_IFCP_MEMBER,
                  s == ISNS_NO_ERR?ISNS_DD_MEMBER_ENABLE:ISNS_DD_MEMBER_DISABLE, 0);
               updateFlag = TRUE;
               ISNSAppendKey( p_scn_all_msg_buffer, ISNS_DD_IFCP_MEMBER, attr->len, (char *)&attr->val, 0);
               send_iscsi_scn_to_members (NULL, (char *)&attr->val, p_scn_all_msg_buffer, ISNS_SCN_OBJ_ADDED);

               break;
            }
            default:
               __DEBUG(isns_reg_debug & 1, (Unrecognized tag for function: %u), attr->tag);
               break;
         }
      }

      if (newRegFlag && !strlen(p_dd->sym_name))
      {
         __DEBUG(isns_reg_debug &1, (Symbolic name missing));
         DeleteList( &p_dd->dds_list );
         DeleteList( &p_dd->member_list );
         return ( ISNS_INVALID_REG_ERR );
      }
      /* Store Entry */
      __DEBUG (isns_reg_debug &1, now write dd record dds_list id:%i,entry.data.dd.dds_list.list_id);
      rval = write_DDObject(dd_key.id, entry);
      if ( newRegFlag )
         iSNS_stats.num_dd++;

      ISNSAppendKey (p_rspmsg, ISNS_DD_ID, ISNS_DD_ID_SIZE, NULL, p_dd->id);

      attr = (ISNS_Attr *)(attr_indx[saveIndex]);
      ddKeyIndex = -1;
      if (attr && attr->tag == ISNS_DD_ID)
         continue;
      else
         break;
   }

   return (ISNS_NO_ERR);
}

/*********************************************************************
*********************************************************************/
int
Add_DD_Member ( SOIP_Dd * p_dd, char * p_node_name, int p_len, uint32_t type, 
                uint32_t status, uint32_t index )
{
   ISNS_LIST_NODE *pnode;
   SOIP_Dd_Member *p_member;
   SOIP_Dd_Member new_member;
   SOIP_Iscsi *p_node;
   int rval;
   SOIP_DB_Entry  lentry;
   int index_number;

   __DEBUG (isns_reg_debug &1, Add_DD_Member:%s, p_node_name);

   rval = read_ISCSIObject(p_node_name, &p_node, &lentry);
   if ( rval != SUCCESS)
   {
        index_number = index;
   } 
   else
   {
      index_number = p_node->iscsi_index;
   }
   
   pnode = NULL;
   while ((pnode = GetNextNode(&p_dd->member_list, pnode)))
   {
      p_member = (SOIP_Dd_Member *)GetNodeData(pnode);

      if ((p_member->type == type) &&
          (0 == memcmp (p_member->node_id, p_node_name, strlen(p_node_name) )))
      {
         return (ISNS_NO_ERR);
      }
   }

   memset(&new_member, 0, sizeof(new_member));

   new_member.type = type;
   new_member.status = status;
   __ISNS_COPY (new_member.node_id, sizeof(new_member.node_id), p_node_name, strlen(p_node_name));
   new_member.node_idx = index_number;

   __DEBUG (isns_reg_debug &1, Add_DD_Member member.type:%i,new_member.type);
   __DEBUG (isns_reg_debug &1, Add_DD_Member member.node_id:%s,new_member.node_id);
   __DEBUG (isns_reg_debug &1, Add_DD_Member member.node_idx:%i,new_member.node_idx);

   AddNode(&p_dd->member_list, (char *)&new_member, sizeof(new_member));

   return (ISNS_NO_ERR);
}

/*********************************************************************
_SNSdbAddAttrEntity

Adds one or more entities
*********************************************************************/
int
SNSdbAddAttrEntity ( ISNS_Attr **attr_indx, ISNS_Attr **key_indx,
                     ISNS_Msg_Descp * p_md, ISNS_Msg * p_rspmsg)
{
   int                  ii;
   ISNS_Key             *key = NULL;
   ISNS_Attr            *attr;
   SOIP_Entity          *p_entity;
   SOIP_Entity_Key      db_entity_id;
   int                  entityKeyIndex;
   int                  entityAttrIndex;
   int                  newRegFlag;
   int                  rval;
   static int           entityNumber = 0;
   time_t               t;
   char                 tempEntityId[ ENTITY_ID_SIZE ] = {0};
   int                  updateFlag;
   int          node_type = 0;
   int                  nodeTypeIndex = -1;

   newRegFlag = FALSE;
   __DEBUG (isns_reg_debug & 1, (Registering Entity));
   memset (&db_entity_id, 0, sizeof (db_entity_id));

   /* Search the keys for entity ID. */
   entityKeyIndex = -1;
   entityAttrIndex = -1;

   entityKeyIndex = ISNSFindTag ( 0, ISNS_ENTITY_ID, key_indx );
   if ( entityKeyIndex != -1 )
   {
      /* Entity wasn't in the keys */

      /* Search the op attr */
      key = (ISNS_Key *)(key_indx[ entityKeyIndex ]);
      if ( key->len == 0 )
         entityKeyIndex = -1;

      strncpy ( db_entity_id.id, (char *)&key->val, sizeof(db_entity_id.id) );
   }
   else
   {
      entityAttrIndex = ISNSFindTag (0, ISNS_ENTITY_ID, attr_indx);
      if ( entityAttrIndex != -1 )
      {
         /* Use the Attr */
         attr = (ISNS_Attr *)(attr_indx[ entityAttrIndex ]);
         if ( attr->len == 0 )
            entityAttrIndex = -1;
         strncpy ( db_entity_id.id, (char *)&attr->val, sizeof(db_entity_id.id) );
      }
   }

   if ( entityAttrIndex != -1 && entityKeyIndex != -1 )
   {
      __DEBUG (isns_reg_debug & 1, (Same tag found in Key and Op Attr %u),
               key->tag);
      return (ISNS_MSG_FMT_ERR);
   }
   else if ( entityAttrIndex == -1 && entityKeyIndex == -1 )
   {
      /* Entity ID was not provided so generate one. */
      memset (tempEntityId, 0, sizeof (tempEntityId));
      sprintf(tempEntityId, "isns.entity.%4.4d", entityNumber++);
      strncpy ((char *) db_entity_id.id, (char *) tempEntityId, sizeof(db_entity_id.id) );
   }

   nodeTypeIndex = ISNSFindTag ( 0, ISNS_ISCSI_TYPE, attr_indx );
   if (nodeTypeIndex != -1)
   {
     attr = (ISNS_Attr *) attr_indx[nodeTypeIndex];
     node_type = *(uint32_t *)&attr->val;
   }

   rval = read_EntityObject(db_entity_id.id, &p_entity, &entry);
   /* If an entry was found and the update flag is not set deregister it */
   /* Also if the entry was found and the node_type is different then */
   /* don't de-register it */
   if ( (rval == SUCCESS) && (p_md->msg.hdr.flags & ISNS_FLAG_REPLACE_REG) )
   {
       if ( p_entity->node_type == node_type) 
       {
         __DEBUG (isns_reg_debug &1, RemoveEntity);
         ISNSdbRemoveAttrEntityEntry (db_entity_id.id, NULL, NULL);
         rval = ERROR;
       }
   }

   if ( rval != SUCCESS )
   {
      /* create a new entity element */
      memset ( &entry, 0, sizeof (SOIP_DB_Entry) );
      p_entity->entity_index = ISNSGetNewEntityIdx();

      SOIP_Entity_Id *p_idx;
      memset (&entry3, 0, sizeof(SOIP_Entity_Id));
      p_idx = &entry3.data.entity_idx;
      __ISNS_COPY (p_idx, sizeof(entry3.data.entity_idx), &db_entity_id.id, sizeof(SOIP_Entity_Id));
      rval = write_EntityidxObject(p_entity->entity_index, entry3);

      strncpy (p_entity->eid.id, db_entity_id.id, sizeof(p_entity->eid.id) );
      InitList(ENTITY_PORTAL_LIST, p_entity);
      InitList(ENTITY_ISCSI_LIST, p_entity);
      InitList(ENTITY_FCP_LIST, p_entity);

      if ( ISNSFindTag (0, ISNS_ENTITY_TYPE, attr_indx) == -1 )
      {
         /* No ENTITY TYPE FLAG FOUND! */
         if ( ISNSFindTag (0, ISNS_PORT_NAME, attr_indx) != -1 )
         {
            p_entity->eid_type = ENTITY_TYPE_IFCP;
         }
         else if ( ISNSFindTag (0, ISNS_ISCSI_NODE_ID, attr_indx) != -1 )
         {
            p_entity->eid_type = ENTITY_TYPE_ISCSI;
         }
      }

      newRegFlag = TRUE;
   }
   p_entity->timestamp.t_time = time (&t);

   __DEBUG (isns_reg_debug &1,nodecount:%i,p_entity->iscsi_node_list.node_count);

   updateFlag = FALSE;
   for (ii = 0; (ii < SNS_MAX_ATTRS) && (attr_indx[ ii ]); ii++)
   {
      attr = (ISNS_Attr *) attr_indx[ ii ];
      switch ( attr->tag )
      {
         case ISNS_ENTITY_TYPE:
            if (!( *(uint32_t *)&attr->val == ENTITY_TYPE_ISCSI ||
                   *(uint32_t *)&attr->val == ENTITY_TYPE_IFCP ))
            {
               __DEBUG ( isns_reg_debug & 1, (Invalid Entity Type) );
               return ( ISNS_INVALID_REG_ERR );
            }

            if ((uint32_t)p_entity->eid_type != *(uint32_t *) &attr->val)
            {
               p_entity->eid_type = *(uint32_t *) & attr->val;
               updateFlag = TRUE;
            }
            break;
         case ISNS_MGMT_IP:
            __ISNS_COPY (p_entity->mgmt_ip_addr.v, sizeof(p_entity->mgmt_ip_addr.v), &attr->val, ISNS_IP_SIZE);
            updateFlag = TRUE;
            break;
         case ISNS_PROT_VER:
         {  
            if (attr->val.prot_ver.ver_max < attr->val.prot_ver.ver_min)
            {
               __DEBUG (isns_reg_debug & 1, (Version incorrect));
               return (ISNS_INVALID_REG_ERR);
            }
            p_entity->prot_ver.ver_max = attr->val.prot_ver.ver_max;
            p_entity->prot_ver.ver_min = attr->val.prot_ver.ver_min;
            updateFlag = TRUE;
            break;
         }
         case ISNS_ENTITY_PERIOD:
         {
            if (p_entity->period != attr->val.period)
            {
               p_entity->period = attr->val.period;
               updateFlag = TRUE;
            }
            break;
         }  
         case ISNS_ENTITY_CERT:
            AddCert ( &p_entity->ptr_cert, (int *)&p_entity->cert_size,
                      &attr->val, attr->len );
            updateFlag = TRUE;
            break;
         case ISNS_ISCSI_TYPE:
            p_entity->node_type = p_entity->node_type | *(uint32_t *)&attr->val;
            updateFlag = TRUE;
            break;

         default:
            break;
      }
   }

   /* Compose a response msg. */
   ISNSAppendKey ( p_rspmsg, ISNS_ENTITY_ID, PAD4 (strlen (db_entity_id.id)),
                   db_entity_id.id, 0 );

   if ( newRegFlag )
      iSNS_stats.num_entity++;

   rval = write_EntityObject(db_entity_id.id, entry);
   __DEBUG (isns_reg_debug & 1, (Save Entity:%i),rval);

   return ( rval );
}

/*********************************************************************
_ISNSdbAddAttrNode

Adds one or more FC Nodes to the database.
*********************************************************************/
int
ISNSdbAddAttrNode (ISNS_Attr **attr_indx, ISNS_Attr **key_indx, ISNS_Msg_Descp * p_md,
                   ISNS_Msg * p_rspmsg)
{
   int ii, rval;
   ISNS_Key *key;
   ISNS_Attr *attr;
   SOIP_Fc_Node *p_node;
   SOIP_DB_Node_Name db_node_name;
   int fcNodeIndex;
   int saveIndex;
   int stopFlag;
   int newRegFlag;
   int newKeyFlag;

   __DEBUG (isns_reg_debug & 1, (Registering FC Node));

   /* Search for Node Name Key */
   newKeyFlag = TRUE;
   newRegFlag = TRUE;
   fcNodeIndex = ISNSFindTag (0, ISNS_NODE_NAME, key_indx);

   /* Search Op Attributes for Enitity_id */
   saveIndex = 0;

   /* Also see conditions at bottom */
   while (saveIndex < SNS_MAX_ATTRS)
   {
      key = NULL;
      fcNodeIndex = -1;
      if (newRegFlag)
      {
         /* New registration */
         for (ii = saveIndex;
              ii < SNS_MAX_ATTRS && attr_indx[ii] && fcNodeIndex == -1; ii++)
         {
            attr = (ISNS_Attr *) attr_indx[ii];
            if (attr->tag == ISNS_NODE_NAME)
            {
               fcNodeIndex = ii;
            }
         }
         saveIndex = ii;
         key = (ISNS_Key *) attr_indx[fcNodeIndex];
      }
      else
      {
         /* Update */
         key = (ISNS_Key *) key_indx[fcNodeIndex];
      }

      if (!key)
      {
         __DEBUG (isns_reg_debug & 1, (Fatal Error));
         return (ERROR);
      }

      /* 
       * Set the node name to be unique for db fetch
       */

      memset (&db_node_name, 0, sizeof (db_node_name));

      __ISNS_COPY (db_node_name.v, sizeof(db_node_name.v), key->val.node_name.v, ISNS_NODE_NAME_SIZE);
      db_node_name.v[8] = (char)SOIP_DB_Node_Id;

      rval = read_FCNodeObject(db_node_name.v, &p_node, &entry);
      if ( (rval == SUCCESS) && (p_md->msg.hdr.flags & ISNS_FLAG_REPLACE_REG) )
      {
         __DEBUG (isns_reg_debug & 2, (Deregistering FC Node));
         SNSRemoveFCNodeEntry (key->val.node_name.v, NULL);
         rval = ERROR;
      }

      if ( rval != SUCCESS )
      {
         /* create a new node element */
         memset (&entry, 0, sizeof (SOIP_DB_Entry));
         memset (&entry.data.node.node_ipa, SNS_UNREGISTERED, NODE_IPA_SIZE);
         __ISNS_COPY (entry.data.node.node_name.v, sizeof(entry.data.node.node_name.v), 
                 db_node_name.v, ISNS_NODE_NAME_SIZE);
         InitList(FCP_PORTAL_LIST, p_node);

         newKeyFlag = TRUE;
      }

      /* Store the Node Name */
      __ISNS_COPY (p_node->node_name.v, sizeof(p_node->node_name.v), entry.data.node.node_name.v, ISNS_NODE_NAME_SIZE);

      stopFlag = FALSE;
      for (ii = saveIndex;
           (ii < SNS_MAX_ATTRS) && (attr_indx[ii]) && !stopFlag; ii++)
      {
         attr = (ISNS_Attr *) attr_indx[ii];
         switch (attr->tag)
         {
         case ISNS_NODE_SYM_NAME:
            strncpy (p_node->sym_name,
                     attr->val.sym_node_name, NODE_SYM_NAME_SIZE);
            break;

         case ISNS_FC_NODE_IP:
            __ISNS_COPY (&p_node->ip_addr.v, sizeof(p_node->ip_addr.v), &attr->val.ip_addr.v, IP_ADDR_SIZE);
            break;

         case ISNS_FC_NODE_IPA:
            __ISNS_COPY (&p_node->node_ipa.v, sizeof(p_node->node_ipa.v), &attr->val.ipa.v, NODE_IPA_SIZE);
            break;
         case ISNS_NODE_NAME:
            stopFlag = TRUE;
            saveIndex = ii;
            break;
         case ISNS_FC_NODE_CERT:
            AddCert (&p_node->ptr_cert, (int *)&p_node->cert_size,
                     &attr->val, attr->len);
            break;
         default:
            break;
         }
      }

      rval = write_FCNodeObject(db_node_name.v, entry);

      /* Compose a response */
      if (newKeyFlag)
      {
         ISNSAppendKey (p_rspmsg, ISNS_NODE_NAME, ISNS_NODE_NAME_SIZE,
                        p_node->node_name.v, 0);
         newKeyFlag = FALSE;
      }

      /* Decide on Looping */
      attr = (ISNS_Attr *)attr_indx[saveIndex];
      if (attr && attr->tag == ISNS_NODE_NAME)
         continue;
      else
         break;

   }                            /* End While */

   return (rval);
}

/*********************************************************************
_SNSdbAddAttrPort

Adds the iFCP Node(s) to the database.
*********************************************************************/
int
SNSdbAddAttrPort (ISNS_Attr **attr_indx, ISNS_Attr **key_indx, ISNS_Msg_Descp * p_md,
                  ISNS_Msg * p_rspmsg)
{
   SOIP_DB_Node_Name db_node_name;
   ISNS_Key *key;
   ISNS_Attr *attr;
   SOIP_Fc_Node *p_fc_node;
   SOIP_Ifcp *p_ifcp_node;
   SOIP_Entity *p_entity;
   SOIP_Entity_Key db_entity;
   char *entity_id;
   char buffer[MAX_ENTITY_ID_SIZE] = { 0 };
   int entityKeyIndex, portKeyIndex;
   int newPortRegFlag;
   int saveIndex;
   int stopFlag;
   int entity_len;
   int free_slot;
   int updateNodeFlag = FALSE;
   int ii, rval;
   //char *p_wwpn;

   __DEBUG (isns_reg_debug & 1, (Registering iFCP WWPN));

   /* Scan for Entity Keys */
   entityKeyIndex = -1;
   portKeyIndex = -1;
   newPortRegFlag = TRUE;
   for (ii = 0; ii < MAX_PARSE_ATTRS && key_indx[ii]; ii++)
   {
      key = (ISNS_Key *)(key_indx[ii]);
      switch (key->tag)
      {
      case ISNS_PORT_NAME:
         portKeyIndex = ii;
         newPortRegFlag = FALSE;
         break;
      case ISNS_ENTITY_ID:
         entityKeyIndex = ii;
         break;
      default:
         break;
      }
   }

   saveIndex = 0;
   while (saveIndex < MAX_PARSE_ATTRS)
   {
      if (newPortRegFlag)
      {
         portKeyIndex = -1;
         portKeyIndex = ISNSFindTag (saveIndex, ISNS_PORT_NAME, attr_indx);
         saveIndex = portKeyIndex + 1;
         key = (ISNS_Key *) attr_indx[portKeyIndex];
      }
      else
      {
         key = (ISNS_Key *) key_indx[portKeyIndex];
      }

      if (portKeyIndex == -1)
      {
         __DEBUG(isns_reg_debug & 1, "***WARNING: PORT_NAME tag not found.\n");
         return (ISNS_INVALID_REG_ERR);
      }

      rval = read_FCPortObject((char *)&key->val, &p_ifcp_node, &entry);
      if ( (rval == SUCCESS) && (p_md->msg.hdr.flags & ISNS_FLAG_REPLACE_REG) )
      {
         __DEBUG (isns_reg_debug & 2, (Deregistering iFCP Node));
         SNSRemoveIFCPNodeEntry ((char *)&key->val, NULL, NULL);
         rval = ERROR;
      }

      if ( rval != SUCCESS)
      {
         /* Fetch Entity */
         if (entityKeyIndex != -1)
         {
            key = (ISNS_Key *)(key_indx[entityKeyIndex]);

            entity_id = (char *)&key->val;
            entity_len = key->len;

         }
         else
         {
            /* Check to see if we registered an entity id in this msg */
            attr = ISNSFindKeyInResponseMsg (ISNS_ENTITY_ID, p_rspmsg);
            if (attr == NULL)
            {
               int rval;

               /* Create a new entity */
               rval = SNSdbAddAttrEntity (attr_indx, key_indx, p_md,
                                          p_rspmsg);

               if (rval != SUCCESS)
               {
                  __DEBUG (isns_reg_debug & 1, (Unsuccessful entity reg));
                  return (ERROR);
               }

               /* Check to see if we registered an entity id in this msg */
               attr = ISNSFindKeyInResponseMsg (ISNS_ENTITY_ID, p_rspmsg);

               if (attr == NULL)
               {
                  __DEBUG (isns_reg_debug & 1, (Unsuccessful entity reg));
                  return (ERROR);
               }
            }

            entity_id = (char *)&attr->val;
            entity_len = attr->len;
         }

         /* Fetch the entity */
         memset(&db_entity, 0, sizeof(db_entity));
         strncpy(db_entity.id, entity_id, sizeof(db_entity.id) );

         if (SUCCESS != read_EntityObject (db_entity.id, &p_entity, &entry2))
            return (ISNS_INVALID_REG_ERR);

         /* create a new port element */
         memset (&entry, 0, sizeof (SOIP_DB_Entry));
         __ISNS_COPY (p_ifcp_node->entity_id.id, sizeof(p_ifcp_node->entity_id.id), entity_id, entity_len);
         InitList(FCP_DD_LIST, &p_ifcp_node->dd_id_list);

         /* Search for free port */
         if (!FindNode(&p_entity->ifcp_node_list, p_ifcp_node->port_name.v, PORT_NAME_SIZE))
         {
            AddNode(&p_entity->ifcp_node_list, p_ifcp_node->port_name.v, PORT_NAME_SIZE);
         }

         rval = write_EntityObject(db_entity.id, entry2);
      }

      stopFlag = FALSE;
      for (ii = saveIndex;
           (ii < SNS_MAX_ATTRS) && (attr_indx[ii]) && !stopFlag; ii++)
      {
         attr = (ISNS_Attr *) attr_indx[ii];
         switch (attr->tag)
         {
         case ISNS_PORT_NAME:
            stopFlag = TRUE;
            newPortRegFlag = TRUE;
            saveIndex = ii;
            break;

         case ISNS_PORT_SYM_NAME:
            __ISNS_COPY (p_ifcp_node->sym_name, sizeof(p_ifcp_node->sym_name), attr->val.sym_port_name, attr->len);
            break;

         case ISNS_FABRIC_PORT_NAME:
            __ISNS_COPY (&p_ifcp_node->fabric_port_name.v, sizeof(p_ifcp_node->fabric_port_name.v), 
                    &attr->val.fabric_port_name.v, PORT_NAME_SIZE);
            break;

         case ISNS_NODE_NAME:
            __ISNS_COPY (&p_ifcp_node->node_name.v, sizeof(p_ifcp_node->node_name.v), 
                    &attr->val.node_name.v, ISNS_NODE_NAME_SIZE);

            /* Update the FC NODE. */
            updateNodeFlag = TRUE;

            break;

         case ISNS_FC_PORT_IP:
            __ISNS_COPY (p_ifcp_node->ip_addr.v, sizeof(p_ifcp_node->ip_addr.v), 
                    &attr->val, ISNS_IP_SIZE);
            break;

         case ISNS_FC_HARD_ADDR:
            __ISNS_COPY (&p_ifcp_node->hard_addr.v, sizeof(p_ifcp_node->hard_addr.v), 
                    &attr->val.hard_addr.v, HARD_ADDR_SIZE);
            break;

         case ISNS_PORT_ID:
            __ISNS_COPY (&p_ifcp_node->id, sizeof(p_ifcp_node->id), &attr->val, PORT_ID_SIZE);
            break;
         case ISNS_PORT_TYPE:
            p_ifcp_node->type = *(uint32_t *) & attr->val;
            break;
         case ISNS_IFCP_NODE_CERT:
            AddCert (&p_ifcp_node->ptr_cert, (int *)&p_ifcp_node->cert_size,
                     &attr->val, attr->len);
            break;
         case ISNS_FC4_TYPE:
            __DEBUG (isns_reg_debug & 1, (Not Implemented));
            break;
         case ISNS_FC_COS:
            __ISNS_COPY (&p_ifcp_node->cos, sizeof(p_ifcp_node->cos), &attr->val.cos, SNS_COS_SIZE);
            break;
         case ISNS_FC4_DESC:
            strncpy ( p_ifcp_node->fc_descr.v, (char *) &attr->val, sizeof(p_ifcp_node->fc_descr.v) );
            break;
         case ISNS_FC4_FEATURE:
            __ISNS_COPY (&p_ifcp_node->fc_feature, sizeof(p_ifcp_node->fc_feature), &attr->val, FC_FEATURE_SIZE);
            break;
         case ISNS_IFCP_SCN_BITMAP:
            p_ifcp_node->scn_bitmap=*(uint32_t *)&attr->val;
            if (p_md->cb.sock.socketType == ISNS_SOCKET_TYPE_TCP)
            {
               p_ifcp_node->scn_sockfd = p_md->cb.sock.sockfd;
            }
            else
            {
               p_ifcp_node->scn_sockfd = 0;
            }
            break;
         default:
            break;
         }
      }

      memset (buffer, 0, sizeof (buffer));
      __ISNS_COPY (buffer, sizeof(buffer), p_ifcp_node->port_name.v, PORT_NAME_SIZE);

      /* Activate DD Membership */
      if (SUCCESS != Activate_DD_Membership (ISNS_DD_IFCP_MEMBER, p_ifcp_node))
         return (ERROR);

      /* Store Port Name */
      rval = write_FCPortObject(p_ifcp_node->port_name.v,entry);

      /* response */
      ISNSAppendKey (p_rspmsg, ISNS_PORT_NAME, PORT_NAME_SIZE,
                     p_ifcp_node->port_name.v, 0);

      /* Update the Node Entry */
      memset (&db_node_name, 0, sizeof (db_node_name));

      if (memcmp (db_node_name.v, p_ifcp_node->node_name.v, ISNS_NODE_NAME_SIZE))
      {
         /* Node Name is not NULL */
         __ISNS_COPY (db_node_name.v, sizeof(db_node_name.v), p_ifcp_node->node_name.v, ISNS_NODE_NAME_SIZE);
         db_node_name.v[8] = (char)SOIP_DB_Node_Id;

         rval = read_FCNodeObject(db_node_name.v, &p_fc_node, &entry2);
         if ( rval != SUCCESS)
         {
            /* We will create the node */
            memset (&entry2, 0, sizeof (entry2));
            memset (&entry2.data.node.node_ipa, SNS_UNREGISTERED,
                    NODE_IPA_SIZE);
            __ISNS_COPY (entry2.data.node.node_name.v, sizeof(entry2.data.node.node_name.v), 
                    db_node_name.v, ISNS_NODE_NAME_SIZE);

         }

         /* insert port name into free slot in node's port list */
         free_slot = -1;

         /* Is this port name already associated with the node? */
         if (!FindNode(&p_fc_node->port_list, (char *)&p_ifcp_node->port_name.v, PORT_NAME_SIZE))
         {
            AddNode(&p_fc_node->port_list, (char *)&p_ifcp_node->port_name.v, PORT_NAME_SIZE);
         }

         rval = write_FCNodeObject(db_node_name.v, entry2);
      }

      /* Determine to loop */
      attr = (ISNS_Attr *)(attr_indx[saveIndex]);
      if (attr && attr->tag == ISNS_PORT_NAME)
         continue;
      else
         break;
   }
   return (rval);
}

/*********************************************************************
Registers one or more iSCSI Node(s) by adding it to the database.
*********************************************************************/
int
ISNSdbAddAttrISCSINode ( ISNS_Attr **attr_indx, ISNS_Attr **key_indx,
                        ISNS_Msg_Descp * p_md, ISNS_Msg * p_rspmsg )
{
   int                  ii;
   int                  rval;
   ISNS_Key             *key;
   ISNS_Attr            *attr;
   SOIP_Entity          *p_entity;
   SOIP_Iscsi           *p_node;
   SOIP_ISCSI_Node_Id   db_node_name;
   SOIP_Entity_Key      db_entity;
   int                  nodeIndex;
   int                  entityIndex;
   int                  saveIndex;
   int                  stopFlag;
   int                  newRegFlag;
   int                  updateFlag;
   uint32_t             scn_bitmap= 0;
   uint32_t             safe_len;
   int                  newIscsiNodeFlag;

   __DEBUG ( isns_reg_debug & 1, (Registering iSCSI Node) );

   newRegFlag = FALSE;

   /* Search for ISCSI NODE ID Key */
   nodeIndex = ISNSFindTag ( 0, ISNS_ISCSI_NODE_ID, attr_indx );
   if (nodeIndex != -1)
      newRegFlag = TRUE;

   /* Also see conditions at bottom */
   saveIndex = 0;
   while ( saveIndex < SNS_MAX_ATTRS )
   {
      newIscsiNodeFlag = FALSE;
      key = NULL;
      if ( newRegFlag )
      {
         /* New registration */
         nodeIndex = -1;
         nodeIndex = ISNSFindTag (saveIndex, ISNS_ISCSI_NODE_ID, attr_indx);

         if ( nodeIndex == -1 )
         {
            __DEBUG(isns_reg_debug & 1, (iSCSI Node tag not found));
            return (ERROR);
         }

         saveIndex = nodeIndex + 1;

         key = (ISNS_Key *) (attr_indx[ nodeIndex ]);
      }
      else
      {
         /* Update */
         key = (ISNS_Key *) (key_indx[ nodeIndex ]);
      }

      if ( !key )
      {
         __DEBUG (isns_reg_debug & 1, (No ISNS_ISCSI_NODE_ID tag));
         return ( ISNS_MSG_FMT_ERR );
      }

      memset ( &db_node_name, 0, sizeof (db_node_name) );
      strncpy ((char *) db_node_name.v, (char *) &key->val, key->len );

      rval = read_ISCSIObject(db_node_name.v, &p_node, &entry);
      if ( (rval == SUCCESS) && (p_md->msg.hdr.flags & ISNS_FLAG_REPLACE_REG))
      {
         __DEBUG ( isns_reg_debug & 2, (Deregistering iSCSI Node %s), db_node_name.v);
         SNSRemoveISCSINodeEntry ( db_node_name.v, NULL, NULL );
         rval = ERROR;
      }

      if ( rval != SUCCESS)
      {
         /* Create new node */
         if ( !newRegFlag )
         {
            __DEBUG (isns_reg_debug & 1, (iSCSI Node not found %s),
                     db_node_name.v);
            return ( ISNS_INVALID_REG_ERR );
         }

         scn_bitmap |= ISNS_SCN_OBJ_ADDED;

         /* Search for an ENTITY Key */
         entityIndex = -1;
         entityIndex = ISNSFindTag ( 0, ISNS_ENTITY_ID, key_indx );

         memset ( &db_entity, 0, sizeof (db_entity) );

         if ( entityIndex != -1 )
         {
            key = (ISNS_Key *)( key_indx[ entityIndex ] );
            strncpy(db_entity.id, (char *)&key->val, sizeof(db_entity.id) );
         }
         else
         {
            /* if not found, search rsp mesg */
            attr = ISNSFindKeyInResponseMsg ( ISNS_ENTITY_ID, p_rspmsg );
            if ( attr == NULL )
            {
               /* Create a new entity */
               rval = SNSdbAddAttrEntity ( attr_indx, key_indx,
                                          p_md, p_rspmsg );

               if ( rval != ISNS_NO_ERR )
               {
                  __DEBUG ( isns_reg_debug & 1, (Error Creating entity) );
                  return (rval);
               }

               /* Check again */
               attr = ISNSFindKeyInResponseMsg ( ISNS_ENTITY_ID, p_rspmsg );

               if ( attr == NULL )
               {
                  __DEBUG ( isns_reg_debug & 1, (Error Creating entity) );
                  return ( ISNS_UNKNOWN_ERR );
               }
            }

            strncpy(db_entity.id, (char *)&attr->val, sizeof(db_entity.id) );
         }

         /* Create a new node entry */
         memset ( &entry, 0, sizeof (SOIP_DB_Entry) );
         strncpy ( p_node->id.v, db_node_name.v, sizeof(p_node->id.v) );
         p_node->iscsi_id_len = PAD4(strlen(p_node->id.v));

         p_node->iscsi_index = ISNSGetNewISCSIIdx();

         SOIP_ISCSI_Node_Id *p_idx;
         memset (&entry3, 0, sizeof(SOIP_ISCSI_Node_Id));

         p_idx = &entry3.data.iscsi_idx;
         __ISNS_COPY (p_idx, sizeof(entry3.data.iscsi_idx), &p_node->id.v, sizeof(SOIP_ISCSI_Node_Id));

         rval = write_ISCSIidxObject(p_node->iscsi_index, entry3);

         strncpy ( p_node->entity_id.id, db_entity.id, sizeof(p_node->entity_id.id) );
         InitList(ISCSI_DD_LIST, p_node );

         /* Setup for an entity fetch */
         __DEBUG ( isns_reg_debug &1,call read_EntityObject:%s,db_entity.id);
         if ( SUCCESS != read_EntityObject (db_entity.id, &p_entity, &entry2) )
         {
            return ( ISNS_INVALID_REG_ERR );
         }

         if ( p_entity->eid_type != ENTITY_TYPE_ISCSI )
         {
            __DEBUG( isns_reg_debug & 1, (Entity is not ISCSI type: %d), p_entity->eid_type );
            return ( ISNS_UNKNOWN_ERR );
         }

         __DEBUG (isns_reg_debug &1,iscsi_node_count:%i,p_entity->iscsi_node_list.node_count);

         /* Insert into iscsi_node_list */
         if (!FindNode( &p_entity->iscsi_node_list, p_node->id.v, PAD4(strlen (p_node->id.v)) ))
         {
            AddNode( &p_entity->iscsi_node_list, p_node->id.v, PAD4(strlen (p_node->id.v)) );
         }

         p_node->entity_index = p_entity->entity_index;
         newIscsiNodeFlag = TRUE;

         /* Check if member of a Domain */
         Check_if_member_of_DD (p_node->id.v);
      }

      memset( p_scn_all_msg_buffer, 0, sizeof(ISNS_Msg));
      p_scn_all_msg_buffer->hdr.type = ISNS_SCN;
      ISNSAppendKey( p_scn_all_msg_buffer, ISNS_ISCSI_NODE_ID, p_node->iscsi_id_len, p_node->id.v, 0 );

      updateFlag = FALSE;
      stopFlag = FALSE;
      for (ii = saveIndex;
           (ii < SNS_MAX_ATTRS) && (attr_indx[ii]) && !stopFlag; ii++)
      {
         attr = (ISNS_Attr *) attr_indx[ii];
         switch ( attr->tag )
         {
            case ISNS_ISCSI_NODE_ID:
            {
               stopFlag = TRUE;
               saveIndex = ii;
               break;
            }
            case ISNS_ISCSI_TYPE:
            {
               if ( p_node->type != *(uint32_t *)&attr->val )
               {
                  p_node->type = *(uint32_t *)&attr->val;
                  ISNSAppendKey( p_scn_all_msg_buffer, ISNS_ISCSI_TYPE, 
                                 ISNS_ISCSI_TYPE_SIZE, NULL, p_node->type );
                  updateFlag = TRUE;
               }
               break;
            }
            case ISNS_ISCSI_ALIAS:
            {
               safe_len = MIN( strlen(p_node->alias), attr->len );
               if ( !memcmp(p_node->alias, &attr->val, safe_len ) )
               {
                  strncpy( p_node->alias, (char *)&attr->val, sizeof(p_node->alias) );
                  p_node->alias_len = safe_len;
                  ISNSAppendKey( p_scn_all_msg_buffer, ISNS_ISCSI_ALIAS, 
                                 PAD4(strlen(p_node->alias)), p_node->alias, 0);
                  updateFlag = TRUE;
               }
               break;
            }
            case ISNS_ISCSI_SCN_BITMAP:
            {
               if (p_node->scn_bitmap!=*(uint32_t *)&attr->val)
               {
                  p_node->scn_bitmap = *(uint32_t *)&attr->val;

                  ISNSAppendKey( p_scn_all_msg_buffer, ISNS_ISCSI_SCN_BITMAP, 
                                 ISNS_SCN_BITMAP_SIZE, NULL, p_node->scn_bitmap );
                  updateFlag = TRUE;
               }

               if ( p_md->cb.sock.socketType == ISNS_SOCKET_TYPE_TCP )
               {
                  p_node->scn_sockfd = p_md->cb.sock.sockfd;
               }
               else
               {
                  p_node->scn_sockfd = 0;
               }

               break;
            }
            case ISNS_ISCSI_CERT:
            {
               AddCert (&p_node->ptr_cert, (int *)&p_node->cert_size,
                        &attr->val, attr->len);
               break;
            }
            default:
               break;
         }
      }

      /* Activate DD Membership */
      Activate_DD_Membership ( ISNS_DD_ISCSI_MEMBER, p_node );

      /* Store iSCSI Node Entry */
      rval = write_ISCSIObject(p_node->id.v, entry);

      if ( newIscsiNodeFlag )
      {
         iSNS_stats.num_iscsi++;

         ISNSAppendKey ( p_rspmsg, ISNS_ISCSI_NODE_ID,
                         PAD4 (strlen (p_node->id.v)), p_node->id.v, 0 );

         /* Store Entity */
         rval = write_EntityObject(db_entity.id, entry2);
      }

      if ( newIscsiNodeFlag )
         send_iscsi_scn_to_members ( p_node, NULL, p_scn_all_msg_buffer, ISNS_SCN_OBJ_ADDED );
      else if ( updateFlag )
         send_iscsi_scn_to_members ( p_node, NULL, p_scn_all_msg_buffer, ISNS_SCN_OBJ_UPDATED );

      /* Decide on Looping */
      attr = (ISNS_Attr *) ( attr_indx[ saveIndex ] );
      if ( attr && attr->tag == ISNS_ISCSI_NODE_ID )
         continue;
      else
         break;
   }

   return ( ISNS_NO_ERR );
}

/*********************************************************************
Registers a SCN.
*********************************************************************/
int
ISNS_RegisterSCN ( ISNS_Msg_Descp * p_md, ISNS_Msg * rspmsg)
{
   int                  rval;
   int                  ii;
   int                  iscsiKeyIndex;
   int                  entityKeyIndex;
   int                  ifcpKeyIndex;
   int                  scn_bitmap_index;
   ISNS_Key             *key;
   SOIP_Iscsi           *p_node;
   SOIP_Ifcp            *p_ifcp_node;
   SOIP_ISCSI_Node_Id   db_node_name;
   SOIP_Entity          *p_entity = NULL;
   ISNS_LIST_NODE       *pnode = NULL;
   ISNS_Attr            *src_attr;
   ISNS_Attr            *attr_indx[SNS_MAX_ATTRS];
   ISNS_Attr            *key_indx[SNS_MAX_ATTRS];
   SOIP_Portal_Key  *db_portal;

   __DEBUG (isns_reg_debug &1,(RegisterSCN));

   ISNSParseMsg( &p_md->msg, attr_indx, key_indx, &src_attr );

   /* Validate the src attr for Permission */

   /* Scan the keys */
   iscsiKeyIndex = -1;
   ifcpKeyIndex = -1;
   entityKeyIndex = -1;

   for (ii = 0; ii < MAX_PARSE_ATTRS && key_indx[ ii ]; ii++)
   {
      key = (ISNS_Key *)(key_indx[ ii ]);
      switch ( key->tag )
      {
          case ISNS_ISCSI_NODE_ID:
             iscsiKeyIndex = ii;
             break;
          case ISNS_PORT_NAME:
             ifcpKeyIndex = ii;
             break;
          default:
             break;
      }
   }

   if ( iscsiKeyIndex != -1 )
   {
      key = (ISNS_Key *)(key_indx[ iscsiKeyIndex ]);

      /* Fetch the node */
      memset (&db_node_name, 0, sizeof (db_node_name));
      strncpy ((char *) db_node_name.v, (char *) &key->val, sizeof(db_node_name.v) );

      rval = read_ISCSIObject(db_node_name.v, &p_node, &entry);
      if (rval != SUCCESS)
        return rval;

      scn_bitmap_index = ISNSFindTag (0, ISNS_ISCSI_SCN_BITMAP, attr_indx);
      if (scn_bitmap_index == -1)
      {
         __DEBUG (isns_reg_debug & 1, (No SCN Bitmap in request));
         return (ISNS_SCN_REG_REJECTED_ERR);
      }
      key = (ISNS_Key *)(attr_indx[scn_bitmap_index]);
      p_node->scn_bitmap=*(uint32_t *)&key->val;

      if ( p_md->cb.sock.socketType == ISNS_SOCKET_TYPE_TCP )
      {
         /* Use TCP for SCN */
         p_node->scn_sockfd = p_md->cb.sock.sockfd;
         __DEBUG (isns_reg_debug &1,(SCN sockfd = %#x),p_md->cb.sock.sockfd);
      }
      else
      {
         /* Find a UDP port for SCN */
         int foundSCNPortal;
         SOIP_Portal *p_portal;

         p_node->scn_sockfd = 0;

         /* Scan Portals */

         /* Fetch entity */
         read_EntityObject (p_node->entity_id.id, &p_entity, &entry2);

         /* Fetch portal */
         foundSCNPortal = FALSE;
         pnode = NULL;
         while ( (pnode = GetNextNode(&p_entity->iportal_list, pnode)) != NULL)
         {
            db_portal = GetNodeData(pnode);
            rval = read_PortalObject(db_portal, &p_portal, &entry3);
            if (rval != SUCCESS)
              return rval;

            /* Get first ESI/SCN Portal */
            if (p_portal->esi_port != 0)
            {
               foundSCNPortal = TRUE;
               break;
            }
         }

         /* Send out SCN */
         if (!foundSCNPortal)
         {
            __DEBUG (isns_reg_debug & 1, (No portals to send out SCN));
            return (ISNS_SCN_REG_REJECTED_ERR);
         }
      }

      /* Store the node */
      write_ISCSIObject(db_node_name.v, entry);

      return (ISNS_NO_ERR);
   }
   else if (ifcpKeyIndex != -1)
   {
      key = (ISNS_Key *)(key_indx[ ifcpKeyIndex ]);

      rval = read_FCPortObject((char *)&key->val, &p_ifcp_node, &entry);
      if (rval != SUCCESS)
        return rval;

      scn_bitmap_index = ISNSFindTag (0, ISNS_IFCP_SCN_BITMAP, attr_indx);
      if (scn_bitmap_index == -1)
      {
         __DEBUG (isns_reg_debug & 1, (No SCN Bitmap in request));
         return ( ISNS_SCN_REG_REJECTED_ERR );
      }
      key = (ISNS_Key *)(attr_indx[scn_bitmap_index]);
      p_ifcp_node->scn_bitmap=*(uint32_t *)&key->val;

      if (p_md->cb.sock.socketType == ISNS_SOCKET_TYPE_TCP)
      {
         p_ifcp_node->scn_sockfd = p_md->cb.sock.sockfd;
      }
      else
      {
         p_ifcp_node->scn_sockfd = 0;
      }

      /* Store the node */
      write_FCPortObject((char *)&key->val, entry);

      return ( ISNS_NO_ERR );
   }
   else
   {
      __DEBUG (isns_reg_debug & 1, (No valid key in SCN request));
      return ( ERROR );
   }

   return ( ERROR );
}

int Check_if_member_of_DD (char * id)
{
   return SUCCESS;
}
/*********************************************************************
_Add_DD_to_ISCSI_Node 
Adds the DD to the iSCSI Node's DD list.
*********************************************************************/
int
Add_DD_to_ISCSI_Node (SOIP_Dd *p_dd, char *p_node_name)
{
   SOIP_DB_Entry lentry;
   SOIP_Iscsi *p_node;
   int id;
   int rval;

   __DEBUG (isns_reg_debug &1,(ISCSI_Node:%s),p_node_name);
   id = p_dd->id;

   rval = read_ISCSIObject(p_node_name, &p_node, &lentry);
   if (rval != SUCCESS)
     return rval;

   if (!FindNode(&p_node->dd_id_list, (char *)&id, sizeof(id)))
   {
      AddNode(&p_node->dd_id_list, (char *)&id, sizeof(id));
   }

   if ( p_dd->activeFlag )
      p_node->activeFlag = TRUE;

   rval = write_ISCSIObject(p_node_name, lentry);

   return ( ISNS_NO_ERR );
}

/*********************************************************************
_Add_DD_to_IFCP_Node

Add the DD to the iFCP's DD list.
*********************************************************************/
int
Add_DD_to_IFCP_Node (int id, char *p_node_name)
{
   SOIP_Ifcp *p_node;
   SOIP_DB_Entry lentry;
   int rval;

   rval = read_FCPortObject(p_node_name, &p_node, &lentry);
   if (rval != SUCCESS)
      return rval;

   if (!FindNode(&p_node->dd_id_list, (char *) &id, sizeof(id)))
   {
      AddNode(&p_node->dd_id_list, (char *)&id, sizeof(id));
   }

   rval = write_FCPortObject(p_node_name, lentry);

   return (ISNS_NO_ERR);
}

/*********************************************************************
_Add_dds_to_dd

This will add a DDS to the DD's DDS list.  Keep in mind that each
DD maintains a list of DDS.
*********************************************************************/
int
Add_dds_to_dd (SOIP_Dds *p_dds, int dd_id)
{
   SOIP_DB_Entry lentry;
   SOIP_Dd *p_dd;
   int rval;

   __DEBUG (isns_reg_debug &1, Add_dds_to_dd );

   rval = read_DDObject(dd_id, &p_dd, &lentry);
   if (rval != SUCCESS)
     return rval;

   if (!FindNode(&p_dds->dd_list, (char *) &dd_id, sizeof(dd_id)))
   {
      AddNode(&p_dds->dd_list, (char *) &dd_id, sizeof(dd_id));
   }

   if ( p_dds->status && !p_dd->activeFlag )
   {
      p_dd->activeFlag = TRUE;
   }

   if (!FindNode(&p_dd->dds_list, (char *) &p_dds->id, sizeof(p_dds->id)))
   {
      AddNode(&p_dd->dds_list, (char *) &p_dds->id, sizeof(p_dds->id));
   }

   write_DDObject(dd_id, lentry);

   return ( ISNS_NO_ERR );
}

/*********************************************************************
_Check_Unique_DDS_Sym

Checks for a unique DDS symbolic name.
*********************************************************************/
int
Check_Unique_DDS_Sym (SOIP_Dds * p_dds, char *p_sym_name)
{
   SOIP_DB_Entry lentry;
   SOIP_Dds *p_dds2;
   int rval;
   ISNS_DBKey key = { 0 };

   key.tag = DDS_ID_KEY;

   while (SNSdbGetNextOfKey (&key) == SUCCESS)
   {
      if (key.val.dd_key.id == p_dds->id)
         continue;

      rval = read_DDSObject(p_dds->id, &p_dds2, &lentry);
      if (rval != SUCCESS)
         return (rval);

      if (0 == strcmp (p_dds2->sym_name, p_sym_name))
      {
         return (FALSE);
      }

   }

   return (TRUE);
}

/*********************************************************************
_ISNS_GetNewDD_ID

Gets a new unique DD ID.
*********************************************************************/
int
ISNS_GetNewDD_ID ( void )
{
   static int current_ID = 0;
   SOIP_DB_Entry lentry;
   int rval;
   SOIP_Dd *p_dd;

   do
   {
      current_ID++;
      rval = read_DDObject(current_ID, &p_dd, &lentry);
   }
   while ( rval == SUCCESS);

   return (current_ID);
}

/*********************************************************************
_Check_Unique_DD_Sym

Checks for a unique DD name.

RETURNS:
Return TRUE if sym name is unique.
*********************************************************************/
int
Check_Unique_DD_Sym (SOIP_Dd * p_dd, char *p_sym_name)
{
   SOIP_DB_Entry lentry;
   SOIP_Dd *p_dd2;
   int rval;
   ISNS_DBKey key = { 0 };

   /* retrieve all DD */
   key.tag = DD_ID_KEY;

   while (SNSdbGetNextOfKey (&key) == SUCCESS)
   {
      if (key.val.dd_key.id == p_dd->id)
         continue;

      rval = read_DDObject(key.val.dd_key.id, &p_dd2, &lentry);
      if (rval != SUCCESS)
         return rval;

      if (0 == strcmp (p_dd2->sym_name, p_sym_name))
      {
         return (FALSE);
      }
   }

   return (TRUE);
}

/********************************************************************
********************************************************************/
int
ISNSAddSCNCallbackEntry ( char *nodename, int type, int bitmap,
                          void (*cfunc)(ISNS_ATTR_VALS_CB *avcb_ptr) )
{
   SOIP_Scn_Callback_Entry new_entry;

   memset( &new_entry, 0, sizeof( new_entry ) );

   if ( ISNS_PORT_NAME == type )
   {
      __ISNS_COPY (new_entry.name.v, sizeof(new_entry.name.v), nodename, ISNS_PORT_NAME_SIZE );
   }
   else if ( ISNS_ISCSI_NODE_ID == type )
   {
      strncpy( new_entry.name.v, nodename, sizeof(new_entry.name.v) );
   }
   else if ( ISNS_ENTITY_ID == type )
   {
      strncpy( new_entry.name.v, nodename, sizeof(new_entry.name.v) );
   }
   else
   {
      __DEBUG( isns_reg_debug & 1, (SCN node type wrong: %d), type );
      return ( ERROR );
   }

   new_entry.type = type;
   new_entry.scn_callback_func = cfunc;
   new_entry.bitmap = bitmap;

   if ( NULL == FindNode( &scn_callback_list, (char *)&new_entry, sizeof(new_entry)) )
   {
      AddNode( &scn_callback_list, (char *)&new_entry, sizeof(new_entry) );
   }

   return ( SUCCESS );
}


/**************************************************
**************************************************/
int
ISNS_RegisterSCNCallback ( ISNS_Msg_Descp *p_md)
{
   int         rval;
   int         bitmapIdx;
   int         cbIdx;
   void        *func;
   ISNS_Attr   *attr;
   ISNS_Attr   *attr_indx[SNS_MAX_ATTRS];
   ISNS_Attr   *key_indx[SNS_MAX_ATTRS];

   ISNSParseMsg( &p_md->msg, attr_indx, key_indx, NULL );
   if ( key_indx[0] == NULL )
   {
      __DEBUG( isns_reg_debug & 1, (Missing key) );
      return ( ERROR );
   }

   if (-1 == (bitmapIdx = ISNSFindTag(0, ISNS_ISCSI_SCN_BITMAP, attr_indx)))
   {
      __DEBUG( isns_reg_debug & 1, (Missing key) );
      return ( ERROR );
   }

   if (-1 == (cbIdx = ISNSFindTag(0, ISNS_SCN_CALLBACK, attr_indx)))
   {
      __DEBUG( isns_reg_debug & 1, (Missing key) );
      return ( ERROR );
   }

   attr = (ISNS_Attr *)attr_indx[ cbIdx ];
   func = attr->val.scn_callback;

   rval = ISNSAddSCNCallbackEntry((char *)&((ISNS_Key *)key_indx[ 0 ])->val, 
       ((ISNS_Key *)key_indx[ 0 ])->tag, ((ISNS_Key *)key_indx[ bitmapIdx ])->tag, func);

   return (rval);
}

/*********************************************************************
_AddCert

Allocates space and copies the cert.
*********************************************************************/
int
AddCert (void **ptr, int *size, void *p_cert, int cert_size)
{
   if (*ptr)
   {
      ISNSFreeBuffer (*ptr);
      *ptr = NULL;
   }
   *size = cert_size;
   *ptr = (void *)ISNSAllocBuffer (cert_size);
   memcpy (*ptr, p_cert, *size);

   return (SUCCESS);
}

/*********************************************************************
Actives a DD membership.  A DD member can be inactive when it is
deleted.  When the member is reregistered, this function is called to
reactivate it.
*********************************************************************/
int
Activate_DD_Membership (uint32_t type, void *p_node)
{
   SOIP_DB_Entry     lentry;
   SOIP_Dd           *p_dd;
   ISNS_DBKey        key = { 0 };
   ISNS_LIST_NODE    *pnode;
   SOIP_Dd_Member    *p_member;
   SOIP_Iscsi        *p_iscsi = NULL;
   SOIP_Ifcp         *p_ifcp = NULL;
   uint32_t          index;
   char              *p_node_name;
   ISNS_LIST         *ddlist;
   int               memberAddedFlag;
   int               rval;


   __DEBUG (isns_reg_debug &1,Activate_DD_Membership);

   /* Get First DD Entry */
   key.tag = DD_ID_KEY;
   key.len = 0;

   switch ( type )
   {
      case ISNS_DD_ISCSI_MEMBER:
         p_iscsi = (SOIP_Iscsi *) p_node;
         p_node_name = p_iscsi->id.v;
         ddlist = &p_iscsi->dd_id_list;
         index = p_iscsi->iscsi_index;
         break;
      case ISNS_DD_IFCP_MEMBER:
         p_ifcp = (SOIP_Ifcp *) p_node;
         p_node_name = (char *)&p_ifcp->port_name;
         ddlist = &p_ifcp->dd_id_list;
         index = p_ifcp->ifcp_index;
         break;
      default:
         __LOG_ERROR ("Activate_DD_Membership(): Unknown type =%u.", type);
         return (ERROR);
         break;
   }

   /* Loop */
   memberAddedFlag = FALSE;
   while ( SNSdbGetNextOfKey ( &key ) == SUCCESS )
   {
      rval = read_DDObject(key.val.dd_key.id, &p_dd, &lentry);
      if (rval != SUCCESS)
         return rval;

      /* Find iSCSI Node */
      pnode = NULL;
      while ( (pnode=GetNextNode(&p_dd->member_list, pnode)) )
      {

         p_member=(SOIP_Dd_Member *)GetNodeData( pnode );

         if (p_member->type != type)
            continue;

         if (type == ISNS_DD_ISCSI_MEMBER &&
             0 != strcmp ( p_member->node_id, p_node_name ))
            continue;

         if (type == ISNS_DD_IFCP_MEMBER &&
             0 != memcmp ( p_member->node_id, p_node_name, 8))
            continue;

         /* Add to iSCSI Node's DD List */
         if (!FindNode( ddlist, (char *)&p_dd->id, sizeof(p_dd->id) ))
         {
            AddNode( ddlist, (char *)&p_dd->id, sizeof(p_dd->id) );
         }

         /* Active DD's Node Membership */
         p_member->status = ISNS_DD_MEMBER_ENABLE;
         p_member->node_idx = index;

         switch ( type )
         {
            case ISNS_DD_ISCSI_MEMBER:
               Add_DD_Member(p_dd,p_iscsi->id.v,PAD4(strlen(p_iscsi->id.v)),ISNS_DD_ISCSI_MEMBER,ISNS_DD_MEMBER_ENABLE,p_iscsi->iscsi_index);
               break;
            default:
               break;
         }

         if ( p_dd->activeFlag )
         {
            switch ( type )
            {
               case ISNS_DD_ISCSI_MEMBER:
                  p_iscsi->activeFlag = TRUE;
                  break;
               case ISNS_DD_IFCP_MEMBER:
                  p_ifcp->activeFlag = TRUE;
                  break;
               default:
                  break;
            }
         }

         memberAddedFlag = TRUE;
         ISNSAppendKey( p_scn_all_msg_buffer, ISNS_DD_ID, ISNS_DD_ID_SIZE, NULL, p_dd->id);

         switch ( type )
         {
            case ISNS_DD_ISCSI_MEMBER:
               ISNSAppendKey( p_scn_all_msg_buffer, ISNS_ISCSI_NODE_ID, PAD4(strlen(p_member->node_id)),
                              (char *)p_member->node_id, 0);

               ISNSAppendKey( p_scn_all_msg_buffer, ISNS_ISCSI_SCN_BITMAP, 
                              ISNS_SCN_BITMAP_SIZE, NULL, ISNS_SCN_MEMBER_ADDED);
               break;
            case ISNS_DD_IFCP_MEMBER:
               ISNSAppendKey( p_scn_all_msg_buffer, ISNS_PORT_NAME, ISNS_PORT_NAME_SIZE,
                              (char *)p_member->node_id, 0);

               ISNSAppendKey( p_scn_all_msg_buffer, ISNS_IFCP_SCN_BITMAP, 
                              ISNS_SCN_BITMAP_SIZE, NULL, ISNS_SCN_MEMBER_ADDED);
               break;
            default:
               break;
         }

         /* Store Entry */
         write_DDObject(p_dd->id,lentry);

         break;
      }
   }

   if ( memberAddedFlag )
   {
        send_iscsi_scn_to_members ( p_node, NULL, p_scn_all_msg_buffer, ISNS_SCN_OBJ_ADDED );
   }
   else
   {
      Add_ISCSINode_Default_DD (p_node);

      switch ( type )
      {
           case ISNS_DD_ISCSI_MEMBER:
                p_iscsi->activeFlag = TRUE;
                break;
           case ISNS_DD_IFCP_MEMBER:
                p_ifcp->activeFlag = TRUE;
                break;
      }
   }

  return ( SUCCESS );
}

#if 0
/**************************************************
This will reregister SCN when a connection drops
**************************************************/
int
ISNS_Re_Reg_SCN(void)
{
   ISNS_LIST_NODE *lnode;
   SOIP_Scn_Callback_Entry *pdata;

   if (IsEmptyList(&scn_callback_list))
      return (SUCCESS);

   memset(p_scn_md, 0, sizeof(ISNS_Msg_Descp));
   p_scn_md->msg.hdr.type = ISNS_SCN_REG_REQ;
   p_scn_md->msg.hdr.xid = ISNSGetXID();
   lnode = NULL;
   lnode = GetNextNode(&scn_callback_list, lnode);
   while (lnode)
   {
      p_scn_md->msg.hdr.msg_len = 0;

      pdata = (SOIP_Scn_Callback_Entry *)GetNodeData(lnode);

      __DEBUG( isns_reg_debug & 1, (Sending SCN re-reg msg node = %s), pdata->name.v);

      /* SRC */
      ISNSAppendKey(&p_scn_md->msg, 0, 0, NULL, 0);

      /* Key */
      ISNSAppendKey(&p_scn_md->msg, ISNS_ISCSI_NODE_ID, PAD4(strlen(pdata->name.v)), pdata->name.v, 0);
      ISNSAppendKey(&p_scn_md->msg, ISNS_DELIMITER, ISNS_DELIMITER_SIZE, NULL, 0);

      /* Attr */
      ISNSAppendKey(&p_scn_md->msg, ISNS_ISCSI_SCN_BITMAP, ISNS_SCN_BITMAP_SIZE, NULL, pdata->bitmap);

      ISNSSendMsg2Server(&p_scn_md->msg);
      lnode = GetNextNode(&scn_callback_list, lnode);
   }

   return (SUCCESS);
}
#endif

/*********************************************************************
*********************************************************************/
int
change_dd_member_active_flag( SOIP_Dd *p_dd )
{
   ISNS_LIST_NODE       *lnode;
   ISNS_LIST_NODE       *lnode2;
   SOIP_Dd_Member       *p_member;
   SOIP_DB_Entry        lentry;
   uint32_t             activeFlag;
   SOIP_Iscsi           *p_node;
   SOIP_ISCSI_Node_Id   db_node_name;
   uint32_t             old_activeFlag;
   int                  rval;

   /* For each dd member */
   lnode = NULL;
   while ( (lnode = GetNextNode(&p_dd->member_list, lnode)) )
   {
      p_member = (SOIP_Dd_Member *) GetNodeData(lnode);

      if (!p_member->type || p_member->status == ISNS_DD_MEMBER_DISABLE)
         continue;

      switch (p_member->type)
      {
         case ISNS_DD_ISCSI_MEMBER:
            /* Fetch the node */
            memset (&db_node_name, 0, sizeof (db_node_name));
            strncpy ((char *) db_node_name.v, p_member->node_id, sizeof(db_node_name.v) );

            rval = read_ISCSIObject(db_node_name.v, &p_node, &lentry);
            if (rval != SUCCESS)
               return rval;

            old_activeFlag = p_node->activeFlag;
            
            if ( p_dd->activeFlag )
            {
               p_node->activeFlag = TRUE;
            }
            else
            {
               lnode2 = NULL;
               activeFlag = FALSE;
               while ( (lnode2 = GetNextNode(&p_node->dd_id_list, lnode2)) )
               {
                  if (IsDDActive( *(uint32_t *)GetNodeData( lnode2 ), 0 ))
                  {
                     activeFlag = TRUE;
                     break;
                  }
               }
               p_node->activeFlag = activeFlag;
            }

            if (old_activeFlag != p_node->activeFlag)
            {
               write_ISCSIObject(db_node_name.v, lentry);
            }

            break;
         case ISNS_DD_IFCP_MEMBER:
            __LOG_ERROR("change_dd_member_active_flag(): IFCP not implemented.");
            break;
         default:
            __LOG_ERROR("change_dd_member_active_flag(): Unknown type = %u.", p_member->type);
            break;
      }
   }

   return (SUCCESS);
}

/*********************************************************************
*********************************************************************/
int
ISNS_GetDD_ID_from_sym ( char *p_sym )
{
   SOIP_DB_Entry  lentry;
   SOIP_Dd        *p_dd;
   ISNS_DBKey     qkey = { 0 };
   int            rval;

   if ( p_sym )
   {
      memset(&qkey, 0, sizeof(qkey));
      qkey.tag = DD_ID_KEY;

      while (SNSdbGetNextOfKey (&qkey) == SUCCESS)
      {
         /* If a symbolic name is given, 
            search for any entries that
            has that dd, return that DD */
         rval = read_DDObject(qkey.val.dd_key.id, &p_dd, &lentry);
         if (rval != SUCCESS)
            return 0;

         if (0 == strcmp(p_dd->sym_name, p_sym))
            return ( p_dd->id );

      }
   }

   return (0);
}

/*********************************************************************
_Copy_DD_List:
This will copy the DD_List to static array.
*********************************************************************/
int
Copy_DD_List(ISNS_LIST *dd_list, ISNS_DD_LIST p_dlist)
{
   ISNS_LIST_NODE *pnode;
   int index;

   index=0;
   pnode=NULL;
   while ((pnode=GetNextNode(dd_list, pnode)))
   {
      p_dlist[index]=*(uint32_t *)GetNodeData(pnode);
      index++;
   }

   return (SUCCESS);
}

/*********************************************************************
_ISNS_GetDDS_ID_from_sym

Gets a new unique DDS ID.
*********************************************************************/
int
ISNS_GetDDS_ID_from_sym (char *p_sym)
{
   SOIP_DB_Entry lentry;
   SOIP_Dds      *p_dds;
   ISNS_DBKey    qkey = { 0 };
   int           rval;

   if ( p_sym )
   {
      memset(&qkey, 0, sizeof(qkey));
      qkey.tag = DDS_ID_KEY;

      while (SNSdbGetNextOfKey (&qkey) == SUCCESS)
      {
         rval = read_DDSObject(qkey.val.dds_key.id, &p_dds, &lentry);
         if ( rval != SUCCESS)
            return (0);

         if (0 == strcmp (p_dds->sym_name, p_sym))
            return (p_dds->id);
      }
   }

   return ( 0 );
}

/*********************************************************************
_ISNS_GetNewDDS_ID

Gets a new unique DDS ID.
*********************************************************************/
int
ISNS_GetNewDDS_ID (void)
{
   static int currentDDS_ID = 0;
   SOIP_DB_Entry lentry;
   SOIP_Dds *p_dds;
   int rval;

   do
   {
      currentDDS_ID++;
      rval = read_DDSObject(currentDDS_ID, &p_dds, &lentry);
   } while ( rval == SUCCESS);

   return (currentDDS_ID);
}

/*********************************************************************
Appends a DD to a DD list.  Checks to make sure there aren't duplicate
entries.
*********************************************************************/
int
Append_DD_to_List (ISNS_DD_LIST p_dlist, uint32_t id)
{
   int ii;

   __DEBUG (isns_reg_debug &1,(Append_DD_to_List id=%i),id);

   for (ii = 0; ii < MAX_DD_PER_LIST; ii++)
   {
      if (p_dlist[ii] == 0)
      {
         p_dlist[ii] = id;
         return (ISNS_NO_ERR);
      }
      else if (p_dlist[ii] == id)
      {
         return (ISNS_NO_ERR);
      }
   }

   return (ERROR);
}
/*********************************************************************
Appends a DD member to a list.
*********************************************************************/
int
append_to_member_list (SOIP_Dd_Member *p_ddmem, char *p_nodename,
                       int type)
{
   int ii;
   int freeSlot = -1;

   for (ii = 0; ii < MAX_MEMBER_PER_DD; ii++)
   {
      if (p_ddmem[ii].type==0)
      {
         freeSlot = ii;
         break;
      }

      if (memcmp (p_ddmem[ii].node_id, p_nodename,
                  p_ddmem[ii].type ==
                  ISNS_DD_IFCP_MEMBER ? 8 : PAD4 (strlen (p_nodename))) ==
          0)
      {
         /* Duplicate */
         return (ISNS_NO_ERR);
      }
   }

   if (freeSlot == -1)
   {
      __LOG_ERROR ("***ERROR: No free spots found in list.");
      return (ERROR);
   }
   else
   {
      /* Add Node to list */
      memset (p_ddmem[freeSlot].node_id, 0,
              sizeof (p_ddmem[freeSlot].node_id));
      p_ddmem[freeSlot].type = type;
      memcpy (p_ddmem[freeSlot].node_id, p_nodename,
              p_ddmem[freeSlot].type ==
              ISNS_DD_IFCP_MEMBER ? 8 : PAD4 (strlen (p_nodename)));
   }

   return (ISNS_NO_ERR);
}

/*********************************************************************
Appends the active nodes from a DD List.
*********************************************************************/
int
Append_Nodes_From_DD_List (ISNS_DD_LIST p_dlist,
                           SOIP_Dd_Member p_ddmem[], uint32_t type)
{
   int ii;
   SOIP_Dd *p_dd;
   SOIP_DB_Entry lentry;
   ISNS_LIST_NODE *pnode;
   int rval;

   __DEBUG (isns_reg_debug &1,(Append_Nodes_From_DD_List));

   for (ii = 0; ii < MAX_DD_PER_LIST && p_dlist[ii]!=0; ii++)
   {
      __DEBUG (isns_reg_debug &1,(Append Node - dd:%i),p_dlist[ii]);
      rval = read_DDObject(p_dlist[ii], &p_dd, &lentry);
      if (rval != SUCCESS)
        return rval;

      pnode=NULL;
      while ((pnode=GetNextNode(&p_dd->member_list, pnode)))
      {
         SOIP_Dd_Member *p_member;

         p_member=(SOIP_Dd_Member *)GetNodeData(pnode);
         if (p_member->status == ISNS_DD_MEMBER_ENABLE &&
             (type == 0 || p_member->type == type))
         {
            __DEBUG (isns_reg_debug &1,(Append member to list));
            append_to_member_list (p_ddmem, p_member->node_id,
                                   p_member->type);
         }
      }
   }

   return (ISNS_NO_ERR);
}

/*********************************************************************
*********************************************************************/
int
Create_Default_DD ()
{
   SOIP_DB_Entry     lentry1;
   SOIP_DB_Entry     lentry2;
   SOIP_Dd           *p_dd = NULL;
   SOIP_Dds          *p_dds = NULL;
   ISNS_DBKey        dds_key = { 0 };
   ISNS_DBKey        dd_key = { 0 };
   int               rval;
   int               found;

   __DEBUG (isns_reg_debug &1,Create Default Domains and Domain Sets);

   /* check if default DDS exists, if not create it */
   dds_key.tag = DDS_ID_KEY;

   found = FALSE;
   while ( SNSdbGetNextOfKey ( &dds_key ) == SUCCESS )
   {
      rval = read_DDSObject(dds_key.val.dds_key.id, &p_dds, &lentry1);
      if (rval != SUCCESS)
         return rval;
      if ( !strcmp("default DDS",p_dds->sym_name) )
      {
        found = TRUE;
      }
   }

   if (!found)
   {
       __DEBUG (isns_reg_debug &1, Create Default DDS);
       dds_key.val.dds_key.id = ISNS_GetNewDDS_ID ();
       p_dds = (SOIP_Dds *)&lentry1.data;
       p_dds->id = dds_key.val.dds_key.id;
       strcpy(p_dds->sym_name,"default DDS");
       InitList(DDS_DD_LIST, p_dds);
       p_dds->status = 1;
       rval = write_DDSObject(dds_key.val.dds_key.id, lentry1);
       __DEBUG (isns_reg_debug &1,Create Default DDS rval:%i,rval);
   }

   /* check if default DD exists, if not create it */
   dd_key.tag = DD_ID_KEY;
   found = FALSE;
   while ( SNSdbGetNextOfKey ( &dd_key ) == SUCCESS )
   {
      rval = read_DDObject(dd_key.val.dd_key.id, &p_dd, &lentry2);
      if (rval != SUCCESS)
         return rval;
      if ( !strcmp("default DD",p_dd->sym_name) )
      {
        found = TRUE;
      }
   }

   if (!found)
   {
       __DEBUG (isns_reg_debug &1, Create Default DD);
       p_dd = (SOIP_Dd *)&lentry2.data;
       dd_key.val.dd_key.id = ISNS_GetNewDD_ID();
       p_dd->id = dd_key.val.dd_key.id;
       strcpy(p_dd->sym_name,"default DD");
       InitList(DD_MEMBER_LIST, p_dd);
       InitList(DD_DDS_LIST, p_dd);
       p_dd->activeFlag = p_dds->status;

       rval = write_DDObject(dd_key.val.dd_key.id, lentry2);
       __DEBUG (isns_reg_debug &1,Create Default DD rval:%i,rval);

       /* Add DD to DDS */
       Add_dds_to_dd (p_dds,p_dd->id);
       rval = write_DDSObject(dds_key.val.dds_key.id, lentry1);
   }

   return SUCCESS;
}

static int Add_ISCSINode_Default_DD (SOIP_Iscsi *p_node)
{
   SOIP_DB_Entry     lentry2;
   SOIP_Dd           *p_dd = NULL;
   ISNS_DBKey        dd_key = { 0 };
   int               rval;
   int               found;

   rval = 0;
   dd_key.tag = DD_ID_KEY;
   found = FALSE;
   while ( SNSdbGetNextOfKey ( &dd_key ) == SUCCESS )
   {
      rval = read_DDObject(dd_key.val.dd_key.id, &p_dd, &lentry2);
      if (rval != SUCCESS)
         return rval;

      if ( !strcmp("default DD",p_dd->sym_name) )
      {
        found = TRUE;
        break;
      }
   }

   if (found)
   {
      if (!FindNode(&p_node->dd_id_list, (char *)&p_dd->id, sizeof(p_dd->id)))
      {
        AddNode(&p_node->dd_id_list, (char *)&p_dd->id, sizeof(p_dd->id));
      }

      if ( p_dd->activeFlag )
         p_node->activeFlag = TRUE;

      Add_DD_Member(p_dd,p_node->id.v,PAD4(strlen(p_node->id.v)),ISNS_DD_ISCSI_MEMBER,ISNS_DD_MEMBER_ENABLE,p_node->iscsi_index);

      rval = write_DDObject(dd_key.val.dd_key.id, lentry2);
   }
   return SUCCESS;
}

/*********************************************************************
*********************************************************************/
int
Check_Authorization (ISNS_Attr *src_attr)
{
  __DEBUG (isns_reg_debug &1,Check_Authorization:%s,src_attr->val.node_name.v);

  if ( 0 == strcmp (src_attr->val.node_name.v,isns_control_node) )
      return SUCCESS;
  else
      return ISNS_AUTH_FAILED_ERR;
}
