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

#include "iSNS.h"
#include "iSNStypes.h"
#include "iSNSList.h"
#include "iSNSdb.h"
#include "iSNSbuffer.h"
#include "iSNSdebug.h"

int isns_list_debug = 0;

/********************************************************************
Initializes a doubly linked list structure.  The linked list has a
dummy node at the head of the list.
********************************************************************/
int
InitList(int list_id, void * record)
{
   ISNS_LIST *plist=NULL;

   switch (list_id)
   {
      case ISCSI_DD_LIST:
      {
         SOIP_Iscsi *p_entry;
         p_entry = record;
         plist = &p_entry->dd_id_list;
         plist->p_entry = p_entry;
         break;
      } 
      case DDS_DD_LIST:
      {
         SOIP_Dds *p_entry;
         p_entry = record;
         plist = &p_entry->dd_list;
         plist->p_entry = p_entry;
         break;
      } 
      case DD_DDS_LIST:
      {
         SOIP_Dd *p_entry;
         p_entry = record;
         plist = &p_entry->dds_list;
         plist->p_entry = p_entry;
         break;
      } 
      case DD_MEMBER_LIST:
      {
         SOIP_Dd *p_entry;
         p_entry = record;
         plist = &p_entry->member_list;
         plist->p_entry = p_entry;
         break;
      } 
      case ENTITY_PORTAL_LIST:
      {
         SOIP_Entity *p_entry;
         p_entry = record;
         plist = &p_entry->iportal_list;
         plist->p_entry = p_entry;
         break;
      } 
      case ENTITY_ISCSI_LIST:
      {
         SOIP_Entity *p_entry;
         p_entry = record;
         plist = &p_entry->iscsi_node_list;
         plist->p_entry = p_entry;
         break;
      } 
      case ENTITY_FCP_LIST:
      {
         SOIP_Entity *p_entry;
         p_entry = record;
         plist = &p_entry->ifcp_node_list;
         plist->p_entry = p_entry;
         break;
      } 
      case FCP_PORTAL_LIST:
      {
         SOIP_Fc_Node *p_entry;
         p_entry = record;
         plist = &p_entry->port_list;
         plist->p_entry = p_entry;
         break;
      } 
      case FCP_DD_LIST:
      {
         SOIP_Ifcp *p_entry;
         p_entry = record;
         plist = &p_entry->dd_id_list;
         plist->p_entry = p_entry;
         break;
      }
      case SCN_CALLBACK_LIST:
      {
         ISNS_LIST *p_entry;
         p_entry = record;
         plist = p_entry;
         plist->p_entry = p_entry;
         break;
      }
      case SCN_LIST:
      {
         ISNS_LIST *p_entry;
         p_entry = record;
         plist = p_entry;
         plist->p_entry = p_entry;
         break;
      }

   }

   plist->list_id = list_id;
   plist->first_index = 0;
   plist->last_index = 0;
   plist->node_count = 0;

   __DEBUG (isns_list_debug &1,InitList list_id:%i,plist->list_id);
   __DEBUG (isns_list_debug &1,InitList node_count:%i,plist->node_count);

   return ( SUCCESS );
}

/********************************************************************
Deletes a list.
********************************************************************/
int
DeleteList(ISNS_LIST *plist)
{
   ISNS_LIST_NODE pnode;

   __DEBUG (isns_list_debug &1,DeleteList list_id:%i,plist->list_id);

   while (!IsEmptyList(plist))
   {
      pnode.index = plist->first_index;
      RemoveNode(plist, &pnode);
   }

   return (SUCCESS);
}

