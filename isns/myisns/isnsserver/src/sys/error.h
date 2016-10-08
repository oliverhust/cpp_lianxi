/*****************************************************************************
 Copyright (c) 2008, Hangzhou H3C Technologies Co., Ltd. All rights reserved.
------------------------------------------------------------------------------
                            error.h
  Project Code: Comware V7
   Module Name:
  Date Created: 2008-4-7 
        Author: c02254
   Description: 通用错误码

------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
                                                                             
*****************************************************************************/

#ifndef _SYS_ERROR_H_
#define _SYS_ERROR_H_

#ifdef  __cplusplus
extern "C"{
#endif

/* 去掉优先级等标记,从错误码转换成 资源ID */
#define ERROR_GET_RCID(errno)           ((errno) & 0x0FFFFFFF);  

#define ERROR_SEVERITY_SUCCESS          0x0
#define ERROR_SEVERITY_INFO             0x1
#define ERROR_SEVERITY_WARNING          0x2
#define ERROR_SEVERITY_ERROR            0x3

#define ERROR_GET_MODULEFACILITY(ulMID) (((ulMID)>>16)&0xfff)

#define ERROR_DEFINE(sev,fac,code)      ((ULONG)(((ULONG)((sev)&3)<<29) | \
                                        ((ULONG)((fac)& 0x0FFF)<<16)    | \
                                        ((ULONG)((code)&0xFFFF))))

#define ERROR_GET_SEVERITY(errno)       (((ULONG)(errno)>>29)&3) 
#define ERROR_GET_FACILITY(errno)       (((ULONG)(errno)>>16)&0x0FFF)
#define ERROR_GET_CODE(errno)           ((ULONG)(errno)&0xFFFF)

#define IS_SUCCED(errno)                ((((ULONG)(errno)>>29)&3) == ERROR_SEVERITY_SUCCESS)
#define IS_INFO(errno)                  ((((ULONG)(errno)>>29)&3) == ERROR_SEVERITY_INFO)
#define IS_WARNING(errno)               ((((ULONG)(errno)>>29)&3) == ERROR_SEVERITY_WARNING)
#define IS_ERROR(errno)                 ((((ULONG)(errno)>>29)&3) == ERROR_SEVERITY_ERROR)

#define ERROR_COMMON_FACILITY                 ERROR_GET_MODULEFACILITY(0x00010000UL)
/* Leopard的错误码 取值从 0x00010000 (Facility为0x001) 开始。
   公共错误码的Facility为0x001,其他模块的错误码的Facility从0x002开始。
*/   
                                        
