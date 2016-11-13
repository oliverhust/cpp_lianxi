#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sched.h>

#include <sys/basetype.h>
#include <sys/error.h>
#include <sys/list.h>
#include <sys/in.h>
#include <sys/socket.h>

#include "../iscsi/include/iscsi_com.h"
#include "../iscsi/include/iscsi_util.h"
#include "../iscsi/include/iscsi_event.h"

#include "../iscsi/include/iscsi_basetype.h"
#include "../iscsi/include/iscsi_packet.h"
#include "../iscsi/include/iscsi_main.h"

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

#include "stub.h"

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

enum
{
    STUB_SOCK = 0,
    STUB_SETSOCK,
};


STATIC struct func_stub g_stStubFun[64] = { { 0 } };

static int ut_socket(int domain, int type, int protocol)
{
    int iRet;

    if(type == SOCK_DGRAM && 0 == protocol)
    {
        protocol = IPPROTO_UDP;
    }
    else if(type == SOCK_STREAM && 0 == protocol)
    {
        protocol = IPPROTO_TCP;
    }

    stub_reset(&g_stStubFun[STUB_SOCK]);
    iRet = socket(domain, type, protocol);
    stub_set(&g_stStubFun[STUB_SOCK], socket, ut_socket);

    return iRet;
}

static int ut_setsockopt (int fd, int level, int optname, const void *optval, socklen_t optlen)
{
    stub_reset(&g_stStubFun[STUB_SETSOCK]);
    if(0 != setsockopt(fd, level, optname, optval, optlen))
    {
        printf("Warning: ut_setsockopt failed.\r\n");
    }
    stub_set(&g_stStubFun[STUB_SETSOCK], setsockopt, ut_setsockopt);

    return 0;
}

STATIC VOID _set_stub()
{
    memset(g_stStubFun, 0, sizeof(g_stStubFun));

    stub_set(&g_stStubFun[STUB_SOCK], socket, ut_socket);
    stub_set(&g_stStubFun[STUB_SETSOCK], setsockopt, ut_setsockopt);
}

STATIC INT _isns_main()
{
    sns_bcast_debug = 1;
    sns_cb_debug = 1;
    sns_comm_debug = 1;
    isns_db_debug = 1;
    sns_fsm_debug = 1;
    sns_hb_debug = 1;
    isns_main_debug = 1;
    sns_recv_debug = 1;
    sns_rsync_debug = 1;
    sns_scn_debug = 1;
    isns_parse_debug = 1;
    sns_tcp_debug = 1;
    isns_list_debug = 1;
    isns_query_debug = 1;
    isns_reg_debug = 1;
    isns_dereg_debug = 1;
    isns_esi_debug = 1;

    return SNSMain(ISNS_SECONDARY_SERVER);
}

INT main()
{
    INT iRet;

    _set_stub();

    iRet = _isns_main();

    return iRet;
}


VOID ISCSI_UTL_GetSysTime(IN ULONG *pulTmNowSec, IN ULONG *pulTmNowInMsec)
{
    return ;
}

ULONG ISCSI_UTL_GetSysTimeInSec(VOID)
{
    return 0;
}

ULONG ISCSI_UTL_GetSysTimeInMSec(VOID)
{
    return 0;
}

ULONG ISCSI_UTL_GetManaIPAddr(OUT INET_ADDR_S *pstAddr)
{
    return 0;
}

ULONG ISCSI_UTL_GetTimeDiff(IN ULONG ulNewTmInMsec, IN ULONG ulOldTmInMsec)
{
    return 0;
}

LONG ISCSI_UTL_CompareAddress(IN const UCHAR* pucAddr1, IN UCHAR ucAddr1Len,
                           IN const UCHAR* pucAddr2, IN UCHAR ucAddr2Len)
{
    return 0;
}

ULONG ISCSI_UTL_ComputeHashIndex(IN ULONG ulHashSize, IN UCHAR* pucKey, IN UCHAR ucKeyLen)
{
    return 0;
}

INT ISCSI_UTL_EpollAddFd(IN INT iEpollHandle, IN INT iFd,
                      IN UINT uiEvents, IN ISCSI_EPOLL_CALLBACK_PF pfFunc)
{
    return 0;
}

