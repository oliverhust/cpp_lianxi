/***********************************************************************
  Copyright (c) 2007, Novell, Inc.
  All rights reserved.
  
  Redistribution and use in source and binary forms, with or without 
  modification, are permitted provided that the following conditions are 
  met:
  
  - Redistributions of source code must retain the above copyright notice, 
    this list of conditions and the following disclaimer. 
  
  - Redistributions in binary form must reproduce the above copyright 
    notice, this list of conditions and the following disclaimer in the 
    documentation and/or other materials provided with the distribution. 
  
  - Neither the name of the Novell, Inc nor the names of its 
    contributors may be used to endorse or promote products derived from 
    this software without specific prior written permission. 
  
  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
  IMPLIED WARRANTIES OF MERCHANTABILITY, NON-INFRINGEMENT AND FITNESS FOR A 
  PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL NOVELL, INC. 
  OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; 
  OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
  WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR 
  OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 
***********************************************************************/

/*
 * This file contains source code for managing read and writing of iSNS objects
 * to the database
 *
 */

#include "iSNS.h"
#include "iSNSdb.h"
#include "iSNSobjects.h"
#include "iSNStypes.h"
#include "iSNSmsg.h"
#include "iSNSdebug.h"

/* Global */
extern int isns_db_debug;

/*********************************************************************
read_FCNodeObject
*********************************************************************/
int
read_FCNodeObject (char *p_id, SOIP_Fc_Node **p_node, SOIP_DB_Entry *p_entry)
{
   ISNS_DBKey key;
   int rval;

   if ( (p_id == NULL ) || (strlen(p_id) <= 0) )
      return (ERROR);

   key.tag = NODE_NAME_KEY;
   strcpy(key.val.node_key.v, p_id);
   rval = ISNSdbRead(&key,p_entry);
   *p_node = (SOIP_Fc_Node *)&p_entry->data;
   return rval;
}

/*********************************************************************
read_FCPortObject
*********************************************************************/
int
read_FCPortObject (char *p_id, SOIP_Ifcp **p_node, SOIP_DB_Entry *p_entry)
{
   ISNS_DBKey key;
   int rval;

   if ( (p_id == NULL ) || (strlen(p_id) <= 0) )
      return (ERROR);

   key.tag = PORT_NAME_KEY;
   strcpy(key.val.port_key.v, p_id);
   rval = ISNSdbRead(&key,p_entry);
   *p_node = (SOIP_Ifcp *)&p_entry->data;
   return rval;
}

/*********************************************************************
read_ISCSIObject
*********************************************************************/
int
read_ISCSIObject (char *p_id, SOIP_Iscsi **p_node, SOIP_DB_Entry *p_entry)
{
   ISNS_DBKey key;
   int rval;

   if ( (p_id == NULL ) || (strlen(p_id) <= 0) )
      return (ERROR);

   key.tag = ISCSI_ID_KEY;
   strcpy(key.val.iscsi_key.v, p_id);
   rval = ISNSdbRead(&key,p_entry);
   *p_node = (SOIP_Iscsi *)&p_entry->data;
   return rval;
}

/*********************************************************************
read_EntityObject

Fetches an entity from the database

PARAMETERS:
1. p_entity_id - Entity ID.
2. p_entity    - Address where to place results
2. p_entry     - ENTRY Record

RETURNS:
1. p_entity - will point to entry.

Returns "no_error" if successful.
*********************************************************************/
int
read_EntityObject (char *p_entity_id, SOIP_Entity **p_entity, SOIP_DB_Entry *p_entry)
{
   ISNS_DBKey key;
   int rval;

   if ( (p_entity_id == NULL ) || (strlen(p_entity_id) <= 0) )
      return (ERROR);

   key.tag = ENTITY_ID_KEY;
   strcpy(key.val.entity_key.id,p_entity_id);
   rval = ISNSdbRead(&key,p_entry);
   *p_entity = (SOIP_Entity *)&p_entry->data;
   return rval;
}

