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
#include "iSNS.h"
#include "iSNSNdb.h"
#include "iSNSdb.h"
//#include <gdbm.h>
#include "iSNStypes.h"
#include "iSNSmsg.h"
#include "iSNSbuffer.h"
#include "iSNStbl.h"
#include "iSNSList.h"
#include "iSNSdebug.h"
#include "iSNSdbMem.h"

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

    iRet = ISNS_MEM_Write(key, &entry);
    if(ISNS_NO_ERR != iRet)
    {
        return iRet;
    }

    /* ֮������Ӵ���LDAP/DBM, eg:
    iRet = ISNS_LDAP_Write(key, &entry);
    if(ISNS_NO_ERR != iRet)
    {
        return iRet;
    }
    */

    return ISNS_NO_ERR;
}

/*********************************************************************
Delete Entry in Database referenced by key
*********************************************************************/
int
ISNSdbDelete (ISNS_DBKey *key)
{
    int iRet = ISNS_NO_ERR;

    iRet |= ISNS_MEM_Delete(key);

    /* ֮�������ɾ��LDAP/DBM */

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
   int index = 0;

   key.tag = ISCSI_IDX_KEY;
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
      �ȳ�ʼ���ڴ棬�ٳ�ʼ��LDAP/DBM
    */

    if(ERROR_SUCCESS != ISNS_MEM_Init(ISNS_DB_MAX_TYPE_COUNT))
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
    //ndb_close();
}


/*********************************************************************
Gets the next object after the passed in key.
��ʱ�����˾ɽӿڣ����������Ҫ�󣬲��Ƽ����˽ӿ���������
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
�����ݱ����ӿ�
ppIterΪһ��VOID *��ָ��(��ֵ��ʼ��Ϊ0)��ַ
                 eg: ISNS_DBKey key = { 0 };  key.tag = xxx;
                     VOID *pIter = NULL;
                     SOIP_DB_Entry entry;
                 while(SNSdbGetNextOfData(&key, &pIter, &entry) == SUCCESS)
                 { ... ÿһ�����ݼ�Ϊkey��entry }
                 �����������Ϊ0��pIter����NULL
                 ���ֻ�����KEY��ȡ���������������ΪNULL
*********************************************************************/
int SNSdbGetNextOfData(ISNS_DBKey *pstDbKey, VOID **ppIter, SOIP_DB_Entry *pstEntry)
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


