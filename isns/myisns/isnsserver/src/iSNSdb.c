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
#include "iSNSNdb.h"
#include "iSNSdb.h"
//#include <gdbm.h>
#include "iSNStypes.h"
#include "iSNSmsg.h"
#include "iSNSbuffer.h"
#include "iSNStbl.h"
#include "iSNSList.h"
#include "iSNSdebug.h"

//#define ndb_store_sns(a,b,c,d) ndb_store(dbfp,b,c,d)
/*
#define ndb_open(a,b,c,d) gdbm_open(a,b,c,d)
#define ndb_close(a) gdbm_close(a)
#define ndb_fetch(a,b) gdbm_fetch(dbfp[a],b)
#define ndb_fetch_sns(a,b,c) gdbm_fetch(dbfp[a],b); memcpy(c,d.dptr,d.dsize)
#define ndb_store_sns(a,b,c,d) gdbm_store(dbfp[a],b,c,d)
#define ndb_delete(a,b) gdbm_delete(dbfp[a],b)
#define ndb_nextkey(a,b) gdbm_nextkey (dbfp[a],b)
#define ndb_firstkey(a) gdbm_firstkey (dbfp[a])
*/

/* Global */
extern int isns_db_debug;

/*
 * SoIP service Database file descriptor
 */
//static NDB_FILE dbfp;
//static GDBM_FILE  dbfp[25];

/*
 * GDBM errno
 */
//extern ndb_error ndb_errno;
extern int ndbm_errno;

/*
 * static common variables for gdbm fetches
 * entry is for common registrations and queries
 * scn_entry is for state change notifications
 * opn_entry is for access control
 */

uint32_t *dlist_src; /*[MAX_DD_PER_LIST];*/
uint32_t *dlist_node; /*[MAX_DD_PER_LIST];*/
SOIP_Dd_Member *ddmem; /* [100]; */

ISNS_LIST scn_callback_list;

ISNS_Msg *p_scn_msg_buffer;
ISNS_Msg *p_scn_all_msg_buffer;
ISNS_Msg_Descp *p_scn_md;
ISNS_Msg_Descp *p_rspMd;

dbStats iSNS_stats;

