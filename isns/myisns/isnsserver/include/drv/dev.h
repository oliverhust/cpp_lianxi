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

/* �豸������Чֵ,��ʾ�����ĵĿ��/�ۺ�/�Ӳۺŵ� */
#define DEV_INVALID_ID                      0xffff

#define DEV_FILE_NAME_MAX_LEN               64UL    /* �ļ�����󳤶� */
#define DEV_BOOTIMAGE_MAX_FILENUM           32      /* �����ļ��б����ļ������Ŀ */

#define DEV_MAX_DESC_LEN                    64UL    /* ������Ϣ��󳤶� */
#define DEV_MAX_VER_LEN                     64UL    /* ������ְ汾��Ϣ����󳤶� */

#define DEV_COLD_BOOT                       0       /* ������ */
#define DEV_WARM_BOOT                       1       /* ������ */

#define DEV_LED_TYPE_RUN                    0UL     /* RUNָʾ��*/
#define DEV_LED_TYPE_ALARM                  1UL     /* ALARMָʾ��*/
#define DEV_LED_TYPE_LOCATOR                2UL     /* LOCATORָʾ��*/

#define DEV_LED_STATUS_ON                   0       /* ��� */
#define DEV_LED_STATUS_OFF                  1       /* ��� */

#define DEV_OID_MAXLEN                      256UL   /* OID���е���󳤶� */
#define DEV_DEVICE_INFO_MAX_LEN             4096UL  /* ��Դ��ʾ��Ϣ��󳤶� */

#define BOOTROM_BASE                        0U   /* ���������� */
#define BOOTROM_EXT                         1U   /* ������չ�� */
#define BOOTROM_ALL                         2U   /* ��������������չ�� */
#define BOOTROM_EXT_IN_ALL                  3U   /* �Ӱ�������������չ�ε��ļ��У�ֻ�������е���չ�� */

#define DEV_PID_MAXLEN                     128UL    /* PID��󳤶� */
#define DEV_SN_MAXLEN                      128UL    /* SN��󳤶� */

/* ÿ���(��������)�����֧�ֵĴ�������Ŀ,��������,�����,���ȵ� */
#define DEV_MAX_SENSOR_NUMBER               9

/* ���޵���Чֵ */
#define DEV_INVALID_LIMIT                   (INT)0xFFFF

/* ��ȡ��ʾ��Ϣ�����������Ŀ�ʼ�ͽ�����ǵĺ궨�� */
#define DEV_DEVICEINFO_START                0UL
#define DEV_DEVICEINFO_END                  ~0UL

/* DEV_EVT_BMAC_CHANGE�¼���Ϣ */
typedef struct tagDevEvtBmacInfo
{
    UCHAR aucOldMac[MAC_ADDR_LEN];   /* �ɵ�BMAC */
    UCHAR aucNewMac[MAC_ADDR_LEN];   /* �µ�BMAC */
}DEV_EVT_BMAC_INFO_S;
/* ���ڴ�������ȡOIDֵ�����ݽṹ */
typedef struct tagDEV_OID
{
    UINT  uiLen; /* OID���еĳ���,��UINTΪ��λ,���ΪDEV_OID_MAXLEN */
    UINT *puiOID;/* ����OID���е��ڴ��ַ,ƽ̨���䳤��ΪDEV_OID_MAXLEN��UINT���ڴ� */
}DEV_OID_S;

/* �豸���� */
ISSU typedef enum tagDEV_DEVICE_TYPE
{
    DEV_TYPE_SLOT,                  /* ������ӿ���Ϣ */
    DEV_TYPE_HDISK,                 /* Ӳ����Ϣ */
    DEV_TYPE_CFCARD,                /* CF����Ϣ */
    DEV_TYPE_USB,                   /* USB����Ϣ */
    DEV_TYPE_POWER,                 /* ��Դ��Ϣ */
    DEV_TYPE_RPS,                   /* RPS��Ϣ */
    DEV_TYPE_FAN,                   /* ������Ϣ */
    DEV_TYPE_SENSOR,                /* ������ */
    DEV_TYPE_POWERMONITOR,          /* ��Դ�����Ϣ */
    DEV_TYPE_CHASSIS,               /* ����Դ��Ϣ */
    DEV_TYPE_CPU,                   /* CPU��Ϣ */
    DEV_TYPE_FLASH,                 /* FLASH��Ϣ */
    DEV_TYPE_VOLTAGESENSOR,         /* ��ѹ��Ϣ */
    DEV_TYPE_SDCARD,                /* SD����Ϣ */
    DEV_TYPE_BUTT
}DEV_DEVICE_TYPE_E;

