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
 * This file contains definitions of macros and data
 * structures used for implementing UDP socket based
 * communication between SOIP service entities.
 *
 */

#ifndef _SNSCOMM_h_
#define _SNSCOMM_h_

int ifAddrGet(char *, char *);
int ifBroadcastGet(char *, char *);

char  snsp_ip[16];
char  snsp_subnet[16];
#include "iSNSmsg.h"


/*
 * Macros used for configuring the client/server socket
 * communication
 */
#define SNS_SCGRP_IPMC_ADDR  "255.255.255.255"
#define SNS_BROADCAST_ADDR  "255.255.255.255"

#define SNS_UDP_PORT         3205
#define SNS_MC_UDP_PORT      3206

#define SNS_LAYER_2          1
#define SNS_LAYER_3          2

#ifndef IN_CLASSD
#define IN_CLASSD(i) (((long)(i) & 0xf0000000) == 0xe0000000)
#endif

extern char  snsp_ip[16];
extern char  snsp_bip[16];
extern int   snsp_port;

extern uint32_t
SNSGetServerAddr(void);

extern int
SNSCommInit (ISNS_Entity);

extern int
SNSGetMsg (ISNS_Msg_Descp *);

extern int
ISNSSendMsg (ISNS_Msg_Descp *);

int SNSSendMsg_UDP(ISNS_Msg_Descp* p_md);

int SNSSendMsg_TCP(ISNS_Msg_Descp* p_md);

extern void
ISNSCommUpdate (struct in_addr *);

extern struct in_addr *
SNSGetLocalAddr (void);

extern struct in_addr *
SNSGetMCgrpAddr (void);

extern int
ISNSSendMsg2Server (ISNS_Msg *);

extern int
SNSSendMsg2MCgrp (ISNS_Msg *);

extern int
SNSGetIfAddr(void);

extern int
SNSGetIfBroadcastAddr(void);

extern int
SNSGetMgmtAddr(void);

extern int
SNSGetIPAddress(void);

#endif

