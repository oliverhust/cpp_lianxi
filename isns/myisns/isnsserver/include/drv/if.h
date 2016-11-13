/*****************************************************************************
 Copyright (c) 2007, Hangzhou H3C Technologies Co., Ltd. All rights reserved.
------------------------------------------------------------------------------
                            if_drv.h
  Project Code: Comware Leopard
   Module Name: IFNET DRV
  Date Created: 2009-6-16
        Author: h01260
   Description: Header file of ifnet module for product

------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/

#ifndef _DRV_IF_H_
#define _DRV_IF_H_

#ifdef  __cplusplus
extern "C" {
#endif

#define IF_MAX_NAME_LEN                         47UL        /* 接口全名最大长度 */

/* 对于小于100M的速率，uiData直接表示速率，最高bit位为0
 * 对于大于或等于100M的速率，uiData的数值单位为K，最高bit位为1
 * if(0 != (uiData & IF_SPEED_HI))
 * {
 *     实际速率 = (ulData & (~IF_SPEED_HI)) * IF_HISPEED_UNIT;
 *          使用时请注意，实际速率可能会超过32bit
 * }
 */
#define IF_HISPEED_UNIT 1000U                      /* 1K为单位 */
#define IF_SPEED_HI     0x80000000U                /* 最高bit位为1表示该值以IF_HISPEED_UNIT为单位 */

#define IF_BAUD_10M     10000000U                  /* 十兆波特率 */
#define IF_BAUD_100M    100000000U                /* 百兆波特率 */
#define IF_BAUD_1G      (IF_SPEED_HI | 1000000U)   /* 千兆波特率 */
#define IF_BAUD_10G     (IF_SPEED_HI | 10000000U)  /* 万兆波特率 */
#define IF_BAUD_20G     (IF_SPEED_HI | 20000000U)  /* 二万兆波特率 */
#define IF_BAUD_40G     (IF_SPEED_HI | 40000000U)  /* 四万兆波特率 */
#define IF_BAUD_100G    (IF_SPEED_HI | 100000000U) /* 十万兆波特率 */

/* 二/三层接口 */
enum enPORTLAYER
{
    IF_LAYER_L2 = 1,      /* 二层端口 */
    IF_LAYER_L3,          /* 三层接口 */
    IF_LAYER_IDS          /* IDS不确定口 */
};

/* 物理层类型 */
ISSU typedef enum enPHYTYPE
{
    PHY_UNKNOWN = 0,        /* 0: 不确定的物理类型，启动时配置恢复用 */

    /* =========== ETH相关的物理类型 ============ */
    PHY_ETH_10M,            /* 1: 10M以太网口 */
    PHY_ETH_FE,             /* 2: 快速以太网口 100M */
    PHY_ETH_GE,             /* 3: G比特以太网口 GE */
    PHY_ETH_XGE,            /* 4: 10G比特以太网口 10GE */
    PHY_ETH_FGE,            /* 5: 40G比特以太网口 */
    PHY_ETH_HGE,            /* 6: 100G比特以太网口 */

    /* =========== ATM相关的物理类型 ============ */
    PHY_ATM,                /* 7 : ATM物理口，使用ATM扩展特性的产品请使用下面的细分类型 */
    PHY_ATM_25M,            /* 8 : 25M ATM口 */
    PHY_ATM_155M,           /* 9 : 155M */
    PHY_ATM_622M,           /* 10: 622M */
    PHY_ATM_ADSL,           /* 11: ADSL接口，不对称数字用户线 */
    PHY_ATM_SHDSL,          /* 12: SHDSL接口，对称高速数字用户线 */
    PHY_ATM_E1,             /* 13: 基于E1的ATM */
    PHY_ATM_E3,             /* 14: 基于E1的ATM */
    PHY_ATM_T1,             /* 15: 基于T1的ATM */
    PHY_ATM_T3,             /* 16: 基于T3的ATM */
    PHY_ATM_IMA,            /* 17: ATM捆绑口 */
    PHY_ATM_ADSL_2PLUS,     /* 18: ADSL 2Plus接口 */
    PHY_ATM_SHDSL_4WIRE,    /* 19: 4线 SHDSL ATM口 */
    PHY_ATM_SHDSL_4WIRE_BIS,/* 20: 4线G.BIS的SHDSL ATM口 */
    PHY_ATM_SHDSL_8WIRE,    /* 21: 8线 SHDSL ATM接口 */
    PHY_ATM_SHDSL_8WIRE_BIS,/* 22: 8线G.BIS的SHDSL ATM口 */
    PHY_ATM_IMA_SHDSL_8WIRE,/* 23: 基于8线 SHDSL ATM捆绑口 */

    /* =========== Controller相关的物理类型 ============ */
    PHY_FT1,                /* 24: 部分化的T1口 */
    PHY_T1,                 /* 25: T1口 */
    PHY_DVT1,               /* 26: 同时支持语音数据的T1口 */
    PHY_VT1,                /* 27: 只支持语音的T1口 */
    PHY_T3,                 /* 28: T3口 */
    PHY_CT3,                /* 29: 通道化的T3口 */
    PHY_FE1,                /* 30: 部分化的E1口 */
    PHY_E1,                 /* 31: E1口 */
    PHY_DVE1,               /* 32: 同时支持语音数据的E1口 */
    PHY_VE1,                /* 33: 只支持语音的E1口 */
    PHY_E3,                 /* 34: E3口 */
    PHY_CE3,                /* 35: 通道化的E3口 */
    PHY_E1POS,              /* 36: E1POS */
    PHY_CELLULAR,           /* 37: CELLULAR接口 */

    /* =========== POS/CPOS/E-CPOS口相关物理类型 ============*/
    PHY_POS,                /* 38: POS口 */
    PHY_CPOS_CE1,           /* 39: 基于CE1的通道化POS口 */
    PHY_CPOS_CT1,           /* 40: 基于CT1的通道化POS口 */
    PHY_CPOS_E1,            /* 41: 基于E1的通道化POS口 */
    PHY_CPOS_E3,            /* 42: 基于E3的通道化POS口 */
    PHY_CPOS_FE3,           /* 43: 基于部分化E3的POS口 */
    PHY_CPOS_T1,            /* 44: 基于T1的通道化POS口 */
    PHY_CPOS_T3,            /* 45: 基于T3的通道化POS口 */
    PHY_CPOS_FT3,           /* 46: 基于部分化T3的POS口 */
    PHY_CPOS_E3CE1,         /* 47: 基于CE1E1融合后通道化的POS口 */
    PHY_CPOS_T3CT1,         /* 48: 基于CT1T1融合后通道化的POS口 */
    PHY_CPOS_155ME3T3,      /* 49: 155ME-Cpos接口 */
    PHY_CPOS_622M,          /* 50: 622M E-Cpos接口 */
    PHY_CPOS_622ME3T3,      /* 51: 可通道化到E3T3 622ME-Cpos接口*/
    PHY_CPOS_2DOT5G,        /* 52: 2.5G E-Cpos */

    /* =========== 其它物理口的物理类型 ============ */
    PHY_CONSOLE,            /* 53: 控制台console口 */
    PHY_ISDN,               /* 54: ISDN口 */
    PHY_PRI,                /* 55: PRI口,对应接口类型为同步串口 */
    PHY_SYNC,               /* 56: 同步串口 */
    PHY_VOICE_PORT,         /* 57: 语音口 */
    PHY_ASYNC,              /* 58: 异步串口 */
    PHY_AUX,                /* 59: AUX口 */
    PHY_BCHAN,              /* 60: BRI口的B道物理类型 */
    PHY_BRI,                /* 61: BRI口的D道物理类型 */
    PHY_BSV,                /* 62: Bri S/T Voice接口物理类型 */
    PHY_CASCADE,            /* 63: 堆叠口 */
    PHY_FCM,                /* 64: FCM接口 */
    PHY_INASYNC,            /* 65: 内部同异步串口 */

    /* =========== 以下是逻辑口对应的物理类型 ============ */
    PHY_LOGICTUNNEL,        /* 66: 逻辑隧道 */
    PHY_LOOP,               /* 67: LOOP口 */
    PHY_LOOPBACK,           /* 68: LOOPBACK口 */
    PHY_MFR,                /* 69: 捆绑帧中继 */
    PHY_MP_GROUP,           /* 70: PPP捆绑组 */
    PHY_NULL,               /* 71: NULL口 */
    PHY_TEMPLATE,           /* 72: VT模板 */
    PHY_VIRTUALACCESS,      /* 73: VA口   */
    PHY_TEMPLATEMPCHAN,     /* 74: MP通道 */
    PHY_TUNNEL,             /* 75: 隧道口 */
    PHY_VIRTUAL_ETHERNET,   /* 76: VE口 */
    PHY_VLAN,               /* 77: VLAN虚接口 */
    PHY_DIALER,             /* 78: 拨号口 */
    PHY_BVI,                /* 79: 桥模板 */
    PHY_REGISTERTUNNEL,     /* 80: Register-Tunnel注册接口 */
    PHY_MTUNNEL,            /* 81: 多播隧道逻辑接口 */
    PHY_HDLCB,              /* 82: HDLCB捆绑接口 */
    PHY_EVI_LINK,           /* 83: EVI LINK */
    PHY_SCHANNEL,           /* 84: SCHANNEL */
    PHY_L2TP,               /* 85: Virtual-PPP接口 */
 
    /* =========== 加密卡相关的物理类型 ============ */
    PHY_NDEC,               /* 86: 加密卡1 */
    PHY_CAVIUM,             /* 87: 加密卡2 */
    PHY_SSLVPN,             /* 88: SSLVPN接口物理类型 */
    PHY_SSL_ENC,            /* 89: SSL类型加密卡 */
    PHY_IPSEC_ENC,          /* 90: IPSEC类型加密卡 */

    /* BEGIN L3+ */
    PHY_NAT,                /* 91: L3+ NAT业务接口 */
    PHY_EACL,               /* 92: L3+ EACL业务接口 */
    PHY_NETSTREAM,          /* 93: L3+ NetStream业务接口 */
    PHY_URPF,               /* 94: L3+ URPF业务接口 */
    PHY_MPLS_VPLS,          /* 95: L3+ MPLS-L2VPN-VPLS业务接口 */
    PHY_MPLS_VLL,           /* 96: L3+ MPLS-L2VPN-VLL业务接口 */
    PHY_MPLS_L3VPN,         /* 97: L3+ MPLS-L3VPN业务接口 */
    PHY_MPLS,               /* 98: L3+ 普通MPLS业务接口 */
    /* END   L3+ */

    /* BEGIN RPR */
    PHY_RPR_GE,             /* 99: G比特 RPR逻辑口 */
    PHY_RPR_XGE,            /* 100: 10G比特 RPR逻辑口 */
    PHY_RPR_XGPOS,          /* 101: 10G POS RPR逻辑口 */
    PHY_RPR_2DOT5GPOS,      /* 102: 2.5G POS RPR逻辑口 */
    PHY_RPR_PHY_GE,         /* 103: G比特 RPR物理口 */
    PHY_RPR_PHY_XGE,        /* 104: 10G比特 RPR物理口 */
    PHY_RPR_PHY_XGPOS,      /* 105: 10G POS RPR物理口 */
    PHY_RPR_PHY_2DOT5GPOS,  /* 106: 2.5G POS RPR物理口 */
    /* END   PRR */

    /* BEGIN: VOICE */
    PHY_CAS,                /* 107: 在DVE1或者VE1接口下通过Timeslot-set命令创建出该接口类型 */
    PHY_CCS,                /* 108: 在DVE1或者VE1接口下通过命令PRI-SET创建的给语音保存配置的时隙组接口类型 */
    PHY_FXS,                /* 109: Fxs */
    PHY_FXO,                /* 110: Fxo */
    PHY_AEM,                /* 111: 模拟em */
    PHY_VTS,                /* 112: 给语音使用的B通道接口类型 */
    PHY_BSV_CCS,            /* 113: Bri S/T Voice 接口对应的语音用户线主接口 */
    /* END: VOICE */

    /* Begin: WLAN 相关的物理类型 */
    PHY_RADIO,              /* 114: WLAN-Radio接口 */
    PHY_WLAN_ESS,           /* 115: 无线交换机二层WLAN模板逻辑口 */
    PHY_WLAN_DBSS,          /* 116: 无线交换机二层WLAN接入虚接口 */
    PHY_WLAN_BSS,           /* 117: 无线路由器二层WLAN接入虚接口 */
    PHY_WLAN_ETHERNET,      /* 118: 无线路由器三层WLAN接入虚接口 */
    PHY_WLAN_TUNNEL,        /* 119: 无线产品二层tunnel虚接口 */
    PHY_WLAN_MESHLINK,      /* 120: 无线WLAN-MESH虚拟口 */
    PHY_WLAN_MESH,          /* 121: 无线产品WLAN-MESH配置模板虚接口 */
    /* End: WLAN 相关的物理类型 */

    /* BEGIN: ONU/OLT */
    PHY_OLT_64,             /* 122: 分光比为1:64的OLT(Optical Line Terminal)接口 */
    PHY_ONU,                /* 123: ONU(Optical Network Unit)接口 */
    /* END: ONU/OLT */

    /* BEGIN: LAGG */
    PHY_RAGG,               /* 124: 三层聚合接口 */
    PHY_BAGG,               /* 125: 二层聚合接口 */
    /* END: LAGG */

    /* BEGIN: FC */
    PHY_FC,                 /* 126: FC */
    PHY_VFC,                /* 127: VFC */
    PHY_FCB,                /* 128: FCB */
    /* END: FC */

    PHY_FC_XGE,             /* 129: 可切换FC的XGE*/

    PHY_ETH_TGE,            /* 130: 20G比特以太网口 */
    /*========= PE上行口=====================*/
    PHY_PEGE,               /* 131: PE侧GE上行口*/
    PHY_PEXGE,              /* 132: PE侧XGE上行口*/
    PHY_PEFGE,              /* 133: PE侧FGE上行口*/
    PHY_PEHGE,              /* 134: PE侧HGE上行口*/
    PHY_BLADE,              /* 135: 引擎口 */
    PHY_BLAGG,              /* 136: 引擎聚合口 */

    /* ETH冗余口 */
    PHY_BETH,               /* 137: 二层ETH冗余口 */
    PHY_RETH,               /* 138: 三层ETH冗余口 */
    PHY_VSI,                /* 139: 全局VSI口，MPLS组使用 */
    PHY_CEM,                /* 140: CEM口物理类型 */
    PHY_INCOMM,             /* 141: 内部通信口 */
    PHY_E1DM,               /* 142: E1DM */
    PHY_E1POSDM,            /* 143: E1POSDM */
    PHY_RPR_PHY_L2LOCAL,    /* 144: 二层RPR局部口 */
    PHY_SCHBDL,             /* 145: Schannle-Bundle口 */
    PHY_CMTUNNEL,           /* 146: CMTunnel口物理类型 */
    PHY_BUTT = 0xffff       /* 不存在的物理类型 */
}IF_PHYTYPE_E;

/* 接口类型 */
ISSU typedef enum tagIF_TYPE
{
    IF_COMMON_TYPE = 0,          /* 0 */
    IF_AM_TYPE,                  /* 1 */
    IF_ASY_TYPE,                 /* 2 */
    IF_IMA_TYPE,                 /* 3 */
    IF_ATM_TYPE,                 /* 4 */
    IF_AUX_TYPE,                 /* 5 */
    IF_BRI_TYPE,                 /* 6 */
    IF_BVI_TYPE,                 /* 7 */
    IF_CONSOLE_TYPE,             /* 8 */
    IF_CPOS_TYPE,                /* 9 */
    IF_DIALER_TYPE,              /* 10 */
    IF_LOCAL_VA_TYPE,            /* 11 */
    IF_E1_TYPE,                  /* 12 */
    IF_E3_TYPE,                  /* 13 */
    IF_INETH_TYPE,               /* 14 */
    IF_INLOOPBACK_TYPE,          /* 15 */
    IF_LOOPBACK_TYPE,            /* 16 */
    IF_ENCRYPT_TYPE,             /* 17 */
    IF_L2ETHERNET_TYPE,          /* 18 */
    IF_L2GE_TYPE,                /* 19 */
    IF_L2VE_TYPE,                /* 20 */
    IF_L2XGE_TYPE,               /* 21 */
    IF_L3ETHERNET_TYPE,          /* 22 */
    IF_L3GE_TYPE,                /* 23 */
    IF_L3VE_TYPE,                /* 24 */
    IF_L3XGE_TYPE,               /* 25 */
    IF_ME_TYPE,                  /* 26 */
    IF_MFR_TYPE,                 /* 27 */
    IF_MGE_TYPE,                 /* 28 */
    IF_MPGROUP_TYPE,             /* 29 */
    IF_NULL_TYPE,                /* 30 */
    IF_POS_TYPE,                 /* 31 */
    IF_RPR_POS_TYPE,             /* 32 */
    IF_RPR_XGE_TYPE,             /* 33 */
    IF_RPR_GE_TYPE,              /* 34 */
    IF_SERIAL_TYPE,              /* 35 */
    IF_SUBSCRIBER_LINE_TYPE,     /* 36 */
    IF_T1_TYPE,                  /* 37 */
    IF_T3_TYPE,                  /* 38 */
    IF_TUNNEL_TYPE,              /* 39 */
    IF_GLOBAL_VA_TYPE,           /* 40 */
    IF_VLAN_TYPE,                /* 41 */
    IF_VT_TYPE,                  /* 42 */
    IF_RADIO_DOT11_TYPE,         /* 43 */
    IF_WLAN_ESS_TYPE,            /* 44 */
    IF_WLAN_DBSS_TYPE,           /* 45 */
    IF_WLAN_BSS_TYPE,            /* 46 */
    IF_WLAN_ETH_TYPE,            /* 47 */
    IF_WLAN_TUN_TYPE,            /* 48 */
    IF_OLT_TYPE,                 /* 49 */
    IF_ONU_TYPE,                 /* 50 */
    IF_MTUNNEL_TYPE,             /* 51 */
    IF_CELLULAR_TYPE,            /* 52 */
    IF_INASY_TYPE,               /* 53 */
    IF_L2RPR_TYPE,               /* 54 */
    IF_L3RPR_TYPE,               /* 55 */
    IF_BAGG_TYPE,                /* 56 */
    IF_REGISTER_TUNNEL_TYPE,     /* 57 */
    IF_LOOPFE_TYPE,              /* 58 */
    IF_LOOPGE_TYPE,              /* 59 */
    IF_LOOPXGE_TYPE,             /* 60 */
    IF_MIRRORFE_TYPE,            /* 61 */
    IF_MIRRORGE_TYPE,            /* 62 */
    IF_MIRRORXGE_TYPE,           /* 63 */
    IF_IDSFE_TYPE,               /* 64 */
    IF_IDSGE_TYPE,               /* 65 */
    IF_IDSXGE_TYPE,              /* 66 */
    IF_RAGG_TYPE,                /* 67 */
    IF_L2FGE_TYPE,               /* 68 */
    IF_L2HGE_TYPE,               /* 69 */
    IF_L3FGE_TYPE,               /* 70 */
    IF_L3HGE_TYPE,               /* 71 */
    IF_LOOPFGE_TYPE,             /* 72 */
    IF_LOOPHGE_TYPE,             /* 73 */
    IF_MIRRORFGE_TYPE,           /* 74 */
    IF_MIRRORHGE_TYPE,           /* 75 */
    IF_IDSFGE_TYPE,              /* 76 */
    IF_IDSHGE_TYPE,              /* 77 */
    IF_FC_TYPE,                  /* 78 */
    IF_VFC_TYPE,                 /* 79 */
    IF_FCB_TYPE,                 /* 80 */
    IF_EVI_LINK_TYPE,            /* 81 */
    IF_SCHANNEL_TYPE,            /* 82 */
    IF_FCM_TYPE,                 /* 83 */
    IF_VPPP_TYPE,                /* 84 */
    IF_TRILL_REMOTE_TYPE,        /* 85 */
    IF_HDLCBUNDLE_TYPE,          /* 86 */
    IF_GSCHANNEL_TYPE,           /* 87 */
    IF_L2TGE_TYPE,               /* 88 */
    IF_L3TGE_TYPE,               /* 89 */
    IF_LOOPTGE_TYPE,             /* 90 */
    IF_MIRRORTGE_TYPE,           /* 91 */
    IF_IDSTGE_TYPE,              /* 92 */
    IF_AC_TYPE,                  /* 93 */
    IF_UPW_TYPE,                 /* 94 */
    IF_NPW_TYPE,                 /* 95 */
    IF_TUNNEL_BUNDLE_TYPE,       /* 96 */
    IF_EFM_TYPE,                 /* 97 */
    IF_MAPPED_TYPE,              /* 98 */
    IF_CBGE_TYPE,                /* 99 */
    IF_CBXGE_TYPE,               /* 100 */
    IF_CBFGE_TYPE,               /* 101 */
    IF_CBHGE_TYPE,               /* 102 */
    IF_PEX_TYPE,                 /* 103 */
    IF_VE_L2VPN_TYPE,            /* 104 */
    IF_VE_L3VPN_TYPE,            /* 105 */
    IF_BLADE_TYPE,               /* 106 */
    IF_BLAGG_TYPE,               /* 107 */
    IF_ETHCHANNEL_TYPE,          /* 108 */
    IF_BETH_TYPE,                /* 109 */
    IF_RETH_TYPE,                /* 110 */
    IF_VSI_TYPE,                 /* 111 */
    IF_CEM_TYPE,                 /* 112 */
    IF_INCOMM_TYPE,              /* 113 */
    IF_SSLVPNAC_TYPE,            /* 114 */
    IF_LISP_TYPE,                /* 115 */
    IF_L2RPRLOCAL_TYPE,          /* 116 */
    IF_SCHANNEL_BUNDLE_TYPE,     /* 117 */
    IF_WLAN_MESH_TYPE,           /* 118 */
    IF_WLAN_MESHLINK_TYPE,       /* 119 */
    IF_CMTUNNEL_TYPE,            /* 120 */
    IF_MAX_TYPE = 128            /* 128 */
} IF_TYPE_E;

