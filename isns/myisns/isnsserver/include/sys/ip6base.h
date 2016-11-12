/*****************************************************************************
 Copyright (c) 2010, Hangzhou H3C Technologies Co., Ltd. All rights reserved.
------------------------------------------------------------------------------
                            ip6base.h
  Project Code:    Comware V7
   Module Name: IP6Base
  Date Created: 2010-9-9
        Author:     Z03132
   Description: IPv6 基础模块守护进程公共头文件 

------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
                                                                             
*****************************************************************************/
#ifndef _IP6BASE_H_
#define _IP6BASE_H_

#ifdef  __cplusplus
extern "C"{
#endif


/* HA SG */
#define IP6D_SG_NAME     "IP6Base"

/* epoll callback return */
#define EPOLL_SUCCESS   0x0            /* 正常处理完成 */
#define EPOLL_RESTART   0x1            /* 重新下一轮事件读取 */
#define EPOLL_STOP         0x2            /* 进程停止 */
#define EPOLL_NEXT         EPOLL_SUCCESS  /* 正常处理完成 */

/* IP6D public epoll */
extern INT IP6D_GetEpollFd( VOID );

extern ULONG IP6_Init(IN INT argc, IN CHAR *argv[]);
extern VOID IP6_Exit(VOID);

/* IP6D Module init function declare */
extern ULONG IP6FS_Init(IN INT argc, IN CHAR *argv[]);

/* IP6D Module exit function declare */
extern VOID IP6FS_Exit( VOID );

extern ULONG URPF6_Init(IN INT argc, IN CHAR *argv[]);
extern VOID URPF6_ACLReInit(VOID);

extern VOID URPF6_UnInit( VOID );
extern VOID URPF6_ACLUnInit(VOID);
extern ULONG NDD_Init(IN INT argc, IN CHAR *argv[]);
extern VOID NDD_Exit (VOID);
extern VOID NDD_RAGUARD_ACLSmooth(VOID);

extern ULONG IP6ADDR_Init(IN INT argc, IN CHAR *argv[]);
extern VOID IP6ADDR_Exit(VOID);


extern ULONG PMTU_Init(IN INT argc, IN CHAR *argv[]);
extern VOID PMTU_Exit(VOID);

extern ULONG IP6_DaemonCheck(IN UINT uiUndoFlag);
extern BOOL_T IP6_IsDaemonExist(IN LIP_ADDR stLipNode);
extern ULONG IP6_StartDaemon(IN LIP_ADDR stLipNode);
extern CHAR * IP6_GetServiceName(VOID);
extern ULONG IP6_ServiceStart(VOID);

#ifdef  __cplusplus
}
#endif  /* end of __cplusplus */

#endif  /* end of _IPBASE_H_ */

