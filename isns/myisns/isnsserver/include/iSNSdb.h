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
 * This file contains definitions related to iSNS database
 * management
 *
 */

#ifndef _SNSdb_h_
#define _SNSdb_h_

#include <time.h>
#include "iSNSmsg.h"

#ifndef SNS_LINUX
#define bcmp(a,b,c)     memcmp(a,b,c)
#endif

/*
 * Default initialization value for device attributes.
 * When a database table is created all attributes are
 * initialized to SNS_UNREGISTERED
 */
#define SNS_UNREGISTERED     0xFF


/* 数据库中的LDAP */
#define ISNS_LDAP_SERVER_URL            "ldap://127.0.0.1:38989"
#define ISNS_LDAP_ADMIN_DN              "cn=root,dc=abc,dc=com"
#define ISNS_LDAP_ADMIN_PASS            "123456"
#define ISNS_LDAP_ISNS_BASE             "dc=abc,dc=com"


/*
 * Function prototypes
 */
typedef struct _dbStats {
   int num_dd;
   int num_dds;
   int num_entity;
   int num_iscsi;
   int num_ifcp;
   int num_portals;
   int num_portal_groups;
} dbStats;

extern int
ISNSdbOpen (void);

extern void
ISNSdbClose (void);

int
SNSdbGetNextOfKey (ISNS_DBKey * key);

int
ISNSdbRead (ISNS_DBKey *key, SOIP_DB_Entry *entry);

int
ISNSdbWrite (ISNS_DBKey *key, SOIP_DB_Entry entry);

int
ISNSdbDelete (ISNS_DBKey *key);

uint32_t ISNSGetNewPortalIdx(void);

int ISNS_GetNewDDS_ID (void);
int ISNS_GetNewDD_ID (void);

uint32_t ISNSGetNewEntityIdx(void);
uint32_t ISNSGetNewISCSIIdx(void);
uint32_t ISNSGetNewPortalIdx(void);

void ISNSInitDBTables (void);

#endif