/* 接口类型名称字符串，供产品组装接口名称 */
#define IF_TYPENAME_AM          "Analogmodem"           /* 1 : IF_AM_TYPE               */ 
#define IF_TYPENAME_ASY         "Async"                 /* 2 : IF_ASY_TYPE              */ 
#define IF_TYPENAME_IMA         "IMA-group"             /* 3 : IF_IMA_TYPE              */
#define IF_TYPENAME_ATM         "ATM"                   /* 4 : IF_ATM_TYPE              */
#define IF_TYPENAME_AUX         "Aux"                   /* 5 : IF_AUX_TYPE              */ 
#define IF_TYPENAME_BRI         "Bri"                   /* 6 : IF_BRI_TYPE              */ 
#define IF_TYPENAME_BVI         "Bridge-template"       /* 7 : IF_BVI_TYPE              */ 
#define IF_TYPENAME_CONSOLE     "Console"               /* 8 : IF_CONSOLE_TYPE          */ 
#define IF_TYPENAME_CPOS        "Cpos"                  /* 9 : IF_CPOS_TYPE             */ 
#define IF_TYPENAME_DIALER      "Dialer"                /* 10: IF_DIALER_TYPE           */
#define IF_TYPENAME_LOCALVA     "Virtual-Access"        /* 11: IF_LOCAL_VA_TYPE         */
#define IF_TYPENAME_E1          "E1 "                   /* 12: IF_E1_TYPE               */
#define IF_TYPENAME_E3          "E3 "                   /* 13: IF_E3_TYPE               */
#define IF_TYPENAME_INETH       "Inner-Ethernet"        /* 14: IF_INETH_TYPE            */
#define IF_TYPENAME_INLOOPBACK  "InLoopBack"            /* 15: IF_INLOOPBACK_TYPE       */
#define IF_TYPENAME_LOOPBACK    "LoopBack"              /* 16: IF_LOOPBACK_TYPE         */
#define IF_TYPENAME_ENCRYPT     "Encrypt"               /* 17: IF_ENCRYPT_TYPE          */
#define IF_TYPENAME_L2FE        "Ethernet"              /* 18: IF_L2ETHERNET_TYPE       */
#define IF_TYPENAME_L2GE        "GigabitEthernet"       /* 19: IF_L2GE_TYPE             */
#define IF_TYPENAME_L2VE        "VE-Bridge"             /* 20: IF_L2VE_TYPE             */
#define IF_TYPENAME_L2XGE       "Ten-GigabitEthernet"   /* 21: IF_L2XGE_TYPE            */
#define IF_TYPENAME_L3FE        "Ethernet"              /* 22: IF_L3ETHERNET_TYPE       */
#define IF_TYPENAME_L3GE        "GigabitEthernet"       /* 23: IF_L3GE_TYPE             */
#define IF_TYPENAME_L3VE        "Virtual-Ethernet"      /* 24: IF_L3VE_TYPE             */
#define IF_TYPENAME_L3XGE       "Ten-GigabitEthernet"   /* 25: IF_L3XGE_TYPE            */
#define IF_TYPENAME_ME          "M-Ethernet"            /* 26: IF_ME_TYPE               */
#define IF_TYPENAME_MFR         "MFR"                   /* 27: IF_MFR_TYPE              */
#define IF_TYPENAME_MGE         "M-GigabitEthernet"     /* 28: IF_MGE_TYPE              */
#define IF_TYPENAME_MPGROUP     "MP-group"              /* 29: IF_MPGROUP_TYPE          */
#define IF_TYPENAME_NULL        "NULL"                  /* 30: IF_NULL_TYPE             */
#define IF_TYPENAME_POS         "Pos"                   /* 31: IF_POS_TYPE              */
#define IF_TYPENAME_RPRPOS      "RPRPOS"                /* 32: IF_RPR_POS_TYPE          */
#define IF_TYPENAME_RPRXGE      "RPRXGE"                /* 33: IF_RPR_XGE_TYPE          */
#define IF_TYPENAME_RPRGE       "RPRGE"                 /* 34: IF_RPR_GE_TYPE           */
#define IF_TYPENAME_SERIAL      "Serial"                /* 35: IF_SERIAL_TYPE           */
#define IF_TYPENAME_VOICE       "subscriber-line"       /* 36: IF_SUBSCRIBER_LINE_TYPE  */
#define IF_TYPENAME_T1          "T1 "                   /* 37: IF_T1_TYPE               */
#define IF_TYPENAME_T3          "T3 "                   /* 38: IF_T3_TYPE               */
#define IF_TYPENAME_TUNNEL      "Tunnel"                /* 39: IF_TUNNEL_TYPE           */
#define IF_TYPENAME_GLOBALVA    "Virtual-Access"        /* 40: IF_GLOBAL_VA_TYPE        */
#define IF_TYPENAME_VLAN        "Vlan-interface"        /* 41: IF_VLAN_TYPE             */
#define IF_TYPENAME_VT          "Virtual-Template"      /* 42: IF_VT_TYPE               */
#define IF_TYPENAME_RADIODOT11  "WLAN-Radio"            /* 43: IF_RADIO_DOT11_TYPE      */
#define IF_TYPENAME_WLANESS     "WLAN-ESS"              /* 44: IF_WLAN_ESS_TYPE         */
#define IF_TYPENAME_WLANDBSS    "WLAN-DBSS"             /* 45: IF_WLAN_DBSS_TYPE        */
#define IF_TYPENAME_WLANBSS     "WLAN-BSS"              /* 46: IF_WLAN_BSS_TYPE         */
#define IF_TYPENAME_WLANETH     "WLAN-Ethernet"         /* 47: IF_WLAN_ETH_TYPE         */
#define IF_TYPENAME_WLANTUNNEL  "WLAN-Tunnel"           /* 48: IF_WLAN_TUN_TYPE         */
#define IF_TYPENAME_OLT         "Olt"                   /* 49: IF_OLT_TYPE              */
#define IF_TYPENAME_ONU         "Onu"                   /* 50: IF_ONU_TYPE              */
#define IF_TYPENAME_MTUNNEL     "MTunnel"               /* 51: IF_MTUNNEL_TYPE          */
#define IF_TYPENAME_CELLULAR    "Cellular"              /* 52: IF_CELLULAR_TYPE         */
#define IF_TYPENAME_INASY       "InAsync"               /* 53: IF_INASY_TYPE            */
#define IF_TYPENAME_L2RPR       "RPR-Bridge"            /* 54: IF_L2RPR_TYPE            */
#define IF_TYPENAME_L3RPR       "RPR-Router"            /* 55: IF_L3RPR_TYPE            */
#define IF_TYPENAME_BAGG        "Bridge-Aggregation"    /* 56: IF_BAGG_TYPE,            */
#define IF_TYPENAME_REGTUNNEL   "Register-Tunnel"       /* 57: IF_REGISTER_TUNNEL_TYPE  */
#define IF_TYPENAME_LOOPFE      "Ethernet"              /* 58: IF_LOOPFE_TYPE           */
#define IF_TYPENAME_LOOPGE      "GigabitEthernet"       /* 59: IF_LOOPGE_TYPE           */
#define IF_TYPENAME_LOOPXGE     "Ten-GigabitEthernet"   /* 60: IF_LOOPXGE_TYPE          */
#define IF_TYPENAME_MIRRORFE    "Ethernet"              /* 61: IF_MIRRORFE_TYPE         */
#define IF_TYPENAME_MIRRORGE    "GigabitEthernet"       /* 62: IF_MIRRORGE_TYPE         */
#define IF_TYPENAME_MIRRORXGE   "Ten-GigabitEthernet"   /* 63: IF_MIRRORXGE_TYPE        */
#define IF_TYPENAME_IDSFE       "Ethernet"              /* 64: IF_IDSFE_TYPE            */
#define IF_TYPENAME_IDSGE       "GigabitEthernet"       /* 65: IF_IDSGE_TYPE            */
#define IF_TYPENAME_IDSXGE      "Ten-GigabitEthernet"   /* 66: IF_IDSXGE_TYPE           */
#define IF_TYPENAME_RAGG        "Route-Aggregation"     /* 67: IF_RAGG_TYPE             */
#define IF_TYPENAME_L2FGE       "FortyGigE"             /* 68: IF_L2FGE_TYPE            */
#define IF_TYPENAME_L2HGE       "HundredGigE"           /* 69: IF_L2HGE_TYPE            */
#define IF_TYPENAME_L3FGE       "FortyGigE"             /* 70: IF_L3FGE_TYPE            */
#define IF_TYPENAME_L3HGE       "HundredGigE"           /* 71: IF_L3HGE_TYPE            */
#define IF_TYPENAME_LOOPFGE     "FortyGigE"             /* 72: IF_LOOPFGE_TYPE          */
#define IF_TYPENAME_LOOPHGE     "HundredGigE"           /* 73: IF_LOOPHGE_TYPE          */
#define IF_TYPENAME_MIRRORFGE   "FortyGigE"             /* 74: IF_MIRRORFGE_TYPE        */
#define IF_TYPENAME_MIRRORHGE   "HundredGigE"           /* 75: IF_MIRRORHGE_TYPE        */
#define IF_TYPENAME_IDSFGE      "FortyGigE"             /* 76: IF_IDSFGE_TYPE           */
#define IF_TYPENAME_IDSHGE      "HundredGigE"           /* 77: IF_IDSHGE_TYPE           */
#define IF_TYPENAME_FC          "Fc"                    /* 78: IF_FC_TYPE               */
#define IF_TYPENAME_VFC         "Vfc"                   /* 79: IF_VFC_TYPE              */
#define IF_TYPENAME_FCB         "SAN-Aggregation"       /* 80: IF_FCB_TYPE              */
#define IF_TYPENAME_EVILINK     "EVI-Link"              /* 81: IF_EVI_LINK_TYPE         */
#define IF_TYPENAME_SCHANNEL    "S-Channel"             /* 82: IF_SCHANNEL_TYPE         */
#define IF_TYPENAME_FCM         "Fcm"                   /* 83: IF_FCM_TYPE              */
#define IF_TYPENAME_VPPP        "Virtual-PPP"           /* 84: IF_VPPP_TYPE             */
#define IF_TYPENAME_HDLCBUNDLE  "HDLC-bundle"           /* 86: IF_HDLCBUNDLE_TYPE       */
#define IF_TYPENAME_GSCHANNEL   "Schannel-Aggregation"  /* 87: IF_GSCHANNEL_TYPE        */
#define IF_TYPENAME_L2TGE       "TwentyGigE"            /* 88: IF_L2TGE_TYPE            */
#define IF_TYPENAME_L3TGE       "TwentyGigE"            /* 89: IF_L3TGE_TYPE            */
#define IF_TYPENAME_LOOPTGE     "TwentyGigE"            /* 90: IF_LOOPTGE_TYPE          */
#define IF_TYPENAME_MIRRORTGE   "TwentyGigE"            /* 91: IF_MIRRORTGE_TYPE        */
#define IF_TYPENAME_IDSTGE      "TwentyGigE"            /* 92: IF_IDSTGE_TYPE           */
#define IF_TYPENAME_TUNNELBUNDLE "Tunnel-Bundle"        /* 96: IF_TUNNEL_BUNDLE_TYPE    */
#define IF_TYPENAME_EFM         "EFM"                   /* 97: IF_EFM_TYPE              */
#define IF_TYPENAME_CBGE        "GigabitEthernet"       /* 99: IF_CBGE_TYPE             */
#define IF_TYPENAME_CBXGE       "Ten-GigabitEthernet"   /* 100: IF_CBXGE_TYPE           */
#define IF_TYPENAME_CBFGE       "FortyGigE"             /* 101: IF_CBFGE_TYPE           */
#define IF_TYPENAME_CBHGE       "HundredGigE"           /* 102: IF_CBHGE_TYPE           */
#define IF_TYPENAME_PEX         "Pex"                   /* 103: IF_PEX_TYPE             */
#define IF_TYPENAME_VE_L2VPN    "VE-L2VPN"              /* 104: IF_VE-L2VPN_TYPE        */
#define IF_TYPENAME_VE_L3VPN    "VE-L3VPN"              /* 105: IF_VE-L3VPN_TYPE        */
#define IF_TYPENAME_BLADE       "Blade"                 /* 106: IF_BLADE_TYPE           */
#define IF_TYPENAME_BLAGG       "Blade-Aggregation"     /* 107: IF_BLAGG_TYPE           */
#define IF_TYPENAME_ETHCHANNEL  "Eth-channel"           /* 108: IF_ETHCHANNEL_TYPE      */
#define IF_TYPENAME_RETH        "Reth"                  /* 110: IF_RETH_TYPE            */
#define IF_TYPENAME_VSI         "Vsi-interface"         /* 111: IF_VSI_TYPE             */
#define IF_TYPENAME_CEM         "Circuit-Emulation"     /* 112: IF_CEM_TYPE             */
#define IF_TYPENAME_INCOMM      "InComm"                /* 113: IF_INCOMM_TYPE          */
#define IF_TYPENAME_SSLVPNAC    "SSLVPN-AC"             /* 114: IF_SSLVPNAC_TYPE        */
#define IF_TYPENAME_LISP        "LISP"                  /* 115: IF_LISP_TYPE            */
#define IF_TYPENAME_L2RPR_LOCAL "RPR"                   /* 116: IF_L2RPRLOCAL_TYPE      */
#define IF_TYPENAME_SCHBUNDLE   "Schannel-Bundle"       /* 117: IF_SCHANNEL_BUNDLE_TYPE  */
#define IF_TYPENAME_WLANMESH    "WLAN-Mesh"             /* 118: IF_WLANMESH_TYPE        */
#define IF_TYPENAME_WLANMESHLINK "WLAN-MeshLink"        /* 119: IF_WLANMESHLINK_TYPE    */
#define IF_TYPENAME_CMTUNNEL     "CMTunnel"             /* 120: IF_CMTUNNEL_TYPE        */
struct tagIfPortInfo;
typedef ULONG (*IF_CREATE_COMPLETE_ROUTINE_PF)(IN struct tagIfPortInfo *pstPortInfo);

/*
uiProperty 接口属性，低3位表示接口二、三层类型，从第4位开始表示接口硬件能力
限制，0表示没有限制，具体说明如下：
[0]:最后一位表示接口是否可以进行二、三层类型间切换，1表示可以切换，0表示不可以。
[1-2]: 表示接口缺省的二、三层类型。01表示缺省为二层接口，10表示缺省为三层接口，
       00表示完全由驱动控制的接口，目前未使用，11预留。
[3]:IPV4能力限制，  1表述不具有IPV4能力，0表示有
[4]:IPV6能力限制，  1表述不具有IPV6能力，0表示有
[5]:TUNNEL能力限制，1表示不具有隧道能力，0表示有
[6]:MPLS能力限制，  1表示不具有MPLS能力，0表示有
[7]:IPX能力限制，   1表示不具有IPX能力，0表示有
[8]:子接口创建限制，1表示不能创建子接口，0表示可以创建子接口
[9]:二层接口是否能创建二层子接口，默认不能创建，置上后能创建
[10]:组播VPN能力限制，1表示支持组播VPN功能，0表示不支持
[11]:当前接口是否是双机热备接口。若是，平台会将该接口设置成不可见不可配。
[12-13]:表示接口在MDC中的分配能力，0表示不能分配，1表示独占分配，2表示共享分配

以上分别对应下面的宏：
*/
#define IF_PORT_NOSWITCH            ((UINT)0x0000)      /* 0: 不支持二三层切换，适用于所有非以太网接口 */
#define IF_PORT_L2L3_SWITCH         ((UINT)0x0001)      /* 0: 支持二三层切换，只适用于以太网类接口 */
#define IF_PORT_DEFAULT_L2          ((UINT)0x0002)      /* 1: 缺省为二层接口，只适用于以太网类接口 */
#define IF_PORT_DEFAULT_L3          ((UINT)0x0004)      /* 2: 缺省为三层接口，与IF_PORT_DEFAULT_L2互斥 */
#define IF_PORT_LIMITATIONS_IPv4    ((UINT)0x0008)      /* 3: */
#define IF_PORT_LIMITATIONS_IPv6    ((UINT)0x0010)      /* 4: */
#define IF_PORT_LIMITATIONS_TUNNEL  ((UINT)0x0020)      /* 5: */
#define IF_PORT_LIMITATIONS_MPLS    ((UINT)0x0040)      /* 6: */
#define IF_PORT_LIMITATIONS_IPX     ((UINT)0x0080)      /* 7: */
#define IF_PORT_LIMITATIONS_SUBIF   ((UINT)0x0100)      /* 8: */
#define IF_PORT_LIMITATIONS_L2SUBIF ((UINT)0x0200)      /* 9: */
#define IF_PORT_LIMITATIONS_MTUNNEL ((UINT)0x0400)      /* 10 */
#define IF_PORT_LIMITATIONS_DHBK    ((UINT)0x0800)      /* 11 */
#define IF_PORT_ALLOCMDC_FIXED      ((UINT)0x1000)      /* 12 */
#define IF_PORT_ALLOCMDC_SHARE      ((UINT)0x2000)      /* 13 */
#define IF_PORT_COMBO_FIBER         ((UINT)0x4000)      /* 14: combo光口模式 */
#define IF_PORT_COMBO_COPPER        ((UINT)0x8000)      /* 15: combo电口模式 */
#define IF_PORT_FCOE                ((UINT)0x10000)     /* 16: FCOE能力 */
#define IF_PORT_TRILL               ((UINT)0x20000)     /* 17: TRILL能力 */
#define IF_PORT_FC                  ((UINT)0x40000)     /* 18: FC切换能力*/   
#define IF_PORT_SPLIT_FROM_FGE      ((UINT)0x80000)     /* 19: 接口从40GE口分裂而来 */
#define IF_PORT_SPLIT_FROM_HGE      ((UINT)0x100000)    /* 20: 接口从100GE口分裂而来 */
#define IF_PORT_AUTOCFG             ((UINT)0x200000)    /* 21: 接口为autocfg配置所创建*/
#define IF_PORT_CONNECTION_MODE     ((UINT)0x400000)    /* 22: 接口支持ACFP引流扩展模式 */
#define IF_PORT_LIMITATIONS_MULTIPORT  ((UINT)0x800000) /* 23 */
#define IF_PORT_LIMITATIONS_VSIGATEWAY ((UINT)0x1000000)/* 24 */

/* IF_PORT_INFO_S.usChkSumAbility 取值在mbuf.h中定义
   此字段定义了接口的CheckSum硬件计算能力，包括用位表示的UDP、TCP等报文的校验和计算能力
   上层模块可以通过IF_GetDataByIfIndex接口IF_PORT_CHECKSUM命令字查询此字段
0表示不支持硬件计算CheckSum。
此能力在接口实体所在板（接口板、主控板、备板等）获取
*/

/* 驱动上下文信息数组尺寸 */
#define IF_DRVCTX_SIZE 2

ISSU typedef struct tagIfPortInfo
{
    IF_CREATE_COMPLETE_ROUTINE_PF pfIfCreateCompleteRoutine;    /* 接口创建回调函数 */
    UINT64   aui64DrvContext[IF_DRVCTX_SIZE];           /* 驱动上下文 */
    IF_INDEX ifindex;                                   /* 接口索引 */
    UINT     uiIfType;                                  /* 接口类型 */
    UINT     uiProperty;                                /* 接口属性 */
    UINT     uiMDCIntfGroup;                            /* 接口所在组号 */
    UINT     uiPort;                                    /* 接口编号 */
    USHORT   usSlot;                                    /* 槽号 */
    USHORT   usSubSlot;                                 /* 子卡号 */
    USHORT   usPhyType;                                 /* 物理类型 */
    USHORT   usChkSumAbility;                           /* 硬件校验和计算能力 */
    CHAR     szPortName[IF_MAX_NAME_LEN + 1];           /* 接口名字 */
}IF_PORT_INFO_S, *PIF_PORT_INFO_S;

/* 接口报文缓存信息的驱动接口数据结构 */
ISSU typedef struct tagIFBufferInfo
{
    INT    iPktBufFree;          /* 获取报文缓存空闲值 */
    INT    iPktBufInit;          /* 获取报文缓存初始值 */
    INT    iPktBufMin;           /* 获取报文缓存最小值 */
    UINT64 uiPktBufMiss;         /* 获取报文丢弃值 */
}IF_BUFFER_INFO_S;

/* 命令字IOCTL_COMMON_BLOCK/IOCTL_COMMON_UNDO_BLOCK使用 */
#define IF_BLOCKTYPE_DLDP       0x1UL
#define IF_BLOCKTYPE_ETHOAM     0x2UL
#define IF_BLOCKTYPE_MACSEC     0x3UL

/* oam使用 */
typedef struct tagIF_PORT_INFO_EXT_S
{
    UINT64 aui64DrvContext[IF_DRVCTX_SIZE]; /* 驱动上下文 */
    IF_INDEX ifindex;                       /* 接口索引 */
    UINT uiIfType;                          /* 接口类型 */
    UINT uiMDCNo;                           /* 接口所属的MDC */
    UINT uiPhyNo;                           /* 物理编号 */
    USHORT usLogicSlotNo;                   /* 逻辑板号 */
    USHORT usPhySlotNo;                     /* 物理板号 */
    USHORT usSubSlot;                       /* 子卡号 */
    USHORT usSubNo;                         /* 子接口号 */
    USHORT usSubType;                       /* 子接口类型 */
}IF_PORT_INFO_EXT_S, *PIF_PORT_INFO_EXT_S;

typedef struct tagLINK_ENCAP_S
{
    UINT uiProtoType;
    VOID *pLinkEncapParam;
}LINK_ENCAP_S;

/* 用于alloc操作下驱动检测 */
typedef struct tagIF_MDC_ALLOC_INTF_CHECK
{
    UINT uiDestMDC;
    UINT uiSrcMDC;
    UINT uiAllocIfNum;
}IF_MDC_ALLOC_INTF_CHECK_S;

typedef struct tagPROVISION_TYPESWITCH_S
{
    IF_INDEX ifIndex;
    UINT uiNewIftype;
}PROVISION_TYPESWITCH_S;

/* 生成PhyIOCtl命令字的宏 */
#define IF_IOCTL_CMD_DEF(ModId,IfType,Index) ((ModId) | ((IfType) << 8) | (Index))

/* 公共命令字 */
#define IOCTL_COMMON_POWERUP                     IF_IOCTL_CMD_DEF(MID_IFNET, IF_COMMON_TYPE, 0)
#define IOCTL_COMMON_SHUTDOWN                    IF_IOCTL_CMD_DEF(MID_IFNET, IF_COMMON_TYPE, 1)
#define IOCTL_COMMON_UNDO_SHUTDOWN               IF_IOCTL_CMD_DEF(MID_IFNET, IF_COMMON_TYPE, 2)
#define IOCTL_COMMON_BUILDRUN                    IF_IOCTL_CMD_DEF(MID_IFNET, IF_COMMON_TYPE, 3)
#define IOCTL_COMMON_CHECK_PORT_STATUS           IF_IOCTL_CMD_DEF(MID_IFNET, IF_COMMON_TYPE, 4)
#define IOCTL_COMMON_CHECK_FLOW                  IF_IOCTL_CMD_DEF(MID_IFNET, IF_COMMON_TYPE, 5)
#define IOCTL_COMMON_GET_VERSION                 IF_IOCTL_CMD_DEF(MID_IFNET, IF_COMMON_TYPE, 6)
#define IOCTL_COMMON_ENABLE                      IF_IOCTL_CMD_DEF(MID_IFNET, IF_COMMON_TYPE, 7)
#define IOCTL_COMMON_DISABLE                     IF_IOCTL_CMD_DEF(MID_IFNET, IF_COMMON_TYPE, 8)
#define IOCTL_COMMON_ENABLE_RECVEIVE             IF_IOCTL_CMD_DEF(MID_IFNET, IF_COMMON_TYPE, 9)
#define IOCTL_COMMON_DISABLE_RECEIVE             IF_IOCTL_CMD_DEF(MID_IFNET, IF_COMMON_TYPE, 10)
#define IOCTL_COMMON_GET_INFO                    IF_IOCTL_CMD_DEF(MID_IFNET, IF_COMMON_TYPE, 11)
#define IOCTL_COMMON_CLEAR_PORT                  IF_IOCTL_CMD_DEF(MID_IFNET, IF_COMMON_TYPE, 12)
#define IOCTL_COMMON_GET_MAX_BD                  IF_IOCTL_CMD_DEF(MID_IFNET, IF_COMMON_TYPE, 13)
#define IOCTL_COMMON_GET_MIB_INFO                IF_IOCTL_CMD_DEF(MID_IFNET, IF_COMMON_TYPE, 14)
#define IOCTL_COMMON_DELETE                      IF_IOCTL_CMD_DEF(MID_IFNET, IF_COMMON_TYPE, 17)
#define IOCTL_COMMON_INIT                        IF_IOCTL_CMD_DEF(MID_IFNET, IF_COMMON_TYPE, 18)
#define IOCTL_COMMON_SWITCH                      IF_IOCTL_CMD_DEF(MID_IFNET, IF_COMMON_TYPE, 20)
#define IOCTL_COMMON_CREATE_CHANNEL              IF_IOCTL_CMD_DEF(MID_IFNET, IF_COMMON_TYPE, 21)
#define IOCTL_COMMON_CANDELETE                   IF_IOCTL_CMD_DEF(MID_IFNET, IF_COMMON_TYPE, 22)
#define IOCTL_COMMON_GET_HARD_INFO               IF_IOCTL_CMD_DEF(MID_IFNET, IF_COMMON_TYPE, 23)
#define IOCTL_COMMON_GET_PORT_STATUS             IF_IOCTL_CMD_DEF(MID_IFNET, IF_COMMON_TYPE, 24)
#define IOCTL_COMMON_GET_PORT_STATISTIC          IF_IOCTL_CMD_DEF(MID_IFNET, IF_COMMON_TYPE, 25)
#define IOCTL_COMMON_FLOWSTATTABLE               IF_IOCTL_CMD_DEF(MID_IFNET, IF_COMMON_TYPE, 26)
#define IOCTL_COMMON_GET_FE1FT1_INFO             IF_IOCTL_CMD_DEF(MID_IFNET, IF_COMMON_TYPE, 27)
#define IOCTL_COMMON_GET_STATUS_INFO             IF_IOCTL_CMD_DEF(MID_IFNET, IF_COMMON_TYPE, 29)
#define IOCTL_COMMON_SET_MSP_SYN_CLOCK           IF_IOCTL_CMD_DEF(MID_IFNET, IF_COMMON_TYPE, 30)
#define IOCTL_COMMON_SET_MSP_SYN_CLOCK_INTERVAL  IF_IOCTL_CMD_DEF(MID_IFNET, IF_COMMON_TYPE, 31)
#define IOCTL_COMMON_GET_MSP_TYPE                IF_IOCTL_CMD_DEF(MID_IFNET, IF_COMMON_TYPE, 32)
#define IOCTL_COMMON_SWITCH_CHECK                IF_IOCTL_CMD_DEF(MID_IFNET, IF_COMMON_TYPE, 33)
#define IOCTL_COMMON_GET_BSV_RELATED_IF          IF_IOCTL_CMD_DEF(MID_IFNET, IF_COMMON_TYPE, 34)
#define IOCTL_COMMON_LINK_ENCAP                  IF_IOCTL_CMD_DEF(MID_IFNET, IF_COMMON_TYPE, 40)
#define IOCTL_COMMON_LINK_DECAP                  IF_IOCTL_CMD_DEF(MID_IFNET, IF_COMMON_TYPE, 41)
#define IOCTL_COMMON_GET_STATISTICS              IF_IOCTL_CMD_DEF(MID_IFNET, IF_COMMON_TYPE, 42)
#define IOCTL_COMMON_BLOCK                       IF_IOCTL_CMD_DEF(MID_IFNET, IF_COMMON_TYPE, 43)
#define IOCTL_COMMON_UNDO_BLOCK                  IF_IOCTL_CMD_DEF(MID_IFNET, IF_COMMON_TYPE, 44)
#define IOCTL_COMMON_SET_INTF_PAIR               IF_IOCTL_CMD_DEF(MID_IFNET, IF_COMMON_TYPE, 45)
#define IOCTL_COMMON_TYPESWITCH_CHECK            IF_IOCTL_CMD_DEF(MID_IFNET, IF_COMMON_TYPE, 46)
#define IOCTL_COMMON_SET_RCV_ADDRESS             IF_IOCTL_CMD_DEF(MID_IFNET, IF_COMMON_TYPE, 47)
#define IOCTL_COMMON_POWERDOWN                   IF_IOCTL_CMD_DEF(MID_IFNET, IF_COMMON_TYPE, 48)
#define IOCTL_COMMON_LINKDELAY                   IF_IOCTL_CMD_DEF(MID_IFNET, IF_COMMON_TYPE, 49)
#define IOCTL_COMMON_RESET_INFO_DROP             IF_IOCTL_CMD_DEF(MID_IFNET, IF_COMMON_TYPE, 50)
#define IOCTL_COMMON_GET_INFO_DROP               IF_IOCTL_CMD_DEF(MID_IFNET, IF_COMMON_TYPE, 51)
#define IOCTL_COMMON_SET_UP_MODE                 IF_IOCTL_CMD_DEF(MID_IFNET, IF_COMMON_TYPE, 52)/* 设置二三层以太网口的PORTUPMODE ENABLE */
#define IOCTL_COMMON_AUTO_POWER_DOWN             IF_IOCTL_CMD_DEF(MID_IFNET, IF_COMMON_TYPE, 53)/* 设置二三层以太网口的接口自动DOWN 的ENABLE */
#define IOCTL_COMMON_GET_PAYLOAD                 IF_IOCTL_CMD_DEF(MID_IFNET, IF_COMMON_TYPE, 54)
#define IOCTL_COMMON_GET_INERRORPERCENT          IF_IOCTL_CMD_DEF(MID_IFNET, IF_COMMON_TYPE, 55)
#define IOCTL_COMMON_SET_EEEENABLE               IF_IOCTL_CMD_DEF(MID_IFNET, IF_COMMON_TYPE, 56)
#define IOCTL_COMMON_LINK_ENCAP_CHECK            IF_IOCTL_CMD_DEF(MID_IFNET, IF_COMMON_TYPE, 57)
#define IOCTL_COMMON_LINK_DECAP_CHECK            IF_IOCTL_CMD_DEF(MID_IFNET, IF_COMMON_TYPE, 58)
#define IOCTL_COMMON_GET_BER_DEFAULTTHRESHOLD    IF_IOCTL_CMD_DEF(MID_IFNET, IF_COMMON_TYPE, 59)/* 获取XGE/CPOS/EPOS/ATM/RPR口threshold值 */
#define IOCTL_COMMON_SET_BER_THRESHOLD           IF_IOCTL_CMD_DEF(MID_IFNET, IF_COMMON_TYPE, 60)/* 设置XGE/CPOS/EPOS/ATM/RPR口threshold值 */
#define IOCTL_COMMON_SET_AUTOCLOCKCHANGE         IF_IOCTL_CMD_DEF(MID_IFNET, IF_COMMON_TYPE, 61)/* 设置clock-change auto */
#define IOCTL_COMMON_SET_AUTO_DOWN               IF_IOCTL_CMD_DEF(MID_IFNET, IF_COMMON_TYPE, 62)
#define IOCTL_COMMON_SWITCH_RELEASE              IF_IOCTL_CMD_DEF(MID_IFNET, IF_COMMON_TYPE, 63)
#define IOCTL_COMMON_GET_PORTENVINFO             IF_IOCTL_CMD_DEF(MID_IFNET, IF_COMMON_TYPE, 64)
#define IOCTL_COMMON_SET_SERVICESLOT             IF_IOCTL_CMD_DEF(MID_IFNET, IF_COMMON_TYPE, 65)  /* service slot */
#define IOCTL_COMMON_DELETE_PREPROC              IF_IOCTL_CMD_DEF(MID_IFNET, IF_COMMON_TYPE, 66)
#define IOCTL_COMMON_SWITCHTYPE                  IF_IOCTL_CMD_DEF(MID_IFNET, IF_COMMON_TYPE, 67)
#define IOCTL_COMMON_GET_IFBUFFERINFO            IF_IOCTL_CMD_DEF(MID_IFNET, IF_COMMON_TYPE, 68)/* 获取接口上报文缓存信息 */
#define IOCTL_COMMON_GET_ENTPHYID                IF_IOCTL_CMD_DEF(MID_IFNET, IF_COMMON_TYPE, 69)/* 获取接口物理编号(子卡内编号) */
#define IOCTL_COMMON_SET_ALARMACTION             IF_IOCTL_CMD_DEF(MID_IFNET, IF_COMMON_TYPE, 70)/* set alarm-detect action */
#define IOCTL_COMMON_GET_ONLINESTATE             IF_IOCTL_CMD_DEF(MID_IFNET, IF_COMMON_TYPE, 71)/* 获取online/offline状态 */
#define IOCTL_COMMON_SET_CVTHRESHOLDB1           IF_IOCTL_CMD_DEF(MID_IFNET, IF_COMMON_TYPE, 72) /* 设置 threshold B1 阈值 */
#define IOCTL_COMMON_SET_CVTHRESHOLDB2           IF_IOCTL_CMD_DEF(MID_IFNET, IF_COMMON_TYPE, 73) /* 设置 threshold B2 阈值 */
#define IOCTL_COMMON_SET_CVTHRESHOLDB3           IF_IOCTL_CMD_DEF(MID_IFNET, IF_COMMON_TYPE, 74) /* 设置 threshold B3 阈值 */
#define IOCTL_COMMON_GET_SONETMIBINFO            IF_IOCTL_CMD_DEF(MID_IFNET, IF_COMMON_TYPE, 75) /* 获取sonet相关计数信息 */
#define IOCTL_COMMON_GET_MTURANGE                IF_IOCTL_CMD_DEF(MID_IFNET, IF_COMMON_TYPE, 76) /* 获取MTU的范围 */
#define IOCTL_COMMON_GET_IPV4_STATISTICS         IF_IOCTL_CMD_DEF(MID_IFNET, IF_COMMON_TYPE, 77)
#define IOCTL_COMMON_GET_IPV6_STATISTICS         IF_IOCTL_CMD_DEF(MID_IFNET, IF_COMMON_TYPE, 78)
#define IOCTL_COMMON_GET_LSWPORTTABLE_INFO       IF_IOCTL_CMD_DEF(MID_IFNET, IF_COMMON_TYPE, 79) /* 获取端口物理类型以及端口光模块插拔情况 */