/*********************************************************************
read_PortalObject
*********************************************************************/
int
read_PortalObject (SOIP_Portal_Key *p_portal_key, SOIP_Portal **p_portal, SOIP_DB_Entry *p_entry)
{
   ISNS_DBKey key;
   int rval;

   if ( p_portal_key == NULL )
      return (ERROR);

   key.tag = PORTAL_ID_KEY;
   __ISNS_COPY (&key.val.portal_key, sizeof(SOIP_Portal_Key), p_portal_key, sizeof(SOIP_Portal_Key));
   rval = ISNSdbRead(&key,p_entry);
   *p_portal = (SOIP_Portal *)&p_entry->data;
   return rval;
}

/*********************************************************************
read_PortalGroupObject
*********************************************************************/
int
read_PortalGroupObject (SOIP_Portal_Group_Key *p_portal_key, SOIP_Portal_Group **p_portal, SOIP_DB_Entry *p_entry)
{
   ISNS_DBKey key;
   int rval;

   if ( p_portal_key == NULL )
      return (ERROR);

   key.tag = PORTAL_GROUP_ID_KEY;
   __ISNS_COPY (&key.val.portal_group_key, sizeof(SOIP_Portal_Group_Key), p_portal_key, sizeof(SOIP_Portal_Group_Key));
   rval = ISNSdbRead(&key,p_entry);
   *p_portal = (SOIP_Portal_Group *)&p_entry->data;
   return rval;
}

/*********************************************************************
read_DDObject
Reads a DD object from the database
*********************************************************************/
int
read_DDObject (int id, SOIP_Dd **p_dd, SOIP_DB_Entry *p_entry)
{
   ISNS_DBKey key;
   int rval;

   if (id == 0)
      return (ERROR);

   key.tag = DD_ID_KEY;
   key.val.dd_key.id = id;
   rval = ISNSdbRead(&key,p_entry);
   *p_dd = (SOIP_Dd *)&p_entry->data;
   return rval;
}

/*********************************************************************
read_DDSObject
Reads a DDS object from the database
*********************************************************************/
int
read_DDSObject (int id, SOIP_Dds **p_dds, SOIP_DB_Entry *p_entry)
{
   ISNS_DBKey key;
   int rval;

   if (id == 0)
      return (ERROR);

   key.tag = DDS_ID_KEY;
   key.val.dds_key.id = id;
   rval = ISNSdbRead(&key,p_entry);
   *p_dds = (SOIP_Dds *)&p_entry->data;
   return rval;
}

/*********************************************************************
read_EntityidxObject
Reads a idx object from the database
*********************************************************************/
int
read_EntityidxObject (int id, void **p_idx, SOIP_DB_Entry *p_entry)
{
   ISNS_DBKey key;
   int rval;

   if (id == 0)
      return (ERROR);

   key.tag = ENTITY_IDX_KEY;
   key.val.idx.index = id;
   key.val.idx.index_type = ENTITY_IDX_KEY;

   rval = ISNSdbRead(&key,p_entry);
   *p_idx = (char *)&p_entry->data.entity_idx.id;

   return rval;
}

/*********************************************************************
read_ISCSIidxObject
Reads a idx object from the database
*********************************************************************/
int
read_ISCSIidxObject (int id, void **p_idx, SOIP_DB_Entry *p_entry)
{
   ISNS_DBKey key;
   int rval;

   if (id == 0)
      return (ERROR);

   key.tag = ISCSI_IDX_KEY;
   key.val.idx.index = id;
   key.val.idx.index_type = ISCSI_IDX_KEY;

   rval = ISNSdbRead(&key,p_entry);
   *p_idx = (char *)&p_entry->data.iscsi_idx;

   return rval;
}

