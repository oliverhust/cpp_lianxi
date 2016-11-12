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

#ifndef _ISNSDBLDAP_H
#define _ISNSDBLDAP_H

#define ISNS_LDAP_BIT_STR_SIZE                  64
#define ISNS_LDAP_IPADDR_SIZE                   46

#define ISNS_LDAP_OBJCLASS_DD                   "DiscoveryDomain"
#define ISNS_LDAP_OBJCLASS_DDS                  "DiscoveryDomainSet"
#define ISNS_LDAP_OBJCLASS_ISCSI                "StorageNode"
#define ISNS_LDAP_OBJCLASS_ENTITY               "NetworkEntity"
#define ISNS_LDAP_OBJCLASS_PORTAL               "Portal"
#define ISNS_LDAP_OBJCLASS_PG                   "PortalGroup"


enum
{
    ISNS_LDAP_DIR_DD = 0,
    ISNS_LDAP_DIR_DDS,
    ISNS_LDAP_DIR_ISCSI,
    ISNS_LDAP_DIR_ENTITY,
    ISNS_LDAP_DIR_PORTAL,
    ISNS_LDAP_DIR_PG,
    ISNS_LDAP_DIR_OBJ_MAX
};

enum
{
    ISNS_LDAP_ORD_DD_OBJCLASS = 0,
    ISNS_LDAP_ORD_DD_ID,
    ISNS_LDAP_ORD_DD_SYMNAME,
    ISNS_LDAP_ORD_DD_FEATURE,
    ISNS_LDAP_ORD_DD_DDSMAP,
    ISNS_LDAP_ORD_DD_MEMBER,
    ISNS_LDAP_ORD_DD_PORTAL,
    ISNS_LDAP_ORD_DD_MAX
};

enum
{
    ISNS_LDAP_ORD_DDS_OBJCLASS = 0,
    ISNS_LDAP_ORD_DDS_ID,
    ISNS_LDAP_ORD_DDS_SYMNAME,
    ISNS_LDAP_ORD_DDS_STATUS,
    ISNS_LDAP_ORD_DDS_MEMBER,
    ISNS_LDAP_ORD_DDS_MAX
};

enum
{
    ISNS_LDAP_ORD_ISCSI_OBJCLASS = 0,
    ISNS_LDAP_ORD_ISCSI_NAME,
    ISNS_LDAP_ORD_ISCSI_TYPE,
    ISNS_LDAP_ORD_ISCSI_ALIAS,
    ISNS_LDAP_ORD_ISCSI_INDEX,
    ISNS_LDAP_ORD_ISCSI_SCNMAP,
    ISNS_LDAP_ORD_ISCSI_DDMAP,
    ISNS_LDAP_ORD_ISCSI_ENTITYID,
    ISNS_LDAP_ORD_ISCSI_MAX
};

enum
{
    ISNS_LDAP_ORD_ENTITY_OBJCLASS = 0,
    ISNS_LDAP_ORD_ENTITY_ID,
    ISNS_LDAP_ORD_ENTITY_INDEX,
    ISNS_LDAP_ORD_ENTITY_PROTOCOL,
    ISNS_LDAP_ORD_ENTITY_MIPADDR,
    ISNS_LDAP_ORD_ENTITY_TIMESTAMP,
    ISNS_LDAP_ORD_ENTITY_PRTLMAX,
    ISNS_LDAP_ORD_ENTITY_PRTLMIN,
    ISNS_LDAP_ORD_ENTITY_REGPERIOD,
    ISNS_LDAP_ORD_ENTITY_PORTALMEM,
    ISNS_LDAP_ORD_ENTITY_NODEMEM,
    ISNS_LDAP_ORD_ENTITY_MAX
};

