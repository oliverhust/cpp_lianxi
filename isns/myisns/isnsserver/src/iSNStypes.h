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
 * This file contains definitions of data structures
 * and macros used in storing attributes in the SoIP
 * service database.
 *
 */

#ifndef _SNStypes_h_
#define _SNStypes_h_

#include "../../include/iscsi_com.h"
#include "iSNSLinux.h"


#include "iSNSdefs.h"
#include "iSNSList.h"

#define SNS_IF_NAME           "eth0"
#define SNS_IFMGMT_NAME       "eth1"

#ifndef INET_ADDR_LEN
#define INET_ADDR_LEN   18
#endif

#include "iSNSfctypes.h"

#define MAX_PORTS_PER_NODE      32
#define SNS_PORTS_PER_LIST     128

/*
 * Maximum number of attributes that can be sent in response
 * to an iSNS query. The maximum payload has small enough to
 * fit in an ethernet frame.
 */
#define SNS_MAX_ATTRS          64

/*
 * Definition of attribute sizes in bytes
 */
#define MAX_DD_PER_LIST       (1024)
#define MAX_MEMBER_PER_DD     (1024)
#define NODE_SYM_NAME_SIZE    256
#define PORT_NAME_SIZE        8
#define PORT_SYM_NAME_SIZE    256
#define PORT_ID_SIZE          3
#define PORT_ID_PAD           1
#define ZONE_SYM_NAME_SIZE    64
#define NX_PORT_TYPE_SIZE     10
#define NODE_IPA_SIZE         8
#define HARD_ADDR_SIZE        3
#define HARD_ADDR_PAD         1
#define DB_NODE_NAME_SIZE     9
#define SNS_COS_SIZE          4
#define PORT_TYPE_SIZE        1
#define PORT_TYPE_PAD         3
#define FC4_BITMAP_SIZE       32 
#define FC4_TYPE_SIZE         1
#define FC4_TYPE_PAD          3
#define ZONE_BITMAP_SIZE      64
#define SAN_SYM_NAME_SIZE     64

#define ENTITY_ID_SIZE        128
#define ESI_SIZE              4
#define ESI_PORT_SIZE         4
#define ENTITY_TYPE_SIZE      4
#define IP_PORT_SIZE          4
#define SPACE_ID_SIZE         16

#define IP_ADDR_SIZE_V4       4
#define IP_ADDR_SIZE_V6       16

#define IP_ADDR_SIZE          IP_ADDR_SIZE_V6

#define  XID_BEGIN            0
#define  XID_IN_PROGRESS      1
#define  XID_END              2
#define  SOIP_DB_Node_Id      0xFE 

#define ISNS_DD_MEMBER_ENABLE       (1)
#define ISNS_DD_MEMBER_DISABLE      (0)


#define ISNS_SOCKET_TYPE_UDP     (0x12)
#define ISNS_SOCKET_TYPE_TCP     (0x34)

#define ISNS_ISCSI_TYPE_SIZE     (4)

#define ESI_SIZE              4
#define ESI_PORT_SIZE         4
#define ENTITY_TYPE_SIZE      4
#define PORTAL_ID_SIZE        4

/* 四个字节的数字转为字符串的最大长度 */
#define ISNS_NUM_STR_MAX_SIZE        16

/*
 * Declaration of the attribute value structure.
 */
typedef struct {
    uint32_t   attr_id;
    uint32_t   attr_len;
    void       *attr_val;
} ISNS_ATTR_VALS;

/*
 * Declaration of the attribute value control block.
 */
typedef struct {
    uint32_t        flags;
    uint32_t        num_entries;
    uint32_t        total_size;
    ISNS_ATTR_VALS  *attr_val_sptr;
} ISNS_ATTR_VALS_CB;

/*
 * External Node Name key:
 */
typedef struct soip_node_name {

    char     v[NODE_SYM_NAME_SIZE];

} SOIP_Node_Name, SOIP_Node_Key;


/*
 * Internal Node Name key:
 */
