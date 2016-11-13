/*******************************************************************************
 Copyright (c) 2007, Hangzhou H3C Technologies Co., Ltd. All rights reserved.
--------------------------------------------------------------------------------
                                drv/dev.h
  Project Code: Comware
   Module Name: DEV_DRV
  Date Created: 2009-08-27
        Author: lifengguang 03035
   Description: DEV driver header file

--------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
--------------------------------------------------------------------------------
  YYYY-MM-DD

*******************************************************************************/
#ifndef _DRV_DEV_H_
#define _DRV_DEV_H_

#ifdef  __cplusplus
extern "C"{
#endif

/* 设备管理无效值,表示不关心的框号/槽号/子槽号等 */
#define DEV_INVALID_ID                      0xffff

#define DEV_FILE_NAME_MAX_LEN               64UL    /* 文件名最大长度 */
#define DEV_BOOTIMAGE_MAX_FILENUM           32      /* 启动文件列表中文件最大数目 */

#define DEV_MAX_DESC_LEN                    64UL    /* 描述信息最大长度 */
#define DEV_MAX_VER_LEN                     64UL    /* 单板各种版本信息的最大长度 */

#define DEV_COLD_BOOT                       0       /* 冷启动 */
#define DEV_WARM_BOOT                       1       /* 热启动 */

#define DEV_LED_TYPE_RUN                    0UL     /* RUN指示灯*/
#define DEV_LED_TYPE_ALARM                  1UL     /* ALARM指示灯*/
#define DEV_LED_TYPE_LOCATOR                2UL     /* LOCATOR指示灯*/

#define DEV_LED_STATUS_ON                   0       /* 点灯 */
#define DEV_LED_STATUS_OFF                  1       /* 灭灯 */

#define DEV_OID_MAXLEN                      256UL   /* OID序列的最大长度 */
#define DEV_DEVICE_INFO_MAX_LEN             4096UL  /* 资源显示信息最大长度 */

#define BOOTROM_BASE                        0U   /* 操作基本段 */
#define BOOTROM_EXT                         1U   /* 操作扩展段 */
#define BOOTROM_ALL                         2U   /* 操作基本段与扩展段 */
#define BOOTROM_EXT_IN_ALL                  3U   /* 从包含基本段与扩展段的文件中，只操作其中的扩展段 */

#define DEV_PID_MAXLEN                     128UL    /* PID最大长度 */
#define DEV_SN_MAXLEN                      128UL    /* SN最大长度 */

/* 每块板(包括机框)上最大支持的传感器数目,包括入风口,出风口,和热点 */
#define DEV_MAX_SENSOR_NUMBER               9

/* 门限的无效值 */
#define DEV_INVALID_LIMIT                   (INT)0xFFFF

/* 获取显示信息的驱动上下文开始和结束标记的宏定义 */
#define DEV_DEVICEINFO_START                0UL
#define DEV_DEVICEINFO_END                  ~0UL

/* DEV_EVT_BMAC_CHANGE事件信息 */
typedef struct tagDevEvtBmacInfo
{
    UCHAR aucOldMac[MAC_ADDR_LEN];   /* 旧的BMAC */
    UCHAR aucNewMac[MAC_ADDR_LEN];   /* 新的BMAC */
}DEV_EVT_BMAC_INFO_S;
/* 用于从驱动获取OID值的数据结构 */
typedef struct tagDEV_OID
{
    UINT  uiLen; /* OID序列的长度,以UINT为单位,最大为DEV_OID_MAXLEN */
    UINT *puiOID;/* 保存OID序列的内存地址,平台分配长度为DEV_OID_MAXLEN个UINT的内存 */
}DEV_OID_S;

/* 设备类型 */
ISSU typedef enum tagDEV_DEVICE_TYPE
{
    DEV_TYPE_SLOT,                  /* 单板或子卡信息 */
    DEV_TYPE_HDISK,                 /* 硬盘信息 */
    DEV_TYPE_CFCARD,                /* CF卡信息 */
    DEV_TYPE_USB,                   /* USB口信息 */
    DEV_TYPE_POWER,                 /* 电源信息 */
    DEV_TYPE_RPS,                   /* RPS信息 */
    DEV_TYPE_FAN,                   /* 风扇信息 */
    DEV_TYPE_SENSOR,                /* 传感器 */
    DEV_TYPE_POWERMONITOR,          /* 电源监控信息 */
    DEV_TYPE_CHASSIS,               /* 框资源信息 */
    DEV_TYPE_CPU,                   /* CPU信息 */
    DEV_TYPE_FLASH,                 /* FLASH信息 */
    DEV_TYPE_VOLTAGESENSOR,         /* 电压信息 */
    DEV_TYPE_SDCARD,                /* SD卡信息 */
    DEV_TYPE_BUTT
}DEV_DEVICE_TYPE_E;

/* 设备管理资源物理状态枚举值定义 */
ISSU typedef enum tagDevStatus
{
    DEV_STATUS_ABSENT,              /*0 不在位 */
    DEV_STATUS_NORMAL,              /*1 正常 */
    DEV_STATUS_FAIL,                /*2 故障态 */
    DEV_STATUS_BUTT
}DEV_STATUS_E;


/* 风扇资源物理状态枚举值定义，其中Absent、Normal、fail状态必须与DEV_STATUS_E保持一致 */
ISSU typedef enum tagDevFanStatus
{
    DEV_FANSTATUS_ABSENT,              /*0 不在位 */
    DEV_FANSTATUS_NORMAL,              /*1 正常 */
    DEV_FANSTATUS_FAIL,                /*2 故障态 */
    DEV_FANSTATUS_DIRECTIONFAULT,      /*3 风扇风向错误 */
    DEV_FANSTATUS_BUTT
}DEV_FANSTATUS_E;

/* 电源资源物理状态枚举值定义，其中Absent、Normal、Fail状态必须与DEV_STATUS_E保持一致 */
typedef enum tagDevPowerStatus
{
    DEV_POWERSTATUS_ABSENT,       /*0 不在位 */
    DEV_POWERSTATUS_NORMAL,       /*1 正常 */
    DEV_POWERSTATUS_FAIL,         /*2 故障态 */
    DEV_POWERSTATUS_INCOMPATIBLE, /*3 电源不兼容错误 */
    DEV_POWERSTATUS_BUTT
}DEV_POWERSTATUS_E;

/* 通用环境状态的取值 */
ISSU typedef enum tagDevEnvState
{
    DEV_ENV_OK,                     /* 正常 */
    DEV_ENV_HIGH,                   /* 偏高 */
    DEV_ENV_LOW,                    /* 偏低 */
    DEV_ENV_BUTT
}DEV_ENV_STATE_E;

/* 温度状态取值 */
ISSU typedef enum tagDevTemState
{
    DEV_TEM_OK,                     /* 温度正常态 */
    DEV_TEM_LOWER,                  /* 温度处于下限态 */
    DEV_TEM_UPPER,                  /* 温度处于上限态 */
    DEV_TEM_UPPER2,                 /* 严重上限态 */
    DEV_TEM_UPPER3,                 /* 关断上限态 */
    DEV_TEM_BUTT
}DEV_TEM_STATE_E;

/* 启动文件的主备属性定义 */
ISSU typedef enum tagDevBootImgType
{
    DEV_BOOTIMAGE_MAIN,                    /* 主启动 */
    DEV_BOOTIMAGE_BACKUP,                  /* 备启动 */
    DEV_BOOTIMAGE_BUTT
}DEV_BOOTIMAGE_TYPE_E;

/* 启动文件命令字 */
ISSU typedef enum tagBootImageCmd
{
    BOOTIMAGE_SET_NEXTMAIN = 1,     /* 设置下次主启动文件 */
    BOOTIMAGE_SET_NEXTBACKUP,       /* 设置下次备用启动文件 */

    BOOTIMAGE_GET_CURRENT_ATTR,     /* 获取本次启动文件属性 */
    BOOTIMAGE_GET_NEXTMAIN,         /* 获取下次主启动文件 */
    BOOTIMAGE_GET_NEXTBACKUP,       /* 获取下次备用启动文件 */
    BOOTIMAGE_GET_LOCATION,         /* 获取启动文件存储位置 */

    BOOTIMAGE_VERIFYBYFILE,         /* 按照文件名验证启动文件合法性 */
    BOOTIMAGE_VERIFYBYCONT,         /* 按照内容验证启动文件合法性 */

    BOOTIMAGE_CMD_BUTT
}BOOTIMAGE_CMD_E;

/* Bootrom操作命令字 */
typedef enum tagBootromCmd
{
    BOOTROM_UPDATE = 1,             /* 升级Bootrom */
    BOOTROM_READ,                   /* 读取到文件系统中 */
    BOOTROM_BACKUP,                 /* 备份到备份区 */
    BOOTROM_RESTORE,                /* 从备份区恢复 */
    BOOTROM_VERIFYBYFILE,           /* 验证文件合法性 */
    BOOTROM_GET_ACCESS,             /* 用于获取是否允许用户访问BootRom */
    BOOTROM_SET_ACCESS,             /* 用于设置是否允许用户访问BootRom */
    BOOTROM_UPDATE_CHK,             /* 用于设置是否对升级bootrom进行检查*/

    BOOTROM_CMD_BUTT
}BOOTROM_CMD_E;


/* 设备基本规格信息获取命令字 */
typedef enum tagDEV_INFO_CMD
{
    DEV_INFO_Version,               /* 此结构的版本号 */
    DEV_INFO_MaxChassisID,          /* 最大框号 */
    DEV_INFO_MinChassisID,          /* 最小框号 */
    DEV_INFO_MinSlotID,             /* 最小槽位号 */
    DEV_INFO_MaxSlotID,             /* 最大槽位号 */
    DEV_INFO_MaxMPUNum,             /* 最多主控板数目 */
    DEV_INFO_MaxSubCardNum,         /* 支持的最多子卡数目 */
    DEV_INFO_MaxPortNum,            /* 支持的最大端口数 */
    DEV_INFO_FirstFanId,            /* 风扇首索引 */
    DEV_INFO_MaxFanNum,             /* 最多风扇数 */
    DEV_INFO_MaxCPUNum,             /* 最多CPU数目 */
    DEV_INFO_FirstPowerId,          /* 电源首索引 */
    DEV_INFO_MaxPowerNum,           /* 最多电源数 */
    DEV_INFO_MaxRPSNum,             /* 最多RPS数目 */
    DEV_INFO_MaxCFCardNum,          /* 最多CF卡槽位数 */
    DEV_INFO_MaxUSBNum,             /* 最大USB数量 */
    DEV_INFO_MaxHDDNum,             /* 最大HDD数量 */

    DEV_INFO_TempSensorInfo,        /* 温度传感器数目类型信息 */
    DEV_INFO_DefVoltageInfo,        /* 缺省电压信息 */
    DEV_INFO_DefCurrentInfo,        /* 缺省电流信息 */

    DEV_INFO_Sup_FanExtShow,        /* 是否支持查看风扇详细信息 */
    DEV_INFO_Sup_PnPNotify,         /* 是否支持热插拔上报 */
    DEV_INFO_Sup_SubCardMaintain,   /* 是否支持子卡维护 */
    DEV_INFO_Sup_BootromExtend,     /* 是否支持Bootrom扩展操作 */
    DEV_INFO_Sup_BootImageSlave,    /* 是否支持BootImage主备属性 */
    DEV_INFO_Sup_SubSlot2Slot,      /* 是否支持二维转三维 */
    DEV_INFO_Sup_SetTempLimit,      /* 是否支持设置温度门限 */
    DEV_INFO_Sup_LEDControl,        /* 是否支持LED点灯控制 */
    DEV_INFO_BOOTROM_TAG,           /* 获得单板对应Bootware的DeviceCode */
    DEV_INFO_OS_TAG,                /* 获得单板对应主机软件的DeviceCode */
    DEV_INFO_MinPwrMonId,           /* 电源监控模块最小实体索引  */
    DEV_INFO_MaxPwrMonId,           /* 电源监控模块最大实体索引  */
    DEV_INFO_MaxChannelNum,         /* 最大通道数（如转发芯片、网板等）*/
    DEV_INFO_OAP,                   /* 支持的OAP板卡信息*/
    DEV_INFO_LINE,                  /* 支持的LINE配置信息 */
    DEV_INFO_VirtualChassisID,      /* 虚拟框ID */
    DEV_INFO_IsExtendedModule,      /* 取本节点是不是扩展模块 */
    DEV_INFO_Voltage1stPhysicalID,  /* 获取某位置的电压首物理ID */
    DEV_INFO_VoltageSensorNum,      /* 获取某位置的电压传感器个数 */
    DEV_INFO_BUTT
}DEV_INFO_CMD_E;

typedef struct tagDEV_PID_INFO
{
    UCHAR aucBuf[DEV_PID_MAXLEN];    /* 设备PID内容 */
    UINT uiLen;       /* 设备PID长度 */
    UINT uiAlignMode; /* 裁减或补齐模式,取值参考DEV_PID_TYPE_E */
}DEV_PID_INFO_S;
/* License PID类型枚举 */
typedef enum tagLicDeviceIDType
{
    DEV_PID_ALIGN_NONE = 0,      /* 正好18字节，不填充，不丢弃 */
    DEV_PID_ALIGN_RIGHT_PAD,     /* 右侧补充数据，补足18字节 */
    DEV_PID_ALIGN_LEFT_DISCARD,  /* 左侧丢弃数据，保留后18字节 */
    DEV_PID_ALIGN_RIGHT_DISCARD,  /* 右侧丢弃数据，保留前18字节 */
    DEV_PID_ALIGN_BUTT
}DEV_PID_TYPE_E;
typedef struct tagDEV_SN_INFO
{
    UCHAR aucBuf[DEV_SN_MAXLEN];    /* 设备SN内容 */
    UINT uiLen;       /* 设备SN长度 */
}DEV_SN_INFO_S;

/* 重启原因枚举值 */
ISSU typedef enum tagDevBootCause{
    BOOT_CAUSE_UI,              /* 0 通过命令行,SNMP,Web等管理手段重启设备 */
    BOOT_CAUSE_EXCEP,           /* 1 内核异常重启 */
    BOOT_CAUSE_DEADLOOP,        /* 2 内核死循环重启 */
    BOOT_CAUSE_DEVHANDSHAKE,    /* 3 设备管理握手失败重启 */
    BOOT_CAUSE_LCSOVERTIME,     /* 4 LICENSE超时 */
    BOOT_CAUSE_SLAVESWITCH,     /* 5 主备倒换重启原主板 */
    BOOT_CAUSE_SLAVECANNOTUPDATE,/* 6 备板不能升级为主板 */
    BOOT_CAUSE_IRFMERGE,        /* 7 堆叠Merge重启本板 */
    BOOT_CAUSE_AUTOUPDATE,      /* 8 版本自动升级 */
    BOOT_CAUSE_MASTERLOST,      /* 9 在本板批备时，主板重启 */
    BOOT_CAUSE_UNKNOWN,         /* 10 未知原因 */
    BOOT_CAUSE_OOM,             /* 11 单板内存耗尽重启 */
    BOOT_CAUSE_CRYPTOTESTFAIL,   /* 12 算法库自检失败 */
    BOOT_CAUSE_BUTT
}DEV_BOOTCAUSE_E;

/* 传感器类型 */
ISSU typedef enum tagDevSensorType{
    DEV_SENSOR_INFLOW = 0,     /* 入风 */
    DEV_SENSOR_OUTFLOW,        /* 出风 */
    DEV_SENSOR_HOTSPOT,        /* 热点 */
    DEV_SENSOR_INVALID         /* 非法值 */
}DEV_SENSOR_TYPE_E;

/* 门限值结构,可以用于温度、电压和电流的告警门限值，分为4级门限 */
ISSU typedef struct  tagDEV_THRESHOLD
{
      INT iLowerLimit;                  /* 告警下限 */
      INT iUpperLimit;                  /* 一般级(Warning)告警门限 */
      INT iUpperLimit2;                 /* 严重级(Alarm)告警门限 */
      INT iUpperLimit3;                 /* 关断级别告警门限 */
}DEV_THRESHOLD_S;

/* 传感器门限值结构定义,用于产品定制各个传感器的门限默认值和范围 */
ISSU typedef struct tagDEV_THRESHOLD_REG_INFO
{
    INT iDefLowerLimit;             /* 默认告警下限 */
    INT iMinLowerLimit;             /* 最低告警下限 */
    INT iMaxLowerLimit;             /* 最高告警下限 */
    INT iDefUpperLimit;             /* 默认一般级(Warning)告警门限 */
    INT iMinUpperLimit;             /* 最低一般级(Warning)告警门限 */
    INT iMaxUpperLimit;             /* 最高一般级(Warning)告警门限 */
    INT iDefUpperLimit2;            /* 默认严重级(Alarm)告警门限 */
    INT iMinUpperLimit2;            /* 最低严重级(Alarm)告警门限 */
    INT iMaxUpperLimit2;            /* 最高严重级(Alarm)告警门限 */
    INT iDefUpperLimit3;            /* 默认关断级别告警门限 */
    INT iMinUpperLimit3;            /* 最低关断级别告警门限 */
    INT iMaxUpperLimit3;            /* 最高关断级别告警门限 */
}DEV_THRESHOLD_REG_INFO_S;

/* 环境门限结构，包括上下两个门限和其最大最小值范围 */
ISSU typedef struct tagDEV_ENV_LIMIT
{
    INT iDefLowerLimit;           /* 默认告警下限 */
    INT iMinLowerLimit;           /* 最低告警下限 */
    INT iMaxLowerLimit;           /* 最高告警下限 */
    INT iDefUpperLimit;           /* 默认告警上限 */
    INT iMinUpperLimit;           /* 最低告警上限 */
    INT iMaxUpperLimit;           /* 最高告警上限 */
}DEV_ENV_LIMIT_S;


/* 温度传感器信息结构 */
ISSU typedef struct tagDEV_TEMP_SENSOR_INFO
{
    UINT uiInflowNum;               /* 入风口传感器数目，0表示不支持该种类型 */
    UINT uiInflowStartID;           /* 入风口传感器起始索引 */
    UINT uiHotspotNum;              /* 热点传感器数目 */
    UINT uiHotspotStartID;          /* 热点传感器起始索引 */
    UINT uiOutflowNum;              /* 出风口传感器数目 */
    UINT uiOutflowStartID;          /* 出风口传感器起始索引 */

    /* 各个传感器的默认门限信息,按照单板（或机框上）内偏移顺序存放,同一类型之间按照位置排列,
       由产品按照规范负责对应，对于不支持的门限请输入无效值 */
    DEV_THRESHOLD_REG_INFO_S astThresholdRegInfo[DEV_MAX_SENSOR_NUMBER];
}DEV_TEMP_SENSOR_INFO_S;

/* 电子标签信息获取的操作类型 */
#define DEV_GET_MANUINFO_PART           0UL
#define DEV_GET_MANUINFO_ALL            1UL


/* 单板类型结构 */
typedef struct tagDevBoardType
{
   UINT  uiMibBoardType;                    /* MIB板类型,该板类型在MIB中对应的编号*/
   UINT  uiBoardType;                       /* 该单板类型在驱动中的编号*/
   UCHAR szBoardType[DEV_MAX_DESC_LEN];     /* 该单板类型的字符串信息*/
}DEV_BOARD_TYPE_S;

/* OAP板卡规格信息 */
typedef struct tagDevOapInfo
{
    UINT uiMaxOapNum;                /* 系统支持的OAP卡的总数 */
    UINT uiBoardOapNum;              /* 每个板上支持的OAP卡的subslot数 */
    UINT uiOapStartSubslot;          /* 每个板上从哪个subslot开始支持OAP卡 */
}DEV_OAP_CONF_S;

/* LINE规格数据 */
#define LINE_CONF_TTYNAME_LENGTH     31UL
typedef struct tagDevLineConf
{
    UINT uiMaxNodeCon;                                      /* 每个板上支持Console口的个数 */
    UINT uiMaxNodeAux;                                      /* 每个板上支持Aux口的个数 */
    UINT uiMaxTty;                                          /* 每个板上支持TTY口的个数 */
    UINT uiMaxOneDimensionNum;                              /* 最大一维串口数目*/
    UINT uiMaxBoardNum;                                     /* 最大支持TTY的单板数目*/
    UINT uiMaxBoardCard;                                    /* 单板最大子卡数目*/
    UINT uiMaxCardTtyNum;                                   /* 子卡最大TTY数目*/
    UINT uiTtyStartMinor;                                   /* tty起始次设备号 */
    UINT uiAuxMinor;                                        /* con、aux合一设备专用，合一口的次设备号 */
    CHAR szConsoleDevName[LINE_CONF_TTYNAME_LENGTH+1];      /* console口对应的设备文件名 */
    CHAR szAuxDevName[LINE_CONF_TTYNAME_LENGTH+1];          /* aux对应的设备文件名 */
    CHAR szDefaultDevName[LINE_CONF_TTYNAME_LENGTH+1];      /* 默认串口对应的设备文件(simware和接口板使用) */
    CHAR szConAuxName[LINE_CONF_TTYNAME_LENGTH+1];          /* con、aux合一设备专用，合一口对应的设备文件名 */
    UINT uiMaxCardETtyNum;                                  /* 子卡最大ETTY数目*/
    UINT uiEttyStartMinor;                                  /* etty起始次设备号 */
} DEV_LINE_CONF_S;

/* 设备类型的枚举 */
ISSU typedef enum tagDevType
{
    DEVICE_TYPE_OTHER,               /* 其他设备类型 */
    DEVICE_TYPE_PSE,                 /* PSE */
    DEVICE_TYPE_PSETYPE2,            /* PSE TYEP 2*/
    DEVICE_TYPE_PD,                  /* PD */
    DEVICE_TYPE_PDTYPE2,             /* PD TYPE 2*/
    DEVICE_TYPE_BUTT
}DEV_DEVTYPE_E;

ISSU typedef struct DEV_DeviceMess
{
    USHORT usGlobSlot;               /* 全局槽号 */
    UINT uiDeviceType;               /* 设备类型取值DEV_DEVTYPE_E */
}DEV_DEVICEMESS_S;

/* LED Status控制状态 */
#define DEV_LED_SETSTATUS_START  0   /* 开始控制状态 */
#define DEV_LED_SETSTATUS_STOP   1   /* 结束控制状态 */

/* LED Status灯控制状态 */
#define DEV_LED_ALWAYS_OFF       0x0
#define DEV_LED_ALWAYS_ON        0x01
#define DEV_LED_SLOW_RAY         0x02
#define DEV_LED_QUICK_RAY        0x04
#define DEV_LED_BUSY_RAY         0x08

/* 设备管理驱动接口结构 */
typedef struct tagDEV_DRV_INTF_S
{
    ULONG (*pfDRV_DEV_GetInfo)(IN ULONG ulClassType,
                               IN USHORT usChassis,
                               IN USHORT usSlot,
                               IN USHORT usSubSlot,
                               IN DEV_INFO_CMD_E enCmd,
                               OUT VOID *pParam);
    ULONG (*pfDRV_DEV_GetSelfLocation)(OUT USHORT *pusChassis,
                                       OUT USHORT *pusSlot);
    ULONG (*pfDRV_DEV_GetMasterSlot)(OUT USHORT *pusMasterSlot);
    ULONG (*pfDRV_DEV_GetSlaveSlot)(IN ULONG ulBufLen,
                                    OUT USHORT *pusSlot,
                                    OUT ULONG *pulSlotNum);
    ULONG (*pfDRV_DEV_GetLocalMasterSlot)(IN USHORT usChassis,
                                          OUT USHORT *pusLocalMasterSlot);
    ULONG (*pfDRV_DEV_GetBootType)(IN USHORT usSlot, IN USHORT usSubSlot, OUT ULONG *pulBootType);
    ULONG (*pfDRV_DEV_GetDeviceStatus)(IN DEV_DEVICE_TYPE_E enDeviceType,
                                       IN USHORT usChassis,
                                       IN USHORT usSlot,
                                       IN USHORT usSubSlot,
                                       IN USHORT usIndex,
                                       OUT DEV_STATUS_E *penStatus);
    ULONG (*pfDRV_DEV_GetDeviceInfo)(IN DEV_DEVICE_TYPE_E enDeviceType,
                                     IN USHORT usChassis,
                                     IN USHORT usSlot,
                                     IN USHORT usSubSlot,
                                     IN USHORT usIndex,
                                     IN BOOL_T bGetSelf,
                                     IN BOOL_T bVerbose,
                                     INOUT ULONG *pulContext,
                                     OUT CHAR *pchInfo);
    ULONG (*pfDRV_DEV_Reboot)(IN USHORT usChassis,
                              IN USHORT usSlot,
                              IN USHORT usSubSlot,
                              IN DEV_BOOTCAUSE_E enCause);
    ULONG (*pfDRV_DEV_LEDControl)(IN USHORT usSlot,
                                  IN ULONG ulType,
                                  IN ULONG ulStatus);
    ULONG (*pfDRV_BootRom_Ioctl)(IN USHORT usSlot,
                                 IN USHORT usSubSlot,
                                 IN ULONG ulCmd,
                                 IN UINT uiFunType,
                                 INOUT VOID *pParam,
                                 INOUT ULONG *pulLen);
    ULONG (*pfDRV_BootImage_Ioctl)(IN USHORT usSlot,
                                   IN USHORT usSubSlot,
                                   IN ULONG ulCmd,
                                   OUT VOID *pParam,
                                   INOUT ULONG *pulLen);
    ULONG (*pfDRV_DEV_SetManufactureInfo)(IN DEV_DEVICE_TYPE_E enDevType,
                                          IN USHORT usSlot,
                                          IN USHORT usSubSlot,
                                          IN USHORT usIndex,
                                          IN UCHAR *pucInfo);
    ULONG (*pfDRV_DEV_GetManufactureInfo)(IN DEV_DEVICE_TYPE_E enDevType,
                                          IN USHORT usSlot,
                                          IN USHORT usSubSlot,
                                          IN USHORT usIndex,
                                          IN ULONG ulCmd,
                                          OUT UCHAR *pucInfo);
    ULONG (*pfDRV_DEV_GetRebootCause)(IN USHORT usSlot, IN USHORT usSubSlot, OUT CHAR *pcCause);
    DEV_DEVTYPE_E (*pfDRV_DEV_GetDeviceType)(VOID);
    ULONG (*pfDRV_DEV_GetFabricSlot)(IN ULONG ulBufLen,
                                     OUT USHORT *pusSlot,
                                     OUT ULONG *pulSlotNum);
    ULONG (*pfDRV_DEV_SetLEDStatus)(IN USHORT usSlot, IN ULONG ulLEDType, IN ULONG ulOpType, IN ULONG ulStatus);
    /* 获取当前系统的在位的光板的槽位号列表 */
    ULONG (*pfDRV_DEV_GetFakeSlot)(IN ULONG ulBufLen,
                                   OUT USHORT *pusSlot,
                                   OUT ULONG *pulSlotNum);
    /* 获取光板的所有依赖节点和生效依赖节点信息 */
    ULONG (*pfDRV_DEV_GetFakeSlotInfo)(IN USHORT usChassis,
                                       IN USHORT usSlot,
                                       IN ULONG ulBufLen,
                                       OUT LIP_ADDR *pstRelyNodes,
                                       OUT ULONG *pulNodeNum,
                                       OUT LIP_ADDR *pstActiveRelyNode);
}DEV_DRV_INTF_S, *PDEV_DRV_INTF_S;

extern ULONG DEV_Register_Driver(IN ULONG ulVersion, IN const DEV_DRV_INTF_S *pstIntf);
extern VOID DEV_BridgeMacChangeNotify(IN UINT uiMDCId, IN const DEV_EVT_BMAC_INFO_S *pstMacInfo);
extern VOID Dev_SetMinOnlineFan(IN UINT uiFanNum);

#ifdef __cplusplus
}
#endif

#endif  /* end of _DRV_DEV_H_ */

