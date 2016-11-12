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

/*
 * This file contains source code for implementing the
 * main processing loop for the SoIP service task.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sched.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/basetype.h>
#include <sys/error.h>
#include <sys/epoll.h>
#include <sys/list.h>
#include <sys/in.h>
#include <sys/eventfd.h>
#include <sys/assert.h>

#include "../../include/iscsi_basetype.h"
#include "../../include/iscsi_com.h"
#include "../../include/iscsi_main.h"
#include "../../include/iscsi_util.h"
#include "../../include/iscsi_event.h"

#include "iSNSLinux.h"
#include "iSNStypes.h"
#include "iSNS.h"
#include "iSNScomm.h"
#include "iSNSmsg.h"
#include "iSNSqueue.h"
#include "iSNSfsm.h"
#include "iSNSbuffer.h"
#include "iSNSipc.h"
#include "iSNSparse.h"
#include "iSNStbl.h"
#include "iSNSdb.h"
#include "iSNSreg.h"
#include "iSNSquery.h"
#include "iSNSesi.h"
#include "iSNSscn.h"
#include "iSNSdebug.h"
#include "iSNSEpoll.h"
#include "iSNSevent.h"
#include "iSNSreset.h"

STATIC INT g_iIsnsEventFd  = ISCSI_FD_INIT;
STATIC RWSTQ_HEAD_S       g_stIsnsEventMsgQue;
STATIC RWSTQ_HEAD_S       g_stIsnsEventDataQue;

/*********************************************************************
     Func Name : ISCSI_ISNS_ThreadEventMsgQueGet
  Date Created : 2016/10/10
        Author : lichao@dian
   Description : 获取线程调度事件队列
         Input : 无
        Output : 无
        Return : RWSTQ_HEAD_S*   调度事件队列
       Caution : 无
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
RWSTQ_HEAD_S* ISCSI_ISNS_ThreadEventMsgQueGet(VOID)
{
    return &g_stIsnsEventMsgQue;
}

/*********************************************************************
     Func Name : ISCSI_ISNS_ThreadEventDataQueGet
  Date Created : 2016/10/10
        Author : lichao
   Description : 获取线程调度数据队列
         Input : 无
        Output : 无
        Return : RWSTQ_HEAD_S*   调度数据队列
       Caution : 无
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
RWSTQ_HEAD_S* ISCSI_ISNS_ThreadEventDataQueGet(VOID)
{
    return &g_stIsnsEventDataQue;
}

/*********************************************************************
     Func Name : ISCSI_ISNS_EventFdGet
  Date Created : 2016/10/10
        Author : lichao@dian
   Description : 获取EventFd句柄
         Input : 无
        Output : 无
        Return : INT   epoll句柄
       Caution : 无
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
INT ISCSI_ISNS_EventFdGet(VOID)
{
    return g_iIsnsEventFd;
}

/*********************************************************************
     Func Name : ISCSI_ISNS_EventFdSet
  Date Created : 2016/10/10
        Author : lichao@dian
   Description : 设置EventFd句柄
         Input : INT iEventFd   EVENT句柄
        Output : 无
        Return : 无
       Caution : 无
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
VOID ISCSI_ISNS_EventFdSet(IN INT iEventFd)
{
    g_iIsnsEventFd = iEventFd;

    return;
}

/*********************************************************************
     Func Name : isUpdtMsgProcessReset
  Date Created : 2010/09/08
        Author : yangjun
   Description : RESET消息处理
         Input : IN VOID *pMsg    消息结构
        Output : NONE
        Return :
       Caution : NONE
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
STATIC VOID iscsi_isns_ResetMsgProcess(IN VOID *pMsg)
{
    ISCSI_ISNS_EVENT_RESET_MSG_S *pstIsnsMsg = (ISCSI_ISNS_EVENT_RESET_MSG_S *)pMsg;

    switch (pstIsnsMsg->stIsMsgHdr.uiSubMsgType)
    {
        case ISCSI_RESET_STOPWORK:
        {
            ISCSI_ISNS_HandleStopWork(pMsg);
            break;
        }

        case ISCSI_RESET_DISABLE:
        {
            ISCSI_ISNS_HandleDisable(pMsg);
            break;
        }

        case ISCSI_RESET_ENABLE:
        {
            ISCSI_ISNS_SysEnable();
            break;
        }

        default:
        {
            DBGASSERT(0);
            break;
        }
    }


    return;
}

/*********************************************************************
     Func Name : iscsi_isns_ProcessOneThreadMsg
  Date Created : 2016/10/311
        Author : lichao@dian
   Description : 单个消息处理函数
         Input : IN VOID* pMsg    消息结构
        Output : 无
        Return : 无
       Caution : 无
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
STATIC VOID iscsi_isns_ProcessOneThreadMsg(IN VOID* pMsg)
{
    //ISCSI_THREAD_MSG_HEAD_S *pstMsgHead = (ISCSI_THREAD_MSG_HEAD_S *)pMsg;

    /*调用相应函数进行处理，待调整，目前只处理了使能和去使能信号*/

    iscsi_isns_ResetMsgProcess(pMsg);

    return;
}

/*****************************************************************************
    Func Name: ISCSI_ISNS_FreeOneThreadQueueMsg
 Date Created: 2016/10/11
       Author: lichao@dian
        Input: IN VOID* pMsg, 线程消息指针
       Output: 无
       Return: 无
      Caution: 无
  Description: 释放线程消息中的复合资源线程消息
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
*****************************************************************************/
VOID ISCSI_ISNS_FreeOneThreadQueueMsg(IN VOID* pMsg)
{
    ISCSI_THREAD_MSG_HEAD_S *pstMsgHead;
    UINT uiMsgType;

    DBGASSERT (NULL != pMsg);
    pstMsgHead = (ISCSI_THREAD_MSG_HEAD_S *)pMsg;
    uiMsgType = pstMsgHead->uiMsgType;

    /*调用相应函数进行处理*/

    free(pMsg);
    return;
}

/*********************************************************************
     Func Name : ISCSI_ISNS_EventProcess
  Date Created : 2016/10/11
        Author : lichao@dian
   Description : ISNS模块事件处理入口
         Input : IN INT iFd          文件字
                 IN UINT uiEvent     事件类型
        Output : 无
        Return : 无
       Caution : 无
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
ULONG ISCSI_ISNS_EventProcess(IN UINT uiEvent, IN VOID *pHandle)
{
    RWSTQ_HEAD_S *pstEventMsgQue = ISCSI_ISNS_ThreadEventMsgQueGet();
    RWSTQ_HEAD_S *pstEventDataQue = ISCSI_ISNS_ThreadEventDataQueGet();
    eventfd_t stData = 0;
    INT iFd = (INT)(LONG)pHandle;

    if (0 != (uiEvent & EPOLLIN))
    {
        (VOID)eventfd_read(iFd, &stData);

        /* 线程消息队列消息处理 */
        ISCSI_EVT_ThreadQueueProcess(iFd,
                                  pstEventMsgQue,
                                  pstEventDataQue,
                                  iscsi_isns_ProcessOneThreadMsg,
                                  ISCSI_ISNS_FreeOneThreadQueueMsg);
    }

    return ERROR_SUCCESS;
}

