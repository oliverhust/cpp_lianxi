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

#define IF_MAX_NAME_LEN                         47UL        /* �ӿ�ȫ����󳤶� */

/* ����С��100M�����ʣ�uiDataֱ�ӱ�ʾ���ʣ����bitλΪ0
 * ���ڴ��ڻ����100M�����ʣ�uiData����ֵ��λΪK�����bitλΪ1
 * if(0 != (uiData & IF_SPEED_HI))
 * {
 *     ʵ������ = (ulData & (~IF_SPEED_HI)) * IF_HISPEED_UNIT;
 *          ʹ��ʱ��ע�⣬ʵ�����ʿ��ܻᳬ��32bit
 * }
 */
#define IF_HISPEED_UNIT 1000U                      /* 1KΪ��λ */
#define IF_SPEED_HI     0x80000000U                /* ���bitλΪ1��ʾ��ֵ��IF_HISPEED_UNITΪ��λ */

#define IF_BAUD_10M     10000000U                  /* ʮ�ײ����� */
#define IF_BAUD_100M    100000000U                /* ���ײ����� */
#define IF_BAUD_1G      (IF_SPEED_HI | 1000000U)   /* ǧ�ײ����� */
#define IF_BAUD_10G     (IF_SPEED_HI | 10000000U)  /* ���ײ����� */
#define IF_BAUD_20G     (IF_SPEED_HI | 20000000U)  /* �����ײ����� */
#define IF_BAUD_40G     (IF_SPEED_HI | 40000000U)  /* �����ײ����� */
#define IF_BAUD_100G    (IF_SPEED_HI | 100000000U) /* ʮ���ײ����� */

/* ��/����ӿ� */
enum enPORTLAYER
{
    IF_LAYER_L2 = 1,      /* ����˿� */
    IF_LAYER_L3,          /* ����ӿ� */
    IF_LAYER_IDS          /* IDS��ȷ���� */
};

/* ��������� */
ISSU typedef enum enPHYTYPE
{
    PHY_UNKNOWN = 0,        /* 0: ��ȷ�����������ͣ�����ʱ���ûָ��� */

    /* =========== ETH��ص��������� ============ */
    PHY_ETH_10M,            /* 1: 10M��̫���� */
    PHY_ETH_FE,             /* 2: ������̫���� 100M */
    PHY_ETH_GE,             /* 3: G������̫���� GE */
    PHY_ETH_XGE,            /* 4: 10G������̫���� 10GE */
    PHY_ETH_FGE,            /* 5: 40G������̫���� */
    PHY_ETH_HGE,            /* 6: 100G������̫���� */

    /* =========== ATM��ص��������� ============ */
    PHY_ATM,                /* 7 : ATM����ڣ�ʹ��ATM��չ���ԵĲ�Ʒ��ʹ�������ϸ������ */
    PHY_ATM_25M,            /* 8 : 25M ATM�� */
    PHY_ATM_155M,           /* 9 : 155M */
    PHY_ATM_622M,           /* 10: 622M */
    PHY_ATM_ADSL,           /* 11: ADSL�ӿڣ����Գ������û��� */
    PHY_ATM_SHDSL,          /* 12: SHDSL�ӿڣ��ԳƸ��������û��� */
    PHY_ATM_E1,             /* 13: ����E1��ATM */
    PHY_ATM_E3,             /* 14: ����E1��ATM */
    PHY_ATM_T1,             /* 15: ����T1��ATM */
    PHY_ATM_T3,             /* 16: ����T3��ATM */
    PHY_ATM_IMA,            /* 17: ATM����� */
    PHY_ATM_ADSL_2PLUS,     /* 18: ADSL 2Plus�ӿ� */
    PHY_ATM_SHDSL_4WIRE,    /* 19: 4�� SHDSL ATM�� */
    PHY_ATM_SHDSL_4WIRE_BIS,/* 20: 4��G.BIS��SHDSL ATM�� */
    PHY_ATM_SHDSL_8WIRE,    /* 21: 8�� SHDSL ATM�ӿ� */
    PHY_ATM_SHDSL_8WIRE_BIS,/* 22: 8��G.BIS��SHDSL ATM�� */
    PHY_ATM_IMA_SHDSL_8WIRE,/* 23: ����8�� SHDSL ATM����� */

    /* =========== Controller��ص��������� ============ */
    PHY_FT1,                /* 24: ���ֻ���T1�� */
    PHY_T1,                 /* 25: T1�� */
    PHY_DVT1,               /* 26: ͬʱ֧���������ݵ�T1�� */
    PHY_VT1,                /* 27: ֻ֧��������T1�� */
    PHY_T3,                 /* 28: T3�� */
    PHY_CT3,                /* 29: ͨ������T3�� */
    PHY_FE1,                /* 30: ���ֻ���E1�� */
    PHY_E1,                 /* 31: E1�� */
    PHY_DVE1,               /* 32: ͬʱ֧���������ݵ�E1�� */
    PHY_VE1,                /* 33: ֻ֧��������E1�� */
    PHY_E3,                 /* 34: E3�� */
    PHY_CE3,                /* 35: ͨ������E3�� */
    PHY_E1POS,              /* 36: E1POS */
    PHY_CELLULAR,           /* 37: CELLULAR�ӿ� */

    /* =========== POS/CPOS/E-CPOS������������� ============*/
    PHY_POS,                /* 38: POS�� */
    PHY_CPOS_CE1,           /* 39: ����CE1��ͨ����POS�� */
    PHY_CPOS_CT1,           /* 40: ����CT1��ͨ����POS�� */
    PHY_CPOS_E1,            /* 41: ����E1��ͨ����POS�� */
    PHY_CPOS_E3,            /* 42: ����E3��ͨ����POS�� */
    PHY_CPOS_FE3,           /* 43: ���ڲ��ֻ�E3��POS�� */
    PHY_CPOS_T1,            /* 44: ����T1��ͨ����POS�� */
    PHY_CPOS_T3,            /* 45: ����T3��ͨ����POS�� */
    PHY_CPOS_FT3,           /* 46: ���ڲ��ֻ�T3��POS�� */
    PHY_CPOS_E3CE1,         /* 47: ����CE1E1�ںϺ�ͨ������POS�� */
    PHY_CPOS_T3CT1,         /* 48: ����CT1T1�ںϺ�ͨ������POS�� */
    PHY_CPOS_155ME3T3,      /* 49: 155ME-Cpos�ӿ� */
    PHY_CPOS_622M,          /* 50: 622M E-Cpos�ӿ� */
    PHY_CPOS_622ME3T3,      /* 51: ��ͨ������E3T3 622ME-Cpos�ӿ�*/
    PHY_CPOS_2DOT5G,        /* 52: 2.5G E-Cpos */

    /* =========== ��������ڵ��������� ============ */
    PHY_CONSOLE,            /* 53: ����̨console�� */
    PHY_ISDN,               /* 54: ISDN�� */
    PHY_PRI,                /* 55: PRI��,��Ӧ�ӿ�����Ϊͬ������ */
    PHY_SYNC,               /* 56: ͬ������ */
    PHY_VOICE_PORT,         /* 57: ������ */
    PHY_ASYNC,              /* 58: �첽���� */
    PHY_AUX,                /* 59: AUX�� */
    PHY_BCHAN,              /* 60: BRI�ڵ�B���������� */
    PHY_BRI,                /* 61: BRI�ڵ�D���������� */
    PHY_BSV,                /* 62: Bri S/T Voice�ӿ��������� */
    PHY_CASCADE,            /* 63: �ѵ��� */
    PHY_FCM,                /* 64: FCM�ӿ� */
    PHY_INASYNC,            /* 65: �ڲ�ͬ�첽���� */

    /* =========== �������߼��ڶ�Ӧ���������� ============ */
    PHY_LOGICTUNNEL,        /* 66: �߼���� */
    PHY_LOOP,               /* 67: LOOP�� */
    PHY_LOOPBACK,           /* 68: LOOPBACK�� */
    PHY_MFR,                /* 69: ����֡�м� */
    PHY_MP_GROUP,           /* 70: PPP������ */
    PHY_NULL,               /* 71: NULL�� */
    PHY_TEMPLATE,           /* 72: VTģ�� */
    PHY_VIRTUALACCESS,      /* 73: VA��   */
    PHY_TEMPLATEMPCHAN,     /* 74: MPͨ�� */
    PHY_TUNNEL,             /* 75: ����� */
    PHY_VIRTUAL_ETHERNET,   /* 76: VE�� */
    PHY_VLAN,               /* 77: VLAN��ӿ� */
    PHY_DIALER,             /* 78: ���ſ� */
    PHY_BVI,                /* 79: ��ģ�� */
    PHY_REGISTERTUNNEL,     /* 80: Register-Tunnelע��ӿ� */
    PHY_MTUNNEL,            /* 81: �ಥ����߼��ӿ� */
    PHY_HDLCB,              /* 82: HDLCB����ӿ� */
    PHY_EVI_LINK,           /* 83: EVI LINK */
    PHY_SCHANNEL,           /* 84: SCHANNEL */
    PHY_L2TP,               /* 85: Virtual-PPP�ӿ� */
 
    /* =========== ���ܿ���ص��������� ============ */
    PHY_NDEC,               /* 86: ���ܿ�1 */
    PHY_CAVIUM,             /* 87: ���ܿ�2 */
    PHY_SSLVPN,             /* 88: SSLVPN�ӿ��������� */
    PHY_SSL_ENC,            /* 89: SSL���ͼ��ܿ� */
    PHY_IPSEC_ENC,          /* 90: IPSEC���ͼ��ܿ� */

    /* BEGIN L3+ */
    PHY_NAT,                /* 91: L3+ NATҵ��ӿ� */
    PHY_EACL,               /* 92: L3+ EACLҵ��ӿ� */
    PHY_NETSTREAM,          /* 93: L3+ NetStreamҵ��ӿ� */
    PHY_URPF,               /* 94: L3+ URPFҵ��ӿ� */
    PHY_MPLS_VPLS,          /* 95: L3+ MPLS-L2VPN-VPLSҵ��ӿ� */
    PHY_MPLS_VLL,           /* 96: L3+ MPLS-L2VPN-VLLҵ��ӿ� */
    PHY_MPLS_L3VPN,         /* 97: L3+ MPLS-L3VPNҵ��ӿ� */
    PHY_MPLS,               /* 98: L3+ ��ͨMPLSҵ��ӿ� */
    /* END   L3+ */

    /* BEGIN RPR */
    PHY_RPR_GE,             /* 99: G���� RPR�߼��� */
    PHY_RPR_XGE,            /* 100: 10G���� RPR�߼��� */
    PHY_RPR_XGPOS,          /* 101: 10G POS RPR�߼��� */
    PHY_RPR_2DOT5GPOS,      /* 102: 2.5G POS RPR�߼��� */
    PHY_RPR_PHY_GE,         /* 103: G���� RPR����� */
    PHY_RPR_PHY_XGE,        /* 104: 10G���� RPR����� */
    PHY_RPR_PHY_XGPOS,      /* 105: 10G POS RPR����� */
    PHY_RPR_PHY_2DOT5GPOS,  /* 106: 2.5G POS RPR����� */
    /* END   PRR */

    /* BEGIN: VOICE */
    PHY_CAS,                /* 107: ��DVE1����VE1�ӿ���ͨ��Timeslot-set��������ýӿ����� */
    PHY_CCS,                /* 108: ��DVE1����VE1�ӿ���ͨ������PRI-SET�����ĸ������������õ�ʱ϶��ӿ����� */
    PHY_FXS,                /* 109: Fxs */
    PHY_FXO,                /* 110: Fxo */
    PHY_AEM,                /* 111: ģ��em */
    PHY_VTS,                /* 112: ������ʹ�õ�Bͨ���ӿ����� */
    PHY_BSV_CCS,            /* 113: Bri S/T Voice �ӿڶ�Ӧ�������û������ӿ� */
    /* END: VOICE */

    /* Begin: WLAN ��ص��������� */
    PHY_RADIO,              /* 114: WLAN-Radio�ӿ� */
    PHY_WLAN_ESS,           /* 115: ���߽���������WLANģ���߼��� */
    PHY_WLAN_DBSS,          /* 116: ���߽���������WLAN������ӿ� */
    PHY_WLAN_BSS,           /* 117: ����·��������WLAN������ӿ� */
    PHY_WLAN_ETHERNET,      /* 118: ����·��������WLAN������ӿ� */
    PHY_WLAN_TUNNEL,        /* 119: ���߲�Ʒ����tunnel��ӿ� */
    PHY_WLAN_MESHLINK,      /* 120: ����WLAN-MESH����� */
    PHY_WLAN_MESH,          /* 121: ���߲�ƷWLAN-MESH����ģ����ӿ� */
    /* End: WLAN ��ص��������� */

    /* BEGIN: ONU/OLT */
    PHY_OLT_64,             /* 122: �ֹ��Ϊ1:64��OLT(Optical Line Terminal)�ӿ� */
    PHY_ONU,                /* 123: ONU(Optical Network Unit)�ӿ� */
    /* END: ONU/OLT */

    /* BEGIN: LAGG */
    PHY_RAGG,               /* 124: ����ۺϽӿ� */
    PHY_BAGG,               /* 125: ����ۺϽӿ� */
    /* END: LAGG */

    /* BEGIN: FC */
    PHY_FC,                 /* 126: FC */
    PHY_VFC,                /* 127: VFC */
    PHY_FCB,                /* 128: FCB */
    /* END: FC */

    PHY_FC_XGE,             /* 129: ���л�FC��XGE*/

    PHY_ETH_TGE,            /* 130: 20G������̫���� */
    /*========= PE���п�=====================*/
    PHY_PEGE,               /* 131: PE��GE���п�*/
    PHY_PEXGE,              /* 132: PE��XGE���п�*/
    PHY_PEFGE,              /* 133: PE��FGE���п�*/
    PHY_PEHGE,              /* 134: PE��HGE���п�*/
    PHY_BLADE,              /* 135: ����� */
    PHY_BLAGG,              /* 136: ����ۺϿ� */

    /* ETH����� */
    PHY_BETH,               /* 137: ����ETH����� */
    PHY_RETH,               /* 138: ����ETH����� */
    PHY_VSI,                /* 139: ȫ��VSI�ڣ�MPLS��ʹ�� */
    PHY_CEM,                /* 140: CEM���������� */
    PHY_INCOMM,             /* 141: �ڲ�ͨ�ſ� */
    PHY_E1DM,               /* 142: E1DM */
    PHY_E1POSDM,            /* 143: E1POSDM */
    PHY_RPR_PHY_L2LOCAL,    /* 144: ����RPR�ֲ��� */
    PHY_SCHBDL,             /* 145: Schannle-Bundle�� */
    PHY_CMTUNNEL,           /* 146: CMTunnel���������� */
    PHY_BUTT = 0xffff       /* �����ڵ��������� */
}IF_PHYTYPE_E;

/* �ӿ����� */
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

/* �ӿ����������ַ���������Ʒ��װ�ӿ����� */
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
uiProperty �ӿ����ԣ���3λ��ʾ�ӿڶ����������ͣ��ӵ�4λ��ʼ��ʾ�ӿ�Ӳ������
���ƣ�0��ʾû�����ƣ�����˵�����£�
[0]:���һλ��ʾ�ӿ��Ƿ���Խ��ж����������ͼ��л���1��ʾ�����л���0��ʾ�����ԡ�
[1-2]: ��ʾ�ӿ�ȱʡ�Ķ����������͡�01��ʾȱʡΪ����ӿڣ�10��ʾȱʡΪ����ӿڣ�
       00��ʾ��ȫ���������ƵĽӿڣ�Ŀǰδʹ�ã�11Ԥ����
[3]:IPV4�������ƣ�  1����������IPV4������0��ʾ��
[4]:IPV6�������ƣ�  1����������IPV6������0��ʾ��
[5]:TUNNEL�������ƣ�1��ʾ���������������0��ʾ��
[6]:MPLS�������ƣ�  1��ʾ������MPLS������0��ʾ��
[7]:IPX�������ƣ�   1��ʾ������IPX������0��ʾ��
[8]:�ӽӿڴ������ƣ�1��ʾ���ܴ����ӽӿڣ�0��ʾ���Դ����ӽӿ�
[9]:����ӿ��Ƿ��ܴ��������ӽӿڣ�Ĭ�ϲ��ܴ��������Ϻ��ܴ���
[10]:�鲥VPN�������ƣ�1��ʾ֧���鲥VPN���ܣ�0��ʾ��֧��
[11]:��ǰ�ӿ��Ƿ���˫���ȱ��ӿڡ����ǣ�ƽ̨�Ὣ�ýӿ����óɲ��ɼ������䡣
[12-13]:��ʾ�ӿ���MDC�еķ���������0��ʾ���ܷ��䣬1��ʾ��ռ���䣬2��ʾ�������

���Ϸֱ��Ӧ����ĺ꣺
*/
#define IF_PORT_NOSWITCH            ((UINT)0x0000)      /* 0: ��֧�ֶ������л������������з���̫���ӿ� */
#define IF_PORT_L2L3_SWITCH         ((UINT)0x0001)      /* 0: ֧�ֶ������л���ֻ��������̫����ӿ� */
#define IF_PORT_DEFAULT_L2          ((UINT)0x0002)      /* 1: ȱʡΪ����ӿڣ�ֻ��������̫����ӿ� */
#define IF_PORT_DEFAULT_L3          ((UINT)0x0004)      /* 2: ȱʡΪ����ӿڣ���IF_PORT_DEFAULT_L2���� */
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
#define IF_PORT_COMBO_FIBER         ((UINT)0x4000)      /* 14: combo���ģʽ */
#define IF_PORT_COMBO_COPPER        ((UINT)0x8000)      /* 15: combo���ģʽ */
#define IF_PORT_FCOE                ((UINT)0x10000)     /* 16: FCOE���� */
#define IF_PORT_TRILL               ((UINT)0x20000)     /* 17: TRILL���� */
#define IF_PORT_FC                  ((UINT)0x40000)     /* 18: FC�л�����*/   
#define IF_PORT_SPLIT_FROM_FGE      ((UINT)0x80000)     /* 19: �ӿڴ�40GE�ڷ��Ѷ��� */
#define IF_PORT_SPLIT_FROM_HGE      ((UINT)0x100000)    /* 20: �ӿڴ�100GE�ڷ��Ѷ��� */
#define IF_PORT_AUTOCFG             ((UINT)0x200000)    /* 21: �ӿ�Ϊautocfg����������*/
#define IF_PORT_CONNECTION_MODE     ((UINT)0x400000)    /* 22: �ӿ�֧��ACFP������չģʽ */
#define IF_PORT_LIMITATIONS_MULTIPORT  ((UINT)0x800000) /* 23 */
#define IF_PORT_LIMITATIONS_VSIGATEWAY ((UINT)0x1000000)/* 24 */

