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

#ifndef _ISNSNDBLDAP_H_
#define _ISNSNDBLDAP_H_


/* Modeled after ldap functions */

#define NDB_DIR_NAME_MAX_LEN        31
#define NDB_NON_KEY_MAX_LEN         63

#define NDB_ATTR_NUM_MAX            3
#define NDB_ATTR_KEY                "isnsKey"
#define NDB_ATTR_VALUE              "isnsValue"
#define NDB_OBJCLASS_DATA           "isnsData"

#define NDB_OBJCLASS                "objectclass"
#define NDB_OU_OBJCLASS             "organizationalUnit"
#define NDB_OBJ_OU                  "ou"

#define NDB_RECONNECT_LDAP_TIMES    63

/* 对某个目录的遍历的回调函数, 返回非0值就终止遍历 */
typedef int (*NDB_LDAP_SCAN_PF)(int iDirId, datum stKey, datum stValue, void *pSelfData);

int ndb_ldap_init(const NDB_INITIALIZE_S *pstNdbInit);
int ndb_ldap_reconnect();
void ndb_ldap_close();

int ndb_ldap_store_sns (int iDirId, datum stKey, datum stValue, int iFlag);
datum ndb_ldap_fetch (int iDirId, datum stKey);
datum ndb_ldap_fetch_sns (int iDirId, datum stKey, void *pDst);
int ndb_ldap_delete (int iDirId, datum stKey);
datum ndb_ldap_firstkey (int iDirId);
datum ndb_ldap_nextkey (int iDirId, datum stKey);
int ndb_ldap_scan_dir(int iDirId, NDB_LDAP_SCAN_PF pfCallback, void *pSelfData);




#endif

