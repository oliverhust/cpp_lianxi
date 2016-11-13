/*****************************************************************************
 Copyright (c) 2011, Hangzhou H3C Technologies Co., Ltd. All rights reserved.
------------------------------------------------------------------------------
                                  nat.h
  Project Code: Comware V7
   Module Name: nat
  Date Created: 2013-4-16 
        Author: p03838
   Description: Header file of nat module for product
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
                                                                             
*****************************************************************************/
#ifndef _DRV_NAT_H_
#define _DRV_NAT_H_

#ifdef  __cplusplus
extern "C"{
#endif

#ifdef __KERNEL__

/*****************************************************************************
                                 �ṹ�嶨��
*****************************************************************************/

/* NAT�����ӿڽṹ */
typedef struct tagNAT_DRV_INTF_S
{
    ULONG (*pfDRV_NAT_SetServiceNode)(IN IF_INDEX ifIndex, IN LIP_ADDR stNode);
} NAT_DRV_INTF_S, *PNAT_DRV_INTF_S;

/*****************************************************************************
       ƽ̨���ʵ��NATģ���ʼ������������ƺ����������ӿ�ע�ắ����
*****************************************************************************/

extern ULONG NAT_Init(IN LPVOID pStartContext);

extern ULONG NAT_Run(IN LPVOID pStartContext);

extern ULONG NAT_Register_Driver(IN ULONG ulVersion, IN const NAT_DRV_INTF_S *pstIntf);

/*****************************************************************************
                         ��Ʒʵ��NATģ���ʼ������
*****************************************************************************/

extern ULONG USR_NAT_StartUpInit(IN LPVOID pStartContext);

extern ULONG USR_NAT_StartUpAutoRun(IN LPVOID pStartContext);

#endif

#ifdef  __cplusplus
}
#endif  /* end of __cplusplus */

#endif  /* end of _DRV_NAT_H_ */