/* IF_PORT_INFO_S.usChkSumAbility ȡֵ��mbuf.h�ж���
   ���ֶζ����˽ӿڵ�CheckSumӲ������������������λ��ʾ��UDP��TCP�ȱ��ĵ�У��ͼ�������
   �ϲ�ģ�����ͨ��IF_GetDataByIfIndex�ӿ�IF_PORT_CHECKSUM�����ֲ�ѯ���ֶ�
0��ʾ��֧��Ӳ������CheckSum��
�������ڽӿ�ʵ�����ڰ壨�ӿڰ塢���ذ塢����ȣ���ȡ
*/

/* ������������Ϣ����ߴ� */
#define IF_DRVCTX_SIZE 2

ISSU typedef struct tagIfPortInfo
{
    IF_CREATE_COMPLETE_ROUTINE_PF pfIfCreateCompleteRoutine;    /* �ӿڴ����ص����� */
    UINT64   aui64DrvContext[IF_DRVCTX_SIZE];           /* ���������� */
    IF_INDEX ifindex;                                   /* �ӿ����� */
    UINT     uiIfType;                                  /* �ӿ����� */
    UINT     uiProperty;                                /* �ӿ����� */
    UINT     uiMDCIntfGroup;                            /* �ӿ�������� */
    UINT     uiPort;                                    /* �ӿڱ�� */
    USHORT   usSlot;                                    /* �ۺ� */
    USHORT   usSubSlot;                                 /* �ӿ��� */
    USHORT   usPhyType;                                 /* �������� */
    USHORT   usChkSumAbility;                           /* Ӳ��У��ͼ������� */
    CHAR     szPortName[IF_MAX_NAME_LEN + 1];           /* �ӿ����� */
}IF_PORT_INFO_S, *PIF_PORT_INFO_S;

/* �ӿڱ��Ļ�����Ϣ�������ӿ����ݽṹ */
ISSU typedef struct tagIFBufferInfo
{
    INT    iPktBufFree;          /* ��ȡ���Ļ������ֵ */
    INT    iPktBufInit;          /* ��ȡ���Ļ����ʼֵ */
    INT    iPktBufMin;           /* ��ȡ���Ļ�����Сֵ */
    UINT64 uiPktBufMiss;         /* ��ȡ���Ķ���ֵ */
}IF_BUFFER_INFO_S;

/* ������IOCTL_COMMON_BLOCK/IOCTL_COMMON_UNDO_BLOCKʹ�� */
#define IF_BLOCKTYPE_DLDP       0x1UL
#define IF_BLOCKTYPE_ETHOAM     0x2UL
#define IF_BLOCKTYPE_MACSEC     0x3UL

/* oamʹ�� */
typedef struct tagIF_PORT_INFO_EXT_S
{
    UINT64 aui64DrvContext[IF_DRVCTX_SIZE]; /* ���������� */
    IF_INDEX ifindex;                       /* �ӿ����� */
    UINT uiIfType;                          /* �ӿ����� */
    UINT uiMDCNo;                           /* �ӿ�������MDC */
    UINT uiPhyNo;                           /* ������ */
    USHORT usLogicSlotNo;                   /* �߼���� */
    USHORT usPhySlotNo;                     /* ������ */
    USHORT usSubSlot;                       /* �ӿ��� */
    USHORT usSubNo;                         /* �ӽӿں� */
    USHORT usSubType;                       /* �ӽӿ����� */
}IF_PORT_INFO_EXT_S, *PIF_PORT_INFO_EXT_S;

typedef struct tagLINK_ENCAP_S
{
    UINT uiProtoType;
    VOID *pLinkEncapParam;
}LINK_ENCAP_S;

/* ����alloc������������� */
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

/* ����PhyIOCtl�����ֵĺ� */
#define IF_IOCTL_CMD_DEF(ModId,IfType,Index) ((ModId) | ((IfType) << 8) | (Index))

/* ���������� */
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
#define IOCTL_COMMON_SET_UP_MODE                 IF_IOCTL_CMD_DEF(MID_IFNET, IF_COMMON_TYPE, 52)/* ���ö�������̫���ڵ�PORTUPMODE ENABLE */
#define IOCTL_COMMON_AUTO_POWER_DOWN             IF_IOCTL_CMD_DEF(MID_IFNET, IF_COMMON_TYPE, 53)/* ���ö�������̫���ڵĽӿ��Զ�DOWN ��ENABLE */
#define IOCTL_COMMON_GET_PAYLOAD                 IF_IOCTL_CMD_DEF(MID_IFNET, IF_COMMON_TYPE, 54)
#define IOCTL_COMMON_GET_INERRORPERCENT          IF_IOCTL_CMD_DEF(MID_IFNET, IF_COMMON_TYPE, 55)
#define IOCTL_COMMON_SET_EEEENABLE               IF_IOCTL_CMD_DEF(MID_IFNET, IF_COMMON_TYPE, 56)
#define IOCTL_COMMON_LINK_ENCAP_CHECK            IF_IOCTL_CMD_DEF(MID_IFNET, IF_COMMON_TYPE, 57)
#define IOCTL_COMMON_LINK_DECAP_CHECK            IF_IOCTL_CMD_DEF(MID_IFNET, IF_COMMON_TYPE, 58)
#define IOCTL_COMMON_GET_BER_DEFAULTTHRESHOLD    IF_IOCTL_CMD_DEF(MID_IFNET, IF_COMMON_TYPE, 59)/* ��ȡXGE/CPOS/EPOS/ATM/RPR��thresholdֵ */
#define IOCTL_COMMON_SET_BER_THRESHOLD           IF_IOCTL_CMD_DEF(MID_IFNET, IF_COMMON_TYPE, 60)/* ����XGE/CPOS/EPOS/ATM/RPR��thresholdֵ */
#define IOCTL_COMMON_SET_AUTOCLOCKCHANGE         IF_IOCTL_CMD_DEF(MID_IFNET, IF_COMMON_TYPE, 61)/* ����clock-change auto */
#define IOCTL_COMMON_SET_AUTO_DOWN               IF_IOCTL_CMD_DEF(MID_IFNET, IF_COMMON_TYPE, 62)
#define IOCTL_COMMON_SWITCH_RELEASE              IF_IOCTL_CMD_DEF(MID_IFNET, IF_COMMON_TYPE, 63)
#define IOCTL_COMMON_GET_PORTENVINFO             IF_IOCTL_CMD_DEF(MID_IFNET, IF_COMMON_TYPE, 64)
#define IOCTL_COMMON_SET_SERVICESLOT             IF_IOCTL_CMD_DEF(MID_IFNET, IF_COMMON_TYPE, 65)  /* service slot */
#define IOCTL_COMMON_DELETE_PREPROC              IF_IOCTL_CMD_DEF(MID_IFNET, IF_COMMON_TYPE, 66)
#define IOCTL_COMMON_SWITCHTYPE                  IF_IOCTL_CMD_DEF(MID_IFNET, IF_COMMON_TYPE, 67)
#define IOCTL_COMMON_GET_IFBUFFERINFO            IF_IOCTL_CMD_DEF(MID_IFNET, IF_COMMON_TYPE, 68)/* ��ȡ�ӿ��ϱ��Ļ�����Ϣ */
#define IOCTL_COMMON_GET_ENTPHYID                IF_IOCTL_CMD_DEF(MID_IFNET, IF_COMMON_TYPE, 69)/* ��ȡ�ӿ�������(�ӿ��ڱ��) */
#define IOCTL_COMMON_SET_ALARMACTION             IF_IOCTL_CMD_DEF(MID_IFNET, IF_COMMON_TYPE, 70)/* set alarm-detect action */
#define IOCTL_COMMON_GET_ONLINESTATE             IF_IOCTL_CMD_DEF(MID_IFNET, IF_COMMON_TYPE, 71)/* ��ȡonline/offline״̬ */
#define IOCTL_COMMON_SET_CVTHRESHOLDB1           IF_IOCTL_CMD_DEF(MID_IFNET, IF_COMMON_TYPE, 72) /* ���� threshold B1 ��ֵ */
#define IOCTL_COMMON_SET_CVTHRESHOLDB2           IF_IOCTL_CMD_DEF(MID_IFNET, IF_COMMON_TYPE, 73) /* ���� threshold B2 ��ֵ */
#define IOCTL_COMMON_SET_CVTHRESHOLDB3           IF_IOCTL_CMD_DEF(MID_IFNET, IF_COMMON_TYPE, 74) /* ���� threshold B3 ��ֵ */
#define IOCTL_COMMON_GET_SONETMIBINFO            IF_IOCTL_CMD_DEF(MID_IFNET, IF_COMMON_TYPE, 75) /* ��ȡsonet��ؼ�����Ϣ */
#define IOCTL_COMMON_GET_MTURANGE                IF_IOCTL_CMD_DEF(MID_IFNET, IF_COMMON_TYPE, 76) /* ��ȡMTU�ķ�Χ */
#define IOCTL_COMMON_GET_IPV4_STATISTICS         IF_IOCTL_CMD_DEF(MID_IFNET, IF_COMMON_TYPE, 77)
#define IOCTL_COMMON_GET_IPV6_STATISTICS         IF_IOCTL_CMD_DEF(MID_IFNET, IF_COMMON_TYPE, 78)
#define IOCTL_COMMON_GET_LSWPORTTABLE_INFO       IF_IOCTL_CMD_DEF(MID_IFNET, IF_COMMON_TYPE, 79) /* ��ȡ�˿����������Լ��˿ڹ�ģ������� */

/* ��̫��PhyIOCtl������ */
#define IOCTL_ETHERNET_SET_SPEED                 IF_IOCTL_CMD_DEF(MID_IFNET, IF_L2ETHERNET_TYPE, 0) /* ����ָ���˿ڵ�����*/
#define IOCTL_ETHERNET_SET_DUPLEX                IF_IOCTL_CMD_DEF(MID_IFNET, IF_L2ETHERNET_TYPE, 1) /* ����ָ���˿ڵ�˫�� */
#define IOCTL_ETHERNET_GET_SPEED                 IF_IOCTL_CMD_DEF(MID_IFNET, IF_L2ETHERNET_TYPE, 2) /* ��ȡָ���˿ڵ�ʵ������ */
#define IOCTL_ETHERNET_GET_DEFAULT_SPEED         IF_IOCTL_CMD_DEF(MID_IFNET, IF_L2ETHERNET_TYPE, 3) /* ���ָ���˿ڵ�ȱʡ���� */
#define IOCTL_ETHERNET_GET_DUPLEX                IF_IOCTL_CMD_DEF(MID_IFNET, IF_L2ETHERNET_TYPE, 4) /* ��ȡָ���˿ڵ�˫��״̬ */
#define IOCTL_ETHERNET_GET_DEFAULT_DUPLEX        IF_IOCTL_CMD_DEF(MID_IFNET, IF_L2ETHERNET_TYPE, 5) /* ��ȡָ���˿ڵ�ȱʡ˫��״̬ */
#define IOCTL_ETHERNET_GET_DEFAULT_MDIX          IF_IOCTL_CMD_DEF(MID_IFNET, IF_L2ETHERNET_TYPE, 6) /* ��ȡָ���˿ڵ�ȱʡMDIXֵ */
#define IOCTL_ETHERNET_SET_MDIX                  IF_IOCTL_CMD_DEF(MID_IFNET, IF_L2ETHERNET_TYPE, 7) /* ����ָ���˿ڵ�MDIXֵ */
#define IOCTL_ETHERNET_GET_MTU                   IF_IOCTL_CMD_DEF(MID_IFNET, IF_L2ETHERNET_TYPE, 8) /* ��ȡָ���˿ڵ�MTU */
#define IOCTL_ETHERNET_GET_AUTO_SPEED_CAPABILITY IF_IOCTL_CMD_DEF(MID_IFNET, IF_L2ETHERNET_TYPE, 9) /* ��ȡָ���˿ڵ�MTU */
#define IOCTL_ETHERNET_IS_LOOPBACK_SUPPORT       IF_IOCTL_CMD_DEF(MID_IFNET, IF_L2ETHERNET_TYPE, 10)/* ��ȡĳһ���˿��Ƿ�֧�ֶ˿ڻػ����� */
#define IOCTL_ETHERNET_LOOPBACK                  IF_IOCTL_CMD_DEF(MID_IFNET, IF_L2ETHERNET_TYPE, 11)/* ����/ȡ��ָ���˿ڵĻ��ع��� */
#define IOCTL_ETHERNET_XGE_LAN_WAN_MODE          IF_IOCTL_CMD_DEF(MID_IFNET, IF_L2ETHERNET_TYPE, 12)/* ����10GE�ӿڹ���ģʽΪLAN����WAN */
#define IOCTL_ETHERNET_GET_MAC_ADDRESS           IF_IOCTL_CMD_DEF(MID_IFNET, IF_L2ETHERNET_TYPE, 13)
#define IOCTL_ETHERNET_GET_PAD_CAPABILITY        IF_IOCTL_CMD_DEF(MID_IFNET, IF_L2ETHERNET_TYPE, 14)
#define IOCTL_ETHERNET_SET_FLOW_INTERVAL         IF_IOCTL_CMD_DEF(MID_IFNET, IF_L2ETHERNET_TYPE, 15)
#define IOCTL_ETHERNET_GET_STATISTICS            IF_IOCTL_CMD_DEF(MID_IFNET, IF_L2ETHERNET_TYPE, 16)
#define IOCTL_ETHERNET_GET_STATISTICS_ALL        IF_IOCTL_CMD_DEF(MID_IFNET, IF_L2ETHERNET_TYPE, 17)/* ��ȡָ���˿ڵ�����ͳ����Ϣ */
#define IOCTL_ETHERNET_SET_JUMBOFRAME            IF_IOCTL_CMD_DEF(MID_IFNET, IF_L2ETHERNET_TYPE, 18)/* ���ö�������̫���ڵ�JUMBOFRAME */
#define IOCTL_ETHERNET_GET_JUMBOFRAME_RANGE      IF_IOCTL_CMD_DEF(MID_IFNET, IF_L2ETHERNET_TYPE, 19)/* ��ȡ��������̫���е�JUMBOFRAME��Χ */
#define IOCTL_ETHERNET_COMBO_SWITCH              IF_IOCTL_CMD_DEF(MID_IFNET, IF_L2ETHERNET_TYPE, 20)/* ����combo�ڵĹ���ģʽ */
#define IOCTL_ETHERNET_SET_STORM_CONTROL         IF_IOCTL_CMD_DEF(MID_IFNET, IF_L2ETHERNET_TYPE, 21)/* ���ö�����̫���ڵķ籩����ֵ */
#define IOCTL_ETHERNET_GET_STORM_CONTROL         IF_IOCTL_CMD_DEF(MID_IFNET, IF_L2ETHERNET_TYPE, 22)/* ��ȡ������̫���ڵķ籩����ֵ */
#define IOCTL_ETHERNET_PRIORITY_FLOW_CONTROL     IF_IOCTL_CMD_DEF(MID_IFNET, IF_L2ETHERNET_TYPE, 23)/* ���ö˿��������ȼ� */
#define IOCTL_ETHERNET_SET_PORTISOLATE           IF_IOCTL_CMD_DEF(MID_IFNET, IF_L2ETHERNET_TYPE, 24)
#define IOCTL_ETHERNET_SET_STATISTICS            IF_IOCTL_CMD_DEF(MID_IFNET, IF_L2ETHERNET_TYPE, 25)
#define IOCTL_ETHERNET_SET_STORM_BLOCK           IF_IOCTL_CMD_DEF(MID_IFNET, IF_L2ETHERNET_TYPE, 26)
#define IOCTL_ETHERNET_FLOW_CONTROL              IF_IOCTL_CMD_DEF(MID_IFNET, IF_L2ETHERNET_TYPE, 27)/* ���ö˿�flow-control����ģʽ */
#define IOCTL_ETHERNET_SET_BPDUDROP              IF_IOCTL_CMD_DEF(MID_IFNET, IF_L2ETHERNET_TYPE, 28)
#define IOCTL_ETHERNET_VIRTUAL_CABLE_TEST        IF_IOCTL_CMD_DEF(MID_IFNET, IF_L2ETHERNET_TYPE, 29)/* VCT��ȡ������Ϣ*/
#define IOCTL_ETHERNET_IFEXTTRPS                 IF_IOCTL_CMD_DEF(MID_IFNET, IF_L2ETHERNET_TYPE, 30)/* ���ýӿڸ澯��ֵ */
#define IOCTL_ETHERNET_GET_LANWANINFO            IF_IOCTL_CMD_DEF(MID_IFNET, IF_L2ETHERNET_TYPE, 31)/* ��ȡXGE��LAN/WAN��Ϣ */
#define IOCTL_ETHERNET_GET_RMON_STATISTICS       IF_IOCTL_CMD_DEF(MID_IFNET, IF_L2ETHERNET_TYPE, 33)/* ��ȡRMONͳ����Ϣ */
#define IOCTL_ETHERNET_SET_PORT_CONNECTIONMODE   IF_IOCTL_CMD_DEF(MID_IFNET, IF_L2ETHERNET_TYPE, 34)/* ����port connection-mode */
#define IOCTL_ETHERNET_GET_DEFAULT_MACADDRESS    IF_IOCTL_CMD_DEF(MID_IFNET, IF_L2ETHERNET_TYPE, 35)

/* ������̫��PhyIOCtl������ */
#define IOCTL_ETHERNET_SET_PROMISCUOUS_MODE      IF_IOCTL_CMD_DEF(MID_IFNET, IF_L3ETHERNET_TYPE, 0) /* ������̫����Ϊ���ӹ���ģʽ/��ͨ����ģʽ */

/* FC�ӿ�PhyIOCtl������ */
#define IOCTL_FC_SET_SPEED                      IF_IOCTL_CMD_DEF(MID_IFNET, IF_FC_TYPE, 0)     /* ����ָ���˿ڵ�����*/
#define IOCTL_FC_GET_SPEED                      IF_IOCTL_CMD_DEF(MID_IFNET, IF_FC_TYPE, 4)     /* ��ȡָ���˿ڵ����� */
#define IOCTL_FC_GET_DEFAULTSPEED               IF_IOCTL_CMD_DEF(MID_IFNET, IF_FC_TYPE, 7)     /* ��ȡָ���˿ڵ����� */

/* 40G�ӿڷ���PhyIOCtl������ */
#define IOCTL_FGE_USINGXGE                      IF_IOCTL_CMD_DEF(MID_IFNET, IF_L2FGE_TYPE, 0)  /* ʹ���ĸ�10G�� */
#define IOCTL_FGE_USINGFGE                      IF_IOCTL_CMD_DEF(MID_IFNET, IF_L2FGE_TYPE, 1)  /* ʹ��һ��40G�� */
#define IOCTL_FGE_USINGHGE                      IF_IOCTL_CMD_DEF(MID_IFNET, IF_L2FGE_TYPE, 2)  /* ʹ��һ��100G�� */
#define IOCTL_FGE_USINGTGE                      IF_IOCTL_CMD_DEF(MID_IFNET, IF_L2FGE_TYPE, 3)  /* ʹ������20G�� */

