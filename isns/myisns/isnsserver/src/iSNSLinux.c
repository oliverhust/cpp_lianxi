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


extern int sns_ready;
extern int sns_local_priority;
extern int sns_local_version;
extern int sns_heartbeat_interval;
extern int sns_esi_interval;
extern int sns_request_timeout;
extern int sns_comm_main_port_set;

/* Linux */
extern pthread_cond_t esi_cond;
extern pthread_cond_t fsm_cond;
extern pthread_cond_t resync_cond;
extern pthread_mutex_t sns_request_timer;
extern pthread_cond_t request_cond;
extern pthread_mutex_t sns_resync_timer;
extern pthread_mutex_t sns_fsm_timer;
extern pthread_mutex_t sns_esi_timer;

/* log */
int daemon_state = TRUE;

/* debug */
extern int sns_bcast_debug;
extern int sns_cb_debug;
extern int sns_fsm_debug;
extern int sns_hb_debug;
extern int isns_main_debug;
extern int sns_recv_debug;
extern int sns_rsync_debug;
extern int sns_scn_debug;
extern int sns_comm_debug;
extern int isns_db_debug;
extern int isns_list_debug;
extern int isns_parse_debug;
extern int sns_tcp_debug;
extern int isns_query_debug;
extern int isns_reg_debug;
extern int isns_dereg_debug;
extern int isns_esi_debug;

int pauseFlag = FALSE;
int snsServiceFlag = FALSE;

/*ISNS维护线程ID*/
STATIC pthread_t  g_stIsnsThreadID = 0;
STATIC ISCSI_THREAD_STATUS_E g_enIsnsThreadStatus = ISCSI_THREAD_STOPPED;

extern void SNSReqTimeoutHdlr(void);


HANDLE init_ipc_rcv (void);
void DebugOn (char *);

void
taskDelay (int time)
{
   sleep (time);
}

int
sysClkRateGet (void)
{
   return (1);
}

void
taskDelete (pthread_t task)
{
   if (task == 0)
      return;

   if ((void *)task != NULL)
   {
     pthread_cancel (task);
   }
}

void
taskSuspend (void)
{
   return;
}

int
taskIdSelf (void)
{
   return (0);
}

/* Linux */
int
LinuxTaskSpawn (char *taskName, void *funcPtr)
{
  int result;
  pthread_t athread;
  result =  (pthread_create (&athread, NULL, (void*) funcPtr, NULL));
  if (result != 0)
    perror ("taskSpawn");
  return (result);
}

char Usage[] = "\
Usage: isnss [options] -i IP -b BROADCAST IP\n\
NOTES: IP-Addresses must be in IP dotted notation.\n\
Options:\n\
   [-a addr]   IP-Address for SNSP traffic.\n\
   [-b addr]   Broadcast Address for SNSP traffic.\n\
   [-p port]   primary port for SNSP traffic.\n\
   [-d switch] debug options: parse,db,list,reg,fsm,esi,scn,all \n\
   [-f]        Foreground - no daemon.\n\
   [-?]        Help.\n";

/*********************************************************************
     Func Name : ISCSI_ISNS_ThreadIdGet
  Date Created : 2016/9/27
        Author : lichao@dian
   Description : 获取ISNS线程ID
         Input : 无
        Output : 无
        Return : pthread_t    线程ID
       Caution : 无
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
pthread_t ISCSI_ISNS_ThreadIdGet(VOID)
{
    return g_stIsnsThreadID;
}

/*********************************************************************
     Func Name : ISCSI_ISNS_ThreadIdSet
  Date Created : 2016/9/27
        Author : lichao@dian
   Description : 设置ISNS线程ID
         Input : pthread_t stThreadId    线程ID
        Output : 无
        Return : 无
       Caution : 无
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
VOID ISCSI_ISNS_ThreadIdSet(IN pthread_t stThreadId)
{
    g_stIsnsThreadID = stThreadId;

    return;
}

/*********************************************************************
     Func Name : ISCSI_ADJ_ThreadStatusSet
  Date Created : 2016/10/4
        Author : lichao@dian
   Description : 设置ISNS线程状态
         Input : pthread_t stThreadId    线程ID
        Output : 无
        Return : 无
       Caution : 无
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
VOID ISCSI_ISNS_ThreadStatusSet(ISCSI_THREAD_STATUS_E enThreadStatus)
{
    g_enIsnsThreadStatus = enThreadStatus;

    return;
}
/*****************************************************************************
    Func Name: ISCSI_ISNS_ThreadStatusGet
 Date Created: 2016/10/4
       Author: lichao@dian
  Description: 获取线程状态
        Input: 无
       Output: 无
       Return: ISCSI_THREAD_STATUS_E 线程状态
      Caution: 运行在所有线程中
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
*****************************************************************************/
ISCSI_THREAD_STATUS_E ISCSI_ISNS_ThreadStatusGet(VOID)
{
    return g_enIsnsThreadStatus;
}

