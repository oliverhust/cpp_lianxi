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
 * This file contains source code for implementing the
 * main processing loop for the SoIP service task.
 *
 */

#ifndef _ISNSNDB_H_
#define _ISNSNDB_H_


/* Modeled after ldap functions */

/* 持久化数据的压缩级别，0为不压缩 */
#define NDB_DATA_COMPRESS_LEVEL             2

#define NDB_SUCCESS                         0
#define NDB_FAILED                          (-1)

typedef struct
{
    char  *dptr;
    int   dsize;
} datum;

/* 数据在传入/传出时不可修改/释放 */
typedef struct
{
    const char  *dptr;
    int   dsize;
}datum_s;

typedef struct
{
    char *pcLdapUrl;
    char *pcAdminDn;
    char *pcPassword;
    char *pcBase;
    int iDirMaxCount;
} NDB_INITIALIZE_S;

typedef int NDB_FILE;

enum
{
    NDBM_INSERT = 0,
    NDBM_REPLACE = 1,
};

extern int ndbm_errno;

int ndb_init(const NDB_INITIALIZE_S *pstNdbInit);
void ndb_close ();

int ndb_store_sns (int iDirId, datum stKey, datum stValue, int iFlag);
datum ndb_fetch (int iDirId, datum stKey);
datum ndb_fetch_sns (int iDirId, datum stKey, void *pDst);
int ndb_delete (int iDirId, datum stKey);
datum ndb_firstkey (int iDirId);
datum ndb_nextkey (int iDirId, datum stKey);


#endif

