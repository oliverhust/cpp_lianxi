/*******************************************************************************
 Copyright (c) 2013, Hangzhou H3C Technologies Co., Ltd. All rights reserved.
--------------------------------------------------------------------------------
                              acl.h
  Project Code: Comware V7
   Module Name: ACL
  Date Created: 2013-10-10
        Author:
   Description: ACLģ���ṩ�������Ĺ���ͷ�ļ�, ֻ���ں�̬����,
                �ں˺��û�̬���õ�, ��sys/�¶���

--------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
--------------------------------------------------------------------------------
  YYYY-MM-DD

*******************************************************************************/

#ifndef   _DRV_ACL_H_
#define   _DRV_ACL_H_

#ifdef  __cplusplus
extern "C"{
#endif

#ifndef _SYS_ACL_H_
#include <sys/acl.h>
#endif

/* ACL EOM ID���� */
#define EOM_ID_ACL            EOM_OID_DEFINE(MID_LAN_BASE, 0x1b)

/* �ں�̬���û�̬���صı�����Ϣƥ��ACL��Ľ�� */
typedef struct tagACL_MatchCount
{
    UINT uiRuleIndex;           /* ������ */
    UINT64 uiMatchCount;        /* ƥ�����Ķ��� */
}ACL_MATCHCOUNT_S;

typedef union tagACLMatchPktInfo
{
    ACL_MATCH_IPPKTINFO_S stIpInfo;
    ACL_MATCH_IP6PKTINFO_S stIp6Info;
    ACL_MATCH_MACPKTINFO_S stMacInfo;
}ACL_MATCH_PKT_INFO_U;

extern ULONG DRV_ACL_AddGroup(IN ACL_VERSION_E enACLVer, IN UINT uiGroupIndex);
extern ULONG DRV_ACL_DeleteGroup(IN ACL_VERSION_E enACLVer, IN UINT uiGroupIndex);
extern ULONG DRV_ACL_AddRule(IN ACL_VERSION_E enACLVer,
                             IN UINT uiGroupIndex,
                             IN UINT uiRearRuleIndex,
                             IN const ACL_RULECONTENT_S *pstRuleContent);
extern ULONG DRV_ACL_ModifyRule(IN ACL_VERSION_E enACLVer,
                                IN UINT uiGroupIndex,
                                IN const ACL_RULECONTENT_S *pstRuleContent);
extern ULONG DRV_ACL_DeleteRule(IN ACL_VERSION_E enACLVer,
                                IN UINT uiGroupIndex,
                                IN UINT uiRuleIndex);
extern ULONG DRV_ACL_Match(IN ACL_VERSION_E enACLVer,
                           IN UINT uiGroupIndex,
                           IN const ACL_MATCH_PKT_INFO_U *punPktInfo,
                           IN BOOL_T bIsCount,
                           OUT ACL_MATCH_RESULT_S *pstMatchResult,
                           OUT ACL_ACTION_E *penRuleAction);
extern ULONG DRV_ACL_GetRuleMatchCount(IN ACL_VERSION_E enACLVer,
                                       IN UINT uiGroupIndex,
                                       IN UINT uiRuleNum,
                                       INOUT ACL_MATCHCOUNT_S *pstMatchCount);
extern ULONG DRV_ACL_ClearCount(IN ACL_VERSION_E enACLVer,
                                IN UINT uiGroupIndex);

extern ULONG DRV_ACL_SetGroupStep(IN ACL_VERSION_E enACLVer,
                                  IN UINT uiGroupIndex,
                                  IN UCHAR ucStep);

#ifdef  __cplusplus
}
#endif /* end of __cplusplus */

#endif /* _DRV_ACL_H_ */

