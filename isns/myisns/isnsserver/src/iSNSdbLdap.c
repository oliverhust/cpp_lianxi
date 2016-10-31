/*****************************************************************************
 Copyright (c) 2010, Hangzhou H3C Technologies Co., Ltd. All rights reserved.
------------------------------------------------------------------------------
                               iSNSNdbLdap.c

  Project Code: Comware V7
   Module Name: ISNS
  Date Created: 2016-10-29
        Author: liangjinchao@dian
   Description: 对LDAP数据进行操作，各种数据的查找、添加、删除、遍历操作

--------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
--------------------------------------------------------------------------------

*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/basetype.h>
#include <sys/error.h>
#include <sys/assert.h>
#include <sys/list.h>

#include "iSNStypes.h"
#include "iSNSList.h"
#include "iSNSmsg.h"
#include "iSNSdebug.h"
#include "iSNSNdb.h"

#ifndef LDAP_DEPRECATED
#define LDAP_DEPRECATED 1
#endif
//#include <openldap/ldap.h>
#include <ldap.h>


/*********************************************************************
     Func Name : ISNS_LDAP_Write
  Date Created : 2016/10/29
        Author : liangjinchao@dian
   Description : 保存数据到LDAP，如果已存在则覆盖
         Input : IN const ISNS_DBKey *pstDbKey, IN const SOIP_DB_Entry *pstEntry
        Output : 无
        Return : 成功/失败
       Caution : 无
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
INT ISNS_LDAP_Write(IN const ISNS_DBKey *pstDbKey, IN const SOIP_DB_Entry *pstEntry)
{

    return SUCCESS;
}

/*********************************************************************
     Func Name : ISNS_LDAP_Delete
  Date Created : 2016/10/29
        Author : liangjinchao@dian
   Description : 删除LDAP中的数据
         Input : IN const ISNS_DBKey *pstDbKey
        Output : 无
        Return : 成功/失败
       Caution : 无
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
INT ISNS_LDAP_Delete(IN const ISNS_DBKey *pstDbKey)
{

    return SUCCESS;
}

/*********************************************************************
     Func Name : ISNS_LDAP_List_AddNode
  Date Created : 2016/10/29
        Author : liangjinchao@dian
   Description : 初始化内存数据库
         Input : 无
        Output : 无
        Return : 成功/失败
       Caution : 如果parent不存在则新建(调用顺序问题)
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
INT ISNS_LDAP_List_AddNode(IN UINT uiListType, IN const VOID *pParent,
                           IN const CHAR *pcData, IN INT iSize)
{

    return SUCCESS;
}

/*********************************************************************
     Func Name : ISNS_LDAP_List_RemoveNode
  Date Created : 2016/10/29
        Author : liangjinchao@dian
   Description : 初始化内存数据库
         Input : 无
        Output : 无
        Return : 成功/失败
       Caution : 无
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
INT ISNS_LDAP_List_RemoveNode(IN UINT uiListType, IN const VOID *pParent,
                              IN const CHAR *pcData, IN INT iSize)
{

    return SUCCESS;
}

/*********************************************************************
     Func Name : ISNS_LDAP_List_Free
  Date Created : 2016/10/29
        Author : liangjinchao@dian
   Description : 初始化内存数据库
         Input : 无
        Output : 无
        Return : 成功/失败
       Caution : 无
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
INT ISNS_LDAP_List_Free(IN UINT uiListType, IN const VOID *pParent)
{

    return SUCCESS;
}



