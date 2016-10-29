/*****************************************************************************
 Copyright (c) 2010, Hangzhou H3C Technologies Co., Ltd. All rights reserved.
------------------------------------------------------------------------------
                               iSNSNdbLdap.c

  Project Code: Comware V7
   Module Name: ISNS
  Date Created: 2016-10-29
        Author: liangjinchao@dian
   Description: ��LDAP���ݽ��в������������ݵĲ��ҡ���ӡ�ɾ������������

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


/*********************************************************************
     Func Name : ISNS_LDAP_Write
  Date Created : 2016/10/29
        Author : liangjinchao@dian
   Description : �������ݵ�LDAP������Ѵ����򸲸�
         Input : IN const ISNS_DBKey *pstDbKey, IN const SOIP_DB_Entry *pstEntry
        Output : ��
        Return : �ɹ�/ʧ��
       Caution : ��
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
   Description : ɾ��LDAP�е�����
         Input : IN const ISNS_DBKey *pstDbKey
        Output : ��
        Return : �ɹ�/ʧ��
       Caution : ��
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
   Description : ��ʼ���ڴ����ݿ�
         Input : ��
        Output : ��
        Return : �ɹ�/ʧ��
       Caution : ���parent���������½�(����˳������)
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
   Description : ��ʼ���ڴ����ݿ�
         Input : ��
        Output : ��
        Return : �ɹ�/ʧ��
       Caution : ��
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
   Description : ��ʼ���ڴ����ݿ�
         Input : ��
        Output : ��
        Return : �ɹ�/ʧ��
       Caution : ��
----------------------------------------------------------------------
 Modification History
    DATE        NAME             DESCRIPTION
----------------------------------------------------------------------

*********************************************************************/
INT ISNS_LDAP_List_Free(IN UINT uiListType, IN const VOID *pParent)
{

    return SUCCESS;
}