enum
{
    ISNS_LDAP_ORD_PORTAL_OBJCLASS = 0,
    ISNS_LDAP_ORD_PORTAL_KEY,
    ISNS_LDAP_ORD_PORTAL_SYMNAME,
    ISNS_LDAP_ORD_PORTAL_ESINTVAL,
    ISNS_LDAP_ORD_PORTAL_ESIPORT,
    ISNS_LDAP_ORD_PORTAL_INDEX,
    ISNS_LDAP_ORD_PORTAL_SCNPORT,
    ISNS_LDAP_ORD_PORTAL_SECURBITMAP,
    ISNS_LDAP_ORD_PORTAL_ENTITYID,
    ISNS_LDAP_ORD_PORTAL_MAX
};

enum
{
    ISNS_LDAP_ORD_PG_OBJCLASS = 0,
    ISNS_LDAP_ORD_PG_KEY,
    ISNS_LDAP_ORD_PG_PGT,
    ISNS_LDAP_ORD_PG_INDEX,
    ISNS_LDAP_ORD_PG_MAX
};


typedef struct {
    CHAR *pcLdapUrl;
    CHAR *pcAdminDn;
    CHAR *pcPassword;
    CHAR *pcBase;
} ISNS_LDAP_INIT_S;


INT ISNS_LDAP_Write(IN const ISNS_DBKey *pstDbKey, IN const SOIP_DB_Entry *pstEntry);
INT ISNS_LDAP_Delete(IN const ISNS_DBKey *pstDbKey);

INT ISNS_LDAP_List_AddNode(IN UINT uiListType, IN const VOID *pParent,
                           IN const CHAR *pcData, IN INT iSize);
INT ISNS_LDAP_List_RemoveNode(IN UINT uiListType, IN const VOID *pParent,
                              IN const CHAR *pcData, IN INT iSize);
INT ISNS_LDAP_List_Delete(IN UINT uiListType, IN const VOID *pParent);

ULONG ISNS_LDAP_Init(IN const ISNS_LDAP_INIT_S *pstInit);
VOID ISNS_LDAP_Fini(VOID);


ULONG ISNS_LDAP_AddDD(IN const SOIP_Dd *pstCfg);
ULONG ISNS_LDAP_DelDD(IN UINT32 uiDDId);
ULONG ISNS_LDAP_AddDDS(IN const SOIP_Dds *pstCfg);
ULONG ISNS_LDAP_DelDDS(IN UINT32 uiDDSId);
ULONG ISNS_LDAP_AddIscsi(IN const SOIP_Iscsi *pstCfg);
ULONG ISNS_LDAP_DelIscsi(IN const CHAR *pcIscsiName);
ULONG ISNS_LDAP_AddEntity(IN const SOIP_Entity *pstCfg);
ULONG ISNS_LDAP_DelEntity(IN const CHAR *pcEntityID);
ULONG ISNS_LDAP_AddPortal(IN const SOIP_Portal *pstCfg);
ULONG ISNS_LDAP_DelPortal(IN const SOIP_Portal_Key *pstPotralKey);
ULONG ISNS_LDAP_AddPG(IN const SOIP_Portal_Group *pstCfg);
ULONG ISNS_LDAP_DelPG(IN const SOIP_Portal_Group_Key *pstKey);

ULONG ISNS_LDAP_AddDDIscsi(IN UINT32 uiDDId, IN const CHAR *pcIscsiName, IN UINT uiIscsiIndex);
ULONG ISNS_LDAP_DelDDIscsi(IN UINT32 uiDDId, IN const CHAR *pcIscsiName, IN UINT uiIscsiIndex);
ULONG ISNS_LDAP_DelAllDDIscsi(IN UINT32 uiDDId);
ULONG ISNS_LDAP_AddDdDdsMap(IN UINT32 uiDDId, IN UINT32 uiDDSId);
ULONG ISNS_LDAP_DelDdDdsMap(IN UINT32 uiDDId, IN UINT32 uiDDSId);
ULONG ISNS_LDAP_DelAllDdDdsMap(IN UINT32 uiDDId);



#endif