/* �豸������Դ����״̬ö��ֵ���� */
ISSU typedef enum tagDevStatus
{
    DEV_STATUS_ABSENT,              /*0 ����λ */
    DEV_STATUS_NORMAL,              /*1 ���� */
    DEV_STATUS_FAIL,                /*2 ����̬ */
    DEV_STATUS_BUTT
}DEV_STATUS_E;


/* ������Դ����״̬ö��ֵ���壬����Absent��Normal��fail״̬������DEV_STATUS_E����һ�� */
ISSU typedef enum tagDevFanStatus
{
    DEV_FANSTATUS_ABSENT,              /*0 ����λ */
    DEV_FANSTATUS_NORMAL,              /*1 ���� */
    DEV_FANSTATUS_FAIL,                /*2 ����̬ */
    DEV_FANSTATUS_DIRECTIONFAULT,      /*3 ���ȷ������ */
    DEV_FANSTATUS_BUTT
}DEV_FANSTATUS_E;

/* ��Դ��Դ����״̬ö��ֵ���壬����Absent��Normal��Fail״̬������DEV_STATUS_E����һ�� */
typedef enum tagDevPowerStatus
{
    DEV_POWERSTATUS_ABSENT,       /*0 ����λ */
    DEV_POWERSTATUS_NORMAL,       /*1 ���� */
    DEV_POWERSTATUS_FAIL,         /*2 ����̬ */
    DEV_POWERSTATUS_INCOMPATIBLE, /*3 ��Դ�����ݴ��� */
    DEV_POWERSTATUS_BUTT
}DEV_POWERSTATUS_E;

/* ͨ�û���״̬��ȡֵ */
ISSU typedef enum tagDevEnvState
{
    DEV_ENV_OK,                     /* ���� */
    DEV_ENV_HIGH,                   /* ƫ�� */
    DEV_ENV_LOW,                    /* ƫ�� */
    DEV_ENV_BUTT
}DEV_ENV_STATE_E;

/* �¶�״̬ȡֵ */
ISSU typedef enum tagDevTemState
{
    DEV_TEM_OK,                     /* �¶�����̬ */
    DEV_TEM_LOWER,                  /* �¶ȴ�������̬ */
    DEV_TEM_UPPER,                  /* �¶ȴ�������̬ */
    DEV_TEM_UPPER2,                 /* ��������̬ */
    DEV_TEM_UPPER3,                 /* �ض�����̬ */
    DEV_TEM_BUTT
}DEV_TEM_STATE_E;

/* �����ļ����������Զ��� */
ISSU typedef enum tagDevBootImgType
{
    DEV_BOOTIMAGE_MAIN,                    /* ������ */
    DEV_BOOTIMAGE_BACKUP,                  /* ������ */
    DEV_BOOTIMAGE_BUTT
}DEV_BOOTIMAGE_TYPE_E;

/* �����ļ������� */
ISSU typedef enum tagBootImageCmd
{
    BOOTIMAGE_SET_NEXTMAIN = 1,     /* �����´��������ļ� */
    BOOTIMAGE_SET_NEXTBACKUP,       /* �����´α��������ļ� */

    BOOTIMAGE_GET_CURRENT_ATTR,     /* ��ȡ���������ļ����� */
    BOOTIMAGE_GET_NEXTMAIN,         /* ��ȡ�´��������ļ� */
    BOOTIMAGE_GET_NEXTBACKUP,       /* ��ȡ�´α��������ļ� */
    BOOTIMAGE_GET_LOCATION,         /* ��ȡ�����ļ��洢λ�� */

    BOOTIMAGE_VERIFYBYFILE,         /* �����ļ�����֤�����ļ��Ϸ��� */
    BOOTIMAGE_VERIFYBYCONT,         /* ����������֤�����ļ��Ϸ��� */

    BOOTIMAGE_CMD_BUTT
}BOOTIMAGE_CMD_E;

