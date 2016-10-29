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
#include "iSNSdbMem.h"
#include "iSNSdbLdap.h"


/********************************************************************
Initializes a doubly linked list structure.  The linked list has a
dummy node at the head of the list.
********************************************************************/
int
InitList(int list_id, void * record)
{
    return ISNS_MEM_List_Init(list_id, record);
}

/********************************************************************
Deletes/DeInit a list.
********************************************************************/
int
DeleteList(ISNS_LIST *pstList)
{
    INT iRet = SUCCESS;
    VOID *pParent;

    if(BOOL_FALSE == ISNS_MEM_List_IsInit(pstList))
    {
        return ISNS_UNKNOWN_ERR;
    }

    pParent = ISNS_MEM_List_GetParent(pstList);

    /* 所有删除操作先从LDAP/DBM开始 */
    iRet |= ISNS_LDAP_List_Free(pstList->list_id, pParent);

    iRet |= ISNS_MEM_List_Free(pstList);

    return iRet;
}

/********************************************************************
Removes a node from a list.
********************************************************************/
int
RemoveNode(ISNS_LIST *pstList, ISNS_LIST_NODE *pstNode)
{
    INT iRet = SUCCESS;
    VOID *pParent;

    if(BOOL_FALSE == ISNS_MEM_List_IsInit(pstList))
    {
        return ISNS_UNKNOWN_ERR;
    }

    pParent = ISNS_MEM_List_GetParent(pstList);

    /* 删除操作先从LDAP/DBM开始 */
    iRet |= ISNS_LDAP_List_RemoveNode(pstList->list_id, pParent, pstNode->data, pstNode->data_size);

    iRet |= ISNS_MEM_List_RemoveNode(pstList, pstNode);

    return iRet;
}

/********************************************************************
Retrieves the data pointer from a node.
********************************************************************/
void *
GetNodeData(ISNS_LIST_NODE *pnode )
{
   return ISNS_MEM_List_GetNodeData(pnode, NULL);
}

/********************************************************************
Finds an object in the list.
********************************************************************/
ISNS_LIST_NODE *
FindNode(ISNS_LIST *pstList, char *pdata, int data_size)
{
    return ISNS_MEM_List_FindNode(pstList, pdata, data_size);
}

/********************************************************************
Adds a node to a list.
********************************************************************/
int
AddNode(ISNS_LIST *pstList, char *pdata, int data_size)
{
    INT iRet;
    const VOID *pParent;

    iRet = ISNS_MEM_List_AddNode(pstList, pdata, data_size);
    if(SUCCESS != iRet)
    {
        return iRet;
    }

    pParent = ISNS_MEM_List_GetParent(pstList);

    /* 添加时先添加到内存，删除时最后才从内存删除 */
    iRet = ISNS_LDAP_List_AddNode(pstList->list_id, pParent, pdata, data_size);

    return iRet;
}

/********************************************************************
Returns true if the list is empty.
********************************************************************/
int
IsEmptyList(ISNS_LIST *pstList)
{
    return ISNS_MEM_List_IsEmpty(pstList);
}

/********************************************************************
Returns the next node in a list.
********************************************************************/
ISNS_LIST_NODE *
GetNextNode(ISNS_LIST *pstList, ISNS_LIST_NODE *pstNode)
{
    return ISNS_MEM_List_GetNext(pstList, pstNode, NULL, NULL);
}


