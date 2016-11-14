/*****************************************************************************
 Copyright (c) 2010, Hangzhou H3C Technologies Co., Ltd. All rights reserved.
------------------------------------------------------------------------------
                               iSNSNdbLdap.c

  Project Code: Comware V7
   Module Name: ISNS
  Date Created: 2016-10-29
        Author: liangjinchao@dian
   Description: 对LDAP数据进行操作，各种数据的查找、添加、删除、遍历操作

--------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
--------------------------------------------------------------------------------

*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/basetype.h>
#include <sys/error.h>
#include <sys/assert.h>
#include <sys/list.h>

#ifndef LDAP_DEPRECATED
#define LDAP_DEPRECATED 1
#endif
#include <openldap/ldap.h>
//#include <ldap.h>

#include "iSNStypes.h"
#include "iSNSList.h"
#include "iSNSmsg.h"
#include "iSNSdebug.h"
#include "iSNSNdb.h"
#include "iSNSdb.h"
#include "iSNSdbLdap.h"
#include "iSNSLdapLib.h"

/* RDN属性名所在数组下标为1(如果两个属性组成RDN则为1和2) */

/* DD 的属性名数组，最后一个字符串指针要为NULL */
STATIC const CHAR * const g_apcAttrsDD[ISNS_LDAP_ORD_DD_MAX + 1] =
{
    [ISNS_LDAP_ORD_DD_OBJCLASS] = ISNS_LDAP_OBJCLASS,
    [ISNS_LDAP_ORD_DD_ID] = "DDID",
    [ISNS_LDAP_ORD_DD_SYMNAME] = "DDSymbolicname",
    [ISNS_LDAP_ORD_DD_FEATURE] = "Feature",
    [ISNS_LDAP_ORD_DD_DDSMAP] = "DDSMap",
    [ISNS_LDAP_ORD_DD_MEMBER] = "DDMember",
    [ISNS_LDAP_ORD_DD_PORTAL] = "PortalIPAddr",
    [ISNS_LDAP_ORD_DD_MAX] = NULL
};

/* DDS 的属性名数组 */
STATIC const CHAR * const g_apcAttrsDDS[ISNS_LDAP_ORD_DDS_MAX + 1] =
{
    [ISNS_LDAP_ORD_DDS_OBJCLASS] = ISNS_LDAP_OBJCLASS,
    [ISNS_LDAP_ORD_DDS_ID] = "DDSID",
    [ISNS_LDAP_ORD_DDS_SYMNAME] = "DDSSymbolicName",
    [ISNS_LDAP_ORD_DDS_STATUS] = "DDSStatus",
    [ISNS_LDAP_ORD_DDS_MEMBER] = "DDSMember",
    [ISNS_LDAP_ORD_DDS_MAX] = NULL
};

/* ISCSI 的属性名数组 */
STATIC const CHAR * const g_apcAttrsIscsi[ISNS_LDAP_ORD_ISCSI_MAX + 1] =
{
    [ISNS_LDAP_ORD_ISCSI_OBJCLASS] = ISNS_LDAP_OBJCLASS,
    [ISNS_LDAP_ORD_ISCSI_NAME] = "NodeName",
    [ISNS_LDAP_ORD_ISCSI_TYPE] = "NodeType",
    [ISNS_LDAP_ORD_ISCSI_ALIAS] = "NodeAlias",
    [ISNS_LDAP_ORD_ISCSI_INDEX] = "NodeIndex",
    [ISNS_LDAP_ORD_ISCSI_SCNMAP] = "SCNBitmap",
    [ISNS_LDAP_ORD_ISCSI_DDMAP] = "DDMap",
    [ISNS_LDAP_ORD_ISCSI_ENTITYID] = "EntityID",
    [ISNS_LDAP_ORD_ISCSI_MAX] = NULL
};

/* ENTITY 的属性名数组 */
STATIC const CHAR * const g_apcAttrsEntity[ISNS_LDAP_ORD_ENTITY_MAX + 1] =
{
    [ISNS_LDAP_ORD_ENTITY_OBJCLASS] = ISNS_LDAP_OBJCLASS,
    [ISNS_LDAP_ORD_ENTITY_ID] = "EID",
    [ISNS_LDAP_ORD_ENTITY_INDEX] = "EntityIndex",
    [ISNS_LDAP_ORD_ENTITY_PROTOCOL] = "EntityProtocol",
    [ISNS_LDAP_ORD_ENTITY_MIPADDR] = "MgtIPAddr",
    [ISNS_LDAP_ORD_ENTITY_TIMESTAMP] = "Timestamp",
    [ISNS_LDAP_ORD_ENTITY_PRTLMAX] = "ProtocolVerMax",
    [ISNS_LDAP_ORD_ENTITY_PRTLMIN] = "ProtocolVerMin",
    [ISNS_LDAP_ORD_ENTITY_REGPERIOD] = "RegPeriod",
    [ISNS_LDAP_ORD_ENTITY_PORTALMEM] = "PortalMember",
    [ISNS_LDAP_ORD_ENTITY_NODEMEM] = "NodeMember",
    [ISNS_LDAP_ORD_ENTITY_MAX] = NULL
};

/* PORTAL 的属性名数组 */
STATIC const CHAR * const g_apcAttrsPortal[ISNS_LDAP_ORD_PORTAL_MAX + 1] =
{
    [ISNS_LDAP_ORD_PORTAL_OBJCLASS] = ISNS_LDAP_OBJCLASS,
    [ISNS_LDAP_ORD_PORTAL_KEY] = "PortalKey",
    [ISNS_LDAP_ORD_PORTAL_SYMNAME] = "PortalSymbolicName",
    [ISNS_LDAP_ORD_PORTAL_ESINTVAL] = "ESIInterval",
    [ISNS_LDAP_ORD_PORTAL_ESIPORT] = "ESIPort",
    [ISNS_LDAP_ORD_PORTAL_INDEX] = "PortalIndex",
    [ISNS_LDAP_ORD_PORTAL_SCNPORT] = "SCNPort",
    [ISNS_LDAP_ORD_PORTAL_SECURBITMAP] = "PortalSecurityBitmap",
    [ISNS_LDAP_ORD_PORTAL_ENTITYID] = "EntityID",
    [ISNS_LDAP_ORD_PORTAL_MAX] = NULL
};