/* 100G�ӿڷ���PhyIOCtl������ */
#define IOCTL_HGE_USINGXGE                      IF_IOCTL_CMD_DEF(MID_IFNET, IF_L2HGE_TYPE, 0)  /* ʹ��ʮ��10G�� */
#define IOCTL_HGE_USINGHGE                      IF_IOCTL_CMD_DEF(MID_IFNET, IF_L2HGE_TYPE, 1)  /* ʹ��һ��100G�� */
#define IOCTL_HGE_USINGFGE                      IF_IOCTL_CMD_DEF(MID_IFNET, IF_L2HGE_TYPE, 2)  /* ʹ��һ��40G�� */

/* ���ѡ��ϲ��Ƿ���Ҫ������Ĭ����Ҫ���� */
#define IF_USING_NEEDREBOOT     0
#define IF_USING_NOREBOOT       1

/* ATM �ӿڵ����������� */
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

/* BLADE �ӿڵ�����������  */
/* IOCTL_BLADE_GETTYPE pData����ΪULONG *�����IF_BLADETYPE_E���� */
#define IOCTL_BLADE_GETTYPE                     IF_IOCTL_CMD_DEF(MID_IFNET, IF_BLADE_TYPE, 0) 

/* ������㵽��·��������¼� */
ISSU typedef enum enumIf_LinkUp
{
    IF_LINKUP_UP = 0,               /* 0: �˿�UP */
    IF_LINKUP_DOWN,                 /* 1: �˿�DOWN */
    IF_LINKUP_BAUD_CHANGED ,        /* 2: �˿ڲ����ʱ仯 */
    IF_LINKUP_ASY_TO_SYN ,          /* 3: �˿����첽ģʽ���ͬ��ģʽ */
    IF_LINKUP_SYN_TO_ASY ,          /* 4: �˿���ͬ��ģʽ����첽ģʽ */
    IF_LINKUP_SET_MAC ,             /* 5: ����MAC��ַ������PPPoE�Ƚӿ� */
    IF_LINKUP_SPEED_CHANGED,        /* 6: �˿����ʱ仯 */
    IF_LINKUP_DUPLEX_CHANGED,       /* 7: �˿�˫���仯 */
    IF_LINKUP_ACTIVE,               /* 8: ��·������ */
    IF_LINKUP_DEACTIVE,             /* 9: ��·��ȥ���� */
    IF_LINKUP_ATM_OAMSTATE,         /* 10: ATM OAM״̬�ϱ� */
    IF_LINKUP_ATM_OAMPINGRECEVIED,  /* 11: OAM PING����ϱ� */
    IF_LINKUP_ATM_EOA_MAPDOWN,      /* 12: ATM PVCӳ���ϵʧЧ, Ŀǰֻ����XoEoA */
    IF_LINKUP_E1VOICE,              /* 13: E1VI�������� */
    IF_LINKUP_4VIVOICE,             /* 14: E1VI�������� */
    IF_LINKUP_WORKPROTOCOLDOWN,     /* 15: L2VPN��ƭ��·��ģ������DOWN */
    IF_LINKUP_WORKPROTOCOLUP,       /* 16: L2VPN�ָ������UP */
    IF_LINKUP_CABLECHANGE,          /* 17: ͬ�첽���ڵײ���½�ͷ�����ı�(DTE/DCEC) */
    IF_LINKUP_IPSEC_RESET,          /* 18: IPsec���ܿ��ӿ������ݻָ�Ϊ��ʼ״̬ */
    IF_LINKUP_SMOOTHUP,             /* 19: �����ƽ��UP */
    IF_LINKUP_RPR_MISCABLE_STATUS,  /* 20: RPR�������״̬ */
    IF_LINKUP_RPR_CONGEST_STATUS,   /* 21: RPRӵ��״̬ */
    IF_LINKUP_RPR_PROTECT_CHANGE,   /* 22: RPR����ģʽ�ı� */
    IF_LINKUP_FASTEV,               /* 23: �˿�UP/DOWN�����ϱ� */
    IF_LINKUP_ACTIVE_FAILED,        /* 24: ��·����ʧ�� */
    IF_LINKUP_RPR_MATE_STATUS,      /* 25: MATE��״̬�仯 */
    IF_LINKUP_ENCAP_CHANGE,         /* 26: ����㱨�ķ�װ��ʽ�����仯 */
    IF_LINKUP_AGG_JOIN,             /* 27: �˿ڼ���ۺ��� */
    IF_LINKUP_AGG_LEAVE,            /* 28: �˿��˳��ۺ��� */
    IF_LINKUP_SERVICESLOT,          /* 29: service slot */
    IF_LINKUP_ACTIVE_IN_PROGRESS,   /* 30: BRI�ڼ�������� */
    IF_LINKUP_BBCREDIT_CHANGED,     /* 31: �˿�BBCREDIT�仯 */
    IF_LINKUP_BITERROR_CHANGED,     /* 32: �˿�BITERROR�仯 */
    IF_LINKUP_VSAN_UP,              /* 33: FC�ӿ�VSAN��UP */
    IF_LINKUP_VSAN_DOWN,            /* 34: FC�ӿ�VSAN��DOWN */
    IF_LINKUP_SET_DEFAULT_MAC,      /* 35: ����ȱʡMAC��ַ, �ѵ�����ʱ�����ϱ�ƽ̨*/
    IF_LINKUP_LINEPROTOCOL_UP,      /* 36: ��·��Э��UP */
    IF_LINKUP_LINEPROTOCOL_DOWN,    /* 37: ��·��Э��DOWN */
    IF_LINKUP_DAMP_UP,              /* 38: ��·DAMPUP */
    IF_LINKUP_DAMP_DOWN,            /* 39: ��·DAMPDOWN */
    IF_LINKUP_TUNNEL_IPSEC_UP,      /* 40: Tunnel��·��Э��IPsec UP */
    IF_LINKUP_TUNNEL_IPSEC_DOWN,    /* 41: Tunnel��·��Э��IPsec Down */ 
    IF_LINKUP_RADIO_IDLETIMEREACH,  /* 42: ����ʱ�䵽�� */
    IF_LINKUP_RADIO_RADARDETECTED,  /* 43: �ڵ�ǰ�����ŵ���⵽�״� */
    IF_LINKUP_NETWORK_SWITCH,       /* 44: 2G/3G/4G�����л� */
    IF_LINKUP_HANGUP,               /* 45: ISDN E1 PRIʱ϶�Ҷ� */
    IF_LINKUP_CUPID_RESP,           /* 46: ���߶�λ��������ϱ� */
    IF_LINKUP_MAXCMD
}IF_LINKUP_E;

/* �жϵײ��Ƿ�֧�ֶԸ����ͳ��,ֻ�и���ֵ��ΪIF_PORT_STATISTIC_NOT_SUPPORTʱ,��Ϊ��֧�� */
#define IF_PORT_STATISTIC_NOT_SUPPORT ((UINT64)(-1))

ISSU typedef struct tagPeakSpeed
{
    UINT64    ui64SpeedPeakByte;
    DATE_S    stSpeedPeakDate;
    TIME_S    stSpeedPeakTime;
}IF_PEAKSPEED_S;

/* ��̫��ͳ����Ϣ */
ISSU typedef struct  tagIFLanPortPktStatics
{
    IF_PEAKSPEED_S stInSpeedPeak;       /* �����ֵ��Ϣ */
    IF_PEAKSPEED_S stOutSpeedPeak;      /* �����ֵ��Ϣ */
    UINT64   ui64InByte;                /* ���뱨���ֽ��� */
    UINT64   ui64InPkt;                 /* ���뱨���� */
    UINT64   ui64InUcastPkt;            /* ���뵥���� */
    UINT64   ui64InBrdcastPkt;          /* ����㲥�� */
    UINT64   ui64InMuticastPkt;         /* ����ಥ�� */
    UINT64   ui64InPause;               /* ����pause ֡ */
    UINT64   ui64InNormalByte;          /* �������б����ֽ��� */
    UINT64   ui64InNormalBrdcastByte;   /* ����㲥�����ֽ��� */
    UINT64   ui64InNormalMuticastByte;  /* �����鲥�����ֽ��� */
    UINT64   ui64InNormalUnicastByte;   /* �����������������ֽ��� */
    UINT64   ui64InUnknownUnicastByte;  /* ����δ֪���������ֽ��� */
    UINT64   ui64InNormalPkt;
    UINT64   ui64InNormalUcastPkt;      /* �������������� */
    UINT64   ui64InUnknownUcastPkt;     /* ����δ֪���������� */
    UINT64   ui64InNormalBrdcastPkt;
    UINT64   ui64InNormalMuticastPkt;
    UINT64   ui64InNormalPause;         /* ��������pause ֡�� */                /* Modified by zhangzhang05015, 2009-01-17 of LSD34939 */
    UINT64   ui64InPktErr;
    UINT64   ui64InSpeedPkt;            /* ��������� */
    UINT64   ui64InSpeedByte;
    UINT64   ui64InErrPkt;              /* �������� */
    UINT64   ui64InRunt;
    UINT64   ui64InGiant;
    UINT64   ui64InThrottle;
    UINT64   ui64InErrCRC;
    UINT64   ui64InFrame;
    UINT64   ui64InOverrun;
    UINT64   ui64InAbort;
    UINT64   ui64InIgnore;
    UINT64   ui64InParity;
    UINT64   ui64InSpeedBrdcastPkt;     /* ����㲥������ */
    UINT64   ui64InSpeedMulticastPkt;   /* �����鲥������ */
    UINT64   ui64OutByte;
    UINT64   ui64OutPkt;                /* ��������� */
    UINT64   ui64OutUcastPkt;           /* ��������� */
    UINT64   ui64OutBrdcastPkt;         /* ����㲥�� */
    UINT64   ui64OutMuticastPkt;        /* ����鲥�� */
    UINT64   ui64OutPause;
    UINT64   ui64OutNormalByte;         /* ������б����ֽ��� */
    UINT64   ui64OutNormalBrdcastByte;  /* ����㲥�����ֽ��� */
    UINT64   ui64OutNormalMuticastByte; /* ����鲥�����ֽ��� */
    UINT64   ui64OutNormalUnicastByte;  /* ����������������ֽ��� */
    UINT64   ui64OutUnknownUnicastByte; /* ���δ֪���������ֽ��� */
    UINT64   ui64OutNormalPkt;
    UINT64   ui64OutNormalUcastPkt;     /* ������������� */
    UINT64   ui64OutUnknownUcastPkt;    /* ���δ֪���������� */
    UINT64   ui64OutNormalBrdcastPkt;
    UINT64   ui64OutNormalMuticastPkt;
    UINT64   ui64OutNormalPause;
    UINT64   ui64OutPktErr;
    UINT64   ui64OutSpeedPkt;           /* ��������� */
    UINT64   ui64OutSpeedByte;
    UINT64   ui64OutErrPkt;             /* �������� */
    UINT64   ui64OutUnderRun;
    UINT64   ui64OutBuffFail;
    UINT64   ui64OutAbort;
    UINT64   ui64OutDefer;
    UINT64   ui64OutCollision;
    UINT64   ui64OutLateCollision;
    UINT64   ui64OutLostCarrier;
    UINT64   ui64OutNoCarrier;
    UINT64   ui64OutSpeedBrdcastPkt;    /* ����㲥������ */
    UINT64   ui64OutSpeedMulticastPkt;  /* ����鲥������ */
}IF_LANPORT_PKT_STATICSS_S;

/* ������ײ�ȡ�ýӿ���Ϣ */
typedef struct tagIF_INFO_S
{
    IF_INDEX ifIndex;                   /* �ӿ�ָ������ */
    CHAR     *pcInfo;                   /* ��Ϣ������ */
    ULONG    ulSize;                    /* ��������С */
    ULONG    ulChanNo;                  /* ͨ����ţ���E3,T3,Cpos��������ʹ��,
                                           �����ʾָ��ͨ�������ʾ����ͨ�� */
}IF_INFO_S;

/* ������������ȡMTU�ķ�Χ */
ISSU typedef struct tagIFMtuRange
{
    UINT uiMinMtu; /* ����֧�ֵ���Сֵ */
    UINT uiMaxMtu; /* ����֧�ֵ����ֵ */
}IF_MTU_RANGE_S;

/* �˿ڻ������� */
#define PORT_NO_LOOPBACK                        0x1U   /* ȡ�����������û򻷻ؼ�⣬���߻��� */
#define PORT_INTERNAL_LOOPBACK_DETECT           0x2U   /* �ڻ���� */
#define PORT_EXTERNAL_LOOPBACK_DETECT           0x3U   /* �⻷��� */
#define PORT_SERVICE_LOOP                       0x4U   /* ���������� */
#define PORT_INTERNAL_LOOPBACK_DETECT_MANUAL    0x5U   /* �����ڻ����ԡ������Զ�ȡ�� */
#define PORT_EXTERNAL_LOOPBACK_DETECT_MANUAL    0x6U   /* �����⻷���ԡ������Զ�ȡ�� */
#define PORT_STOP_LOOPBACK                      0x7U   /* �жϵ�ǰ���� */

/* �˿ڻ��ز���״̬ */
#define PORT_LOOPBACK_OK                        0x4U
#define PORT_LOOPBACK_FAIL                      0x5U
#define PORT_LOOPBACK_TESTING                   0x6U

/* �˿�˫�������� IOCTL_ETHERNET_SET_MDIX */
#define PORT_MDIX_MODE                          0x1U
#define PORT_MDI_MODE                           0x2U
#define PORT_MDIX_AUTO                          0x3U
#define PORT_MDIX_NOTSUPPORT                    0x4U

/* �˿�˫�����ͣ�IOCTL_ETHERNET_GET_DUPLEX */
#define PORT_DUPLEX_FULL                        0x1U
#define PORT_DUPLEX_HALF                        0x2U
#define PORT_DUPLEX_AUTO                        0x3U

/* �˿����� */
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

/* ��̫���˿�������Э�� ����λ */
#define IF_ETH_AUTO_SPEED_MASK                  0xFFF00000U

/* ��̫����Э�����ʣ� ��16λÿ��BITλ����һ������
   ʵ�����õ�Э������ֵ������ �����������Ľ��
*/
#define IF_ETH_AUTO_SPEED_10M                   0x00100000U
#define IF_ETH_AUTO_SPEED_100M                  0x00200000U
#define IF_ETH_AUTO_SPEED_1G                    0x00400000U

/* threshold set type */
#define IF_THRESHOLD_SET_SD    0x01               /* threshold sd ���� */
#define IF_THRESHOLD_SET_SF    0x10               /* threshold sf ���� */
#define IF_THRESHOLD_SET_BIND  (IF_THRESHOLD_SET_SD | IF_THRESHOLD_SET_SF)
                               /* threshold bind ���� */

/* ����thresholdֵ��Ӧ���������� */
ISSU typedef struct tagIfThresholdCmd
{
    UINT uiSdValue;
    UINT uiSfValue;
    UINT uiType;
}IF_THRESHOLD_S;

/* alarm-detect actiontype */
#define IF_ALARM_ACTION_LINKDOWN 0x10000000
#define IF_ALARM_ACTION_DEFAULT 0 /* UNDO������*/

/* alarm-detect alarmtype */
#define IF_ALARM_SD 0x10000000
#define IF_ALARM_SF 0x20000000
#define IF_ALARM_RDI 0x40000000

/* ����alarmdetectֵ��Ӧ���������� */
ISSU typedef struct tagIfAlarmDetect
{
    UINT uiAlarmType;
    UINT uiActionType;
}IF_ALARMDETECT_S;

/* posmib trap ���� */
#define IF_SONET_TRAPTYPE_B1          1
#define IF_SONET_TRAPTYPE_B2          2
#define IF_SONET_TRAPTYPE_B3          3

/* posmib��ȡB1B2B3������������Я���������ݽṹ */
typedef struct tagIFSONETMIB 
{
    UINT64 uiSectionCVs;
    UINT64 uiLineCVs;
    UINT64 uiPathCVs;
}IF_SONET_MIB_S;

#define IF_CODINGVIOLATION_THRESHOLD_DEFAULT 6
/* ����ע��ϵͳ�ӿڹ�� */
typedef struct tagIF_SYSTEMINFO_S
{
    USHORT usMaxSubSlotPerSlot; /* ÿ��λ����ӿ����� */
    USHORT usMaxIfPerSubSlot;   /* ÿ�ӿ����ӿ����� */
    USHORT usMaxSlotNumber;     /* ϵͳ����λ���� */
    USHORT usMaxIfPerSlot;      /* ÿ�������������� */
    UCHAR  ucIfDimension;    /*����ӿ�������"/"�ĸ���*/
}IF_SYSTEMINFO_S,*PIF_SYSTEMINFO_S;

/* ���ķ������ȼ� */
#define IF_PRI_CRITICAL     7UL
#define IF_PRI_HIGH         5UL
#define IF_PRI_MIDDLE       3UL
#define IF_PRI_LOW          0UL

/* BEGIN: ��̫���ڷ���ģʽԤ�� */
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
    IF_ETH_MODE_E enMode;                   /* �ӿ�ʹ��ģʽ IF_FGE_USEMODE_EN */
    CHAR szIfName[IF_MAX_NAME_LEN + 1];     /* �ӿ���*/
}IF_ETH_MODE_INFO_S;

IF_ETH_MODE_INFO_S *IF_GetEthModeFromCfg(OUT ULONG *pulNum);
/* END: ��̫���ڷ���ģʽԤ�� */

/* BEGIN: MIB��غ궨�弰���ݽӿ� */

/* MIB��Ϣ�ṹ */
typedef struct  tagIFMIB
{
    ULONG ulTableAddr;                          /* ��ṹ��ַ,�ֱ��Ÿ�����ṹ���׵�ַ */
    USHORT usTableType;                         /* MIB������ */
}IF_MIB_S;

typedef struct tagIFMIBPORTTABLE
{
    UINT uiPortType;        /* ��ο�mib�ڵ�hh3cLswPortType������ö��ֵ */
    BOOL_T bIsPlugged;
}IF_MIB_PORTTABLE_S;

/* MIB������*/
#define IFTABLE             1
#define IFXTABLE            2
#define IFRCVADDRTABLE      3
#define IFEXTTABLE          4                   /* �ӿ���չ�����ͣ��ṹ��IFEXTTABLEMIB_S���� */

/* ifTable��usCountSpec�����Բ�֧�ֵĽڵ�궨�� */
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

