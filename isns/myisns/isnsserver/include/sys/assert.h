/*****************************************************************************
 Copyright (c) 2010, Hangzhou H3C Technologies Co., Ltd. All rights reserved.
-------------------------------------------------------------------------------- 
                            assert.h
  Project Code: Leopard
   Module Name:  
  Date Created: 2010-01-01
        Author: 
   Description: Comware Assert Definition
------------------------------------------------------------------------------  
Modification History
  DATE       NAME               DESCRIPTION
------------------------------------------------------------------------------
  
------------------------------------------------------------------------------ 
******************************************************************************/

#ifndef _SYS_ASSERT_H_
#define _SYS_ASSERT_H_

#ifdef  __cplusplus
extern "C" {
#endif

/* ASSERT MID */


#ifdef _DEBUG_VERSION
extern void AssertHandler(const char *pcExpression,
	                      unsigned int uiLine,
	                      const char *pcFunction);
#define DBGASSERT(expression) \
    do \
    {\
        if (unlikely(!(expression))) \
        {\
            AssertHandler(#expression, __LINE__, __FUNCTION__); \
        }\
    }while(0)
#else
#define DBGASSERT(expression)  
#endif

extern struct file *g_pstStackTraceFile;

#ifdef  __cplusplus
}
#endif

#endif  /* _SYS_ASSERT_H_ */