/*********************************************************************
read_PortalidxObject
Reads a idx object from the database
*********************************************************************/
int
read_PortalidxObject (int id, void **p_idx, SOIP_DB_Entry *p_entry)
{
   ISNS_DBKey key;
   int rval;

   if (id == 0)
      return (ERROR);

   key.tag = PORTAL_IDX_KEY;
   key.val.idx.index = id;
   key.val.idx.index_type = PORTAL_IDX_KEY;

   rval = ISNSdbRead(&key,p_entry);
   *p_idx = (char *)&p_entry->data.portal_idx;

   return rval;
}


/*********************************************************************
write_FCNodeObject
Writes a FCNode object from the database
*********************************************************************/
int
write_FCNodeObject (char *p_id, SOIP_DB_Entry p_entry)
{
   ISNS_DBKey key;
   int rval;

   if ( (p_id == NULL ) || (strlen(p_id) <= 0) )
      return (ERROR);

   key.tag = NODE_NAME_KEY;
   strcpy(key.val.node_key.v,p_id);
   rval = ISNSdbWrite(&key, p_entry);
   return rval;
}

/*********************************************************************
write_FCPortObject
Writes a FCPort object from the database
*********************************************************************/
int
write_FCPortObject (char *p_id, SOIP_DB_Entry p_entry)
{
   ISNS_DBKey key;
   int rval;

   if ( (p_id == NULL ) || (strlen(p_id) <= 0) )
      return (ERROR);

   key.tag = PORT_NAME_KEY;
   strcpy(key.val.port_key.v,p_id);
   rval = ISNSdbWrite(&key, p_entry);
   return rval;
}

/*********************************************************************
write_ISCSIObject
Writes a ISCSI object from the database
*********************************************************************/
int
write_ISCSIObject (char *p_id, SOIP_DB_Entry p_entry)
{
   ISNS_DBKey key;
   int rval;

   if ( (p_id == NULL ) || (strlen(p_id) <= 0) )
      return (ERROR);

   key.tag = ISCSI_ID_KEY;
   strcpy(key.val.iscsi_key.v,p_id);
   rval = ISNSdbWrite(&key, p_entry);
   return rval;
}

/*********************************************************************
write_EntityObject
Writes a Entity object from the database
*********************************************************************/
int
write_EntityObject (char *p_entity_id, SOIP_DB_Entry p_entry)
{
   ISNS_DBKey key;
   int rval;

   if ( (p_entity_id == NULL ) || (strlen(p_entity_id) <= 0) )
      return (ERROR);

   key.tag = ENTITY_ID_KEY;
   strcpy(key.val.entity_key.id,p_entity_id);
   rval = ISNSdbWrite(&key, p_entry);
   return rval;
}

/*********************************************************************
write_PortalObject
Writes a Portal object from the database
*********************************************************************/
int
write_PortalObject (SOIP_Portal_Key *p_id, SOIP_DB_Entry p_entry)
{
   ISNS_DBKey key;
   int rval;

   if ( (p_id == NULL ) )
      return (ERROR);

   key.tag = PORTAL_ID_KEY;
   __ISNS_COPY(&key.val.portal_key, sizeof(SOIP_Portal_Key), p_id, sizeof(SOIP_Portal_Key));
   rval = ISNSdbWrite(&key, p_entry);
   return rval;
}

/*********************************************************************
write_PortalGroupObject
Writes a PortalGroup object from the database
*********************************************************************/
int
write_PortalGroupObject (SOIP_Portal_Group_Key *p_id, SOIP_DB_Entry p_entry)
{
   ISNS_DBKey key;
   int rval;

   if ( (p_id == NULL ) )
      return (ERROR);

   key.tag = PORTAL_GROUP_ID_KEY;
   __ISNS_COPY(&key.val.portal_group_key, sizeof(SOIP_Portal_Group_Key), p_id, sizeof(SOIP_Portal_Group_Key));
   rval = ISNSdbWrite(&key, p_entry);
   return rval;
}