/* ifTable�����ṹ */
ISSU typedef struct  tagIFTABLEMIB
{
    UINT uiLastChange;                          /* �ӿڵ����һ���޸�ʱ��ϵͳ��������ʱ�� */
    UINT uiMtu;                                 /* �ӿڵ�MTUֵ */
    UINT uiSpeed;                               /* �ӿڵ�ǰ���� */
    UINT uiInOctets;                            /* ��վ���ֽ��� */
    UINT uiInUcastPkts;                         /* ��վ�ĵ������� */
    UINT uiInNUcastPkts;                        /* ��վ�ķǵ������� */
    UINT uiInDiscards;                          /* ��������վ���� */
    UINT uiInErrors;                            /* �д������վ���� */
    UINT uiInUnknownProtos;                     /* ��֧�ֵ�Э�����վ���� */
    UINT uiOutOctets;                           /* ��վ���ֽ��� */
    UINT uiOutUcastPkts;                        /* ��վ�ĵ������� */
    UINT uiOutNUcastPkts;                       /* ��վ�ĵ������� */
    UINT uiOutDiscards;                         /* �����ĳ�վ���� */
    UINT uiOutErrors;                           /* �д���ĳ�վ���� */
    UINT uiOutQLen;                             /* �������г��� */
    USHORT usOperStatus;                        /* �ӿڵĵ�ǰ����״̬ */
    USHORT usCountSpec;                         /* ��uiInOctets��uiOutErrorsͳ�ƣ���֧�ֵ�ͳ����λ��
                                                   ����֧�֣��ֱ��λ��ʼ��λ
                                                   �粻֧��uiInNUcastPkts��uiInUnknownProtos��
                                                   ֵΪ 0000 0000 0010 0100 */
    UCHAR aucPhysAddress[MAC_ADDR_LEN];         /* �ӿ���Э���Ӳ�������ַ */
    UCHAR aucReserve[2];
}IFTABLEMIB_S;

/* ifXTable��uiCountSpec�����Բ�֧�ֵĽڵ�궨�� */
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

/* ifXTable�����ṹ */
ISSU typedef struct  tagIFXTABLEMIB
{
    UINT64 uiHCInOctets;                        /* ��վ���ֽ���(64) */
    UINT64 uiHCInUcastPkts;                     /* ��վ��������(64) */
    UINT64 uiHCInMulticastPkts;                 /* ��վ�ಥ����(64) */
    UINT64 uiHCInBroadcastPkts;                 /* ��վ�㲥����(64) */
    UINT64 uiHCOutOctets;                       /* ��վ���ֽ���(64) */
    UINT64 uiHCOutUcastPkts;                    /* ��վ��������(64) */
    UINT64 uiHCOutMulticastPkts;                /* ��վ�ಥ����(64) */
    UINT64 uiHCOutBroadcastPkts;                /* ��վ�㲥����(64) */
    UINT uiInMulticastPkts;                     /* ��վ�Ķಥ���� */
    UINT uiInBroadcastPkts;                     /* ��վ�Ĺ㲥���� */
    UINT uiOutMulticastPkts;                    /* ��վ�Ķಥ���� */
    UINT uiOutBroadcastPkts;                    /* ��վ�Ĺ㲥���� */
    UINT uiHighSpeed;                           /* �ӿڵĵ�ǰ������λ��1,000,000 bits/�� */
    UINT uiCounterDiscontinuityTime;
    USHORT usPromiscuousMode;                   /* ����ģʽ����״̬ */
    USHORT usConnectorPresent;                  /* �ӿ��²��Ƿ���һ������������ */
    UINT uiCountSpec;                           /* ��uiHCInOctets��uiOutBroadcastPktsͳ�ƣ���֧�ֵ�ͳ����λ��
                                                   ����֧�֣��ֱ��λ��ʼ��λ
                                                   �粻֧��uiHCInMulticastPkts��uiInMulticastPkts��
                                                   ֵΪ 0000 0001 0000 0100 */
}IFXTABLEMIB_S;

/* ifRcvAddressTable�����ṹ */
ISSU typedef struct  tagIFRCVADDRTABLEMIB
{
    USHORT usMACAddrType;                       /* ���Ͷ��������ĺ궨��*/
    USHORT usRcvAddressStauts;                  /* �õ�ַ��״̬ */
    USHORT usRcvAddressType;                    /* ���յ�ַ���������� */
    UCHAR aucRcvAddressAddress[MAC_ADDR_LEN];   /* �ӿ���Э���Ӳ�������ַ */
}IFRCVADDRTABLEMIB_S;

/* IFRCVADDRTABLEMIB_S�ĳ�ԱusMACAddrType�Ķ��� */
#define IFRCVTBL_GET_FIRST_ADDR 1               /* ��ȡ��һ����ַ����Ϣ */
#define IFRCVTBL_GET_CUR_ADDR   2               /* ��ȡ��ǰָ����ַ����Ϣ��ָ����ַ����szRcvAddressAddress�� */
#define IFRCVTBL_GET_NEXT_ADDR  3               /* ��ȡ��һ����ַ����Ϣ��ǰһ����ַ�����szRcvAddressAddress�� */

#define IF_SET_RCVADDR_CREATE   1               /* ����RCV ADDR  */
#define IF_SET_RCVADDR_DELETE   2               /* ɾ��RCV ADDR  */
#define IF_SET_RCVADDR_MODIFY   3               /* �޸�RCV ADDR  */
                                                
#define IF_RCVADDR_OTHER        1               /* �����ĵ�ַ�Ƿ�ı� */
#define IF_RCVADDR_VOLATILE     2               /* ��ַ�ɱ��ı䣬��ֵ�ı��δ�����棬����ϵͳ��ı���ֵ�����ڡ� */
#define IF_RCVADDR_NONVOLATILE  3               /* ��ַ���ɱ��ı� */

/* ifRcvAddressTable��set�Ľṹ */
ISSU typedef struct  tagSETRCVADDR_S
{
    UCHAR ucOperType;                           /* SET�Ĳ������� */
    UCHAR ucRcvAddrType;                        /* ���յ�ַ���������� */
    UCHAR aucRcvAddr [MAC_ADDR_LEN];            /* �ӿڵ������ַ */
}IF_SET_RCVADDR_S;

/* IFEXTTABLE: hh3cIfTable��������ص����ݽṹ */
typedef struct tagIFEXTTABLEMIB
{
    UINT64 ui64HCInNUcastPkts;                  /* �ӿڽ��յķǵ��������� */
    UINT64 ui64HCOutNUcastPkts;                 /* �ӿڷ��͵ķǵ��������� */
}IFEXTTABLEMIB_S;

/* hh3cIfStatTable��������ȡ��ص����ݽṹ */
ISSU typedef struct tagIfFlowStatTable
{
    UINT64 ui64IfStatFlowInBits;                /* ���յı����� */
    UINT64 ui64IfStatFlowOutBits;               /* ���͵ı����� */
    UINT64 ui64IfStatFlowInPkts;                /* ���յİ���   */
    UINT64 ui64IfStatFlowOutPkts;               /* ���͵İ���   */
    UINT64 ui64IfStatFlowInBytes;               /* ���յ��ֽ��� */
    UINT64 ui64IfStatFlowOutBytes;              /* ���͵��ֽ��� */
}IF_FLOW_STAT_TABLE_S;

/* hh3cPortProtocolStatTable��IPv4����ͳ�Ƶ����ݽṹ */
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

/* hh3cPortProtocolStatTable��IPv6����ͳ�Ƶ����ݽṹ */
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

/* hh3cifXXTable��������ȡ��ص����ݽṹ */
typedef struct tagIfPayLoadStatics
{
    UINT64 ui64InByte;                          /* ���յ���Ч�غ��ֽ��� */
    UINT64 ui64OutByte;                         /* ���͵���Ч�غ��ֽ��� */
}IF_PAYLOAD_STATICSS_S;

/* RMON ͳ������غ궨�壬���ǲ�Ʒ��ֲ��, ȡֵ��V5һ�� */
/* rfc2819:  �˿��ϲ����������ĵ��¼���(���¼���Ҳ�����Ǳ������ı�����) */
#define PORT_RMON_STAT_DROP                           100
/* rfc2819:�˿ڴ������Ͻ��յ������ֽ���(Ҫ�����𻵵����ݰ��Լ�У���ֽڣ����������ָ�λ) */
#define PORT_RMON_STAT_BYTES                          101
/* rfc2819:�˿ڽ��յ��İ�������(Ҫ�����𻵵����ݰ����㲥���Լ��鲥��) */
#define PORT_RMON_STAT_PKTS                           102
/* rfc2819:�˿ڽ��յ���ù㲥�����������������鲥���� */
#define PORT_RMON_STAT_BROAD_PKS                      103
/* rfc2819:�˿ڽ��յ�����鲥�����������������㲥����*/
#define PORT_RMON_STAT_MULTI_PKTS                     104
/* rfc2819:�˿��յ��ı��Ľ���[64, 1518]���Һ���У������������*/
#define PORT_RMON_STAT_CRC_ALIGN                      105
/* rfc2819: �˿��յ�����̫С��С��64���İ���*/
#define PORT_RMON_STAT_UNDER_SZ                       106
/* rfc2819: �˿��յ�����Ҵ���1518�İ���*/
#define PORT_RMON_STAT_OVER_SZ                        107
/* rfc2819:�˿ڽ��յ���bad FCS��FCS Error��Alignment Error�ĳ���С��64���ֽڣ��������ָ�λ��������FCS�������ݰ���*/
#define PORT_RMON_STAT_FRAGMENTS                      108
/* rfc2819:�˿ڽ��յ���bad FCS��FCS Error��Alignment Error�Ĵ���1518���ֽڣ��������ָ�λ��������FCS�������ݰ���*/
#define PORT_RMON_STAT_JABBERS                        109
/* rfc2819:�˿ڼ����ĳ�ͻ�� */
#define PORT_RMON_STAT_COLLISIONS                     110
/* rfc2819: ����Ϊ�˿ڽ��յ������ȣ��������ָ�λ��������У��λ����Χ�ڵ����ݰ��������𻵵İ�����6��ͳ�����  */
#define PORT_RMON_STAT_PKT64                          111
#define PORT_RMON_STAT_PKT65TO127                     112
#define PORT_RMON_STAT_PKT128TO255                    113
#define PORT_RMON_STAT_PKT256TO511                    114
#define PORT_RMON_STAT_PKT512TO1023                   115
#define PORT_RMON_STAT_PKT1024TO1518                  116
#define PORT_DOT1D_PORT_CAPABILITIES                  200   /* no rfc */

typedef struct tagIfGetEthernetStatistics
{
    UINT   uiItem;                              /* ��ȡ���ͣ��μ�ͳ����ĺ궨�� */
    UINT64 ui64Data;                            /* ��Ӧ��ȡ��ͳ����Ϣ */
}IF_GET_ETHERNET_STATISTICS_S;

typedef enum en_IF_IFEXTTRAPTYPE
{
    IF_BANDWIDTHUSAGEHIGH = 1,                  /* h3cIfBandwidthUsageHigh */
    IF_DISCARDPKTRATEHIGH                       /* h3cIfDiscardPktRateHigh */
}IF_IFEXTTRAPTYPE_E;

typedef struct tagIF_IFEXTTRAPS
{
    ULONG ulIfTrapsType;                        /* �澯����ȡֵIF_IFEXTTRAPTYPE_E */
    ULONG ulUpperLimit;                         /* �澯���� */
}IF_IFEXTTRAPS_S;

/* RMON MIB������  */
#define RMON_STAT_TYPE 1                        /* etherStatsTable���� */
#define RMON_HIST_TYPE 2                        /* etherHistoryTable���� */

typedef struct tagIfGetRmonStatistics
{
    USHORT usTableType;                         /* RMON MIB������ */
    ULONG  ulTableAddr;                         /* ��ṹ��ַ����ű�ṹ���׵�ַ */
}IF_GET_RMON_STATISTICS_S;

/* rmon mib����ͳ�ƽṹ */
ISSU typedef struct tagIFRMONMIB
{
    UINT64 ui64DropEvents;              /* �ӿ��ϲ����Ķ������ĵ��¼����򱻶����ı����� */
    UINT64 ui64Octets;                  /* �ӿڽ��յ��ֽ��� */
    UINT64 ui64Pkts;                    /* �ӿڽ��յı��İ��� */
    UINT64 ui64BroadcastPkts;           /* �ӿڽ��յ���ù㲥���ģ��������ಥ���ģ����� */
    UINT64 ui64MulticastPkts;           /* �ӿڽ��յ���öಥ���ģ��������㲥���ģ����� */
    UINT64 ui64CRCAlignErrors;          /* �ӿڽ��յĽ���[64, 1518]�ֽ��Ұ���FCS Error��Alignment Error�ı��İ��� */
    UINT64 ui64UndersizePkts;           /* �ӿڽ��յ������С��64�ֽڵı��İ��� */
    UINT64 ui64OversizePkts;            /* �ӿڽ��յ�����Ҵ���1518�ֽڵı��İ��� */
    UINT64 ui64Fragments;               /* �ӿڽ��յ�С��64�ֽ��Ұ���FCS Error��Alignment Error�ı��İ��� */
    UINT64 ui64Jabbers;                 /* �ӿڽ��յĴ���1518�ֽ��Ұ���FCS Error��Alignment Error�ı��İ��� */
    UINT64 ui64Collisions;              /* �ӿڼ����ĳ�ͻ�� */
    UINT64 ui64Pkts64Octets;            /* �ӿڽ��յ�64�ֽڵı��ģ����𻵵ģ����� */
    UINT64 ui64Pkts65to127Octets;       /* �ӿڽ��յĽ���[65, 127]�ֽڵı��ģ����𻵵ģ����� */
    UINT64 ui64Pkts128to255Octets;      /* �ӿڽ��յĽ���[128, 255]�ֽڵı��ģ����𻵵ģ����� */
    UINT64 ui64Pkts256to511Octets;      /* �ӿڽ��յĽ���[256, 511]�ֽڵı��ģ����𻵵ģ����� */
    UINT64 ui64Pkts512to1023Octets;     /* �ӿڽ��յĽ���[512, 1023]�ֽڵı��ģ����𻵵ģ����� */
    UINT64 ui64Pkts1024to1518Octets;    /* �ӿڽ��յĽ���[1024, 1518]�ֽڵı��ģ����𻵵ģ����� */
}IF_RMON_MIB_S;

/* END: MIB��غ궨�弰���ݽӿ� */

/* �ӿ�ͳ�ƽṹ */
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


/* BEGIN: IFС�������*/

ISSU typedef enum enIfGlobalCmd
{
    IF_GLOBAL_SET_CUT_THROUGH,        /* 0: ����cut throughģʽ */
    IF_GLOBAL_SET_BURST_MODE,         /* 1: ����burstģʽ */
    IF_GLOBAL_GET_PACKETDROP_SUMMARY, /* 2: ��ȡȫ��packet��Ϣ */
    IF_GLOBAL_SET_FLOW_INTERVAL,      /* 3: ����ȫ��flow interval */
    IF_GLOBAL_ADD_RESERVE_VLAN_IF,    /* 4: ����vlan��ӿ� */
    IF_GLOBAL_DELETE_RESERVE_VLAN_IF, /* 5: ɾ������vlan��ӿ� */
    IF_GLOBAL_ADD_ISOLATE_GROUP,      /* 6: ���������� Add the isolate group.*/
    IF_GLOBAL_DELETE_ISOLATE_GROUP,   /* 7: ɾ�������� Delete the isolate group.*/
    IF_GLOBAL_MDC_ALLOC_INTF_CHECK,   /* 8: ������ALLOC�ֿڽ��м��*/
    IF_GLOBAL_SET_USING_FX,           /* 9: ���ö˿ڷ��� FGE TO XGE*/
    IF_GLOBAL_SET_USING_FF,           /* 10: ���ö˿ڷ��� �ָ�FGE*/
    IF_GLOBAL_SET_USING_FH,           /* 11: ���ö˿ڷ��� FGE TO HGE*/
    IF_GLOBAL_SET_USING_HX,           /* 12: ���ö˿ڷ��� HGE TO XGE*/
    IF_GLOBAL_SET_USING_HF,           /* 13: ���ö˿ڷ��� HGE TO FGE*/
    IF_GLOBAL_SET_USING_HH,           /* 14: ���ö˿ڷ��� �ָ�HGE*/
    IF_GLOBAL_SET_PORT_TYPESWITCH,    /* 15: ���ö˿������л� */
    IF_GLOBAL_SET_MAX = 1000,
}IF_GLOBAL_CMD_E;

/* IOCTL_ETHERNET_PRIORITY_FLOW_CONTROL */
#define IF_PRIORITY_FLOW_CONTROL_FUNCTION       1UL     /* ��ʾ����PFC���� */
#define IF_PRIORITY_FLOW_CONTROL_NODROP_DOT1P   2UL     /* ��ʾ����PFC��802.1p�Ĳ�����״̬ */
#define IF_PRIORITY_FLOW_CONTROL_ENABLE         1       /* ����PFC���� */
#define IF_PRIORITY_FLOW_CONTROL_DISABLE        2       /* �ر�PFC���ܣ�ȱʡ���á� */
#define IF_PRIORITY_FLOW_CONTROL_AUTO           3       /* ����PFC��Э�̹��� */
                                                        
/* IF_COMMON_CFG_FLOWCONTROL */                         
#define IF_FLOW_CONTROL_ENABLE_RECIEVE          3       /* flow-control receive enable, ENABLE = 1, DISABLE = 2*/
                                                        
/* IOCTL_ETHERNET_SET_PORTISOLATE */                    
#define IF_PISO_DISABLE                         0       /* �ӿ��˳������� */
#define IF_PISO_ENABLE                          1       /* �ӿڼ���Ĭ�ϸ����飬������Ų��ٶ���� */

/* IOCTL_COMMON_LINKDELAY: Link delay�ӿ�phyioctl����ʱ��λbit�궨�� */
#define IF_LINKDELAY_UNDO                       0x0           /* �˿ڹر�LINKDELAYģʽ */
#define IF_LINKDELAY_DOWN                       0x10000000    /* �˿�DOWN״̬����ģʽ����λΪ�� */
#define IF_LINKDELAY_UP                         0x20000000    /* �˿�UP״̬����ģʽ����λΪ�� */
#define IF_LINKDELAY_UPDOWN                     0x30000000    /* �˿�UPDOWN״̬����ģʽ����λΪ�� */
#define IF_LINKDELAY_MSEC                       0x40000000    /* �˿�DOWN״̬����ģʽ����λΪ���루�������йؼ���msec�� */
#define IF_LINKDELAY_MSEC_UP                    0x50000000    /* �˿�UP״̬����ģʽ����λΪ���루�������йؼ���msec�� */
#define IF_LINKDELAY_MSEC_UPDOWN                0x60000000    /* �˿�UPDOWN״̬����ģʽ����λΪ���루�������йؼ���msec�� */