INT ISCSI_UTL_EpollModFd(IN INT iEpollHandle, IN INT iFd,
                      IN UINT uiEvents, IN ISCSI_EPOLL_CALLBACK_PF pfFunc)
{
    return 0;
}

INT ISCSI_UTL_EpollDelFd(IN INT iEpollHandle, IN INT iFd)
{
    return 0;
}

VOID ISCSI_UTL_EpollDestroy(IN INT iEpollHandle)
{
    return ;
}

VOID ISCSI_UTL_EpollEventProc(IN INT iEpollHandle)
{
    return ;
}

VOID ISCSI_UTL_SystemIdToSourceId(IN const ISCSI_SYSTEM_ID_S * pstSysId, OUT ISCSI_SOURCE_ID_S * pstTgtSrcId)
{
    return ;
}

LONG ISCSI_UTL_SystemIDCmp(IN const ISCSI_SYSTEM_ID_S *pstSysID1, IN const ISCSI_SYSTEM_ID_S *pstSysID2)
{
    return 0;
}

LONG ISCSI_UTL_SnpaCmp(IN const ISCSI_SNPA_ADDR_S *pstSnpa1, IN const ISCSI_SNPA_ADDR_S *pstSnpa2)
{
    return 0;
}

LONG ISCSI_UTL_SourceIDCmp(IN const ISCSI_SOURCE_ID_S *pstSrcID1, IN const ISCSI_SOURCE_ID_S *pstSrcID2)
{
    return 0;
}

CHAR* ISCSI_UTL_SystemIdToString(IN const UCHAR *pucSource,OUT CHAR *pcString)
{
    return 0;
}

CHAR* ISCSI_UTL_SourceIdToString(IN const UCHAR *pucSource,OUT CHAR *pcString)
{
    return 0;
}

VOID ISCSI_UTL_FdListDestroy(IN ISCSI_RECV_LIST_S *pstFdList)
{
    return ;
}

VOID ISCSI_UTL_FdListInit(IN INT iEpollHandle, INOUT ISCSI_RECV_LIST_S *pstFdList)
{
    return ;
}

ULONG ISCSI_UTL_RecvFdAdd(IN ISCSI_RECV_LIST_S *pstFdList, IN INT iFd, IN ISCSI_EPOLL_CALLBACK_PF pfEpollProc)
{
    return 0;
}

VOID ISCSI_UTL_RecvFdRemove(IN ISCSI_RECV_LIST_S *pstFdList, IN INT iFd)
{
    return ;
}

VOID ISCSI_UTL_PluginReply(IN ISCSI_RECV_LIST_S *pstFdList,
                        IN INT iFd,
                        IN ISCSI_PLUGIN_REPLY_E enFlag,
                        IN const VOID *pReplyData,
                        IN UINT uiReplyLen)
{
    return ;
}

VOID ISCSI_UTL_RecvFdCreate(IN ISCSI_RECV_LIST_S *pstFdList, IN INT iFd,
                         IN UINT uiEvent, IN ISCSI_EPOLL_CALLBACK_PF pfEpollProc)
{
    return ;
}

VOID ISCSI_UTL_RecvFdDestroy(IN ISCSI_RECV_LIST_S *pstFdList, IN INT iFd)
{
    return ;
}

VOID ISCSI_EVT_ThreadQueueInit(OUT RWSTQ_HEAD_S *pstMsgQueue,
                            OUT RWSTQ_HEAD_S *pstDataQueue)
{
    return ;
}

VOID ISCSI_EVT_ThreadQueueDeInit(IN RWSTQ_HEAD_S *pstMsgQueue,
                              IN RWSTQ_HEAD_S *pstDataQueue,
                              IN ISCSI_THREADMSG_FREE_CALLBACK_PF pfFreeOneQueueMsg)
{
    return ;
}

VOID ISCSI_EVT_ThreadQueueMsgAdd(IN INT         iEventFd,
                              IN RWSTQ_HEAD_S *pstThreadQueue,
                              IN VOID*        pNotifyMsg)
{
    return ;
}

VOID ISCSI_EVT_ThreadQueueProcess(IN INT iEventFd,
                               IN RWSTQ_HEAD_S *pstMsgQueue,
                               IN RWSTQ_HEAD_S *pstDataQueue,
                               IN ISCSI_THREADMSG_PROC_CALLBACK_PF pfProcCallBack,
                               IN ISCSI_THREADMSG_FREE_CALLBACK_PF pfFreeCallBack)
{
    return ;
}

