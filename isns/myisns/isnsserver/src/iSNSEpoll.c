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

STATIC INT g_iIscsiIsnsEpollHdl = ISCSI_FD_INIT;     /*isns收包线程epoll*/

STATIC INT g_iIscsiIsnsMainEpollHdl = ISCSI_FD_INIT;     /*isns主线程epoll*/

STATIC ISCSI_RECV_LIST_S   g_stIsnsRecvFdList;       /*保存fd和eopll的对应关系*/

extern IPC_EP  ipc_ep[NUM_IPC_EP_NAMES+1];
/*****************************************************************************
    Func Name: ISCSI_ISNS_RecvFdListGet
 Date Created: 2016/10/5
       Author: lichao@dian
        Input: 无
       Output: 无
       Return: list指针
      Caution: 运行在所有线程中
  Description: 获取保存接收配置fd的list
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
*****************************************************************************/
ISCSI_RECV_LIST_S *ISCSI_ISNS_RecvFdListGet(VOID)
{
    return &g_stIsnsRecvFdList;
}

/*********************************************************************
     Func Name : ISCSI_ISNS_EpollHandleGet
  Date Created : 2016/10/4
        Author : lichao@dian
   Description : 获取Epoll句柄
         Input :
        Output : NONE
        Return : INT   epoll句柄
       Caution : NONE
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
INT ISCSI_ISNS_EpollHandleGet(VOID)
{
    return g_iIscsiIsnsEpollHdl;
}

/*********************************************************************
     Func Name : ISCSI_ISNS_EpollHandleSet
  Date Created : 2016/10/4
        Author : lichao@dian
   Description : 设置Epoll句柄
         Input : INT iEpollHandle   epoll句柄
        Output : NONE
        Return : NONE
       Caution : NONE
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
VOID ISCSI_ISNS_EpollHandleSet(IN INT iEpollHandle)
{
    g_iIscsiIsnsEpollHdl = iEpollHandle;

    return;
}

/*********************************************************************
     Func Name : ISCSI_ISNS_EpollHandleGet
  Date Created : 2016/10/4
        Author : lichao@dian
   Description : 获取Epoll句柄
         Input :
        Output : NONE
        Return : INT   epoll句柄
       Caution : NONE
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
INT ISCSI_ISNS_MainEpollHandleGet(VOID)
{
    return g_iIscsiIsnsMainEpollHdl;
}

/*********************************************************************
     Func Name : ISCSI_ISNS_EpollHandleSet
  Date Created : 2016/10/4
        Author : lichao@dian
   Description : 设置Epoll句柄
         Input : INT iEpollHandle   epoll句柄
        Output : NONE
        Return : NONE
       Caution : NONE
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
VOID ISCSI_ISNS_MainEpollHandleSet(IN INT iEpollHandle)
{
    g_iIscsiIsnsMainEpollHdl = iEpollHandle;

    return;
}

/*********************************************************************
     Func Name : ISCSI_ISNS_EpollInitv
  Date Created : 2016/10/4
        Author : lichao@dian
   Description : 初始化主线程EPOLL
         Input : 无
        Output : 无
        Return : ERROR_SUCCESS
                 ERROR_FAILED
       Caution : NONE
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
ULONG ISCSI_ISNS_EpollInit(VOID)
{
    INT iEpollHdl = ISCSI_FD_INIT;
    ULONG ulRet = ERROR_SUCCESS;

    /*创建epoll*/
    iEpollHdl = epoll_create(1);
    if (0 > iEpollHdl)
    {
        return ERROR_FAILED;
    }
    ISCSI_ISNS_EpollHandleSet(iEpollHdl);

    return ulRet;
}

/*********************************************************************
     Func Name : ISCSI_ISNS_EpollDeInit
  Date Created : 2016/10/4
        Author : lichao@dian
   Description : 去初始化主线程EPOLL
         Input : VOID
        Output : NONE
        Return : NONE
       Caution : NONE
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
VOID ISCSI_ISNS_EpollDeInit(VOID)
{
    INT iEpoll = ISCSI_ISNS_EpollHandleGet();

    /*删除epoll*/
    if (ISCSI_FD_INIT == iEpoll)
    {
        return;
    }

    ISCSI_UTL_FdListDestroy(ISCSI_ISNS_RecvFdListGet());

    /*销毁epoll handle*/
    ISCSI_ISNS_EpollHandleSet(ISCSI_FD_INIT);
    ISCSI_UTL_EpollDestroy(iEpoll);

    return ;
}

/*********************************************************************
     Func Name : ISCSI_ISNS_EpollInit
  Date Created : 2016/10/4
        Author : lichao@dian
   Description : 初始化主线程EPOLL
         Input : 无
        Output : 无
        Return : ERROR_SUCCESS
                 ERROR_FAILED
       Caution : NONE
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
ULONG ISCSI_ISNS_MainEpollInit(VOID)
{
    INT iEpollHdl = ISCSI_FD_INIT;
    ULONG ulRet = ERROR_SUCCESS;

    /*创建epoll*/
    iEpollHdl = epoll_create(1);
    if (0 > iEpollHdl)
    {
        return ERROR_FAILED;
    }
    ISCSI_ISNS_MainEpollHandleSet(iEpollHdl);

    return ulRet;
}

/*********************************************************************
     Func Name : ISCSI_ISNS_EpollDeInit
  Date Created : 2016/10/4
        Author : lichao@dian
   Description : 去初始化主线程EPOLL
         Input : VOID
        Output : NONE
        Return : NONE
       Caution : NONE
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
VOID ISCSI_ISNS_MainEpollDeInit(VOID)
{
    INT iEpoll = ISCSI_ISNS_MainEpollHandleGet();

    /*删除epoll*/
    if (ISCSI_FD_INIT == iEpoll)
    {
        return;
    }

    ISCSI_UTL_EpollDelFd(iEpoll, ipc_ep[SNS_EP].s);
    ISCSI_UTL_EpollDelFd(iEpoll, ISCSI_ISNS_EventFdGet());

    /*销毁epoll handle*/
    ISCSI_ISNS_MainEpollHandleSet(ISCSI_FD_INIT);
    ISCSI_UTL_EpollDestroy(iEpoll);

    return ;
}