/*********************************************************************
     Func Name : iscsi_isns_ThreadDestroy
  Date Created : 2016/9/27
        Author : lichao@dian
   Description : ISNS线程销毁
         Input : 无
        Output : 无
        Return : ERROR_SUCCESS 成功
       Caution : 无
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
STATIC ULONG iscsi_isns_ThreadDestroy(VOID)
{
    VOID *pRes = NULL;
    pthread_t stThreadId = ISCSI_ISNS_ThreadIdGet();
    INT iRet = 0;

    if(0 != stThreadId)
    {
        /*同步等待信号删除结束*/
        iRet = pthread_join (stThreadId, &pRes);
        DBGASSERT(0 == iRet);

        /*线程ID清0*/
        ISCSI_ISNS_ThreadIdSet(0UL);

    }

    (VOID)pRes;
    (VOID)iRet;

    return ERROR_SUCCESS;
}

/*****************************************************************************
    Func Name: ISCSI_ISNS_ThreadMain
 Date Created: 2016-9-27
       Author: lichao@dian
  Description: 线程主函数
        Input: IN VOID *pThreadInfo
       Output:
       Return: VOID
      Caution:
-----------------------------------------------------------------------------
   Modification History
   DATE        NAME             DESCRIPTION
-----------------------------------------------------------------------------

******************************************************************************/
VOID *ISCSI_ISNS_ThreadMain(IN VOID *pThreadInfo)
{
    IGNORE_PARAM(pThreadInfo);

    ISNS_Entity role = ISNS_SECONDARY_SERVER;

    sns_bcast_debug = -1;
    sns_cb_debug = -1;
    sns_comm_debug = -1;
    isns_db_debug = -1;
    sns_fsm_debug = -1;
    sns_hb_debug = -1;
    isns_main_debug = -1;
    sns_recv_debug = -1;
    sns_rsync_debug = -1;
    sns_scn_debug = -1;
    isns_parse_debug = -1;
    sns_tcp_debug = -1;
    isns_list_debug = -1;
    isns_query_debug = -1;
    isns_reg_debug = -1;
    isns_dereg_debug = -1;
    isns_esi_debug = -1;

    SNSMain (role);

    return NULL;
}

/*********************************************************************
     Func Name : iscsi_isns_SystemDeInit
  Date Created : 2016/9/27
        Author : lichao@dian
   Description : ISNS去初始化
                 IN BOOL_T bDelThread    是否销毁线程
        Output : 无
        Return : ERROR_SUCCESS   成功
       Caution : 无
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
STATIC ULONG iscsi_isns_SystemDeInit(IN BOOL_T bDelThread)
{
    ULONG ulRet = ERROR_SUCCESS;

    /*邻居维护线程销毁*/
    if(bDelThread)
    {
        ulRet = iscsi_isns_ThreadDestroy();
    }

    return ulRet;
}

/*********************************************************************
     Func Name : iscsi_isns_ThreadCreate
  Date Created : 2016/9/27
        Author : lichao@dian
   Description : ISNS线程创建
         Input : 无
        Output : 无
        Return : ERROR_SUCCESS   成功
                 ERROR_FAILED    失败
       Caution : 无
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
STATIC ULONG iscsi_isns_ThreadCreate(VOID)
{
    ULONG ulRet = ERROR_SUCCESS;
    INT iStatus = 0;
    pthread_t  stThreadID = 0;

    /*创建线程*/
    iStatus = pthread_create(&stThreadID,
                             NULL,
                             ISCSI_ISNS_ThreadMain,
                             NULL);
    if (0 == iStatus)
    {
        ISCSI_ISNS_ThreadIdSet(stThreadID);
    }
    else
    {
        ulRet = ERROR_FAILED;
    }

    return ulRet;
}