ULONG ISCSI_PKT_CommHdrDecode(IN const UCHAR *pucBuff, OUT ISCSI_COMM_PKT_HDR_S *pstIsCommHdr)
{
    return 0;
}

VOID ISCSI_PKT_CommHdrEncode(IN UCHAR ucPduType, IN UCHAR ucHdrLen, OUT UCHAR *pucBuff)
{
    return ;
}

VOID ISCSI_PKT_MultiGroupMacGet(OUT UCHAR *pucSnpaAddr)
{
    return ;
}

VOID ISCSI_BPDU_MultiGroupMacGet( OUT UCHAR *pucMacAddr )
{
    return ;
}

ULONG ISCSI_PacketCommHdrLenCheck(IN UCHAR ucPduType, IN UCHAR ucHdrLen)
{
    return 0;
}

VOID  *ISCSI_PROTO_GetAdjData(VOID)
{
    return 0;
}

VOID  *ISCSI_PROTO_GetUpdateData(VOID)
{
    return 0;
}

VOID  ISCSI_PROTO_GetSourceMAC(OUT UCHAR* pucMacAddr)
{
    return ;
}

VOID  ISCSI_PROTO_SetAdjData(IN VOID * pAdjData)
{
    return ;
}

VOID  ISCSI_PROTO_SetUpdateData(IN VOID * pUpdateData)
{
    return ;
}

VOID ISCSI_PROTO_RegCb(IN ISCSI_MODULE_ID_E enModuleId,
                             IN ISCSI_PROTO_REGCB_PF pfProtoInit,
                             IN ISCSI_PROTO_REGCB_PF pfProtoDeInit)
{
    return ;
}

VOID ISCSI_PROTO_DeRegCb(IN ISCSI_MODULE_ID_E enModuleId)
{
    return ;
}

VOID ISCSI_MAIN_ThreadRegCb(IN ISCSI_MODULE_ID_E enModuleId, IN ISCSI_THREAD_REGCB_PF pfThreadCb)
{
    return ;
}

VOID ISCSI_MAIN_ThreadDeRegCb(IN ISCSI_MODULE_ID_E enModuleId)
{
    return ;
}

VOID ISCSI_MAIN_ThreadOptNotifyBack(IN IS_THREADOPT_MSGTYPE_E enNotifyType,
                                    IN ISCSI_MODULE_ID_E enModuleId)
{
    return ;
}

VOID  *ISCSI_INSTANCE_GetAdjData(IN VRF_INDEX vrfIndex)
{
    return 0;
}

VOID  *ISCSI_INSTANCE_GetUpdateData(IN VRF_INDEX vrfIndex)
{
    return 0;
}

ULONG  ISCSI_INSTANCE_SetAdjData(IN VRF_INDEX vrfIndex, IN VOID * pAdjData)
{
    return 0;
}

ULONG  ISCSI_INSTANCE_SetUpdateData(IN VRF_INDEX vrfIndex, IN VOID * pUpdateData)
{
    return 0;
}

VOID ISCSI_INSTANCE_RegCb(IN ISCSI_MODULE_ID_E enModuleId,
                          IN ISCSI_INST_REGCB_PF pfInstInit,
                          IN ISCSI_INST_REGCB_PF pfInstDeInit)
{
    return ;
}

VOID ISCSI_INSTANCE_DeRegCb(IN ISCSI_MODULE_ID_E enModuleId)
{
    return ;
}

VRF_INDEX ISCSI_INSTANCE_GetFirst(VOID)
{
    return 0;
}

VRF_INDEX ISCSI_INSTANCE_GetNext(IN VRF_INDEX vrfIndex)
{
    return 0;
}

ULONG ISCSI_SYS_GetInstId(OUT VRF_INDEX *pvrfIndex)
{
    return 0;
}

ULONG  ISCSI_SYS_SetLspBufsize(IN USHORT usSysLspBufsize)
{
    return 0;
}

ULONG  ISCSI_SYS_GetLspBufsize(OUT USHORT *pusLspBuff)
{
    return 0;
}

ULONG  ISCSI_SYS_GetMinMtusize(OUT USHORT *pusMinMtu)
{
    return 0;
}