/* PG 的属性名数组，最后一个字符串指针要为NULL */
STATIC const CHAR * const g_apcAttrsPG[ISNS_LDAP_ORD_PG_MAX + 1] =
{
    [ISNS_LDAP_ORD_PG_OBJCLASS] = ISNS_LDAP_OBJCLASS,
    [ISNS_LDAP_ORD_PG_KEY] = "PGKey",
    [ISNS_LDAP_ORD_PG_PGT] = "PGT",
    [ISNS_LDAP_ORD_PG_INDEX] = "PGIndex",
    [ISNS_LDAP_ORD_PG_MAX] = NULL
};

/* 目录名 最后一个字符串指针要为NULL */
STATIC const CHAR * const g_apcIsnsLdapDir[ISNS_LDAP_DIR_OBJ_MAX + 1] =
{
    [ISNS_LDAP_DIR_DD] = ISNS_LDAP_OBJCLASS_DD,
    [ISNS_LDAP_DIR_DDS] = ISNS_LDAP_OBJCLASS_DDS,
    [ISNS_LDAP_DIR_ISCSI] = ISNS_LDAP_OBJCLASS_ISCSI,
    [ISNS_LDAP_DIR_ENTITY] = ISNS_LDAP_OBJCLASS_ENTITY,
    [ISNS_LDAP_DIR_PORTAL] = ISNS_LDAP_OBJCLASS_PORTAL,
    [ISNS_LDAP_DIR_PG] = ISNS_LDAP_OBJCLASS_PG,
    [ISNS_LDAP_DIR_OBJ_MAX] = NULL
};

/* 所有的属性汇总 */
STATIC const CHAR * const * const g_appcAllAttrs[ISNS_LDAP_DIR_OBJ_MAX + 1] =
{
    [ISNS_LDAP_DIR_DD] = g_apcAttrsDD,
    [ISNS_LDAP_DIR_DDS] = g_apcAttrsDDS,
    [ISNS_LDAP_DIR_ISCSI] = g_apcAttrsIscsi,
    [ISNS_LDAP_DIR_ENTITY] = g_apcAttrsEntity,
    [ISNS_LDAP_DIR_PORTAL] = g_apcAttrsPortal,
    [ISNS_LDAP_DIR_PG] = g_apcAttrsPG,
    [ISNS_LDAP_DIR_OBJ_MAX] = NULL
};

#define ISNS_LDAP_FMT_PORTAL_KEY        "IP=%s,Port=%d"
#define ISNS_LDAP_FMT_PG                "IP=%s,Port=%d,Node=%s"