/*********************************************************************
Read Entry from Database referenced by key
*********************************************************************/
int
ISNSdbRead (ISNS_DBKey *key, SOIP_DB_Entry *entry)
{
   datum k;
   datum d;

   k.dsize = 0;
   d.dsize = sizeof (SOIP_DB_Entry);

   switch (key->tag)
   {
     case DDS_ID_KEY:
       __DEBUG (isns_db_debug & 1,(read DDS %i),key->val.dds_key.id);
       k.dptr = (char *)&key->val.dds_key.id;
       k.dsize = DDS_KEY_SIZE;
       d = ndb_fetch_sns (DDS_ID_KEY, k, (char *)entry);
       if (d.dptr == NULL)
       {
         __DEBUG (isns_db_debug & 1, (read failed DDS %u),key->val.dds_key.id);
         return (ISNS_NO_SUCH_ENTRY_ERR);
       }
       if (entry->data_type != DDS_ID_KEY)
          return (ISNS_UNKNOWN_ERR);
       entry->data.dds.dd_list.p_entry = &entry->data.dds;
       break;
     case DD_ID_KEY:
       __DEBUG (isns_db_debug & 1,(read DD %i),key->val.dd_key.id);
       k.dptr = (char *)&key->val.dd_key.id;
       k.dsize = DD_KEY_SIZE;
       d = ndb_fetch_sns (DD_ID_KEY, k, (char *)entry);
       if (d.dptr == NULL)
       {
         __DEBUG (isns_db_debug & 1, (read failed DD %u),key->val.dd_key.id);
         return (ISNS_NO_SUCH_ENTRY_ERR);
       }
       if (entry->data_type != DD_ID_KEY)
          return (ISNS_UNKNOWN_ERR);

       entry->data.dd.member_list.p_entry = &entry->data.dd;
       entry->data.dd.dds_list.p_entry = &entry->data.dd;
       __DEBUG (isns_db_debug &1,read dd - dds list_id:%i,entry->data.dd.dds_list.list_id);
       break;
     case ISCSI_ID_KEY:
       __DEBUG (isns_db_debug & 1,(read ISCSI node %s),key->val.node_key.v);
       k.dptr = (char *)key->val.node_key.v;
       k.dsize = strlen(key->val.node_key.v);
       d = ndb_fetch_sns (ISCSI_ID_KEY, k, (char *)entry);
       if (d.dptr == NULL)
       {
         __DEBUG (isns_db_debug & 1, (read failed ISCSI node %s),key->val.node_key.v);
         return (ISNS_NO_SUCH_ENTRY_ERR);
       }
       if (entry->data_type != ISCSI_ID_KEY)
          return (ISNS_UNKNOWN_ERR);
       entry->data.scsi_node.dd_id_list.p_entry = &entry->data.scsi_node;
       break;
     case ENTITY_ID_KEY:
       __DEBUG (isns_db_debug & 1,(read ENTITY node %s),key->val.entity_key.id);
       k.dptr = (char *)key->val.entity_key.id;
       k.dsize = strlen(key->val.entity_key.id);
       d = ndb_fetch_sns (ENTITY_ID_KEY, k, (char *)entry);
       if (d.dptr == NULL)
       {
         __DEBUG (isns_db_debug & 1, (read failed ENTITY node %s),key->val.entity_key.id);
         return (ISNS_NO_SUCH_ENTRY_ERR);
       }
       if (entry->data_type != ENTITY_ID_KEY)
       {
          __DEBUG (isns_db_debug & 1, (read failed ENTITY node - invalid data in database %s),key->val.entity_key.id);
          return (ISNS_UNKNOWN_ERR);
       }
       entry->data.entity.iportal_list.p_entry = &entry->data.entity;
       entry->data.entity.iscsi_node_list.p_entry = &entry->data.entity;
       entry->data.entity.ifcp_node_list.p_entry = &entry->data.entity;
       break;
     case PORT_NAME_KEY:
       k.dptr = (char *)key->val.port_key.v;
       k.dsize = strlen(key->val.port_key.v);
       d = ndb_fetch_sns (PORT_NAME_KEY, k, (char *)entry);
       if (d.dptr == NULL)
       {
         __DEBUG (isns_db_debug & 1, (read failed PORT_NAME %s),key->val.port_key.v);
         return (ISNS_NO_SUCH_ENTRY_ERR);
       }
       if (entry->data_type != PORT_NAME_KEY)
          return (ISNS_UNKNOWN_ERR);
       entry->data.port.dd_id_list.p_entry = &entry->data.port;
       break;
     case NODE_NAME_KEY:
       k.dptr = (char *)key->val.node_key.v;
       k.dsize = strlen(key->val.node_key.v);
       d = ndb_fetch_sns (NODE_NAME_KEY, k, (char *)entry);
       if (d.dptr == NULL)
       {
         __DEBUG (isns_db_debug & 1, (read failed Node Name %s),key->val.node_key.v);
         return (ISNS_NO_SUCH_ENTRY_ERR);
       }
       if (entry->data_type != NODE_NAME_KEY)
          return (ISNS_UNKNOWN_ERR);
       entry->data.node.port_list.p_entry = &entry->data.node;
       break;
     case PORTAL_ID_KEY:
     {
       SOIP_Portal_Key db_portal;
       __ISNS_COPY (&db_portal, sizeof(db_portal), &key->val, sizeof(SOIP_Portal_Key));
       __DEBUG (isns_db_debug &1,read Portal ip_addr:%s port:%i,
        inet_ntoa(*(struct in_addr *)(db_portal.ip_addr.v+12)),db_portal.ip_port);
       k.dptr = (char *)&key->val;
       k.dsize = sizeof(SOIP_Portal_Key);
       d = ndb_fetch_sns (PORTAL_ID_KEY, k, (char *)entry);
       if (d.dptr == NULL)
       {
          __DEBUG (isns_db_debug &1,read failure Portal ip_addr:%s port:%i,
        inet_ntoa(*(struct in_addr *)(db_portal.ip_addr.v+12)),db_portal.ip_port);
         return (ISNS_NO_SUCH_ENTRY_ERR);
       }
       if (entry->data_type != PORTAL_ID_KEY)
          return (ISNS_UNKNOWN_ERR);
       break;
     }
     case PORTAL_GROUP_ID_KEY:
     {
       SOIP_Portal_Group_Key db_portal;
       __ISNS_COPY (&db_portal, sizeof(db_portal), &key->val, sizeof(SOIP_Portal_Group_Key));
       __DEBUG (isns_db_debug &1,read Portal Group name:%s ip_addr:%s port:%i,
                db_portal.id.v,
        inet_ntoa(*(struct in_addr *)(db_portal.ip_addr.v+12)),
                db_portal.ip_port);

       k.dptr = (char *)&key->val;
       k.dsize = sizeof(SOIP_Portal_Group_Key);
       d = ndb_fetch_sns (PORTAL_GROUP_ID_KEY, k, (char *)entry);
       if (d.dptr == NULL)
       {
         __DEBUG (isns_db_debug &1,read Portal Group name:%s ip_addr:%s port:%i,
                db_portal.id.v,
        inet_ntoa(*(struct in_addr *)(db_portal.ip_addr.v+12)),
                db_portal.ip_port);
         return (ISNS_NO_SUCH_ENTRY_ERR);
       }
       if (entry->data_type != PORTAL_GROUP_ID_KEY)
          return (ISNS_UNKNOWN_ERR);
       break;
     }
     case ENTITY_IDX_KEY:
     case ISCSI_IDX_KEY:
     case PORTAL_IDX_KEY:
       k.dptr = (char *)&key->val;
       k.dsize = sizeof(SOIP_IDX_Key);
       d = ndb_fetch_sns (key->tag, k, (char *)entry);
       if (d.dptr == NULL)
       {
         __DEBUG (isns_db_debug & 1, (read failed index:%i),*(int *)&key->val);
         return (ISNS_NO_SUCH_ENTRY_ERR);
       }
       if (entry->data_type != key->tag)
          return (ISNS_UNKNOWN_ERR);
       break;
     case LIST_KEY:
       __DEBUG (isns_db_debug & 1,(read listid:%i index:%i),key->val.list.list_id,key->val.list.list_index);
       k.dptr = (char *)&key->val;
       switch (key->val.list.list_id)
       {
         case ENTITY_ISCSI_LIST:
          __DEBUG (isns_db_debug & 1, (read list:%s),(char *)&key->val.list.key.entity_id.id);
          k.dsize = LIST_KEY_SIZE + strlen(key->val.list.key.entity_id.id);
          break;
         case ENTITY_FCP_LIST:
          __DEBUG (isns_db_debug & 1, (read list:%s),(char *)&key->val.list.key.entity_id.id);
          k.dsize = LIST_KEY_SIZE + strlen(key->val.list.key.entity_id.id);
          break;
         case ENTITY_PORTAL_LIST:
          __DEBUG (isns_db_debug & 1, (read list:%s),(char *)&key->val.list.key.entity_id.id);
          k.dsize = LIST_KEY_SIZE + strlen(key->val.list.key.entity_id.id);
          break;
         case PORTAL_ENTITY_LIST:
          __DEBUG (isns_db_debug & 1, (read list:%i),key->val.list.key.portal.ip_port);
          k.dsize = LIST_KEY_SIZE + sizeof(SOIP_Portal_Key);
          break;
         case ISCSI_DD_LIST:
          k.dsize = LIST_KEY_SIZE + strlen(key->val.list.key.node_name.v);
          break;
         case FCP_PORTAL_LIST:
          k.dsize = LIST_KEY_SIZE + strlen(key->val.list.key.node_name.v);
          break;
         case FCP_DD_LIST:
          k.dsize = LIST_KEY_SIZE + strlen(key->val.list.key.node_name.v);
          break;
         case DD_MEMBER_LIST:
         case DD_DDS_LIST:
         case DDS_DD_LIST:
          k.dsize = 4 + LIST_KEY_SIZE;
          break;
         case SCN_LIST:
         case SCN_CALLBACK_LIST:
          k.dsize = LIST_KEY_SIZE + strlen(key->val.list.key.node_name.v);
          break;
       }
       d = ndb_fetch_sns (key->tag, k, (char *)entry);
       if (d.dptr == NULL)
       {
         __DEBUG (isns_db_debug & 1, (read failed list:%s),(char *)&key->val.list.key.entity_id.id);
         return (ISNS_NO_SUCH_ENTRY_ERR);
       }
       if (entry->data_type != key->tag)
          return (ISNS_UNKNOWN_ERR);
       break;
     default:
          __LOG_ERROR ("Undefined read database key key->tag:%i",key->tag);
          return ERROR;
       break;
   }

   if (entry->data_type != key->tag)
   {
          __LOG_ERROR ("Invalid record type:%i read from database key->tag:%i",entry->data_type,key->tag);
          return (ISNS_UNKNOWN_ERR);
   }

   return SUCCESS;

}