typedef struct soip_db_node_name {

    char     v[NODE_SYM_NAME_SIZE];
    char     pad[3];

} SOIP_DB_Node_Name;

/*
 * Port Name key:
 */
typedef struct soip_port_name {

    char      v[PORT_NAME_SIZE];

} SOIP_Port_Name, SOIP_Port_Key;


/*
 * IP Address Key:
 */
typedef struct IP_address {

   char      v[IP_ADDR_SIZE];

} IP_Address;


/*
 * Port ID Key:
 */
typedef struct SOIP_port_id {
    char     v[PORT_ID_SIZE];
    char     bachelor_pad;
} SOIP_Port_Id;


/*
 * Port Type:
 */
typedef uint8_t   SOIP_Port_Type;


/*
 * Class of Service:
 */
typedef uint32_t  SOIP_COS;



typedef struct soip_node_ipa {

    char   v[NODE_IPA_SIZE];

} SOIP_Node_IPA;


typedef struct soip_hard_addr {

    char     v[HARD_ADDR_SIZE];
    char     paddington_bear;

} SOIP_Hard_Addr;


typedef struct soip_device_type {

    uint16_t lun;
    uint8_t  type;
    uint8_t  launch_pad;

} SOIP_Device_Type;

/*
 * Socket based IPC endpoint used for communicating with a remote
 * SOIP service entity
 */
typedef struct socket_ep {

    struct sockaddr_in  addr;
    uint32_t            len;
    uint32_t            socketType;
    int                 sockfd;

} ISNS_Socket_Ep;


/* 
 * Definition of registration operational types.
 */
typedef int OP_TYPE;

/*
 * Definition of Database Key. 
 * 
 */
typedef enum {

      NODE_NAME_KEY = 1,
      PORT_NAME_KEY,
      ENTITY_ID_KEY,
      PORTAL_ID_KEY,    
      DDS_ID_KEY,
      DD_ID_KEY,
      ISCSI_ID_KEY,
      PORTAL_GROUP_ID_KEY,
      ENTITY_IDX_KEY,
      ISCSI_IDX_KEY,
      PORTAL_IDX_KEY,
      LIST_KEY,
      ISNS_DATABASE_MAX,

} ISNS_Database_Key;


/*
 * Definition of Device Attribute Tags. SoIP device attributes 
 * are sent to the server in a UDP message and are encoded 
 * in TLV format.
 * 
 */

typedef enum {

      START_SNS_DEVI_ATTR = 20,/* 20 */
      SYM_NODE_NAME = START_SNS_DEVI_ATTR,
      NODE_NAME,
      PORT_NAME,
      FABRIC_PORT_NAME,
      PORT_ID,
      SYM_PORT_NAME,           /* 25 */
      IP_ADDR,
      PORT_TYPE,
      FC4_TYPES,
      COS,
      PORT_PRIORITY,           /* 30 */
      ZONE_TAG,
      ZONE_SYM_NAME,
      ZONE_BITMAP,
      ZONE_DEST_PORT_NAME,
      ZONE_ORIG_PORT_NAME,     /* 35 */
      ZONE_PREV_BITMAP,
      NODE_IPA,
      HARD_ADDR,
      RSCN_TYPE,
      RSCN_CALLBACK,           /* 40 */
      RSCN_EVENT,
      REMOTE_FLAG,
      DEVICE_TYPE,
      ENTITY_ID,
      ESI,                     /* 45 */
      ESI_PORT,
      ENTITY_TYPE,
      IP_PORT,
      PORTAL_ID,
      SPACE_ID,                /* 50 */
      AREA_ID,

      END_SNS_DEVI_ATTR

} SNS_Devi_Attr;


/* RSCN_Types (defined to fit in one byte) */

typedef struct soip_fc_feature
{
   char v[FC_FEATURE_SIZE];

} SOIP_FC_Features;

typedef struct soip_fc_descr
{
   char v[FC_DESCRIPTOR_SIZE];

} SOIP_FC_Descr;

typedef struct soip_dds {

    uint32_t              id;
    uint32_t              status;
    char                  sym_name[DDS_SYM_NAME_SIZE];

    ISNS_LIST             dd_list;

} SOIP_Dds;

