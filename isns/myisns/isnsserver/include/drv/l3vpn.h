/*****************************************************************************
 Copyright (c) 2010, Hangzhou H3C Technologies Co., Ltd. All rights reserved.
------------------------------------------------------------------------------
                              drv/l3vpn.h
  Project Code: Comware
   Module Name: L3VPN
  Date Created: 2010-11-20
        Author: l06046
   Description: driver macros/data types/interfaces for L3VPN
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
  YYYY-MM-DD

*****************************************************************************/

#ifndef _DRV_L3VPN_H_
#define _DRV_L3VPN_H_

#ifdef __cplusplus
extern "C"{
#endif

/* L3VPN-EOM-ID定义 */
#define EOM_ID_L3VPN                  ((ULONG)EOM_OID_DEFINE(MID_WAN_BASE, 0x03))

/* 拓扑策略变化的标志位 */
#define MT_MTPOLICY_MOD_TOPO            0x0001         /* apply topology的值变化 */
#define MT_MTPOLICY_MOD_DSCP            0x0002         /* if-match dscp的值变化 */
#define MT_MTPOLICY_MOD_IPPRECE         0x0004         /* if-match ip-precedence的值变化 */
#define MT_MTPOLICY_MOD_ACL_EVENT       0x00008        /* 引用ACL规则发生变化 */
#define MT_MTPOLICY_MOD_ACL_UPDATE      0x00010        /* 引用ACL需要强制刷新 */
#define MT_MTPOLICY_MOD_MATCH           0x00020        /* if-match配置变化 */

/* 拓扑策略if-match的标志位 */
#define MTPOLICY_CFG_IFMATCH_IPPRECE  0x01
#define MTPOLICY_CFG_IFMATCH_DSCP     0x02
#define MTPOLICY_CFG_IFMATCH_ACL      0x04


/* 下驱动命令字定义 */
typedef enum tagL3VPN_IOCTL_E
{
    L3VPN_IOCTL_VRF_ADD,            /* 创建VPN */
    L3VPN_IOCTL_VRF_DEL,            /* 删除VPN */
    L3VPN_IOCTL_INTFVRFBIND,        /* 接口绑定VPN */
    L3VPN_IOCTL_INTFVRFUNBIND,      /* 接口去绑定VPN */
    L3VPN_IOCTL_MT_ADD,             /* 创建MT */
    L3VPN_IOCTL_MT_DEL,             /* 删除MT */
    L3VPN_IOCTL_MTPOLICY_ADD,       /* 创建拓扑策略 */
    L3VPN_IOCTL_MTPOLICY_MOD,       /* 修改拓扑策略 */
    L3VPN_IOCTL_MTPOLICY_DEL,       /* 删除拓扑策略 */
    L3VPN_IOCTL_BUTT
}L3VPN_IOCTL_E;

#define L3VPN_DRV_CONTEXT_LEN 2

typedef struct tagL3VPN_IF_CFG
{
    IF_INDEX    ifIndex;    /* 接口索引 */
    VRF_INDEX   vrfIndex;   /* L3VPN索引 */
    UINT64      auiDrvContext[L3VPN_DRV_CONTEXT_LEN];
}L3VPN_IF_CFG_S;

typedef struct tagMTAclEventInfo
{
    UINT uiEventType;               /* Acl事件类型 */
    VOID *pEventData;               /* Acl数据 */
}MT_ACL_EVENT_INFO_S;

typedef struct tagMT_POLICY_CFG
{
    USHORT  usIndex;                       /* 节点号 */
    USHORT  usMatchBit;                    /* 配置的if-match */
    USHORT  usChangeBit;                   /* 变化的标志位 */
    USHORT  usFamily;                      /* 协议类型, AF_INET OR AF_INET6 */
    VRF_INDEX vrfIndex;                    /* APPLY拓扑对应的vrf索引 */
    UCHAR   ucPrecedence;                  /* Match Precedence */
    UCHAR   ucDscp;                        /* Match DSCP */
    UINT    uiAclNum;                      /* Match ACL组ID */
    MT_ACL_EVENT_INFO_S stACLEvent;        /* ACL变化事件通知信息 */
    UINT64 auiDrvContext[L3VPN_DRV_CONTEXT_LEN];
}MT_POLICY_CFG_S;

typedef struct tagL3VPN_DRV_INTF{
    ULONG ( *DRV_L3VPN_Ioctl )(IN L3VPN_IOCTL_E, IN VOID * );
}L3VPN_DRV_INTF_S, *PL3VPN_DRV_INTF_S;

extern ULONG USR_L3VPN_Init (IN VOID* pStartContext);
extern ULONG USR_L3VPN_Run(IN VOID* pStartContext);

ULONG L3VPN_Register_Driver(IN PL3VPN_DRV_INTF_S pstDrvL3vpnIntf);


#ifdef __cplusplus
}
#endif /* end of __cplusplus */

#endif /* end of _DRV_L3VPN_H_ */