/* 以太网PhyIOCtl命令字 */
#define IOCTL_ETHERNET_SET_SPEED                 IF_IOCTL_CMD_DEF(MID_IFNET, IF_L2ETHERNET_TYPE, 0) /* 设置指定端口的速率*/
#define IOCTL_ETHERNET_SET_DUPLEX                IF_IOCTL_CMD_DEF(MID_IFNET, IF_L2ETHERNET_TYPE, 1) /* 设置指定端口的双工 */
#define IOCTL_ETHERNET_GET_SPEED                 IF_IOCTL_CMD_DEF(MID_IFNET, IF_L2ETHERNET_TYPE, 2) /* 获取指定端口的实际速率 */
#define IOCTL_ETHERNET_GET_DEFAULT_SPEED         IF_IOCTL_CMD_DEF(MID_IFNET, IF_L2ETHERNET_TYPE, 3) /* 获得指定端口的缺省速率 */
#define IOCTL_ETHERNET_GET_DUPLEX                IF_IOCTL_CMD_DEF(MID_IFNET, IF_L2ETHERNET_TYPE, 4) /* 获取指定端口的双工状态 */
#define IOCTL_ETHERNET_GET_DEFAULT_DUPLEX        IF_IOCTL_CMD_DEF(MID_IFNET, IF_L2ETHERNET_TYPE, 5) /* 获取指定端口的缺省双工状态 */
#define IOCTL_ETHERNET_GET_DEFAULT_MDIX          IF_IOCTL_CMD_DEF(MID_IFNET, IF_L2ETHERNET_TYPE, 6) /* 获取指定端口的缺省MDIX值 */
#define IOCTL_ETHERNET_SET_MDIX                  IF_IOCTL_CMD_DEF(MID_IFNET, IF_L2ETHERNET_TYPE, 7) /* 设置指定端口的MDIX值 */
#define IOCTL_ETHERNET_GET_MTU                   IF_IOCTL_CMD_DEF(MID_IFNET, IF_L2ETHERNET_TYPE, 8) /* 获取指定端口的MTU */
#define IOCTL_ETHERNET_GET_AUTO_SPEED_CAPABILITY IF_IOCTL_CMD_DEF(MID_IFNET, IF_L2ETHERNET_TYPE, 9) /* 获取指定端口的MTU */
#define IOCTL_ETHERNET_IS_LOOPBACK_SUPPORT       IF_IOCTL_CMD_DEF(MID_IFNET, IF_L2ETHERNET_TYPE, 10)/* 获取某一个端口是否支持端口回环测试 */
#define IOCTL_ETHERNET_LOOPBACK                  IF_IOCTL_CMD_DEF(MID_IFNET, IF_L2ETHERNET_TYPE, 11)/* 设置/取消指定端口的环回功能 */
#define IOCTL_ETHERNET_XGE_LAN_WAN_MODE          IF_IOCTL_CMD_DEF(MID_IFNET, IF_L2ETHERNET_TYPE, 12)/* 设置10GE接口工作模式为LAN还是WAN */
#define IOCTL_ETHERNET_GET_MAC_ADDRESS           IF_IOCTL_CMD_DEF(MID_IFNET, IF_L2ETHERNET_TYPE, 13)
#define IOCTL_ETHERNET_GET_PAD_CAPABILITY        IF_IOCTL_CMD_DEF(MID_IFNET, IF_L2ETHERNET_TYPE, 14)
#define IOCTL_ETHERNET_SET_FLOW_INTERVAL         IF_IOCTL_CMD_DEF(MID_IFNET, IF_L2ETHERNET_TYPE, 15)
#define IOCTL_ETHERNET_GET_STATISTICS            IF_IOCTL_CMD_DEF(MID_IFNET, IF_L2ETHERNET_TYPE, 16)
#define IOCTL_ETHERNET_GET_STATISTICS_ALL        IF_IOCTL_CMD_DEF(MID_IFNET, IF_L2ETHERNET_TYPE, 17)/* 获取指定端口的所有统计信息 */
#define IOCTL_ETHERNET_SET_JUMBOFRAME            IF_IOCTL_CMD_DEF(MID_IFNET, IF_L2ETHERNET_TYPE, 18)/* 设置二三层以太网口的JUMBOFRAME */
#define IOCTL_ETHERNET_GET_JUMBOFRAME_RANGE      IF_IOCTL_CMD_DEF(MID_IFNET, IF_L2ETHERNET_TYPE, 19)/* 获取二三层以太网中的JUMBOFRAME范围 */
#define IOCTL_ETHERNET_COMBO_SWITCH              IF_IOCTL_CMD_DEF(MID_IFNET, IF_L2ETHERNET_TYPE, 20)/* 设置combo口的工作模式 */
#define IOCTL_ETHERNET_SET_STORM_CONTROL         IF_IOCTL_CMD_DEF(MID_IFNET, IF_L2ETHERNET_TYPE, 21)/* 设置二层以太网口的风暴抑制值 */
#define IOCTL_ETHERNET_GET_STORM_CONTROL         IF_IOCTL_CMD_DEF(MID_IFNET, IF_L2ETHERNET_TYPE, 22)/* 获取二层以太网口的风暴抑制值 */
#define IOCTL_ETHERNET_PRIORITY_FLOW_CONTROL     IF_IOCTL_CMD_DEF(MID_IFNET, IF_L2ETHERNET_TYPE, 23)/* 设置端口流量优先级 */
#define IOCTL_ETHERNET_SET_PORTISOLATE           IF_IOCTL_CMD_DEF(MID_IFNET, IF_L2ETHERNET_TYPE, 24)
#define IOCTL_ETHERNET_SET_STATISTICS            IF_IOCTL_CMD_DEF(MID_IFNET, IF_L2ETHERNET_TYPE, 25)
#define IOCTL_ETHERNET_SET_STORM_BLOCK           IF_IOCTL_CMD_DEF(MID_IFNET, IF_L2ETHERNET_TYPE, 26)
#define IOCTL_ETHERNET_FLOW_CONTROL              IF_IOCTL_CMD_DEF(MID_IFNET, IF_L2ETHERNET_TYPE, 27)/* 设置端口flow-control工作模式 */
#define IOCTL_ETHERNET_SET_BPDUDROP              IF_IOCTL_CMD_DEF(MID_IFNET, IF_L2ETHERNET_TYPE, 28)
#define IOCTL_ETHERNET_VIRTUAL_CABLE_TEST        IF_IOCTL_CMD_DEF(MID_IFNET, IF_L2ETHERNET_TYPE, 29)/* VCT获取电缆信息*/
#define IOCTL_ETHERNET_IFEXTTRPS                 IF_IOCTL_CMD_DEF(MID_IFNET, IF_L2ETHERNET_TYPE, 30)/* 设置接口告警阈值 */
#define IOCTL_ETHERNET_GET_LANWANINFO            IF_IOCTL_CMD_DEF(MID_IFNET, IF_L2ETHERNET_TYPE, 31)/* 获取XGE口LAN/WAN信息 */
#define IOCTL_ETHERNET_GET_RMON_STATISTICS       IF_IOCTL_CMD_DEF(MID_IFNET, IF_L2ETHERNET_TYPE, 33)/* 获取RMON统计信息 */
#define IOCTL_ETHERNET_SET_PORT_CONNECTIONMODE   IF_IOCTL_CMD_DEF(MID_IFNET, IF_L2ETHERNET_TYPE, 34)/* 设置port connection-mode */
#define IOCTL_ETHERNET_GET_DEFAULT_MACADDRESS    IF_IOCTL_CMD_DEF(MID_IFNET, IF_L2ETHERNET_TYPE, 35)

/* 三层以太网PhyIOCtl命令字 */
#define IOCTL_ETHERNET_SET_PROMISCUOUS_MODE      IF_IOCTL_CMD_DEF(MID_IFNET, IF_L3ETHERNET_TYPE, 0) /* 设置以太网口为混杂工作模式/普通工作模式 */

/* FC接口PhyIOCtl命令字 */
#define IOCTL_FC_SET_SPEED                      IF_IOCTL_CMD_DEF(MID_IFNET, IF_FC_TYPE, 0)     /* 设置指定端口的速率*/
#define IOCTL_FC_GET_SPEED                      IF_IOCTL_CMD_DEF(MID_IFNET, IF_FC_TYPE, 4)     /* 获取指定端口的速率 */
#define IOCTL_FC_GET_DEFAULTSPEED               IF_IOCTL_CMD_DEF(MID_IFNET, IF_FC_TYPE, 7)     /* 获取指定端口的速率 */

/* 40G接口分裂PhyIOCtl命令字 */
#define IOCTL_FGE_USINGXGE                      IF_IOCTL_CMD_DEF(MID_IFNET, IF_L2FGE_TYPE, 0)  /* 使用四个10G口 */
#define IOCTL_FGE_USINGFGE                      IF_IOCTL_CMD_DEF(MID_IFNET, IF_L2FGE_TYPE, 1)  /* 使用一个40G口 */
#define IOCTL_FGE_USINGHGE                      IF_IOCTL_CMD_DEF(MID_IFNET, IF_L2FGE_TYPE, 2)  /* 使用一个100G口 */
#define IOCTL_FGE_USINGTGE                      IF_IOCTL_CMD_DEF(MID_IFNET, IF_L2FGE_TYPE, 3)  /* 使用两个20G口 */

/* 100G接口分裂PhyIOCtl命令字 */
#define IOCTL_HGE_USINGXGE                      IF_IOCTL_CMD_DEF(MID_IFNET, IF_L2HGE_TYPE, 0)  /* 使用十个10G口 */
#define IOCTL_HGE_USINGHGE                      IF_IOCTL_CMD_DEF(MID_IFNET, IF_L2HGE_TYPE, 1)  /* 使用一个100G口 */
#define IOCTL_HGE_USINGFGE                      IF_IOCTL_CMD_DEF(MID_IFNET, IF_L2HGE_TYPE, 2)  /* 使用一个40G口 */

/* 分裂、合并是否需要重启，默认需要重启 */
#define IF_USING_NEEDREBOOT     0
#define IF_USING_NOREBOOT       1

/* ATM 接口的驱动命令字 */
#define IOCTL_ATM_SET_CABLE                     IF_IOCTL_CMD_DEF(MID_IFNET, IF_ATM_TYPE, 0)
#define IOCTL_ATM_SET_CLOCK                     IF_IOCTL_CMD_DEF(MID_IFNET, IF_ATM_TYPE, 1)
#define IOCTL_ATM_SET_LINE_CODE                 IF_IOCTL_CMD_DEF(MID_IFNET, IF_ATM_TYPE, 2)
#define IOCTL_ATM_SET_FRAMING                   IF_IOCTL_CMD_DEF(MID_IFNET, IF_ATM_TYPE, 3)
#define IOCTL_ATM_SET_LOOPBACK                  IF_IOCTL_CMD_DEF(MID_IFNET, IF_ATM_TYPE, 4)
#define IOCTL_ATM_SET_SCRAMBLE                  IF_IOCTL_CMD_DEF(MID_IFNET, IF_ATM_TYPE, 5)
#define IOCTL_ATM_IMA_SET_MAX_DELAY             IF_IOCTL_CMD_DEF(MID_IFNET, IF_ATM_TYPE, 6)
#define IOCTL_ATM_IMA_SET_FRAME_LENGTH          IF_IOCTL_CMD_DEF(MID_IFNET, IF_ATM_TYPE, 7)
#define IOCTL_ATM_IMA_SET_CLOCK                 IF_IOCTL_CMD_DEF(MID_IFNET, IF_ATM_TYPE, 8)
#define IOCTL_ATM_IMA_SET_STANDARD              IF_IOCTL_CMD_DEF(MID_IFNET, IF_ATM_TYPE, 9)
#define IOCTL_ATM_IMA_SET_TEST                  IF_IOCTL_CMD_DEF(MID_IFNET, IF_ATM_TYPE, 10)
#define IOCTL_ATM_SET_FLAG_C2                   IF_IOCTL_CMD_DEF(MID_IFNET, IF_ATM_TYPE, 11)
#define IOCTL_ATM_SET_FLAG_J0                   IF_IOCTL_CMD_DEF(MID_IFNET, IF_ATM_TYPE, 12)
#define IOCTL_ATM_SET_FLAG_J1                   IF_IOCTL_CMD_DEF(MID_IFNET, IF_ATM_TYPE, 13)
#define IOCTL_ATM_DSL_SET_ACTIVATE              IF_IOCTL_CMD_DEF(MID_IFNET, IF_ATM_TYPE, 14)
#define IOCTL_ATM_DSL_SET_STANDARD              IF_IOCTL_CMD_DEF(MID_IFNET, IF_ATM_TYPE, 15)
#define IOCTL_ATM_DSL_SET_TX_ATTENUATION        IF_IOCTL_CMD_DEF(MID_IFNET, IF_ATM_TYPE, 16)
#define IOCTL_ATM_DSL_SHOW_CONFIG               IF_IOCTL_CMD_DEF(MID_IFNET, IF_ATM_TYPE, 17)
#define IOCTL_ATM_DSL_SHOW_STATUS               IF_IOCTL_CMD_DEF(MID_IFNET, IF_ATM_TYPE, 18)
#define IOCTL_ATM_DSL_SHOW_VERSION              IF_IOCTL_CMD_DEF(MID_IFNET, IF_ATM_TYPE, 19)
#define IOCTL_ATM_DSL_SET_ANNEX                 IF_IOCTL_CMD_DEF(MID_IFNET, IF_ATM_TYPE, 20)
#define IOCTL_ATM_DSL_SET_CAPABILITY            IF_IOCTL_CMD_DEF(MID_IFNET, IF_ATM_TYPE, 21)
#define IOCTL_ATM_DSL_SET_LINE_PROBING          IF_IOCTL_CMD_DEF(MID_IFNET, IF_ATM_TYPE, 22)
#define IOCTL_ATM_DSL_SET_MODE                  IF_IOCTL_CMD_DEF(MID_IFNET, IF_ATM_TYPE, 23)
#define IOCTL_ATM_DSL_SET_PAM                   IF_IOCTL_CMD_DEF(MID_IFNET, IF_ATM_TYPE, 24)
#define IOCTL_ATM_DSL_PBO                       IF_IOCTL_CMD_DEF(MID_IFNET, IF_ATM_TYPE, 25)
#define IOCTL_ATM_DSL_SET_PSD                   IF_IOCTL_CMD_DEF(MID_IFNET, IF_ATM_TYPE, 26)
#define IOCTL_ATM_DSL_SET_LINE_RATE             IF_IOCTL_CMD_DEF(MID_IFNET, IF_ATM_TYPE, 27)
#define IOCTL_ATM_DSL_SET_MARGIN                IF_IOCTL_CMD_DEF(MID_IFNET, IF_ATM_TYPE, 28)
#define IOCTL_ATM_DSL_SET_WIRE                  IF_IOCTL_CMD_DEF(MID_IFNET, IF_ATM_TYPE, 29)
#define IOCTL_ATM_DSL_GET_LINE_INFO             IF_IOCTL_CMD_DEF(MID_IFNET, IF_ATM_TYPE, 30)
#define IOCTL_ATM_DSL_GET_ATU_PHYINFO           IF_IOCTL_CMD_DEF(MID_IFNET, IF_ATM_TYPE, 31)
#define IOCTL_ATM_DSL_GET_ATU_CHANINFO          IF_IOCTL_CMD_DEF(MID_IFNET, IF_ATM_TYPE, 32)
#define IOCTL_ATM_IMA_SET_LINK_NUM              IF_IOCTL_CMD_DEF(MID_IFNET, IF_ATM_TYPE, 33)
#define IOCTL_ATM_ADDLINK_TO_IMA                IF_IOCTL_CMD_DEF(MID_IFNET, IF_ATM_TYPE, 34)
#define IOCTL_ATM_DELLINK_FROM_IMA              IF_IOCTL_CMD_DEF(MID_IFNET, IF_ATM_TYPE, 35)
#define IOCTL_SHDSL_SET_WIRE_WORKMODE           IF_IOCTL_CMD_DEF(MID_IFNET, IF_ATM_TYPE, 36)

typedef enum tagIF_BLADETYPE
{
    IF_BLADE_DEFAULT,              
    IF_BLADE_LOADGROUP,                 
    IF_BLADE_INVALID        
}IF_BLADETYPE_E;

/* BLADE 接口的驱动命令字  */
/* IOCTL_BLADE_GETTYPE pData参数为ULONG *，输出IF_BLADETYPE_E类型 */
#define IOCTL_BLADE_GETTYPE                     IF_IOCTL_CMD_DEF(MID_IFNET, IF_BLADE_TYPE, 0) 

/* 从物理层到链路层的上行事件 */
ISSU typedef enum enumIf_LinkUp
{
    IF_LINKUP_UP = 0,               /* 0: 端口UP */
    IF_LINKUP_DOWN,                 /* 1: 端口DOWN */
    IF_LINKUP_BAUD_CHANGED ,        /* 2: 端口波特率变化 */
    IF_LINKUP_ASY_TO_SYN ,          /* 3: 端口由异步模式变成同步模式 */
    IF_LINKUP_SYN_TO_ASY ,          /* 4: 端口由同步模式变成异步模式 */
    IF_LINKUP_SET_MAC ,             /* 5: 设置MAC地址，用于PPPoE等接口 */
    IF_LINKUP_SPEED_CHANGED,        /* 6: 端口速率变化 */
    IF_LINKUP_DUPLEX_CHANGED,       /* 7: 端口双工变化 */
    IF_LINKUP_ACTIVE,               /* 8: 链路被激活 */
    IF_LINKUP_DEACTIVE,             /* 9: 链路被去激活 */
    IF_LINKUP_ATM_OAMSTATE,         /* 10: ATM OAM状态上报 */
    IF_LINKUP_ATM_OAMPINGRECEVIED,  /* 11: OAM PING结果上报 */
    IF_LINKUP_ATM_EOA_MAPDOWN,      /* 12: ATM PVC映射关系失效, 目前只用于XoEoA */
    IF_LINKUP_E1VOICE,              /* 13: E1VI语音命令 */
    IF_LINKUP_4VIVOICE,             /* 14: E1VI语音命令 */
    IF_LINKUP_WORKPROTOCOLDOWN,     /* 15: L2VPN欺骗链路层模块物理DOWN */
    IF_LINKUP_WORKPROTOCOLUP,       /* 16: L2VPN恢复物理层UP */
    IF_LINKUP_CABLECHANGE,          /* 17: 同异步串口底层电缆接头发生改变(DTE/DCEC) */
    IF_LINKUP_IPSEC_RESET,          /* 18: IPsec加密卡接口内数据恢复为初始状态 */
    IF_LINKUP_SMOOTHUP,             /* 19: 物理层平滑UP */
    IF_LINKUP_RPR_MISCABLE_STATUS,  /* 20: RPR错误电缆状态 */
    IF_LINKUP_RPR_CONGEST_STATUS,   /* 21: RPR拥塞状态 */
    IF_LINKUP_RPR_PROTECT_CHANGE,   /* 22: RPR保护模式改变 */
    IF_LINKUP_FASTEV,               /* 23: 端口UP/DOWN快速上报 */
    IF_LINKUP_ACTIVE_FAILED,        /* 24: 链路激活失败 */
    IF_LINKUP_RPR_MATE_STATUS,      /* 25: MATE口状态变化 */
    IF_LINKUP_ENCAP_CHANGE,         /* 26: 物理层报文封装格式发生变化 */
    IF_LINKUP_AGG_JOIN,             /* 27: 端口加入聚合组 */
    IF_LINKUP_AGG_LEAVE,            /* 28: 端口退出聚合组 */
    IF_LINKUP_SERVICESLOT,          /* 29: service slot */
    IF_LINKUP_ACTIVE_IN_PROGRESS,   /* 30: BRI在激活过程中 */
    IF_LINKUP_BBCREDIT_CHANGED,     /* 31: 端口BBCREDIT变化 */
    IF_LINKUP_BITERROR_CHANGED,     /* 32: 端口BITERROR变化 */
    IF_LINKUP_VSAN_UP,              /* 33: FC接口VSAN内UP */
    IF_LINKUP_VSAN_DOWN,            /* 34: FC接口VSAN内DOWN */
    IF_LINKUP_SET_DEFAULT_MAC,      /* 35: 设置缺省MAC地址, 堆叠分裂时驱动上报平台*/
    IF_LINKUP_LINEPROTOCOL_UP,      /* 36: 链路层协议UP */
    IF_LINKUP_LINEPROTOCOL_DOWN,    /* 37: 链路层协议DOWN */
    IF_LINKUP_DAMP_UP,              /* 38: 链路DAMPUP */
    IF_LINKUP_DAMP_DOWN,            /* 39: 链路DAMPDOWN */
    IF_LINKUP_TUNNEL_IPSEC_UP,      /* 40: Tunnel链路层协议IPsec UP */
    IF_LINKUP_TUNNEL_IPSEC_DOWN,    /* 41: Tunnel链路层协议IPsec Down */ 
    IF_LINKUP_RADIO_IDLETIMEREACH,  /* 42: 空闲时间到达 */
    IF_LINKUP_RADIO_RADARDETECTED,  /* 43: 在当前工作信道检测到雷达 */
    IF_LINKUP_NETWORK_SWITCH,       /* 44: 2G/3G/4G网络切换 */
    IF_LINKUP_HANGUP,               /* 45: ISDN E1 PRI时隙挂断 */
    IF_LINKUP_CUPID_RESP,           /* 46: 无线定位测量结果上报 */
    IF_LINKUP_MAXCMD
}IF_LINKUP_E;

/* 判断底层是否支持对该项的统计,只有该项值置为IF_PORT_STATISTIC_NOT_SUPPORT时,才为不支持 */
#define IF_PORT_STATISTIC_NOT_SUPPORT ((UINT64)(-1))

ISSU typedef struct tagPeakSpeed
{
    UINT64    ui64SpeedPeakByte;
    DATE_S    stSpeedPeakDate;
    TIME_S    stSpeedPeakTime;
}IF_PEAKSPEED_S;