/*********************************************************************
     Func Name : _ldap_IPAddr2Str
  Date Created : 2016/10/29
        Author : liangjinchao@dian
   Description : IP地址转字符串
         Input : IN const IP_Address *pstIPAddr
                 IN UINT uiSize
        Output : OUT CHAR *pcOut
        Return : pcOut
       Caution :
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
STATIC INLINE const CHAR *_ldap_IPAddr2Str(IN const IP_Address *pstIPAddr,
                                           OUT CHAR *pcOut, IN UINT uiSize)
{
    return inet_ntop(AF_INET6, pstIPAddr->v, pcOut, uiSize);
}

/*********************************************************************
     Func Name : _ldap_Str2IPAddr
  Date Created : 2016/10/29
        Author : liangjinchao@dian
   Description : 字符串转IP地址
         Input : IN const CHAR *pcIn
        Output : OUT IP_Address *pstIPAddr
        Return : 成功/失败
       Caution :
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
STATIC INLINE ULONG _ldap_Str2IPAddr(IN const CHAR *pcIn, OUT IP_Address *pstIPAddr)
{
    memset(pstIPAddr, 0, sizeof(IP_Address));
    if(1 != inet_pton(AF_INET6, pcIn, pstIPAddr->v))
    {
        return ERROR_FAILED;
    }

    return ERROR_SUCCESS;
}

/*********************************************************************
     Func Name : _ldap_NKeyFmt
  Date Created : 2016/10/29
        Author : liangjinchao@dian
   Description : 多个值合为一个
         Input : IN const IP_Address *pstIPAddr,
                 IN UINT uiPort,
                 IN const CHAR *pcName
                 IN UINT uiSize
        Output : OUT CHAR *pcOut
        Return : 无
       Caution :
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
STATIC VOID _ldap_NKeyFmt(IN const IP_Address *pstIPAddr, IN UINT uiPort, IN const CHAR *pcName,
                          OUT CHAR *pcOut, IN UINT uiSize)
{
    CHAR szStrIP[ISNS_LDAP_IPADDR_SIZE];

    _ldap_IPAddr2Str(pstIPAddr, szStrIP, sizeof(szStrIP));

    if(NULL != pcName && '\0' != pcName[0])
    {
        scnprintf(pcOut, uiSize, ISNS_LDAP_FMT_PG, szStrIP, uiPort, pcName);
    }
    else
    {
        scnprintf(pcOut, uiSize, ISNS_LDAP_FMT_PORTAL_KEY, szStrIP, uiPort);
    }

    return;
}

/*********************************************************************
     Func Name : _ldap_FillDDDn
  Date Created : 2016/10/29
        Author : liangjinchao@dian
   Description : 构造DN
         Input : IN UINT uiDirId, IN const CHAR *pcKey
        Output : OUT CHAR *pcOut, IN UINT uiSize
        Return : 无
       Caution : key为空则为获取目录的DN
                 失败则写空字符串
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
STATIC VOID _ldap_FillDn(IN UINT uiDirId, IN const CHAR *pcKey,
                         OUT CHAR *pcOut, IN UINT uiSize)
{
    CHAR *pcRdn;

    *pcOut = '\0';
    if(uiDirId >= ISNS_LDAP_DIR_OBJ_MAX)
    {
        return ;
    }

    if(NULL == pcKey)
    {
        scnprintf(pcOut, uiSize, ISNS_LDAP_OBJ_OU"=%s,%s",
                  g_apcIsnsLdapDir[uiDirId], ISNS_LDAP_ISNS_BASE);
        return ;
    }

    pcRdn = ISNS_LDAP_EscapeRdn(g_appcAllAttrs[uiDirId][1], pcKey);
    if(NULL == pcRdn)
    {
        return ;
    }

    scnprintf(pcOut, uiSize, "%s,"ISNS_LDAP_OBJ_OU"=%s,%s",
              pcRdn, g_apcIsnsLdapDir[uiDirId], ISNS_LDAP_ISNS_BASE);

    free(pcRdn);
    return ;
}

/*********************************************************************
     Func Name : _ldap_AddVoidDDIfNeed
  Date Created : 2016/10/29
        Author : liangjinchao@dian
   Description : 如果DN不存在则添加一个初始化为0的DD
         Input : IN UINT32 uiDDId, IN const CHAR *pcDn
        Output : 无
        Return : 成功/失败
       Caution :
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
STATIC ULONG _ldap_AddVoidDDIfNeed(IN UINT32 uiDDId, IN const CHAR *pcDn)
{
    SOIP_Dd stDD;
    ULONG ulRet;
    BOOL_T bIsExist;

    bIsExist = ISNS_LDAP_IsDnExist(pcDn, &ulRet);
    if(ERROR_SUCCESS != ulRet)
    {
        return ulRet;
    }

    if(BOOL_FALSE == bIsExist)
    {
        memset(&stDD, 0, sizeof(stDD));
        stDD.id = uiDDId;
        ulRet = ISNS_LDAP_AddDD(&stDD);
    }

    return ulRet;
}

/*********************************************************************
     Func Name : ISNS_LDAP_AddDD
  Date Created : 2016/10/29
        Author : liangjinchao@dian
   Description : 添加DD
         Input : IN const SOIP_Dd *pstCfg
        Output : 无
        Return : 成功/失败
       Caution : 不存DD_Mem,DDSMAP
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
ULONG ISNS_LDAP_AddDD(IN const SOIP_Dd *pstCfg)
{
    const CHAR *apcValue[ISNS_LDAP_ATTR_MAX_SIZE] = { 0 };
    CHAR szDn[ISNS_LDAP_DN_MAX_SIZE];
    LDAPMod **ppstAttrs;
    ULONG ulRet;

    CHAR szId[ISNS_NUM_STR_MAX_SIZE];
    CHAR szFeature[ISNS_LDAP_BIT_STR_SIZE];

    scnprintf(szId, sizeof(szId), "%u", pstCfg->id);
    ISNS_LDAP_Num2BitStr(pstCfg->feature , szFeature);

    apcValue[ISNS_LDAP_ORD_DD_OBJCLASS] = ISNS_LDAP_OBJCLASS_DD;
    apcValue[ISNS_LDAP_ORD_DD_ID] = szId;
    apcValue[ISNS_LDAP_ORD_DD_SYMNAME] = pstCfg->sym_name;
    apcValue[ISNS_LDAP_ORD_DD_FEATURE] = szFeature;

    ppstAttrs = ISNS_LDAP_NewSingleAttrs(g_apcAttrsDD, apcValue);
    if(NULL == ppstAttrs)
    {
        return ERROR_FAILED;
    }

    _ldap_FillDn(ISNS_LDAP_DIR_DD, szId, szDn, sizeof(szDn));

    ulRet = ISNS_LDAP_ReplaceEntry(szDn, ppstAttrs);
    ISNS_LDAP_FreeAttrs(&ppstAttrs);

    return ulRet;
}

/*********************************************************************
     Func Name : ISNS_LDAP_DelDD
  Date Created : 2016/10/29
        Author : liangjinchao@dian
   Description : 删除DD
         Input : IN UINT32 uiDDId
        Output : 无
        Return : 成功/失败
       Caution :
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
ULONG ISNS_LDAP_DelDD(IN UINT32 uiDDId)
{
    CHAR szDn[ISNS_LDAP_DN_MAX_SIZE];
    CHAR szId[ISNS_NUM_STR_MAX_SIZE];
    ULONG ulRet;

    scnprintf(szId, sizeof(szId), "%u", uiDDId);
    _ldap_FillDn(ISNS_LDAP_DIR_DD, szId, szDn, sizeof(szDn));

    ulRet = ISNS_LDAP_DelEntry(szDn);

    return ulRet;
}

/*********************************************************************
     Func Name : ISNS_LDAP_AddDDS
  Date Created : 2016/10/29
        Author : liangjinchao@dian
   Description : 添加DDS
         Input : IN const SOIP_Dds *pstCfg
        Output : 无
        Return : 成功/失败
       Caution :
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
ULONG ISNS_LDAP_AddDDS(IN const SOIP_Dds *pstCfg)
{
    const CHAR *apcValue[ISNS_LDAP_ATTR_MAX_SIZE] = { 0 };
    CHAR szDn[ISNS_LDAP_DN_MAX_SIZE];
    LDAPMod **ppstAttrs;
    ULONG ulRet;

    CHAR szId[ISNS_NUM_STR_MAX_SIZE];
    CHAR szStatus[ISNS_LDAP_BIT_STR_SIZE];

    scnprintf(szId, sizeof(szId), "%u", pstCfg->id);
    ISNS_LDAP_Num2BitStr(pstCfg->status, szStatus);

    apcValue[ISNS_LDAP_ORD_DDS_OBJCLASS] = ISNS_LDAP_OBJCLASS_DDS;
    apcValue[ISNS_LDAP_ORD_DDS_ID] = szId;
    apcValue[ISNS_LDAP_ORD_DDS_SYMNAME] = pstCfg->sym_name;
    apcValue[ISNS_LDAP_ORD_DDS_STATUS] = szStatus;

    ppstAttrs = ISNS_LDAP_NewSingleAttrs(g_apcAttrsDDS, apcValue);
    if(NULL == ppstAttrs)
    {
        return ERROR_FAILED;
    }

    _ldap_FillDn(ISNS_LDAP_DIR_DDS, szId, szDn, sizeof(szDn));

    ulRet = ISNS_LDAP_ReplaceEntry(szDn, ppstAttrs);
    ISNS_LDAP_FreeAttrs(&ppstAttrs);

    return ulRet;
}

/*********************************************************************
     Func Name : ISNS_LDAP_DelDDS
  Date Created : 2016/10/29
        Author : liangjinchao@dian
   Description : 删除DDS
         Input : IN UINT32 uiDDSId
        Output : 无
        Return : 成功/失败
       Caution :
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
ULONG ISNS_LDAP_DelDDS(IN UINT32 uiDDSId)
{
    CHAR szDn[ISNS_LDAP_DN_MAX_SIZE];
    CHAR szId[ISNS_NUM_STR_MAX_SIZE];
    ULONG ulRet;

    scnprintf(szId, sizeof(szId), "%u", uiDDSId);
    _ldap_FillDn(ISNS_LDAP_DIR_DDS, szId, szDn, sizeof(szDn));

    ulRet = ISNS_LDAP_DelEntry(szDn);

    return ulRet;
}

/*********************************************************************
     Func Name : ISNS_LDAP_AddIscsi
  Date Created : 2016/10/29
        Author : liangjinchao@dian
   Description : 添加ISCSI
         Input : IN const SOIP_Iscsi *pstCfg
        Output : 无
        Return : 成功/失败
       Caution :
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
ULONG ISNS_LDAP_AddIscsi(IN const SOIP_Iscsi *pstCfg)
{
    const CHAR *apcValue[ISNS_LDAP_ATTR_MAX_SIZE] = { 0 };
    CHAR szDn[ISNS_LDAP_DN_MAX_SIZE];
    LDAPMod **ppstAttrs;
    ULONG ulRet;

    CHAR szType[ISNS_LDAP_BIT_STR_SIZE];
    CHAR szIndex[ISNS_NUM_STR_MAX_SIZE];
    CHAR szScnMap[ISNS_LDAP_BIT_STR_SIZE];

    ISNS_LDAP_Num2BitStr(pstCfg->type, szType);
    scnprintf(szIndex, sizeof(szIndex), "%u", pstCfg->iscsi_index);
    ISNS_LDAP_Num2BitStr(pstCfg->scn_bitmap, szScnMap);

    apcValue[ISNS_LDAP_ORD_ISCSI_OBJCLASS] = ISNS_LDAP_OBJCLASS_ISCSI;
    apcValue[ISNS_LDAP_ORD_ISCSI_NAME] = pstCfg->id.v;
    apcValue[ISNS_LDAP_ORD_ISCSI_TYPE] = szType;
    apcValue[ISNS_LDAP_ORD_ISCSI_ALIAS] = pstCfg->alias;
    apcValue[ISNS_LDAP_ORD_ISCSI_INDEX] = szIndex;
    apcValue[ISNS_LDAP_ORD_ISCSI_SCNMAP] = szScnMap;
    apcValue[ISNS_LDAP_ORD_ISCSI_ENTITYID] = pstCfg->entity_id.id;

    ppstAttrs = ISNS_LDAP_NewSingleAttrs(g_apcAttrsIscsi, apcValue);
    if(NULL == ppstAttrs)
    {
        return ERROR_FAILED;
    }

    _ldap_FillDn(ISNS_LDAP_DIR_ISCSI, pstCfg->id.v, szDn, sizeof(szDn));

    ulRet = ISNS_LDAP_ReplaceEntry(szDn, ppstAttrs);
    ISNS_LDAP_FreeAttrs(&ppstAttrs);

    return ulRet;
}

/*********************************************************************
     Func Name : ISNS_LDAP_DelIscsi
  Date Created : 2016/10/29
        Author : liangjinchao@dian
   Description : 删除ISCSI
         Input : IN const CHAR *pcIscsiName
        Output : 无
        Return : 成功/失败
       Caution :
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
ULONG ISNS_LDAP_DelIscsi(IN const CHAR *pcIscsiName)
{
    CHAR szDn[ISNS_LDAP_DN_MAX_SIZE];
    ULONG ulRet;

    _ldap_FillDn(ISNS_LDAP_DIR_ISCSI, pcIscsiName, szDn, sizeof(szDn));

    ulRet = ISNS_LDAP_DelEntry(szDn);

    return ulRet;
}

/*********************************************************************
     Func Name : ISNS_LDAP_AddEntity
  Date Created : 2016/10/29
        Author : liangjinchao@dian
   Description : 添加entity
         Input : IN const SOIP_Entity *pstCfg
        Output : 无
        Return : 成功/失败
       Caution :
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
ULONG ISNS_LDAP_AddEntity(IN const SOIP_Entity *pstCfg)
{
    const CHAR *apcValue[ISNS_LDAP_ATTR_MAX_SIZE] = { 0 };
    CHAR szDn[ISNS_LDAP_DN_MAX_SIZE];
    LDAPMod **ppstAttrs;
    ULONG ulRet;

    CHAR szIndex[ISNS_NUM_STR_MAX_SIZE];
    CHAR szProtocol[ISNS_NUM_STR_MAX_SIZE];
    CHAR szMIPAddr[ISNS_LDAP_IPADDR_SIZE];
    CHAR szTimeStamp[ISNS_NUM_STR_MAX_SIZE];
    CHAR szPrtlMax[ISNS_NUM_STR_MAX_SIZE];
    CHAR szPrtlMin[ISNS_NUM_STR_MAX_SIZE];
    CHAR szRegPeriod[ISNS_NUM_STR_MAX_SIZE];

    scnprintf(szIndex, sizeof(szIndex), "%u", pstCfg->entity_index);
    scnprintf(szProtocol, sizeof(szProtocol), "%u", pstCfg->eid_type);
    _ldap_IPAddr2Str(&pstCfg->mgmt_ip_addr, szMIPAddr, sizeof(szMIPAddr));
    scnprintf(szTimeStamp, sizeof(szTimeStamp), "%u", (UINT)pstCfg->timestamp.t_time);
    scnprintf(szPrtlMax, sizeof(szPrtlMax), "%u", pstCfg->prot_ver.ver_max);
    scnprintf(szPrtlMin, sizeof(szPrtlMin), "%u", pstCfg->prot_ver.ver_min);
    scnprintf(szRegPeriod, sizeof(szRegPeriod), "%u", pstCfg->period);

    apcValue[ISNS_LDAP_ORD_ENTITY_OBJCLASS] = ISNS_LDAP_OBJCLASS_ENTITY;
    apcValue[ISNS_LDAP_ORD_ENTITY_ID] = pstCfg->eid.id;
    apcValue[ISNS_LDAP_ORD_ENTITY_INDEX] = szIndex;
    apcValue[ISNS_LDAP_ORD_ENTITY_PROTOCOL] = szProtocol;
    apcValue[ISNS_LDAP_ORD_ENTITY_MIPADDR] = szMIPAddr;
    apcValue[ISNS_LDAP_ORD_ENTITY_TIMESTAMP] = szTimeStamp;
    apcValue[ISNS_LDAP_ORD_ENTITY_PRTLMAX] = szPrtlMax;
    apcValue[ISNS_LDAP_ORD_ENTITY_PRTLMIN] = szPrtlMin;
    apcValue[ISNS_LDAP_ORD_ENTITY_REGPERIOD] = szRegPeriod;

    ppstAttrs = ISNS_LDAP_NewSingleAttrs(g_apcAttrsEntity, apcValue);
    if(NULL == ppstAttrs)
    {
        return ERROR_FAILED;
    }

    _ldap_FillDn(ISNS_LDAP_DIR_ENTITY, pstCfg->eid.id, szDn, sizeof(szDn));

    ulRet = ISNS_LDAP_ReplaceEntry(szDn, ppstAttrs);
    ISNS_LDAP_FreeAttrs(&ppstAttrs);

    return ulRet;
}

/*********************************************************************
     Func Name : ISNS_LDAP_DelEntity
  Date Created : 2016/10/29
        Author : liangjinchao@dian
   Description : 删除ENTITY
         Input : IN const CHAR *pcEntityID
        Output : 无
        Return : 成功/失败
       Caution :
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
ULONG ISNS_LDAP_DelEntity(IN const CHAR *pcEntityID)
{
    CHAR szDn[ISNS_LDAP_DN_MAX_SIZE];
    ULONG ulRet;

    _ldap_FillDn(ISNS_LDAP_DIR_ENTITY, pcEntityID, szDn, sizeof(szDn));

    ulRet = ISNS_LDAP_DelEntry(szDn);

    return ulRet;
}

/*********************************************************************
     Func Name : ISNS_LDAP_AddPortal
  Date Created : 2016/10/29
        Author : liangjinchao@dian
   Description : 添加PORTAL
         Input : IN const SOIP_Portal *pstCfg
        Output : 无
        Return : 成功/失败
       Caution :
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
ULONG ISNS_LDAP_AddPortal(IN const SOIP_Portal *pstCfg)
{
    const CHAR *apcValue[ISNS_LDAP_ATTR_MAX_SIZE] = { 0 };
    CHAR szDn[ISNS_LDAP_DN_MAX_SIZE];
    LDAPMod **ppstAttrs;
    ULONG ulRet;

    CHAR szKey[ISNS_LDAP_IPADDR_SIZE + ISNS_NUM_STR_MAX_SIZE];
    CHAR szEsiInterval[ISNS_NUM_STR_MAX_SIZE];
    CHAR szEsiPort[ISNS_NUM_STR_MAX_SIZE];
    CHAR szIndex[ISNS_NUM_STR_MAX_SIZE];
    CHAR szSCNPort[ISNS_NUM_STR_MAX_SIZE];
    CHAR szSecurBitmap[ISNS_LDAP_BIT_STR_SIZE];

    _ldap_NKeyFmt(&pstCfg->ip_addr, pstCfg->ip_port, NULL, szKey, sizeof(szKey));
    scnprintf(szEsiInterval, sizeof(szEsiInterval), "%u", pstCfg->esi_interval);
    scnprintf(szEsiPort, sizeof(szEsiPort), "%u", pstCfg->esi_port);
    scnprintf(szIndex, sizeof(szIndex), "%u", pstCfg->portal_index);
    scnprintf(szSCNPort, sizeof(szSCNPort), "%u", pstCfg->esi_port); /* ????? 与ESI一样  */
    ISNS_LDAP_Num2BitStr(pstCfg->default_portal_tag, szSecurBitmap);

    apcValue[ISNS_LDAP_ORD_PORTAL_OBJCLASS] = ISNS_LDAP_OBJCLASS_PORTAL;
    apcValue[ISNS_LDAP_ORD_PORTAL_KEY] = szKey;
    apcValue[ISNS_LDAP_ORD_PORTAL_SYMNAME] = pstCfg->sym_name;
    apcValue[ISNS_LDAP_ORD_PORTAL_ESINTVAL] = szEsiInterval;
    apcValue[ISNS_LDAP_ORD_PORTAL_ESIPORT] = szEsiPort;
    apcValue[ISNS_LDAP_ORD_PORTAL_INDEX] = szIndex;
    apcValue[ISNS_LDAP_ORD_PORTAL_SCNPORT] = szSCNPort;
    apcValue[ISNS_LDAP_ORD_PORTAL_SECURBITMAP] = szSecurBitmap;
    apcValue[ISNS_LDAP_ORD_PORTAL_ENTITYID] = pstCfg->entity_id.id;

    ppstAttrs = ISNS_LDAP_NewSingleAttrs(g_apcAttrsPortal, apcValue);
    if(NULL == ppstAttrs)
    {
        return ERROR_FAILED;
    }

    _ldap_FillDn(ISNS_LDAP_DIR_PORTAL, szKey, szDn, sizeof(szDn));

    ulRet = ISNS_LDAP_ReplaceEntry(szDn, ppstAttrs);
    ISNS_LDAP_FreeAttrs(&ppstAttrs);

    return ulRet;
}

