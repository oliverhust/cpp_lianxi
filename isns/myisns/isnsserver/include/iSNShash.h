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

#ifndef _SNS_HASH_H_
#define _SNS_HASH_H_

#define HASH_KEY_TABLE_NUM_ENTRIES 256

#define HASH_NUM_PORTS_SUPPORTED  256
#define HASH_TABLE_NUM_ENTRIES  HASH_NUM_PORTS_SUPPORTED*4
#define HASH_NUM_BUFFERS  HASH_NUM_PORTS_SUPPORTED*4
#define HASH_BUFFER_SIZE  (sizeof(SNSHashEntry)+sizeof(SOIP_DB_Entry))
#define HASH_KEY_SIZE  256
#define HASH_MAX_DATA_SIZE  sizeof(SOIP_DB_Entry)

typedef struct _sns_hash_entry {

    struct _sns_hash_entry *next;
    struct _sns_hash_entry *prev;

    uint16_t key_len;
    uint16_t data_len;

    uint8_t key[HASH_KEY_SIZE];
    uint8_t data[4];

} SNSHashEntry;

#endif