VOID ISCSI_SYS_GetLspBackOffParam(OUT ISCSI_BACKOFF_PARAM_S* pstLspBackoffParam)
{
    return ;
}

ULONG ISCSI_SYS_GetSystemId(OUT ISCSI_SYSTEM_ID_S *pstSystemId)
{
    return 0;
}

VOID *ISCSI_SYS_GetUpdateData(VOID)
{
    return 0;
}

UINT ISCSI_SYS_GetRdpIntervalCfg(VOID)
{
    return 0;
}

ULONG ISCSI_SYS_SetUpdateData(IN VOID * pUpdateData)
{
    return 0;
}

VOID ISCSI_SYS_RegCb(IN ISCSI_MODULE_ID_E enModuleId,
                     IN ISCSI_SYS_REGCB_PF pfSysInit,
                     IN ISCSI_SYS_REGCB_PF pfSysDeInit)
{
    return ;
}

VOID ISCSI_SYS_DeRegCb(IN ISCSI_MODULE_ID_E enModuleId)
{
    return ;
}

USHORT ISCSI_SYS_GetFirstProcId(VOID)
{
    return 0;
}

USHORT ISCSI_SYS_GetNextProcId(USHORT usProcessId)
{
    return 0;
}

BOOL_T ISCSI_SYS_ErrorDbgIsOn(IN UCHAR ucCheckFlag)
{
    return 0;
}

BOOL_T ISCSI_SYS_EventDbgIsOn(IN UCHAR ucCheckFlag)
{
    return 0;
}

BOOL_T ISCSI_SYS_TimerDbgIsOn(IN UCHAR ucCheckFlag)
{
    return 0;
}

BOOL_T ISCSI_SYS_AdjPktDbgIsOn(IN IN UCHAR ucCircIndex, IN UCHAR ucCheckFlag)
{
    return 0;
}

BOOL_T ISCSI_SYS_SnpPktDbgIsOn(IN IN UCHAR ucCircIndex, IN UCHAR ucCheckFlag)
{
    return 0;
}

BOOL_T ISCSI_SYS_UpdtPktDbgIsOn(IN IN UCHAR ucCircIndex, IN UCHAR ucCheckFlag)
{
    return 0;
}

BOOL_T ISCSI_SYS_PktDbgIsOn(IN IN UCHAR ucCircIndex, IN UCHAR ucCheckFlag)
{
    return 0;
}

BOOL_T ISCSI_PROTO_HADbgIsOn(VOID)
{
    return 0;
}

ULONG  ISCSI_CIRC_GetName(IN UCHAR ucCircIndex, OUT CHAR *pcNameBuf)
{
    return 0;
}

ULONG ISCSI_CIRC_GetSnpaAddr(IN UCHAR ucCircIndex, OUT ISCSI_SNPA_ADDR_S *pstCircSnpaAddr)
{
    return 0;
}

ULONG  ISCSI_CIRC_GetMTUSize(IN UCHAR ucCircIndex, OUT ULONG *pulMTUSize)
{
    return 0;
}

ULONG ISCSI_CIRC_OperStateGet(IN UCHAR ucCircIndex, OUT UCHAR * pucCircOperState)
{
    return 0;
}

UINT ISCSI_CIRC_GetNetType(IN UCHAR ucCircIndex)
{
    return 0;
}

ULONG ISCSI_CIRC_GetIfNetIndex(IN UCHAR ucCircIndex,OUT IF_INDEX *pifIndex)
{
    return 0;
}

ULONG  ISCSI_CIRC_DeleteNotify(IN UCHAR ucCircIndex)
{
    return 0;
}

ULONG ISCSI_CIRC_IpStateGet(IN UCHAR ucCircIndex, OUT UCHAR * pucCircIpState)
{
    return 0;
}

ULONG ISCSI_CIRC_ActStateGet(IN UCHAR ucCircIndex, OUT BOOL_T *pbActive)
{
    return 0;
}

VOID  *ISCSI_CIRC_GetAdjData(IN UCHAR ucCircIndex)
{
    return 0;
}

VOID  *ISCSI_CIRC_GetAdjData_Force(IN UCHAR ucCircIndex)
{
    return 0;
}