/*********************************************************************
     Func Name : ISNS_LDAP_DelPortal
  Date Created : 2016/10/29
        Author : liangjinchao@dian
   Description : 删除PORTAL
         Input : IN const SOIP_Portal_Key *pstPotralKey
        Output : 无
        Return : 成功/失败
       Caution :
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
ULONG ISNS_LDAP_DelPortal(IN const SOIP_Portal_Key *pstPotralKey)
{
    CHAR szDn[ISNS_LDAP_DN_MAX_SIZE];
    ULONG ulRet;

    CHAR szKey[ISNS_LDAP_IPADDR_SIZE + ISNS_NUM_STR_MAX_SIZE];

    _ldap_NKeyFmt(&pstPotralKey->ip_addr, pstPotralKey->ip_port, NULL, szKey, sizeof(szKey));
    _ldap_FillDn(ISNS_LDAP_DIR_PORTAL, szKey, szDn, sizeof(szDn));

    ulRet = ISNS_LDAP_DelEntry(szDn);

    return ulRet;
}

/*********************************************************************
     Func Name : ISNS_LDAP_AddPG
  Date Created : 2016/10/29
        Author : liangjinchao@dian
   Description : 添加PG
         Input : IN const SOIP_Portal_Group *pstCfg
        Output : 无
        Return : 成功/失败
       Caution :
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
ULONG ISNS_LDAP_AddPG(IN const SOIP_Portal_Group *pstCfg)
{
    const CHAR *apcValue[ISNS_LDAP_ATTR_MAX_SIZE] = { 0 };
    CHAR szDn[ISNS_LDAP_DN_MAX_SIZE];
    LDAPMod **ppstAttrs;
    ULONG ulRet;

    CHAR szKey[ISNS_LDAP_IPADDR_SIZE + ISNS_NUM_STR_MAX_SIZE + MAX_ISCSI_NODE_ID_SIZE];
    CHAR szPGT[ISNS_NUM_STR_MAX_SIZE];
    CHAR szIndex[ISNS_NUM_STR_MAX_SIZE];

    _ldap_NKeyFmt(&pstCfg->ip_addr, pstCfg->ip_port, pstCfg->id.v, szKey, sizeof(szKey));
    scnprintf(szPGT, sizeof(szPGT), "%u", pstCfg->portal_tag);
    scnprintf(szIndex, sizeof(szIndex), "%u", pstCfg->portal_group_index);

    apcValue[ISNS_LDAP_ORD_PG_OBJCLASS] = ISNS_LDAP_OBJCLASS_PG;
    apcValue[ISNS_LDAP_ORD_PG_KEY] = szKey;
    apcValue[ISNS_LDAP_ORD_PG_PGT] = szPGT;
    apcValue[ISNS_LDAP_ORD_PG_INDEX] = szIndex;

    ppstAttrs = ISNS_LDAP_NewSingleAttrs(g_apcAttrsPG, apcValue);
    if(NULL == ppstAttrs)
    {
        return ERROR_FAILED;
    }

    _ldap_FillDn(ISNS_LDAP_DIR_PG, szKey, szDn, sizeof(szDn));

    ulRet = ISNS_LDAP_ReplaceEntry(szDn, ppstAttrs);
    ISNS_LDAP_FreeAttrs(&ppstAttrs);

    return ulRet;
}

/*********************************************************************
     Func Name : ISNS_LDAP_DelPG
  Date Created : 2016/10/29
        Author : liangjinchao@dian
   Description : 删除PG
         Input : IN const SOIP_Portal_Group_Key *pstKey
        Output : 无
        Return : 成功/失败
       Caution :
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
ULONG ISNS_LDAP_DelPG(IN const SOIP_Portal_Group_Key *pstKey)
{
    CHAR szDn[ISNS_LDAP_DN_MAX_SIZE];
    ULONG ulRet;

    CHAR szKey[ISNS_LDAP_IPADDR_SIZE + ISNS_NUM_STR_MAX_SIZE + MAX_ISCSI_NODE_ID_SIZE];

    _ldap_NKeyFmt(&pstKey->ip_addr, pstKey->ip_port, pstKey->id.v, szKey, sizeof(szKey));
    _ldap_FillDn(ISNS_LDAP_DIR_PG, szKey, szDn, sizeof(szDn));

    ulRet = ISNS_LDAP_DelEntry(szDn);

    return ulRet;
}

/*********************************************************************
     Func Name : ISNS_LDAP_AddDDIscsi
  Date Created : 2016/10/29
        Author : liangjinchao@dian
   Description : 添加DD ISCSI MEMBER
         Input : IN UINT32 uiDDId, IN const CHAR *pcIscsiName, IN UINT uiIscsiIndex
        Output : 无
        Return : 成功/失败
       Caution :
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
ULONG ISNS_LDAP_AddDDIscsi(IN UINT32 uiDDId, IN const CHAR *pcIscsiName, IN UINT uiIscsiIndex)
{
    CHAR szDn[ISNS_LDAP_DN_MAX_SIZE];

    CHAR szId[ISNS_NUM_STR_MAX_SIZE];
    CHAR szValue[ISNS_NUM_STR_MAX_SIZE + MAX_ISCSI_NODE_ID_SIZE];

    scnprintf(szId, sizeof(szId), "%u", uiDDId);
    scnprintf(szValue, sizeof(szValue), "%d %s", uiIscsiIndex, pcIscsiName);

    _ldap_FillDn(ISNS_LDAP_DIR_DD, szId, szDn, sizeof(szDn));

    if(ERROR_SUCCESS != _ldap_AddVoidDDIfNeed(uiDDId, szDn))
    {
        return ERROR_FAILED;
    }

    return ISNS_LDAP_MultValueAdd(szDn, g_apcAttrsDD[ISNS_LDAP_ORD_DD_MEMBER], szValue);
}

/*********************************************************************
     Func Name : ISNS_LDAP_DelDDIscsi
  Date Created : 2016/10/29
        Author : liangjinchao@dian
   Description : 删除DD ISCSI MEMBER
         Input : IN UINT32 uiDDId, IN const CHAR *pcIscsiName, IN UINT uiIscsiIndex
        Output : 无
        Return : 成功/失败
       Caution :
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
ULONG ISNS_LDAP_DelDDIscsi(IN UINT32 uiDDId, IN const CHAR *pcIscsiName, IN UINT uiIscsiIndex)
{
    CHAR szDn[ISNS_LDAP_DN_MAX_SIZE];

    CHAR szId[ISNS_NUM_STR_MAX_SIZE];
    CHAR szValue[ISNS_NUM_STR_MAX_SIZE + MAX_ISCSI_NODE_ID_SIZE];

    scnprintf(szId, sizeof(szId), "%u", uiDDId);
    scnprintf(szValue, sizeof(szValue), "%d %s", uiIscsiIndex, pcIscsiName);

    _ldap_FillDn(ISNS_LDAP_DIR_DD, szId, szDn, sizeof(szDn));

    return ISNS_LDAP_MultValueDel(szDn, g_apcAttrsDD[ISNS_LDAP_ORD_DD_MEMBER], szValue);
}

/*********************************************************************
     Func Name : ISNS_LDAP_DelAllDDIscsi
  Date Created : 2016/10/29
        Author : liangjinchao@dian
   Description : 删除DD的全部ISCSI MEMBER
         Input : IN UINT32 uiDDId
        Output : 无
        Return : 成功/失败
       Caution :
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
ULONG ISNS_LDAP_DelAllDDIscsi(IN UINT32 uiDDId)
{
    CHAR szDn[ISNS_LDAP_DN_MAX_SIZE];

    CHAR szId[ISNS_NUM_STR_MAX_SIZE];

    scnprintf(szId, sizeof(szId), "%u", uiDDId);
    _ldap_FillDn(ISNS_LDAP_DIR_DD, szId, szDn, sizeof(szDn));

    return ISNS_LDAP_MultValueDelAll(szDn, g_apcAttrsDD[ISNS_LDAP_ORD_DD_MEMBER]);
}

/*********************************************************************
     Func Name : ISNS_LDAP_AddDdDdsMap
  Date Created : 2016/10/29
        Author : liangjinchao@dian
   Description : 添加DD的DDSMAP
         Input : IN UINT32 uiDDId, IN UINT32 uiDDSId
        Output : 无
        Return : 成功/失败
       Caution :
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
ULONG ISNS_LDAP_AddDdDdsMap(IN UINT32 uiDDId, IN UINT32 uiDDSId)
{
    CHAR szDn[ISNS_LDAP_DN_MAX_SIZE];

    CHAR szId[ISNS_NUM_STR_MAX_SIZE];
    CHAR szDDSId[ISNS_NUM_STR_MAX_SIZE];

    scnprintf(szId, sizeof(szId), "%u", uiDDId);
    scnprintf(szDDSId, sizeof(szDDSId), "%u", uiDDSId);

    _ldap_FillDn(ISNS_LDAP_DIR_DD, szId, szDn, sizeof(szDn));

    if(ERROR_SUCCESS != _ldap_AddVoidDDIfNeed(uiDDId, szDn))
    {
        return ERROR_FAILED;
    }

    return ISNS_LDAP_MultValueAdd(szDn, g_apcAttrsDD[ISNS_LDAP_ORD_DD_DDSMAP], szDDSId);
}

/*********************************************************************
     Func Name : ISNS_LDAP_DelDdDdsMap
  Date Created : 2016/10/29
        Author : liangjinchao@dian
   Description : 删除DD 的 DDS MAP
         Input : IN UINT32 uiDDId, IN UINT32 uiDDSId
        Output : 无
        Return : 成功/失败
       Caution :
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
ULONG ISNS_LDAP_DelDdDdsMap(IN UINT32 uiDDId, IN UINT32 uiDDSId)
{
    CHAR szDn[ISNS_LDAP_DN_MAX_SIZE];

    CHAR szId[ISNS_NUM_STR_MAX_SIZE];
    CHAR szDDSId[ISNS_NUM_STR_MAX_SIZE];

    scnprintf(szId, sizeof(szId), "%u", uiDDId);
    scnprintf(szDDSId, sizeof(szDDSId), "%u", uiDDSId);

    _ldap_FillDn(ISNS_LDAP_DIR_DD, szId, szDn, sizeof(szDn));

    return ISNS_LDAP_MultValueDel(szDn, g_apcAttrsDD[ISNS_LDAP_ORD_DD_DDSMAP], szDDSId);
}

/*********************************************************************
     Func Name : ISNS_LDAP_DelAllDdDdsMap
  Date Created : 2016/10/29
        Author : liangjinchao@dian
   Description : 删除DD的全部dds map
         Input : IN UINT32 uiDDId
        Output : 无
        Return : 成功/失败
       Caution :
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
ULONG ISNS_LDAP_DelAllDdDdsMap(IN UINT32 uiDDId)
{
    CHAR szDn[ISNS_LDAP_DN_MAX_SIZE];
    CHAR szId[ISNS_NUM_STR_MAX_SIZE];

    scnprintf(szId, sizeof(szId), "%u", uiDDId);
    _ldap_FillDn(ISNS_LDAP_DIR_DD, szId, szDn, sizeof(szDn));

    return ISNS_LDAP_MultValueDelAll(szDn, g_apcAttrsDD[ISNS_LDAP_ORD_DD_DDSMAP]);
}

/*********************************************************************
     Func Name : ISNS_LDAP_Write
  Date Created : 2016/10/29
        Author : liangjinchao@dian
   Description : 保存数据到LDAP，如果已存在则覆盖
         Input : IN const ISNS_DBKey *pstDbKey, IN const SOIP_DB_Entry *pstEntry
        Output : 无
        Return : 成功/失败
       Caution : 无
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
INT ISNS_LDAP_Write(IN const ISNS_DBKey *pstDbKey, IN const SOIP_DB_Entry *pstEntry)
{
    ULONG ulRet = ERROR_SUCCESS;

    switch(pstDbKey->tag)
    {
        case DD_ID_KEY:
        {
            ulRet = ISNS_LDAP_AddDD(&pstEntry->data.dd);
            break;
        }
        case DDS_ID_KEY:
        {
            ulRet = ISNS_LDAP_AddDDS(&pstEntry->data.dds);
            break;
        }
        case ISCSI_ID_KEY:
        {
            ulRet = ISNS_LDAP_AddIscsi(&pstEntry->data.scsi_node);
            break;
        }
        case ENTITY_ID_KEY:
        {
            ulRet = ISNS_LDAP_AddEntity(&pstEntry->data.entity);
            break;
        }
        case PORTAL_ID_KEY:
        {
            ulRet = ISNS_LDAP_AddPortal(&pstEntry->data.portal);
            break;
        }
        case PORTAL_GROUP_ID_KEY:
        {
            ulRet = ISNS_LDAP_AddPG(&pstEntry->data.portal_group);
            break;
        }
        case ENTITY_IDX_KEY:
        case ISCSI_IDX_KEY:
        case PORTAL_IDX_KEY:
        {
            ulRet = ERROR_SUCCESS;
            break;
        }
        default:
        {
            ulRet = ERROR_FAILED;
            break;
        }
    }

    if(ERROR_SUCCESS != ulRet)
    {
        return ISNS_UNKNOWN_ERR;
    }

    return SUCCESS;
}

/*********************************************************************
     Func Name : ISNS_LDAP_Delete
  Date Created : 2016/10/29
        Author : liangjinchao@dian
   Description : 删除LDAP中的数据
         Input : IN const ISNS_DBKey *pstDbKey
        Output : 无
        Return : 成功/失败
       Caution : 无
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
INT ISNS_LDAP_Delete(IN const ISNS_DBKey *pstDbKey)
{
    ULONG ulRet = ERROR_SUCCESS;

    switch(pstDbKey->tag)
    {
        case DD_ID_KEY:
        {
            ulRet = ISNS_LDAP_DelDD(pstDbKey->val.dd_key.id);
            break;
        }
        case DDS_ID_KEY:
        {
            ulRet = ISNS_LDAP_DelDDS(pstDbKey->val.dds_key.id);
            break;
        }
        case ISCSI_ID_KEY:
        {
            ulRet = ISNS_LDAP_DelIscsi(pstDbKey->val.iscsi_key.v);
            break;
        }
        case ENTITY_ID_KEY:
        {
            ulRet = ISNS_LDAP_DelEntity(pstDbKey->val.entity_key.id);
            break;
        }
        case PORTAL_ID_KEY:
        {
            ulRet = ISNS_LDAP_DelPortal(&pstDbKey->val.portal_key);
            break;
        }
        case PORTAL_GROUP_ID_KEY:
        {
            ulRet = ISNS_LDAP_DelPG(&pstDbKey->val.portal_group_key);
            break;
        }
        case ENTITY_IDX_KEY:
        case ISCSI_IDX_KEY:
        case PORTAL_IDX_KEY:
        {
            ulRet = ERROR_SUCCESS;
            break;
        }
        default:
        {
            ulRet = ERROR_FAILED;
            break;
        }
    }

    if(ERROR_SUCCESS != ulRet)
    {
        return ISNS_UNKNOWN_ERR;
    }

    return SUCCESS;
}

/*********************************************************************
     Func Name : ISNS_LDAP_List_AddNode
  Date Created : 2016/10/29
        Author : liangjinchao@dian
   Description : 初始化内存数据库
         Input : 无
        Output : 无
        Return : 成功/失败
       Caution : 如果parent不存在则新建(调用顺序问题)
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
INT ISNS_LDAP_List_AddNode(IN UINT uiListType, IN const VOID *pParent,
                           IN const CHAR *pcData, IN INT iSize)
{
    ULONG ulRet = ERROR_SUCCESS;

    /* 有的链可以不存LDAP */
    if(uiListType <= 0 || uiListType >= DATA_LIST_MAX)
    {
        return ISNS_UNKNOWN_ERR;
    }

    switch(uiListType)
    {
        case DD_MEMBER_LIST:
        {
            SOIP_Dd *pstDd = (SOIP_Dd *)pParent;
            SOIP_Dd_Member *pstIscMem = (SOIP_Dd_Member *)pcData;
            ulRet = ISNS_LDAP_AddDDIscsi(pstDd->id, pstIscMem->node_id, pstIscMem->node_idx);
            break;
        }
        case DD_DDS_LIST:
        {
            SOIP_Dd *pstDd = (SOIP_Dd *)pParent;
            UINT32 uiId = *(UINT32 *)pcData;
            ulRet = ISNS_LDAP_AddDdDdsMap(pstDd->id, uiId);
            break;
        }
        default:
        {
            ulRet = ERROR_SUCCESS;
            break;
        }

    }

    if(ERROR_SUCCESS != ulRet)
    {
        return ISNS_UNKNOWN_ERR;
    }

    return SUCCESS;
}