/*********************************************************************
Write Entry in Database referenced by key
*********************************************************************/
int
ISNSdbWrite (ISNS_DBKey *key, SOIP_DB_Entry entry)
{
   datum k;
   datum d;
   int rc;

   d.dptr = (char *) &entry;
   d.dsize = sizeof (SOIP_DB_Entry);
   k.dsize = 0;

   entry.data_type = key->tag;

   switch (key->tag)
   {
     case DDS_ID_KEY:
       __DEBUG (isns_db_debug &1,(Write DDS Entry:%i),key->val.dds_key.id);
       k.dptr = (char *)&key->val.dds_key.id;
       k.dsize = DDS_KEY_SIZE;
       rc = ndb_store_sns (key->tag, k, d, NDBM_REPLACE);
       if (rc == ERROR)
         __DEBUG (isns_db_debug &1,DDS Write failed);
       break;
     case DD_ID_KEY:
       __DEBUG (isns_db_debug &1,(Write DD Entry:%i),key->val.dd_key.id);
       __DEBUG (isns_db_debug &1,write dd - dds list_id:%i,entry.data.dd.dds_list.list_id);
       k.dptr = (char *)&key->val.dd_key.id;
       k.dsize = DD_KEY_SIZE;
       rc = ndb_store_sns (key->tag, k, d, NDBM_REPLACE);
       if (rc == ERROR)
         __DEBUG (isns_db_debug &1,DD Write failed);
       break;
     case ISCSI_ID_KEY:
       __DEBUG (isns_db_debug &1,ISCSI write:%s,key->val.node_key.v);
       k.dptr = (char *)key->val.node_key.v;
       k.dsize = strlen(key->val.node_key.v);
       rc = ndb_store_sns (key->tag, k, d, NDBM_REPLACE);
       if (rc == ERROR)
         __DEBUG (isns_db_debug &1,ISCSI Write failed);
       break;
     case ENTITY_ID_KEY:
     {
       SOIP_Entity * p_entity;
       p_entity = &entry.data.entity;
       __DEBUG (isns_db_debug &1,Entity write:%s,key->val.entity_key.id);
       __DEBUG (isns_db_debug &1,Entity write:%s,p_entity->eid.id);
       k.dptr = (char *)key->val.entity_key.id;
       k.dsize = strlen(key->val.entity_key.id);
       rc = ndb_store_sns (key->tag, k, d, NDBM_REPLACE);
       if (rc == ERROR)
         __DEBUG (isns_db_debug &1,ENTITY Write failed errno:%i,ndbm_errno);
       //ISNSDisplay_Entity (&entry.data.entity,HI_DETAIL);
       break;
     }
     case PORT_NAME_KEY:
       k.dptr = (char *)key->val.port_key.v;
       k.dsize = strlen(key->val.port_key.v);
       rc = ndb_store_sns (key->tag, k, d, NDBM_REPLACE);
       if (rc == ERROR)
         __DEBUG (isns_db_debug &1,PORT_NAME Write failed);
       break;
     case NODE_NAME_KEY:
       k.dptr = (char *)key->val.node_key.v;
       k.dsize = strlen(key->val.node_key.v);
       rc = ndb_store_sns (key->tag, k, d, NDBM_REPLACE);
       if (rc == ERROR)
         __DEBUG (isns_db_debug &1,FC NODE_NAME Write failed);
       break;
     case PORTAL_ID_KEY:
     {
       SOIP_Portal_Key db_portal;
       __ISNS_COPY (&db_portal, sizeof(db_portal), &key->val, sizeof(SOIP_Portal_Key));
       __DEBUG (isns_db_debug &1,write Portal ip_addr:%s port:%i,
        inet_ntoa(*(struct in_addr *)(db_portal.ip_addr.v+12)),db_portal.ip_port);
       k.dptr = (char *)&key->val;
       k.dsize = sizeof(SOIP_Portal_Key);
       rc = ndb_store_sns (key->tag, k, d, NDBM_REPLACE);
       if (rc == ERROR)
         __DEBUG (isns_db_debug &1,PORTAL Name Write failed);
       break;
     }
     case PORTAL_GROUP_ID_KEY:
       __DEBUG (isns_db_debug &1,PORTAL Group Write);
       k.dptr = (char *)&key->val;
       k.dsize = sizeof(SOIP_Portal_Group_Key);
       rc = ndb_store_sns (key->tag, k, d, NDBM_REPLACE);
       if (rc == ERROR)
         __DEBUG (isns_db_debug &1,PORTAL Group Write failed);
       break;
     case ENTITY_IDX_KEY:
     case ISCSI_IDX_KEY:
     case PORTAL_IDX_KEY:
       k.dptr = (char *)&key->val;
       k.dsize = sizeof(SOIP_IDX_Key);
       rc = ndb_store_sns (key->tag, k, d, NDBM_REPLACE);
       if (rc == ERROR)
         __DEBUG (isns_db_debug &1,Index Write failed);
       break;
     case LIST_KEY:
       k.dptr = (char *)&key->val;
       __DEBUG (isns_db_debug & 1,(write listid:%i index:%i),key->val.list.list_id,key->val.list.list_index);
       k.dptr = (char *)&key->val;
       switch (key->val.list.list_id)
       {
         case ENTITY_ISCSI_LIST:
          __DEBUG (isns_db_debug & 1, (write list:%s),(char *)&key->val.list.key.entity_id.id);
          k.dsize = LIST_KEY_SIZE + strlen(key->val.list.key.entity_id.id);
          break;
         case ENTITY_FCP_LIST:
          __DEBUG (isns_db_debug & 1, (write list:%s),(char *)&key->val.list.key.entity_id.id);
          k.dsize = LIST_KEY_SIZE + strlen(key->val.list.key.entity_id.id);
          break;
         case ENTITY_PORTAL_LIST:
          __DEBUG (isns_db_debug & 1, (write list:%s),(char *)&key->val.list.key.entity_id.id);
          k.dsize = LIST_KEY_SIZE + strlen(key->val.list.key.entity_id.id);
          break;
         case PORTAL_ENTITY_LIST:
          __DEBUG (isns_db_debug & 1, (write list:%i),key->val.list.key.portal.ip_port);
          k.dsize = LIST_KEY_SIZE + sizeof(SOIP_Portal_Key);
          break;
         case ISCSI_DD_LIST:
          k.dsize = LIST_KEY_SIZE + strlen(key->val.list.key.node_name.v);
          break;
         case FCP_PORTAL_LIST:
          k.dsize = LIST_KEY_SIZE + strlen(key->val.list.key.node_name.v);
          break;
         case FCP_DD_LIST:
          k.dsize = LIST_KEY_SIZE + strlen(key->val.list.key.node_name.v);
          break;
         case DD_MEMBER_LIST:
         case DD_DDS_LIST:
         case DDS_DD_LIST:
          k.dsize = 4 + LIST_KEY_SIZE;
          break;
         case SCN_LIST:
         case SCN_CALLBACK_LIST:
          k.dsize = LIST_KEY_SIZE + strlen(key->val.list.key.node_name.v);
          break;
       }
       rc = ndb_store_sns (key->tag, k, d, NDBM_REPLACE);
       if (rc == ERROR)
         __DEBUG (isns_db_debug &1,List Write failed);
       break;
     default:
         __LOG_ERROR ("When writing -- Undefined database key->tag:%i",key->tag);
       break;
   }
   return SUCCESS;

}