/*********************************************************************
write_DDObject
Writes a DD object from the database
*********************************************************************/
int
write_DDObject (int id, SOIP_DB_Entry p_entry)
{
   ISNS_DBKey key;
   int rval;

   if (id == 0)
      return (ERROR);

   key.tag = DD_ID_KEY;
   key.val.dd_key.id = id;
   rval = ISNSdbWrite(&key, p_entry);
   return rval;
}

/*********************************************************************
write_DDSObject
Writes a DDS object from the database
*********************************************************************/
int
write_DDSObject (int id, SOIP_DB_Entry p_entry)
{
   ISNS_DBKey key;
   int rval;

   if (id == 0)
      return (ERROR);

   key.tag = DDS_ID_KEY;
   key.val.dds_key.id = id;
   rval = ISNSdbWrite(&key, p_entry);
   return rval;
}

/*********************************************************************
write_ISCSIidxObject
Writes a index object from the database
*********************************************************************/
int
write_ISCSIidxObject (int id, SOIP_DB_Entry p_entry)
{
  ISNS_DBKey idx_key;
  int rval;

  if (id == 0)
     return (ERROR);

  idx_key.tag = ISCSI_IDX_KEY;
  idx_key.val.idx.index_type = ISCSI_IDX_KEY;
  idx_key.val.idx.index = id;
  rval = ISNSdbWrite(&idx_key,p_entry);

  return rval;
}

/*********************************************************************
write_EntityidxObject
Writes a index object from the database
*********************************************************************/
int
write_EntityidxObject (int id, SOIP_DB_Entry p_entry)
{
  ISNS_DBKey idx_key;
  int rval;

  if (id == 0)
     return (ERROR);

  idx_key.tag = ENTITY_IDX_KEY;
  idx_key.val.idx.index_type = ENTITY_IDX_KEY;
  idx_key.val.idx.index = id;
  rval = ISNSdbWrite(&idx_key,p_entry);

  return rval;
}

/*********************************************************************
write_PortalidxObject
Writes a index object from the database
*********************************************************************/
int
write_PortalidxObject (int id, SOIP_DB_Entry p_entry)
{
  ISNS_DBKey idx_key;
  int rval;

  if (id == 0)
     return (ERROR);

  idx_key.tag = PORTAL_IDX_KEY;
  idx_key.val.idx.index_type = PORTAL_IDX_KEY;
  idx_key.val.idx.index = id;
  rval = ISNSdbWrite(&idx_key,p_entry);

  return rval;
}

/*********************************************************************
delete_DDObject
delete a DD object from the database
*********************************************************************/
int
delete_DDObject (int id)
{
   ISNS_DBKey key;
   int rval;

   if (id == 0)
      return (ERROR);

   key.tag = DD_ID_KEY;
   key.val.dd_key.id = id;
   rval = ISNSdbDelete(&key);
   return rval;
}

/*********************************************************************
delete_DDSObject
delete a DDS object from the database
*********************************************************************/
int
delete_DDSObject (int id)
{
   ISNS_DBKey key;
   int rval;

   if (id == 0)
      return (ERROR);

   key.tag = DDS_ID_KEY;
   key.val.dds_key.id = id;
   rval = ISNSdbDelete(&key);
   return rval;
}

/*********************************************************************
delete_FCNodeObject
delete a FCNode object from the database
*********************************************************************/
int
delete_FCNodeObject (char *p_id)
{
   ISNS_DBKey key;
   int rval;

   if ( (p_id == NULL ) || (strlen(p_id) <= 0) )
      return (ERROR);

   key.tag = NODE_NAME_KEY;
   strcpy(key.val.node_key.v,p_id);

   rval = ISNSdbDelete(&key);
   return rval;
}

