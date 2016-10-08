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

#ifndef _ntsns_h
#define _ntsns_h

#define _WIN32_WINNT 0x0400
#include <windows.h>
#include <stdio.h>
#include <io.h>

extern WSADATA g_WsaData;

#define S_IRUSR  0x0100     /* read permission, owner */
#define S_IWUSR  0x0080     /* write permission, owner */
#define S_IRGRP  0x0020     /* read permission, group */

#define SNS_NT_FSM_TIMEOUT    10
#define SNS_NT_RS_TIMEOUT     10

#define WDOG_ID               UINT_PTR
#define OK                    SUCCESS
#define FUNCPTR               int
#define tickGet               GetTickCount
#define bzero(a,b)            {memset(a, 0, b);}
#define inet_ntoa_b(a,b)      {strcpy(b,inet_ntoa(a));}

void* NTtaskSpawn(char* taskName, void* funcPtr);

#define taskSpawn(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o) NTtaskSpawn(a,e)
#define close(a) _close(a)

typedef char uint8_t;
typedef unsigned int uint32_t;
typedef unsigned short int uint16_t;
typedef int STATUS;
typedef HANDLE MSG_Q_ID;


void wdCancel(void *timer);
void taskDelay (int time);
int sysClkRateGet(void);
int wdStart(void *sns_request_timer, int next_timeout,
void *nptr1, void *nptr2);
int taskIdSelf(void);
void taskSuspend(void);

void taskDelete(int task);
DWORD WINAPI
SNSReqTimeoutThread(LPVOID lparam);

DWORD WINAPI
SNSFSMTimeoutThread(LPVOID lparam);

DWORD WINAPI
SNSESITimeoutThread(LPVOID lparam);

DWORD WINAPI
SNSResyncTimeoutThread(LPVOID lparam);

void
SNSTrap(void);

#endif