/*********************************************************************
Delete Entry in Database referenced by key
*********************************************************************/
int
ISNSdbDelete (ISNS_DBKey *key)
{
   datum k;
   int rc;

   rc = SUCCESS;

   k.dsize = 0;

   switch (key->tag)
   {
     case DDS_ID_KEY:
       __DEBUG (isns_db_debug &1,Remove DDS Entry:%i,key->val.dds_key.id);
       k.dptr = (char *)&key->val.dds_key.id;
       k.dsize = DDS_KEY_SIZE;
       rc =  ndb_delete (key->tag, k);
       if (rc == ERROR)
         __DEBUG (isns_db_debug &1,DDS Delete failed);
       break;
     case DD_ID_KEY:
       __DEBUG (isns_db_debug &1,Remove DD Entry:%i,key->val.dd_key.id);
       k.dptr = (char *)&key->val.dd_key.id;
       k.dsize = DD_KEY_SIZE;
       rc =  ndb_delete (key->tag, k);
       if (rc == ERROR)
         __DEBUG (isns_db_debug &1,DD Delete failed);
       break;
     case ISCSI_ID_KEY:
       k.dptr = (char *)key->val.node_key.v;
       k.dsize = strlen(key->val.node_key.v);
       rc =  ndb_delete (key->tag, k);
       if (rc == ERROR)
         __DEBUG (isns_db_debug &1,ISCSI Delete failed);
       break;
     case ENTITY_ID_KEY:
       __DEBUG (isns_db_debug &1,Delete ENTITY:%s,key->val.entity_key.id);
       k.dptr = (char *)key->val.entity_key.id;
       k.dsize = strlen(key->val.entity_key.id);
       rc =  ndb_delete (key->tag, k);
       if (rc == ERROR)
         __DEBUG (isns_db_debug &1,ENTITY Delete failed);
       break;
     case PORT_NAME_KEY:
       k.dptr = (char *)key->val.port_key.v;
       k.dsize = strlen(key->val.port_key.v);
       rc =  ndb_delete (key->tag, k);
       if (rc == ERROR)
         __DEBUG (isns_db_debug &1,PORT_NAME Delete failed);
       break;
     case NODE_NAME_KEY:
       k.dptr = (char *)key->val.node_key.v;
       k.dsize = strlen(key->val.node_key.v);
       rc =  ndb_delete (key->tag, k);
       if (rc == ERROR)
         __DEBUG (isns_db_debug &1,FC NODE_NAME Delete failed);
       break;
     case PORTAL_ID_KEY:
       k.dptr = (char *)&key->val;
       k.dsize = sizeof(SOIP_Portal_Key);
       rc =  ndb_delete (key->tag, k);
       if (rc == ERROR)
         __DEBUG (isns_db_debug &1,PORTAL Name Delete failed);
       break;
     case PORTAL_GROUP_ID_KEY:
       __DEBUG (isns_db_debug &1,PORTAL Group Delete);
       k.dptr = (char *)&key->val;
       k.dsize = sizeof(SOIP_Portal_Group_Key);
       rc =  ndb_delete (key->tag, k);
       if (rc == ERROR)
         __DEBUG (isns_db_debug &1,PORTAL Group Delete failed);
       break;
     case ENTITY_IDX_KEY:
     case ISCSI_IDX_KEY:
     case PORTAL_IDX_KEY:
       k.dptr = (char *)&key->val;
       k.dsize = sizeof(SOIP_IDX_Key);
       rc =  ndb_delete (key->tag, k);
       if (rc == ERROR)
         __DEBUG (isns_db_debug &1, Index Delete failed);
       break;
     case LIST_KEY:
       k.dptr = (char *)&key->val;
       switch (key->val.list.list_id)
       {
         case ENTITY_ISCSI_LIST:
          __DEBUG (isns_db_debug & 1, (write list:%s),(char *)&key->val.list.key.entity_id.id);
          k.dsize = LIST_KEY_SIZE + strlen(key->val.list.key.entity_id.id);
          break;
         case ENTITY_FCP_LIST:
          __DEBUG (isns_db_debug & 1, (write list:%s),(char *)&key->val.list.key.entity_id.id);
          k.dsize = LIST_KEY_SIZE + strlen(key->val.list.key.entity_id.id);
          break;
         case ENTITY_PORTAL_LIST:
          __DEBUG (isns_db_debug & 1, (write list:%s),(char *)&key->val.list.key.entity_id.id);
          k.dsize = LIST_KEY_SIZE + strlen(key->val.list.key.entity_id.id);
          break;
         case PORTAL_ENTITY_LIST:
          k.dsize = LIST_KEY_SIZE + sizeof(SOIP_Portal_Key);
          break;
         case ISCSI_DD_LIST:
          k.dsize = LIST_KEY_SIZE + strlen(key->val.list.key.node_name.v);
          break;
         case FCP_PORTAL_LIST:
          k.dsize = LIST_KEY_SIZE + strlen(key->val.list.key.node_name.v);
          break;
         case FCP_DD_LIST:
          k.dsize = LIST_KEY_SIZE + strlen(key->val.list.key.node_name.v);
          break;
         case DD_MEMBER_LIST:
         case DD_DDS_LIST:
         case DDS_DD_LIST:
          k.dsize = 4 + LIST_KEY_SIZE;
          break;
         case SCN_LIST:
         case SCN_CALLBACK_LIST:
          k.dsize = LIST_KEY_SIZE + strlen(key->val.list.key.node_name.v);
          break;
       }
       rc =  ndb_delete (key->tag, k);
       if (rc == ERROR)
         __DEBUG (isns_db_debug &1, List Delete failed);
       break;
     default:
       __LOG_ERROR("Undefined Delete key->tag:%i",key->tag);
       break;
   }
   return SUCCESS;
}

