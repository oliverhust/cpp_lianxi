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
judge If the list is initialized
********************************************************************/
STATIC BOOL_T _list_IsInit(IN const ISNS_LIST *pstList)
{
    if(pstList->list_id <= 0 || pstList->list_id >= DATA_LIST_MAX ||
       NULL == pstList->p_entry)
    {
        return BOOL_FALSE;
    }
    return BOOL_TRUE;
}

/********************************************************************
Initializes a doubly linked list structure.  The linked list has a
dummy node at the head of the list.
********************************************************************/
int
InitList(int list_id, void * record)
{
   ISNS_LIST *pstList = NULL;

   switch (list_id)
   {
      case ISCSI_DD_LIST:
      {
         SOIP_Iscsi *p_entry;
         p_entry = record;
         pstList = &p_entry->dd_id_list;
         pstList->p_entry = p_entry;
         break;
      }
      case DDS_DD_LIST:
      {
         SOIP_Dds *p_entry;
         p_entry = record;
         pstList = &p_entry->dd_list;
         pstList->p_entry = p_entry;
         break;
      }
      case DD_DDS_LIST:
      {
         SOIP_Dd *p_entry;
         p_entry = record;
         pstList = &p_entry->dds_list;
         pstList->p_entry = p_entry;
         break;
      }
      case DD_MEMBER_LIST:
      {
         SOIP_Dd *p_entry;
         p_entry = record;
         pstList = &p_entry->member_list;
         pstList->p_entry = p_entry;
         break;
      }
      case ENTITY_PORTAL_LIST:
      {
         SOIP_Entity *p_entry;
         p_entry = record;
         pstList = &p_entry->iportal_list;
         pstList->p_entry = p_entry;
         break;
      }
      case ENTITY_ISCSI_LIST:
      {
         SOIP_Entity *p_entry;
         p_entry = record;
         pstList = &p_entry->iscsi_node_list;
         pstList->p_entry = p_entry;
         break;
      }
      case ENTITY_FCP_LIST:
      {
         SOIP_Entity *p_entry;
         p_entry = record;
         pstList = &p_entry->ifcp_node_list;
         pstList->p_entry = p_entry;
         break;
      }
      case FCP_PORTAL_LIST:
      {
         SOIP_Fc_Node *p_entry;
         p_entry = record;
         pstList = &p_entry->port_list;
         pstList->p_entry = p_entry;
         break;
      }
      case FCP_DD_LIST:
      {
         SOIP_Ifcp *p_entry;
         p_entry = record;
         pstList = &p_entry->dd_id_list;
         pstList->p_entry = p_entry;
         break;
      }
      case SCN_CALLBACK_LIST:
      {
         ISNS_LIST *p_entry;
         p_entry = record;
         pstList = p_entry;
         pstList->p_entry = p_entry;
         break;
      }
      case SCN_LIST:
      {
         ISNS_LIST *p_entry;
         p_entry = record;
         pstList = p_entry;
         pstList->p_entry = p_entry;
         break;
      }
      default:
      {
         break;
      }

   }

    if(NULL == pstList)
    {
        __LOG_ERROR("Init List: Unknown list type %d", list_id);
        return ISNS_UNKNOWN_ERR;
    }

    memset(pstList, 0, sizeof(ISNS_LIST));

    pstList->pstHead = (DTQ_HEAD_S *)malloc(sizeof(DTQ_HEAD_S));
    if(NULL == pstList->pstHead)
    {
        return ISNS_UNKNOWN_ERR;
    }
    DTQ_Init(pstList->pstHead);
    pstList->list_id = list_id;

    __DEBUG (isns_list_debug &1,InitList list_id:%i,pstList->list_id);

    return ( SUCCESS );
}

