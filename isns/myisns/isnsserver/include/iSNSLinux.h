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

#ifndef _linuxsns_h
#define _linuxsns_h

#include <stdio.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <stdarg.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <pthread.h>
#include <thread_db.h>

#define HANDLE pthread_t*
#define ULONG ulong
#define FALSE 0
#define TRUE  1

#define LPVOID void *
#define itoa(a,b,c) (((c)==10)?sprintf(b,"%d",a):sprintf(b,"%x",a))

typedef unsigned char BYTE;

#define SNS_NT_FSM_TIMEOUT    10
#define SNS_NT_RS_TIMEOUT     10

#define WDOG_ID               UINT_PTR
#define OK                    SUCCESS
#define FUNCPTR               int

//#define bzero(a,b)            {memset(a, 0, b);}
#define inet_ntoa_b(a,b)      {strcpy(b,inet_ntoa(a));}

typedef int STATUS;
typedef HANDLE MSG_Q_ID;
typedef int SOCKET;
typedef unsigned int DWORD;

void taskDelay (int time);
int sysClkRateGet(void);
int taskIdSelf(void);
void taskSuspend(void);

void taskDelete(pthread_t task);

int
SNSReqTimeoutThread();

int
SNSFSMTimeoutThread();

int
SNSESITimeoutThread();

int
SNSResyncTimeoutThread();

void
SNSTrap(void);

int LinuxTaskSpawn(char* taskName, void* funcPtr);

#define taskSpawn(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o) LinuxTaskSpawn(a,e) 

#endif




