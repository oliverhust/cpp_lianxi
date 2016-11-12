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

#ifndef _ISNSLDAPLIB_H
#define _ISNSLDAPLIB_H


/* 一次最多配置的属性个数+1 */
#define ISNS_LDAP_ATTR_MAX_SIZE             16

/* DN的最大长度+1 */
#define ISNS_LDAP_DN_MAX_SIZE               1024

/* 启动时尝试连接的次数与时间间隔 */
#define ISNS_LDAP_INIT_TRY_MAX              64
#define ISNS_LDAP_INIT_TRY_INTERVAL         2

#define ISNS_LDAP_OBJCLASS                  "objectClass"
#define ISNS_LDAP_OU_OBJCLASS               "organizationalUnit"
#define ISNS_LDAP_OBJ_OU                    "ou"


/* 对某个目录的遍历的回调函数, 返回非0值就终止遍历 */
typedef ULONG (*ISNS_LDAP_SCAN_PF)(IN const CHAR *const *ppcNameList,
                                   IN CHAR **const *pppcValuesList,
                                   INOUT VOID *pSelfData);

VOID ISNS_LDAP_Num2BitStr(IN UINT32 uiNum, OUT CHAR *pcOut);

CHAR *ISNS_LDAP_EscapeRdn(IN const CHAR *pcAttrName, IN const CHAR *pcAttrValue);

ULONG ISNS_LDAP_SetStringAttr(IN const CHAR *pcAttrName, IN const CHAR *pcAttrValue, OUT LDAPMod **ppstAttr);

ULONG ISNS_LDAP_AddStringAttr(IN const CHAR *pcAttrName, IN const CHAR *pcAttrValue, OUT LDAPMod **ppstAttr);

ULONG ISNS_LDAP_DelStringAttr(IN const CHAR *pcAttrName, IN const CHAR *pcAttrValue, OUT LDAPMod **ppstAttr);

LDAPMod **ISNS_LDAP_NewSingleAttrs(IN const CHAR *const *ppcNameList,
                                   IN const CHAR *const *ppcValueList);

ULONG ISNS_LDAP_MultValueAdd(IN const CHAR *pcDn, IN const CHAR *pcAttrName, IN const CHAR *pcValue);

ULONG ISNS_LDAP_MultValueDel(IN const CHAR *pcDn, IN const CHAR *pcAttrName, IN const CHAR *pcValue);

ULONG ISNS_LDAP_MultValueDelAll(IN const CHAR *pcDn, IN const CHAR *pcAttrName);

LDAPMod **ISNS_LDAP_NewAttrs(VOID);

VOID ISNS_LDAP_FreeAttrs(IN LDAPMod ***pppstAttrs);

ULONG ISNS_LDAP_ServerInit(IN const ISNS_LDAP_INIT_S *pstInit,
                           IN const CHAR *const *ppcPreDirs);

VOID ISNS_LDAP_ServerFini(VOID);

BOOL_T ISNS_LDAP_IsDnExist(IN const CHAR *pcDn, OUT ULONG *pulErr);

ULONG ISNS_LDAP_AddEntry(IN const CHAR *pcDn, IN LDAPMod **ppstAttrs);

ULONG ISNS_LDAP_ModifyEntry(IN const CHAR *pcDn, IN LDAPMod **ppstAttrs);

ULONG ISNS_LDAP_ReplaceEntry(IN const CHAR *pcDn, IN LDAPMod **ppstAttrs);

ULONG ISNS_LDAP_DelEntry(IN const CHAR *pcDn);

ULONG ISNS_LDAP_SearchEntry(
    IN const CHAR *pcBase,
    IN INT iScope,
    IN const CHAR *pcFilter,
    IN CHAR **ppcAttrs,
    IN INT iAttrsonly,
    OUT LDAPMessage **ppstRes,
    OUT LDAP **ppstLdap);

ULONG ISNS_LDAP_ScanDir(IN const CHAR *pcDirDn, IN const CHAR *const *ppcNameList,
                        IN ISNS_LDAP_SCAN_PF pfCallback, INOUT VOID *pSelfData);






#endif