#define ERROR_SUCCESS                         0x0000UL
#define ERROR_FAILED                          ERROR_DEFINE(ERROR_SEVERITY_WARNING, ERROR_COMMON_FACILITY, 0x1UL)
#define ERROR_INVALID_PARAMETER               ERROR_DEFINE(ERROR_SEVERITY_INFO,    ERROR_COMMON_FACILITY, 0x2UL)
#define ERROR_BAD_STATE                       ERROR_DEFINE(ERROR_SEVERITY_ERROR,   ERROR_COMMON_FACILITY, 0x3UL)
#define ERROR_BAD_MSG                         ERROR_DEFINE(ERROR_SEVERITY_ERROR,   ERROR_COMMON_FACILITY, 0x4UL)
#define ERROR_NOT_FOUND                       ERROR_DEFINE(ERROR_SEVERITY_WARNING, ERROR_COMMON_FACILITY, 0x5UL)
#define ERROR_ALREADY_EXIST                   ERROR_DEFINE(ERROR_SEVERITY_WARNING, ERROR_COMMON_FACILITY, 0x6UL)
#define ERROR_TIMEOUT                         ERROR_DEFINE(ERROR_SEVERITY_WARNING, ERROR_COMMON_FACILITY, 0x7UL)
#define ERROR_NOTSUPPORT                      ERROR_DEFINE(ERROR_SEVERITY_WARNING, ERROR_COMMON_FACILITY, 0x8UL)
#define ERROR_INCOMPLETE                      ERROR_DEFINE(ERROR_SEVERITY_WARNING, ERROR_COMMON_FACILITY, 0x9UL)
#define ERROR_NOT_INITIALIZED                 ERROR_DEFINE(ERROR_SEVERITY_WARNING, ERROR_COMMON_FACILITY, 0xAUL)
#define ERROR_NO_ENOUGH_RESOURCE              ERROR_DEFINE(ERROR_SEVERITY_WARNING, ERROR_COMMON_FACILITY, 0xBUL)
#define ERROR_VALUE_OUT_OF_RANGE              ERROR_DEFINE(ERROR_SEVERITY_INFO,    ERROR_COMMON_FACILITY, 0xCUL)
#define ERROR_INIT_ERROR                      ERROR_DEFINE(ERROR_SEVERITY_ERROR,   ERROR_COMMON_FACILITY, 0xDUL)
#define ERROR_NOT_READY                       ERROR_DEFINE(ERROR_SEVERITY_INFO,    ERROR_COMMON_FACILITY, 0xEUL)
#define ERROR_NO_ENOUGH_HARDWARE_RESOURCE     ERROR_DEFINE(ERROR_SEVERITY_WARNING, ERROR_COMMON_FACILITY, 0xFUL)
#define ERROR_RESOURCE_CONFLICT               ERROR_DEFINE(ERROR_SEVERITY_WARNING, ERROR_COMMON_FACILITY, 0x10UL)
#define ERROR_VLAN_NOTEXIST                   ERROR_DEFINE(ERROR_SEVERITY_ERROR,   ERROR_COMMON_FACILITY, 0x11UL)
#define ERROR_VLAN_NOTSTATIC                  ERROR_DEFINE(ERROR_SEVERITY_ERROR,   ERROR_COMMON_FACILITY, 0x12UL)
#define ERROR_VLAN_ISSUBVLAN                  ERROR_DEFINE(ERROR_SEVERITY_ERROR,   ERROR_COMMON_FACILITY, 0x13UL)
#define ERROR_VLAN_OVERFLOWLIMIT              ERROR_DEFINE(ERROR_SEVERITY_ERROR,   ERROR_COMMON_FACILITY, 0x14UL)
#define ERROR_VLAN_NOINTERFACE                ERROR_DEFINE(ERROR_SEVERITY_ERROR,   ERROR_COMMON_FACILITY, 0x15UL)  /* The vlan-interface does not exist.*/
#define ERROR_VLAN_GETSUPERVLANIDFAILED       ERROR_DEFINE(ERROR_SEVERITY_ERROR,   ERROR_COMMON_FACILITY, 0x16UL)  /* Failed to get super vlan ID */
#define ERROR_VLAN_GETSUPERVLANIFFAILED       ERROR_DEFINE(ERROR_SEVERITY_ERROR,   ERROR_COMMON_FACILITY, 0x17UL)  /* Failed to get super vlan interface */
#define ERROR_INVALID_IP                      ERROR_DEFINE(ERROR_SEVERITY_ERROR,   ERROR_COMMON_FACILITY, 0x18UL)  /* Invalid IP address*/
#define ERROR_INVALID_MAC                     ERROR_DEFINE(ERROR_SEVERITY_ERROR,   ERROR_COMMON_FACILITY, 0x19UL)  /* Invalid MAC address*/
#define ERROR_FEATURE_NOT_ENABLED             ERROR_DEFINE(ERROR_SEVERITY_ERROR,   ERROR_COMMON_FACILITY, 0x1AUL)  /* process hasn't been started */
#define ERROR_VPN_NOTEXIST                    ERROR_DEFINE(ERROR_SEVERITY_ERROR,   ERROR_COMMON_FACILITY, 0x20UL)  /* VPN-Instance does not exist */
#define ERROR_PORT_NOTALLOWVLAN               ERROR_DEFINE(ERROR_SEVERITY_ERROR,   ERROR_COMMON_FACILITY, 0x21UL)  /* Port doesn't belong to VLAN or trunk doesn't allow VLAN to pass */
#define ERROR_ENTRY_REACHLIMIT                ERROR_DEFINE(ERROR_SEVERITY_ERROR,   ERROR_COMMON_FACILITY, 0x22UL)  /* Too many entries */
#define ERROR_END_OF_TABLE                    ERROR_DEFINE(ERROR_SEVERITY_ERROR,   ERROR_COMMON_FACILITY, 0x23UL)  /* Too many entries */
#define ERROR_BATCH_COMPLETE                  ERROR_DEFINE(ERROR_SEVERITY_SUCCESS, ERROR_COMMON_FACILITY, 0x24UL)
#define ERROR_PERMISSION_DENY                 ERROR_DEFINE(ERROR_SEVERITY_ERROR,   ERROR_COMMON_FACILITY, 0x25UL)  /* Permission denied */
#define ERROR_VLAN_ENDVLANLESSTHENSTARTVLAN   ERROR_DEFINE(ERROR_SEVERITY_ERROR,   ERROR_COMMON_FACILITY, 0x26UL)  /* The end VLAN ID must be greater than or equal to the start VLAN ID. */
#define ERROR_GROUP_DENY                      ERROR_DEFINE(ERROR_SEVERITY_ERROR,   ERROR_COMMON_FACILITY, 0x27UL)  /* group denied */
#define ERROR_CTRL_C                          ERROR_DEFINE(ERROR_SEVERITY_WARNING, ERROR_COMMON_FACILITY, 0x28UL)
#define ERROR_EFECT_BYREBOOT                  ERROR_DEFINE(ERROR_SEVERITY_INFO,    ERROR_COMMON_FACILITY, 0x29UL)   /* Reboot device to make the configuration take effect. */
#define ERROR_INVALID_CIPHER_PASSWD           ERROR_DEFINE(ERROR_SEVERITY_WARNING, ERROR_COMMON_FACILITY, 0x2AUL)   /* The ciphertext password is invalid. */
#define ERROR_INVALID_CIPHER_KEY              ERROR_DEFINE(ERROR_SEVERITY_WARNING, ERROR_COMMON_FACILITY, 0x2BUL)   /* The ciphertext key string is invalid. */
#define ERROR_NO_LICENSE                      ERROR_DEFINE(ERROR_SEVERITY_WARNING, ERROR_COMMON_FACILITY, 0x2CUL)   /* The feature %s has no available license. */
#define ERROR_NO_LICENSE_EXIT                 ERROR_DEFINE(ERROR_SEVERITY_ERROR,   ERROR_COMMON_FACILITY, 0x2DUL)   /* The %s feature is being disabled, because it has no license. */
#define ERROR_MAC_EXIST                       ERROR_DEFINE(ERROR_SEVERITY_ERROR,   ERROR_COMMON_FACILITY, 0x2EUL)   /* The MAC Address already exists. */
#define ERROR_FEATURE_NOT_CONFIGURED          ERROR_DEFINE(ERROR_SEVERITY_ERROR,   ERROR_COMMON_FACILITY, 0x2FUL)   /* %s is not configured. */
#define ERROR_PSK_WRONG_COMPOSITION           ERROR_DEFINE(ERROR_SEVERITY_ERROR,   ERROR_COMMON_FACILITY, 0x30UL)
#define ERROR_PSK_WRONG_LENGTH                ERROR_DEFINE(ERROR_SEVERITY_ERROR,   ERROR_COMMON_FACILITY, 0x31UL) 
#define ERROR_FEATURE_NOT_READY               ERROR_DEFINE(ERROR_SEVERITY_ERROR,   ERROR_COMMON_FACILITY, 0x32UL)   /* %s is not ready. */
#define ERROR_RBAC_DENY_SOME_PORTS            ERROR_DEFINE(ERROR_SEVERITY_ERROR,   ERROR_COMMON_FACILITY, 0x33UL)   /* Permission denied on some ports */
#define ERROR_RBAC_DENY_SOME_VLAN             ERROR_DEFINE(ERROR_SEVERITY_ERROR,   ERROR_COMMON_FACILITY, 0x34UL)   /* Permission denied on some VLANs */
#define ERROR_MDC_BUSY                        ERROR_DEFINE(ERROR_SEVERITY_WARNING, ERROR_COMMON_FACILITY, 0x35UL)   /* The MDC is busy. Please try again later. */
#define ERROR_CONTEXT_BUSY                    ERROR_DEFINE(ERROR_SEVERITY_WARNING, ERROR_COMMON_FACILITY, 0x36UL)   /* The Context is busy. Please try again later. */
#define ERROR_CLUSTER_SHUT_DENY               ERROR_DEFINE(ERROR_SEVERITY_WARNING, ERROR_COMMON_FACILITY, 0x37UL)   /* cluster port shutdown deny*/
#define ERROR_GROUP_DENY_IRF                  ERROR_DEFINE(ERROR_SEVERITY_ERROR,   ERROR_COMMON_FACILITY, 0x38UL)   /* Group denied for undo shutdown on IRF ports */
#define ERROR_GROUP_DENY_PEX                  ERROR_DEFINE(ERROR_SEVERITY_ERROR,   ERROR_COMMON_FACILITY, 0x39UL)   /* Group denied for undo shutdown on PEX ports */
#define ERROR_GROUP_DENY_IRFPEX               ERROR_DEFINE(ERROR_SEVERITY_ERROR,   ERROR_COMMON_FACILITY, 0x3aUL)   /* Group denied for undo shutdown on IRF/PEX ports */
#define ERROR_NOTPREPROVISIONED               ERROR_DEFINE(ERROR_SEVERITY_ERROR,   ERROR_COMMON_FACILITY, 0x3bUL)   /* This feature can not be pre-provisioned */

