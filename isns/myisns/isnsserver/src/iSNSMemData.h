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


#ifndef _ISNSMEMDATA_H
#define _ISNSMEMDATA_H


typedef struct tagIsnsMemHead
{
    DTQ_HEAD_S stHead;
    UINT uiSubListCount;
}ISNS_MEMD_HEAD_S;


/* Modeled after DTQ */
ULONG ISNS_MEMDATA_Init(UINT uiMaxTypeCount);
datum_s ISNS_MEMDATA_Read(IN ISNS_MEMD_HEAD_S *pstMHead, IN datum_s stKey);
ULONG ISNS_MEMDATA_Write(IN ISNS_MEMD_HEAD_S *pstMHead, IN datum_s stKey, IN datum_s stValue);
ULONG ISNS_MEMDATA_Delete(IN ISNS_MEMD_HEAD_S *pstMHead, IN datum_s stKey);
datum_s ISNS_MEMDATA_GetNext(IN ISNS_MEMD_HEAD_S *pstMHead, IN datum_s stKey, OUT datum_s *pstNextValue);
ULONG ISNS_MEMDATA_Iter(IN ISNS_MEMD_HEAD_S *pstMHead, INOUT VOID **ppIter,
                        OUT datum_s *pstNextKey, OUT datum_s *pstNextValue);





#endif