/********************************************************************
Removes a node from a list.
********************************************************************/
int
RemoveNode(ISNS_LIST *plist, ISNS_LIST_NODE *pnode)
{
   int foundFlag;

   ISNS_LIST_NODE *curr_node;
   int rval;
   ISNS_DBKey key;
   SOIP_DB_Entry entry;
   int next_index;
   int prev_index;
   SOIP_Entity *p_entity;
   SOIP_Iscsi *p_iscsi;
   SOIP_Dds *p_dds;
   SOIP_Dd *p_dd;
   SOIP_Portal *p_portal;
   SOIP_Fc_Node *p_ifcp;
   ISNS_LIST *p_list;


   __DEBUG (isns_list_debug &1,Remove Node);

   foundFlag = FALSE;

   curr_node = NULL;
   curr_node = GetNextNode(plist, curr_node);
   while ( curr_node != NULL )
   {
      __DEBUG (isns_list_debug &1, curr_node:%i pnode:%i,curr_node->index,pnode->index);
      if (curr_node->index ==  pnode->index )
      {
         foundFlag = TRUE;
         break;
      }
      curr_node = GetNextNode(plist, curr_node);
   }

   if ( (curr_node == NULL) && (plist->node_count > 0) )
   {
      __DEBUG (isns_list_debug &1,Remove Node FAILURE node_count:%i,plist->node_count);
      plist->node_count--;
      return ERROR; /* we should have found a node in this case */
   }

   __DEBUG (isns_list_debug &1, Remove Node FoundFlag: %i,foundFlag);       
   if ( foundFlag )
   {
       __DEBUG (isns_list_debug &1, FoundFlag);       

        /* read current node */
       memset(&key,0,sizeof(ISNS_DBKey));
       key.tag = LIST_KEY;
       key.val.list.list_id = plist->list_id;
       key.val.list.list_index = pnode->index;
       memset(&entry,0,sizeof(SOIP_DB_Entry));

       if (plist->list_id == ENTITY_ISCSI_LIST)
       {
         p_entity = plist->p_entry;
         strcpy(key.val.list.key.node_name.v,p_entity->eid.id);
       }
       else if (plist->list_id == ENTITY_FCP_LIST)
       {
         p_entity = plist->p_entry;
         strcpy(key.val.list.key.node_name.v,p_entity->eid.id);
       }
       else if (plist->list_id == ENTITY_PORTAL_LIST)
       {
         p_entity = plist->p_entry;
         strcpy(key.val.list.key.node_name.v,p_entity->eid.id);
       }
       else if (plist->list_id == ISCSI_DD_LIST)
       {
         p_iscsi = plist->p_entry;
         strcpy(key.val.list.key.node_name.v,p_iscsi->id.v);
       }
       else if (plist->list_id == DDS_DD_LIST)
       {
         p_dds = plist->p_entry;
         key.val.list.key.dds.id = p_dds->id;
       }
       else if (plist->list_id == DD_DDS_LIST)
       {
         p_dd = plist->p_entry;
         key.val.list.key.dd.id = p_dd->id;
       }
       else if (plist->list_id == DD_MEMBER_LIST)
       {
         p_dd = plist->p_entry;
         key.val.list.key.dd.id = p_dd->id;
       }
       else if (plist->list_id == FCP_PORTAL_LIST)
       {
         p_ifcp = plist->p_entry;
         strcpy (key.val.list.key.node_name.v,p_ifcp->node_name.v);
       }
       else if (plist->list_id == FCP_DD_LIST)
       {
         p_ifcp = plist->p_entry;
         strcpy (key.val.list.key.node_name.v,p_ifcp->node_name.v);
       }
       else if (plist->list_id == PORTAL_ENTITY_LIST)
       {
         p_portal = plist->p_entry;
         __ISNS_COPY (&key.val.list.key.portal.ip_addr, sizeof(key.val.list.key.portal.ip_addr), &p_portal->ip_addr, sizeof(IP_Address));
         key.val.list.key.portal.ip_port = p_portal->ip_port;
       }
       else if (plist->list_id == SCN_LIST)
       {
         p_list = plist->p_entry;
         strcpy (key.val.list.key.node_name.v,"scn_list");
       }
       else if (plist->list_id == SCN_CALLBACK_LIST)
       {
         p_list = plist->p_entry;
         strcpy (key.val.list.key.node_name.v,"scn_callback_list");
       }

       rval = ISNSdbRead(&key,&entry);
       next_index = entry.data.list.next_index;
       prev_index = entry.data.list.prev_index;

       /*delete current node*/
       rval = ISNSdbDelete(&key);

       /*pdate first_index if necessary*/
       if (plist->first_index == key.val.list.list_index )
       {
          plist->first_index = next_index;
       }

       /*update last_index if necessary*/
       if (plist->last_index == key.val.list.list_index )
       {
          plist->last_index = prev_index;
       }

       /*update previous node*/
       if (prev_index != 0)
       {
           key.val.list.list_index = prev_index;
           rval = ISNSdbRead(&key,&entry);
           entry.data.list.next_index = next_index;
           rval = ISNSdbWrite(&key,entry);
       }

       /*update next node*/
       if (next_index != 0)
       {
           key.val.list.list_index = next_index;
           rval = ISNSdbRead(&key,&entry);
           entry.data.list.prev_index = prev_index;
           rval = ISNSdbWrite(&key,entry);
       }

       /*update node_count*/
       __DEBUG (isns_list_debug &1, RemoveNode - node_count:%i,plist->node_count);
       plist->node_count--;
   }

   return ( SUCCESS );
}

