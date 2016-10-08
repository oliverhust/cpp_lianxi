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
#ifndef _isns_h
#define _isns_h

#ifdef SNS_LINUX
#include <stdio.h>
#include <sched.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

#define WINAPI 
#define HANDLE pthread_t*
#define LPVOID void*
typedef unsigned int DWORD;

#define FALSE 0
#define TRUE 1
#define SOCKET int

#define itoa(a,b,c) (((c)==10)?sprintf(b,"%d",a):sprintf(b,"%x",a))

#else
// #include <windows.h>
#include <io.h>
#include <winsock2.h>
#include <Ws2tcpip.h>
#include <stdio.h>

#define uint16_t unsigned short
#define uint32_t unsigned long
#endif

#include <stdlib.h>
#include <time.h>
#include "iSNSdefs.h"

#ifndef MIN
#define MIN(a,b)   ((a) <= (b) ? (a) : (b))
#endif

#ifndef MAX
#define MAX(a,b)   ((a) >= (b) ? (a) : (b))
#endif

#ifndef PAD4
#define PAD4(a)   ((a%4)?(4-a%4)+a:(a+4))
#endif

#define DEFAULT_PORTAL_PORT   (8000)
#define DEFAULT_IP_ADDR       "127.0.0.1"

#define MAX_PAYLOAD_SIZE      (8*1024)
#define MAX_VALUE_LENGTH      (8)

#define ISNS_TAG_LEN          (4)
#define ISNS_LEN_LEN          (4)

#define ISNS_INDEX_SIZE       (4)
#define NISHAN_OUI (0x010F)

typedef struct sns_hb_payload {
   char ip_ptr[16];  
   uint16_t udp_port;
   uint16_t tcp_port;

   uint32_t  interval;        /* heartbeat interval */
   uint32_t  counter;         /* incremental hb counter */

} SNS_Hb;

typedef struct isns_hdr {
   uint16_t version;
   uint16_t func_id;
   uint16_t len;
   uint16_t flags;
   uint16_t xid;
   uint16_t seq;
} ISNS_HDR, *ISNS_HDR_P;

typedef struct _soip_time {
   uint32_t t_pad;
   uint32_t t_time;
} SOIP_Time;

typedef struct _soip_ver {
   uint16_t    max;
   uint16_t    min;
} SOIP_Ver;

typedef struct _soip_grp {
   char  pad[3];
   char  val;
} SOIP_GRP;

typedef struct ISNS_attr {

    uint32_t    tag;
    uint32_t    len;
    union {
       char     c;
       uint32_t etype;
       uint32_t integer;
       SOIP_Time timestamp;
       SOIP_Ver ver;
       SOIP_GRP p_grp;
    } val;
} ISNS_Attr;

typedef struct isns_tlv {
   uint32_t attrib_id;
   uint32_t attrib_len;
   char attrib_val[MAX_VALUE_LENGTH];
} ISNS_TLV, *ISNS_TLV_P;

typedef struct cmd {
    ISNS_HDR hdr;
   char buffer[MAX_PAYLOAD_SIZE];
} ISNS_CMD;

typedef struct sns_esi_rsp_payload {
   char entity_id[256];
   char portal_ip[16];
   int  portal_port;
} SNS_ESI_RSP_PAYLOAD;

typedef struct sns_esi_payload {
   char entity_id[256];
   char portal_ip[16];
   int  portal_port;
} SNS_ESI_PAYLOAD;

typedef struct _listener_cb {
   int port;
   int ip;
   char eid[256];
} LISTENER_CB, *LISTENER_CB_P;

typedef struct _msg_cb {
   int size;
   void *p_msg;
} MSG_CB;

#define MSG_Q_SIZE      (100)


#ifdef SNS_LINUX
int
#else
DWORD WINAPI
#endif
ESIListener(DWORD lparam);

#ifndef BUFFER_SIZE
#define BUFFER_SIZE     (16*1024*10)
#endif

#define PAYLOAD_OFFSET                  (8)

#define CLIENT_CFG_FILE             "client.cfg\0"
#define SERVER_CFG_FILE             "server.cfg\0"

#define TOKENS                      "\n\r"

#define MAX_SEND                        (1024*1024)

#define ISNS_SERVER_PORT        (3205)
#define ISNS_HEARTBEAT_PORT       (3206)

/* This should be a string */
#define ISNS_VERSION "1.2.0"

#define CONTROL_NODE "iqn.control.node"

/* This should be an integer, and is the protocol version */
#define ISNSP_VERSION            1

void ISNSInsertQryHdr(ISNS_FUNC_ID func_id, ISNS_CMD *cmd);

void GetDDS(void);

void GetDD(void);

void RegDDS(void);

void UpdateDDS(void);

void DeRegDDS(void);

void Remove_DD_from_DDS(void);

void RegDD(void);

void UpdateDD(void);

void DeRegDD(void);

void Remove_Member_from_DD(void);

void Add_DD_to_DDS(void);

void DisableESI(void);

void EnableESI(void);

void Send_SCN_Event(void);

int Send_ESI_Response(ISNS_CMD *p_cmd, int size);

void Process_SCN(ISNS_CMD *p_msg, int size);

int RegSCN(void);

void DeRegSCN(void);

void InitESITable(void);

void AddESIPortal(char *p_name, int port, HANDLE hdle);

void KillESIThreads(void);

void GetNext(void);

void SaveDb(void);

#endif