/* Bootrom���������� */
typedef enum tagBootromCmd
{
    BOOTROM_UPDATE = 1,             /* ����Bootrom */
    BOOTROM_READ,                   /* ��ȡ���ļ�ϵͳ�� */
    BOOTROM_BACKUP,                 /* ���ݵ������� */
    BOOTROM_RESTORE,                /* �ӱ������ָ� */
    BOOTROM_VERIFYBYFILE,           /* ��֤�ļ��Ϸ��� */
    BOOTROM_GET_ACCESS,             /* ���ڻ�ȡ�Ƿ������û�����BootRom */
    BOOTROM_SET_ACCESS,             /* ���������Ƿ������û�����BootRom */
    BOOTROM_UPDATE_CHK,             /* ���������Ƿ������bootrom���м��*/

    BOOTROM_CMD_BUTT
}BOOTROM_CMD_E;


/* �豸���������Ϣ��ȡ������ */
typedef enum tagDEV_INFO_CMD
{
    DEV_INFO_Version,               /* �˽ṹ�İ汾�� */
    DEV_INFO_MaxChassisID,          /* ����� */
    DEV_INFO_MinChassisID,          /* ��С��� */
    DEV_INFO_MinSlotID,             /* ��С��λ�� */
    DEV_INFO_MaxSlotID,             /* ����λ�� */
    DEV_INFO_MaxMPUNum,             /* ������ذ���Ŀ */
    DEV_INFO_MaxSubCardNum,         /* ֧�ֵ�����ӿ���Ŀ */
    DEV_INFO_MaxPortNum,            /* ֧�ֵ����˿��� */
    DEV_INFO_FirstFanId,            /* ���������� */
    DEV_INFO_MaxFanNum,             /* �������� */
    DEV_INFO_MaxCPUNum,             /* ���CPU��Ŀ */
    DEV_INFO_FirstPowerId,          /* ��Դ������ */
    DEV_INFO_MaxPowerNum,           /* ����Դ�� */
    DEV_INFO_MaxRPSNum,             /* ���RPS��Ŀ */
    DEV_INFO_MaxCFCardNum,          /* ���CF����λ�� */
    DEV_INFO_MaxUSBNum,             /* ���USB���� */
    DEV_INFO_MaxHDDNum,             /* ���HDD���� */

    DEV_INFO_TempSensorInfo,        /* �¶ȴ�������Ŀ������Ϣ */
    DEV_INFO_DefVoltageInfo,        /* ȱʡ��ѹ��Ϣ */
    DEV_INFO_DefCurrentInfo,        /* ȱʡ������Ϣ */

    DEV_INFO_Sup_FanExtShow,        /* �Ƿ�֧�ֲ鿴������ϸ��Ϣ */
    DEV_INFO_Sup_PnPNotify,         /* �Ƿ�֧���Ȳ���ϱ� */
    DEV_INFO_Sup_SubCardMaintain,   /* �Ƿ�֧���ӿ�ά�� */
    DEV_INFO_Sup_BootromExtend,     /* �Ƿ�֧��Bootrom��չ���� */
    DEV_INFO_Sup_BootImageSlave,    /* �Ƿ�֧��BootImage�������� */
    DEV_INFO_Sup_SubSlot2Slot,      /* �Ƿ�֧�ֶ�άת��ά */
    DEV_INFO_Sup_SetTempLimit,      /* �Ƿ�֧�������¶����� */
    DEV_INFO_Sup_LEDControl,        /* �Ƿ�֧��LED��ƿ��� */
    DEV_INFO_BOOTROM_TAG,           /* ��õ����ӦBootware��DeviceCode */
    DEV_INFO_OS_TAG,                /* ��õ����Ӧ���������DeviceCode */
    DEV_INFO_MinPwrMonId,           /* ��Դ���ģ����Сʵ������  */
    DEV_INFO_MaxPwrMonId,           /* ��Դ���ģ�����ʵ������  */
    DEV_INFO_MaxChannelNum,         /* ���ͨ��������ת��оƬ������ȣ�*/
    DEV_INFO_OAP,                   /* ֧�ֵ�OAP�忨��Ϣ*/
    DEV_INFO_LINE,                  /* ֧�ֵ�LINE������Ϣ */
    DEV_INFO_VirtualChassisID,      /* �����ID */
    DEV_INFO_IsExtendedModule,      /* ȡ���ڵ��ǲ�����չģ�� */
    DEV_INFO_Voltage1stPhysicalID,  /* ��ȡĳλ�õĵ�ѹ������ID */
    DEV_INFO_VoltageSensorNum,      /* ��ȡĳλ�õĵ�ѹ���������� */
    DEV_INFO_BUTT
}DEV_INFO_CMD_E;

