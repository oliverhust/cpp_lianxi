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

#ifndef _SNSEpoll_h_
#define _SNSEpoll_h_

INT ISCSI_ISNS_EpollHandleGet(VOID);

VOID ISCSI_ISNS_EpollHandleSet(IN INT iEpollHandle);

VOID ISCSI_ISNS_EpollHandleSet(IN INT iEpollHandle);

INT ISCSI_ISNS_MainEpollHandleGet(VOID);

ULONG ISCSI_ISNS_EpollInit(VOID);

VOID ISCSI_ISNS_EpollDeInit(VOID);

ISCSI_RECV_LIST_S *ISCSI_ISNS_RecvFdListGet(VOID);

ULONG ISCSI_ISNS_MainEpollInit(VOID);

VOID ISCSI_ISNS_MainEpollDeInit(VOID);

#endif

