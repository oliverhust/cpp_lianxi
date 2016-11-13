/*******************************************************************************
 Copyright (c) 2007, Hangzhou H3C Technologies Co., Ltd. All rights reserved.
--------------------------------------------------------------------------------
                              lipc.h
  Project Code: Leopard
   Module Name: LIPC_DRV
  Date Created: 2008-10-10
        Author: y05535
   Description: LIP driver header

--------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
--------------------------------------------------------------------------------
  YYYY-MM-DD

*******************************************************************************/

#ifndef _DRV_LIPC_H_
#define _DRV_LIPC_H_

#ifdef  __cplusplus
extern "C"{
#endif

#ifdef __KERNEL__

/* driver group operation */
typedef enum tagLIPC_GroupOp
{
    LIPC_GROUP_CREATE = 1,  /* create */
    LIPC_GROUP_DELETE,      /* delete */
    LIPC_GROUP_UPDATE,      /* update */
    LIPC_GROUP_OP_MAX
}LIPC_GROUP_OP_E;

/* LIPC driver sending mode */
typedef enum tagLIPC_Mode
{
    LIPC_UNICAST = 1,  /* unicast */
    LIPC_MULTICAST,    /* multicast */
    LIPC_BROADCAST,    /* broadcast */
}LIPC_MODE_E;

/* Node physic info */
typedef struct tagLIPC_NODE
{
    USHORT usChassID;    /* Chass ID */
    USHORT usSlotID;     /* Slot ID */
    USHORT usCpuID;      /* CPU ID */
}LIPC_NODE_S;

/* LIPC drv cfg info */
typedef struct tagLIPC_CONF
{
    LIPC_NODE_S stNode;     /* Physic node info */
    ULONG ulMTU;            /* Link MTU */
    ULONG ulDrvHdrLen;      /* drive header length */
}LIPC_CONF_S;

typedef enum tagLIPC_LIPType
{
    LIP_TYPE_NORMAL = 0,      /* 传统LIP，框号小于8的节点注册的LIP地址类型 */
    LIP_TYPE_SLOT_ONLY,       /* 固定slot的LIP，虚拟紧耦合设备节点注册的LIP地址类型 */
    LIP_TYPE_AUTO_ID,         /* 动态分配的LIP，框号大于等于8节点注册的LIP地址类型 */
    LIP_TYPE_INVALID,         /* 无效LIP，LIP地址恒为0x8000 */
}LIPC_LIPTYPE_E;

#define LIPC_PRIORITY_NONE     0     /* no priority */

/* LIPC drviver */
typedef struct tagLIPC_DRV_INTF
{
    ULONG (*pfDRV_LIPC_Send)(IN ULONG ulFlag,
                           IN ULONG ulGroupID,
                           IN LIPC_NODE_S *pstNode,
                           IN MBUF_S *pstMbuf,
                           IN ULONG ulPriority);
    ULONG (*pfDRV_LIPC_HandleGroupChange)(IN ULONG ulOperation,
                                        IN ULONG ulGroupID,
                                        IN LIPC_NODE_S *pstNodeList,
                                        IN ULONG ulNodeNum);
}LIPC_DRV_INTF_S,*PLIPC_DRV_INTF_S;


ULONG LIPC_Packet_Announce(IN MBUF_S *pstMbuf);
ULONG LIPC_Packet_TransmitDone(IN MBUF_S *pstMbuf);
ULONG LIPC_Node_Remove(IN LIPC_NODE_S *pstNode);
ULONG LIPC_Init_Conf(IN LIPC_CONF_S* pstCfg);
ULONG LIPC_Register_Driver(IN ULONG ulVersion,IN const PLIPC_DRV_INTF_S  pstLIPCDriverIntf);
ULONG LIPC_SetLIPType(IN LIPC_LIPTYPE_E enLIPType);

#endif	/* end of __KERNEL__ */


#ifdef __cplusplus
}
#endif

#endif	/* end of _DRV_LIPC_H_ */