/* 以太网统计信息 */
ISSU typedef struct  tagIFLanPortPktStatics
{
    IF_PEAKSPEED_S stInSpeedPeak;       /* 输入峰值信息 */
    IF_PEAKSPEED_S stOutSpeedPeak;      /* 输出峰值信息 */
    UINT64   ui64InByte;                /* 输入报文字节数 */
    UINT64   ui64InPkt;                 /* 输入报文数 */
    UINT64   ui64InUcastPkt;            /* 输入单播包 */
    UINT64   ui64InBrdcastPkt;          /* 输入广播包 */
    UINT64   ui64InMuticastPkt;         /* 输入多播包 */
    UINT64   ui64InPause;               /* 输入pause 帧 */
    UINT64   ui64InNormalByte;          /* 输入所有报文字节数 */
    UINT64   ui64InNormalBrdcastByte;   /* 输入广播报文字节数 */
    UINT64   ui64InNormalMuticastByte;  /* 输入组播报文字节数 */
    UINT64   ui64InNormalUnicastByte;   /* 输入正常单播报文字节数 */
    UINT64   ui64InUnknownUnicastByte;  /* 输入未知单播报文字节数 */
    UINT64   ui64InNormalPkt;
    UINT64   ui64InNormalUcastPkt;      /* 输入正常单播包 */
    UINT64   ui64InUnknownUcastPkt;     /* 输入未知单播报文数 */
    UINT64   ui64InNormalBrdcastPkt;
    UINT64   ui64InNormalMuticastPkt;
    UINT64   ui64InNormalPause;         /* 输入正常pause 帧数 */                /* Modified by zhangzhang05015, 2009-01-17 of LSD34939 */
    UINT64   ui64InPktErr;
    UINT64   ui64InSpeedPkt;            /* 输入包速率 */
    UINT64   ui64InSpeedByte;
    UINT64   ui64InErrPkt;              /* 输入错误包 */
    UINT64   ui64InRunt;
    UINT64   ui64InGiant;
    UINT64   ui64InThrottle;
    UINT64   ui64InErrCRC;
    UINT64   ui64InFrame;
    UINT64   ui64InOverrun;
    UINT64   ui64InAbort;
    UINT64   ui64InIgnore;
    UINT64   ui64InParity;
    UINT64   ui64InSpeedBrdcastPkt;     /* 输入广播包速率 */
    UINT64   ui64InSpeedMulticastPkt;   /* 输入组播包速率 */
    UINT64   ui64OutByte;
    UINT64   ui64OutPkt;                /* 输出报文数 */
    UINT64   ui64OutUcastPkt;           /* 输出单播包 */
    UINT64   ui64OutBrdcastPkt;         /* 输出广播包 */
    UINT64   ui64OutMuticastPkt;        /* 输出组播包 */
    UINT64   ui64OutPause;
    UINT64   ui64OutNormalByte;         /* 输出所有报文字节数 */
    UINT64   ui64OutNormalBrdcastByte;  /* 输出广播报文字节数 */
    UINT64   ui64OutNormalMuticastByte; /* 输出组播报文字节数 */
    UINT64   ui64OutNormalUnicastByte;  /* 输出正常单播报文字节数 */
    UINT64   ui64OutUnknownUnicastByte; /* 输出未知单播报文字节数 */
    UINT64   ui64OutNormalPkt;
    UINT64   ui64OutNormalUcastPkt;     /* 输出正常单播包 */
    UINT64   ui64OutUnknownUcastPkt;    /* 输出未知单播报文数 */
    UINT64   ui64OutNormalBrdcastPkt;
    UINT64   ui64OutNormalMuticastPkt;
    UINT64   ui64OutNormalPause;
    UINT64   ui64OutPktErr;
    UINT64   ui64OutSpeedPkt;           /* 输出包速率 */
    UINT64   ui64OutSpeedByte;
    UINT64   ui64OutErrPkt;             /* 输出错误包 */
    UINT64   ui64OutUnderRun;
    UINT64   ui64OutBuffFail;
    UINT64   ui64OutAbort;
    UINT64   ui64OutDefer;
    UINT64   ui64OutCollision;
    UINT64   ui64OutLateCollision;
    UINT64   ui64OutLostCarrier;
    UINT64   ui64OutNoCarrier;
    UINT64   ui64OutSpeedBrdcastPkt;    /* 输出广播包速率 */
    UINT64   ui64OutSpeedMulticastPkt;  /* 输出组播包速率 */
}IF_LANPORT_PKT_STATICSS_S;

/* 用于向底层取得接口信息 */
typedef struct tagIF_INFO_S
{
    IF_INDEX ifIndex;                   /* 接口指针索引 */
    CHAR     *pcInfo;                   /* 信息缓冲区 */
    ULONG    ulSize;                    /* 缓冲区大小 */
    ULONG    ulChanNo;                  /* 通道编号，仅E3,T3,Cpos控制器口使用,
                                           非零表示指定通道，零表示所有通道 */
}IF_INFO_S;

/* 用于向驱动获取MTU的范围 */
ISSU typedef struct tagIFMtuRange
{
    UINT uiMinMtu; /* 驱动支持的最小值 */
    UINT uiMaxMtu; /* 驱动支持的最大值 */
}IF_MTU_RANGE_S;

/* 端口环回类型 */
#define PORT_NO_LOOPBACK                        0x1U   /* 取消环回组设置或环回检测，二者互斥 */
#define PORT_INTERNAL_LOOPBACK_DETECT           0x2U   /* 内环检测 */
#define PORT_EXTERNAL_LOOPBACK_DETECT           0x3U   /* 外环检测 */
#define PORT_SERVICE_LOOP                       0x4U   /* 环回组设置 */
#define PORT_INTERNAL_LOOPBACK_DETECT_MANUAL    0x5U   /* 进行内环测试。不能自动取消 */
#define PORT_EXTERNAL_LOOPBACK_DETECT_MANUAL    0x6U   /* 进行外环测试。不能自动取消 */
#define PORT_STOP_LOOPBACK                      0x7U   /* 中断当前测试 */

/* 端口环回测试状态 */
#define PORT_LOOPBACK_OK                        0x4U
#define PORT_LOOPBACK_FAIL                      0x5U
#define PORT_LOOPBACK_TESTING                   0x6U

/* 端口双绞线类型 IOCTL_ETHERNET_SET_MDIX */
#define PORT_MDIX_MODE                          0x1U
#define PORT_MDI_MODE                           0x2U
#define PORT_MDIX_AUTO                          0x3U
#define PORT_MDIX_NOTSUPPORT                    0x4U

/* 端口双工类型，IOCTL_ETHERNET_GET_DUPLEX */
#define PORT_DUPLEX_FULL                        0x1U
#define PORT_DUPLEX_HALF                        0x2U
#define PORT_DUPLEX_AUTO                        0x3U

/* 端口速率 */
#define PORT_SPEED_AUTO                         0U
#define PORT_SPEED_2M                           2U
#define PORT_SPEED_10M                          10U
#define PORT_SPEED_100M                         100U
#define PORT_SPEED_1G                           1000U
#define PORT_SPEED_2G                           2000U
#define PORT_SPEED_4G                           4000U
#define PORT_SPEED_8G                           8000U
#define PORT_SPEED_10G                          10000U
#define PORT_SPEED_16G                          16000U
#define PORT_SPEED_20G                          20000U
#define PORT_SPEED_2DOT5G                       2500U
#define PORT_SPEED_40G                          40000U
#define PORT_SPEED_100G                         100000U

/* 以太网端口速率自协商 掩码位 */
#define IF_ETH_AUTO_SPEED_MASK                  0xFFF00000U

/* 以太网自协商速率， 高16位每个BIT位代表一种速率
   实际设置的协商速率值可以是 几个速率相或的结果
*/
#define IF_ETH_AUTO_SPEED_10M                   0x00100000U
#define IF_ETH_AUTO_SPEED_100M                  0x00200000U
#define IF_ETH_AUTO_SPEED_1G                    0x00400000U

/* threshold set type */
#define IF_THRESHOLD_SET_SD    0x01               /* threshold sd 设置 */
#define IF_THRESHOLD_SET_SF    0x10               /* threshold sf 设置 */
#define IF_THRESHOLD_SET_BIND  (IF_THRESHOLD_SET_SD | IF_THRESHOLD_SET_SF)
                               /* threshold bind 设置 */

/* 设置threshold值对应驱动命令字 */
ISSU typedef struct tagIfThresholdCmd
{
    UINT uiSdValue;
    UINT uiSfValue;
    UINT uiType;
}IF_THRESHOLD_S;

/* alarm-detect actiontype */
#define IF_ALARM_ACTION_LINKDOWN 0x10000000
#define IF_ALARM_ACTION_DEFAULT 0 /* UNDO动作项*/

/* alarm-detect alarmtype */
#define IF_ALARM_SD 0x10000000
#define IF_ALARM_SF 0x20000000
#define IF_ALARM_RDI 0x40000000

/* 设置alarmdetect值对应驱动命令字 */
ISSU typedef struct tagIfAlarmDetect
{
    UINT uiAlarmType;
    UINT uiActionType;
}IF_ALARMDETECT_S;

/* posmib trap 类型 */
#define IF_SONET_TRAPTYPE_B1          1
#define IF_SONET_TRAPTYPE_B2          2
#define IF_SONET_TRAPTYPE_B3          3

/* posmib获取B1B2B3计数，命令字携带参数数据结构 */
typedef struct tagIFSONETMIB 
{
    UINT64 uiSectionCVs;
    UINT64 uiLineCVs;
    UINT64 uiPathCVs;
}IF_SONET_MIB_S;

#define IF_CODINGVIOLATION_THRESHOLD_DEFAULT 6
/* 驱动注册系统接口规格 */
typedef struct tagIF_SYSTEMINFO_S
{
    USHORT usMaxSubSlotPerSlot; /* 每槽位最大子卡数量 */
    USHORT usMaxIfPerSubSlot;   /* 每子卡最大接口数量 */
    USHORT usMaxSlotNumber;     /* 系统最大槽位数量 */
    USHORT usMaxIfPerSlot;      /* 每板最大物理口数量 */
    UCHAR  ucIfDimension;    /*物理接口名称中"/"的个数*/
}IF_SYSTEMINFO_S,*PIF_SYSTEMINFO_S;

/* 报文发送优先级 */
#define IF_PRI_CRITICAL     7UL
#define IF_PRI_HIGH         5UL
#define IF_PRI_MIDDLE       3UL
#define IF_PRI_LOW          0UL

/* BEGIN: 以太网口分裂模式预读 */
typedef enum enIF_ETH_Mode
{
    IF_ETH_MODE_XGE = 1,    /* 10G  */
    IF_ETH_MODE_FGE,        /* 40G  */
    IF_ETH_MODE_HGE,        /* 100G */
    IF_ETH_MODE_FC,         /* FC */
    IF_ETH_MODE_BUTT
}IF_ETH_MODE_E;

typedef struct tagIF_Eth_Mode_Info
{
    IF_ETH_MODE_E enMode;                   /* 接口使用模式 IF_FGE_USEMODE_EN */
    CHAR szIfName[IF_MAX_NAME_LEN + 1];     /* 接口名*/
}IF_ETH_MODE_INFO_S;

IF_ETH_MODE_INFO_S *IF_GetEthModeFromCfg(OUT ULONG *pulNum);
/* END: 以太网口分裂模式预读 */

/* BEGIN: MIB相关宏定义及数据接口 */

/* MIB信息结构 */
typedef struct  tagIFMIB
{
    ULONG ulTableAddr;                          /* 表结构地址,分别存放各个表结构的首地址 */
    USHORT usTableType;                         /* MIB表类型 */
}IF_MIB_S;

typedef struct tagIFMIBPORTTABLE
{
    UINT uiPortType;        /* 请参考mib节点hh3cLswPortType的属性枚举值 */
    BOOL_T bIsPlugged;
}IF_MIB_PORTTABLE_S;

/* MIB表类型*/
#define IFTABLE             1
#define IFXTABLE            2
#define IFRCVADDRTABLE      3
#define IFEXTTABLE          4                   /* 接口扩展表类型，结构见IFEXTTABLEMIB_S定义 */

/* ifTable中usCountSpec描述对不支持的节点宏定义 */
#define IF_MIB_INOCTETS_NOSUPPORT           ((USHORT)(0x01 << 0))
#define IF_MIB_INUCASTPKTS_NOSUPPORT        ((USHORT)(0x01 << 1))
#define IF_MIB_INNUCASTPKTS_NOSUPPORT       ((USHORT)(0x01 << 2))
#define IF_MIB_INDISCARDS_NOSUPPORT         ((USHORT)(0x01 << 3))
#define IF_MIB_INERRORS_NOSUPPORT           ((USHORT)(0x01 << 4))
#define IF_MIB_INUNKNOWNPROTOS_NOSUPPORT    ((USHORT)(0x01 << 5))
#define IF_MIB_OUTOCTETS_NOSUPPORT          ((USHORT)(0x01 << 6))
#define IF_MIB_OUTUCASTPKTS_NOSUPPORT       ((USHORT)(0x01 << 7))
#define IF_MIB_OUTNUCASTPKTS_NOSUPPORT      ((USHORT)(0x01 << 8))
#define IF_MIB_OUTDISCARDS_NOSUPPORT        ((USHORT)(0x01 << 9))
#define IF_MIB_OUTERRORS_NOSUPPORT          ((USHORT)(0x01 << 10))

/* ifTable变量结构 */
ISSU typedef struct  tagIFTABLEMIB
{
    UINT uiLastChange;                          /* 接口的最后一次修改时的系统启动以来时间 */
    UINT uiMtu;                                 /* 接口的MTU值 */
    UINT uiSpeed;                               /* 接口当前带宽 */
    UINT uiInOctets;                            /* 入站包字节数 */
    UINT uiInUcastPkts;                         /* 入站的单播包数 */
    UINT uiInNUcastPkts;                        /* 入站的非单播包数 */
    UINT uiInDiscards;                          /* 丢弃的入站包数 */
    UINT uiInErrors;                            /* 有错误的入站包数 */
    UINT uiInUnknownProtos;                     /* 不支持的协议的入站包数 */
    UINT uiOutOctets;                           /* 出站包字节数 */
    UINT uiOutUcastPkts;                        /* 出站的单播包数 */
    UINT uiOutNUcastPkts;                       /* 出站的单播包数 */
    UINT uiOutDiscards;                         /* 丢弃的出站包数 */
    UINT uiOutErrors;                           /* 有错误的出站包数 */
    UINT uiOutQLen;                             /* 出包队列长度 */
    USHORT usOperStatus;                        /* 接口的当前运行状态 */
    USHORT usCountSpec;                         /* 对uiInOctets到uiOutErrors统计，不支持的统计置位项
                                                   若不支持，分别低位开始置位
                                                   如不支持uiInNUcastPkts及uiInUnknownProtos，
                                                   值为 0000 0000 0010 0100 */
    UCHAR aucPhysAddress[MAC_ADDR_LEN];         /* 接口在协议子层的物理地址 */
    UCHAR aucReserve[2];
}IFTABLEMIB_S;

/* ifXTable中uiCountSpec描述对不支持的节点宏定义 */
#define IF_MIB_HCINOCTETS_NOSUPPORT           ((UINT)(0x01 << 0))
#define IF_MIB_HCINUCASTPKTS_NOSUPPORT        ((UINT)(0x01 << 1))
#define IF_MIB_HCINMULTICASTPKTS_NOSUPPORT    ((UINT)(0x01 << 2))
#define IF_MIB_HCINBROADCASTPKTS_NOSUPPORT    ((UINT)(0x01 << 3))
#define IF_MIB_HCOUTOCTETS_NOSUPPORT          ((UINT)(0x01 << 4))
#define IF_MIB_HCOUTUCASTPKTS_NOSUPPORT       ((UINT)(0x01 << 5))
#define IF_MIB_HCOUTMULTICASTPKTS_NOSUPPORT   ((UINT)(0x01 << 6))
#define IF_MIB_HCOUTBROADCASTPKTS_NOSUPPORT   ((UINT)(0x01 << 7))
#define IF_MIB_INMULTICASTPKTS_NOSUPPORT      ((UINT)(0x01 << 8))
#define IF_MIB_INBROADCASTPKTS_NOSUPPORT      ((UINT)(0x01 << 9))
#define IF_MIB_OUTMULTICASTPKTS_NOSUPPORT     ((UINT)(0x01 << 10))
#define IF_MIB_OUTBROADCASTPKTS_NOSUPPORT     ((UINT)(0x01 << 11))

/* ifXTable变量结构 */
ISSU typedef struct  tagIFXTABLEMIB
{
    UINT64 uiHCInOctets;                        /* 入站包字节数(64) */
    UINT64 uiHCInUcastPkts;                     /* 入站单播包数(64) */
    UINT64 uiHCInMulticastPkts;                 /* 入站多播包数(64) */
    UINT64 uiHCInBroadcastPkts;                 /* 入站广播包数(64) */
    UINT64 uiHCOutOctets;                       /* 出站包字节数(64) */
    UINT64 uiHCOutUcastPkts;                    /* 出站单播包数(64) */
    UINT64 uiHCOutMulticastPkts;                /* 出站多播包数(64) */
    UINT64 uiHCOutBroadcastPkts;                /* 出站广播包数(64) */
    UINT uiInMulticastPkts;                     /* 入站的多播包数 */
    UINT uiInBroadcastPkts;                     /* 入站的广播包数 */
    UINT uiOutMulticastPkts;                    /* 出站的多播包数 */
    UINT uiOutBroadcastPkts;                    /* 出站的广播包数 */
    UINT uiHighSpeed;                           /* 接口的当前带宽，单位是1,000,000 bits/秒 */
    UINT uiCounterDiscontinuityTime;
    USHORT usPromiscuousMode;                   /* 混杂模式设置状态 */
    USHORT usConnectorPresent;                  /* 接口下层是否有一个物理链接器 */
    UINT uiCountSpec;                           /* 对uiHCInOctets到uiOutBroadcastPkts统计，不支持的统计置位项
                                                   若不支持，分别低位开始置位
                                                   如不支持uiHCInMulticastPkts及uiInMulticastPkts，
                                                   值为 0000 0001 0000 0100 */
}IFXTABLEMIB_S;

/* ifRcvAddressTable变量结构 */
ISSU typedef struct  tagIFRCVADDRTABLEMIB
{
    USHORT usMACAddrType;                       /* 类型定义见下面的宏定义*/
    USHORT usRcvAddressStauts;                  /* 该地址的状态 */
    USHORT usRcvAddressType;                    /* 接收地址的类型属性 */
    UCHAR aucRcvAddressAddress[MAC_ADDR_LEN];   /* 接口在协议子层的物理地址 */
}IFRCVADDRTABLEMIB_S;

/* IFRCVADDRTABLEMIB_S的成员usMACAddrType的定义 */
#define IFRCVTBL_GET_FIRST_ADDR 1               /* 获取第一个地址的信息 */
#define IFRCVTBL_GET_CUR_ADDR   2               /* 获取当前指定地址的信息，指定地址存在szRcvAddressAddress中 */
#define IFRCVTBL_GET_NEXT_ADDR  3               /* 获取下一个地址的信息，前一个地址存放在szRcvAddressAddress中 */

#define IF_SET_RCVADDR_CREATE   1               /* 创建RCV ADDR  */
#define IF_SET_RCVADDR_DELETE   2               /* 删除RCV ADDR  */
#define IF_SET_RCVADDR_MODIFY   3               /* 修改RCV ADDR  */
                                                
#define IF_RCVADDR_OTHER        1               /* 不关心地址是否改变 */
#define IF_RCVADDR_VOLATILE     2               /* 地址可被改变，若值改变后未被保存，重启系统后改变后的值不存在。 */
#define IF_RCVADDR_NONVOLATILE  3               /* 地址不可被改变 */

/* ifRcvAddressTable表set的结构 */
ISSU typedef struct  tagSETRCVADDR_S
{
    UCHAR ucOperType;                           /* SET的操作类型 */
    UCHAR ucRcvAddrType;                        /* 接收地址的类型属性 */
    UCHAR aucRcvAddr [MAC_ADDR_LEN];            /* 接口的物理地址 */
}IF_SET_RCVADDR_S;

/* IFEXTTABLE: hh3cIfTable表，驱动相关的数据结构 */
typedef struct tagIFEXTTABLEMIB
{
    UINT64 ui64HCInNUcastPkts;                  /* 接口接收的非单播报文数 */
    UINT64 ui64HCOutNUcastPkts;                 /* 接口发送的非单播报文数 */
}IFEXTTABLEMIB_S;

/* hh3cIfStatTable表，驱动获取相关的数据结构 */
ISSU typedef struct tagIfFlowStatTable
{
    UINT64 ui64IfStatFlowInBits;                /* 接收的比特数 */
    UINT64 ui64IfStatFlowOutBits;               /* 发送的比特数 */
    UINT64 ui64IfStatFlowInPkts;                /* 接收的包数   */
    UINT64 ui64IfStatFlowOutPkts;               /* 发送的包数   */
    UINT64 ui64IfStatFlowInBytes;               /* 接收的字节数 */
    UINT64 ui64IfStatFlowOutBytes;              /* 发送的字节数 */
}IF_FLOW_STAT_TABLE_S;

/* hh3cPortProtocolStatTable表，IPv4报文统计的数据结构 */
typedef struct tagIF_IPV4_STATISTIC
{
    UINT64 uiIPv4InOctets;
    UINT64 uiIPv4InUcastPkts;
    UINT64 uiIPv4InMultiPkts;
    UINT64 uiIPv4InBroadcastPkts;
    UINT64 uiIPv4InDiscards;
    UINT64 uiIPv4InErrors;
    UINT64 uiIPv4OutOctets;
    UINT64 uiIPv4OutUcastPkts;
    UINT64 uiIPv4OutMultiPkts;
    UINT64 uiIPv4OutBroadcastPkts;
    UINT64 uiIPv4OutDiscards;
    UINT64 uiIPv4OutErrors;
}IF_IPV4_STATISTIC_S;

/* hh3cPortProtocolStatTable表，IPv6报文统计的数据结构 */
typedef struct tagIF_IPV6_STATISTIC
{
    UINT64 uiIPv6InOctets;
    UINT64 uiIPv6InUcastPkts;
    UINT64 uiIPv6InMultiPkts;
    UINT64 uiIPv6InAnycastPkts;
    UINT64 uiIPv6InDiscards;
    UINT64 uiIPv6InErrors;
    UINT64 uiIPv6OutOctets;
    UINT64 uiIPv6OutUcastPkts;
    UINT64 uiIPv6OutMultiPkts;
    UINT64 uiIPv6OutAnycastPkts;
    UINT64 uiIPv6OutDiscards;
    UINT64 uiIPv6OutErrors;
}IF_IPV6_STATISTIC_S;

typedef struct tagIF_PROTOCOL_STATISTIC
{
    IF_IPV4_STATISTIC_S stIpv4Stat;
    IF_IPV6_STATISTIC_S stIpv6Stat;
}IF_PROTOCOL_STATISTIC_S;

/* hh3cifXXTable表，驱动获取相关的数据结构 */
typedef struct tagIfPayLoadStatics
{
    UINT64 ui64InByte;                          /* 接收的有效载荷字节数 */
    UINT64 ui64OutByte;                         /* 发送的有效载荷字节数 */
}IF_PAYLOAD_STATICSS_S;

/* RMON 统计项相关宏定义，考虑产品移植性, 取值与V5一致 */
/* rfc2819:  端口上产生丢弃报文的事件数(该事件数也可以是被丢弃的报文数) */
#define PORT_RMON_STAT_DROP                           100
/* rfc2819:端口从网络上接收到的总字节数(要包括损坏的数据包以及校验字节，但不包括分割位) */
#define PORT_RMON_STAT_BYTES                          101
/* rfc2819:端口接收到的包的总数(要包括损坏的数据包、广播包以及组播包) */
#define PORT_RMON_STAT_PKTS                           102
/* rfc2819:端口接收到完好广播包的数量（不包括组播包） */
#define PORT_RMON_STAT_BROAD_PKS                      103
/* rfc2819:端口接收到完好组播包的数量（不包括广播包）*/
#define PORT_RMON_STAT_MULTI_PKTS                     104
/* rfc2819:端口收到的报文介于[64, 1518]并且含有校验错误包的数量*/
#define PORT_RMON_STAT_CRC_ALIGN                      105
/* rfc2819: 端口收到报文太小（小于64）的包数*/
#define PORT_RMON_STAT_UNDER_SZ                       106
/* rfc2819: 端口收到完好且大于1518的包数*/
#define PORT_RMON_STAT_OVER_SZ                        107
/* rfc2819:端口接收到带bad FCS、FCS Error或Alignment Error的长度小于64个字节（不包括分割位，但保含FCS）的数据包数*/
#define PORT_RMON_STAT_FRAGMENTS                      108
/* rfc2819:端口接收到带bad FCS、FCS Error或Alignment Error的大于1518个字节（不包括分割位，但保含FCS）的数据包数*/
#define PORT_RMON_STAT_JABBERS                        109
/* rfc2819:端口检测出的冲突数 */
#define PORT_RMON_STAT_COLLISIONS                     110
/* rfc2819: 以下为端口接收到各长度（不包括分割位，但包括校验位）范围内的数据包数（含损坏的包）的6个统计项宏  */
#define PORT_RMON_STAT_PKT64                          111
#define PORT_RMON_STAT_PKT65TO127                     112
#define PORT_RMON_STAT_PKT128TO255                    113
#define PORT_RMON_STAT_PKT256TO511                    114
#define PORT_RMON_STAT_PKT512TO1023                   115
#define PORT_RMON_STAT_PKT1024TO1518                  116
#define PORT_DOT1D_PORT_CAPABILITIES                  200   /* no rfc */

typedef struct tagIfGetEthernetStatistics
{
    UINT   uiItem;                              /* 获取类型，参见统计项的宏定义 */
    UINT64 ui64Data;                            /* 对应获取的统计信息 */
}IF_GET_ETHERNET_STATISTICS_S;

typedef enum en_IF_IFEXTTRAPTYPE
{
    IF_BANDWIDTHUSAGEHIGH = 1,                  /* h3cIfBandwidthUsageHigh */
    IF_DISCARDPKTRATEHIGH                       /* h3cIfDiscardPktRateHigh */
}IF_IFEXTTRAPTYPE_E;

typedef struct tagIF_IFEXTTRAPS
{
    ULONG ulIfTrapsType;                        /* 告警类型取值IF_IFEXTTRAPTYPE_E */
    ULONG ulUpperLimit;                         /* 告警上限 */
}IF_IFEXTTRAPS_S;

/* RMON MIB表类型  */
#define RMON_STAT_TYPE 1                        /* etherStatsTable类型 */
#define RMON_HIST_TYPE 2                        /* etherHistoryTable类型 */

typedef struct tagIfGetRmonStatistics
{
    USHORT usTableType;                         /* RMON MIB表类型 */
    ULONG  ulTableAddr;                         /* 表结构地址，存放表结构的首地址 */
}IF_GET_RMON_STATISTICS_S;

/* rmon mib数据统计结构 */
ISSU typedef struct tagIFRMONMIB
{
    UINT64 ui64DropEvents;              /* 接口上产生的丢弃报文的事件数或被丢弃的报文数 */
    UINT64 ui64Octets;                  /* 接口接收的字节数 */
    UINT64 ui64Pkts;                    /* 接口接收的报文包数 */
    UINT64 ui64BroadcastPkts;           /* 接口接收的完好广播报文（不包括多播报文）包数 */
    UINT64 ui64MulticastPkts;           /* 接口接收的完好多播报文（不包括广播报文）包数 */
    UINT64 ui64CRCAlignErrors;          /* 接口接收的介于[64, 1518]字节且包含FCS Error或Alignment Error的报文包数 */
    UINT64 ui64UndersizePkts;           /* 接口接收的完好且小于64字节的报文包数 */
    UINT64 ui64OversizePkts;            /* 接口接收的完好且大于1518字节的报文包数 */
    UINT64 ui64Fragments;               /* 接口接收的小于64字节且包含FCS Error或Alignment Error的报文包数 */
    UINT64 ui64Jabbers;                 /* 接口接收的大于1518字节且包含FCS Error或Alignment Error的报文包数 */
    UINT64 ui64Collisions;              /* 接口检测出的冲突数 */
    UINT64 ui64Pkts64Octets;            /* 接口接收的64字节的报文（含损坏的）包数 */
    UINT64 ui64Pkts65to127Octets;       /* 接口接收的介于[65, 127]字节的报文（含损坏的）包数 */
    UINT64 ui64Pkts128to255Octets;      /* 接口接收的介于[128, 255]字节的报文（含损坏的）包数 */
    UINT64 ui64Pkts256to511Octets;      /* 接口接收的介于[256, 511]字节的报文（含损坏的）包数 */
    UINT64 ui64Pkts512to1023Octets;     /* 接口接收的介于[512, 1023]字节的报文（含损坏的）包数 */
    UINT64 ui64Pkts1024to1518Octets;    /* 接口接收的介于[1024, 1518]字节的报文（含损坏的）包数 */
}IF_RMON_MIB_S;