/*********************************************************************
     Func Name : ISNS_LDAP_List_RemoveNode
  Date Created : 2016/10/29
        Author : liangjinchao@dian
   Description : 初始化内存数据库
         Input : 无
        Output : 无
        Return : 成功/失败
       Caution : 无
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
INT ISNS_LDAP_List_RemoveNode(IN UINT uiListType, IN const VOID *pParent,
                              IN const CHAR *pcData, IN INT iSize)
{
    ULONG ulRet = ERROR_SUCCESS;

    if(uiListType <= 0 || uiListType >= DATA_LIST_MAX)
    {
        return ISNS_UNKNOWN_ERR;
    }

    switch(uiListType)
    {
        case DD_MEMBER_LIST:
        {
            SOIP_Dd *pstDd = (SOIP_Dd *)pParent;
            SOIP_Dd_Member *pstIscMem = (SOIP_Dd_Member *)pcData;
            ulRet = ISNS_LDAP_DelDDIscsi(pstDd->id, pstIscMem->node_id, pstIscMem->node_idx);
            break;
        }
        case DD_DDS_LIST:
        {
            SOIP_Dd *pstDd = (SOIP_Dd *)pParent;
            UINT32 uiId = *(UINT32 *)pcData;
            ulRet = ISNS_LDAP_DelDdDdsMap(pstDd->id, uiId);
            break;
        }
        default:
        {
            ulRet = ERROR_SUCCESS;
            break;
        }

    }

    if(ERROR_SUCCESS != ulRet)
    {
        return ISNS_UNKNOWN_ERR;
    }

    return SUCCESS;
}

/*********************************************************************
     Func Name : ISNS_LDAP_List_Delete
  Date Created : 2016/10/29
        Author : liangjinchao@dian
   Description : 初始化内存数据库
         Input : 无
        Output : 无
        Return : 成功/失败
       Caution : 无
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
INT ISNS_LDAP_List_Delete(IN UINT uiListType, IN const VOID *pParent)
{
    ULONG ulRet = ERROR_SUCCESS;

    if(uiListType <= 0 || uiListType >= DATA_LIST_MAX)
    {
        return ISNS_UNKNOWN_ERR;
    }

    switch(uiListType)
    {
        case DD_MEMBER_LIST:
        {
            SOIP_Dd *pstDd = (SOIP_Dd *)pParent;
            ulRet = ISNS_LDAP_DelAllDDIscsi(pstDd->id);
            break;
        }
        case DD_DDS_LIST:
        {
            SOIP_Dd *pstDd = (SOIP_Dd *)pParent;
            ulRet = ISNS_LDAP_DelAllDdDdsMap(pstDd->id);
            break;
        }
        default:
        {
            ulRet = ERROR_SUCCESS;
            break;
        }

    }

    if(ERROR_SUCCESS != ulRet)
    {
        return ISNS_UNKNOWN_ERR;
    }

    return SUCCESS;
}

/*****************************************************************************
    Func Name: ISNS_LDAP_Init
 Date Created: 2016/11/3
       Author: liangjinchao@dian
  Description: LDAP全局初始化
        Input: IN const ISNS_LDAP_INIT_S *pstInit
       Output: 无
       Return: 无
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
ULONG ISNS_LDAP_Init(IN const ISNS_LDAP_INIT_S *pstInit)
{
    ULONG ulRet;

    ulRet = ISNS_LDAP_ServerInit(pstInit, g_apcIsnsLdapDir);
    if(ERROR_SUCCESS != ulRet)
    {
        return ulRet;
    }

    return ulRet;
}

/*****************************************************************************
    Func Name: ISNS_LDAP_Fini
 Date Created: 2016/11/3
       Author: liangjinchao@dian
  Description: LDAP全局去初始化
        Input: IN const ISNS_LDAP_INIT_S *pstInit
       Output: 无
       Return: 无
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
VOID ISNS_LDAP_Fini()
{
    ISNS_LDAP_ServerFini();
}

