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
 * This file contains source code for managing the
 * iSNS database.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/basetype.h>
#include <sys/error.h>
#include <sys/list.h>
#include "iSNStypes.h"
#include "iSNS.h"
#include "iSNSNdb.h"
#include "iSNSdb.h"
#include "iSNSmsg.h"
#include "iSNSbuffer.h"
#include "iSNStbl.h"
#include "iSNSList.h"
#include "iSNSdebug.h"
#include "iSNSdbMem.h"
#include "iSNSdbLdap.h"

//#define ndb_store_sns(a,b,c,d) ndb_store(dbfp,b,c,d)
/*
#define ndb_open(a,b,c,d) gdbm_open(a,b,c,d)
#define ndb_close(a) gdbm_close(a)
#define ndb_fetch(a,b) gdbm_fetch(dbfp[a],b)
#define ndb_fetch_sns(a,b,c) gdbm_fetch(dbfp[a],b); memcpy(c,d.dptr,d.dsize)
#define ndb_store_sns(a,b,c,d) gdbm_store(dbfp[a],b,c,d)
#define ndb_delete(a,b) gdbm_delete(dbfp[a],b)
#define ndb_nextkey(a,b) gdbm_nextkey (dbfp[a],b)
#define ndb_firstkey(a) gdbm_firstkey (dbfp[a])
*/

/* Global */
extern int isns_db_debug;

/*
 * SoIP service Database file descriptor
 */
//static NDB_FILE dbfp;
//static GDBM_FILE  dbfp[25];

/*
 * GDBM errno
 */
//extern ndb_error ndb_errno;
extern int ndbm_errno;

/*
 * static common variables for gdbm fetches
 * entry is for common registrations and queries
 * scn_entry is for state change notifications
 * opn_entry is for access control
 */

uint32_t *dlist_src; /*[MAX_DD_PER_LIST];*/
uint32_t *dlist_node; /*[MAX_DD_PER_LIST];*/
SOIP_Dd_Member *ddmem; /* [100]; */

ISNS_LIST scn_callback_list;

ISNS_Msg *p_scn_msg_buffer;
ISNS_Msg *p_scn_all_msg_buffer;
ISNS_Msg_Descp *p_scn_md;
ISNS_Msg_Descp *p_rspMd;

dbStats iSNS_stats;

/*********************************************************************
Read Entry from Database referenced by key
*********************************************************************/
int
ISNSdbRead (ISNS_DBKey *key, SOIP_DB_Entry *entry)
{
    int iRet;

    iRet = ISNS_MEM_Read(key, entry);

    return iRet;
}

/*********************************************************************
Write Entry in Database referenced by key
*********************************************************************/
int
ISNSdbWrite (ISNS_DBKey *key, SOIP_DB_Entry entry)
{
    int iRet;

    /* 先存入内存 */
    iRet = ISNS_MEM_Write(key, &entry);
    if(SUCCESS != iRet)
    {
        return iRet;
    }

    /* 再存入LDAP/DBM */
    iRet = ISNS_LDAP_Write(key, &entry);
    if(SUCCESS != iRet)
    {
        return iRet;
    }

    return SUCCESS;
}

/*********************************************************************
Delete Entry in Database referenced by key
*********************************************************************/
int
ISNSdbDelete (ISNS_DBKey *key)
{
    int iRet = SUCCESS;

    /* 先从LDAP删除 */
    iRet |= ISNS_LDAP_Delete(key);

    /* 再从内存删除 */
    iRet |= ISNS_MEM_Delete(key);

    return iRet;
}

/*********************************************************************
Returns a new entity Index.
*********************************************************************/
uint32_t
ISNSGetNewEntityIdx( void )
{
   static uint32_t indx = 0;
   ISNS_DBKey key;
   int index = 0;

   key.tag = ENTITY_IDX_KEY;
   key.len = 0;

   if (indx == 0)
   {
     while (SNSdbGetNextOfKey (&key) == SUCCESS)
     {
      index = key.val.idx.index;
      if ( index > indx )
         indx = index;
    }
   }

   indx++;

   return ( indx );
}

/*********************************************************************
Returns a new iSCSI Index.
*********************************************************************/
uint32_t
ISNSGetNewISCSIIdx( void )
{
   static uint32_t indx = 0;
   ISNS_DBKey key;
   SOIP_DB_Entry stEntry;
   VOID *pIter;

   int index_tmp = 0;

   key.tag = ISCSI_IDX_KEY;
   key.len = 0;

   if (indx == 0)  /* 仅当第一次调用本函数时才遍历 */
   {
        while (SNSdbGetNextOfData (&key, &pIter, &stEntry) == SUCCESS)
        {
            index_tmp = stEntry.data.iscsi_idx.uiIndex;
            if ( index_tmp > indx )
            {
                indx = index_tmp;
            }
        }
   }

   indx++;

   return ( indx );
}