/********************************************************************
Retrieves the data pointer from a node.
********************************************************************/
void *
GetNodeData(ISNS_LIST_NODE *pnode )
{
   __DEBUG (isns_list_debug &1, GetNodeData);
   return ( pnode->data );
}

/********************************************************************
Finds an object in the list.
********************************************************************/
ISNS_LIST_NODE *
FindNode(ISNS_LIST *plist, char *pdata, int data_size)
{
   ISNS_LIST_NODE *ptr;

   __DEBUG (isns_list_debug &1,FindNode);

   ptr = NULL;

   while ( (ptr=GetNextNode(plist, ptr)) )
   {
      if (ptr->data_size == data_size &&
          !memcmp(ptr->data, pdata, data_size))
      {
         return (ptr);
      }
   }

   return ( NULL );
}

/********************************************************************
Adds a node to a list.
********************************************************************/
int
AddNode(ISNS_LIST *plist, char *pdata, int data_size)
{
   ISNS_DBKey key;
   SOIP_DB_Entry entry;

   int rval;
   SOIP_Entity *p_entity;
   SOIP_Iscsi *p_iscsi;
   SOIP_Dds *p_dds;
   SOIP_Dd *p_dd;
   SOIP_Portal *p_portal;
   SOIP_Fc_Node *p_ifcp;

   __DEBUG (isns_list_debug &1, AddNode - list_id:%i,plist->list_id);

   memset(&key,0,sizeof(ISNS_DBKey));
   key.tag = LIST_KEY;
   key.val.list.list_id = plist->list_id;
   memset(&entry,0,sizeof(SOIP_DB_Entry));

   if (plist->list_id == ENTITY_ISCSI_LIST)
   {
     p_entity = plist->p_entry;
     strcpy(key.val.list.key.node_name.v,p_entity->eid.id);
     __ISNS_COPY (entry.data.list.key.node_name.v, sizeof(entry.data.list.key.node_name.v), pdata, data_size);
   }
   else if (plist->list_id == ENTITY_FCP_LIST)
   {
     p_entity = plist->p_entry;
     strcpy(key.val.list.key.node_name.v,p_entity->eid.id);
     __ISNS_COPY (entry.data.list.key.node_name.v, sizeof(entry.data.list.key.node_name.v), pdata, data_size);
   }
   else if (plist->list_id == ENTITY_PORTAL_LIST)
   {
     p_entity = plist->p_entry;
     strcpy(key.val.list.key.node_name.v,p_entity->eid.id);
     __ISNS_COPY (&entry.data.list.key.portalmember, sizeof(entry.data.list.key.portalmember), pdata, data_size);
   }
   else if (plist->list_id == ISCSI_DD_LIST)
   {
     p_iscsi = plist->p_entry;
     strcpy(key.val.list.key.node_name.v,p_iscsi->id.v);
     __ISNS_COPY (&entry.data.list.key.dd, sizeof(entry.data.list.key.dd), pdata, data_size);
   }
   else if (plist->list_id == DDS_DD_LIST)
   {
     p_dds = plist->p_entry;
     key.val.list.key.dds.id = p_dds->id;
     __ISNS_COPY (&entry.data.list.key.dd, sizeof(entry.data.list.key.dd), pdata, data_size);
   }
   else if (plist->list_id == DD_DDS_LIST)
   {
     p_dd = plist->p_entry;
     key.val.list.key.dd.id = p_dd->id;
     __ISNS_COPY (&entry.data.list.key.dds, sizeof(entry.data.list.key.dds), pdata, data_size);
   }
   else if (plist->list_id == DD_MEMBER_LIST)
   {
     p_dd = plist->p_entry;
     key.val.list.key.dd.id = p_dd->id;
     __ISNS_COPY (&entry.data.list.key.ddmember, sizeof(entry.data.list.key.ddmember), pdata, data_size);
   }
   else if (plist->list_id == FCP_PORTAL_LIST)
   {
     p_ifcp = plist->p_entry;
     strcpy (key.val.list.key.node_name.v,p_ifcp->node_name.v);
     __ISNS_COPY (&entry.data.list.key.portal, sizeof(entry.data.list.key.portal), pdata, data_size);
   }
   else if (plist->list_id == FCP_DD_LIST)
   {
     p_ifcp = plist->p_entry;
     strcpy (key.val.list.key.node_name.v,p_ifcp->node_name.v);
     __ISNS_COPY (&entry.data.list.key.dd, sizeof(entry.data.list.key.dd), pdata, data_size);
   }
   else if (plist->list_id == PORTAL_ENTITY_LIST)
   {
     p_portal = plist->p_entry;
     __ISNS_COPY (&key.val.list.key.portal.ip_addr, sizeof(key.val.list.key.portal.ip_addr), &p_portal->ip_addr, sizeof(IP_Address));
     key.val.list.key.portal.ip_port = p_portal->ip_port;
     __ISNS_COPY (&entry.data.list.key.portal, sizeof(entry.data.list.key.portal), pdata, data_size);
   }
   else if (plist->list_id == SCN_LIST)
   {
     strcpy (key.val.list.key.node_name.v,"scn_list");
     __ISNS_COPY (&entry.data.list.key.node_name.v, sizeof(entry.data.list.key.node_name.v), pdata, data_size);
   }
   else if (plist->list_id == SCN_CALLBACK_LIST)
   {
     strcpy (key.val.list.key.node_name.v,"scn_callback_list");
     __ISNS_COPY (&entry.data.list.key.node_name.v, sizeof(entry.data.list.key.node_name.v), pdata,data_size);
   }
   
   __DEBUG (isns_list_debug &1, update last_index);

   plist->last_index++;
   key.val.list.list_index = plist->last_index;

   entry.data_type = LIST_KEY;
   entry.data.list.index = plist->last_index;
   entry.data.list.prev_index = plist->last_index-1;
   entry.data.list.next_index = 0;

   if (plist->first_index == 0)
         plist->first_index = plist->last_index;

   rval = ISNSdbWrite(&key,entry);

   if (entry.data.list.prev_index != 0 )
   {
     key.val.list.list_index = entry.data.list.prev_index;
     rval = ISNSdbRead(&key,&entry);
     entry.data.list.next_index = plist->last_index;
     rval = ISNSdbWrite(&key,entry);
   }

   plist->node_count++;
   __DEBUG (isns_list_debug &1, AddNode node_count:%i,plist->node_count);

   return ( SUCCESS );
}