typedef struct tagDEV_PID_INFO
{
    UCHAR aucBuf[DEV_PID_MAXLEN];    /* �豸PID���� */
    UINT uiLen;       /* �豸PID���� */
    UINT uiAlignMode; /* �ü�����ģʽ,ȡֵ�ο�DEV_PID_TYPE_E */
}DEV_PID_INFO_S;
/* License PID����ö�� */
typedef enum tagLicDeviceIDType
{
    DEV_PID_ALIGN_NONE = 0,      /* ����18�ֽڣ�����䣬������ */
    DEV_PID_ALIGN_RIGHT_PAD,     /* �Ҳಹ�����ݣ�����18�ֽ� */
    DEV_PID_ALIGN_LEFT_DISCARD,  /* ��ඪ�����ݣ�������18�ֽ� */
    DEV_PID_ALIGN_RIGHT_DISCARD,  /* �Ҳඪ�����ݣ�����ǰ18�ֽ� */
    DEV_PID_ALIGN_BUTT
}DEV_PID_TYPE_E;
typedef struct tagDEV_SN_INFO
{
    UCHAR aucBuf[DEV_SN_MAXLEN];    /* �豸SN���� */
    UINT uiLen;       /* �豸SN���� */
}DEV_SN_INFO_S;

/* ����ԭ��ö��ֵ */
ISSU typedef enum tagDevBootCause{
    BOOT_CAUSE_UI,              /* 0 ͨ��������,SNMP,Web�ȹ����ֶ������豸 */
    BOOT_CAUSE_EXCEP,           /* 1 �ں��쳣���� */
    BOOT_CAUSE_DEADLOOP,        /* 2 �ں���ѭ������ */
    BOOT_CAUSE_DEVHANDSHAKE,    /* 3 �豸��������ʧ������ */
    BOOT_CAUSE_LCSOVERTIME,     /* 4 LICENSE��ʱ */
    BOOT_CAUSE_SLAVESWITCH,     /* 5 ������������ԭ���� */
    BOOT_CAUSE_SLAVECANNOTUPDATE,/* 6 ���岻������Ϊ���� */
    BOOT_CAUSE_IRFMERGE,        /* 7 �ѵ�Merge�������� */
    BOOT_CAUSE_AUTOUPDATE,      /* 8 �汾�Զ����� */
    BOOT_CAUSE_MASTERLOST,      /* 9 �ڱ�������ʱ���������� */
    BOOT_CAUSE_UNKNOWN,         /* 10 δ֪ԭ�� */
    BOOT_CAUSE_OOM,             /* 11 �����ڴ�ľ����� */
    BOOT_CAUSE_CRYPTOTESTFAIL,   /* 12 �㷨���Լ�ʧ�� */
    BOOT_CAUSE_BUTT
}DEV_BOOTCAUSE_E;

/* ���������� */
ISSU typedef enum tagDevSensorType{
    DEV_SENSOR_INFLOW = 0,     /* ��� */
    DEV_SENSOR_OUTFLOW,        /* ���� */
    DEV_SENSOR_HOTSPOT,        /* �ȵ� */
    DEV_SENSOR_INVALID         /* �Ƿ�ֵ */
}DEV_SENSOR_TYPE_E;

