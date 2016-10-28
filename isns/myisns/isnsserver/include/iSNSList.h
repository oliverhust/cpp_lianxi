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

#ifndef _ISNS_LIST_H
#define _ISNS_LIST_H
/* Implementation for iSNS Linked List structures */

#include <sys/basetype.h>
#include <sys/error.h>
#include <sys/list.h>

typedef struct _list_node
{
    DTQ_NODE_S stNode;
    void *data;
    uint32_t data_size;
} ISNS_LIST_NODE;

typedef struct _list
{
    DTQ_HEAD_S *pstHead;
    uint32_t list_id;
} ISNS_LIST;

ISNS_LIST_NODE *
GetNextNode(ISNS_LIST *plist, ISNS_LIST_NODE *pnode);

int
IsEmptyList(ISNS_LIST *plist);

int
AddNode(ISNS_LIST *plist, char *pdata, int data_size);

ISNS_LIST_NODE *
FindNode(ISNS_LIST *plist, char *pdata, int data_size);

void *
GetNodeData(ISNS_LIST_NODE *pnode);

int
RemoveNode(ISNS_LIST *plist, ISNS_LIST_NODE *pnode);

int
DeleteList(ISNS_LIST *plist);

int
InitList(int list_id, void * record);

#endif