/*********************************************************************
     Func Name : iscsi_isns_SystemInit
  Date Created : 2016/9/27
        Author : lichao@dian
   Description : ISNS线程创建
                 IN BOOL_T bCreateThread   是否创建线程
        Output : 无
        Return : ERROR_SUCCESS   成功
                 ERROR_FAILED    失败
       Caution : 无
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
STATIC ULONG iscsi_isns_SystemInit(IN BOOL_T bCreateThread)
{
    ULONG ulRet = ERROR_SUCCESS;

    /*邻居维护线程创建*/
    if (bCreateThread)
    {
        ulRet = iscsi_isns_ThreadCreate();
    }

    return ulRet;
}

/*****************************************************************************
    Func Name: iscsi_isns_DeInit
 Date Created: 2016-10-18
       Author: lichao@dian
  Description: isns线程去初始化
        Input: VOID
       Output:
       Return: STATIC
      Caution:
-----------------------------------------------------------------------------
   Modification History
   DATE        NAME             DESCRIPTION
-----------------------------------------------------------------------------

******************************************************************************/
STATIC VOID iscsi_isns_DeInit(VOID)
{
    RWSTQ_HEAD_S* pstEventMsgQue = ISCSI_ISNS_ThreadEventMsgQueGet();
    RWSTQ_HEAD_S* pstEventDataQue = ISCSI_ISNS_ThreadEventDataQueGet();

    /*删除主线程消息队列*/
    ISCSI_EVT_ThreadQueueDeInit(pstEventMsgQue,pstEventDataQue, ISCSI_ISNS_FreeOneThreadQueueMsg);

    /*释放全局EPOLL*/
    ISCSI_ISNS_EpollDeInit();

    ISCSI_ISNS_MainEpollDeInit();

    return ;
}

/*****************************************************************************
    Func Name: ISCSI_ISNS_Init
 Date Created: 2016-9-27
       Author: lichao@dian
  Description: ISNS初始化
        Input:
       Output:
       Return: int
      Caution:
-----------------------------------------------------------------------------
   Modification History
   DATE        NAME             DESCRIPTION
-----------------------------------------------------------------------------

******************************************************************************/
int ISCSI_ISNS_Init()
{
    /*注册system相关资源创建、销毁函数、RESET函数*/
   ISCSI_SYS_RegCb(ISCSI_ISNS_MODULE_ID, iscsi_isns_SystemInit, iscsi_isns_SystemDeInit);

   /*注册线程状态获取回调函数*/
   ISCSI_MAIN_ThreadStatusRegCb(ISCSI_ISNS_MODULE_ID, ISCSI_ISNS_ThreadStatusGet);

   /*注册reset通知函数*/
   ISCSI_RESET_RegNotifyCb(ISCSI_ISNS_MODULE_ID, ISCSI_ISNS_ResetNotify);

   return 0;
}

/*****************************************************************************
    Func Name: ISCSI_ISNS_Init
 Date Created: 2016-9-27
       Author: lichao@dian
  Description: ISNS初始化
        Input:
       Output:
       Return: int
      Caution:
-----------------------------------------------------------------------------
   Modification History
   DATE        NAME             DESCRIPTION
-----------------------------------------------------------------------------

******************************************************************************/
int ISCSI_ISNS_DelInit()
{
    /*反注册system相关资源创建、销毁函数*/
    ISCSI_SYS_DeRegCb(ISCSI_ISNS_MODULE_ID);

    /*反注册线程消息回调函数*/
    ISCSI_MAIN_ThreadStatusDeRegCb(ISCSI_ISNS_MODULE_ID);

    /* 资源去初始化 */
    iscsi_isns_DeInit();

   return 0;
}

/*********************************************************
  SNSReqTimeoutThread
*********************************************************/
int
SNSReqTimeoutThread ()
{
   struct timeval now;
   struct timespec timeout;
   int rc;

   __DEBUG ( isns_main_debug & 16,"SNSReqTimeoutHdlr Started.");

   while (1)
   {
    pthread_mutex_lock(&sns_request_timer);
    gettimeofday(&now, NULL);
    timeout.tv_sec = now.tv_sec + sns_request_timeout;
    timeout.tv_nsec = now.tv_usec * 1000;
    rc = pthread_cond_timedwait (&request_cond,&sns_request_timer,&timeout);
        if (rc != ETIMEDOUT)
        perror ("Error in Timeout Request in SNSReqTimeoutThread");

    pthread_mutex_unlock(&sns_request_timer);
    SNSReqTimeoutHdlr ();

      if (pauseFlag)
      {
            return (0);
      }
   }

   return (0);
}

/***********************************************************
SSNSFSMTimeoutThread
***********************************************************/
/* Linux */

