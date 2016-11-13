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

/* L3VPN-EOM-ID���� */
#define EOM_ID_L3VPN                  ((ULONG)EOM_OID_DEFINE(MID_WAN_BASE, 0x03))

/* ���˲��Ա仯�ı�־λ */
#define MT_MTPOLICY_MOD_TOPO            0x0001         /* apply topology��ֵ�仯 */
#define MT_MTPOLICY_MOD_DSCP            0x0002         /* if-match dscp��ֵ�仯 */
#define MT_MTPOLICY_MOD_IPPRECE         0x0004         /* if-match ip-precedence��ֵ�仯 */
#define MT_MTPOLICY_MOD_ACL_EVENT       0x00008        /* ����ACL�������仯 */
#define MT_MTPOLICY_MOD_ACL_UPDATE      0x00010        /* ����ACL��Ҫǿ��ˢ�� */
#define MT_MTPOLICY_MOD_MATCH           0x00020        /* if-match���ñ仯 */

/* ���˲���if-match�ı�־λ */
#define MTPOLICY_CFG_IFMATCH_IPPRECE  0x01
#define MTPOLICY_CFG_IFMATCH_DSCP     0x02
#define MTPOLICY_CFG_IFMATCH_ACL      0x04


/* �����������ֶ��� */
typedef enum tagL3VPN_IOCTL_E
{
    L3VPN_IOCTL_VRF_ADD,            /* ����VPN */
    L3VPN_IOCTL_VRF_DEL,            /* ɾ��VPN */
    L3VPN_IOCTL_INTFVRFBIND,        /* �ӿڰ�VPN */
    L3VPN_IOCTL_INTFVRFUNBIND,      /* �ӿ�ȥ��VPN */
    L3VPN_IOCTL_MT_ADD,             /* ����MT */
    L3VPN_IOCTL_MT_DEL,             /* ɾ��MT */
    L3VPN_IOCTL_MTPOLICY_ADD,       /* �������˲��� */
    L3VPN_IOCTL_MTPOLICY_MOD,       /* �޸����˲��� */
    L3VPN_IOCTL_MTPOLICY_DEL,       /* ɾ�����˲��� */
    L3VPN_IOCTL_BUTT
}L3VPN_IOCTL_E;

#define L3VPN_DRV_CONTEXT_LEN 2

typedef struct tagL3VPN_IF_CFG
{
    IF_INDEX    ifIndex;    /* �ӿ����� */
    VRF_INDEX   vrfIndex;   /* L3VPN���� */
    UINT64      auiDrvContext[L3VPN_DRV_CONTEXT_LEN];
}L3VPN_IF_CFG_S;

typedef struct tagMTAclEventInfo
{
    UINT uiEventType;               /* Acl�¼����� */
    VOID *pEventData;               /* Acl���� */
}MT_ACL_EVENT_INFO_S;

typedef struct tagMT_POLICY_CFG
{
    USHORT  usIndex;                       /* �ڵ�� */
    USHORT  usMatchBit;                    /* ���õ�if-match */
    USHORT  usChangeBit;                   /* �仯�ı�־λ */
    USHORT  usFamily;                      /* Э������, AF_INET OR AF_INET6 */
    VRF_INDEX vrfIndex;                    /* APPLY���˶�Ӧ��vrf���� */
    UCHAR   ucPrecedence;                  /* Match Precedence */
    UCHAR   ucDscp;                        /* Match DSCP */
    UINT    uiAclNum;                      /* Match ACL��ID */
    MT_ACL_EVENT_INFO_S stACLEvent;        /* ACL�仯�¼�֪ͨ��Ϣ */
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