VOID  *ISCSI_CIRC_GetUpdateData(IN UCHAR ucCircIndex)
{
    return 0;
}

VOID  *ISCSI_CIRC_GetUpdateData_Force(IN UCHAR ucCircIndex)
{
    return 0;
}

ULONG  ISCSI_CIRC_SetAdjData(IN UCHAR ucCircIndex, IN VOID * pAdjData)
{
    return 0;
}

ULONG  ISCSI_CIRC_SetUpdateData(IN UCHAR ucCircIndex, IN VOID * pUpdateData)
{
    return 0;
}

VOID ISCSI_CIRC_Delete(IN UCHAR ucCircIndex)
{
    return ;
}

ULONG ISCSI_CIRC_GetDelFlag(IN UCHAR ucCircIndex, OUT BOOL_T *pbDelFlag)
{
    return 0;
}

ULONG ISCSI_CIRC_GetModuleDelFlag(IN UCHAR ucCircIndex,OUT UCHAR *pucModuleDelFlag)
{
    return 0;
}

ULONG ISCSI_CIRC_SetModuleDelFlag(IN UCHAR ucCircIndex, IN ISCSI_MODULE_ID_E enEvent)
{
    return 0;
}

VOID ISCSI_CIRC_RegCb(IN ISCSI_MODULE_ID_E enModuleId, IN ISCSI_CIRC_REGCB_PF pfCircInit,
                          IN ISCSI_CIRC_REGCB_PF pfCircDeInit, IN ISCSI_CIRC_DEL_NTY_REGCB_PF pfCircDelNotify)
{
    return ;
}

VOID ISCSI_CIRC_DeRegCb(IN ISCSI_MODULE_ID_E enModuleId)
{
    return ;
}

UCHAR ISCSI_CIRC_GetFirst(VOID)
{
    return 0;
}

UCHAR ISCSI_CIRC_GetNext(IN UCHAR ucCircIndex)
{
    return 0;
}

VOID ISCSI_CIRC_GetCircAndSysIndexByIfName(IN const CHAR *pcIfName,
                                        OUT UCHAR *pucCircIndex)
{
    return ;
}

VOID ISCSI_CIRC_GetCircAndSysIndexByIfNetIndex(IN IF_INDEX ifIndex,
                                            OUT UCHAR *pucCircIndex)
{
    return ;
}

BOOL_T  ISCSI_CIRC_IsDeleting(IN UCHAR ucCircIndex)
{
    return 0;
}

VOID ISCSI_RESET_RegNotifyCb(IN ISCSI_MODULE_ID_E enModuleId, IN ISCSI_RESET_CB_REG_PF pfSysResetBegin)
{
    return ;
}

VOID ISCSI_RESET_DeRegNotifyCb(IN ISCSI_MODULE_ID_E enModuleId)
{
    return ;
}

VOID ISCSI_MAIN_ThreadStatusRegCb(IN ISCSI_MODULE_ID_E enModuleId,
                                  IN IS_THREAD_STATUS_CB_REG_PF pfSysGetThreadStatus)
{
    return ;
}

VOID ISCSI_MAIN_ThreadStatusDeRegCb(IN ISCSI_MODULE_ID_E enModuleId)
{
    return ;
}

VOID TR_MAIN_NicknameWriteDbm(IN ISCSI_MODULE_ID_E enModuleId)
{
    return ;
}

VOID ISCSI_RESET_Trigger(IN ISCSI_RESET_TRIGGER_TYPE_E enNotifyType,
                      IN ISCSI_MODULE_ID_E enModuleId)
{
    return ;
}

VOID ISCSI_RESET_NotifyBack(IN ISCSI_RESET_NOTIFY_TYPE_E enNotifyType,
                         IN ISCSI_MODULE_ID_E enModuleId)
{
    return ;
}

BOOL_T ISCSI_RESET_SysIscsiDisable(VOID)
{
    return 0;
}

VOID ISCSI_MAIN_IfDelNotifyBack(IN UCHAR ucCircIndex,
                             IN ISCSI_MODULE_ID_E enModuleId)
{
    return ;
}

VOID ISCSI_IF_SysEnable(VOID)
{
    return ;
}

VOID ISCSI_IF_SysDisable(VOID)
{
    return ;
}

ISCSI_SYS_ARRAY_NODE_S *ISCSI_PROTO_GetSystemNode(VOID)
{
    return 0;
}