#define IF_LINKDELAY_UNDO_UP                    0x70000000    /* �˿ڹر�LINKDELAY UPģʽ */
#define IF_LINKDELAY_UNDO_DOWN                  0x80000000    /* �˿ڹر�LINKDELAY DOWNģʽ */
#define IF_LINKDELAY_UNDO_UPDOWN                0x90000000    /* �˿ڹر�LINKDELAY UPDOWNģʽ */
#define IF_LINKDELAY_UNDO_MSEC_UP               0xa0000000    /* �˿ڹر�LINKDELAY MSEC UPģʽ */
#define IF_LINKDELAY_UNDO_MSEC_DOWN             0xb0000000    /* �˿ڹر�LINKDELAY MSEC DOWNģʽ */
#define IF_LINKDELAY_UNDO_MSEC_UPDOWN           0xc0000000    /* �˿ڹر�LINKDELAY MSEC UPDOWNģʽ */

/* IOCTL_ETHERNET_SET_STORM_BLOCK */
#define IF_ETHERNET_UNDO_STORM_BLOCK            0x00    /* ȡ����ǰ������̫���ӿڵ�����block */
#define IF_ETHERNET_DO_STORM_BLOCK              0x01    /* ʹ�ܵ�ǰ������̫���ӿ�����block */
#define IF_ETHERNET_STORM_BLOCK_BROADCAST       0x01    /* block�㲥���� */
#define IF_ETHERNET_STORM_BLOCK_MULTICAST       0x02    /* block�鲥���� */
#define IF_ETHERNET_STORM_BLOCK_UNICAST         0x04    /* block�������� */
typedef struct tagIFEthStormBlock
{
    ULONG ulBlockAction;    /* ��������orȡ�����ƣ��μ����Ϻ궨�� */
    ULONG ulBlockType;      /* ���Ƶı������ͣ��μ����Ϻ궨��  */
}IF_ETHERNET_STORM_BLOCK_S;

/* ���� IOCTL_ETHERNET_SET_STATISTICS ������ */
#define IF_IN_NORMAL_UNICAST_BYTES              1       /* �뷽�������ĵ��������ֽ��� */
#define IF_IN_NORMAL_BRDCAST_BYTES              2       /* �뷽�������Ĺ㲥�����ֽ��� */
#define IF_IN_NORMAL_MUTICAST_BYTES             3       /* �뷽���������鲥�����ֽ��� */
#define IF_IN_NORMAL_UNICAST_PACKETS            4       /* �뷽�������ĵ������İ��� */
#define IF_IN_NORMAL_BRDCAST_PACKETS            5       /* �뷽�������Ĺ㲥���İ��� */
#define IF_IN_NORMAL_MUTICAST_PACKETS           6       /* �뷽���������鲥���İ��� */

/* ���㲥����PPS��KBPS��ʽ֧��ʱ����������Ͷ˿ڵ��������ֵ */
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
    ULONG ulType;   /* ������9�ֺ궨��*/
    ULONG ulValue;  /* Ratio��0 ~ 100��pps��BPSʱΪʵ��ֵ */
}IF_STORM_CONTROL_S;

/* END: IFС���� */

/* �忨�ӿ��Ȳ��״̬���壬�ϱ��塢�ӿ����ӿ�״̬�ı�ʹ�� */
ISSU typedef enum tagIF_HotState
{
    IF_HOT_STATE_INVALID,
    IF_NODE_STATE_ABSENT,           /* �ڵ�γ� */
    IF_NODE_STATE_PRESENT,          /* �ڵ����:Ŀǰֻ�й����� */
    IF_NODE_STATE_DELETE,           /* �ڵ�ɾ�� */
    IF_NODE_STATE_SWITCH,           /* �������� */
    IF_SUBSLOT_STATE_ABSENT,        /* �ӿ��γ� */
    IF_SUBSLOT_STATE_PRESENT,       /* �ӿ����� */
    IF_INTF_STATE_ABSENT,           /* �ӿڰγ� */
    IF_INTF_STATE_PRESENT,          /* �ӿڲ��� */
    IF_NODE_STATE_FASTABSENT,       /* �ڵ���ٰγ� */
    IF_INTF_PRECFG_STATE_PRESENT,   /* Ԥ���ýӿڲ��� */
    IF_NODE_STATE_PROVISION_PRESENT,/* Ԥ���ð���� */
    IF_NODE_STATE_PROVISION_DELETE, /* Ԥ���ð�γ� */
    IF_HOT_STATE_MAX
}IF_HOT_STATE_E;

/*********************BEGIN: WAN�ӿ����********************************/

#define IF_TIMESLOT_MAX                         32
                                                
#define IF_CHANGROUP_PRI                        50      /* pri������� */
#define IF_CHANGROUP_VOICE                      51      /* ����������� */
#define IF_CHANGROUP_NULL                       64      /* ����� */
                                                        
#define IF_E3_E1NUM                             16      /* E3����E1������������ */
#define IF_T3_T1NUM                             28      /* T3����T1������������ */
#define IF_CPOS_E1NUM                           63      /* CPOS����E1������������ */
#define IF_CPOS_T1NUM                           84      /* CPOS����T1������������ */
#define IF_CPOS_E3NUM                           3       /* CPOS����E3������������ */
#define IF_CPOS_T3NUM                           3       /* CPOS����T3������������ */

/* DO FLAG */
#define IF_OPERFLAG_DO                          0       /* ������Ϣ��������ֵ */
#define IF_OPERFLAG_UNDO                        1       /* ��������Ϣ��������ֵ */

/* DETECT SWITCH */            
#define IF_DETECT_ON                            0
#define IF_DETECT_OFF                           1
                                                        
#define IF_AUTOCLOCKCHANGE_NULL                 0   
#define IF_AUTOCLOCKCHANGE_AUTO                 1  

/* CLOCK */                                                
#define IF_CLOCK_LINE                           0       /* ��·ʱ��ģʽ */
#define IF_CLOCK_INTERNAL                       1       /* �ڲ�ʱ��ģʽ */

/* LOOPBACK */
#define IF_LOOPBACK_NOLOOP                      0       /* ��ֹ���� */
#define IF_LOOPBACK_LOCAL                       1       /* �����Ի� */
#define IF_LOOPBACK_REMOTE                      2       /* ����Զ�˻��� */
#define IF_LOOPBACK_PAYLOAD                     3       /* ����Զ���غɻ��� */

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
#define IF_CHANMODE_FRAMED                      0       /* ͨ����ģʽ */
#define IF_CHANMODE_UNFRAMED                    1       /* ��ͨ��ģʽ */                                                        

/* ʱ϶��ϢIF_TIMESLOTINFO_S��ͨ�����ͱ�־�ĺ꣬��λ��ʾ */
#define IF_FLAG_ISDN                            0x1     /* E1 PRIͨ����־ */
#define IF_FLAG_UNFRAMED                        0x2     /* ��ͨ��ģʽ��־ */
#define IF_FLAG_SPEED_56K                       0x4     /* 56k ���ʱ�־ */
#define IF_FLAG_E1T1VI_CCS                      0x8     /* CCS�źű�־��pri-setʱ�·� */
#define IF_FLAG_E1T1VI_CAS                      0x10    /* CAS�źű�־��E1/T1 timeslot-setʱ�·� */
#define IF_FLAG_E3                              0x20    /* ����CPOS�ӿ�E3��־ */
#define IF_FLAG_T3                              0x40    /* ����CPOS�ӿ�E3��־ */
#define IF_FLAG_FRAMED                          0x80    /* ����CPOS�ӿ�ͨ��ģʽ */
#define IF_FLAG_POS                             0x100   /* POS��־��pri-setʱ�·� */
#define IF_FLAG_DM                              0x200   /* DM��־��pri-setʱ�·� */

/* �ӿ�ʱ϶��Ϣ������ͨ�����ӿ�ʱ������IOCTL_COMMON_CREATE_CHANNEL�Ĳ��� */
ISSU typedef struct tagIfTimeSlotInfo
{
    IF_INDEX ifIndex;                           /* �����ɽӿڵ����� */
    UINT     uiFlag;                            /* ͨ�����ͱ�־, ��λ��ʾ */
    UINT     uiChanNo;                          /* ͨ���� */
    UINT     uiTSCount;                         /* ʱ϶���� */    
    UINT     auiTSList[IF_TIMESLOT_MAX];        /* ʱ϶�������� */
}IF_TIMESLOTINFO_S;

/* IF_COMMON_CHANDATA_S�������CE3/CT3/POS/CPOS�Ƚӿ�������ʽ���ƵĹ������ݽṹ */
ISSU typedef struct tagIfCommonChanDataCmd
{
    UINT uiChanNo;                              /* CE3/CT3�ӿڻ�E1/T1ͨ����CPOSͨ�� */
    UINT uiSetValue;                            /* ����ֵ */
}IF_COMMON_CHANDATA_S, *PIF_COMMON_CHANDATA_S;

ISSU typedef struct tagIfCommonChanDataFlagCmd
{
    UINT uiChanNo;                              /* �ӿڻ�ͨ���� */
    UINT uiSetValue;                            /* ����ֵ */
    UINT uiDoFlag;                              /* �������������*/
}IF_COMMON_CHANDATAFLAG_S, *PIF_COMMON_CHANDATAFLAG_S;

ISSU typedef struct IF_CHANINFO
{
    IF_INDEX ifIndex;
    UINT uiChanNo;
}IF_CHANINFO_S;

typedef struct tagIfBertData
{
    UINT uiOnFlag;                              /* ֹͣ��ʼֵ*/
    UINT uiPatternValue;                        /* ����ģʽ*/
    UINT uiTimeValue;                           /* BERT���Եĳ���ʱ��ֵ */
    UINT uiFramedFlag;                          /* ��֡ģʽ��ǳ�֡ģʽ */
    UINT uiChanNo;                              /* CE3/CT3�ӿڻ�E1/T1ͨ���� */
}IF_BERT_DATA_S, *PIF_BERT_DATA_S;

/* IOCTL_CELLULAR_NETWORK ��ȡ�ӿ�֧�ֵ����� */
#define IF_CELLULAR_NETWORK_NONE  0x0UL  /* ����Ч���� */
#define IF_CELLULAR_NETWORK_2G    0x1UL  /* 2G ���� */
#define IF_CELLULAR_NETWORK_3G    0x2UL  /* 3G ���� */
#define IF_CELLULAR_NETWORK_4G    0x4UL  /* 4G ���� */

/* Cellular��PhyIOCtl������ */
#define IOCTL_CELLULAR_CREATE_CHANNEL           IF_IOCTL_CMD_DEF(MID_IFNET, IF_CELLULAR_TYPE, 0)
#define IOCTL_CELLULAR_NETWORK                  IF_IOCTL_CMD_DEF(MID_IFNET, IF_CELLULAR_TYPE, 1) /* ��ѯcell-eth֧�ֵ����� */

/* ===BEGIN: ���������֡���=== */               
#define IOCTL_SERIAL_GET_WORK_MODE              IF_IOCTL_CMD_DEF(MID_IFNET, IF_SERIAL_TYPE, 1)
#define IOCTL_SERIAL_SET_ACCM                   IF_IOCTL_CMD_DEF(MID_IFNET, IF_SERIAL_TYPE, 2)
#define IOCTL_SERIAL_QUERY_SOFTCVT              IF_IOCTL_CMD_DEF(MID_IFNET, IF_SERIAL_TYPE, 3)
#define IOCTL_SERIAL_SET_ASYNC_WORK_MODE        IF_IOCTL_CMD_DEF(MID_IFNET, IF_SERIAL_TYPE, 4)  /* �����첽����ģʽ */
#define IOCTL_SERIAL_RESET_LEAD                 IF_IOCTL_CMD_DEF(MID_IFNET, IF_SERIAL_TYPE, 5)
#define IOCTL_SERIAL_SET_LEAD                   IF_IOCTL_CMD_DEF(MID_IFNET, IF_SERIAL_TYPE, 6)
#define IOCTL_SERIAL_SET_FLOWCTLPEER            IF_IOCTL_CMD_DEF(MID_IFNET, IF_SERIAL_TYPE, 7)
#define IOCTL_SERIAL_SET_BAUDRATE               IF_IOCTL_CMD_DEF(MID_IFNET, IF_SERIAL_TYPE, 8)  /* ���ò����� */
#define IOCTL_SERIAL_SET_DTE_CLOCK              IF_IOCTL_CMD_DEF(MID_IFNET, IF_SERIAL_TYPE, 9)  /* ����DTEʱ��ģʽ */
#define IOCTL_SERIAL_SET_LINE_CODE              IF_IOCTL_CMD_DEF(MID_IFNET, IF_SERIAL_TYPE, 10) /* ������·���� */
#define IOCTL_SERIAL_DETECT_DCD                 IF_IOCTL_CMD_DEF(MID_IFNET, IF_SERIAL_TYPE, 11) /* �����Ƿ���DCD */
#define IOCTL_SERIAL_DETECT_DSR                 IF_IOCTL_CMD_DEF(MID_IFNET, IF_SERIAL_TYPE, 12) /* �����Ƿ���DSR */
#define IOCTL_SERIAL_SET_IDLE_MARK              IF_IOCTL_CMD_DEF(MID_IFNET, IF_SERIAL_TYPE, 13) /* ������·������ */
#define IOCTL_SERIAL_SET_ELIMINATE_PULSE        IF_IOCTL_CMD_DEF(MID_IFNET, IF_SERIAL_TYPE, 14) /* ���������� */
#define IOCTL_SERIAL_INVERT_RCLK                IF_IOCTL_CMD_DEF(MID_IFNET, IF_SERIAL_TYPE, 15) /* ���÷�ת����ʱ�� */
#define IOCTL_SERIAL_INVERT_TCLK                IF_IOCTL_CMD_DEF(MID_IFNET, IF_SERIAL_TYPE, 16) /* ���÷�ת����ʱ�� */
#define IOCTL_SERIAL_SET_LOOPBACK               IF_IOCTL_CMD_DEF(MID_IFNET, IF_SERIAL_TYPE, 17) /* ���û���ģʽ */
#define IOCTL_SERIAL_SET_PHYSICAL_MODE          IF_IOCTL_CMD_DEF(MID_IFNET, IF_SERIAL_TYPE, 18) /* ���ô��ڹ���ģʽ */
#define IOCTL_SERIAL_SET_PHYMRU                 IF_IOCTL_CMD_DEF(MID_IFNET, IF_SERIAL_TYPE, 19) /* ���ô��������յ�ԪMRU */
#define IOCTL_SERIAL_REVERSE_RTS                IF_IOCTL_CMD_DEF(MID_IFNET, IF_SERIAL_TYPE, 20) /* ���÷�תRTS */
#define IOCTL_SERIAL_SET_VIRTUAL_BAUDRATE       IF_IOCTL_CMD_DEF(MID_IFNET, IF_SERIAL_TYPE, 21) /* �����鲨���� */
#define IOCTL_SERIAL_SET_CRC_TYPE               IF_IOCTL_CMD_DEF(MID_IFNET, IF_SERIAL_TYPE, 22) /* ����CRCУ��ʹ�õ�λ�� */
#define IOCTL_SERIAL_SET_ITF_NUMBER             IF_IOCTL_CMD_DEF(MID_IFNET, IF_SERIAL_TYPE, 23) /* ����֡���������� */
#define IOCTL_SERIAL_GET_CABLETYPE              IF_IOCTL_CMD_DEF(MID_IFNET, IF_SERIAL_TYPE, 24) /* ��ȡ��������(DTE/DCE) */

/* ���ڵ������ֶ�Ӧ�Ĳ������� */
/* IOCTL_SERIAL_SET_ASYNC_WORK_MODE */
#define IF_SERIAL_MODE_FLOW_EXEC                1   /* AM, Aux��Ĭ��Ϊ��ģʽ */
#define IF_SERIAL_MODE_PROTOCOL                 2   /* �첽����Ĭ��ΪЭ��ģʽ */
                                            
/* IOCTL_SERIAL_SET_DTE_CLOCK */            
#define IF_SERIAL_CLKSLT_DTE1                   1   /* Ĭ������ */
#define IF_SERIAL_CLKSLT_DTE2                   2
#define IF_SERIAL_CLKSLT_DTE3                   3
#define IF_SERIAL_CLKSLT_DTE4                   4
#define IF_SERIAL_CLKSLT_DTE5                   5
#define IF_SERIAL_CLKSLT_DTEAUTO                6   /* �Զ����� */
                                                
#define IF_SERIAL_CLKSLT_DCE1                   7   /* �շ�ʱ�Ӿ�ȡ�Ա��ء���֧������DCE��ΪDCEģʽ�µ�Ĭ�����á�*/
#define IF_SERIAL_CLKSLT_DCE2                   8   /* ����ʱ��ȡ�Ա��ء�����ʱ��ȡ����·�� */
#define IF_SERIAL_CLKSLT_DCE3                   9   /* �շ�ʱ�Ӿ�ȡ����·�� */
#define IF_SERIAL_CLKSLT_DCEC                   10  /* No Command Line Set */

#define IF_SERIAL_FLOWCTL_ENABLE                1   /* ֪ͨ�Զ˴����� */
#define IF_SERIAL_FLOWCTL_DISABLE               2   /* ֪ͨ�Զ�ȡ������ */
                                                
/* IOCTL_SERIAL_SET_LINE_CODE */                
#define IF_SERIAL_LINECODE_NRZ                  0
#define IF_SERIAL_LINECODE_NRZI                 1
                                                
/* IOCTL_SERIAL_DETECT_DCD */                   
#define IF_SERIAL_DETECT_DCD_NONE               0
#define IF_SERIAL_DETECT_DCD                    1   /* Ĭ������ */
                                                
/* IOCTL_SERIAL_DETECT_DSR */                   
#define IF_SERIAL_DETECT_DSR_NONE               0
#define IF_SERIAL_DETECT_DSR                    1   /* Ĭ������ */
                                                
/* IOCTL_SERIAL_SET_IDLE_MARK */                
#define IF_SERIAL_IDLE_FLAG                     0   /* Ĭ������ */
#define IF_SERIAL_IDLE_MARK                     1
                                            
/* IOCTL_SERIAL_SET_ELIMINATE_PULSE */      
#define IF_SERIAL_ELIMINATE_PULSE_NONE          0   /* Ĭ������ */
#define IF_SERIAL_ELIMINATE_PULSE               1
                                                
/* IOCTL_SERIAL_INVERT_RCLK */                  
#define IF_SERIAL_INVERT_RCLK_NONE              0   /* Ĭ������ */
#define IF_SERIAL_INVERT_RCLK                   1
                                                
/* IOCTL_SERIAL_INVERT_TCLK */                  
#define IF_SERIAL_INVERT_TCLK_NONE              0   /* Ĭ������ */
#define IF_SERIAL_INVERT_TCLK                   1
                                                