/*********************************************************************
Returns a new entity Index.
*********************************************************************/
uint32_t
ISNSGetNewEntityIdx( void )
{
   static uint32_t indx = 0;
   ISNS_DBKey key;
   int index = 0;

   key.tag = ENTITY_IDX_KEY;
   key.len = 0;

   if (indx == 0)
   {
     while (SNSdbGetNextOfKey (&key) == SUCCESS)
     {
      index = key.val.idx.index;
      if ( index > indx )
         indx = index;
    }
   }

   indx++;

   return ( indx );
}

/*********************************************************************
Returns a new iSCSI Index.
*********************************************************************/
uint32_t
ISNSGetNewISCSIIdx( void )
{
   static uint32_t indx = 0;
   ISNS_DBKey key;
   int index = 0;

   key.tag = ISCSI_IDX_KEY;
   key.len = 0;

   if (indx == 0)
   {
     while (SNSdbGetNextOfKey (&key) == SUCCESS)
     {
      index = key.val.idx.index;
      if ( index > indx )
         indx = index;
    }
   }

   indx++;

   return ( indx );
}

/*********************************************************************
Returns a new Portal Index.
*********************************************************************/
uint32_t
ISNSGetNewPortalIdx( void )
{
   static uint32_t indx = 0;
   ISNS_DBKey key;
   int index = 0;

   key.tag = PORTAL_IDX_KEY;
   key.len = 0;

   if (indx == 0)
   {
     while (SNSdbGetNextOfKey (&key) == SUCCESS)
     {
      index = key.val.idx.index;
      if ( index > indx )
         indx = index;
    }
   }

   indx++;

   return ( indx );
}