/* END: MIB相关宏定义及数据接口 */

/* 接口统计结构 */
ISSU typedef struct tagIFSTATISTIC
{
    /* 64 bits statics */
    UINT64 uiHCInUcastPkts;                     /* packets received via unicast */
    UINT64 uiHCInMulticastPkts;                 /* packets received via multicast */
    UINT64 uiHCInBroadcastPkts;                 /* packets received via boardcast */
    UINT64 uiHCOutUcastPkts;                    /* packets sent via unicast */
    UINT64 uiHCOutMulticastPkts;                /* packets sent via multicast */
    UINT64 uiHCOutBroadcastPkts;                /* packets sent via Broadcast */
    UINT64 uiHCInOctets;                        /* total number of octets received */
    UINT64 uiHCOutOctets;                       /* total number of octets sent */
    /* input statics */                         
    UINT uiInPackets;                           /* packets received on interface */
    UINT uiInUcastPkts;                         /* packets received via unicast */
    UINT uiInNUcastPkts;                        /* packets received via non-unicast */
    UINT uiInMultiPkts;                         /* packets received via multicast */
    UINT uiInBroadPkts;                         /* packets received via Broadcast */
    UINT uiInErrors;                            /* input errors on interface */
    UINT uiInDiscards;                          /* input discards on interface */
    UINT uiInUnknownProtos;                     /* input UnknownProtos on interface */
    /* Output statics */                        
    UINT uiOutPackets;                          /* packets sent on interface */
    UINT uiOutUcastPkts;                        /* packets sent via unicast */
    UINT uiOutNUcastPkts;                       /* packets sent via non-unicast */
    UINT uiOutMultiPkts;                        /* packets sent via multicast */
    UINT uiOutBroadPkts;                        /* packets sent via Broadcast */
    UINT uiOutErrors;                           /* output errors on interface */
    UINT uiOutDiscards;                         /* output discards on interface */
    UINT uiOutUnknownProtos;                    /* output UnknownProtos on interface */
    /* common statics */                        
    UINT uiCollisions;                          /* collisions on csma interfaces */
    UINT uiInOctets;                            /* total number of octets received */
    UINT uiOutOctets;                           /* total number of octets sent */
}IFSTATISTIC_S;


/* BEGIN: IF小特性相关*/

ISSU typedef enum enIfGlobalCmd
{
    IF_GLOBAL_SET_CUT_THROUGH,        /* 0: 设置cut through模式 */
    IF_GLOBAL_SET_BURST_MODE,         /* 1: 设置burst模式 */
    IF_GLOBAL_GET_PACKETDROP_SUMMARY, /* 2: 获取全局packet信息 */
    IF_GLOBAL_SET_FLOW_INTERVAL,      /* 3: 设置全局flow interval */
    IF_GLOBAL_ADD_RESERVE_VLAN_IF,    /* 4: 保留vlan虚接口 */
    IF_GLOBAL_DELETE_RESERVE_VLAN_IF, /* 5: 删除保留vlan虚接口 */
    IF_GLOBAL_ADD_ISOLATE_GROUP,      /* 6: 创建隔离组 Add the isolate group.*/
    IF_GLOBAL_DELETE_ISOLATE_GROUP,   /* 7: 删除隔离组 Delete the isolate group.*/
    IF_GLOBAL_MDC_ALLOC_INTF_CHECK,   /* 8: 驱动对ALLOC分口进行检测*/
    IF_GLOBAL_SET_USING_FX,           /* 9: 设置端口分裂 FGE TO XGE*/
    IF_GLOBAL_SET_USING_FF,           /* 10: 设置端口分裂 恢复FGE*/
    IF_GLOBAL_SET_USING_FH,           /* 11: 设置端口分裂 FGE TO HGE*/
    IF_GLOBAL_SET_USING_HX,           /* 12: 设置端口分裂 HGE TO XGE*/
    IF_GLOBAL_SET_USING_HF,           /* 13: 设置端口分裂 HGE TO FGE*/
    IF_GLOBAL_SET_USING_HH,           /* 14: 设置端口分裂 恢复HGE*/
    IF_GLOBAL_SET_PORT_TYPESWITCH,    /* 15: 设置端口类型切换 */
    IF_GLOBAL_SET_MAX = 1000,
}IF_GLOBAL_CMD_E;

/* IOCTL_ETHERNET_PRIORITY_FLOW_CONTROL */
#define IF_PRIORITY_FLOW_CONTROL_FUNCTION       1UL     /* 表示设置PFC功能 */
#define IF_PRIORITY_FLOW_CONTROL_NODROP_DOT1P   2UL     /* 表示设置PFC按802.1p的不丢弃状态 */
#define IF_PRIORITY_FLOW_CONTROL_ENABLE         1       /* 开启PFC功能 */
#define IF_PRIORITY_FLOW_CONTROL_DISABLE        2       /* 关闭PFC功能，缺省配置。 */
#define IF_PRIORITY_FLOW_CONTROL_AUTO           3       /* 开启PFC自协商功能 */
                                                        
/* IF_COMMON_CFG_FLOWCONTROL */                         
#define IF_FLOW_CONTROL_ENABLE_RECIEVE          3       /* flow-control receive enable, ENABLE = 1, DISABLE = 2*/
                                                        
/* IOCTL_ETHERNET_SET_PORTISOLATE */                    
#define IF_PISO_DISABLE                         0       /* 接口退出隔离组 */
#define IF_PISO_ENABLE                          1       /* 接口加入默认隔离组，其他组号不再定义宏 */

/* IOCTL_COMMON_LINKDELAY: Link delay接口phyioctl设置时高位bit宏定义 */
#define IF_LINKDELAY_UNDO                       0x0           /* 端口关闭LINKDELAY模式 */
#define IF_LINKDELAY_DOWN                       0x10000000    /* 端口DOWN状态抑制模式，单位为秒 */
#define IF_LINKDELAY_UP                         0x20000000    /* 端口UP状态抑制模式，单位为秒 */
#define IF_LINKDELAY_UPDOWN                     0x30000000    /* 端口UPDOWN状态抑制模式，单位为秒 */
#define IF_LINKDELAY_MSEC                       0x40000000    /* 端口DOWN状态抑制模式，单位为毫秒（命令中有关键字msec） */
#define IF_LINKDELAY_MSEC_UP                    0x50000000    /* 端口UP状态抑制模式，单位为毫秒（命令中有关键字msec） */
#define IF_LINKDELAY_MSEC_UPDOWN                0x60000000    /* 端口UPDOWN状态抑制模式，单位为毫秒（命令中有关键字msec） */

#define IF_LINKDELAY_UNDO_UP                    0x70000000    /* 端口关闭LINKDELAY UP模式 */
#define IF_LINKDELAY_UNDO_DOWN                  0x80000000    /* 端口关闭LINKDELAY DOWN模式 */
#define IF_LINKDELAY_UNDO_UPDOWN                0x90000000    /* 端口关闭LINKDELAY UPDOWN模式 */
#define IF_LINKDELAY_UNDO_MSEC_UP               0xa0000000    /* 端口关闭LINKDELAY MSEC UP模式 */
#define IF_LINKDELAY_UNDO_MSEC_DOWN             0xb0000000    /* 端口关闭LINKDELAY MSEC DOWN模式 */
#define IF_LINKDELAY_UNDO_MSEC_UPDOWN           0xc0000000    /* 端口关闭LINKDELAY MSEC UPDOWN模式 */

/* IOCTL_ETHERNET_SET_STORM_BLOCK */
#define IF_ETHERNET_UNDO_STORM_BLOCK            0x00    /* 取消当前二层以太网接口的流量block */
#define IF_ETHERNET_DO_STORM_BLOCK              0x01    /* 使能当前二层以太网接口流量block */
#define IF_ETHERNET_STORM_BLOCK_BROADCAST       0x01    /* block广播报文 */
#define IF_ETHERNET_STORM_BLOCK_MULTICAST       0x02    /* block组播报文 */
#define IF_ETHERNET_STORM_BLOCK_UNICAST         0x04    /* block单播报文 */
typedef struct tagIFEthStormBlock
{
    ULONG ulBlockAction;    /* 设置抑制or取消抑制，参见以上宏定义 */
    ULONG ulBlockType;      /* 抑制的报文类型，参见以上宏定义  */
}IF_ETHERNET_STORM_BLOCK_S;

/* 用于 IOCTL_ETHERNET_SET_STATISTICS 命令字 */
#define IF_IN_NORMAL_UNICAST_BYTES              1       /* 入方向正常的单播报文字节数 */
#define IF_IN_NORMAL_BRDCAST_BYTES              2       /* 入方向正常的广播报文字节数 */
#define IF_IN_NORMAL_MUTICAST_BYTES             3       /* 入方向正常的组播报文字节数 */
#define IF_IN_NORMAL_UNICAST_PACKETS            4       /* 入方向正常的单播报文包数 */
#define IF_IN_NORMAL_BRDCAST_PACKETS            5       /* 入方向正常的广播报文包数 */
#define IF_IN_NORMAL_MUTICAST_PACKETS           6       /* 入方向正常的组播报文包数 */

/* 当广播抑制PPS、KBPS方式支持时定义各种类型端口的最大抑制值 */
#define PORT_SUPPRESSION_PPS_VALUE_10M          14881
#define PORT_SUPPRESSION_BPS_VALUE_10M          10000
                                                
#define PORT_SUPPRESSION_PPS_VALUE_100M         148810
#define PORT_SUPPRESSION_BPS_VALUE_100M         100000
                                                
#define PORT_SUPPRESSION_PPS_VALUE_GE           1488100
#define PORT_SUPPRESSION_BPS_VALUE_GE           1000000
                                                
#define PORT_SUPPRESSION_PPS_VALUE_10GE         14881000
#define PORT_SUPPRESSION_BPS_VALUE_10GE         10000000

#define PORT_SUPPRESSION_PPS_VALUE_20GE         29762000
#define PORT_SUPPRESSION_BPS_VALUE_20GE         20000000
                                                
#define PORT_SUPPRESSION_PPS_VALUE_40GE         59524000
#define PORT_SUPPRESSION_BPS_VALUE_40GE         40000000
                                                
#define PORT_SUPPRESSION_PPS_VALUE_100GE        148810000
#define PORT_SUPPRESSION_BPS_VALUE_100GE        100000000

/* IOCTL_ETHERNET_SET_STORM_CONTROL */
#define IF_SET_PORT_BROADCAST_SUPPRESSION       1
#define IF_SET_PORT_MULTICAST_SUPPRESSION       2
#define IF_SET_PORT_UNICAST_SUPPRESSION         3
#define IF_SET_PORT_BROADCAST_SUPPRESSION_PPS   4
#define IF_SET_PORT_MULTICAST_SUPPRESSION_PPS   5
#define IF_SET_PORT_UNICAST_SUPPRESSION_PPS     6
#define IF_SET_PORT_BROADCAST_SUPPRESSION_BPS   7
#define IF_SET_PORT_MULTICAST_SUPPRESSION_BPS   8
#define IF_SET_PORT_UNICAST_SUPPRESSION_BPS     9

#define IF_XGE_LANWAN_INFO                      2048

typedef struct tagIfStormControl
{
    ULONG ulType;   /* 见以上9种宏定义*/
    ULONG ulValue;  /* Ratio是0 ~ 100，pps、BPS时为实际值 */
}IF_STORM_CONTROL_S;

/* END: IF小特性 */

/* 板卡接口热插拔状态定义，上报板、子卡、接口状态改变使用 */
ISSU typedef enum tagIF_HotState
{
    IF_HOT_STATE_INVALID,
    IF_NODE_STATE_ABSENT,           /* 节点拔出 */
    IF_NODE_STATE_PRESENT,          /* 节点插入:目前只有光板情况 */
    IF_NODE_STATE_DELETE,           /* 节点删除 */
    IF_NODE_STATE_SWITCH,           /* 主备倒换 */
    IF_SUBSLOT_STATE_ABSENT,        /* 子卡拔出 */
    IF_SUBSLOT_STATE_PRESENT,       /* 子卡插入 */
    IF_INTF_STATE_ABSENT,           /* 接口拔出 */
    IF_INTF_STATE_PRESENT,          /* 接口插入 */
    IF_NODE_STATE_FASTABSENT,       /* 节点快速拔出 */
    IF_INTF_PRECFG_STATE_PRESENT,   /* 预配置接口插入 */
    IF_NODE_STATE_PROVISION_PRESENT,/* 预配置板插入 */
    IF_NODE_STATE_PROVISION_DELETE, /* 预配置板拔出 */
    IF_HOT_STATE_MAX
}IF_HOT_STATE_E;

/*********************BEGIN: WAN接口相关********************************/

#define IF_TIMESLOT_MAX                         32
                                                
#define IF_CHANGROUP_PRI                        50      /* pri特殊组号 */
#define IF_CHANGROUP_VOICE                      51      /* 语音特殊组号 */
#define IF_CHANGROUP_NULL                       64      /* 空组号 */
                                                        
#define IF_E3_E1NUM                             16      /* E3下面E1控制器的数量 */
#define IF_T3_T1NUM                             28      /* T3下面T1控制器的数量 */
#define IF_CPOS_E1NUM                           63      /* CPOS下面E1控制器的数量 */
#define IF_CPOS_T1NUM                           84      /* CPOS下面T1控制器的数量 */
#define IF_CPOS_E3NUM                           3       /* CPOS下面E3控制器的数量 */
#define IF_CPOS_T3NUM                           3       /* CPOS下面T3控制器的数量 */

/* DO FLAG */
#define IF_OPERFLAG_DO                          0       /* 发送消息功能配置值 */
#define IF_OPERFLAG_UNDO                        1       /* 不发送消息功能配置值 */

/* DETECT SWITCH */            
#define IF_DETECT_ON                            0
#define IF_DETECT_OFF                           1
                                                        
#define IF_AUTOCLOCKCHANGE_NULL                 0   
#define IF_AUTOCLOCKCHANGE_AUTO                 1  

/* CLOCK */                                                
#define IF_CLOCK_LINE                           0       /* 线路时钟模式 */
#define IF_CLOCK_INTERNAL                       1       /* 内部时钟模式 */

/* LOOPBACK */
#define IF_LOOPBACK_NOLOOP                      0       /* 禁止环回 */
#define IF_LOOPBACK_LOCAL                       1       /* 对内自环 */
#define IF_LOOPBACK_REMOTE                      2       /* 对外远端环回 */
#define IF_LOOPBACK_PAYLOAD                     3       /* 对外远端载荷环回 */

/* FRAME */
#define IF_FRAME_SDH                            0       /* SDH */
#define IF_FRAME_SONET                          1       /* SONET */

/* E1/FE1/CE3-E1/CPOS-E1 FRAMING */
#define IF_E1_FRAMING_NOCRC4                    0  
#define IF_E1_FRAMING_CRC4                      1

/* T1/FT1/CT3-T1/CPOS-T1 FRAMING */
#define IF_T1_FRAMING_ESF                       0       /* Extended Super Frame */
#define IF_T1_FRAMING_SF                        1       /* Super Frame */

/* CT3/CPOS-T3 FRAMING */
#define IF_CT3_FRAMING_CBIT                     0       /* C-BIT */
#define IF_CT3_FRAMING_M23                      1       /* M23 */

/* T1/CT3-T1 FDL */
#define IF_T1_FDL_NONE                          0   
#define IF_T1_FDL_ANSI                          1
#define IF_T1_FDL_ATT                           2
#define IF_T1_FDL_BOTH                          3

/* CE3/CPOS-E3 NATIONAL BIT */                                                    
#define IF_CE3_NATIONALBIT_0                    0
#define IF_CE3_NATIONALBIT_1                    1 
                                                
/* CT3/CPOS-T3 ALARM GENERATE */                            
#define IF_CT3_ALARM_RAI                        0
#define IF_CT3_ALARM_AIS                        1
#define IF_CT3_ALARM_IDLE                       2
#define IF_CT3_ALARM_FEBE                       3

/* CT3/CPOS-T3 FEAC */
#define IF_CT3_FEAC_DS3LOS                      0
#define IF_CT3_FEAC_DS3OOF                      1
#define IF_CT3_FEAC_DS3AIS                      2
#define IF_CT3_FEAC_DS3IDLE                     3
#define IF_CT3_FEAC_DS3EQPTFAIL                 4
#define IF_CT3_FEAC_DS3LINE                     5
#define IF_CT3_FEAC_DS3PAYLOAD                  6

/* CT3/CPOS-T3 MDL */
#define IF_CT3_MDL_GENERATE_ALL                 0
#define IF_CT3_MDL_GENERATE_PATH                1
#define IF_CT3_MDL_GENERATE_IDLE                2
#define IF_CT3_MDL_GENERATE_TEST                3
                                                
#define IF_CT3_MDL_DATA_ALL                     0
#define IF_CT3_MDL_DATA_EIC                     1
#define IF_CT3_MDL_DATA_LIC                     2
#define IF_CT3_MDL_DATA_FIC                     3
#define IF_CT3_MDL_DATA_UNIT                    4
#define IF_CT3_MDL_DATA_PFI                     5
#define IF_CT3_MDL_DATA_PORT                    6
#define IF_CT3_MDL_DATA_GEN                     7
                                                
#define IF_MDL_DATA_LEN                         38 
#define IF_MDL_NUMBER                           7

/* BERT PATTERN */
#define IF_BERT_PAT_2E7                         0
#define IF_BERT_PAT_2E11                        1
#define IF_BERT_PAT_2E15                        2
#define IF_BERT_PAT_2E20                        3
#define IF_BERT_PAT_2E23                        4
#define IF_BERT_PAT_QRSS                        5
                                                
/* DEFAULT ITF NUMBER */                        
#define IF_ITFNUMBER_DEFAULT                    4

/* CHANNEL MODE */
#define IF_CHANMODE_FRAMED                      0       /* 通道化模式 */
#define IF_CHANMODE_UNFRAMED                    1       /* 净通道模式 */                                                        

/* 时隙信息IF_TIMESLOTINFO_S的通道类型标志的宏，按位表示 */
#define IF_FLAG_ISDN                            0x1     /* E1 PRI通道标志 */
#define IF_FLAG_UNFRAMED                        0x2     /* 静通道模式标志 */
#define IF_FLAG_SPEED_56K                       0x4     /* 56k 速率标志 */
#define IF_FLAG_E1T1VI_CCS                      0x8     /* CCS信号标志，pri-set时下发 */
#define IF_FLAG_E1T1VI_CAS                      0x10    /* CAS信号标志，E1/T1 timeslot-set时下发 */
#define IF_FLAG_E3                              0x20    /* 高速CPOS接口E3标志 */
#define IF_FLAG_T3                              0x40    /* 高速CPOS接口E3标志 */
#define IF_FLAG_FRAMED                          0x80    /* 高速CPOS接口通道模式 */
#define IF_FLAG_POS                             0x100   /* POS标志，pri-set时下发 */
#define IF_FLAG_DM                              0x200   /* DM标志，pri-set时下发 */

/* 接口时隙信息，创建通道化接口时命令字IOCTL_COMMON_CREATE_CHANNEL的参数 */
ISSU typedef struct tagIfTimeSlotInfo
{
    IF_INDEX ifIndex;                           /* 所生成接口的索引 */
    UINT     uiFlag;                            /* 通道类型标志, 按位表示 */
    UINT     uiChanNo;                          /* 通道号 */
    UINT     uiTSCount;                         /* 时隙数量 */    
    UINT     auiTSList[IF_TIMESLOT_MAX];        /* 时隙号码数组 */
}IF_TIMESLOTINFO_S;

/* IF_COMMON_CHANDATA_S用来存放CE3/CT3/POS/CPOS等接口数据形式相似的公共数据结构 */
ISSU typedef struct tagIfCommonChanDataCmd
{
    UINT uiChanNo;                              /* CE3/CT3接口或E1/T1通道或CPOS通道 */
    UINT uiSetValue;                            /* 配置值 */
}IF_COMMON_CHANDATA_S, *PIF_COMMON_CHANDATA_S;

ISSU typedef struct tagIfCommonChanDataFlagCmd
{
    UINT uiChanNo;                              /* 接口或通道号 */
    UINT uiSetValue;                            /* 配置值 */
    UINT uiDoFlag;                              /* 正向或反向操作标记*/
}IF_COMMON_CHANDATAFLAG_S, *PIF_COMMON_CHANDATAFLAG_S;

ISSU typedef struct IF_CHANINFO
{
    IF_INDEX ifIndex;
    UINT uiChanNo;
}IF_CHANINFO_S;

typedef struct tagIfBertData
{
    UINT uiOnFlag;                              /* 停止或开始值*/
    UINT uiPatternValue;                        /* 测试模式*/
    UINT uiTimeValue;                           /* BERT测试的持续时间值 */
    UINT uiFramedFlag;                          /* 成帧模式或非成帧模式 */
    UINT uiChanNo;                              /* CE3/CT3接口或E1/T1通道号 */
}IF_BERT_DATA_S, *PIF_BERT_DATA_S;

/* IOCTL_CELLULAR_NETWORK 获取接口支持的网络 */
#define IF_CELLULAR_NETWORK_NONE  0x0UL  /* 无有效网络 */
#define IF_CELLULAR_NETWORK_2G    0x1UL  /* 2G 网络 */
#define IF_CELLULAR_NETWORK_3G    0x2UL  /* 3G 网络 */
#define IF_CELLULAR_NETWORK_4G    0x4UL  /* 4G 网络 */

/* Cellular口PhyIOCtl命令字 */
#define IOCTL_CELLULAR_CREATE_CHANNEL           IF_IOCTL_CMD_DEF(MID_IFNET, IF_CELLULAR_TYPE, 0)
#define IOCTL_CELLULAR_NETWORK                  IF_IOCTL_CMD_DEF(MID_IFNET, IF_CELLULAR_TYPE, 1) /* 查询cell-eth支持的网络 */

/* ===BEGIN: 串口命令字、宏=== */               
#define IOCTL_SERIAL_GET_WORK_MODE              IF_IOCTL_CMD_DEF(MID_IFNET, IF_SERIAL_TYPE, 1)
#define IOCTL_SERIAL_SET_ACCM                   IF_IOCTL_CMD_DEF(MID_IFNET, IF_SERIAL_TYPE, 2)
#define IOCTL_SERIAL_QUERY_SOFTCVT              IF_IOCTL_CMD_DEF(MID_IFNET, IF_SERIAL_TYPE, 3)
#define IOCTL_SERIAL_SET_ASYNC_WORK_MODE        IF_IOCTL_CMD_DEF(MID_IFNET, IF_SERIAL_TYPE, 4)  /* 设置异步工作模式 */
#define IOCTL_SERIAL_RESET_LEAD                 IF_IOCTL_CMD_DEF(MID_IFNET, IF_SERIAL_TYPE, 5)
#define IOCTL_SERIAL_SET_LEAD                   IF_IOCTL_CMD_DEF(MID_IFNET, IF_SERIAL_TYPE, 6)
#define IOCTL_SERIAL_SET_FLOWCTLPEER            IF_IOCTL_CMD_DEF(MID_IFNET, IF_SERIAL_TYPE, 7)
#define IOCTL_SERIAL_SET_BAUDRATE               IF_IOCTL_CMD_DEF(MID_IFNET, IF_SERIAL_TYPE, 8)  /* 设置波特率 */
#define IOCTL_SERIAL_SET_DTE_CLOCK              IF_IOCTL_CMD_DEF(MID_IFNET, IF_SERIAL_TYPE, 9)  /* 设置DTE时钟模式 */
#define IOCTL_SERIAL_SET_LINE_CODE              IF_IOCTL_CMD_DEF(MID_IFNET, IF_SERIAL_TYPE, 10) /* 设置线路编码 */
#define IOCTL_SERIAL_DETECT_DCD                 IF_IOCTL_CMD_DEF(MID_IFNET, IF_SERIAL_TYPE, 11) /* 设置是否检测DCD */
#define IOCTL_SERIAL_DETECT_DSR                 IF_IOCTL_CMD_DEF(MID_IFNET, IF_SERIAL_TYPE, 12) /* 设置是否检测DSR */
#define IOCTL_SERIAL_SET_IDLE_MARK              IF_IOCTL_CMD_DEF(MID_IFNET, IF_SERIAL_TYPE, 13) /* 设置线路空闲码 */
#define IOCTL_SERIAL_SET_ELIMINATE_PULSE        IF_IOCTL_CMD_DEF(MID_IFNET, IF_SERIAL_TYPE, 14) /* 设置脉冲宽度 */
#define IOCTL_SERIAL_INVERT_RCLK                IF_IOCTL_CMD_DEF(MID_IFNET, IF_SERIAL_TYPE, 15) /* 设置翻转接收时钟 */
#define IOCTL_SERIAL_INVERT_TCLK                IF_IOCTL_CMD_DEF(MID_IFNET, IF_SERIAL_TYPE, 16) /* 设置翻转发送时钟 */
#define IOCTL_SERIAL_SET_LOOPBACK               IF_IOCTL_CMD_DEF(MID_IFNET, IF_SERIAL_TYPE, 17) /* 设置环回模式 */
#define IOCTL_SERIAL_SET_PHYSICAL_MODE          IF_IOCTL_CMD_DEF(MID_IFNET, IF_SERIAL_TYPE, 18) /* 设置串口工作模式 */
#define IOCTL_SERIAL_SET_PHYMRU                 IF_IOCTL_CMD_DEF(MID_IFNET, IF_SERIAL_TYPE, 19) /* 设置串口最大接收单元MRU */
#define IOCTL_SERIAL_REVERSE_RTS                IF_IOCTL_CMD_DEF(MID_IFNET, IF_SERIAL_TYPE, 20) /* 设置翻转RTS */
#define IOCTL_SERIAL_SET_VIRTUAL_BAUDRATE       IF_IOCTL_CMD_DEF(MID_IFNET, IF_SERIAL_TYPE, 21) /* 设置虚波特率 */
#define IOCTL_SERIAL_SET_CRC_TYPE               IF_IOCTL_CMD_DEF(MID_IFNET, IF_SERIAL_TYPE, 22) /* 设置CRC校验使用的位数 */
#define IOCTL_SERIAL_SET_ITF_NUMBER             IF_IOCTL_CMD_DEF(MID_IFNET, IF_SERIAL_TYPE, 23) /* 设置帧间填充符数量 */
#define IOCTL_SERIAL_GET_CABLETYPE              IF_IOCTL_CMD_DEF(MID_IFNET, IF_SERIAL_TYPE, 24) /* 获取线缆类型(DTE/DCE) */