/* ����ֵ�ṹ,���������¶ȡ���ѹ�͵����ĸ澯����ֵ����Ϊ4������ */
ISSU typedef struct  tagDEV_THRESHOLD
{
      INT iLowerLimit;                  /* �澯���� */
      INT iUpperLimit;                  /* һ�㼶(Warning)�澯���� */
      INT iUpperLimit2;                 /* ���ؼ�(Alarm)�澯���� */
      INT iUpperLimit3;                 /* �ضϼ���澯���� */
}DEV_THRESHOLD_S;

/* ����������ֵ�ṹ����,���ڲ�Ʒ���Ƹ���������������Ĭ��ֵ�ͷ�Χ */
ISSU typedef struct tagDEV_THRESHOLD_REG_INFO
{
    INT iDefLowerLimit;             /* Ĭ�ϸ澯���� */
    INT iMinLowerLimit;             /* ��͸澯���� */
    INT iMaxLowerLimit;             /* ��߸澯���� */
    INT iDefUpperLimit;             /* Ĭ��һ�㼶(Warning)�澯���� */
    INT iMinUpperLimit;             /* ���һ�㼶(Warning)�澯���� */
    INT iMaxUpperLimit;             /* ���һ�㼶(Warning)�澯���� */
    INT iDefUpperLimit2;            /* Ĭ�����ؼ�(Alarm)�澯���� */
    INT iMinUpperLimit2;            /* ������ؼ�(Alarm)�澯���� */
    INT iMaxUpperLimit2;            /* ������ؼ�(Alarm)�澯���� */
    INT iDefUpperLimit3;            /* Ĭ�Ϲضϼ���澯���� */
    INT iMinUpperLimit3;            /* ��͹ضϼ���澯���� */
    INT iMaxUpperLimit3;            /* ��߹ضϼ���澯���� */
}DEV_THRESHOLD_REG_INFO_S;

/* �������޽ṹ�����������������޺��������Сֵ��Χ */
ISSU typedef struct tagDEV_ENV_LIMIT
{
    INT iDefLowerLimit;           /* Ĭ�ϸ澯���� */
    INT iMinLowerLimit;           /* ��͸澯���� */
    INT iMaxLowerLimit;           /* ��߸澯���� */
    INT iDefUpperLimit;           /* Ĭ�ϸ澯���� */
    INT iMinUpperLimit;           /* ��͸澯���� */
    INT iMaxUpperLimit;           /* ��߸澯���� */
}DEV_ENV_LIMIT_S;


/* �¶ȴ�������Ϣ�ṹ */
ISSU typedef struct tagDEV_TEMP_SENSOR_INFO
{
    UINT uiInflowNum;               /* ���ڴ�������Ŀ��0��ʾ��֧�ָ������� */
    UINT uiInflowStartID;           /* ���ڴ�������ʼ���� */
    UINT uiHotspotNum;              /* �ȵ㴫������Ŀ */
    UINT uiHotspotStartID;          /* �ȵ㴫������ʼ���� */
    UINT uiOutflowNum;              /* ����ڴ�������Ŀ */
    UINT uiOutflowStartID;          /* ����ڴ�������ʼ���� */

    /* ������������Ĭ��������Ϣ,���յ��壨������ϣ���ƫ��˳����,ͬһ����֮�䰴��λ������,
       �ɲ�Ʒ���չ淶�����Ӧ�����ڲ�֧�ֵ�������������Чֵ */
    DEV_THRESHOLD_REG_INFO_S astThresholdRegInfo[DEV_MAX_SENSOR_NUMBER];
}DEV_TEMP_SENSOR_INFO_S;

/* ���ӱ�ǩ��Ϣ��ȡ�Ĳ������� */
#define DEV_GET_MANUINFO_PART           0UL
#define DEV_GET_MANUINFO_ALL            1UL


/* �������ͽṹ */
typedef struct tagDevBoardType
{
   UINT  uiMibBoardType;                    /* MIB������,�ð�������MIB�ж�Ӧ�ı��*/
   UINT  uiBoardType;                       /* �õ��������������еı��*/
   UCHAR szBoardType[DEV_MAX_DESC_LEN];     /* �õ������͵��ַ�����Ϣ*/
}DEV_BOARD_TYPE_S;