/*********************************************************************
Database initialization.
*********************************************************************/
int
ISNSdbOpen( void )
{

    /*
     * Initialize database envirnoment and resources
     */
    if(NDB_SUCCESS != ndb_init(ISNS_LDAP_SERVER_URL,
                               ISNS_LDAP_ADMIN_DN,
                               ISNS_LDAP_ADMIN_PASS,
                               ISNS_LDAP_ISNS_BASE))
    {
        return ERROR;
    }

    ddmem=(SOIP_Dd_Member *)calloc( MAX_MEMBER_PER_DD, sizeof(SOIP_Dd_Member) );
    dlist_src=(uint32_t *)calloc(MAX_DD_PER_LIST, sizeof(uint32_t));
    dlist_node=(uint32_t *)calloc(MAX_DD_PER_LIST, sizeof(uint32_t));
    p_scn_msg_buffer=(ISNS_Msg *)calloc(1, sizeof(ISNS_Msg));
    p_scn_all_msg_buffer=(ISNS_Msg *)calloc(1, sizeof(ISNS_Msg));
    p_scn_md=(ISNS_Msg_Descp *)calloc(1, sizeof(ISNS_Msg_Descp));
    p_rspMd=(ISNS_Msg_Descp *)calloc(1, sizeof(ISNS_Msg_Descp));


    return (SUCCESS);
}

