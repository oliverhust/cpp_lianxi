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

#ifndef _ISNSDBMEM_H
#define _ISNSDBMEM_H


ULONG ISNS_MEM_Init(UINT uiMaxTypeCount);
VOID ISNS_MEM_Fini(VOID);

INT ISNS_MEM_Write(IN const ISNS_DBKey *pstDbKey, IN const SOIP_DB_Entry *pstEntry);
INT ISNS_MEM_Delete(IN const ISNS_DBKey *pstDbKey);
INT ISNS_MEM_Read(IN const ISNS_DBKey *pstDbKey, OUT SOIP_DB_Entry *pstEntry);
INT ISNS_MEM_NextKey(INOUT ISNS_DBKey *pstDbKey);
INT ISNS_MEM_Iter(INOUT ISNS_DBKey *pstDbKey, INOUT VOID **ppIter, OUT SOIP_DB_Entry *pstEntry);

INT ISNS_MEM_List_Init(IN INT iListId, IN VOID *pRecord);
BOOL_T ISNS_MEM_List_IsInit(IN const ISNS_LIST *pstList);
INT ISNS_MEM_List_Delete(IN ISNS_LIST *pstList);
VOID *ISNS_MEM_List_GetParent(IN ISNS_LIST *pstList);
INT ISNS_MEM_List_RemoveNode(IN ISNS_LIST *pstList, IN ISNS_LIST_NODE *pstNode);
VOID *ISNS_MEM_List_GetNodeData(IN ISNS_LIST_NODE *pstNode, OUT INT *piSize);
ISNS_LIST_NODE *ISNS_MEM_List_FindNode(IN ISNS_LIST *pstList, IN const CHAR *pdata, IN INT iDataSize);
INT ISNS_MEM_List_AddNode(IN ISNS_LIST *pstList, IN CHAR *pcData, IN INT iDataSize);
BOOL_T ISNS_MEM_List_IsEmpty(IN ISNS_LIST *pstList);
ISNS_LIST_NODE *ISNS_MEM_List_GetNext(IN ISNS_LIST *pstList, IN ISNS_LIST_NODE *pstNode,
                                      OUT const CHAR **ppcData, OUT INT *piSize);
ULONG ISNS_MEM_List_NextData(IN ISNS_LIST *pstListPPtr, INOUT ISNS_LIST_NODE **ppstNode,
                             INOUT VOID *pOutBuff, IN UINT uiBuffSize);



#endif

