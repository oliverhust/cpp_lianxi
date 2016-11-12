/*****************************************************************************
 Copyright (c) 2010, Hangzhou H3C Technologies Co., Ltd. All rights reserved.
------------------------------------------------------------------------------
                             iSNSreset.c

   Project Code : Comware V7 trill
   Module Name  : TRILL_UPDT
   Date Created : 2016/10/124
   Author       : lichao@dian
   Description  : 实现维护ISNS线程reset处理。
------------------------------------------------------------------------------
    Modification History
    DATE        NAME             DESCRIPTION
------------------------------------------------------------------------------
    2010/8/13   zhouudan         Create
    2010/9/07   yaobo            Modify
******************************************************************************/

#ifdef __cplusplus
extern "C"{
#endif
#include <semaphore.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/basetype.h>
#include <sys/error.h>
#include <sys/assert.h>
#include <sys/list.h>
#include <sys/epoll.h>
#include <sys/in.h>

#include "../../include/iscsi_com.h"
#include "../../include/iscsi_event.h"
#include "../../include/iscsi_util.h"

#include "../../include/iscsi_basetype.h"
#include "../../include/iscsi_packet.h"
#include "../../include/iscsi_main.h"

#include "iSNSLinux.h"
#include "iSNStypes.h"
#include "iSNS.h"
#include "iSNScomm.h"
#include "iSNSdb.h"
#include "iSNSmsg.h"
#include "iSNSqueue.h"
#include "iSNSfsm.h"
#include "iSNScomm.h"
#include "iSNSbuffer.h"
#include "iSNSipc.h"
#include "iSNSdebug.h"
#include "iSNSevent.h"
#include "iSNSEpoll.h"

extern pthread_cond_t sns_status_cond;

extern int sns_status;

/*********************************************************************
     Func Name : ISCSI_UPDT_HandleStopWork
  Date Created : 2010/10/23
        Author : y07661
   Description : 进程Reset第一阶段处理
         Input : IN VOID* pMsg      Reset消息指针
        Output : 无
        Return : 无
       Caution : 无
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
VOID ISCSI_ISNS_HandleStopWork(IN VOID* pMsg)
{
    return;
}

/*********************************************************************
     Func Name : ISCSI_UPDT_HandleDisable
  Date Created : 2010/10/23
        Author : y07661
   Description : 进程Reset第二阶段处理
         Input : IN VOID* pMsg      Reset消息指针
        Output : 无
        Return : 无
       Caution : 无
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
VOID ISCSI_ISNS_HandleDisable(IN VOID* pMsg)
{
    sns_status = DISABLE;

    pthread_cond_signal(&sns_status_cond);

    return;
}

/*********************************************************************
     Func Name : ISCSI_UPDT_SysEnable
  Date Created : 2011/3/11
        Author : yangjun
   Description : 执行系统使能操作
         Input : IN UINT uiSysIndex     进程索引
                 IN UCHAR ucLevel       LEVEL
        Output : NONE
        Return :
       Caution : NONE
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
VOID ISCSI_ISNS_SysEnable(VOID)
{
    sns_status = ENABLE;

    pthread_cond_signal(&sns_status_cond);

    return;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