typedef struct soip_dd_member {

   char                   node_id[DD_MAX_DD_MEMBER_SIZE];
   uint32_t               type;
   uint32_t               status;
   uint32_t               node_idx;

} SOIP_Dd_Member, SOIP_DD_Member_Key;

typedef uint32_t* ISNS_DD_LIST;

typedef struct soip_dd {
   uint32_t               id;
   char                   sym_name[DD_SYM_NAME_SIZE];
   ISNS_LIST              member_list;
   ISNS_LIST              dds_list;
   uint32_t               activeFlag;
   uint32_t               feature;
} SOIP_Dd;

typedef struct _soip_iscsi_node_name {
    char   v[MAX_ISCSI_NODE_ID_SIZE];
} SOIP_ISCSI_Node_Id;

typedef struct _soip_time {
   uint32_t t_pad;
   time_t t_time;
} SOIP_Time;

typedef struct soip_entity_id {
   char id[ ENTITY_ID_SIZE ];
} SOIP_Entity_Id;

/*
 * Definition of Node Name record stored in the SoIP 
 * service database
 */
typedef struct soip_node {

    char              sym_name[ NODE_SYM_NAME_SIZE ];
    SOIP_Node_Name    node_name;
    SOIP_Node_IPA     node_ipa;
    IP_Address        ip_addr;
    uint32_t          port_map;

   /*
    * Store Port names their ip addresses 
    */
    uint32_t          entity_index;
    SOIP_Port_Name    port_name[ MAX_PORTS_PER_NODE ];
    int               sym_name_len;

    /* iSNS Stuff */
    void              *ptr_cert;
    uint32_t          cert_size;
    ISNS_LIST         port_list;

} SOIP_Node, SOIP_Fc_Node;


/*
 * Definition of Port Name record stored in the SoIP 
 * service database. It contains all attributes associated
 * with a port/device.
 */
typedef struct SOIP_Ifcp {

    SOIP_Port_Name    port_name;
    SOIP_Port_Name    fabric_port_name;
    SOIP_Node_Name    node_name;
    IP_Address        ip_addr;
    SOIP_Hard_Addr    hard_addr;
    SOIP_Port_Id      id;
    SOIP_Port_Type    type;
    char              pad_thai[ PORT_TYPE_PAD ];
    FC4_Types         fc4_types;
    SOIP_COS          cos;
    uint32_t          priority;
    uint32_t          ifcp_index;
    char              zone_bitmap[ ZONE_BITMAP_SIZE ];
    uint32_t          san_id;
    char              sym_name[ PORT_SYM_NAME_SIZE ];
    uint32_t          remote;
    SOIP_Device_Type  dev_type;
    int               sym_name_len;
    SOIP_Entity_Id    entity_id;
    char              space_id[ SPACE_ID_SIZE ];

    /* iSNS Stuff */
    ISNS_LIST         dd_id_list;
    uint32_t          scn_bitmap;
    int               scn_sockfd;
    void              *ptr_cert;
    uint32_t          cert_size;
    SOIP_FC_Features  fc_feature;
    SOIP_FC_Descr     fc_descr;

    /* Internal */
    uint32_t          activeFlag;
    void              (*scn_callback)(ISNS_ATTR_VALS_CB *attr_vals_sptr);

} SOIP_Ifcp;


/*
 * Definition of a port list which is used for maintaining
 * port names associated with a service domain, FC4 type
 * and port type.
 */
typedef struct soip_port_list {

#define SNS_MAP_SIZE      (SNS_PORTS_PER_LIST/32)

    uint32_t              num_entries;
    uint32_t              bitmask[ SNS_MAP_SIZE ]; 
    SOIP_Port_Name        port_name[ SNS_PORTS_PER_LIST ];

} SOIP_Port_List;

typedef struct soip_scn_entry {
   uint32_t type;
   uint32_t scn_bitmap;
   int      scn_sockfd;
   SOIP_ISCSI_Node_Id name;
} SOIP_Scn_Entry;