VOID ISCSI_INFO_TimerDbgOutput_WithLspId(IN UCHAR ucDbgFlag,
                                      IN const CHAR *pcFmt,
                                      IN const ISCSI_LSP_ID_S* pstLspId)
{
    return ;
}

VOID ISCSI_INFO_TimerDbgOutput_WithIfname(IN UCHAR ucDbgFlag,
                                       IN const CHAR *pcFmt,
                                       IN UCHAR ucCircIndex)
{
    return ;
}

VOID ISCSI_INFO_ErrorDbgOutput_WithID(IN UCHAR ucDbgFlag,
                                   IN const CHAR *pcFmt,
                                   IN const UCHAR* pucSystem,
                                   IN const UCHAR* pucSourceId)
{
    return ;
}

VOID ISCSI_INFO_ErrorDbgOutput_WithIfnameVlanID(IN UCHAR ucDbgFlag,
                                             IN const CHAR *pcFmt,
                                             IN UCHAR ucCircIndex,
                                             IN ULONG ulVlanId)
{
    return ;
}

VOID ISCSI_INFO_ErrorDbgOutput_WithIfname(IN UCHAR ucDbgFlag,
                                       IN const CHAR *pcFmt,
                                       IN UCHAR ucCircIndex)
{
    return ;
}

VOID ISCSI_INFO_SelfLspDbgOutput_WithLspId(IN UCHAR ucDbgFlag,
                                        IN const CHAR *pcFmt,
                                        IN const ISCSI_LSP_ID_S* pstLspId,
                                        IN UINT uiSeqNo)
{
    return ;
}

VOID ISCSI_INFO_SelfLspDbgOutput_WithID(IN UCHAR ucDbgFlag,
                                     IN const CHAR *pcFmt,
                                     IN const UCHAR* pucSystem,
                                     IN const UCHAR* pucSourceId)
{
    return ;
}

VOID ISCSI_INFO_AdjPktDbgOutput_WithIfnameVlanID(IN UCHAR ucDbgFlag,
                                              IN const CHAR *pcFmt,
                                              IN UCHAR ucCircIndex,
                                              IN ULONG ulVlanId)
{
    return ;
}

VOID ISCSI_INFO_SnpPktDbgOutput_WithLspId(IN UCHAR ucDbgFlag,
                                       IN UCHAR ucCircIndex,
                                       IN const CHAR *pcFmt,
                                       IN const ISCSI_LSP_ID_S* pstLspId)
{
    return ;
}

VOID ISCSI_INFO_SnpPktDbgOutput_WithIfname(IN UCHAR ucDbgFlag,
                                        IN const CHAR *pcFmt,
                                        IN UCHAR ucCircIndex)
{
    return ;
}

VOID ISCSI_INFO_UpdtPktDbgOutput_WithLspId(IN UCHAR ucDbgFlag,
                                        IN UCHAR ucCircIndex,
                                        IN const CHAR *pcFmt,
                                        IN const ISCSI_LSP_ID_S* pstLspId)
{
    return ;
}

VOID ISCSI_INFO_UpdtPktDbgOutput_WithIfname(IN UCHAR ucDbgFlag,
                                         IN const CHAR *pcFmt,
                                         IN UCHAR ucCircIndex)
{
    return ;
}

VOID ISCSI_INFO_TimerDbgOutput(IN UCHAR ucDbgFlag, IN const CHAR *pcFmt,...)
{
    return ;
}

VOID ISCSI_INFO_EventDbgOutput(IN UCHAR ucDbgFlag, IN const CHAR *pcFmt,...)
{
    return ;
}

VOID ISCSI_INFO_SelfLspDbgOutput(IN UCHAR ucDbgFlag, IN const CHAR *pcFmt,...)
{
    return ;
}

VOID ISCSI_INFO_ErrorDbgOutput(IN UCHAR ucDbgFlag, IN const CHAR *pcFmt,...)
{
    return ;
}

VOID ISCSI_INFO_AdjPktDbgOutput(IN UCHAR ucCircIndex, IN UCHAR ucDbgFlag, IN const CHAR *pcFmt,...)
{
    return ;
}