/* IOCTL_SERIAL_SET_LOOPBACK */                 
#define IF_SERIAL_LOOPBACK_NONE                 0   /* Ĭ������ */
#define IF_SERIAL_LOOPBACK_LOOP                 1           
                                                
/* IOCTL_SERIAL_SET_PHYSICAL_MODE */            
#define IF_SERIAL_PHYLAYER_SYNC                 0   /* Ĭ������ */
#define IF_SERIAL_PHYLAYER_ASYNC                1
                                                
/* IOCTL_SERIAL_REVERSE_RTS */                  
#define IF_SERIAL_REVERSE_RTS_NONE              0   /* Ĭ������ */
#define IF_SERIAL_REVERSE_RTS                   1
                                                
/* IOCTL_SERIAL_SET_CRC_TYPE */                  
#define IF_SERIAL_CRC_16BIT                     0
#define IF_SERIAL_CRC_32BIT                     1
#define IF_SERIAL_CRC_NONE                      2
#define IF_SERIAL_CRC_DEFAULT                   IF_SERIAL_CRC_16BIT
                                                
#define IF_SERIAL_DEFAULT_BAUDRATE_SYNC         64000   /* ͬ��ģʽ�µ�Ĭ�ϲ����� */
#define IF_SERIAL_DEFAULT_BAUDRATE_ASYNC        9600    /* �첽ģʽ�µ�Ĭ�ϲ����� */
#define IF_SERIAL_DEFAULT_MRU                   1700    /* Ĭ��MRU */

/* ===END: ���������֡���=== */


/* ===BEGIN: FCM�����֡���=== */
#define IOCTL_FCM_SET_PCM                       IF_IOCTL_CMD_DEF(MID_IFNET, IF_FCM_TYPE, 1) /* ����Pulse Code Modulation */
                                                
#define IF_FCM_PCM_A_LAW                        1   /* A law */
#define IF_FCM_PCM_U_LAW                        2   /* U law */
/* ===END: FCM�����֡���=== */                  
                                                
                                                
/* ===BEGIN: BRI�����֡���=== */                
#define IOCTL_BRI_ACTIVATE                      IF_IOCTL_CMD_DEF(MID_IFNET, IF_BRI_TYPE, 1)  /* ����BRI */
#define IOCTL_BRI_SET_LOOPBACK                  IF_IOCTL_CMD_DEF(MID_IFNET, IF_BRI_TYPE, 2)  /* ���û��� */
#define IOCTL_BRI_SWITCH_PHYTYPE                IF_IOCTL_CMD_DEF(MID_IFNET, IF_BRI_TYPE, 3)  /* isdn leased-line֪ͨ���� */
                                                
/* IOCTL_BRI_SET_LOOPBACK */                    
#define IF_BRI_LOOPBACK_NO                      0   /* default */
#define IF_BRI_LOOPBACK_B1                      1
#define IF_BRI_LOOPBACK_B2                      2
#define IF_BRI_LOOPBACK_BOTH                    3
#define IF_BRI_LOOPBACK_REMOTE                  4

#define IF_BRI_MAX_CHANNUM                      2   /* Bri�ӿ���ͨ�������� */

#define IF_BRI_ISDN_LEASED_NO                   0x0U
#define IF_BRI_ISDN_LEASED_B1                   0x1U
#define IF_BRI_ISDN_LEASED_B2                   0x2U
#define IF_BRI_ISDN_LEASED_B1B2                 (IF_BRI_ISDN_LEASED_B1 | IF_BRI_ISDN_LEASED_B2)
#define IF_BRI_ISDN_LEASED_128                  0x4U
#define IF_BRI_ISDN_LEASED_144                  0x8U

/* IOCTL_BRI_SWITCH_PHYTYPE�·�����Я�������� */
typedef struct tagIF_BRI_SWITCH_INFO
{
    UINT uiOperType; /* �������� */
    UINT uiLineType; /* ר�����ͣ�128 or 144 */
}IF_BRI_SWITCH_INFO_S;

/* Ԥ��isdn leased-line����ʹ�õĽṹ */
ISSU typedef struct tagIF_BRI_LINE_INFO
{
    UINT uiLineType;                    /* ר�����ͣ�128 or 144 */
    CHAR szIfName[IF_MAX_NAME_LEN + 1]; /* �ӿ���*/
}IF_BRI_LINE_INFO_S;

/* ===END: BRI�����֡���=== */


/* ===BEGIN: E1/T1/E3/T3�����֡���=== */
/* E1/T1������������������ */
#define IOCTL_E1T1_SET_CLOCK                    IF_IOCTL_CMD_DEF(MID_IFNET, IF_T1_TYPE, 1)  /* ����ʱ��ģʽ */
#define IOCTL_E1T1_SET_LINE_CODE                IF_IOCTL_CMD_DEF(MID_IFNET, IF_T1_TYPE, 2)  /* ������·�����ʽ */
#define IOCTL_E1T1_SET_FRAMING                  IF_IOCTL_CMD_DEF(MID_IFNET, IF_T1_TYPE, 3)  /* ���ó�֡��ʽ */
#define IOCTL_E1T1_SET_IDLE_CODE                IF_IOCTL_CMD_DEF(MID_IFNET, IF_T1_TYPE, 4)  /* ���ÿ����� */
#define IOCTL_E1T1_SET_ITF_CODE                 IF_IOCTL_CMD_DEF(MID_IFNET, IF_T1_TYPE, 5)  /* ����֡��������ʽ */
#define IOCTL_E1T1_SET_ITF_NUMBER               IF_IOCTL_CMD_DEF(MID_IFNET, IF_T1_TYPE, 6)  /* ����֡���������� */
#define IOCTL_E1T1_SET_LOOPBACK                 IF_IOCTL_CMD_DEF(MID_IFNET, IF_T1_TYPE, 7)  /* ���û��� */
#define IOCTL_E1T1_SET_CABLE                    IF_IOCTL_CMD_DEF(MID_IFNET, IF_T1_TYPE, 9)  /* ����ʹ�õĵ������� */
#define IOCTL_E1T1_SET_FDL                      IF_IOCTL_CMD_DEF(MID_IFNET, IF_T1_TYPE, 10) /* ����T1ʹ��FDLλ��ģʽ */
#define IOCTL_E1T1_SET_SEND_LOOPCODE            IF_IOCTL_CMD_DEF(MID_IFNET, IF_T1_TYPE, 11) /* ����Զ�˻��� */
#define IOCTL_E1T1_SET_BERT                     IF_IOCTL_CMD_DEF(MID_IFNET, IF_T1_TYPE, 12) /* ����bert */
#define IOCTL_E1T1_SET_ALARMTHRESHOLD_LOS_PD    IF_IOCTL_CMD_DEF(MID_IFNET, IF_T1_TYPE, 13) /* ���ø澯���� */
#define IOCTL_E1T1_SET_ALARMTHRESHOLD_LOS_PR    IF_IOCTL_CMD_DEF(MID_IFNET, IF_T1_TYPE, 14) /* ���ø澯���� */
#define IOCTL_E1T1_SET_ALARMTHRESHOLD_AIS       IF_IOCTL_CMD_DEF(MID_IFNET, IF_T1_TYPE, 15) /* ���ø澯���� */
#define IOCTL_E1T1_SET_ALARMTHRESHOLD_LFA       IF_IOCTL_CMD_DEF(MID_IFNET, IF_T1_TYPE, 16) /* ���ø澯���� */
#define IOCTL_E1T1_SET_ALARMTHRESHOLD_DEFAULT   IF_IOCTL_CMD_DEF(MID_IFNET, IF_T1_TYPE, 17) /* ���ø澯���� */
#define IOCTL_E1T1_SET_DATA_CODING              IF_IOCTL_CMD_DEF(MID_IFNET, IF_T1_TYPE, 18) /* ���������Ƿ�ת */
#define IOCTL_E1T1_SET_TDMCLOCK                 IF_IOCTL_CMD_DEF(MID_IFNET, IF_T1_TYPE, 19) /* ����TDMʱ��ģʽ */
#define IOCTL_E1T1_GET_E1T1MIB                  IF_IOCTL_CMD_DEF(MID_IFNET, IF_T1_TYPE, 20) /* ��ȡE1/T1����ͳ����Ϣ */
#define IOCTL_E1T1_SET_AIS_DETECTION            IF_IOCTL_CMD_DEF(MID_IFNET, IF_T1_TYPE, 21) /* ���ýӿڵ�AIS���ģʽ */
#define IOCTL_E1T1_GET_VIMIB                    IF_IOCTL_CMD_DEF(MID_IFNET, IF_T1_TYPE, 22) /* ��ȡE1VI/T1VI�ӿں���ʱ϶�� */
#define IOCTL_E1T1_GET_VISTATE                  IF_IOCTL_CMD_DEF(MID_IFNET, IF_T1_TYPE, 23) /* �ж�VE1/VT1�ӿ����Ƿ���ں��� */
#define IOCTL_E1T1_SET_ALARM_DETECT             IF_IOCTL_CMD_DEF(MID_IFNET, IF_T1_TYPE, 24) /* ���ü��澯�ź� */
#define IOCTL_E1POSDM_SET_WORK_MODE             IF_IOCTL_CMD_DEF(MID_IFNET, IF_E1_TYPE, 1)  /* ����E1POSDM�Ĺ�����ʽ */
#define IOCTL_E1_SET_CABLETYPE                  IF_IOCTL_CMD_DEF(MID_IFNET, IF_E1_TYPE, 2)  /* ����ʹ�õ������迹���� */

/* E3������������������ */
#define IOCTL_E3_SET_PORT                       IF_IOCTL_CMD_DEF(MID_IFNET, IF_E3_TYPE, 0)  /* ����E3T3����ģʽ */
#define IOCTL_CE3_SET_CLOCK                     IF_IOCTL_CMD_DEF(MID_IFNET, IF_E3_TYPE, 1)  /* ����ʱ��ģʽ */
#define IOCTL_CE3_SET_LOOPBACK                  IF_IOCTL_CMD_DEF(MID_IFNET, IF_E3_TYPE, 2)  /* ���û��� */
#define IOCTL_CE3_SET_NATIONALBIT               IF_IOCTL_CMD_DEF(MID_IFNET, IF_E3_TYPE, 3)  /* ���ù���λ */
#define IOCTL_CE3_SET_FRAMING                   IF_IOCTL_CMD_DEF(MID_IFNET, IF_E3_TYPE, 4)  /* ���ó�֡��ʽ */
#define IOCTL_CE3_SET_BERT                      IF_IOCTL_CMD_DEF(MID_IFNET, IF_E3_TYPE, 5)  /* ����CE3�ӿں�E1ͨ��BERT����ģʽ */
#define IOCTL_FE3_SET_DSUMODE                   IF_IOCTL_CMD_DEF(MID_IFNET, IF_E3_TYPE, 6)  /* ����DSUģʽ */
#define IOCTL_FE3_SET_SUBRATE                   IF_IOCTL_CMD_DEF(MID_IFNET, IF_E3_TYPE, 7)  /* ���������� */

/* T3������������������ */
#define IOCTL_T3_SET_PORT                       IF_IOCTL_CMD_DEF(MID_IFNET, IF_T3_TYPE, 0)   /* ����E3T3����ģʽ */
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
#define IF_E1POSDM_WORKMODE_SYNC                 0   /* Ĭ������ */
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

#define IF_E1_TIMESLOT                          32      /* E1ʱ϶���� */
#define IF_T1_TIMESLOT                          24      /* T1ʱ϶���� */

#define IF_E1_PRICHAN                           16      /* E1 Dͨ�� */
#define IF_T1_PRICHAN                           24      /* T1 Dͨ�� */                                        
                                        
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
#define IF_ATM_CABLE_SHORT                  0   /* �̾����ģʽ */
#define IF_ATM_CABLE_LONG                   1   /* �������ģʽ */

/* IOCTL_ATM_SET_CLOCK */
#define IF_ATM_CLOCKSET_SLAVE               0   /* ��ʱ��ģʽ */
#define IF_ATM_CLOCKSET_MASTER              1   /* ��ʱ��ģʽ */
#define IF_ATM_CLOCKSET_DEFAULT             IF_ATM_CLOCKSET_SLAVE

/* IOCTL_ATM_SET_LINE_CODE */
#define IF_ATM_E1T1_LINECODE_AMI            0
#define IF_ATM_E1T1_LINECODE_B8ZS           1
#define IF_ATM_E1T1_LINECODE_HDB3           2

/* IOCTL_ATM_SET_FRAMING */
#define IF_ATM_FRAMING_SDH                  0   /* SDH֡��ʽ */
#define IF_ATM_FRAMING_SONET                1   /* SONET֡��ʽ */
#define IF_ATM_E3_FRAMING_G751PLCP          2   /* Default, G.751 PLCP��֡��ʽ */
#define IF_ATM_E3_FRAMING_G751ADM           3   /* G.751ֱ�ӳ�֡��ʽ */
#define IF_ATM_E3_FRAMING_G832ADM           4   /* G.832ֱ�ӳ�֡��ʽ */
#define IF_ATM_T3_FRAMING_CBITPLCP          5   /* Default, C-bit PLCP��֡��ʽ */
#define IF_ATM_T3_FRAMING_CBITADM           6   /* C-bitֱ�ӳ�֡��ʽ */
#define IF_ATM_T3_FRAMING_M23ADM            7   /* m23ֱ�ӳ�֡��ʽ */
#define IF_ATM_T3_FRAMING_M23PLCP           8   /* m23 PLCP��֡��ʽ */
#define IF_ATM_E1_FRAMING_CRC4              9   /* Default, CRC4 ADM֡��ʽ */
#define IF_ATM_E1_FRAMING_NOCRC4            10  /* No-CRC4 ADM֡��ʽ */
#define IF_ATM_T1_FRAMING_ESF               11  /* Default, ESF֡��ʽ */
#define IF_ATM_T1_FRAMING_SF                12  /* SF֡��ʽ */

/* IOCTL_ATM_SET_LOOPBACK */
#define IF_ATM_LOOPBACK_NO                  0   /* default����ֹATM�ӿڻ��� */
#define IF_ATM_LOOPBACK_CELL                1   /* cell������ATM�ӿڶ�����Ԫ�Ի� */
#define IF_ATM_LOOPBACK_PAYLOAD             2   /* payload������ATM�ӿڶ����غɻ��� */
#define IF_ATM_LOOPBACK_LINE                3   /* remote������ATM�ӿڶ��⻷�� */
#define IF_ATM_LOOPBACK_LOCAL               4   /* local������ATM�ӿڶ����Ի� */

/* IOCTL_ATM_SET_SCRAMBLE */
#define IF_ATM_SCRAMBLE_ENABLE              0   /* default��ʹ�ܼ��Ź��� */
#define IF_ATM_SCRAMBLE_DISABLE             1   /* ��ֹ���Ź��� */
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
#define IF_ATM_IMA_TESTIMA                  0                       /* ����IMA����·���� */
#define IF_ATM_IMA_NOTTESTIMA               1                       /* ������IMA����·���� */
#define IF_ATM_IMA_IMATESTDEFAULT           IF_ATM_IMA_NOTTESTIMA   /* Ĭ�ϲ�������� */

/* IOCTL_ATM_IMA_MIN_ACTIVE_LINKS*/
#define IF_ATM_IMA_MIN_ACTIVE_LINKS_DEFAULT        1            /* ima-group�����С������·�� */

/* IOCTL_ATM_SET_FLAG_C2 */
#define IF_ATM_FLAG_C2_DEFAULT              0x13    /* Ĭ������ */
#define IF_ATM_FLAG_C2_MIN                  0x00    /* C2 FLAG ����С���� */
#define IF_ATM_FLAG_C2_MAX                  0xff    /* C2 FLAG ��������� */

/* IOCTL_ATM_SET_FLAG_J0 / IOCTL_ATM_SET_FLAG_J1 */
#define IF_ATM_FLAG_SDH_J0                  0x01    /* J0�����ֽ�ʹ��SDH, 16�ֽ�ģʽ */
#define IF_ATM_FLAG_SDH_J1                  0x02    /* J1�����ֽ�ʹ��SDH, 16�ֽ�ģʽ */
#define IF_ATM_FLAG_SONET_J0                0x04    /* J0�����ֽ�ʹ��SONET, 1�ֽ�ģʽ */
#define IF_ATM_FLAG_SONET_J1                0x08    /* J1�����ֽ�ʹ��SONET, 64�ֽ�ģʽ */

#define IF_ATM_FLAG_J0_SONET_DEFAULT        0x01    /* J0���ֽ������Ĭ��ֵΪ1*/

#define IF_ATM_FLAG_J0_SDH_LENGTH           16      /* J0��16�ֽ�ģʽ�µĳ���Ϊ16 */
#define IF_ATM_FLAG_J1_SDH_LENGTH           16      /* J1��SDHģʽ����16�ֽ�ģʽ */
#define IF_ATM_FLAG_J1_SONET_LENGTH         64      /* J1��SONETģʽ����64�ֽ�ģʽ */

#define IF_ATM_MAX_FLAG_LENGTH              IF_ATM_FLAG_J1_SONET_LENGTH

/* IOCTL_ATM_DSL_SET_ACTIVATE */
#define IF_ATM_DSL_ACTIVATE                 0   /* ����ADSL��· */
#define IF_ATM_DSL_NO_ACTIVATE              1   /* ȥ����ADSL��· */

/* IOCTL_ATM_DSL_SET_STANDARD */
#define IF_ATM_DSL_STANDARD_TYPE_T1413      0   /* ʹ��T1.413��׼ */
#define IF_ATM_DSL_STANDARD_TYPE_GLITE      1   /* ʹ��G.Lite(G992.2)��׼ */
#define IF_ATM_DSL_STANDARD_TYPE_GDMT       2   /* ʹ��G.DMT(G992.1)��׼ */
#define IF_ATM_DSL_STANDARD_TYPE_AUTO       3   /* Default, ����Ӧ��ʽ */
#define IF_ATM_DSL_STANDARD_TYPE_ADSL2      4   /* ʹ��G992.3��׼ */
#define IF_ATM_DSL_STANDARD_TYPE_ADSL2PLUS  5   /* ʹ��G992.5��׼ */

/* IOCTL_ATM_DSL_SET_TX_ATTENUATION */
#define IF_ATM_DSL_TX_ATTENUATION_DEFAULT   0

/* IOCTL_ATM_DSL_SET_ANNEX */
#define IF_ATM_DSL_SHDSL_ANNEX_TYPE_A       0   /* ֧�ֵ�annex��׼Ϊ a */
#define IF_ATM_DSL_SHDSL_ANNEX_TYPE_B       1   /* ֧�ֵ�annex��׼Ϊ b */
#define IF_ATM_DSL_SHDSL_ANNEX_TYPE_DEFAULT IF_ATM_DSL_SHDSL_ANNEX_TYPE_B