/* 串口的命令字对应的参数定义 */
/* IOCTL_SERIAL_SET_ASYNC_WORK_MODE */
#define IF_SERIAL_MODE_FLOW_EXEC                1   /* AM, Aux口默认为流模式 */
#define IF_SERIAL_MODE_PROTOCOL                 2   /* 异步串口默认为协议模式 */
                                            
/* IOCTL_SERIAL_SET_DTE_CLOCK */            
#define IF_SERIAL_CLKSLT_DTE1                   1   /* 默认配置 */
#define IF_SERIAL_CLKSLT_DTE2                   2
#define IF_SERIAL_CLKSLT_DTE3                   3
#define IF_SERIAL_CLKSLT_DTE4                   4
#define IF_SERIAL_CLKSLT_DTE5                   5
#define IF_SERIAL_CLKSLT_DTEAUTO                6   /* 自动配置 */
                                                
#define IF_SERIAL_CLKSLT_DCE1                   7   /* 收发时钟均取自本地。若支持配置DCE，为DCE模式下的默认配置。*/
#define IF_SERIAL_CLKSLT_DCE2                   8   /* 发送时钟取自本地、接收时钟取自线路。 */
#define IF_SERIAL_CLKSLT_DCE3                   9   /* 收发时钟均取自线路。 */
#define IF_SERIAL_CLKSLT_DCEC                   10  /* No Command Line Set */

#define IF_SERIAL_FLOWCTL_ENABLE                1   /* 通知对端打开流控 */
#define IF_SERIAL_FLOWCTL_DISABLE               2   /* 通知对端取消流控 */
                                                
/* IOCTL_SERIAL_SET_LINE_CODE */                
#define IF_SERIAL_LINECODE_NRZ                  0
#define IF_SERIAL_LINECODE_NRZI                 1
                                                
/* IOCTL_SERIAL_DETECT_DCD */                   
#define IF_SERIAL_DETECT_DCD_NONE               0
#define IF_SERIAL_DETECT_DCD                    1   /* 默认配置 */
                                                
/* IOCTL_SERIAL_DETECT_DSR */                   
#define IF_SERIAL_DETECT_DSR_NONE               0
#define IF_SERIAL_DETECT_DSR                    1   /* 默认配置 */
                                                
/* IOCTL_SERIAL_SET_IDLE_MARK */                
#define IF_SERIAL_IDLE_FLAG                     0   /* 默认配置 */
#define IF_SERIAL_IDLE_MARK                     1
                                            
/* IOCTL_SERIAL_SET_ELIMINATE_PULSE */      
#define IF_SERIAL_ELIMINATE_PULSE_NONE          0   /* 默认配置 */
#define IF_SERIAL_ELIMINATE_PULSE               1
                                                
/* IOCTL_SERIAL_INVERT_RCLK */                  
#define IF_SERIAL_INVERT_RCLK_NONE              0   /* 默认配置 */
#define IF_SERIAL_INVERT_RCLK                   1
                                                
/* IOCTL_SERIAL_INVERT_TCLK */                  
#define IF_SERIAL_INVERT_TCLK_NONE              0   /* 默认配置 */
#define IF_SERIAL_INVERT_TCLK                   1
                                                
/* IOCTL_SERIAL_SET_LOOPBACK */                 
#define IF_SERIAL_LOOPBACK_NONE                 0   /* 默认配置 */
#define IF_SERIAL_LOOPBACK_LOOP                 1           
                                                
/* IOCTL_SERIAL_SET_PHYSICAL_MODE */            
#define IF_SERIAL_PHYLAYER_SYNC                 0   /* 默认配置 */
#define IF_SERIAL_PHYLAYER_ASYNC                1
                                                
/* IOCTL_SERIAL_REVERSE_RTS */                  
#define IF_SERIAL_REVERSE_RTS_NONE              0   /* 默认配置 */
#define IF_SERIAL_REVERSE_RTS                   1
                                                
/* IOCTL_SERIAL_SET_CRC_TYPE */                  
#define IF_SERIAL_CRC_16BIT                     0
#define IF_SERIAL_CRC_32BIT                     1
#define IF_SERIAL_CRC_NONE                      2
#define IF_SERIAL_CRC_DEFAULT                   IF_SERIAL_CRC_16BIT
                                                
#define IF_SERIAL_DEFAULT_BAUDRATE_SYNC         64000   /* 同步模式下的默认波特率 */
#define IF_SERIAL_DEFAULT_BAUDRATE_ASYNC        9600    /* 异步模式下的默认波特率 */
#define IF_SERIAL_DEFAULT_MRU                   1700    /* 默认MRU */

/* ===END: 串口命令字、宏=== */


/* ===BEGIN: FCM命令字、宏=== */
#define IOCTL_FCM_SET_PCM                       IF_IOCTL_CMD_DEF(MID_IFNET, IF_FCM_TYPE, 1) /* 设置Pulse Code Modulation */
                                                
#define IF_FCM_PCM_A_LAW                        1   /* A law */
#define IF_FCM_PCM_U_LAW                        2   /* U law */
/* ===END: FCM命令字、宏=== */                  
                                                
                                                
/* ===BEGIN: BRI命令字、宏=== */                
#define IOCTL_BRI_ACTIVATE                      IF_IOCTL_CMD_DEF(MID_IFNET, IF_BRI_TYPE, 1)  /* 激活BRI */
#define IOCTL_BRI_SET_LOOPBACK                  IF_IOCTL_CMD_DEF(MID_IFNET, IF_BRI_TYPE, 2)  /* 设置环回 */
#define IOCTL_BRI_SWITCH_PHYTYPE                IF_IOCTL_CMD_DEF(MID_IFNET, IF_BRI_TYPE, 3)  /* isdn leased-line通知驱动 */
                                                
/* IOCTL_BRI_SET_LOOPBACK */                    
#define IF_BRI_LOOPBACK_NO                      0   /* default */
#define IF_BRI_LOOPBACK_B1                      1
#define IF_BRI_LOOPBACK_B2                      2
#define IF_BRI_LOOPBACK_BOTH                    3
#define IF_BRI_LOOPBACK_REMOTE                  4

#define IF_BRI_MAX_CHANNUM                      2   /* Bri接口子通道最大个数 */

#define IF_BRI_ISDN_LEASED_NO                   0x0U
#define IF_BRI_ISDN_LEASED_B1                   0x1U
#define IF_BRI_ISDN_LEASED_B2                   0x2U
#define IF_BRI_ISDN_LEASED_B1B2                 (IF_BRI_ISDN_LEASED_B1 | IF_BRI_ISDN_LEASED_B2)
#define IF_BRI_ISDN_LEASED_128                  0x4U
#define IF_BRI_ISDN_LEASED_144                  0x8U

/* IOCTL_BRI_SWITCH_PHYTYPE下发驱动携带的数据 */
typedef struct tagIF_BRI_SWITCH_INFO
{
    UINT uiOperType; /* 操作类型 */
    UINT uiLineType; /* 专线类型，128 or 144 */
}IF_BRI_SWITCH_INFO_S;

/* 预读isdn leased-line配置使用的结构 */
ISSU typedef struct tagIF_BRI_LINE_INFO
{
    UINT uiLineType;                    /* 专线类型，128 or 144 */
    CHAR szIfName[IF_MAX_NAME_LEN + 1]; /* 接口名*/
}IF_BRI_LINE_INFO_S;

/* ===END: BRI命令字、宏=== */


/* ===BEGIN: E1/T1/E3/T3命令字、宏=== */
/* E1/T1控制器口物理命令字 */
#define IOCTL_E1T1_SET_CLOCK                    IF_IOCTL_CMD_DEF(MID_IFNET, IF_T1_TYPE, 1)  /* 设置时钟模式 */
#define IOCTL_E1T1_SET_LINE_CODE                IF_IOCTL_CMD_DEF(MID_IFNET, IF_T1_TYPE, 2)  /* 设置线路编码格式 */
#define IOCTL_E1T1_SET_FRAMING                  IF_IOCTL_CMD_DEF(MID_IFNET, IF_T1_TYPE, 3)  /* 设置成帧格式 */
#define IOCTL_E1T1_SET_IDLE_CODE                IF_IOCTL_CMD_DEF(MID_IFNET, IF_T1_TYPE, 4)  /* 设置空闲码 */
#define IOCTL_E1T1_SET_ITF_CODE                 IF_IOCTL_CMD_DEF(MID_IFNET, IF_T1_TYPE, 5)  /* 设置帧间填充符格式 */
#define IOCTL_E1T1_SET_ITF_NUMBER               IF_IOCTL_CMD_DEF(MID_IFNET, IF_T1_TYPE, 6)  /* 设置帧间填充符数量 */
#define IOCTL_E1T1_SET_LOOPBACK                 IF_IOCTL_CMD_DEF(MID_IFNET, IF_T1_TYPE, 7)  /* 设置环回 */
#define IOCTL_E1T1_SET_CABLE                    IF_IOCTL_CMD_DEF(MID_IFNET, IF_T1_TYPE, 9)  /* 设置使用的电缆类型 */
#define IOCTL_E1T1_SET_FDL                      IF_IOCTL_CMD_DEF(MID_IFNET, IF_T1_TYPE, 10) /* 配置T1使用FDL位的模式 */
#define IOCTL_E1T1_SET_SEND_LOOPCODE            IF_IOCTL_CMD_DEF(MID_IFNET, IF_T1_TYPE, 11) /* 设置远端环回 */
#define IOCTL_E1T1_SET_BERT                     IF_IOCTL_CMD_DEF(MID_IFNET, IF_T1_TYPE, 12) /* 设置bert */
#define IOCTL_E1T1_SET_ALARMTHRESHOLD_LOS_PD    IF_IOCTL_CMD_DEF(MID_IFNET, IF_T1_TYPE, 13) /* 设置告警门限 */
#define IOCTL_E1T1_SET_ALARMTHRESHOLD_LOS_PR    IF_IOCTL_CMD_DEF(MID_IFNET, IF_T1_TYPE, 14) /* 设置告警门限 */
#define IOCTL_E1T1_SET_ALARMTHRESHOLD_AIS       IF_IOCTL_CMD_DEF(MID_IFNET, IF_T1_TYPE, 15) /* 设置告警门限 */
#define IOCTL_E1T1_SET_ALARMTHRESHOLD_LFA       IF_IOCTL_CMD_DEF(MID_IFNET, IF_T1_TYPE, 16) /* 设置告警门限 */
#define IOCTL_E1T1_SET_ALARMTHRESHOLD_DEFAULT   IF_IOCTL_CMD_DEF(MID_IFNET, IF_T1_TYPE, 17) /* 设置告警门限 */
#define IOCTL_E1T1_SET_DATA_CODING              IF_IOCTL_CMD_DEF(MID_IFNET, IF_T1_TYPE, 18) /* 设置数据是否翻转 */
#define IOCTL_E1T1_SET_TDMCLOCK                 IF_IOCTL_CMD_DEF(MID_IFNET, IF_T1_TYPE, 19) /* 设置TDM时钟模式 */
#define IOCTL_E1T1_GET_E1T1MIB                  IF_IOCTL_CMD_DEF(MID_IFNET, IF_T1_TYPE, 20) /* 获取E1/T1错误统计信息 */
#define IOCTL_E1T1_SET_AIS_DETECTION            IF_IOCTL_CMD_DEF(MID_IFNET, IF_T1_TYPE, 21) /* 设置接口的AIS检测模式 */
#define IOCTL_E1T1_GET_VIMIB                    IF_IOCTL_CMD_DEF(MID_IFNET, IF_T1_TYPE, 22) /* 获取E1VI/T1VI接口呼叫时隙数 */
#define IOCTL_E1T1_GET_VISTATE                  IF_IOCTL_CMD_DEF(MID_IFNET, IF_T1_TYPE, 23) /* 判断VE1/VT1接口下是否存在呼叫 */
#define IOCTL_E1T1_SET_ALARM_DETECT             IF_IOCTL_CMD_DEF(MID_IFNET, IF_T1_TYPE, 24) /* 设置检测告警信号 */
#define IOCTL_E1POSDM_SET_WORK_MODE             IF_IOCTL_CMD_DEF(MID_IFNET, IF_E1_TYPE, 1)  /* 设置E1POSDM的工作方式 */
#define IOCTL_E1_SET_CABLETYPE                  IF_IOCTL_CMD_DEF(MID_IFNET, IF_E1_TYPE, 2)  /* 设置使用的线缆阻抗类型 */

/* E3控制器口物理命令字 */
#define IOCTL_E3_SET_PORT                       IF_IOCTL_CMD_DEF(MID_IFNET, IF_E3_TYPE, 0)  /* 设置E3T3工作模式 */
#define IOCTL_CE3_SET_CLOCK                     IF_IOCTL_CMD_DEF(MID_IFNET, IF_E3_TYPE, 1)  /* 设置时钟模式 */
#define IOCTL_CE3_SET_LOOPBACK                  IF_IOCTL_CMD_DEF(MID_IFNET, IF_E3_TYPE, 2)  /* 设置环回 */
#define IOCTL_CE3_SET_NATIONALBIT               IF_IOCTL_CMD_DEF(MID_IFNET, IF_E3_TYPE, 3)  /* 设置国际位 */
#define IOCTL_CE3_SET_FRAMING                   IF_IOCTL_CMD_DEF(MID_IFNET, IF_E3_TYPE, 4)  /* 设置成帧格式 */
#define IOCTL_CE3_SET_BERT                      IF_IOCTL_CMD_DEF(MID_IFNET, IF_E3_TYPE, 5)  /* 配置CE3接口和E1通道BERT测试模式 */
#define IOCTL_FE3_SET_DSUMODE                   IF_IOCTL_CMD_DEF(MID_IFNET, IF_E3_TYPE, 6)  /* 设置DSU模式 */
#define IOCTL_FE3_SET_SUBRATE                   IF_IOCTL_CMD_DEF(MID_IFNET, IF_E3_TYPE, 7)  /* 设置子速率 */

/* T3控制器口物理命令字 */
#define IOCTL_T3_SET_PORT                       IF_IOCTL_CMD_DEF(MID_IFNET, IF_T3_TYPE, 0)   /* 设置E3T3工作模式 */
#define IOCTL_CT3_SET_CLOCK                     IF_IOCTL_CMD_DEF(MID_IFNET, IF_T3_TYPE, 1)
#define IOCTL_CT3_SET_LOOPBACK                  IF_IOCTL_CMD_DEF(MID_IFNET, IF_T3_TYPE, 2)
#define IOCTL_CT3_SET_FRAMING                   IF_IOCTL_CMD_DEF(MID_IFNET, IF_T3_TYPE, 3)
#define IOCTL_CT3_SET_CABLE                     IF_IOCTL_CMD_DEF(MID_IFNET, IF_T3_TYPE, 4)
#define IOCTL_CT3_SET_ALARM_DETECT              IF_IOCTL_CMD_DEF(MID_IFNET, IF_T3_TYPE, 5)
#define IOCTL_CT3_SET_ALARM_GENERATE            IF_IOCTL_CMD_DEF(MID_IFNET, IF_T3_TYPE, 6)
#define IOCTL_CT3_SET_BERT                      IF_IOCTL_CMD_DEF(MID_IFNET, IF_T3_TYPE, 7)
#define IOCTL_CT3_SET_FEAC_DETECT               IF_IOCTL_CMD_DEF(MID_IFNET, IF_T3_TYPE, 8)
#define IOCTL_CT3_FEAC_GENERATE                 IF_IOCTL_CMD_DEF(MID_IFNET, IF_T3_TYPE, 9)
#define IOCTL_CT3_SET_MDL_DETECT                IF_IOCTL_CMD_DEF(MID_IFNET, IF_T3_TYPE, 10)
#define IOCTL_CT3_SET_MDL_DATA                  IF_IOCTL_CMD_DEF(MID_IFNET, IF_T3_TYPE, 11)
#define IOCTL_CT3_MDL_GENERATE                  IF_IOCTL_CMD_DEF(MID_IFNET, IF_T3_TYPE, 12)
#define IOCTL_CT3_MDL_UNDO                      IF_IOCTL_CMD_DEF(MID_IFNET, IF_T3_TYPE, 13)
#define IOCTL_CT3_SHOW_T1_INFO                  IF_IOCTL_CMD_DEF(MID_IFNET, IF_T3_TYPE, 14)
#define IOCTL_CT3_SET_FDL                       IF_IOCTL_CMD_DEF(MID_IFNET, IF_T3_TYPE, 15)
#define IOCTL_CT3_SEND_LOOPCODE                 IF_IOCTL_CMD_DEF(MID_IFNET, IF_T3_TYPE, 16)
#define IOCTL_FT3_SET_DSUMODE                   IF_IOCTL_CMD_DEF(MID_IFNET, IF_T3_TYPE, 17)
#define IOCTL_FT3_SET_SUBRATE                   IF_IOCTL_CMD_DEF(MID_IFNET, IF_T3_TYPE, 18)

/* IOCTL_E1POSDM_SET_WORK_MODE */            
#define IF_E1POSDM_WORKMODE_SYNC                 0   /* 默认配置 */
#define IF_E1POSDM_WORKMODE_ASYNC                1

#define IF_E1_CABLE_TYPE_75                      75   
#define IF_E1_CABLE_TYPE_120                     120

#define IF_E1T1_CLOCK_DTE                       0   
#define IF_E1T1_CLOCK_DCE                       1
                                                
#define IF_E1T1_LINECODE_AMI                    0
#define IF_E1T1_LINECODE_B8ZS                   1  
#define IF_E1T1_LINECODE_HDB3                   2  
                                                
#define IF_E1T1_IDLECODE_7E                     0
#define IF_E1T1_IDLECODE_FF                     1
                                                
#define IF_E1T1_ITFCODE_7E                      0   
#define IF_E1T1_ITFCODE_FF                      1
                                                
#define IF_E1T1_TDMCLOCK_INTERNAL               0   
#define IF_E1T1_TDMCLOCK_LINE                   1   
#define IF_E1T1_TDMCLOCK_LINEPRI                2
#define IF_E1T1_TDMCLOCK_NONE                   3
                                                
#define IF_E1T1_SST_EM_DELAY                    0x001
#define IF_E1T1_SST_EM_WINK                     0x002
#define IF_E1T1_SST_EM_IMMEDIATE                0x004
#define IF_E1T1_SST_FXS_LOOP                    0x008
#define IF_E1T1_SST_R2_DIGITAL                  0x010
#define IF_E1T1_SST_R2_D_COMPELLED              0x020
#define IF_E1T1_SST_R2_D_C_ANI                  0x040
#define IF_E1T1_SST_LGS_FXS_GROUND              0x080
#define IF_E1T1_SST_LGS_FXO_GROUND              0x100
#define IF_E1T1_SST_LGS_FXS_LOOP                0x200
#define IF_E1T1_SST_LGS_FXO_LOOP                0x400

#define IF_E1_TIMESLOT                          32      /* E1时隙数量 */
#define IF_T1_TIMESLOT                          24      /* T1时隙数量 */

#define IF_E1_PRICHAN                           16      /* E1 D通道 */
#define IF_T1_PRICHAN                           24      /* T1 D通道 */                                        
                                        
#define IF_E1_CABLE_LONG                        0   
#define IF_E1_CABLE_SHORT                       1
                                                
#define IF_T1_CABLE_L0                          0   
#define IF_T1_CABLE_L7                          1
#define IF_T1_CABLE_L15                         2
#define IF_T1_CABLE_L22                         3
#define IF_T1_CABLE_S133                        4
#define IF_T1_CABLE_S266                        5
#define IF_T1_CABLE_S399                        6
#define IF_T1_CABLE_S533                        7
#define IF_T1_CABLE_S655                        8
                                                
#define IF_T1_SENDLOOPCODE_NONE                 0 
#define IF_T1_INBAND_LLB_UP                     1
#define IF_T1_INBAND_LLB_DOWN                   2
#define IF_T1_FDL_ANSI_BOM_LLB_UP               3
#define IF_T1_FDL_ANSI_BOM_LLB_DOWN             4
#define IF_T1_FDL_ANSI_BOM_PLB_UP               5
#define IF_T1_FDL_ANSI_BOM_PLB_DOWN             6
#define IF_T1_FDL_ATT_HDLC_PLB_UP               7
#define IF_T1_FDL_ATT_HDLC_PLB_DOWN             8
                                                
#define IF_BERT_RUN                             0
#define IF_BERT_STOP                            1

#define IF_T1_ALARMTHRESHOLD_LOS_PD_DEFAULT     176 
#define IF_T1_ALARMTHRESHOLD_LOS_PD_MIN         16
#define IF_T1_ALARMTHRESHOLD_LOS_PD_MAX         4096

#define IF_T1_ALARMTHRESHOLD_LOS_PR_DEFAULT     22  
#define IF_T1_ALARMTHRESHOLD_LOS_PR_MIN         1
#define IF_T1_ALARMTHRESHOLD_LOS_PR_MAX         256

#define IF_T1_ALARMTHRESHOLD_AIS_LEVEL1         0  
#define IF_T1_ALARMTHRESHOLD_AIS_LEVEL2         1

#define IF_T1_ALARMTHRESHOLD_LFA_LEVEL1         0  
#define IF_T1_ALARMTHRESHOLD_LFA_LEVEL2         1
#define IF_T1_ALARMTHRESHOLD_LFA_LEVEL3         2
#define IF_T1_ALARMTHRESHOLD_LFA_LEVEL4         3
                                                
#define IF_T1_DATACODING_NORMAL                 0   
#define IF_T1_DATACODING_INVERTED               1

#define IF_FE3_DSUMODE_0                        0
#define IF_FE3_DSUMODE_1                        1
#define IF_FE3_DSUMODE_DEFAULT                  IF_FE3_DSUMODE_1
#define IF_FE3_SUBRATE_DEFAULT                  34010

#define IF_FT3_DSUMODE_0                        0
#define IF_FT3_DSUMODE_1                        1
#define IF_FT3_DSUMODE_2                        2
#define IF_FT3_DSUMODE_3                        3
#define IF_FT3_DSUMODE_4                        4
#define IF_FT3_DSUMODE_DEFAULT                  IF_FT3_DSUMODE_0
#define IF_FT3_SUBRATE_DEFAULT                  44210
                                                
#define IF_CT3_CABLELEN_DEFAULT                 49

#define IF_CT3_SEND_LOOPCODE_INBAND_LINE        0
#define IF_CT3_SEND_LOOPCODE_FDL_LINE           1
#define IF_CT3_SEND_LOOPCODE_FDL_PAYLOAD        2
#define IF_CT3_SEND_LOOPCODE_FDL_ATT_PAYLOAD    3

/* IOCTL_ATM_SET_CABLE */
#define IF_ATM_CABLE_SHORT                  0   /* 短距电缆模式 */
#define IF_ATM_CABLE_LONG                   1   /* 长距电缆模式 */

/* IOCTL_ATM_SET_CLOCK */
#define IF_ATM_CLOCKSET_SLAVE               0   /* 从时钟模式 */
#define IF_ATM_CLOCKSET_MASTER              1   /* 主时钟模式 */
#define IF_ATM_CLOCKSET_DEFAULT             IF_ATM_CLOCKSET_SLAVE

/* IOCTL_ATM_SET_LINE_CODE */
#define IF_ATM_E1T1_LINECODE_AMI            0
#define IF_ATM_E1T1_LINECODE_B8ZS           1
#define IF_ATM_E1T1_LINECODE_HDB3           2

/* IOCTL_ATM_SET_FRAMING */
#define IF_ATM_FRAMING_SDH                  0   /* SDH帧格式 */
#define IF_ATM_FRAMING_SONET                1   /* SONET帧格式 */
#define IF_ATM_E3_FRAMING_G751PLCP          2   /* Default, G.751 PLCP成帧格式 */
#define IF_ATM_E3_FRAMING_G751ADM           3   /* G.751直接成帧格式 */
#define IF_ATM_E3_FRAMING_G832ADM           4   /* G.832直接成帧格式 */
#define IF_ATM_T3_FRAMING_CBITPLCP          5   /* Default, C-bit PLCP成帧格式 */
#define IF_ATM_T3_FRAMING_CBITADM           6   /* C-bit直接成帧格式 */
#define IF_ATM_T3_FRAMING_M23ADM            7   /* m23直接成帧格式 */
#define IF_ATM_T3_FRAMING_M23PLCP           8   /* m23 PLCP成帧格式 */
#define IF_ATM_E1_FRAMING_CRC4              9   /* Default, CRC4 ADM帧格式 */
#define IF_ATM_E1_FRAMING_NOCRC4            10  /* No-CRC4 ADM帧格式 */
#define IF_ATM_T1_FRAMING_ESF               11  /* Default, ESF帧格式 */
#define IF_ATM_T1_FRAMING_SF                12  /* SF帧格式 */

/* IOCTL_ATM_SET_LOOPBACK */
#define IF_ATM_LOOPBACK_NO                  0   /* default，禁止ATM接口环回 */
#define IF_ATM_LOOPBACK_CELL                1   /* cell，允许ATM接口对内信元自环 */
#define IF_ATM_LOOPBACK_PAYLOAD             2   /* payload，允许ATM接口对外载荷环回 */
#define IF_ATM_LOOPBACK_LINE                3   /* remote，允许ATM接口对外环回 */
#define IF_ATM_LOOPBACK_LOCAL               4   /* local，允许ATM接口对内自环 */

/* IOCTL_ATM_SET_SCRAMBLE */
#define IF_ATM_SCRAMBLE_ENABLE              0   /* default，使能加扰功能 */
#define IF_ATM_SCRAMBLE_DISABLE             1   /* 禁止加扰功能 */
#define IF_ATM_SCRAMBLE_DEFAULT             IF_ATM_SCRAMBLE_ENABLE

/* IOCTL_ATM_IMA_SET_MAX_DELAY */
#define IF_ATM_IMA_MAX_DELAY_DEFAULT        25

/* IOCTL_ATM_IMA_SET_FRAME_LENGTH */
#define IF_ATM_IMA_FRAME_32                 0
#define IF_ATM_IMA_FRAME_64                 1
#define IF_ATM_IMA_FRAME_128                2
#define IF_ATM_IMA_FRAME_256                3
#define IF_ATM_IMA_FRAME_LENGTH_DEFAULT     IF_ATM_IMA_FRAME_128

/* IOCTL_ATM_IMA_SET_CLOCK */
#define IF_ATM_IMA_CLOCK_ITC                8
#define IF_ATM_IMA_CLOCK_CTC                9
#define IF_ATM_IMA_CLOCK_DEFAULT            IF_ATM_IMA_CLOCK_CTC