/*********************************************************************
Returns a new Portal Index.
*********************************************************************/
uint32_t
ISNSGetNewPortalIdx( void )
{
   static uint32_t indx = 0;
   ISNS_DBKey key;
   int index = 0;

   key.tag = PORTAL_IDX_KEY;
   key.len = 0;

   if (indx == 0)
   {
     while (SNSdbGetNextOfKey (&key) == SUCCESS)
     {
      index = key.val.idx.index;
      if ( index > indx )
         indx = index;
    }
   }

   indx++;

   return ( indx );
}

/*********************************************************************
Database initialization.
*********************************************************************/
int
ISNSdbOpen( void )
{
    ddmem=(SOIP_Dd_Member *)calloc( MAX_MEMBER_PER_DD, sizeof(SOIP_Dd_Member) );
    dlist_src=(uint32_t *)calloc(MAX_DD_PER_LIST, sizeof(uint32_t));
    dlist_node=(uint32_t *)calloc(MAX_DD_PER_LIST, sizeof(uint32_t));
    p_scn_msg_buffer=(ISNS_Msg *)calloc(1, sizeof(ISNS_Msg));
    p_scn_all_msg_buffer=(ISNS_Msg *)calloc(1, sizeof(ISNS_Msg));
    p_scn_md=(ISNS_Msg_Descp *)calloc(1, sizeof(ISNS_Msg_Descp));
    p_rspMd=(ISNS_Msg_Descp *)calloc(1, sizeof(ISNS_Msg_Descp));

    /*
    * Initialize database envirnoment and resources
      先初始化内存，再初始化LDAP/DBM
    */

    if(ERROR_SUCCESS != ISNS_MEM_Init(ISNS_DB_MAX_TYPE_COUNT))
    {
        return ERROR;
    }

    ISNS_LDAP_INIT_S stLdap;
    stLdap.pcLdapUrl = ISNS_LDAP_SERVER_URL;
    stLdap.pcAdminDn = ISNS_LDAP_ADMIN_DN;
    stLdap.pcPassword = ISNS_LDAP_ADMIN_PASS;
    stLdap.pcBase = ISNS_LDAP_ISNS_BASE;
    if(ERROR_SUCCESS != ISNS_LDAP_Init(&stLdap))
    {
        return ERROR;
    }

    /*
    NDB_INITIALIZE_S stNdb;
    stNdb.pcLdapUrl = ISNS_LDAP_SERVER_URL;
    stNdb.pcAdminDn = ISNS_LDAP_ADMIN_DN;
    stNdb.pcPassword = ISNS_LDAP_ADMIN_PASS;
    stNdb.pcBase = ISNS_LDAP_ISNS_BASE;
    stNdb.iDirMaxCount = ISNS_DB_MAX_TYPE_COUNT;
    if(NDB_SUCCESS != ndb_init(&stNdb))
    {
        return ERROR;
    }
    */

    return (SUCCESS);
}

/*********************************************************************
Closes the Database.
*********************************************************************/
void
ISNSdbClose(void)
{
    ISNS_MEM_Fini();
    ISNS_LDAP_Fini();
    //ndb_close();
}


/*********************************************************************
Gets the next object after the passed in key.
暂时保留此旧接口，如果有性能要求，不推荐调此接口来作遍历
*********************************************************************/
int
SNSdbGetNextOfKey (ISNS_DBKey * key)
{
    if(NULL == key)
    {
        return BOOL_FALSE;
    }
    return ISNS_MEM_NextKey(key);
}


/*********************************************************************
新数据遍历接口
ppIter为一个VOID *型指针(其值初始化为0)地址
                 eg: ISNS_DBKey key = { 0 };  key.tag = DD_ID_KEY;
                     VOID *pIter = NULL;
                     SOIP_DB_Entry entry;
                 while(SNSdbGetNextOfData(&key, &pIter, &entry) == SUCCESS)
                 { ... 每一个数据即为key和entry }
                 如果遍历次数为0则pIter还是NULL
                 如果只想遍历KEY不取数据则第三个参数为NULL
*********************************************************************/
int SNSdbGetNextOfData(INOUT ISNS_DBKey *pstDbKey, INOUT VOID **ppIter, OUT SOIP_DB_Entry *pstEntry)
{
    if(NULL == pstDbKey || NULL == ppIter)
    {
        return ISNS_UNKNOWN_ERR;
    }
    return ISNS_MEM_Iter(pstDbKey, ppIter, pstEntry);
}


/**************************************************
**************************************************/
void ISNSInitDBTables(void)
{
   __DEBUG (isns_db_debug &1, ISNSInitDBTables);
   InitList(SCN_CALLBACK_LIST, &scn_callback_list);
}