/*********************************************************************
delete_FCPortObject
delete a FCPort object from the database
*********************************************************************/
int
delete_FCPortObject (char *p_id)
{
   ISNS_DBKey key;
   int rval;

   if ( (p_id == NULL ) || (strlen(p_id) <= 0) )
      return (ERROR);

   key.tag = PORT_NAME_KEY;
   strcpy(key.val.port_key.v,p_id);

   rval = ISNSdbDelete(&key);
   return rval;
}

/*********************************************************************
delete_ISCSIObject
delete a ISCSI object from the database
*********************************************************************/
int
delete_ISCSIObject (char *p_id)
{
   ISNS_DBKey key;
   int rval;

   if ( (p_id == NULL ) || (strlen(p_id) <= 0) )
      return (ERROR);

   key.tag = ISCSI_ID_KEY;
   strcpy(key.val.node_key.v,p_id);

   rval = ISNSdbDelete(&key);
   return rval;
}

/*********************************************************************
delete_EntityObject
delete a Enity object from the database
*********************************************************************/
int
delete_EntityObject (char *p_entity_id)
{
   ISNS_DBKey key;
   int rval;

   if ( (p_entity_id == NULL ) || (strlen(p_entity_id) <= 0) )
      return (ERROR);

   key.tag = ENTITY_ID_KEY;
   strcpy(key.val.entity_key.id,p_entity_id);

   rval = ISNSdbDelete(&key);
   return rval;
}

/*********************************************************************
delete_PortalObject
delete a Portal object from the database
*********************************************************************/
int
delete_PortalObject (SOIP_Portal_Key *p_id)
{
   ISNS_DBKey key;
   int rval;

   if ( (p_id == NULL ) )
      return (ERROR);

   key.tag = PORTAL_ID_KEY;
   __ISNS_COPY(&key.val.portal_key, sizeof(SOIP_Portal_Key), p_id, sizeof(SOIP_Portal_Key));

   rval = ISNSdbDelete(&key);
   return rval;
}

/*********************************************************************
delete_PortalGroupObject
delete a PortalGroup object from the database
*********************************************************************/
int
delete_PortalGroupObject (SOIP_Portal_Group_Key *p_id)
{
   ISNS_DBKey key;
   int rval;

   if ( (p_id == NULL ) )
      return (ERROR);

   key.tag = PORTAL_GROUP_ID_KEY;
   __ISNS_COPY(&key.val.portal_group_key, sizeof(SOIP_Portal_Group_Key), p_id, sizeof(SOIP_Portal_Group_Key));

   rval = ISNSdbDelete(&key);
   return rval;
}

/*********************************************************************
delete_EntityidxObject
delete a idx object from the database
*********************************************************************/
int
delete_EntityidxObject (int id)
{
   ISNS_DBKey key;
   int rval;

   if (id == 0)
      return (ERROR);

   key.tag = ENTITY_IDX_KEY;
   key.val.idx.index = id;
   key.val.idx.index_type = ENTITY_IDX_KEY;
   rval = ISNSdbDelete(&key);
   return rval;
}

/*********************************************************************
delete_ISCSIidxObject
delete a idx object from the database
*********************************************************************/
int
delete_ISCSIidxObject (int id)
{
   ISNS_DBKey key;
   int rval;

   if (id == 0)
      return (ERROR);

   key.tag = ISCSI_IDX_KEY;
   key.val.idx.index = id;
   key.val.idx.index_type = ISCSI_IDX_KEY;
   rval = ISNSdbDelete(&key);
   return rval;
}
/*********************************************************************
delete_PortalidxObject
delete a idx object from the database
*********************************************************************/
int
delete_PortalidxObject (int id)
{
   ISNS_DBKey key;
   int rval;

   if (id == 0)
      return (ERROR);

   key.tag = PORTAL_IDX_KEY;
   key.val.idx.index = id;
   key.val.idx.index_type = PORTAL_IDX_KEY;
   rval = ISNSdbDelete(&key);
   return rval;
}