/* OAP�忨�����Ϣ */
typedef struct tagDevOapInfo
{
    UINT uiMaxOapNum;                /* ϵͳ֧�ֵ�OAP�������� */
    UINT uiBoardOapNum;              /* ÿ������֧�ֵ�OAP����subslot�� */
    UINT uiOapStartSubslot;          /* ÿ�����ϴ��ĸ�subslot��ʼ֧��OAP�� */
}DEV_OAP_CONF_S;

/* LINE������� */
#define LINE_CONF_TTYNAME_LENGTH     31UL
typedef struct tagDevLineConf
{
    UINT uiMaxNodeCon;                                      /* ÿ������֧��Console�ڵĸ��� */
    UINT uiMaxNodeAux;                                      /* ÿ������֧��Aux�ڵĸ��� */
    UINT uiMaxTty;                                          /* ÿ������֧��TTY�ڵĸ��� */
    UINT uiMaxOneDimensionNum;                              /* ���һά������Ŀ*/
    UINT uiMaxBoardNum;                                     /* ���֧��TTY�ĵ�����Ŀ*/
    UINT uiMaxBoardCard;                                    /* ��������ӿ���Ŀ*/
    UINT uiMaxCardTtyNum;                                   /* �ӿ����TTY��Ŀ*/
    UINT uiTtyStartMinor;                                   /* tty��ʼ���豸�� */
    UINT uiAuxMinor;                                        /* con��aux��һ�豸ר�ã���һ�ڵĴ��豸�� */
    CHAR szConsoleDevName[LINE_CONF_TTYNAME_LENGTH+1];      /* console�ڶ�Ӧ���豸�ļ��� */
    CHAR szAuxDevName[LINE_CONF_TTYNAME_LENGTH+1];          /* aux��Ӧ���豸�ļ��� */
    CHAR szDefaultDevName[LINE_CONF_TTYNAME_LENGTH+1];      /* Ĭ�ϴ��ڶ�Ӧ���豸�ļ�(simware�ͽӿڰ�ʹ��) */
    CHAR szConAuxName[LINE_CONF_TTYNAME_LENGTH+1];          /* con��aux��һ�豸ר�ã���һ�ڶ�Ӧ���豸�ļ��� */
    UINT uiMaxCardETtyNum;                                  /* �ӿ����ETTY��Ŀ*/
    UINT uiEttyStartMinor;                                  /* etty��ʼ���豸�� */
} DEV_LINE_CONF_S;

/* �豸���͵�ö�� */
ISSU typedef enum tagDevType
{
    DEVICE_TYPE_OTHER,               /* �����豸���� */
    DEVICE_TYPE_PSE,                 /* PSE */
    DEVICE_TYPE_PSETYPE2,            /* PSE TYEP 2*/
    DEVICE_TYPE_PD,                  /* PD */
    DEVICE_TYPE_PDTYPE2,             /* PD TYPE 2*/
    DEVICE_TYPE_BUTT
}DEV_DEVTYPE_E;

ISSU typedef struct DEV_DeviceMess
{
    USHORT usGlobSlot;               /* ȫ�ֲۺ� */
    UINT uiDeviceType;               /* �豸����ȡֵDEV_DEVTYPE_E */
}DEV_DEVICEMESS_S;

/* LED Status����״̬ */
#define DEV_LED_SETSTATUS_START  0   /* ��ʼ����״̬ */
#define DEV_LED_SETSTATUS_STOP   1   /* ��������״̬ */

/* LED Status�ƿ���״̬ */
#define DEV_LED_ALWAYS_OFF       0x0
#define DEV_LED_ALWAYS_ON        0x01
#define DEV_LED_SLOW_RAY         0x02
#define DEV_LED_QUICK_RAY        0x04
#define DEV_LED_BUSY_RAY         0x08

/* �豸���������ӿڽṹ */
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
    /* ��ȡ��ǰϵͳ����λ�Ĺ��Ĳ�λ���б� */
    ULONG (*pfDRV_DEV_GetFakeSlot)(IN ULONG ulBufLen,
                                   OUT USHORT *pusSlot,
                                   OUT ULONG *pulSlotNum);
    /* ��ȡ�������������ڵ����Ч�����ڵ���Ϣ */
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