VOID ISCSI_INFO_SnpPktDbgOutput(IN UCHAR ucCircIndex, IN UCHAR ucDbgFlag, IN const CHAR *pcFmt,...)
{
    return ;
}

VOID ISCSI_INFO_LldpPktDbgOutput(IN UCHAR ucCircIndex, IN UCHAR ucDbgFlag, IN const CHAR *pcFmt,...)
{
    return ;
}

VOID ISCSI_INFO_UpdtPktDbgOutput(IN UCHAR ucCircIndex, IN UCHAR ucDbgFlag, IN const CHAR *pcFmt,...)
{
    return ;
}

VOID ISCSI_INFO_MiscErrDbgOutput(IN const CHAR *pcFmt,...)
{
    return ;
}

VOID ISCSI_INFO_HAEventDbgOutput(IN const CHAR *pcFmt,...)
{
    return ;
}

VOID ISCSI_INFO_AdjPduDbgOutput (IN UCHAR ucDbgFlag,
                              IN UCHAR ucCircIndex,
                              IN const UCHAR *pucPduBuf,
                              IN USHORT usPduLen)
{
    return ;
}

VOID ISCSI_INFO_UpdtPduDbgOutput (IN UCHAR ucDbgFlag,
                               IN UCHAR ucCircIndex,
                               IN const UCHAR *pucPduBuf,
                               IN USHORT usPduLen)
{
    return ;
}

VOID ISCSI_INFO_SnpPduDbgOutput (IN UCHAR ucDbgFlag,
                              IN UCHAR ucCircIndex,
                              IN const UCHAR *pucPduBuf,
                              IN USHORT usPduLen)
{
    return ;
}

ULONG ISCSI_CIRC_SetNetType(IN UCHAR ucCircIndex,IN UINT uiNetType)
{
    return 0;
}

VOID ISCSI_CIRC_IncrCircStats(IN UCHAR ucCircIndex,IN ISCSI_CIRCUIT_COUNTER_TYPE_E enType)
{
    return ;
}

VOID ISCSI_CIRC_DecrCircStats(IN UCHAR ucCircIndex,IN ISCSI_CIRCUIT_COUNTER_TYPE_E enType)
{
    return ;
}

ULONG ISCSI_CIRC_GetCircStats(IN UCHAR ucCircIndex,IN ISCSI_CIRCUIT_COUNTER_TYPE_E enType, OUT UINT *puiCnt)
{
    return 0;
}

VOID ISCSI_SYS_IncrSysStats(IN ISCSI_SYSTEM_COUNTER_TYPE_E enType)
{
    return ;
}

VOID ISCSI_SYS_ResetSysStats(VOID)
{
    return ;
}

VOID ISCSI_CIRC_ResetCircStats(VOID)
{
    return ;
}

VOID ISCSI_CIRC_ResetDesiredCircStats(IN UCHAR ucCircIndex)
{
    return ;
}

BOOL_T ISCSI_SYS_IsNetConfiged(VOID)
{
    return 0;
}

BOOL_T ISCSI_HA_IsMaster(VOID)
{
    return 0;
}

BOOL_T ISCSI_MAIN_IsMemAlert(VOID)
{
    return 0;
}

BOOL_T ISCSI_RESET_IsSysDeleting(VOID)
{
    return 0;
}

ULONG ISCSI_SYS_ProcResetDataGet(OUT ISCSI_SYS_RESET_DATA_S *pstResetData)
{
    return 0;
}

ULONG ISCSI_SYS_CliNotifyDataGet(OUT ISCSI_SYS_CLI_NOTIFY_DATA_S *pstCliNotifyData)
{
    return 0;
}

ULONG ISCSI_LLDP_SendMsg(IN IF_INDEX ifIndexOut,
                         IN UINT uiHPSubType,
                         IN const CHAR *pcMsg,
                         IN UINT uiMsgLen)
{
    return 0;
}

VOID ISCSI_SYS_GetEnableVlanAndSetPort(UCHAR ucCircIndex)
{
    return ;
}

VOID ISCSI_SYS_CheckVlanAndSetPort(IN IF_INDEX ifIndex)
{
    return ;
}

BOOL_T ISCSI_MAIN_BinRecoverGet(VOID)
{
    return 0;
}

VOID ISCSI_MAIN_BinRecoverSet(BOOL_T bIsBinRecover)
{
    return ;
}