/********************************************************************
Deletes a list.
********************************************************************/
int
DeleteList(ISNS_LIST *pstList)
{
    DTQ_HEAD_S *pstHead = pstList->pstHead;
    ISNS_LIST_NODE *pstNode;

    __DEBUG (isns_list_debug &1,DeleteList list_id:%i,pstList->list_id);

    if(BOOL_FALSE == _list_IsInit(pstList))
    {
        return ISNS_UNKNOWN_ERR;
    }
    if(NULL == pstList->pstHead)
    {
        return (SUCCESS);
    }

    DTQ_FOREACH_ENTRY(pstHead, pstNode, stNode)
    {
        DTQ_Del(&pstNode->stNode);
        free(pstNode->data);
        free(pstNode);
    }

    free(pstList->pstHead);
    pstList->pstHead = NULL;

    return (SUCCESS);
}

/********************************************************************
Removes a node from a list.
********************************************************************/
int
RemoveNode(ISNS_LIST *pstList, ISNS_LIST_NODE *pstNode)
{
    __DEBUG (isns_list_debug &1,Remove Node);

    if(BOOL_FALSE == _list_IsInit(pstList) || NULL == pstList->pstHead)
    {
        return ISNS_UNKNOWN_ERR;
    }

    DTQ_Del(&pstNode->stNode);
    free(pstNode->data);
    free(pstNode);

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
FindNode(ISNS_LIST *pstList, char *pdata, int data_size)
{
   ISNS_LIST_NODE *ptr;

   __DEBUG (isns_list_debug &1,FindNode list_id:%i, pstList->list_id);

   ptr = NULL;

   while ( (ptr=GetNextNode(pstList, ptr)) )
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
AddNode(ISNS_LIST *pstList, char *pdata, int data_size)
{
    ISNS_LIST_NODE *pstNode;

    __DEBUG (isns_list_debug &1, AddNode - list_id:%i,pstList->list_id);

    if(BOOL_FALSE == _list_IsInit(pstList))
    {
        __LOG_ERROR ("Add Node: List Head is not init, data size = %d", data_size);
        return ISNS_UNKNOWN_ERR;
    }

    if(NULL == pstList->pstHead)
    {
        pstList->pstHead = (DTQ_HEAD_S *)malloc(sizeof(DTQ_HEAD_S));
        if(NULL == pstList->pstHead)
        {
            return ISNS_UNKNOWN_ERR;
        }
        DTQ_Init(pstList->pstHead);
    }

    pstNode = (ISNS_LIST_NODE *)malloc(sizeof(ISNS_LIST_NODE));
    if(NULL == pstNode)
    {
        return ISNS_UNKNOWN_ERR;
    }

    memset(pstNode, 0, sizeof(ISNS_LIST_NODE));
    pstNode->data = (char *)malloc(data_size + 1);
    if(NULL == pstNode->data)
    {
        free(pstNode);
        return ISNS_UNKNOWN_ERR;
    }

    memcpy(pstNode->data, pdata, data_size);
    ((CHAR *)pstNode->data)[data_size] = 0;
    pstNode->data_size = data_size;
    DTQ_AddTail(pstList->pstHead, &pstNode->stNode);

    /* LDAP_ListAdd(list_id, p_entry, pdata, data_size)
       ���ʱ����ӵ��ڴ棬ɾ��ʱ���Ŵ��ڴ�ɾ�� */

    return ( SUCCESS );
}

/********************************************************************
Returns true if the list is empty.
********************************************************************/
int
IsEmptyList(ISNS_LIST *pstList)
{
    if(NULL == pstList->pstHead || DTQ_IsEmpty(pstList->pstHead))
    {
        return TRUE;
    }
    return FALSE;
}

/********************************************************************
Returns the next node in a list.
********************************************************************/
ISNS_LIST_NODE *
GetNextNode(ISNS_LIST *pstList, ISNS_LIST_NODE *pstNode)
{
    __DEBUG (isns_list_debug &1,GetNextNode list_id:%i, pstList->list_id);

    if(NULL == pstList->pstHead)
    {
        return NULL;
    }

    if(NULL == pstNode)
    {
        return DTQ_ENTRY_FIRST(pstList->pstHead, ISNS_LIST_NODE, stNode);
    }
    return DTQ_ENTRY_NEXT(pstList->pstHead, pstNode, stNode);
}