typedef struct soip_zone {

    uint32_t              zone_tag;
    char                  sym_name[ ZONE_SYM_NAME_SIZE ];
    SOIP_Port_List        plist;
    int                   sym_name_len;

} SOIP_Zone;

typedef enum {

  ETYPE_ISCSI = 1,
  ETYPE_IFCP
} SOIP_Entity_Type;

typedef struct soip_prot_ver {
   uint16_t             ver_max;
   uint16_t             ver_min;
} SOIP_Prot_Ver;

typedef struct soip_entity {
    SOIP_Entity_Id       eid;
    uint32_t             eid_len;
    uint32_t             esi;
    SOIP_Entity_Type     eid_type;         //1=iscsi,  2=ifcp
    uint32_t             node_type;        //1=target, 2=initiator, 3=both
    uint32_t             entity_index;
    SOIP_Time            timestamp;
    IP_Address           mgmt_ip_addr;
    ISNS_LIST            iportal_list;
    ISNS_LIST            ifcp_node_list;
    ISNS_LIST            iscsi_node_list;
    uint32_t             scn_bitmap;
    int                  scn_sockfd;
    SOIP_Prot_Ver        prot_ver;
    void                 *ptr_cert;
    uint32_t             cert_size;
    uint32_t             period;
} SOIP_Entity;

typedef struct soip_entity_key {
   char id[ ENTITY_ID_SIZE ];
} SOIP_Entity_Key;

typedef struct entity_list_entry {
    SOIP_Entity_Id   eid;
    uint32_t         entity_index;
} ENTITY_LIST_ENTRY;

typedef struct index_entry {
    uint32_t         index;
    uint32_t         index_type;
} SOIP_IDX_Key;

typedef struct soip_portal {
    IP_Address        ip_addr;
    uint32_t          ip_port;
    char              sym_name[ PORTAL_SYM_NAME_SIZE ];
    uint32_t          node_type;        //1=target, 2=initiator
    SOIP_Entity_Id    entity_id;
    uint32_t          entity_index;
    uint32_t          portal_index;
    uint32_t          esi_sockfd;
    uint32_t          esi_port;
    uint32_t          esi_interval;
    uint32_t          esiSent;
    time_t            esi_timer;
    void              *ptr_cert;
    uint32_t          cert_size;
    uint32_t          default_portal_tag;
} SOIP_Portal;

typedef struct soip_portal_key {
    IP_Address       ip_addr;
    uint32_t         ip_port;
} SOIP_Portal_Key;

typedef struct soip_db_portal {
    IP_Address ip_addr;
    uint32_t   ip_port;
} SOIP_DB_Portal;

typedef struct portal_list_entry {
    SOIP_DB_Portal   portal_ip_port;
    uint32_t         portal_idx;
} PORTAL_LIST_ENTRY, SOIP_Portal_List_Key;

typedef struct soip_portal_group {
    SOIP_ISCSI_Node_Id      id;
    IP_Address       ip_addr;
    uint32_t         ip_port;
    uint32_t         portal_tag;
    uint32_t          portal_group_index;

} SOIP_Portal_Group;

typedef struct soip_portal_group_key {
    SOIP_ISCSI_Node_Id  id;
    IP_Address       ip_addr;
    uint32_t         ip_port;
} SOIP_Portal_Group_Key;

/*
 * Entities that communicate with the SOIP service
 * agent
 */

#define HASH_MAX_BUFFER 256
typedef struct idx_hash_entry {
   uint32_t index;
   void *next;
   char buffer[HASH_MAX_BUFFER];
} IDX_HASH_ENTRY;

#define DD_KEY_SIZE           (4)
#define DDS_KEY_SIZE          (4)

typedef struct soip_dd_key {
   uint32_t    id;
} SOIP_DD_Key;

typedef struct soip_dds_key {
   uint32_t    id;
} SOIP_DDS_Key;