/*********************************************************************
Closes the Database.
*********************************************************************/
void
ISNSdbClose(void)
{
    ndb_close();
}


/*********************************************************************
Gets the next object after the passed in key.
*********************************************************************/
int
SNSdbGetNextOfKey (ISNS_DBKey * key)
{
   datum k, d, next;
   ISNS_DBKey null_key = { 0 };
   SOIP_DB_Entry *p_entry;

   if ( (key->len == 0) ||
        (key->len != 0 && memcmp (&key->val, &null_key.val, key->len) == 0))
   {
      k = ndb_firstkey (key->tag);
      if (k.dptr == NULL)
         return (ERROR);
   }
   else if (key->tag == NODE_NAME_KEY)
   {
      k.dptr = (char *) ISNSAllocBuffer (DB_NODE_NAME_SIZE);
      __ISNS_COPY (k.dptr, DB_NODE_NAME_SIZE, &key->val, key->len);
      k.dsize = DB_NODE_NAME_SIZE;
      next = ndb_nextkey (key->tag, k);
      ISNSFreeBuffer ((char *) k.dptr);
      if (next.dptr == NULL)
         return (ERROR);
      k = next;
   }
   else if (key->tag == PORT_NAME_KEY)
   {
      k.dptr = (char *) ISNSAllocBuffer (PORT_NAME_SIZE+1);
      __ISNS_COPY (k.dptr, PORT_NAME_SIZE+1, &key->val, key->len);
      k.dsize = PORT_NAME_SIZE;
      next = ndb_nextkey (key->tag, k);
      ISNSFreeBuffer ((char *) k.dptr);
      if (next.dptr == NULL)
         return (ERROR);
      k = next;
   }
   else if (key->tag == DDS_ID_KEY)
   {
      k.dptr = (char *) ISNSAllocBuffer (DDS_KEY_SIZE);
      __ISNS_COPY (k.dptr, DDS_KEY_SIZE, &key->val, key->len);
      k.dsize = DDS_KEY_SIZE;
      next = ndb_nextkey (key->tag, k);
      ISNSFreeBuffer ((char *) k.dptr);
      if (next.dptr == NULL)
         return (ERROR);
      k = next;
   }
   else if (key->tag == DD_ID_KEY)
   {
      k.dptr = (char *) ISNSAllocBuffer (DD_KEY_SIZE);
      __ISNS_COPY (k.dptr, DD_KEY_SIZE, &key->val, key->len);
      k.dsize = DD_KEY_SIZE;
      next = ndb_nextkey (key->tag, k);
      ISNSFreeBuffer ((char *) k.dptr);
      if (next.dptr == NULL)
         return (ERROR);
      k = next;
   }
   else if (key->tag == ISCSI_ID_KEY)
   {
      k.dptr = (char *) ISNSAllocBuffer (key->len +1 );
      memset (k.dptr,0,key->len+1);
      __ISNS_COPY (k.dptr, key->len+1, &key->val, key->len);
      k.dsize = strlen(k.dptr);
      next = ndb_nextkey (key->tag, k);
      ISNSFreeBuffer ((char *) k.dptr);
      if (next.dptr == NULL)
      {
         __DEBUG (isns_db_debug &1,(next is null));
         return (ERROR);
      }
      k = next;
   }
   else if (key->tag == ENTITY_ID_KEY)
   {
      k.dptr = (char *) ISNSAllocBuffer (key->len +1 );
      memset (k.dptr,0,key->len+1);
      __ISNS_COPY (k.dptr, key->len+1, &key->val, key->len);
      k.dsize = strlen(k.dptr);
      next = ndb_nextkey (key->tag, k);
      ISNSFreeBuffer ((char *) k.dptr);
      if (next.dptr == NULL)
      {
         return (ERROR);
      }
      k = next;
   }
   else
   {
      k.dptr = (char *) ISNSAllocBuffer (key->len +1 );
      memset (k.dptr,0,key->len+1);
      __ISNS_COPY (k.dptr, key->len+1, &key->val, key->len);
      k.dsize = key->len;
      next = ndb_nextkey (key->tag, k);
      ISNSFreeBuffer ((char *) k.dptr);
      if (next.dptr == NULL)
      {
         return (ERROR);
      }
      k = next;
   }

   while (k.dptr)
   {
      d = ndb_fetch (key->tag, k);

      /* check data type for match */
      p_entry = (SOIP_DB_Entry *) d.dptr;
      if ((p_entry) && ((int)p_entry->data_type == (int)key->tag))
      {
         /* copy new key */
         memset (&key->val,0,k.dsize+1);
         __ISNS_COPY (&key->val, sizeof(key->val), k.dptr, k.dsize);
         key->len = k.dsize;

         /* free buffers */
         free (k.dptr);
         free (d.dptr);

         return (ISNS_NO_ERR);
      }
      else
      {
         /* else, keep searching */
         next = ndb_nextkey (key->tag, k);
         if (d.dptr)
            free (d.dptr);
         free (k.dptr);
         k = next;
      }
   }

   /* not found, return ERROR */
   return (ERROR);
}

/**************************************************
**************************************************/
void ISNSInitDBTables(void)
{
   __DEBUG (isns_db_debug &1, ISNSInitDBTables);
   InitList(SCN_CALLBACK_LIST, &scn_callback_list);
}