/* IOCTL_ATM_IMA_SET_STANDARD */
#define IF_ATM_IMA_STANDARD_NORMAL          0
#define IF_ATM_IMA_STANDARD_ALTERNATE_V10   1
#define IF_ATM_IMA_STANDARD_V10             2
#define IF_ATM_IMA_STANDARD_V11             3
#define IF_ATM_IMA_STANDARD_DEFAULT         IF_ATM_IMA_STANDARD_NORMAL

/* IOCTL_ATM_IMA_SET_TEST */
#define IF_ATM_IMA_TESTIMA                  0                       /* 允许IMA组链路测试 */
#define IF_ATM_IMA_NOTTESTIMA               1                       /* 不允许IMA组链路测试 */
#define IF_ATM_IMA_IMATESTDEFAULT           IF_ATM_IMA_NOTTESTIMA   /* 默认不允许测试 */

/* IOCTL_ATM_IMA_MIN_ACTIVE_LINKS*/
#define IF_ATM_IMA_MIN_ACTIVE_LINKS_DEFAULT        1            /* ima-group组的最小激活链路数 */

/* IOCTL_ATM_SET_FLAG_C2 */
#define IF_ATM_FLAG_C2_DEFAULT              0x13    /* 默认配置 */
#define IF_ATM_FLAG_C2_MIN                  0x00    /* C2 FLAG 的最小配置 */
#define IF_ATM_FLAG_C2_MAX                  0xff    /* C2 FLAG 的最大配置 */

/* IOCTL_ATM_SET_FLAG_J0 / IOCTL_ATM_SET_FLAG_J1 */
#define IF_ATM_FLAG_SDH_J0                  0x01    /* J0开销字节使用SDH, 16字节模式 */
#define IF_ATM_FLAG_SDH_J1                  0x02    /* J1开销字节使用SDH, 16字节模式 */
#define IF_ATM_FLAG_SONET_J0                0x04    /* J0开销字节使用SONET, 1字节模式 */
#define IF_ATM_FLAG_SONET_J1                0x08    /* J1开销字节使用SONET, 64字节模式 */

#define IF_ATM_FLAG_J0_SONET_DEFAULT        0x01    /* J0单字节情况下默认值为1*/

#define IF_ATM_FLAG_J0_SDH_LENGTH           16      /* J0在16字节模式下的长度为16 */
#define IF_ATM_FLAG_J1_SDH_LENGTH           16      /* J1在SDH模式下是16字节模式 */
#define IF_ATM_FLAG_J1_SONET_LENGTH         64      /* J1在SONET模式下是64字节模式 */

#define IF_ATM_MAX_FLAG_LENGTH              IF_ATM_FLAG_J1_SONET_LENGTH

/* IOCTL_ATM_DSL_SET_ACTIVATE */
#define IF_ATM_DSL_ACTIVATE                 0   /* 激活ADSL线路 */
#define IF_ATM_DSL_NO_ACTIVATE              1   /* 去激活ADSL线路 */

/* IOCTL_ATM_DSL_SET_STANDARD */
#define IF_ATM_DSL_STANDARD_TYPE_T1413      0   /* 使用T1.413标准 */
#define IF_ATM_DSL_STANDARD_TYPE_GLITE      1   /* 使用G.Lite(G992.2)标准 */
#define IF_ATM_DSL_STANDARD_TYPE_GDMT       2   /* 使用G.DMT(G992.1)标准 */
#define IF_ATM_DSL_STANDARD_TYPE_AUTO       3   /* Default, 自适应方式 */
#define IF_ATM_DSL_STANDARD_TYPE_ADSL2      4   /* 使用G992.3标准 */
#define IF_ATM_DSL_STANDARD_TYPE_ADSL2PLUS  5   /* 使用G992.5标准 */

/* IOCTL_ATM_DSL_SET_TX_ATTENUATION */
#define IF_ATM_DSL_TX_ATTENUATION_DEFAULT   0

/* IOCTL_ATM_DSL_SET_ANNEX */
#define IF_ATM_DSL_SHDSL_ANNEX_TYPE_A       0   /* 支持的annex标准为 a */
#define IF_ATM_DSL_SHDSL_ANNEX_TYPE_B       1   /* 支持的annex标准为 b */
#define IF_ATM_DSL_SHDSL_ANNEX_TYPE_DEFAULT IF_ATM_DSL_SHDSL_ANNEX_TYPE_B

/* IOCTL_ATM_DSL_SET_CAPABILITY */
#define  IF_ATM_DSL_SHDSL_CAPABILITY_AUTO          0
#define  IF_ATM_DSL_SHDSL_CAPABILITY_GSHDSLBIS     1
#define  IF_ATM_DSL_SHDSL_CAPABILITY_GSHDSL        2

/* IOCTL_ATM_DSL_SET_MODE */
#define IF_ATM_DSL_SHDSL_MODE_TYPE_CPE      0   /* 用户端模式 */
#define IF_ATM_DSL_SHDSL_MODE_TYPE_CO       1   /* CO（局端）模式 */
#define IF_ATM_DSL_SHDSL_MODE_TYPE_DEFAULT  IF_ATM_DSL_SHDSL_MODE_TYPE_CPE

/* IOCTL_ATM_DSL_SET_PAM */
#define IF_ATM_DSL_SHDSL_PAM_AUTO           0
#define IF_ATM_DSL_SHDSL_PAM_16             1
#define IF_ATM_DSL_SHDSL_PAM_32             2

/* IOCTL_ATM_DSL_PBO */
#define IF_ATM_DSL_SHDSL_PBO_AUTO           0xffff

/* IOCTL_ATM_DSL_SET_PSD */
#define IF_ATM_DSL_SHDSL_PSD_TYPE_SYMMETRY  0   /* 对称方式 */
#define IF_ATM_DSL_SHDSL_PSD_TYPE_ASYMMETRY 1   /* 非对称方式 */
#define IF_ATM_DSL_SHDSL_PSD_TYPE_DEFAULT   IF_ATM_DSL_SHDSL_PSD_TYPE_SYMMETRY

/* IOCTL_ATM_DSL_SET_LINE_RATE */
#define IF_ATM_DSL_SHDSL_LINE_RATE_TYPE_AUTO       0   /* 自适应方式 */
#define IF_ATM_DSL_SHDSL_LINE_RATE_TYPE_DEFAULT    1   /* 设置为默认值 */
#define IF_ATM_DSL_SHDSL_4WIRE_RATE_TYPE_DEFAULT   2312

/* IOCTL_ATM_DSL_SET_MARGIN */
#define IF_ATM_DSL_SHDSL_CURRENT_MARGIN_DEFAULT 2
#define IF_ATM_DSL_SHDSL_SNEXT_MARGIN_DEFAULT   0

/* IOCTL_ATM_DSL_SET_WIRE */
#define IF_ATM_DSL_SHDSL_WIRE_TYPE_TWO              0   /*两线模式*/
#define IF_ATM_DSL_SHDSL_WIRE_TYPE_FOUR             1   /*四线enhanced模式*/
#define IF_ATM_DSL_SHDSL_WIRE_TYPE_FOUR_AUTO        2   /*四线自动切换模式*/
#define IF_ATM_DSL_SHDSL_WIRE_TYPE_FOUR_STANDARD    3   /*四线standard模式 */
/* 四线HSDSL接口线路模式默认值为四线enhanced */
#define IF_ATM_DSL_SHDSL_WIRE_TYPE_DEFAULT          IF_ATM_DSL_SHDSL_WIRE_TYPE_FOUR
/* G.BIS的四线HSDSL接口线路模式默认值为四线standard */
#define IF_ATM_DSL_SHDSL_GBIS_WIRE_TYPE_DEFAULT     IF_ATM_DSL_SHDSL_WIRE_TYPE_FOUR_STANDARD
/*自动切换模式*/
#define IF_ATM_DSL_SHDSL_WIRE_TYPE_AUTO             IF_ATM_DSL_SHDSL_WIRE_TYPE_FOUR_AUTO
#define IF_ATM_DSL_SHDSL_WIRE_TYPE_SIX              4   /*六线模式 */
#define IF_ATM_DSL_SHDSL_WIRE_TYPE_EIGHT            5   /*八线模式 */
/* 八线HSDSL接口线路模式默认值为八线 */
#define IF_ATM_DSL_SHDSL_EIGHT_GBIS_WIRE_TYPE_DEFAULT    IF_ATM_DSL_SHDSL_WIRE_TYPE_EIGHT
/* EFM接口工作模式:非绑定和绑定 */
#define IF_SHDSL_WIRE_WORKMODE_NO_BIND 0
#define IF_SHDSL_WIRE_WORKMODE_BIND 1

ISSU typedef struct tagIF_AtmSetFlag
{
    UINT uiFlagType;
    UCHAR aucFlagValue[IF_ATM_MAX_FLAG_LENGTH];
}IF_ATM_SET_FLAG_S;

ISSU typedef struct tagIF_AtmSetMargin
{
    UINT uiCurrent;
    UINT uiSnext;
}IF_ATM_SET_MARGIN_S;

ISSU typedef struct tagIF_ImaSetTest
{
    UINT uiIsUndoTest;
    UINT uiIsSetLinkNum;
    UINT uiLinkNum;
    UINT uiIsSetPatId;
    UINT uiPatternId;
}IF_IMA_SET_TEST_S;

/* adslLineTable表 */
/* 表中adslLineCoding为通道编码类型，取值为如下的宏：*/
#define IF_ATM_DSL_LINE_CODING_TYPE_OTHER 1 /* none of the following */
#define IF_ATM_DSL_LINE_CODING_TYPE_DMT   2 /* Discrete MultiTone */
#define IF_ATM_DSL_LINE_CODING_TYPE_CAP   3 /* Carrierless Amplitude & Phase modulation */
#define IF_ATM_DSL_LINE_CODING_TYPE_QAM   4 /* Quadrature Amplitude Modulation */

/* 表中adslLineType为通道连接类型，取值为如下的宏 */
#define IF_ATM_DSL_LINE_TYPE_NOCHANNEL          1 /* no channels exist */
#define IF_ATM_DSL_LINE_TYPE_FASTONLY           2 /* fast channel exists only */
#define IF_ATM_DSL_LINE_TYPE_INTERLEAVEDONLY    3 /* interleaved channel exists only */
#define IF_ATM_DSL_LINE_TYPE_FASTORINTERLEAVED  4 /* either fast or interleaved channels can exist,
                                                     but only one at any time */
#define IF_ATM_DSL_LINE_TYPE_FASTANDINTERLEAVED 5 /* both fast or interleaved */

#define IF_ATM_DSL_LINE_PROFILE_MAXLEN 32
#define IF_SNMP_MAX_OID_LEN            128

typedef struct tagIFAdslLineInfo
{
    UINT uiLineCoding;
    UINT uiLineType;
    UINT uiOIDLen;   /* OID 长度 */
    UINT auiOID[IF_SNMP_MAX_OID_LEN]; /* array to store ids */
    CHAR acLineConfProfile[IF_ATM_DSL_LINE_PROFILE_MAXLEN + 1];
    CHAR acLineAlarmConfProfile[IF_ATM_DSL_LINE_PROFILE_MAXLEN + 1];
}IF_ATM_DSL_LINE_INFO_S;

/* adslAtucPhysTable表和adslAturPhysTable表 */
/* 表中uiAtuType表示需要获取的表项数据域，取值为如下宏 */
#define IF_ATM_DSL_ATU_PHY_TYPE_CENTRAL 1 /* 局端 */
#define IF_ATM_DSL_ATU_PHY_TYPE_REMOTE  2 /* 远端 */

#define IF_ATM_DSL_ATU_MAXLEN_CURRSTATUS 2
#define IF_ATM_DSL_ATU_MAXLEN_SERIALNUMBER  32
#define IF_ATM_DSL_ATU_MAXLEN_VENDORID      16
#define IF_ATM_DSL_ATU_MAXLEN_VERSIONNUMBER 16

/*
    acAtuCurrStatus字段为Atu当前状态的位图,位图比特位从最左位开始算，依次代表的意思为
    局端，远端通用部分:
    "noDefect";
    "lossOfFraming";
    "lossOfSignal";
    "lossOfPower";
    "lossOfSignalQuality";

    局端特有部分:
    "lossOfLink";
    "dataInitFailure";
    "configInitFailure";
    "protocolInitFailure";
    "noPeerAtuPresent";
*/

/* acAtuCurrStatus为Atu当前状态的位图,为以下宏的或值 */
/* acAtuCurrStatus[0]的赋值 */
#define IF_ATM_DSL_ATU_PHY_STATUS_NODEFECT              (UCHAR)0x80 /* noDefect */
#define IF_ATM_DSL_ATU_PHY_STATUS_LOSSOFFRAMING         (UCHAR)0x40 /* lossOfFraming */
#define IF_ATM_DSL_ATU_PHY_STATUS_LOSSOFSIGNAL          (UCHAR)0x20 /* lossOfSignal */
#define IF_ATM_DSL_ATU_PHY_STATUS_LOSSOFPOWER           (UCHAR)0x10 /* lossOfPower */
#define IF_ATM_DSL_ATU_PHY_STATUS_LOSSOFSIGNALQUALITY   (UCHAR)0x08 /* lossOfSignalQuality */
#define IF_ATM_DSL_ATU_PHY_STATUS_LOSSOFLINK            (UCHAR)0x04 /* lossOfLink,       局端特有 */
#define IF_ATM_DSL_ATU_PHY_STATUS_DATAINITFAILURE       (UCHAR)0x02 /* dataInitFailure,  局端特有 */
#define IF_ATM_DSL_ATU_PHY_STATUS_CONFIGINITFAILURE     (UCHAR)0x01 /* configInitFailure,局端特有 */

/* acAtuCurrStatus[1]的赋值 */
#define IF_ATM_DSL_ATU_PHY_STATUS_PROTOCOLINITFAILURE   (UCHAR)0x80 /* protocolInitFailure,局端特有 */
#define IF_ATM_DSL_ATU_PHY_STATUS_NOPEERATUPRESENT      (UCHAR)0x40 /* noPeerAtuPresent,   局端特有 */

ISSU typedef struct tagAdslAtuPhysTable
{
    UINT uiAtuType;
    UINT uiCurrAtn;
    UINT uiCurrAttainableRate;
    INT iCurrSnrMgn;
    INT iCurrOutputPwr;
    UCHAR aucAtuCurrStatus[IF_ATM_DSL_ATU_MAXLEN_CURRSTATUS];
    CHAR acAtuInvSerialNumber[IF_ATM_DSL_ATU_MAXLEN_SERIALNUMBER + 1];
    CHAR acAtuInvVendorID[IF_ATM_DSL_ATU_MAXLEN_VENDORID + 1];
    CHAR acAtuInvVersionNumber[IF_ATM_DSL_ATU_MAXLEN_VERSIONNUMBER + 1];
} IF_ATM_DSL_ATU_PHY_INFO_S;

/* adslAtucChanTable表和adslAturChanTable表 */
ISSU typedef struct tagAdslAtuChanTable
{
    UINT uiAtuType;
    UINT uiInterleaveDelay;
    UINT uiCurrTxRate;
    UINT uiPrevTxRate;
    UINT uiCrcBlockLength;
}IF_ATM_DSL_ATU_CHAN_INFO_S;

ISSU typedef struct tagIFE1T1MIB
{
    UINT uiInterfaceInErrs;                     /* E1/T1接口收到错包总数 */
    UINT uiInterfaceInRuntsErrs;                /* E1/T1接口收到的超短包的错误数 */
    UINT uiInterfaceInGiantsErrs;               /* E1/T1接口收到的超长包的错包数 */
    UINT uiInterfaceInCrcErrs;                  /* E1/T1接口收到的CRC错包数 */
    UINT uiInterfaceInAlignErrs;                /* E1/T1接口收到的Align错包数 */
    UINT uiInterfaceInOverRunsErrs;             /* E1/T1接口收到的OverRun错包数 */
    UINT uiInterfaceInDribblesErrs;             /* E1/T1接口收到的Dribble错包数 */
    UINT uiInterfaceInAbortedSeqErrs;           /* E1/T1接口收到的AbortedSeq错包数 */
    UINT uiInterfaceInNoBufferErrs;             /* E1/T1接口收到的NoBuffer错包数 */
    UINT uiInterfaceInFramingErrs;              /* E1/T1接口收到的Framing错包数 */
    UINT uiInterfaceOutputErrs;                 /* E1/T1接口发出的错包数 */
    UINT uiInterfaceOutUnderRunErrs;            /* E1/T1接口发出的UnderRun错包数 */
    UINT uiInterfaceOutCollisonsErrs;           /* E1/T1接口发出的Collison错包数 */
    UINT uiInterfaceOutDeferedErrs;             /* E1/T1接口发出的OutDefered错包数 */
}IF_E1T1_MIB_S;

typedef struct tagIFE1T1VIMIB
{
    UINT uiE1T1VIUsingTimeslots;                /* E1/T1VI接口占用时隙总数 */
}IF_E1T1VI_MIB_S;

typedef struct tagIfCT3MdlDataStr
{
    UINT uiChanNo;
    UINT uiMDLDataType;
    UINT uiOperType;
    CHAR szStr[IF_MDL_DATA_LEN + 1];
    UCHAR ucReserved;
}IF_CT3_MDL_DATA_S;

/* ===END: E1/T1/E3/T3命令字、宏=== */


/* ===BEGIN: POS命令字、宏=== */
/* POS接口PhyIOCtl命令字 */
#define IOCTL_POS_SET_CRC_LENGTH                IF_IOCTL_CMD_DEF(MID_IFNET, IF_POS_TYPE, 1) /* 设置POS接口的时钟模式 */
#define IOCTL_POS_SET_CLOCK                     IF_IOCTL_CMD_DEF(MID_IFNET, IF_POS_TYPE, 2) /* 设置POS接口的CRC长度 */
#define IOCTL_POS_SET_SCRAMBLING                IF_IOCTL_CMD_DEF(MID_IFNET, IF_POS_TYPE, 3) /* 设置POS接口加扰 */
#define IOCTL_POS_SET_FRAMING                   IF_IOCTL_CMD_DEF(MID_IFNET, IF_POS_TYPE, 4) /* 设置POS接口的帧格式 */
#define IOCTL_POS_SET_LOOPBACK                  IF_IOCTL_CMD_DEF(MID_IFNET, IF_POS_TYPE, 5) /* 设置POS接口的环回模式 */
#define IOCTL_POS_SET_FLAG                      IF_IOCTL_CMD_DEF(MID_IFNET, IF_POS_TYPE, 6) /* 设置SDH/SONET开销字节 */
#define IOCTL_POS_SET_SPEED                     IF_IOCTL_CMD_DEF(MID_IFNET, IF_POS_TYPE, 7) /* 设置POS接口的SPEED */

/* POS命令字对应的参数定义 */
/* IOCTL_POS_SET_CRC_LENGTH */
#define IF_POS_CRC_32                           0      /* 32位CRC，默认值 */
#define IF_POS_CRC_16                           1      /* 16位CRC */
#define IF_POS_CRC_DEFAULT                      IF_POS_CRC_32

/* IOCTL_POS_SET_CLOCK */
#define IF_POS_CLOCK_DEFAULT                    IF_CLOCK_LINE

/* IOCTL_POS_SET_SCRAMBLING */
#define IF_POS_SCRAMBLE                         0       /* 对载荷数据加扰，默认值 */
#define IF_POS_SCRAMBLE_NONE                    1       /* 禁止对载荷数据加扰 */
#define IF_POS_SCRAMBLE_DEFAULT                 IF_POS_SCRAMBLE

/* IOCTL_POS_SET_FRAMING */
#define IF_POS_FRAME_DEFAULT                    IF_FRAME_SDH

/* IOCTL_POS_SET_LOOPBACK */
#define IF_POS_LOOPBACK_DEFAULT                 IF_LOOPBACK_NOLOOP

/* IOCTL_POS_SET_SPEED */
#define IF_POS_SPEED155                         155
#define IF_POS_SPEED622                         622
#define IF_POS_SPEED2500                        2500
#define IF_POS_SPEED10000                       10000

/* rpr pos 的门限告警默认值定义 */
#define IF_POS_RPR_BER_SD_DEFAULT_VALUE         5
#define IF_POS_RPR_BER_SF_DEFAULT_VALUE         3

/* IOCTL_POS_SET_FLAG */
#define IF_POS_FLAG_C2                          0       /* 信号标记字节C2 */
#define IF_POS_FLAG_J0                          1       /* 再生段跟踪字节J0 */
#define IF_POS_FLAG_J1                          2       /* 通道跟踪字节J1 */

#define IF_POS_FLAG_TYPE_IGNORE                 0       /* 忽略J1检查 */
#define IF_POS_FLAG_TYPE_BYTE                   1       /* 长度为1字节 */
#define IF_POS_FLAG_TYPE_STRING16               16      /* 长度为16字节(最大可设置长度为15，第一个字节用于校验) */
#define IF_POS_FLAG_TYPE_STRING64               64      /* 长度为64字节(最大可设置长度为62), 最后2字节为0x0d0a */

#define IF_POS_FLAG_LEN_STR16                   15UL
#define IF_POS_FLAG_LEN_STR64                   62UL
#define IF_POS_FLAG_LEN_MAX                     64UL

#define IF_POS_FLAG_C2_DEFAULT                  0x16    /* C2默认值 */
#define IF_POS_FLAG_J0_SONET_DEFAULT            0x01    /* J0 SONET格式默认值 */

#define IF_POS_FLAG_IGNORE_ENABLE               0x1     /* 使能忽略检查 */
#define IF_POS_FLAG_IGNORE_DISABLE              0x2     /* 去使能忽略检查 */

#define IF_POS_FLAG_SDH_LEN     IF_POS_FLAG_LEN_STR16   /* SDH格式开销长度 */
#define IF_POS_FLAG_SONET_LEN   IF_POS_FLAG_LEN_STR64   /* SONET格式J1长度 */

ISSU typedef struct tagIfPOSFlagCmd
{
    UINT uiFlag;                                        /* Flag类型 */
    UINT uiLength;                                      /* Flag长度 */
    UCHAR aucVal[IF_POS_FLAG_LEN_MAX];                  /* Flag值 */
}IF_POS_FLAG_CMD_S, *PIF_POS_FLAG_CMD_S;
/* ===END: POS命令字、宏、结构=== */


/* ===BEGIN: CPOS控制器口命令字、宏、结构=== */
#define IOCTL_CPOS_SET_CLOCK                    IF_IOCTL_CMD_DEF(MID_IFNET, IF_CPOS_TYPE, 1) /* 设置CPOS接口时钟模式 */
#define IOCTL_CPOS_SET_FRAMING                  IF_IOCTL_CMD_DEF(MID_IFNET, IF_CPOS_TYPE, 2) /* 设置CPOS接口的帧格式 */
#define IOCTL_CPOS_SET_LOOPBACK                 IF_IOCTL_CMD_DEF(MID_IFNET, IF_CPOS_TYPE, 3) /* 设置CPOS接口的环回模式 */
#define IOCTL_CPOS_SET_MAPPING                  IF_IOCTL_CMD_DEF(MID_IFNET, IF_CPOS_TYPE, 4) /* 设置AUG的复用模式 */
#define IOCTL_CPOS_SET_FLAG                     IF_IOCTL_CMD_DEF(MID_IFNET, IF_CPOS_TYPE, 5) /* 设置SDH/SONET开销字节 */
#define IOCTL_CPOS_SET_E1_CLOCK                 IF_IOCTL_CMD_DEF(MID_IFNET, IF_CPOS_TYPE, 6) /* 设置CPOS下E1通道的时钟模式 */
#define IOCTL_CPOS_SET_T1_CLOCK                 IF_IOCTL_CMD_DEF(MID_IFNET, IF_CPOS_TYPE, 7) /* 设置CPOS下T1通道的时钟模式 */
#define IOCTL_CPOS_SET_E1_FRAMING               IF_IOCTL_CMD_DEF(MID_IFNET, IF_CPOS_TYPE, 8) /* 设置CPOS下E1通道的帧格式 */
#define IOCTL_CPOS_SET_T1_FRAMING               IF_IOCTL_CMD_DEF(MID_IFNET, IF_CPOS_TYPE, 9) /* 设置CPOS下T1通道的帧格式 */
#define IOCTL_CPOS_SET_E1_LOOPBACK              IF_IOCTL_CMD_DEF(MID_IFNET, IF_CPOS_TYPE, 10)/* 设置CPOS下E1通道环回模式 */
#define IOCTL_CPOS_SET_T1_LOOPBACK              IF_IOCTL_CMD_DEF(MID_IFNET, IF_CPOS_TYPE, 11)/* 设置CPOS下T1通道环回模式 */
#define IOCTL_CPOS_SET_PORT                     IF_IOCTL_CMD_DEF(MID_IFNET, IF_CPOS_TYPE, 12)/* 设置加强CPOS接口模式改变 */