typedef struct soip_iscsi {
   SOIP_ISCSI_Node_Id      id;
   uint32_t                iscsi_id_len;
   SOIP_Entity_Id          entity_id;
   uint32_t                entity_id_len;
   uint32_t                type;
   char                    alias[MAX_ISCSI_ALIAS_SIZE];
   uint32_t                alias_len;
   ISNS_LIST               dd_id_list;

   /* SCN */
   uint32_t                entity_index;
   uint32_t                iscsi_index;
   uint32_t                scn_bitmap;
   int                     scn_sockfd;

   /* Cert */
   void                    *ptr_cert;
   uint32_t                cert_size;

   uint32_t                activeFlag;
   void                    (*scn_callback)(ISNS_ATTR_VALS_CB *attr_vals_sptr);

} SOIP_Iscsi;

typedef struct _soip_iscsi_key {
    char   v[MAX_ISCSI_NODE_ID_SIZE];
} SOIP_ISCSI_Key;

/* 表示ISCSI INDEX和ISCSI NAME的对应关系，仅当该ISCSI NAME被DD引用或存在该ISCSI NODE时能查询到该条目:
  当ADD ISCSI NODE时如果查询不到该条目则可以新建该条目，同时分配新INDEX，此时iDdRefCount初始化为0
                    如果查询到了则使用该条目指定的INDEX;
  当AddDDMember时(注意是新的member)如果查询不到该条目则可以新建该条目，同时分配新INDEX，此时iDdRefCount初始化为1,
                 如果查询到了该条目，则使用该条目指定的INDEX，同时使iDdRefCount++;
  当删除ISCSI NODE时如果满足iDdRefCount==0则删除该条目;
  当DelDDMem/DelDD时如果满足(iDdRefCount==1(最后一个引用者)且ISCSI NODE不存在)则删除该条目,
                    否则使iDdRefCount--;
  如上述工作正常，不应有iDdRefCount变成负数的情况
  由于引用计数容易考虑疏漏，类似问题最好分析好各种情况  */
typedef struct _soip_iscsi_inedx {
    char   v[MAX_ISCSI_NODE_ID_SIZE];
    uint32_t uiIndex;
    int iDdRefCount;    /* 被多少DD引用，ADD DD MEM时++，DelDDMem/DelDD时-- */
} SOIP_ISCSI_Index;

/* ids for identifying which list */
typedef enum {
  DATA_LIST_INVALID,
  ISCSI_DD_LIST = 1,
  ENTITY_PORTAL_LIST,
  ENTITY_ISCSI_LIST,
  DD_MEMBER_LIST,
  DD_DDS_LIST,
  DDS_DD_LIST,

  SCN_CALLBACK_LIST,
  SCN_LIST,
  DATA_LIST_MAX,

  /* 应舍弃的 */
  FCP_DD_LIST,
  FCP_PORTAL_LIST,
  ENTITY_FCP_LIST,
  PORTAL_ENTITY_LIST,
  DATA_LIST_OLD_MAX
} DATA_LIST_ID;

#define LIST_KEY_SIZE (8)

typedef struct soip_list_key {
  int list_id;
  int list_index;
  union {  /* The key to which the list belongs */
        SOIP_Entity_Key      entity_id;
        SOIP_DDS_Key         dds;
        SOIP_DD_Key          dd;
        SOIP_Node_Key        node_name;
        SOIP_Portal_Key      portal;
  } key;
} SOIP_LIST_Key;

typedef struct soip_list {
  int index;
  int next_index;
  int prev_index;
  union {
        SOIP_Entity_Key    entity_id;
        SOIP_DDS_Key       dds;
        SOIP_DD_Key        dd;
        SOIP_Node_Key      node_name;
        SOIP_Portal_Key    portal;
        SOIP_DD_Member_Key ddmember;
        SOIP_Portal_List_Key portalmember;
  } key;
} SOIP_DB_List;