/********************************************************************
Returns true if the list is empty.
********************************************************************/
int
IsEmptyList(ISNS_LIST *plist)
{
   __DEBUG (isns_list_debug &1,IsEmptyList node_count:%i,plist->node_count);

   if (plist->node_count == 0)
      return (TRUE);

   else
      return (FALSE);
}

/********************************************************************
Returns the next node in a list.
********************************************************************/
ISNS_LIST_NODE *
GetNextNode(ISNS_LIST *plist, ISNS_LIST_NODE *pnode)
{
   ISNS_LIST_NODE *node;
   ISNS_DBKey key;
   SOIP_DB_Entry entry;
   int rval;
   SOIP_Entity *p_entity;
   SOIP_Iscsi *p_iscsi;
   SOIP_Dds *p_dds;
   SOIP_Dd *p_dd;
   SOIP_Portal *p_portal;
   SOIP_Fc_Node *p_ifcp;
   ISNS_LIST *p_list;

   __DEBUG (isns_list_debug &1,GetNextNode list_id:%i node_count:%i, plist->list_id,plist->node_count);

   if (!plist || plist->node_count == 0)
   {
      return ( NULL );
   }

   memset(&key,0,sizeof(ISNS_DBKey));
   key.tag = LIST_KEY;
   key.val.list.list_id = plist->list_id;
   memset(&entry,0,sizeof(SOIP_DB_Entry));

   if ( pnode == NULL)
   {
     __DEBUG (isns_list_debug &1, AllocBuffer for first node);
     node = (ISNS_LIST_NODE *)ISNSAllocBuffer(sizeof(ISNS_LIST_NODE));
     key.val.list.list_index = plist->first_index;
     if (key.val.list.list_index == 0 )
     {
        __DEBUG (isns_list_debug &1,Error condition node count>0 but first_index is 0);
        plist->node_count = 0;
        return NULL;
     }
   }
   else
   {
     node = pnode;
     __DEBUG (isns_list_debug &1, next_index:%i,node->next_index);
     key.val.list.list_index = node->next_index;
   }

   if (key.val.list.list_index == 0)
   {
     __DEBUG (isns_list_debug &1, No More Entries in list);
     return NULL;
   }
   
   __DEBUG (isns_list_debug &1,Now Read entry);
   if (plist->list_id == ENTITY_ISCSI_LIST)
   {
     p_entity = plist->p_entry;
     strcpy(key.val.list.key.node_name.v,p_entity->eid.id);
     __DEBUG (isns_list_debug &1, read list entry);
     rval = ISNSdbRead(&key,&entry);
     if (rval != SUCCESS)
     {
          plist->node_count--;
          return NULL;
     }
     node->data_size = strlen(entry.data.list.key.node_name.v);
     node->data = (char *)ISNSAllocBuffer(node->data_size+1);
     __ISNS_COPY (node->data, node->data_size+1, entry.data.list.key.node_name.v, node->data_size);
     __DEBUG (isns_list_debug &1, Successfully read list entry);
   }
   else if (plist->list_id == ENTITY_FCP_LIST)
   {
     p_entity = plist->p_entry;
     strcpy(key.val.list.key.node_name.v,p_entity->eid.id);
     rval = ISNSdbRead(&key,&entry);
     if (rval != SUCCESS)
          return NULL;
     node->data_size = strlen(entry.data.list.key.node_name.v);
     node->data = (char *)ISNSAllocBuffer(node->data_size+1);
     __ISNS_COPY (node->data, node->data_size+1, entry.data.list.key.node_name.v,node->data_size);
   }
   else if (plist->list_id == ENTITY_PORTAL_LIST)
   {
     p_entity = plist->p_entry;
     strcpy(key.val.list.key.node_name.v,p_entity->eid.id);
     rval = ISNSdbRead(&key,&entry);
     if (rval != SUCCESS)
          return NULL;
     node->data_size = sizeof(entry.data.list.key.portal);
     node->data = (char *)ISNSAllocBuffer(node->data_size+1);
     __ISNS_COPY (node->data, node->data_size +1, &entry.data.list.key.portal,node->data_size);
   }
   else if (plist->list_id == ISCSI_DD_LIST)
   {
     p_iscsi = plist->p_entry;
     strcpy(key.val.list.key.node_name.v,p_iscsi->id.v);
     rval = ISNSdbRead(&key,&entry);
     if (rval != SUCCESS)
          return NULL;
     node->data_size = sizeof(entry.data.list.key.dd);
     node->data = (char *)ISNSAllocBuffer(node->data_size+1);
     __ISNS_COPY (node->data, node->data_size + 1, &entry.data.list.key.dd,node->data_size);
   }
   else if (plist->list_id == DDS_DD_LIST)
   {
     p_dds = plist->p_entry;
     key.val.list.key.dds.id = p_dds->id;
     rval = ISNSdbRead(&key,&entry);
     if (rval != SUCCESS)
          return NULL;
     node->data_size = sizeof(entry.data.list.key.dd);
     node->data = (char *)ISNSAllocBuffer(node->data_size+1);
     __ISNS_COPY (node->data, node->data_size + 1, &entry.data.list.key.dd,node->data_size);
   }
   else if (plist->list_id == DD_DDS_LIST)
   {
     p_dd = plist->p_entry;
     key.val.list.key.dd.id = p_dd->id;
     rval = ISNSdbRead(&key,&entry);
     if (rval != SUCCESS)
          return NULL;
     node->data_size = sizeof(entry.data.list.key.dds);
     node->data = (char *)ISNSAllocBuffer(node->data_size+1);
     __ISNS_COPY (node->data, node->data_size+1, &entry.data.list.key.dds,node->data_size);
   }
   else if (plist->list_id == DD_MEMBER_LIST)
   {
     p_dd = plist->p_entry;
     key.val.list.key.dd.id = p_dd->id;
     rval = ISNSdbRead(&key,&entry);
     if (rval != SUCCESS)
          return NULL;
     node->data_size = sizeof(entry.data.list.key.ddmember);
     node->data = (char *)ISNSAllocBuffer(node->data_size+1);
     __ISNS_COPY (node->data, node->data_size+1, &entry.data.list.key.ddmember,node->data_size);
   }
   else if (plist->list_id == FCP_PORTAL_LIST)
   {
     p_ifcp = plist->p_entry;
     strcpy (key.val.list.key.node_name.v,p_ifcp->node_name.v);
     rval = ISNSdbRead(&key,&entry);
     if (rval != SUCCESS)
          return NULL;
     node->data_size = sizeof(entry.data.list.key.portal);
     node->data = (char *)ISNSAllocBuffer(node->data_size+1);
     __ISNS_COPY (node->data, node->data_size+1, &entry.data.list.key.portal,node->data_size);
   }
   else if (plist->list_id == FCP_DD_LIST)
   {
     p_ifcp = plist->p_entry;
     strcpy (key.val.list.key.node_name.v,p_ifcp->node_name.v);
     rval = ISNSdbRead(&key,&entry);
     if (rval != SUCCESS)
          return NULL;
     node->data_size = sizeof(entry.data.list.key.dd);
     node->data = (char *)ISNSAllocBuffer(node->data_size+1);
     __ISNS_COPY (node->data, node->data_size + 1, &entry.data.list.key.dd,node->data_size);
   }
   else if (plist->list_id == PORTAL_ENTITY_LIST)
   {
     p_portal = plist->p_entry;
     __ISNS_COPY (&key.val.list.key.portal.ip_addr, sizeof(key.val.list.key.portal.ip_addr), &p_portal->ip_addr,sizeof(IP_Address));
     key.val.list.key.portal.ip_port = p_portal->ip_port;
     rval = ISNSdbRead(&key,&entry);
     if (rval != SUCCESS)
          return NULL;
     node->data_size = sizeof(entry.data.list.key.portal);
     node->data = (char *)ISNSAllocBuffer(node->data_size+1);
     __ISNS_COPY (node->data, node->data_size+1, &entry.data.list.key.portal,node->data_size);
   }
   else if (plist->list_id == SCN_LIST)
   {
     p_list = (ISNS_LIST *)plist->p_entry;
     strcpy (key.val.list.key.node_name.v,"scn_list");
     rval = ISNSdbRead(&key,&entry);
     if (rval != SUCCESS)
          return NULL;
     node->data_size = sizeof(SOIP_Scn_Entry);
     node->data = (char *)ISNSAllocBuffer(node->data_size+1);
     __ISNS_COPY (node->data, node->data_size+1, &entry.data.list.key.entity_id.id,node->data_size);
   }
   else if (plist->list_id == SCN_CALLBACK_LIST)
   {
     p_list = (ISNS_LIST *)plist->p_entry;
     strcpy (key.val.list.key.node_name.v,"scn_callback_list");
     rval = ISNSdbRead(&key,&entry);
     if (rval != SUCCESS)
          return NULL;
     node->data_size = sizeof(SOIP_Scn_Callback_Entry);
     node->data = (char *)ISNSAllocBuffer(node->data_size+1);
     __ISNS_COPY (node->data, node->data_size+1, &entry.data.list.key.entity_id.id,node->data_size);
   }

   node->next_index = entry.data.list.next_index;
   node->prev_index = entry.data.list.prev_index;
   node->index = entry.data.list.index;
   node->list_id= plist->list_id;
   node->p_entry = plist->p_entry;

   __DEBUG (isns_list_debug &1, return GetNextNode node);
   return node;
}

/********************************************************************
Returns the previous node in a list.
********************************************************************/
ISNS_LIST_NODE *
GetPrevNode(ISNS_LIST *plist, ISNS_LIST_NODE *pnode)
{
   //return ( pnode?pnode->prev:NULL );
   return NULL;
}