#define IOCTL_CPOS_SET_E3_CLOCK                 IF_IOCTL_CMD_DEF(MID_IFNET, IF_CPOS_TYPE, 13)/* 设置CPOS下E3通道的时钟模式 */
#define IOCTL_CPOS_SET_T3_CLOCK                 IF_IOCTL_CMD_DEF(MID_IFNET, IF_CPOS_TYPE, 14)/* 设置CPOS下T3通道的时钟模式 */
#define IOCTL_CPOS_SET_E3_LOOPBACK              IF_IOCTL_CMD_DEF(MID_IFNET, IF_CPOS_TYPE, 15)/* 设置CPOS下E3通道环回模式 */
#define IOCTL_CPOS_SET_T3_LOOPBACK              IF_IOCTL_CMD_DEF(MID_IFNET, IF_CPOS_TYPE, 16)/* 设置CPOS下T3通道环回模式 */
#define IOCTL_CPOS_SET_E3_FLAG                  IF_IOCTL_CMD_DEF(MID_IFNET, IF_CPOS_TYPE, 17)/* 设置CPOS下E3通道SDH/SONET开销字节 */
#define IOCTL_CPOS_SET_T3_FLAG                  IF_IOCTL_CMD_DEF(MID_IFNET, IF_CPOS_TYPE, 18)/* 设置CPOS下T3通道SDH/SONET开销字节 */
#define IOCTL_CPOS_SET_FE3_DSUMODE              IF_IOCTL_CMD_DEF(MID_IFNET, IF_CPOS_TYPE, 19)/* 设置CPOS下E3通道DSU模式 */
#define IOCTL_CPOS_SET_FE3_SUBRATE              IF_IOCTL_CMD_DEF(MID_IFNET, IF_CPOS_TYPE, 20)/* 设置CPOS下E3通道子速率 */
#define IOCTL_CPOS_SET_FT3_DSUMODE              IF_IOCTL_CMD_DEF(MID_IFNET, IF_CPOS_TYPE, 21)/* 设置CPOS下T3通道DSU模式 */
#define IOCTL_CPOS_SET_FT3_SUBRATE              IF_IOCTL_CMD_DEF(MID_IFNET, IF_CPOS_TYPE, 22)/* 设置CPOS下T3通道子速率 */
#define IOCTL_CPOS_SET_E3_NATIONALBIT           IF_IOCTL_CMD_DEF(MID_IFNET, IF_CPOS_TYPE, 23)/* 设置CPOS下E3通道国际位 */
#define IOCTL_CPOS_SET_T3_FRAMING               IF_IOCTL_CMD_DEF(MID_IFNET, IF_CPOS_TYPE, 24)/* 设置CPOS下T3通道的帧格式 */
#define IOCTL_CPOS_SET_T3_FEAC_DETECT           IF_IOCTL_CMD_DEF(MID_IFNET, IF_CPOS_TYPE, 25)/* 设置CPOS下T3通道的定时检测FEAC链路信号功能 */
#define IOCTL_CPOS_T3_FEAC_GENERATE             IF_IOCTL_CMD_DEF(MID_IFNET, IF_CPOS_TYPE, 26)/* CPOS下T3通道发送FEAC信号 */
#define IOCTL_CPOS_SET_T3_ALARM_DETECT          IF_IOCTL_CMD_DEF(MID_IFNET, IF_CPOS_TYPE, 27)
#define IOCTL_CPOS_SET_T3_ALARM_GENERATE        IF_IOCTL_CMD_DEF(MID_IFNET, IF_CPOS_TYPE, 28)/* 设置CPOS下T3通道ALARM */
#define IOCTL_CPOS_SET_T3_BERT                  IF_IOCTL_CMD_DEF(MID_IFNET, IF_CPOS_TYPE, 29)
#define IOCTL_CPOS_SET_T3_MDL_DETECT            IF_IOCTL_CMD_DEF(MID_IFNET, IF_CPOS_TYPE, 30)
#define IOCTL_CPOS_T3_MDL_GENERATE              IF_IOCTL_CMD_DEF(MID_IFNET, IF_CPOS_TYPE, 31)/* 设置CPOS下T3通道MDL */
#define IOCTL_CPOS_SET_T3_MDL_DATA              IF_IOCTL_CMD_DEF(MID_IFNET, IF_CPOS_TYPE, 32)
#define IOCTL_CPOS_T3_MDL_UNDO                  IF_IOCTL_CMD_DEF(MID_IFNET, IF_CPOS_TYPE, 33)
#define IOCTL_CPOS_SET_AU3TUG3                  IF_IOCTL_CMD_DEF(MID_IFNET, IF_CPOS_TYPE, 34)/* 设置融合接口的融合状态 */
#define IOCTL_CPOS_CHECK_CHANNEL                IF_IOCTL_CMD_DEF(MID_IFNET, IF_CPOS_TYPE, 35)/* 执行通道化命令前检查驱动 */
#define IOCTL_CPOS_SET_MULTI_CHANNEL            IF_IOCTL_CMD_DEF(MID_IFNET, IF_CPOS_TYPE, 36)/* 设置CPOS接口下channel-align-mode */

/* CPOS相关宏定义 */
#define IF_CPOS_FLAG_TU3_VC3                    1          
#define IF_CPOS_FLAG_TU3_VC4                    2   
                                                
#define IF_CPOS_MAXCHANNUM                      256     /* 每个CPOS下的最大的通道化串口数量 */
                                                
/* CPOS融合口通道融合标记 */                    
#define IF_CPOS_JOINFLAG_E1T1                   1    
#define IF_CPOS_JOINFLAG_E3T3                   2
#define IF_CPOS_JOINFLAG_NULL                   0
                                                
/* CPOS接口融合下驱动状态字 */
#define IF_CPOS_CHANTYPE_E3                     1
#define IF_CPOS_CHANTYPE_E1                     2
#define IF_CPOS_CHANTYPE_T3                     3
#define IF_CPOS_CHANTYPE_T1                     4
#define IF_CPOS_CHANTYPE_DEFAULT                5  

#define IF_CPOS_2DOT5GBITS  3          /*2.5G通道号占通道标记位*/
#define IF_CPOS_622MBITS    3          /*622M通道号占通道标记位*/
#define IF_CPOS_155MBITS    3          /*155M通道号占通道标记位*/
#define IF_CPOS_E3T3BITS    6          /*E3/T3通道号占通道标记位*/
#define IF_CPOS_E1T1BITS    13         /*E1/T1通道号占通道标记位*/
#define IF_CPOS_RESERVED    4          /*保留位*/

#define IF_CPOS_2DOT5GMASK  0x7        /*2.5G通道标记掩码*/
#define IF_CPOS_622MMASK    0x7        /*622M通道标记掩码*/
#define IF_CPOS_155MMASK    0x7         /*155M通道标记掩码*/
#define IF_CPOS_E3T3MASK    0x3f       /*E3/T3通道标记掩码*/
#define IF_CPOS_E1T1MASK    0x1fff     /*E1/T1通道标记掩码*/

/*表明取一阶通道,即通道化2.5GPos通道号*/
#define IF_GET_2DOT5GCHANNEL(uiChannel) (((uiChannel) \
                                         >>  (IF_CPOS_E1T1BITS + IF_CPOS_E3T3BITS + IF_CPOS_155MBITS + IF_CPOS_622MBITS + IF_CPOS_RESERVED)) \
                                         & (IF_CPOS_2DOT5GMASK))

/*表明取二阶通道,即通道化622M通道号*/
#define IF_GET_622MCHANNEL(uiChannel) (((uiChannel) \
                                        >> (IF_CPOS_E1T1BITS + IF_CPOS_E3T3BITS + IF_CPOS_155MBITS + IF_CPOS_RESERVED)) \
                                        & (IF_CPOS_622MMASK))

/*表明取三阶通道,即通道化155通道号*/
#define IF_GET_155MCHANNEL(uiChannel) (((uiChannel) >> (IF_CPOS_E1T1BITS + IF_CPOS_E3T3BITS + IF_CPOS_RESERVED)) & (IF_CPOS_155MMASK))

/*表明取四阶通道,即通道化E3/T3通道号*/
#define IF_GET_E3T3CHANNEL(uiChannel) (((uiChannel) >> (IF_CPOS_E1T1BITS + IF_CPOS_RESERVED)) & (IF_CPOS_E3T3MASK))

/*表明取五阶通道,即通道化E1/T1通道号*/
#define IF_GET_E1T1CHANNEL(uiChannel) (((uiChannel) >> (IF_CPOS_RESERVED)) & (IF_CPOS_E1T1MASK))

/* CPOS命令字对应的参数定义 */                  
/* IOCTL_CPOS_SET_CLOCK  */                     
#define IF_CPOS_CLOCK_DEFAULT                   IF_CLOCK_LINE
                                                
/* IOCTL_CPOS_SET_FRAMING  */                   
#define IF_CPOS_FRAME_DEFAULT                   IF_FRAME_SDH

/* IOCTL_CPOS_SET_LOOPBACK  */
#define IF_CPOS_LOOPBACK_DEFAULT                IF_LOOPBACK_NOLOOP
                                                
/* IOCTL_CPOS_SET_MAPPING  */                   
#define IF_CPOS_MAPPING_AU4                     0       /* AUG通过AU-4得到，default */
#define IF_CPOS_MAPPING_AU3                     1       /* AUG通过AU-3得到 */
#define IF_CPOS_MAPPING_DEFAULT                 IF_CPOS_MAPPING_AU4
                                                
#define IF_CPOS_SUB_CLOCK_DEFAULT               IF_CLOCK_LINE

/* IOCTL_CPOS_SET_T1_FRAMING  */                
#define IF_CPOS_SUB_FRAMING_DEFAULT             IF_T1_FRAMING_ESF
                                                
#define IF_CPOS_SUB_LOOPBACK_DEFAULT            IF_LOOPBACK_NOLOOP

/* IOCTL_CPOS_SET_FE3_DSUMODE */
/* Digital Link，支持子速率范围为358-34010kbps，共95个速率等级，级差358000bps, default */
#define IF_CPOS_FE3_DSUMODE_0                   0
/* Kentrox，支持子速率范围为500-24500,34010kbps，共50个速率等级，级差500000 bps */
#define IF_CPOS_FE3_DSUMODE_1                   1
#define IF_CPOS_FE3_DSUMODE_DEFAULT             1

/* IOCTL_CPOS_SET_FE3_SUBRATE */
#define IF_CPOS_FE3_SUBRATE_DEFAULT             34010

/* IOCTL_CPOS_SET_FT3_DSUMODE */
/* Digital Link，支持子速率范围为300-44210 kbps，共147个速率等级，级差300746 bps, default */
#define IF_CPOS_FT3_DSUMODE_0                   0
/* Kentrox，支持子速率范围为1500-35000 kbps及44210 kbps，共57个速率等级，级差1500000 bps。（目前尚不支持）*/
#define IF_CPOS_FT3_DSUMODE_1                   1
/* Larscom，支持子速率范围为3100-44210 kbps，共14个速率等级，级差3157835 bps */
#define IF_CPOS_FT3_DSUMODE_2                   2
/* Adtran，支持子速率范围为75-44210 kbps，共588个速率等级，级差75187 bps */
#define IF_CPOS_FT3_DSUMODE_3                   3
/* Verilink，支持子速率范围为1500-44210 kbps，共20个速率等级，级差1578918 bps */
#define IF_CPOS_FT3_DSUMODE_4                   4
#define IF_CPOS_FT3_DSUMODE_DEFAULT             0
                                                
/* IOCTL_CPOS_SET_FT3_SUBRATE */                
#define IF_CPOS_FT3_SUBRATE_DEFAULT             44210
                                                
/* IOCTL_CPOS_SET_T3_FEAC_DETECT */
#define IF_CPOS_T3_FEAC_DETECT_DEFAULT          IF_DETECT_ON
                                                
/* IOCTL_CPOS_SET_FLAG  */
#define IF_CPOS_FLAG_C2                         0       /* 信号标记字节C2 */
#define IF_CPOS_FLAG_J0                         1       /* 再生段跟踪字节J0 */
#define IF_CPOS_FLAG_J1                         2       /* 高阶通道跟踪字节J1 */
#define IF_CPOS_FLAG_S1S0                       3       /* H1的SS位, 指示AU或TU类型 */
#define IF_CPOS_FLAG_S1                         4       /* 同步状态 */
#define IF_CPOS_FLAG_J2                         5       /* 低阶通道跟踪字节J2 */
#define IF_CPOS_FLAG_LPC2                       6       /* 低阶通道信号标记字节C2 */
#define IF_CPOS_FLAG_C2_TU3                     7
#define IF_CPOS_FLAG_J1_TU3                     8
#define IF_CPOS_FLAG_S1S0_TU3                   9
                                                
#define IF_CPOS_FLAG_TYPE_BYTE                  1       /* 长度为1字节 */
#define IF_CPOS_FLAG_TYPE_STRING16              16      /* 长度为16字节(最大可设置长度为15，第一个字节用于校验) */
#define IF_CPOS_FLAG_TYPE_STRING64              64      /* 长度为64字节(最大可设置长度为62), 最后2字节为0x0d0a */
                                                
#define IF_CPOS_FLAG_LEN_STR16                  15
#define IF_CPOS_FLAG_LEN_STR64                  62
#define IF_CPOS_FLAG_LEN_MAX                    64
                                                
#define IF_CPOS_FLAG_VC3_C2_DEFAULT             0x04    /* E3/T3 CPOS?TU3 C2的默认值 */
#define IF_CPOS_FLAG_C2_DEFAULT                 0x02    /* C2默认值(TUG) */
                                                
#define IF_CPOS_FLAG_J0_SONET_DEFAULT           0x01    /* J0 SONET格式默认值 */
                                                
#define IF_CPOS_FLAG_S1S0_SONET_DEFAULT         0x00    /* S1S0 SONET默认值 */
#define IF_CPOS_FLAG_S1S0_SDH_DEFAULT           0x02    /* S1S0 SDH默认值 */
                                                
#define IF_CPOS_FLAG_S1_DEFAULT                 0x0f    /* S1默认值(不可用于时钟同步) */
#define IF_CPOS_FLAG_LPC2_DEFAULT               0x02    /* LPC2默认值(TUG) */

/* port connection mode*/
#define IF_CONNECTION_NORMAL 1
#define IF_CONNECTION_EXTEND 2

ISSU typedef struct tagIfCPOSFlagCmd
{
    UINT uiFlag;                                        /* Flag类型 */
    UINT uiLength;                                      /* Flag长度 */
    UINT uiPathNo;                                      /* 通道号 */
    UCHAR aucVal[IF_CPOS_FLAG_LEN_MAX];                 /* Flag值 */
}IF_CPOS_FLAG_CMD_S, *PIF_CPOS_FLAG_CMD_S;

ISSU typedef struct tagIfCPOSPortCmd
{
    UINT uiChanFlag;                                    /* 通道标志 */
    UINT uiModeFlag;                                    /* 设置模式 */
}IF_CPOS_PORT_CMD_S, *PIF_CPOS_PORT_CMD_S;

#define IF_CPOS_MULTICHANNEL_H3CHP  0
#define IF_CPOS_MULTICHANNEL_ALCATE 1
#define IF_CPOS_MULTICHANNEL_LUCENT 2

/* IOCTL_CPOS_SET_channel-align-mode */
#define IF_CPOS_MULTI_CHANNEL_DEFAULT           IF_CPOS_MULTICHANNEL_H3CHP


/* ===END: CPOS控制器口命令字、宏、结构=== */

/*********************END: WAN接口相关********************************/

/*********************************************************************/

typedef ULONG (* IF_LinkUpIOCtl_PF)(IN IF_INDEX*, IN ULONG, IN IF_LINKUP_E, IN VOID *);

IF_LinkUpIOCtl_PF IF_GetLinkUpIOCtl( IN IF_INDEX ifIndex );

#ifdef __KERNEL__
#ifndef _SYS_FSBUF_H_
#include "sys/fsbuf.h"
#endif

typedef ULONG (*IF_PhyIOCtl_PF) (IF_INDEX, ULONG, VOID *);

typedef ULONG (*IF_PhyTransmit_PF)(IF_INDEX, MBUF_S *, ULONG);

typedef ULONG (*IF_PhyTransmitRaw_PF)(IN IF_INDEX , IN VOID *, IN FSBUF_BLOCKINFO_S *, IN FSBUF_PKTINFO_S *, IN ULONG);

typedef ULONG (* IF_LinkInput_PF)(IN IF_INDEX, IN MBUF_S *);

typedef ULONG (* IF_LinkInputRaw_PF)(IN IF_INDEX, IN VOID *, IN FSBUF_PKTINFO_S *, IN FSBUF_BLOCKINFO_S *, IN VOID *);

typedef ULONG (* IF_SPEC_GlobalIoCtl_PF)(IN IF_GLOBAL_CMD_E, INOUT VOID *);

ULONG IF_SetPhyCB (IN IF_INDEX ifIndex, IN VOID* pCB);

VOID* IF_GetPhyCB (IN IF_INDEX ifIndex);

ULONG IF_SetPhyIOCtl( IN IF_INDEX ifIndex, IN const IF_PhyIOCtl_PF pfPhyIOCtl );

IF_PhyIOCtl_PF IF_GetPhyIOCtl( IN IF_INDEX ifIndex );
ULONG IF_PhyIOCtl( IN IF_INDEX ifIndex, IN ULONG ulCmd, IN VOID *pData );

ULONG IF_SetPhyTransmit( IN IF_INDEX ifIndex, IN const IF_PhyTransmit_PF pfPhyTransmit );

IF_PhyTransmit_PF IF_GetPhyTransmit( IN IF_INDEX ifIndex );

ULONG IF_PhyTransmit( IN IF_INDEX ifIndex, IN MBUF_S *pstMBuf, IN ULONG ulPriority );

ULONG IF_SetPhyTransmitRaw( IN IF_INDEX ifIndex, IN const IF_PhyTransmitRaw_PF pfPhyTransmitRaw );

IF_PhyTransmitRaw_PF IF_GetPhyTransmitRaw( IN IF_INDEX ifIndex );

ULONG IF_BlackHoleTransmitRaw(IN IF_INDEX ifIndex, 
                              IN VOID *pData, 
                              IN FSBUF_BLOCKINFO_S *pstBlockInfo, 
                              IN FSBUF_PKTINFO_S *pstPktInfo, 
                              IN ULONG ulPriority);

ULONG IF_PhyTransmitRaw(IN IF_INDEX ifIndex, 
                        IN VOID *pData, 
                        IN FSBUF_BLOCKINFO_S *pstBlockInfo, 
                        IN FSBUF_PKTINFO_S *pstPktInfo, 
                        IN ULONG ulPriority);

IF_LinkInput_PF IF_GetLinkInput( IN IF_INDEX ifIndex );

ULONG IF_LinkInput( IN IF_INDEX ifIndex, IN MBUF_S *pstMbuf );

IF_LinkInputRaw_PF IF_GetLinkInputRaw( IN IF_INDEX ifIndex );

ULONG IF_LinkInputRaw(IN IF_INDEX ifIndex,
                      IN VOID *pData,
                      IN FSBUF_PKTINFO_S *pstPktInfo,
                      IN FSBUF_BLOCKINFO_S *pstBlockInfo,
                      IN VOID *pUcore);

/* 接口连接器变化 */
typedef enum tagIFCONNECTORCHG
{
    IF_CONNECTOR_CHG = 1,           /* 连接器变化 */
    IF_CONNECTOR_FAULT,             /* 连接器故障 */
    IF_CONNECTOR_NORMAL,            /* 连接器恢复正常 */
    IF_CONNECTOR_MAX
}IFCONNECTORCHG_E;

/* 通知连接器变化 */
VOID IF_NotifyConnectorChange(IN IF_INDEX ifIndex, IN IFCONNECTORCHG_E enChange, IN const CHAR *pcConnectorName);

/* 通知接口插入/拔出 */
ULONG IF_NotifyIntfState (IN IF_HOT_STATE_E enIfHotState, IF_PORT_INFO_S *pstInfPortInfo, IN ULONG ulNum);

/* 接口类型定制接口 */

ULONG IF_RegL3FEIf (VOID);

ULONG IF_RegL2FEIf (VOID);

ULONG IF_RegLoopFEIf(VOID);

ULONG IF_RegMirrorFEIf(VOID);

ULONG IF_RegIDSFEIf(VOID);

ULONG IF_RegL3GEIf (VOID);

ULONG IF_RegL2GEIf (VOID);

ULONG IF_RegLoopGEIf(VOID);

ULONG IF_RegMirrorGEIf(VOID);

ULONG IF_RegIDSGEIf(VOID);

ULONG IF_RegL3XGEIf (VOID);

ULONG IF_RegL2XGEIf (VOID);

ULONG IF_RegLoopXGEIf(VOID);

ULONG IF_RegMirrorXGEIf(VOID);

ULONG IF_RegIDSXGEIf(VOID);

ULONG IF_RegPEXIf(VOID);

ULONG IF_RegCBGEIf(VOID);

ULONG IF_RegCBFGEIf(VOID);

ULONG IF_RegCBHGEIf(VOID);

ULONG IF_RegCBXGEIf(VOID);

ULONG IF_RegL3FGEIf(VOID);

ULONG IF_RegL2FGEIf(VOID);

ULONG IF_RegLoopFGEIf(VOID);

ULONG IF_RegMirrorFGEIf(VOID);

ULONG IF_RegIDSFGEIf(VOID);

ULONG IF_RegL3HGEIf(VOID);

ULONG IF_RegL2HGEIf(VOID);

ULONG IF_RegLoopHGEIf(VOID);

ULONG IF_RegMirrorHGEIf(VOID);

ULONG IF_RegIDSHGEIf(VOID);

ULONG IF_RegL3VEIf (VOID);

ULONG IF_RegL2VEIf (VOID);

ULONG IF_RegMEIf (VOID);

ULONG IF_RegMGEIf (VOID);

ULONG IF_RegLoopIf(VOID);

ULONG IF_RegNullIf(VOID);

ULONG IF_RegInLoopIf(VOID);

ULONG IF_RegTunnelIf(VOID);

ULONG IF_RegTunnelBundleIf(VOID);

ULONG IF_RegMTunnelIf(VOID);

ULONG IF_RegVlanIf(VOID);

ULONG IF_RegVTIf(VOID);

ULONG IF_RegVPPPIf(VOID);

ULONG IF_RegHdlcbIf(VOID);

ULONG IF_RegGlobalVAIf(VOID);

ULONG IF_RegLocalVAIf(VOID);

ULONG IF_RegCellularIf(VOID);

ULONG IF_RegMpgroupIf(VOID);

ULONG IF_RegAuxIf (VOID);

ULONG IF_RegRegisterTunnelIf (VOID);

ULONG IF_RegBaggIf(VOID);

ULONG IF_RegRaggIf(VOID);

ULONG IF_RegFCIf(VOID);

ULONG IF_RegVFCIf(VOID);

ULONG IF_RegFCBIf(VOID);

ULONG IF_RegEviLinkIf(VOID);

ULONG IF_RegSChannelIf(VOID);

ULONG IF_RegGSChannelIf(VOID);

ULONG IF_RegEthChannelIf(VOID);

ULONG IF_RegSerialIf(VOID);

ULONG IF_RegAsyncIf(VOID);

ULONG IF_RegInAsyncIf(VOID);

ULONG IF_RegAmIf(VOID);

ULONG IF_RegVoiceIf(VOID);

ULONG IF_RegDialerIf(VOID);

ULONG IF_RegE1If(VOID);

ULONG IF_RegT1If(VOID);

ULONG IF_RegBriIf(VOID);

ULONG IF_RegFcmIf(VOID);

ULONG IF_RegPosIf(VOID);

ULONG IF_RegCposIf(VOID);

ULONG IF_RegEncryptIf(VOID);

ULONG IF_RegE3If(VOID);

ULONG IF_RegT3If(VOID);

ULONG IF_RegL2TGEIf (VOID);

ULONG IF_RegL3TGEIf (VOID);

ULONG IF_RegLoopTGEIf (VOID);

ULONG IF_RegMirrorTGEIf (VOID);

ULONG IF_RegIDSTGEIf (VOID);
ULONG IF_RegVsiIf (VOID);
ULONG IF_RegBladeIf(VOID);
ULONG IF_RegBlaggIf(VOID);

ULONG IF_RegRPRXGEIf (VOID);
ULONG IF_RegRPRGEIf (VOID);
ULONG IF_RegRPRPOSIf (VOID);
ULONG IF_RegL2RPRIf (VOID);
ULONG IF_RegL3RPRIf (VOID);
ULONG IF_RegL2RPRLocalIf (VOID);

ULONG IF_RegAtmIf(VOID);
ULONG IF_RegEfmIf(VOID);
ULONG IF_RegImaIf(VOID);

ULONG IF_RegVeL2VpnIf (VOID);

ULONG IF_RegVeL3VpnIf (VOID);
ULONG IF_RegWlanBssIf(VOID);

ULONG IF_RegWlanRadioIf(VOID);
ULONG IF_RegWlanMeshIf(VOID);
ULONG IF_RegWlanMeshLinkIf(VOID);

ULONG IF_RegRETHIf(VOID);
ULONG IF_RegCemIf(VOID);
ULONG IF_RegMFRIf(VOID);
ULONG IF_RegInCommIf(VOID);
ULONG IF_RegSSLVPNACIf(VOID);
ULONG IF_RegCMTunnelIf(VOID);
ULONG IF_RegSubIfFlowinterval(VOID);
ULONG IF_RegPortFlowInterval(VOID);
ULONG IF_RegGlobalFlowInterval(VOID);
ULONG IF_RegNeedMannulNoLoop(VOID); /* 产品注册端口回环检测状态不需要手动取消特性 */
ULONG IF_RegEee(IN const VOID *pEeeSelect);

ULONG IF_RegLispIf(VOID);
ULONG IF_RegSchBdlIf(VOID);
ULONG IF_RegWTunnelIf (VOID);
ULONG IF_RegInEthIf(VOID);

typedef  struct tagIf_Driver_Intf{
    ULONG (*pfDRV_INTF_GetPortList)(IN USHORT usSlot, 
                                    IN USHORT usSubSlot, 
                                    OUT PIF_PORT_INFO_S * ppstPortList,
                                    OUT ULONG *pulPortNum);
    ULONG (*pfDRV_INTF_GetIfDeviceInfo)(OUT IF_SYSTEMINFO_S *pstDeviceInfo);
    ULONG (*pfDRV_INTF_GlobalIoCtl)(IN IF_GLOBAL_CMD_E enCmd, INOUT VOID *pParam);
    ULONG (*pfDRV_INTF_CreateComplete)(IN PIF_PORT_INFO_EXT_S pstPortInfo);  /* oam 使用*/
    ULONG (*pfDRV_INTF_DeleteComplete)(IN PIF_PORT_INFO_EXT_S pstPortInfo);  /* oam 使用*/
    ULONG (*pfDRV_INTF_SetProvisionMode)(IN const CHAR * pcMode, 
                                         IN USHORT usGSlot,
                                         IN USHORT usSubSlot,
                                         OUT IF_PORT_INFO_S ** ppstPortList,
                                         OUT ULONG * pulPortNum);
    VOID (*DRV_INTF_ClearProvisionMode)(IN USHORT usGSlot, IN USHORT usSubSlot);
} IF_DRV_INTF_S, *PIF_DRV_INTF_S;

typedef struct tagREMOTEIF_DRV_INTF {
    ULONG (*pfDRV_INTF_PhyIOCtl)(IN IF_INDEX ifIndex, IN ULONG ulCmd,VOID *pData);//处理非本板的驱动调用；
    ULONG (*pfDRV_INTF_HandleEvent)(IN IF_INDEX ifIndex, IN UINT uiEvent ,VOID *pData);//通知驱动处理事件；
} REMOTEIF_DRV_INTF_S, *PREMOTEIF_DRV_INTF_S;

ULONG IF_Register_Driver(IN ULONG ulVersion, IN const IF_DRV_INTF_S *pstIfDrvIntf);
ULONG IF_Register_RemoteIf_Driver(IN ULONG ulVersion, IN const REMOTEIF_DRV_INTF_S *pstRemoteDrvIntf);
extern VOID E1T1VI_SendE1T1VITrapTimeSlot(IN INT iIfEntry_ifIndex);

extern VOID IF_SONET_SendTrap(IN IF_INDEX ifIndex, IN ULONG ulType);

/* 预读Bri口isdn leased-line配置的接口函数 */
IF_BRI_LINE_INFO_S *IF_GetBriLineInfoFromCfg(OUT ULONG *pulNum);

#endif

#ifdef  __cplusplus
}
#endif

#endif /* _DRV_IF_H_ */