/* IOCTL_ATM_DSL_SET_CAPABILITY */
#define  IF_ATM_DSL_SHDSL_CAPABILITY_AUTO          0
#define  IF_ATM_DSL_SHDSL_CAPABILITY_GSHDSLBIS     1
#define  IF_ATM_DSL_SHDSL_CAPABILITY_GSHDSL        2

/* IOCTL_ATM_DSL_SET_MODE */
#define IF_ATM_DSL_SHDSL_MODE_TYPE_CPE      0   /* �û���ģʽ */
#define IF_ATM_DSL_SHDSL_MODE_TYPE_CO       1   /* CO���ֶˣ�ģʽ */
#define IF_ATM_DSL_SHDSL_MODE_TYPE_DEFAULT  IF_ATM_DSL_SHDSL_MODE_TYPE_CPE

/* IOCTL_ATM_DSL_SET_PAM */
#define IF_ATM_DSL_SHDSL_PAM_AUTO           0
#define IF_ATM_DSL_SHDSL_PAM_16             1
#define IF_ATM_DSL_SHDSL_PAM_32             2

/* IOCTL_ATM_DSL_PBO */
#define IF_ATM_DSL_SHDSL_PBO_AUTO           0xffff

/* IOCTL_ATM_DSL_SET_PSD */
#define IF_ATM_DSL_SHDSL_PSD_TYPE_SYMMETRY  0   /* �ԳƷ�ʽ */
#define IF_ATM_DSL_SHDSL_PSD_TYPE_ASYMMETRY 1   /* �ǶԳƷ�ʽ */
#define IF_ATM_DSL_SHDSL_PSD_TYPE_DEFAULT   IF_ATM_DSL_SHDSL_PSD_TYPE_SYMMETRY

/* IOCTL_ATM_DSL_SET_LINE_RATE */
#define IF_ATM_DSL_SHDSL_LINE_RATE_TYPE_AUTO       0   /* ����Ӧ��ʽ */
#define IF_ATM_DSL_SHDSL_LINE_RATE_TYPE_DEFAULT    1   /* ����ΪĬ��ֵ */
#define IF_ATM_DSL_SHDSL_4WIRE_RATE_TYPE_DEFAULT   2312

/* IOCTL_ATM_DSL_SET_MARGIN */
#define IF_ATM_DSL_SHDSL_CURRENT_MARGIN_DEFAULT 2
#define IF_ATM_DSL_SHDSL_SNEXT_MARGIN_DEFAULT   0

/* IOCTL_ATM_DSL_SET_WIRE */
#define IF_ATM_DSL_SHDSL_WIRE_TYPE_TWO              0   /*����ģʽ*/
#define IF_ATM_DSL_SHDSL_WIRE_TYPE_FOUR             1   /*����enhancedģʽ*/
#define IF_ATM_DSL_SHDSL_WIRE_TYPE_FOUR_AUTO        2   /*�����Զ��л�ģʽ*/
#define IF_ATM_DSL_SHDSL_WIRE_TYPE_FOUR_STANDARD    3   /*����standardģʽ */
/* ����HSDSL�ӿ���·ģʽĬ��ֵΪ����enhanced */
#define IF_ATM_DSL_SHDSL_WIRE_TYPE_DEFAULT          IF_ATM_DSL_SHDSL_WIRE_TYPE_FOUR
/* G.BIS������HSDSL�ӿ���·ģʽĬ��ֵΪ����standard */
#define IF_ATM_DSL_SHDSL_GBIS_WIRE_TYPE_DEFAULT     IF_ATM_DSL_SHDSL_WIRE_TYPE_FOUR_STANDARD
/*�Զ��л�ģʽ*/
#define IF_ATM_DSL_SHDSL_WIRE_TYPE_AUTO             IF_ATM_DSL_SHDSL_WIRE_TYPE_FOUR_AUTO
#define IF_ATM_DSL_SHDSL_WIRE_TYPE_SIX              4   /*����ģʽ */
#define IF_ATM_DSL_SHDSL_WIRE_TYPE_EIGHT            5   /*����ģʽ */
/* ����HSDSL�ӿ���·ģʽĬ��ֵΪ���� */
#define IF_ATM_DSL_SHDSL_EIGHT_GBIS_WIRE_TYPE_DEFAULT    IF_ATM_DSL_SHDSL_WIRE_TYPE_EIGHT
/* EFM�ӿڹ���ģʽ:�ǰ󶨺Ͱ� */
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

/* adslLineTable�� */
/* ����adslLineCodingΪͨ���������ͣ�ȡֵΪ���µĺ꣺*/
#define IF_ATM_DSL_LINE_CODING_TYPE_OTHER 1 /* none of the following */
#define IF_ATM_DSL_LINE_CODING_TYPE_DMT   2 /* Discrete MultiTone */
#define IF_ATM_DSL_LINE_CODING_TYPE_CAP   3 /* Carrierless Amplitude & Phase modulation */
#define IF_ATM_DSL_LINE_CODING_TYPE_QAM   4 /* Quadrature Amplitude Modulation */

/* ����adslLineTypeΪͨ���������ͣ�ȡֵΪ���µĺ� */
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
    UINT uiOIDLen;   /* OID ���� */
    UINT auiOID[IF_SNMP_MAX_OID_LEN]; /* array to store ids */
    CHAR acLineConfProfile[IF_ATM_DSL_LINE_PROFILE_MAXLEN + 1];
    CHAR acLineAlarmConfProfile[IF_ATM_DSL_LINE_PROFILE_MAXLEN + 1];
}IF_ATM_DSL_LINE_INFO_S;

/* adslAtucPhysTable���adslAturPhysTable�� */
/* ����uiAtuType��ʾ��Ҫ��ȡ�ı���������ȡֵΪ���º� */
#define IF_ATM_DSL_ATU_PHY_TYPE_CENTRAL 1 /* �ֶ� */
#define IF_ATM_DSL_ATU_PHY_TYPE_REMOTE  2 /* Զ�� */

#define IF_ATM_DSL_ATU_MAXLEN_CURRSTATUS 2
#define IF_ATM_DSL_ATU_MAXLEN_SERIALNUMBER  32
#define IF_ATM_DSL_ATU_MAXLEN_VENDORID      16
#define IF_ATM_DSL_ATU_MAXLEN_VERSIONNUMBER 16

/*
    acAtuCurrStatus�ֶ�ΪAtu��ǰ״̬��λͼ,λͼ����λ������λ��ʼ�㣬���δ������˼Ϊ
    �ֶˣ�Զ��ͨ�ò���:
    "noDefect";
    "lossOfFraming";
    "lossOfSignal";
    "lossOfPower";
    "lossOfSignalQuality";

    �ֶ����в���:
    "lossOfLink";
    "dataInitFailure";
    "configInitFailure";
    "protocolInitFailure";
    "noPeerAtuPresent";
*/

/* acAtuCurrStatusΪAtu��ǰ״̬��λͼ,Ϊ���º�Ļ�ֵ */
/* acAtuCurrStatus[0]�ĸ�ֵ */
#define IF_ATM_DSL_ATU_PHY_STATUS_NODEFECT              (UCHAR)0x80 /* noDefect */
#define IF_ATM_DSL_ATU_PHY_STATUS_LOSSOFFRAMING         (UCHAR)0x40 /* lossOfFraming */
#define IF_ATM_DSL_ATU_PHY_STATUS_LOSSOFSIGNAL          (UCHAR)0x20 /* lossOfSignal */
#define IF_ATM_DSL_ATU_PHY_STATUS_LOSSOFPOWER           (UCHAR)0x10 /* lossOfPower */
#define IF_ATM_DSL_ATU_PHY_STATUS_LOSSOFSIGNALQUALITY   (UCHAR)0x08 /* lossOfSignalQuality */
#define IF_ATM_DSL_ATU_PHY_STATUS_LOSSOFLINK            (UCHAR)0x04 /* lossOfLink,       �ֶ����� */
#define IF_ATM_DSL_ATU_PHY_STATUS_DATAINITFAILURE       (UCHAR)0x02 /* dataInitFailure,  �ֶ����� */
#define IF_ATM_DSL_ATU_PHY_STATUS_CONFIGINITFAILURE     (UCHAR)0x01 /* configInitFailure,�ֶ����� */

/* acAtuCurrStatus[1]�ĸ�ֵ */
#define IF_ATM_DSL_ATU_PHY_STATUS_PROTOCOLINITFAILURE   (UCHAR)0x80 /* protocolInitFailure,�ֶ����� */
#define IF_ATM_DSL_ATU_PHY_STATUS_NOPEERATUPRESENT      (UCHAR)0x40 /* noPeerAtuPresent,   �ֶ����� */

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

/* adslAtucChanTable���adslAturChanTable�� */
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
    UINT uiInterfaceInErrs;                     /* E1/T1�ӿ��յ�������� */
    UINT uiInterfaceInRuntsErrs;                /* E1/T1�ӿ��յ��ĳ��̰��Ĵ����� */
    UINT uiInterfaceInGiantsErrs;               /* E1/T1�ӿ��յ��ĳ������Ĵ���� */
    UINT uiInterfaceInCrcErrs;                  /* E1/T1�ӿ��յ���CRC����� */
    UINT uiInterfaceInAlignErrs;                /* E1/T1�ӿ��յ���Align����� */
    UINT uiInterfaceInOverRunsErrs;             /* E1/T1�ӿ��յ���OverRun����� */
    UINT uiInterfaceInDribblesErrs;             /* E1/T1�ӿ��յ���Dribble����� */
    UINT uiInterfaceInAbortedSeqErrs;           /* E1/T1�ӿ��յ���AbortedSeq����� */
    UINT uiInterfaceInNoBufferErrs;             /* E1/T1�ӿ��յ���NoBuffer����� */
    UINT uiInterfaceInFramingErrs;              /* E1/T1�ӿ��յ���Framing����� */
    UINT uiInterfaceOutputErrs;                 /* E1/T1�ӿڷ����Ĵ���� */
    UINT uiInterfaceOutUnderRunErrs;            /* E1/T1�ӿڷ�����UnderRun����� */
    UINT uiInterfaceOutCollisonsErrs;           /* E1/T1�ӿڷ�����Collison����� */
    UINT uiInterfaceOutDeferedErrs;             /* E1/T1�ӿڷ�����OutDefered����� */
}IF_E1T1_MIB_S;

typedef struct tagIFE1T1VIMIB
{
    UINT uiE1T1VIUsingTimeslots;                /* E1/T1VI�ӿ�ռ��ʱ϶���� */
}IF_E1T1VI_MIB_S;

typedef struct tagIfCT3MdlDataStr
{
    UINT uiChanNo;
    UINT uiMDLDataType;
    UINT uiOperType;
    CHAR szStr[IF_MDL_DATA_LEN + 1];
    UCHAR ucReserved;
}IF_CT3_MDL_DATA_S;

/* ===END: E1/T1/E3/T3�����֡���=== */


/* ===BEGIN: POS�����֡���=== */
/* POS�ӿ�PhyIOCtl������ */
#define IOCTL_POS_SET_CRC_LENGTH                IF_IOCTL_CMD_DEF(MID_IFNET, IF_POS_TYPE, 1) /* ����POS�ӿڵ�ʱ��ģʽ */
#define IOCTL_POS_SET_CLOCK                     IF_IOCTL_CMD_DEF(MID_IFNET, IF_POS_TYPE, 2) /* ����POS�ӿڵ�CRC���� */
#define IOCTL_POS_SET_SCRAMBLING                IF_IOCTL_CMD_DEF(MID_IFNET, IF_POS_TYPE, 3) /* ����POS�ӿڼ��� */
#define IOCTL_POS_SET_FRAMING                   IF_IOCTL_CMD_DEF(MID_IFNET, IF_POS_TYPE, 4) /* ����POS�ӿڵ�֡��ʽ */
#define IOCTL_POS_SET_LOOPBACK                  IF_IOCTL_CMD_DEF(MID_IFNET, IF_POS_TYPE, 5) /* ����POS�ӿڵĻ���ģʽ */
#define IOCTL_POS_SET_FLAG                      IF_IOCTL_CMD_DEF(MID_IFNET, IF_POS_TYPE, 6) /* ����SDH/SONET�����ֽ� */
#define IOCTL_POS_SET_SPEED                     IF_IOCTL_CMD_DEF(MID_IFNET, IF_POS_TYPE, 7) /* ����POS�ӿڵ�SPEED */

/* POS�����ֶ�Ӧ�Ĳ������� */
/* IOCTL_POS_SET_CRC_LENGTH */
#define IF_POS_CRC_32                           0      /* 32λCRC��Ĭ��ֵ */
#define IF_POS_CRC_16                           1      /* 16λCRC */
#define IF_POS_CRC_DEFAULT                      IF_POS_CRC_32

/* IOCTL_POS_SET_CLOCK */
#define IF_POS_CLOCK_DEFAULT                    IF_CLOCK_LINE

/* IOCTL_POS_SET_SCRAMBLING */
#define IF_POS_SCRAMBLE                         0       /* ���غ����ݼ��ţ�Ĭ��ֵ */
#define IF_POS_SCRAMBLE_NONE                    1       /* ��ֹ���غ����ݼ��� */
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

/* rpr pos �����޸澯Ĭ��ֵ���� */
#define IF_POS_RPR_BER_SD_DEFAULT_VALUE         5
#define IF_POS_RPR_BER_SF_DEFAULT_VALUE         3

/* IOCTL_POS_SET_FLAG */
#define IF_POS_FLAG_C2                          0       /* �źű���ֽ�C2 */
#define IF_POS_FLAG_J0                          1       /* �����θ����ֽ�J0 */
#define IF_POS_FLAG_J1                          2       /* ͨ�������ֽ�J1 */

#define IF_POS_FLAG_TYPE_IGNORE                 0       /* ����J1��� */
#define IF_POS_FLAG_TYPE_BYTE                   1       /* ����Ϊ1�ֽ� */
#define IF_POS_FLAG_TYPE_STRING16               16      /* ����Ϊ16�ֽ�(�������ó���Ϊ15����һ���ֽ�����У��) */
#define IF_POS_FLAG_TYPE_STRING64               64      /* ����Ϊ64�ֽ�(�������ó���Ϊ62), ���2�ֽ�Ϊ0x0d0a */

#define IF_POS_FLAG_LEN_STR16                   15UL
#define IF_POS_FLAG_LEN_STR64                   62UL
#define IF_POS_FLAG_LEN_MAX                     64UL

#define IF_POS_FLAG_C2_DEFAULT                  0x16    /* C2Ĭ��ֵ */
#define IF_POS_FLAG_J0_SONET_DEFAULT            0x01    /* J0 SONET��ʽĬ��ֵ */

#define IF_POS_FLAG_IGNORE_ENABLE               0x1     /* ʹ�ܺ��Լ�� */
#define IF_POS_FLAG_IGNORE_DISABLE              0x2     /* ȥʹ�ܺ��Լ�� */

#define IF_POS_FLAG_SDH_LEN     IF_POS_FLAG_LEN_STR16   /* SDH��ʽ�������� */
#define IF_POS_FLAG_SONET_LEN   IF_POS_FLAG_LEN_STR64   /* SONET��ʽJ1���� */

ISSU typedef struct tagIfPOSFlagCmd
{
    UINT uiFlag;                                        /* Flag���� */
    UINT uiLength;                                      /* Flag���� */
    UCHAR aucVal[IF_POS_FLAG_LEN_MAX];                  /* Flagֵ */
}IF_POS_FLAG_CMD_S, *PIF_POS_FLAG_CMD_S;
/* ===END: POS�����֡��ꡢ�ṹ=== */


/* ===BEGIN: CPOS�������������֡��ꡢ�ṹ=== */
#define IOCTL_CPOS_SET_CLOCK                    IF_IOCTL_CMD_DEF(MID_IFNET, IF_CPOS_TYPE, 1) /* ����CPOS�ӿ�ʱ��ģʽ */
#define IOCTL_CPOS_SET_FRAMING                  IF_IOCTL_CMD_DEF(MID_IFNET, IF_CPOS_TYPE, 2) /* ����CPOS�ӿڵ�֡��ʽ */
#define IOCTL_CPOS_SET_LOOPBACK                 IF_IOCTL_CMD_DEF(MID_IFNET, IF_CPOS_TYPE, 3) /* ����CPOS�ӿڵĻ���ģʽ */
#define IOCTL_CPOS_SET_MAPPING                  IF_IOCTL_CMD_DEF(MID_IFNET, IF_CPOS_TYPE, 4) /* ����AUG�ĸ���ģʽ */
#define IOCTL_CPOS_SET_FLAG                     IF_IOCTL_CMD_DEF(MID_IFNET, IF_CPOS_TYPE, 5) /* ����SDH/SONET�����ֽ� */
#define IOCTL_CPOS_SET_E1_CLOCK                 IF_IOCTL_CMD_DEF(MID_IFNET, IF_CPOS_TYPE, 6) /* ����CPOS��E1ͨ����ʱ��ģʽ */
#define IOCTL_CPOS_SET_T1_CLOCK                 IF_IOCTL_CMD_DEF(MID_IFNET, IF_CPOS_TYPE, 7) /* ����CPOS��T1ͨ����ʱ��ģʽ */
#define IOCTL_CPOS_SET_E1_FRAMING               IF_IOCTL_CMD_DEF(MID_IFNET, IF_CPOS_TYPE, 8) /* ����CPOS��E1ͨ����֡��ʽ */
#define IOCTL_CPOS_SET_T1_FRAMING               IF_IOCTL_CMD_DEF(MID_IFNET, IF_CPOS_TYPE, 9) /* ����CPOS��T1ͨ����֡��ʽ */
#define IOCTL_CPOS_SET_E1_LOOPBACK              IF_IOCTL_CMD_DEF(MID_IFNET, IF_CPOS_TYPE, 10)/* ����CPOS��E1ͨ������ģʽ */
#define IOCTL_CPOS_SET_T1_LOOPBACK              IF_IOCTL_CMD_DEF(MID_IFNET, IF_CPOS_TYPE, 11)/* ����CPOS��T1ͨ������ģʽ */
#define IOCTL_CPOS_SET_PORT                     IF_IOCTL_CMD_DEF(MID_IFNET, IF_CPOS_TYPE, 12)/* ���ü�ǿCPOS�ӿ�ģʽ�ı� */

