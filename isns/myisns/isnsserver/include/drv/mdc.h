/*******************************************************************************
 Copyright (c) 2011, Hangzhou H3C Technologies Co., Ltd. All rights reserved.
--------------------------------------------------------------------------------
                                drv/mdc.h
  Project Code: Comware V7
   Module Name: MDC_DRV
  Date Created: 2013-01-04
        Author:
   Description: MDC驱动接口头文件

--------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
--------------------------------------------------------------------------------
  YYYY-MM-DD

*******************************************************************************/
#ifndef _DRV_MDC_H_
#define _DRV_MDC_H_

#ifdef  __cplusplus
extern "C"{
#endif

/* MDC定制接口 */
typedef struct tagMDC_CONF
{
    UINT uiMaxMDCNum;
}MDC_CONF_S;

/* VFW定制接口 */
typedef struct tagMDC_VFW_CONF
{
    UINT uiMaxVfwNum;
}MDC_VFW_CONF_S;

extern VOID MDC_Init_Conf(IN const MDC_CONF_S *pstConfData);
extern VOID MDC_VFW_Init_Conf(IN const MDC_VFW_CONF_S *pstConfData);

#ifdef __cplusplus
}
#endif

#endif  /* end of _DRV_MDC_H_ */

