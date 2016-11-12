/*****************************************************************************
 Copyright (c) 2010, Hangzhou H3C Technologies Co., Ltd. All rights reserved.
------------------------------------------------------------------------------
                            ipbase.h
  Project Code: Comware Leopard
   Module Name: IPBase
  Date Created: 2010-5-10 
        Author: L04371
   Description: IP 基础模块守护进程公共头文件 

------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
                                                                             
*****************************************************************************/
#ifndef _IPBASE_H_
#define _IPBASE_H_

#ifdef  __cplusplus
extern "C"{
#endif


/* HA SG */
#define IPD_SG_NAME     "IPBase"

/* epoll callback return */
#define EPOLL_SUCCESS   0x0            /* 正常处理完成 */
#define EPOLL_RESTART   0x1            /* 重新下一轮事件读取 */
#define EPOLL_STOP      0x2            /* 进程停止 */
#define EPOLL_NEXT      EPOLL_SUCCESS  /* 正常处理完成 */

/* IPD public epoll */
extern INT IPD_Get_EpollFd( VOID );

/* IPD Module init function declare */
extern ULONG ARP_Init(IN INT argc, IN CHAR *argv[]);
extern ULONG IPFS_Init(IN INT argc, IN CHAR *argv[]);
extern ULONG IPFW_Init(IN INT argc, IN CHAR *argv[]);
extern ULONG UDPH_Init(IN INT iArgc, IN CHAR *apcArgv[]);
extern ULONG SO_Initialize(IN INT argc, IN CHAR *argv[]);
extern ULONG URPF_Init(IN INT argc, IN CHAR *argv[]);
extern VOID URPF_ACLReInit(VOID);
extern ULONG DHCP_LibInit(IN INT iArgc, IN CHAR *apcArgv[]);
extern ULONG IPVFR_Init(IN INT iArgc, IN CHAR *apcArgv[]);
extern ULONG IPFW_Run(VOID);
/* IPD Module exit function declare */
extern VOID ARP_Exit( VOID );
extern VOID IPFS_Exit( VOID );
extern VOID IPFW_Fini( VOID );
extern VOID UDPH_Exit(VOID);
extern VOID SO_Finalize( VOID );
extern VOID URPF_UnInit( VOID );
extern VOID URPF_ACLUnInit(VOID);
extern VOID DHCP_LibExit(VOID);

extern VOID UDPH_SignalHandle(VOID);
extern VOID IPVFR_Exit(VOID);

extern ULONG IPADDR_Init(IN INT argc, IN CHAR *argv[]);
extern VOID IPADDR_Fini(VOID);

extern ULONG IP_DaemonCheck(IN UINT uiUndoFlag);
extern BOOL_T IP_IsDaemonExist(IN LIP_ADDR stLipNode);
extern ULONG IP_StartDaemon(IN LIP_ADDR stLipNode);
extern CHAR * IP_GetServiceName(VOID);
extern ULONG IP_ServiceStart(VOID);

#ifdef  __cplusplus
}
#endif  /* end of __cplusplus */

#endif  /* end of _IPBASE_H_ */