int
SNSFSMTimeoutThread ()
{
   struct timeval now;
   struct timespec timeout;
   int rc;

   __DEBUG (isns_main_debug & 1666666,"SNSFSMTimeoutHdlr thread started.");

   while (1)
   {
      pthread_mutex_lock(&sns_fsm_timer);
      gettimeofday(&now, NULL);

      timeout.tv_sec = now.tv_sec + sns_heartbeat_interval;
      timeout.tv_nsec = now.tv_usec * 1000;

      rc = pthread_cond_timedwait(&fsm_cond, &sns_fsm_timer, &timeout);
      if (rc != ETIMEDOUT)
        perror ("Error in Timeout Request in SNSFSMTimeoutThread");

      SNSFSMTimeoutHdlr ();

      pthread_mutex_unlock(&sns_fsm_timer);

      if (pauseFlag)
         return (0);
   }

   return (0);
}

/*********************************************************
SNSESITimeoutThread
*********************************************************/

/* Linux */
int
SNSESITimeoutThread ()
{
   struct timeval now;
   struct timespec timeout;
   int rc;

   __DEBUG (isns_main_debug & 16,"SNSESITimeoutHdlr thread started.");

   while (1)
   {
      pthread_mutex_lock(&sns_esi_timer);
      gettimeofday(&now, NULL);
      timeout.tv_sec = now.tv_sec + sns_esi_interval;
      timeout.tv_nsec = now.tv_usec * 1000;
      rc = pthread_cond_timedwait(&esi_cond, &sns_esi_timer, &timeout);
      if (rc != ETIMEDOUT)
        perror ("Error in Timeout Request in SNSESITimeoutThread");

      SNSESITimeoutHdlr ();
      pthread_mutex_unlock(&sns_esi_timer);
      if (pauseFlag)
       return (0);
   }
   return (0);
}

/***********************************************************
SNSResyncTimeoutThread
***********************************************************/
int
SNSResyncTimeoutThread ()
{
   struct timeval now;
   struct timespec timeout;
   int rc;

   while (1)
   {
      pthread_mutex_lock(&sns_resync_timer);
      gettimeofday(&now, NULL);
      timeout.tv_sec = now.tv_sec + 60; //sns_resync_interval;
      timeout.tv_nsec = now.tv_usec * 1000;
      rc = pthread_cond_timedwait(&resync_cond, &sns_resync_timer, &timeout);
      if (rc != ETIMEDOUT)
    perror ("Error in Timeout Request in SNSResyncTimeoutThread");

      pthread_mutex_unlock(&sns_resync_timer);
      if (pauseFlag)
    return (0);
   }

   return (0);
}

void
DebugOn (char * type)
{

   if (!strcmp(type,"parse"))
   {
     isns_parse_debug = -1;
     isns_main_debug = -1;
     sns_comm_debug = -1;
     sns_tcp_debug = -1;
   }
   else if (!strcmp(type,"tcp"))
   {
     sns_comm_debug = -1;
     sns_tcp_debug = -1;
   }
   else if (!strcmp(type,"db"))
   {
     isns_db_debug = -1;
   }
   else if (!strcmp(type,"list"))
   {
     isns_list_debug = -1;
   }
   else if (!strcmp(type,"query"))
   {
     isns_query_debug = -1;
   }
   else if (!strcmp(type,"reg"))
   {
     isns_reg_debug = -1;
     isns_dereg_debug = -1;
   }
   else if (!strcmp(type,"fsm"))
   {
     sns_fsm_debug = -1;
     sns_hb_debug = -1;
     sns_cb_debug = -1;
   }
   else if (!strcmp(type,"esi"))
   {
     isns_esi_debug = -1;
   }
   else if (!strcmp(type,"scn"))
   {
     sns_scn_debug = -1;
   }
   else if (!strcmp(type,"all"))
   {
     sns_bcast_debug = -1;
     sns_cb_debug = -1;
     sns_comm_debug = -1;
     isns_db_debug = -1;
     sns_fsm_debug = -1;
     sns_hb_debug = -1;
     isns_main_debug = -1;
     sns_recv_debug = -1;
     sns_rsync_debug = -1;
     sns_scn_debug = -1;
     isns_parse_debug = -1;
     sns_tcp_debug = -1;
     isns_list_debug = -1;
     isns_query_debug = -1;
     isns_reg_debug = -1;
     isns_dereg_debug = -1;
     isns_esi_debug = -1;
   }
}