#ifndef __KERNEL__

/*****************************************************************************
  Description: 设置本执行线程的错误码
        Input: IN INT   iErrorNo   指定的错误码
                (可以是标准错误码，也可以是Leopard错误码)
       Output: None
       Return: None
      Caution: 只能在用户态调用
*****************************************************************************/
VOID SetLastError(IN INT iErrorNo);

/*****************************************************************************
  Description: 获取本执行线程的错误码
        Input: None
       Output: None
       Return: INT    当前线程最后设置的错误码。 
      Caution: 只能在用户态调用
*****************************************************************************/
INT  GetLastError (VOID);

/*****************************************************************************
  Description: 打印本线程的错误信息(通过GetLastError获取当前错误码),
               功能类似 perror.区别在于:

                1. 该接口既能打印Comware V7错误码，也能打印标准错误码。 
                2. perror打印是缺省是向控制台stdout输出信息，而本接口是向登陆的
                   用户输出提示信息。 
                   该函数主要供作为命令行处理的插件代码调用。
                3. perror打印是短语，比如:No such file or directory
                   本接口打印的是完整语句.

        Input: IN CHAR * pcMsg,    错误码的上下文信息。以辅助表达。
       Output: None
       Return: None
*****************************************************************************/ 
VOID  PrintErrorMsg(IN const CHAR * pcMsg);

#endif



/*****************************************************************************
  Description: 根据输入的错误码iErrorNo格式化错误信息, 调用者提供缓冲区,
               功能类似strerror_r 接口,
               区别在于:
               1. 该接口既能格式化Comware V7错误码，也能格式化标准错误码。 
        Input: IN INT   iErrorNo,     指定的错误码。
               IN ULONG ulLen,        指定的输出缓冲区的长度。
       Output: OUT CHAR *pcBuf,       指定的输出缓冲区
       Return: 输出缓冲区地址
      Caution: 为了保证和标准的strerror_r 接口兼容，本接口的IN、OUT顺序不符合规范
               格式化生成的字符串，一般不带严重级别，可能是完整的语句，也可能是短语。
               
               可以在用户态和内核态调用,但相关错误码必须在内核注册。
*****************************************************************************/ 
CHAR * FormatErrorMsg(IN INT iErrorNo, OUT CHAR * pcBuf,IN ULONG ulLen);


#ifdef  __cplusplus
}
#endif  /* end of __cplusplus */
#endif /* _SYS_ERROR_H_ */