#define IOCTL_CPOS_SET_E3_CLOCK                 IF_IOCTL_CMD_DEF(MID_IFNET, IF_CPOS_TYPE, 13)/* ����CPOS��E3ͨ����ʱ��ģʽ */
#define IOCTL_CPOS_SET_T3_CLOCK                 IF_IOCTL_CMD_DEF(MID_IFNET, IF_CPOS_TYPE, 14)/* ����CPOS��T3ͨ����ʱ��ģʽ */
#define IOCTL_CPOS_SET_E3_LOOPBACK              IF_IOCTL_CMD_DEF(MID_IFNET, IF_CPOS_TYPE, 15)/* ����CPOS��E3ͨ������ģʽ */
#define IOCTL_CPOS_SET_T3_LOOPBACK              IF_IOCTL_CMD_DEF(MID_IFNET, IF_CPOS_TYPE, 16)/* ����CPOS��T3ͨ������ģʽ */
#define IOCTL_CPOS_SET_E3_FLAG                  IF_IOCTL_CMD_DEF(MID_IFNET, IF_CPOS_TYPE, 17)/* ����CPOS��E3ͨ��SDH/SONET�����ֽ� */
#define IOCTL_CPOS_SET_T3_FLAG                  IF_IOCTL_CMD_DEF(MID_IFNET, IF_CPOS_TYPE, 18)/* ����CPOS��T3ͨ��SDH/SONET�����ֽ� */
#define IOCTL_CPOS_SET_FE3_DSUMODE              IF_IOCTL_CMD_DEF(MID_IFNET, IF_CPOS_TYPE, 19)/* ����CPOS��E3ͨ��DSUģʽ */
#define IOCTL_CPOS_SET_FE3_SUBRATE              IF_IOCTL_CMD_DEF(MID_IFNET, IF_CPOS_TYPE, 20)/* ����CPOS��E3ͨ�������� */
#define IOCTL_CPOS_SET_FT3_DSUMODE              IF_IOCTL_CMD_DEF(MID_IFNET, IF_CPOS_TYPE, 21)/* ����CPOS��T3ͨ��DSUģʽ */
#define IOCTL_CPOS_SET_FT3_SUBRATE              IF_IOCTL_CMD_DEF(MID_IFNET, IF_CPOS_TYPE, 22)/* ����CPOS��T3ͨ�������� */
#define IOCTL_CPOS_SET_E3_NATIONALBIT           IF_IOCTL_CMD_DEF(MID_IFNET, IF_CPOS_TYPE, 23)/* ����CPOS��E3ͨ������λ */
#define IOCTL_CPOS_SET_T3_FRAMING               IF_IOCTL_CMD_DEF(MID_IFNET, IF_CPOS_TYPE, 24)/* ����CPOS��T3ͨ����֡��ʽ */
#define IOCTL_CPOS_SET_T3_FEAC_DETECT           IF_IOCTL_CMD_DEF(MID_IFNET, IF_CPOS_TYPE, 25)/* ����CPOS��T3ͨ���Ķ�ʱ���FEAC��·�źŹ��� */
#define IOCTL_CPOS_T3_FEAC_GENERATE             IF_IOCTL_CMD_DEF(MID_IFNET, IF_CPOS_TYPE, 26)/* CPOS��T3ͨ������FEAC�ź� */
#define IOCTL_CPOS_SET_T3_ALARM_DETECT          IF_IOCTL_CMD_DEF(MID_IFNET, IF_CPOS_TYPE, 27)
#define IOCTL_CPOS_SET_T3_ALARM_GENERATE        IF_IOCTL_CMD_DEF(MID_IFNET, IF_CPOS_TYPE, 28)/* ����CPOS��T3ͨ��ALARM */
#define IOCTL_CPOS_SET_T3_BERT                  IF_IOCTL_CMD_DEF(MID_IFNET, IF_CPOS_TYPE, 29)
#define IOCTL_CPOS_SET_T3_MDL_DETECT            IF_IOCTL_CMD_DEF(MID_IFNET, IF_CPOS_TYPE, 30)
#define IOCTL_CPOS_T3_MDL_GENERATE              IF_IOCTL_CMD_DEF(MID_IFNET, IF_CPOS_TYPE, 31)/* ����CPOS��T3ͨ��MDL */
#define IOCTL_CPOS_SET_T3_MDL_DATA              IF_IOCTL_CMD_DEF(MID_IFNET, IF_CPOS_TYPE, 32)
#define IOCTL_CPOS_T3_MDL_UNDO                  IF_IOCTL_CMD_DEF(MID_IFNET, IF_CPOS_TYPE, 33)
#define IOCTL_CPOS_SET_AU3TUG3                  IF_IOCTL_CMD_DEF(MID_IFNET, IF_CPOS_TYPE, 34)/* �����ںϽӿڵ��ں�״̬ */
#define IOCTL_CPOS_CHECK_CHANNEL                IF_IOCTL_CMD_DEF(MID_IFNET, IF_CPOS_TYPE, 35)/* ִ��ͨ��������ǰ������� */
#define IOCTL_CPOS_SET_MULTI_CHANNEL            IF_IOCTL_CMD_DEF(MID_IFNET, IF_CPOS_TYPE, 36)/* ����CPOS�ӿ���channel-align-mode */

/* CPOS��غ궨�� */
#define IF_CPOS_FLAG_TU3_VC3                    1          
#define IF_CPOS_FLAG_TU3_VC4                    2   
                                                
#define IF_CPOS_MAXCHANNUM                      256     /* ÿ��CPOS�µ�����ͨ������������ */
                                                
/* CPOS�ںϿ�ͨ���ںϱ�� */                    
#define IF_CPOS_JOINFLAG_E1T1                   1    
#define IF_CPOS_JOINFLAG_E3T3                   2
#define IF_CPOS_JOINFLAG_NULL                   0
                                                
/* CPOS�ӿ��ں�������״̬�� */
#define IF_CPOS_CHANTYPE_E3                     1
#define IF_CPOS_CHANTYPE_E1                     2
#define IF_CPOS_CHANTYPE_T3                     3
#define IF_CPOS_CHANTYPE_T1                     4
#define IF_CPOS_CHANTYPE_DEFAULT                5  

#define IF_CPOS_2DOT5GBITS  3          /*2.5Gͨ����ռͨ�����λ*/
#define IF_CPOS_622MBITS    3          /*622Mͨ����ռͨ�����λ*/
#define IF_CPOS_155MBITS    3          /*155Mͨ����ռͨ�����λ*/
#define IF_CPOS_E3T3BITS    6          /*E3/T3ͨ����ռͨ�����λ*/
#define IF_CPOS_E1T1BITS    13         /*E1/T1ͨ����ռͨ�����λ*/
#define IF_CPOS_RESERVED    4          /*����λ*/

#define IF_CPOS_2DOT5GMASK  0x7        /*2.5Gͨ���������*/
#define IF_CPOS_622MMASK    0x7        /*622Mͨ���������*/
#define IF_CPOS_155MMASK    0x7         /*155Mͨ���������*/
#define IF_CPOS_E3T3MASK    0x3f       /*E3/T3ͨ���������*/
#define IF_CPOS_E1T1MASK    0x1fff     /*E1/T1ͨ���������*/

/*����ȡһ��ͨ��,��ͨ����2.5GPosͨ����*/
#define IF_GET_2DOT5GCHANNEL(uiChannel) (((uiChannel) \
                                         >>  (IF_CPOS_E1T1BITS + IF_CPOS_E3T3BITS + IF_CPOS_155MBITS + IF_CPOS_622MBITS + IF_CPOS_RESERVED)) \
                                         & (IF_CPOS_2DOT5GMASK))

/*����ȡ����ͨ��,��ͨ����622Mͨ����*/
#define IF_GET_622MCHANNEL(uiChannel) (((uiChannel) \
                                        >> (IF_CPOS_E1T1BITS + IF_CPOS_E3T3BITS + IF_CPOS_155MBITS + IF_CPOS_RESERVED)) \
                                        & (IF_CPOS_622MMASK))

/*����ȡ����ͨ��,��ͨ����155ͨ����*/
#define IF_GET_155MCHANNEL(uiChannel) (((uiChannel) >> (IF_CPOS_E1T1BITS + IF_CPOS_E3T3BITS + IF_CPOS_RESERVED)) & (IF_CPOS_155MMASK))

/*����ȡ�Ľ�ͨ��,��ͨ����E3/T3ͨ����*/
#define IF_GET_E3T3CHANNEL(uiChannel) (((uiChannel) >> (IF_CPOS_E1T1BITS + IF_CPOS_RESERVED)) & (IF_CPOS_E3T3MASK))

/*����ȡ���ͨ��,��ͨ����E1/T1ͨ����*/
#define IF_GET_E1T1CHANNEL(uiChannel) (((uiChannel) >> (IF_CPOS_RESERVED)) & (IF_CPOS_E1T1MASK))

/* CPOS�����ֶ�Ӧ�Ĳ������� */                  
/* IOCTL_CPOS_SET_CLOCK  */                     
#define IF_CPOS_CLOCK_DEFAULT                   IF_CLOCK_LINE
                                                
/* IOCTL_CPOS_SET_FRAMING  */                   
#define IF_CPOS_FRAME_DEFAULT                   IF_FRAME_SDH

/* IOCTL_CPOS_SET_LOOPBACK  */
#define IF_CPOS_LOOPBACK_DEFAULT                IF_LOOPBACK_NOLOOP
                                                
/* IOCTL_CPOS_SET_MAPPING  */                   
#define IF_CPOS_MAPPING_AU4                     0       /* AUGͨ��AU-4�õ���default */
#define IF_CPOS_MAPPING_AU3                     1       /* AUGͨ��AU-3�õ� */
#define IF_CPOS_MAPPING_DEFAULT                 IF_CPOS_MAPPING_AU4
                                                
#define IF_CPOS_SUB_CLOCK_DEFAULT               IF_CLOCK_LINE

/* IOCTL_CPOS_SET_T1_FRAMING  */                
#define IF_CPOS_SUB_FRAMING_DEFAULT             IF_T1_FRAMING_ESF
                                                
#define IF_CPOS_SUB_LOOPBACK_DEFAULT            IF_LOOPBACK_NOLOOP

/* IOCTL_CPOS_SET_FE3_DSUMODE */
/* Digital Link��֧�������ʷ�ΧΪ358-34010kbps����95�����ʵȼ�������358000bps, default */
#define IF_CPOS_FE3_DSUMODE_0                   0
/* Kentrox��֧�������ʷ�ΧΪ500-24500,34010kbps����50�����ʵȼ�������500000 bps */
#define IF_CPOS_FE3_DSUMODE_1                   1
#define IF_CPOS_FE3_DSUMODE_DEFAULT             1

/* IOCTL_CPOS_SET_FE3_SUBRATE */
#define IF_CPOS_FE3_SUBRATE_DEFAULT             34010

/* IOCTL_CPOS_SET_FT3_DSUMODE */
/* Digital Link��֧�������ʷ�ΧΪ300-44210 kbps����147�����ʵȼ�������300746 bps, default */
#define IF_CPOS_FT3_DSUMODE_0                   0
/* Kentrox��֧�������ʷ�ΧΪ1500-35000 kbps��44210 kbps����57�����ʵȼ�������1500000 bps����Ŀǰ�в�֧�֣�*/
#define IF_CPOS_FT3_DSUMODE_1                   1
/* Larscom��֧�������ʷ�ΧΪ3100-44210 kbps����14�����ʵȼ�������3157835 bps */
#define IF_CPOS_FT3_DSUMODE_2                   2
/* Adtran��֧�������ʷ�ΧΪ75-44210 kbps����588�����ʵȼ�������75187 bps */
#define IF_CPOS_FT3_DSUMODE_3                   3
/* Verilink��֧�������ʷ�ΧΪ1500-44210 kbps����20�����ʵȼ�������1578918 bps */
#define IF_CPOS_FT3_DSUMODE_4                   4
#define IF_CPOS_FT3_DSUMODE_DEFAULT             0
                                                
/* IOCTL_CPOS_SET_FT3_SUBRATE */                
#define IF_CPOS_FT3_SUBRATE_DEFAULT             44210
                                                
/* IOCTL_CPOS_SET_T3_FEAC_DETECT */
#define IF_CPOS_T3_FEAC_DETECT_DEFAULT          IF_DETECT_ON
                                                
/* IOCTL_CPOS_SET_FLAG  */
#define IF_CPOS_FLAG_C2                         0       /* �źű���ֽ�C2 */
#define IF_CPOS_FLAG_J0                         1       /* �����θ����ֽ�J0 */
#define IF_CPOS_FLAG_J1                         2       /* �߽�ͨ�������ֽ�J1 */
#define IF_CPOS_FLAG_S1S0                       3       /* H1��SSλ, ָʾAU��TU���� */
#define IF_CPOS_FLAG_S1                         4       /* ͬ��״̬ */
#define IF_CPOS_FLAG_J2                         5       /* �ͽ�ͨ�������ֽ�J2 */
#define IF_CPOS_FLAG_LPC2                       6       /* �ͽ�ͨ���źű���ֽ�C2 */
#define IF_CPOS_FLAG_C2_TU3                     7
#define IF_CPOS_FLAG_J1_TU3                     8
#define IF_CPOS_FLAG_S1S0_TU3                   9
                                                
#define IF_CPOS_FLAG_TYPE_BYTE                  1       /* ����Ϊ1�ֽ� */
#define IF_CPOS_FLAG_TYPE_STRING16              16      /* ����Ϊ16�ֽ�(�������ó���Ϊ15����һ���ֽ�����У��) */
#define IF_CPOS_FLAG_TYPE_STRING64              64      /* ����Ϊ64�ֽ�(�������ó���Ϊ62), ���2�ֽ�Ϊ0x0d0a */
                                                
#define IF_CPOS_FLAG_LEN_STR16                  15
#define IF_CPOS_FLAG_LEN_STR64                  62
#define IF_CPOS_FLAG_LEN_MAX                    64
                                                
#define IF_CPOS_FLAG_VC3_C2_DEFAULT             0x04    /* E3/T3 CPOS?TU3 C2��Ĭ��ֵ */
#define IF_CPOS_FLAG_C2_DEFAULT                 0x02    /* C2Ĭ��ֵ(TUG) */
                                                
#define IF_CPOS_FLAG_J0_SONET_DEFAULT           0x01    /* J0 SONET��ʽĬ��ֵ */
                                                
#define IF_CPOS_FLAG_S1S0_SONET_DEFAULT         0x00    /* S1S0 SONETĬ��ֵ */
#define IF_CPOS_FLAG_S1S0_SDH_DEFAULT           0x02    /* S1S0 SDHĬ��ֵ */
                                                
#define IF_CPOS_FLAG_S1_DEFAULT                 0x0f    /* S1Ĭ��ֵ(��������ʱ��ͬ��) */
#define IF_CPOS_FLAG_LPC2_DEFAULT               0x02    /* LPC2Ĭ��ֵ(TUG) */

/* port connection mode*/
#define IF_CONNECTION_NORMAL 1
#define IF_CONNECTION_EXTEND 2

ISSU typedef struct tagIfCPOSFlagCmd
{
    UINT uiFlag;                                        /* Flag���� */
    UINT uiLength;                                      /* Flag���� */
    UINT uiPathNo;                                      /* ͨ���� */
    UCHAR aucVal[IF_CPOS_FLAG_LEN_MAX];                 /* Flagֵ */
}IF_CPOS_FLAG_CMD_S, *PIF_CPOS_FLAG_CMD_S;

ISSU typedef struct tagIfCPOSPortCmd
{
    UINT uiChanFlag;                                    /* ͨ����־ */
    UINT uiModeFlag;                                    /* ����ģʽ */
}IF_CPOS_PORT_CMD_S, *PIF_CPOS_PORT_CMD_S;

#define IF_CPOS_MULTICHANNEL_H3CHP  0
#define IF_CPOS_MULTICHANNEL_ALCATE 1
#define IF_CPOS_MULTICHANNEL_LUCENT 2

/* IOCTL_CPOS_SET_channel-align-mode */
#define IF_CPOS_MULTI_CHANNEL_DEFAULT           IF_CPOS_MULTICHANNEL_H3CHP


/* ===END: CPOS�������������֡��ꡢ�ṹ=== */

/*********************END: WAN�ӿ����********************************/

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

/* �ӿ��������仯 */
typedef enum tagIFCONNECTORCHG
{
    IF_CONNECTOR_CHG = 1,           /* �������仯 */
    IF_CONNECTOR_FAULT,             /* ���������� */
    IF_CONNECTOR_NORMAL,            /* �������ָ����� */
    IF_CONNECTOR_MAX
}IFCONNECTORCHG_E;

/* ֪ͨ�������仯 */
VOID IF_NotifyConnectorChange(IN IF_INDEX ifIndex, IN IFCONNECTORCHG_E enChange, IN const CHAR *pcConnectorName);

/* ֪ͨ�ӿڲ���/�γ� */
ULONG IF_NotifyIntfState (IN IF_HOT_STATE_E enIfHotState, IF_PORT_INFO_S *pstInfPortInfo, IN ULONG ulNum);

/* �ӿ����Ͷ��ƽӿ� */

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
ULONG IF_RegNeedMannulNoLoop(VOID); /* ��Ʒע��˿ڻػ����״̬����Ҫ�ֶ�ȡ������ */
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
    ULONG (*pfDRV_INTF_CreateComplete)(IN PIF_PORT_INFO_EXT_S pstPortInfo);  /* oam ʹ��*/
    ULONG (*pfDRV_INTF_DeleteComplete)(IN PIF_PORT_INFO_EXT_S pstPortInfo);  /* oam ʹ��*/
    ULONG (*pfDRV_INTF_SetProvisionMode)(IN const CHAR * pcMode, 
                                         IN USHORT usGSlot,
                                         IN USHORT usSubSlot,
                                         OUT IF_PORT_INFO_S ** ppstPortList,
                                         OUT ULONG * pulPortNum);
    VOID (*DRV_INTF_ClearProvisionMode)(IN USHORT usGSlot, IN USHORT usSubSlot);
} IF_DRV_INTF_S, *PIF_DRV_INTF_S;

typedef struct tagREMOTEIF_DRV_INTF {
    ULONG (*pfDRV_INTF_PhyIOCtl)(IN IF_INDEX ifIndex, IN ULONG ulCmd,VOID *pData);//����Ǳ�����������ã�
    ULONG (*pfDRV_INTF_HandleEvent)(IN IF_INDEX ifIndex, IN UINT uiEvent ,VOID *pData);//֪ͨ���������¼���
} REMOTEIF_DRV_INTF_S, *PREMOTEIF_DRV_INTF_S;

ULONG IF_Register_Driver(IN ULONG ulVersion, IN const IF_DRV_INTF_S *pstIfDrvIntf);
ULONG IF_Register_RemoteIf_Driver(IN ULONG ulVersion, IN const REMOTEIF_DRV_INTF_S *pstRemoteDrvIntf);
extern VOID E1T1VI_SendE1T1VITrapTimeSlot(IN INT iIfEntry_ifIndex);

extern VOID IF_SONET_SendTrap(IN IF_INDEX ifIndex, IN ULONG ulType);

/* Ԥ��Bri��isdn leased-line���õĽӿں��� */
IF_BRI_LINE_INFO_S *IF_GetBriLineInfoFromCfg(OUT ULONG *pulNum);

#endif

#ifdef  __cplusplus
}
#endif

#endif /* _DRV_IF_H_ */