typedef struct soip_db_entry {

    /*
     * The "data_type" field identifies the type of data
     * stored in the iSNS database. This field is used 
     * during database lookups to identify the type of
     * data associated with a database key.
     */
    ISNS_Database_Key     data_type;

    union {
        SOIP_Entity       entity;
        SOIP_Portal       portal;
        SOIP_Portal_Group portal_group; 
        SOIP_Fc_Node      ifcp_node;
        SOIP_Iscsi        scsi_node;
        SOIP_Ifcp         port;
        SOIP_Dds          dds;
        SOIP_Dd           dd;
        SOIP_Prot_Ver     prot_ver;
        SOIP_Node         node;
        SOIP_Entity_Id    entity_idx;
        SOIP_ISCSI_Index  iscsi_idx;
        SOIP_DB_Portal    portal_idx;
        SOIP_DB_List      list;
    } data;

} SOIP_DB_Entry;

typedef struct ISNS_dbkey {

    int              tag;
    uint32_t         len;

    union {
        SOIP_ISCSI_Key           iscsi_key;
        SOIP_Entity_Key          entity_key;
        SOIP_Node_Key            node_key;
        SOIP_Port_Key            port_key;
        SOIP_DD_Key              dd_key;
        SOIP_DDS_Key             dds_key;
        SOIP_Portal_Key          portal_key;
        SOIP_Portal_Group_Key    portal_group_key;
        SOIP_IDX_Key             idx;
        SOIP_LIST_Key            list;
    } val;

} ISNS_DBKey;

typedef struct ISNS_key {

    uint32_t         tag;
    uint32_t         len;

    union {
        uint32_t           index;
        uint32_t           node_type;
        uint32_t           ip_port;
        uint32_t           dds_id;
        uint32_t           dd_id;
        SOIP_Dds           dds;
        SOIP_Dd            dd;
        SOIP_Entity_Id     entity_id;
        SOIP_Node_Name     node_name;
        SOIP_Port_Name     port_name;
        IP_Address         ip_addr;
    } val;

} ISNS_Key;

/*
 * Generic data structure for attribute
 */
typedef struct ISNS_attr {

    uint32_t         tag;
    uint32_t         len;

    union {
        uint32_t           index;
        uint32_t           period;
        uint32_t           ip_port;
        uint32_t           dds_id;
        uint32_t           dd_id;
        uint32_t           dds_status;
        SOIP_Prot_Ver      prot_ver;
        uint32_t           scn_bitmap;
        SOIP_Node_Name     node_name;
        char               sym_node_name[NODE_SYM_NAME_SIZE];
        SOIP_Port_Name     port_name;
        SOIP_Port_Name     fabric_port_name;
        char               sym_port_name[PORT_SYM_NAME_SIZE];
        SOIP_Port_Id       port_id;
        IP_Address         ip_addr;
        SOIP_Port_Type     port_type;
        FC4_Types          fc4_types;
        SOIP_COS           cos;
        uint32_t           portal_group;
        uint32_t           priority;
        SOIP_Node_IPA      ipa;
        SOIP_Hard_Addr     hard_addr;
        char               sym_san_name[SAN_SYM_NAME_SIZE];
        uint32_t           remote;
        SOIP_Device_Type   dev_type;
        SOIP_Time          timestamp;
        void               (*scn_callback)(ISNS_ATTR_VALS_CB *attr_vals_sptr);
    SOIP_Entity_Id     entity_id;

    } val;

} ISNS_Attr;

/*
 * Configured role of the SOIP service software. SOIP service software
 * can be configured to run as a ISNS_CLIENT, ISNS_PRIMARY_SERVER or
 * ISNS_SECONDARY_SERVER.
 *
 */

typedef enum {

    ISNS_MGMT_AGENT = 1,
    ISNS_CLIENT,
    ISNS_PRIMARY_SERVER,
    ISNS_SECONDARY_SERVER,
    ISNS_REMOTE,

    ISNS_NUM_ENTITIES

} ISNS_Entity;

typedef struct soip_scn_callback_entry {
   uint32_t bitmap;
   uint32_t type;
   void (*scn_callback_func)(ISNS_ATTR_VALS_CB *avcb_ptr);
   SOIP_ISCSI_Node_Id name;
} SOIP_Scn_Callback_Entry;

extern ISNS_Entity isns_role;

#endif
