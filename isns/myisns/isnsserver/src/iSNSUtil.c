/**********************************************************************
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


#include "iSNS.h"
#include "iSNScomm.h"
#include "iSNSdb.h"
#include "iSNSmsg.h"
#include "iSNSqueue.h"
#include "iSNSfsm.h"
#include "iSNScomm.h"
#include "iSNSbuffer.h"
#include "iSNSipc.h"
#include "iSNSList.h"
#include "iSNSdebug.h"

static char *p_name = "Port Name";

/********************************************************************
********************************************************************/
void 
ISNSDisplayMsg (ISNS_Msg *msg)
{
#if 0
    SNS_Msg_Type    msg_type;
    SOIP_Port_Name  *p_port;


    /*
     * Validate the received message
     */
    msg_type         = msg->hdr.type;

    switch (msg_type) 
    {
        case DEREG_DEVI_REQ: 
        {     
            __LOG_INFO ("\n ********** DEREGISTRATION MESSAGE *************** \n");
            ISNSDisplay_Hdr_Info(msg);

            __LOG_INFO ("PAYLOAD \n");
            p_port = &msg->payload.sns_req.key.val.port_name;
            ISNSDisplay_Port_Name(p_port, p_name);
            __LOG_INFO ("\n ********************** EOM ********************** \n");
        }
        break;

        default: 
            __LOG_INFO ("\nReceived an unknown message.\n");
        break;
    }
#endif
}

/********************************************************************
********************************************************************/
void 
ISNSDisplay_Hdr_Info(ISNS_Msg *msg)
{
    __LOG_INFO ("HEADER \n");
    __LOG_INFO ("Message Type    : %d \n", msg->hdr.type);
    __LOG_INFO ("Message Xid     : %d \n", msg->hdr.xid);
    __LOG_INFO ("Message Length  : %d \n", msg->hdr.msg_len);
    __LOG_INFO ("Message Flags   : %d \n", msg->hdr.flags);
}

/********************************************************************
********************************************************************/
void
ISNSDisplay_DbData(ISNS_Key *p_key, SOIP_DB_Entry *p_entry, int detail)
{

#if 0
    if (detail == HI_DETAIL || ((p_key->tag != PORT_NAME_KEY) &&
                                (p_key->tag != NODE_NAME_KEY)))
    {
       SNSDisplay_Attr((SNS_Attr *)p_key, detail);
       __LOG_INFO("\n");
    }

    if (p_key->next)
    {
       __LOG_INFO ("*************** PORT LIST RECORD **************\n"); 
       SNSDisplay_PortList (&p_entry->data.port_list);
       __LOG_INFO ("***********************************************\n");
       return;
    }
#endif

    switch ( p_entry->data_type )
    {
        case ISCSI_ID_KEY:
             ISNSDisplay_ISCSINode ( &p_entry->data.scsi_node, detail );
             break;
        case ENTITY_ID_KEY:
             ISNSDisplay_Entity ( &p_entry->data.entity, detail );
             break;
        case PORTAL_ID_KEY:
             ISNSDisplay_Portal ( &p_entry->data.portal, detail );
             break;
        case DDS_ID_KEY:
             ISNSDisplay_DDS (&p_entry->data.dds, detail);
             break;
        case DD_ID_KEY:
             ISNSDisplay_DD (&p_entry->data.dd, detail);
             break;
#if 0
        case NODE_NAME_KEY:
             SNSDisplay_DbNode (&p_entry->data.node, detail);
             break;
        case PORT_NAME_KEY:
             SNSDisplay_DbPort (&p_entry->data.port, detail);
             break;
        case PORT_TYPE_KEY:
        case FC4_TYPE_KEY:
        case AREA_ID_KEY:
             __LOG_INFO ("*************** PORT LIST RECORD **************\n"); 
             SNSDisplay_PortList (&p_entry->data.port_list);
             __LOG_INFO ("***********************************************\n");
             break;
        case ZONE_TAG_KEY:
             SNSDisplay_DbZone (&p_entry->data.zone);
             break;
        case RSCN_TYPE_KEY:
             SNSDisplay_RscnList (&p_entry->data.rscn);
             break;
        case SPACE_ID_KEY:
             SNSDisplay_DbSpace (&p_entry->data.area);
             break;

        case IP_ADDR_KEY:
        case NX_PORT_TYPE_KEY:
        case ENTITY_ID_KEY:
        case PORTAL_ID_KEY:
        case END_SNS_DEVI_KEY:
             break;
#endif
        default:
            __LOG_INFO("\nUnknown table entry (data_type %d)\n", 
                   p_entry->data_type);
            break;
    }
}

/********************************************************************
********************************************************************/
void 
ISNSDisplay_Port_Name ( SOIP_Port_Name *p_port, char *p_name )
{
    int indx;

    if (p_port != NULL)
    {
        __LOG_INFO ("%-19s : ", p_name);
        for (indx = 0; indx <  PORT_NAME_SIZE; indx++) 
            __LOG_INFO ("%02x", p_port->v[indx]);
        __LOG_INFO ("\n"); 
    }
}

/********************************************************************
********************************************************************/
void  
ISNSDisplay_Node_Name ( SOIP_Node_Name *p_node )
{
    int indx;

    __LOG_INFO ("Node Name           : ");
    for (indx = 0; indx < ISNS_NODE_NAME_SIZE; indx++) 
        __LOG_INFO ("%02x", p_node->v[indx]);
    __LOG_INFO ("\n"); 
}

/********************************************************************
********************************************************************/
void
ISNSDisplay_Port_Id ( SOIP_Port_Id *p_id )
{
    int indx;
    SOIP_Port_Id null_id = {{0, 0, 0}};

    if ((p_id == NULL) || memcmp(p_id, &null_id, PORT_ID_SIZE) == 0)
       return;

    __LOG_INFO ("Port Id             : ");
    for (indx = 0; indx <  PORT_ID_SIZE; indx++) 
        __LOG_INFO ("%02X", p_id->v[indx]);
    __LOG_INFO ("\n"); 
}

/********************************************************************
********************************************************************/
void 
ISNSDisplay_IPAddress ( IP_Address *ip_addr )
{
    char              ip_saddr[INET_ADDR_LEN+1];
    struct in_addr    i_addr;

    /* Assumes a IPv4 Address */
    __ISNS_COPY (&i_addr.s_addr, sizeof(i_addr.s_addr), ip_addr->v+12, IP_ADDR_SIZE_V4);

    inet_ntoa_b (i_addr, ip_saddr);
    ip_saddr[INET_ADDR_LEN] = '\0';
    __LOG_INFO ("IP Address          : %s     \n", ip_saddr); 
}


/********************************************************************
********************************************************************/
void 
ISNSDisplay_Port ( SOIP_Ifcp *p_port )
{
    int      ii;
    SOIP_COS unreg_cos;

    __LOG_INFO ("******************** PORT RECORD **************\n"); 
    ISNSDisplay_Port_Name (&p_port->port_name, p_name);
    ISNSDisplay_Port_Name (&p_port->fabric_port_name, "Fabric Port");

    __LOG_INFO ("Symbolic Port Name  : %s      \n",  p_port->sym_name); 

    ISNSDisplay_Node_Name (&p_port->node_name);

    ISNSDisplay_IPAddress (&p_port->ip_addr);

    ISNSDisplay_HardAddr (&p_port->hard_addr);

    ISNSDisplay_Port_Id (&p_port->id);

    if (p_port->type != SNS_UNREGISTERED)
        __LOG_INFO ("Port Type           : %d     \n", p_port->type); 

    for (ii = 0; ii < 32; ii++)
        if (p_port->fc4_types.bitmap[ii])
           __LOG_INFO ("FC-4 Types          : 0x%2x [%d] \n", 
                   p_port->fc4_types.bitmap[ii], ii);

    memset(&unreg_cos, SNS_UNREGISTERED, sizeof(SOIP_COS));
    if (memcmp(&p_port->cos, &unreg_cos, sizeof(SOIP_COS)) != 0)
        __LOG_INFO ("Port COS            : %08X \n", (unsigned int)p_port->cos);

    __LOG_INFO ("Port priority       : %d \n", p_port->priority); 

    for (ii = 0; ii < ZONE_BITMAP_SIZE; ii++)
        if (p_port->zone_bitmap[ii])
           __LOG_INFO ("Zone bitmap         : 0x%2x [%d] \n", 
                   p_port->zone_bitmap[ii], ii);

    __LOG_INFO ("Remote flag         : %d \n", p_port->remote); 
    __LOG_INFO ("Device type         : %0lx \n", *(long *)&p_port->dev_type); 

    __LOG_INFO ("\n***********************************************\n"); 
}

/********************************************************************
********************************************************************/
void 
ISNSDisplay_Node ( SOIP_Node *p_node )
{
    int ii;

    __LOG_INFO ("******************** NODE RECORD **************\n"); 
    ISNSDisplay_Node_Name (&p_node->node_name);

    for (ii = 0; ii < NODE_SYM_NAME_SIZE; ii++)
       if (p_node->sym_name[ii] == (char)SNS_UNREGISTERED)
          p_node->sym_name[ii] = '\0';
    __LOG_INFO ("Symbolic Node Name  : %s      \n",  p_node->sym_name); 

    ISNSDisplay_NodeIPA (&p_node->node_ipa);

    ISNSDisplay_IPAddress (&p_node->ip_addr);

    __LOG_INFO ("Port map            : %08X      \n",  p_node->port_map);
    for (ii = 0; ii < MAX_PORTS_PER_NODE; ii++)
       if (p_node->port_map & (1 << ii))
          ISNSDisplay_Port_Name (&p_node->port_name[ii], p_name);

    __LOG_INFO ("***********************************************\n"); 
}

/********************************************************************
********************************************************************/
void 
ISNSDisplay_IPAddr (IP_Address *ip_addr)
{
    __LOG_INFO ("*************** IP ADDRESS RECORD *************\n"); 
    ISNSDisplay_IPAddress (ip_addr);
    __LOG_INFO ("***********************************************\n"); 
}

/********************************************************************
********************************************************************/
void 
ISNSDisplay_Port_List(SOIP_Port_List *p_list)
{
    int ii;

    __LOG_INFO ("Port List Entries   : %d      \n", (int)p_list->num_entries);
    __LOG_INFO ("Port List Bitmask   : ");
    for (ii = 0; ii < SNS_MAP_SIZE; ii++)
      __LOG_INFO("%08lX", (long)p_list->bitmask[ii]);
    __LOG_INFO("\n");
    for (ii = 0; ii < SNS_PORTS_PER_LIST; ii++)
        if ( p_list->bitmask[ii/32] & (1 << (ii % 32)) )
            ISNSDisplay_Port_Name (&p_list->port_name[ii], p_name);
}

/********************************************************************
********************************************************************/
void 
ISNSDisplay_Port_TypeList (SOIP_Port_List *p_list)
{
    __LOG_INFO ("******************* PORT TYPE RECORD **********\n");
    ISNSDisplay_Port_List(p_list);
    __LOG_INFO ("***********************************************\n"); 
}

/********************************************************************
********************************************************************/
void 
ISNSDisplay_FC4TypeList (SOIP_Port_List *p_list)
{
    __LOG_INFO ("****************** FC4 LIST RECORD ************\n");
    ISNSDisplay_Port_List(p_list);
    __LOG_INFO ("***********************************************\n"); 
}

/********************************************************************
********************************************************************/
void 
ISNSDisplay_FC4Type (void *fc4_types, uint32_t attr_len)
{
    uint8_t         i;
    uint32_t        *nptr;

    if (attr_len == FC4_BITMAP_SIZE)
    {
        /*
         * Add the port to all FC4 type lists that
         * are enabled in the bit mask
         */
         for (i = 0, nptr = (uint32_t *)
                                    ((FC4_Types *)fc4_types)->bitmap; 
                                    i < 8; i++, nptr++)
         {
             if (nptr != NULL && *nptr != 0)
             {
                __LOG_INFO ("FC4 Type            : 0x%-4x\n", *nptr);
             }
         }
     }
     else 
        __LOG_INFO ("FC4 Type           : 0x%x\n", *((uint8_t *)fc4_types));
}

/********************************************************************
********************************************************************/
void 
ISNSDisplay_Zone_Tag (SOIP_Zone *p_zone)
{
    int ii;

    __LOG_INFO ("******************* ZONE TAG RECORD ***********\n");
    __LOG_INFO ("Zone Tag           : %08x      \n",  p_zone->zone_tag);
    for (ii = 0; ii < ZONE_SYM_NAME_SIZE; ii++)
       if (p_zone->sym_name[ii] == (char)SNS_UNREGISTERED)
          p_zone->sym_name[ii] = '\0';
    __LOG_INFO ("Zone Symbolic Name : %s      \n",  p_zone->sym_name);

    ISNSDisplay_Port_List(&p_zone->plist);

    __LOG_INFO ("***********************************************\n"); 
}

/********************************************************************
********************************************************************/
void 
ISNSDisplay_Zone_Priority (uint32_t *zone_priority)
{
    __LOG_INFO ("Zone Priority  : 0x%-8x\n", *zone_priority);
}

#if 0
/********************************************************************
********************************************************************/
void 
ISNSDisplay_RSCN_List (SOIP_RSCN_List *p_rscn)
{
    uint32_t  ii;
    SOIP_RSCN *rscn;

    __LOG_INFO ("******* STATE CHANGE NOTIFICATION RECORD ******\n");
    __LOG_INFO ("Number of Entries   : %ld      \n", p_rscn->num_entries);

    for (ii = 0; ii < p_rscn->num_entries; ii++)
    {
       rscn = &p_rscn->rscn_entry[ii];
       ISNSDisplay_Port_Name (&rscn->pn, p_name);
       __LOG_INFO ("Callback Routine    : %08lX \n", (long) rscn->cfunc);
    }
    __LOG_INFO ("***********************************************\n"); 
}
#endif

/********************************************************************
********************************************************************/
void 
ISNSDisplay_Zone (uint32_t *zone_tag)
{
    __LOG_INFO ("Zone Tag            : %d \n", *zone_tag);
}

/********************************************************************
********************************************************************/
void 
ISNSDisplay_HardAddr (SOIP_Hard_Addr *hard_addr)
{
   int ii;

   __LOG_INFO ("Hard Address        : ");
   for (ii = 0; ii < HARD_ADDR_SIZE; ii++)
       __LOG_INFO("%02x", hard_addr->v[ii]);
   __LOG_INFO("\n");
}

/********************************************************************
********************************************************************/
void 
ISNSDisplay_NodeIPA  (SOIP_Node_IPA *node_ipa)
{
   int ii;

   __LOG_INFO ("Node IPA            : ");
   for (ii = 0; ii < NODE_IPA_SIZE; ii++)
       __LOG_INFO("%02x", node_ipa->v[ii]);
   __LOG_INFO("\n");
}

/********************************************************************
********************************************************************/
void 
ISNSDisplay_Node_SymName (char *sym_name)
{
    if (sym_name && sym_name[0] != (char)SNS_UNREGISTERED)
        __LOG_INFO ("Symbolic Node Name  : %s      \n",  sym_name); 
    else
        __LOG_INFO ("Symbolic Node Name  : \n" ); 
}

/********************************************************************
********************************************************************/
void 
ISNSDisplay_Port_SymName (char *sym_name)
{
    if (sym_name && sym_name[0] != (char)SNS_UNREGISTERED)
        __LOG_INFO ("Symbolic Port Name  : %s      \n",  sym_name); 
    else
        __LOG_INFO ("Symbolic Port Name  : \n" ); 
}

/********************************************************************
********************************************************************/
void 
ISNSDisplay_Port_Type (SOIP_Port_Type *p_type)
{
    __LOG_INFO ("Port Type           : %d \n", *p_type);
}

/********************************************************************
********************************************************************/
void 
ISNSDisplay_Port_Cos (SOIP_COS *p_cos)
{
    __LOG_INFO ("PORT COS            : %08X   \n", *(unsigned int *)p_cos);
}

/********************************************************************
********************************************************************/
void 
ISNSDisplay_Portal_Name(char *p_name)
{
   char dot_no[INET_ADDR_LEN];
   PORTAL_LIST_ENTRY *p_lentry;

   p_lentry = (PORTAL_LIST_ENTRY *)p_name;

   inet_ntoa_b(*(struct in_addr *)((char *)p_name+12), dot_no);

   if (p_name !=NULL)
   {
      __LOG_INFO("Portal IP: %s\n", dot_no);

      //__LOG_INFO("Portal Port: %#x\n", p_lentry->portal_ip_port.ip_port & 0xFFFF);
      __LOG_INFO("Portal Port: %i\n", p_lentry->portal_ip_port.ip_port);
      __LOG_INFO("Portal Type: %s\n", (p_lentry->portal_ip_port.ip_port & 0x10000)?"UDP":"TCP");
      __LOG_INFO("Portal Idx : %u\n", p_lentry->portal_idx);
   }
}

/********************************************************************
********************************************************************/
void 
ISNSDisplay_DD( SOIP_Dd *p_dd, int level )
{
   ISNS_LIST_NODE *pnode;

   if ( level == LO_DETAIL )
   {
      __LOG_INFO("%u, %s\n", p_dd->id, p_dd->sym_name);
   }
   else
   {
      __LOG_INFO("*************** DD Entry ***************\n");
      __LOG_INFO("DD ID        : %u\n", p_dd->id);
      __LOG_INFO("DD SYM NAME  : %s\n", p_dd->sym_name);
      __LOG_INFO("DD Enabled  : %s      \n", p_dd->activeFlag ? "Yes" : "No ");
      __LOG_INFO("DD DDS       : ");

      pnode = NULL;
      while ( (pnode = GetNextNode(&p_dd->dds_list, pnode)) )
      {
            __LOG_INFO("%u ", *(uint32_t *)GetNodeData(pnode));
      }

      __LOG_INFO("\n");

      pnode=NULL;
      while ( (pnode = GetNextNode(&p_dd->member_list, pnode)) )
      {
         SOIP_Dd_Member *p_member;

         p_member = (SOIP_Dd_Member *)GetNodeData( pnode );
         switch ( p_member->type )
         {
            case ISNS_DD_ISCSI_MEMBER:
               __LOG_INFO( "DD ISCSI MEMBER:  %s\n", p_member->node_id );
               break;
            case ISNS_DD_IFCP_MEMBER:
               ISNSDisplay_Port_Name( (struct soip_port_name *)p_member->node_id, "DD IFCP MEMBER" );
               break;
            default:
               break;
         }
      }
      __LOG_INFO("****************************************\n");
   }
}


/********************************************************************
********************************************************************/
void 
ISNSDisplay_DDS( SOIP_Dds *p_dds, int level )
{
   ISNS_LIST_NODE *pnode;

    if ( level == LO_DETAIL )
    {
       __LOG_INFO("%u, %#x, %s\n", p_dds->id, p_dds->status, p_dds->sym_name);
    }
    else
    {
       __LOG_INFO("*************** DDS Entry ***************\n");
       __LOG_INFO("DDS ID         : %u\n",       p_dds->id);
       __LOG_INFO("DDS SYM NAME   : %s\n",        p_dds->sym_name);
       __LOG_INFO("DDS STATUS     : %#x (%s)\n", p_dds->status, 
          (p_dds->status & 0x1)?"Enabled":"Disabled");
       __LOG_INFO("DDS DD: ");
       pnode = NULL;
       while ( (pnode = GetNextNode(&p_dds->dd_list, pnode)) )
       {
          __LOG_INFO("%u ", *(uint32_t *)GetNodeData(pnode));
       }
       __LOG_INFO("\n");

       __LOG_INFO("****************************************\n");
    }
}

/********************************************************************
********************************************************************/
static void 
SNSDisplay_Field ( char *p_field_name, char *p_field, int len )
{
    int indx;

    if (p_field != NULL)
    {
        __LOG_INFO ("%-19s : ", p_field_name);
        for (indx = 0; indx <  len; indx++) 
            __LOG_INFO ("%2.2x", *(unsigned char *)&p_field[ indx ]);
        __LOG_INFO ("\n"); 
    }
}

/********************************************************************
********************************************************************/
void ISNSDisplay_iFCPNode ( SOIP_Ifcp *p_node )
{
    int ii;
    __LOG_INFO ("************* iFCP NODE RECORD **************\n"); 
    SNSDisplay_Field ("Port Name", (char *)&p_node->port_name, ISNS_PORT_NAME_SIZE);
    SNSDisplay_Field ("Node Name", (char *)&p_node->node_name, ISNS_NODE_NAME_SIZE);
    __LOG_INFO ("Port Type           : %#x     \n", p_node->type);
    SNSDisplay_Field ("Port ID", (char *)&p_node->id, ISNS_PORT_ID_SIZE);
    __LOG_INFO ("Port Symbolic Name  : %s      \n",  p_node->sym_name);
    SNSDisplay_Field ("FC Port Name", (char *)&p_node->fabric_port_name, 8);
    ISNSDisplay_HardAddr (&p_node->hard_addr);
    ISNSDisplay_IPAddress (&p_node->ip_addr);
    __LOG_INFO ("FC COS              : %08X \n", (unsigned int)p_node->cos);

    for (ii = 0; ii < 32; ii++)
        if (p_node->fc4_types.bitmap[ii])
           __LOG_INFO ("FC-4 Types          : 0x%2x [%d] \n", 
                   p_node->fc4_types.bitmap[ii], ii);

    __LOG_INFO ("**********************************************\n"); 
}

/********************************************************************
********************************************************************/
void ISNSDisplay_FCNode ( SOIP_Fc_Node *p_node )
{
    ISNS_LIST_NODE *pnode;

    __LOG_INFO ("************* FC NODE RECORD **************\n"); 
    SNSDisplay_Field ("Node Name", (char *)&p_node->node_name, ISNS_NODE_NAME_SIZE);
    __LOG_INFO ("Symbolic Name      : %s      \n",  p_node->sym_name);
    ISNSDisplay_IPAddress (&p_node->ip_addr);
    SNSDisplay_Field ("FC Device IPA", (char *)&p_node->node_ipa, 8);

    pnode=NULL;
    while ((pnode=GetNextNode(&p_node->port_list, pnode)))
    {
          SNSDisplay_Field ("Port Name", (char *)GetNodeData(pnode), ISNS_PORT_NAME_SIZE);
    }
    __LOG_INFO ("*********************************************\n"); 
}

/********************************************************************
********************************************************************/
void ISNSDisplay_ISCSINode ( SOIP_Iscsi *p_node, int level )
{
    if (level == LO_DETAIL)
    {
       __LOG_INFO(" %s, %s, %#08x, %s, %08x, %u, %u, %s\n",
              p_node->id.v, p_node->entity_id.id, p_node->type,
              p_node->alias, p_node->scn_bitmap, 
              p_node->iscsi_index, p_node->entity_index,
              p_node->activeFlag?"Active":"Not Active");
    }
    else
    {
       __LOG_INFO ("************* iSCSI NODE RECORD **************\n"); 
       __LOG_INFO ("Index              : %u\n", p_node->iscsi_index);
       __LOG_INFO ("iSCSI Name         : %s      \n", p_node->id.v);
       __LOG_INFO ("Node Type          : %#08x  \n", p_node->type);
       __LOG_INFO ("Alias              : %s      \n", p_node->alias);
       __LOG_INFO ("Entity ID          : %s      \n", p_node->entity_id.id);
       __LOG_INFO ("Entity Index       : %u\n", p_node->entity_index);
       __LOG_INFO ("DD Enabled         : %s      \n", p_node->activeFlag ? "Yes" : "No ");
       __LOG_INFO ("*********************************************\n"); 
    }
}

/********************************************************************
********************************************************************/
void ISNSDisplay_Entity ( SOIP_Entity *p_entity, int level )
{
    ISNS_LIST_NODE *pnode;

    if ( level == LO_DETAIL )
    {
       __LOG_INFO( "%s, %#08x, %u\n", p_entity->eid.id, 
               p_entity->eid_type, p_entity->entity_index );
    }
    else
    {
       __LOG_INFO ("******************** ENTITY RECORD **************\n"); 
       __LOG_INFO ("INDEX           : %#x\n",   p_entity->entity_index);
       __LOG_INFO ("ID              : %s\n",   p_entity->eid.id);
       __LOG_INFO ("Type            : %d\n",   p_entity->eid_type);
       __LOG_INFO ("Prot Ver Max/Min: %d/%d\n", 
                p_entity->prot_ver.ver_max, p_entity->prot_ver.ver_min);
       __LOG_INFO ("Reg Period      : %d\n",   p_entity->period);
       //ISNSDisplay_IPAddress ((struct IP_address *)p_entity->mgmt_ip_addr.v);
       __LOG_INFO ("TimeStamp       : %s",  ctime((const time_t *)&p_entity->timestamp.t_time));

       pnode = NULL;
       while ( (pnode = GetNextNode(&p_entity->iportal_list, pnode)) )
       {
           ISNSDisplay_Portal_Name ( (char *)GetNodeData(pnode) );
       }

       switch ( p_entity->eid_type )
       {
          case ( ENTITY_TYPE_IFCP ):
             pnode = NULL;
             while ( (pnode = GetNextNode( &p_entity->ifcp_node_list, pnode )) )
             {
                ISNSDisplay_Port_Name ((struct soip_port_name *)GetNodeData( pnode ), "iFCP WWPN");
             }
             break;
          case (ENTITY_TYPE_ISCSI):
             pnode=NULL;
             while ( (pnode = GetNextNode(&p_entity->iscsi_node_list, pnode)) )
             {
                   __LOG_INFO("iSCSI Node ID: %s\n", GetNodeData( pnode ));
             }
             break;
          default:
             __LOG_INFO("(Entity type unknown %d)", p_entity->eid_type);
             break;
       }
       __LOG_INFO ("***********************************************\n");
    }
}

/********************************************************************
********************************************************************/
void
ISNSDisplay_Portal(SOIP_Portal *p_portal, int level)
{
   ISNS_LIST_NODE    *pnode;

   /* Assumes a IPv4 Address */
   if (level == LO_DETAIL)
   {
       __LOG_INFO( " %s %#08x,%u\n", 
              inet_ntoa(*(struct in_addr *)(p_portal->ip_addr.v+12)),
              p_portal->ip_port, p_portal->portal_index);
   }
   else
   {
       __LOG_INFO("******************** Portal Record **************\n"); 
       __LOG_INFO("INDEX           : %u\n",      p_portal->portal_index);
       __LOG_INFO("IP              : %s\n",       inet_ntoa(*(struct in_addr *)(p_portal->ip_addr.v+12)));
       __LOG_INFO("PORT            : %#x\n",     p_portal->ip_port);
       __LOG_INFO("SYM NAME        : %s\n",       p_portal->sym_name);

       pnode = NULL;
       __LOG_INFO("ENTITY          : %s\n",      p_portal->entity_id.id);
       __LOG_INFO("ESI INTERVAL    : %u\n",      p_portal->esi_interval);
       __LOG_INFO("ESI/SCN UDP PORT: %u\n",      p_portal->esi_port);
       __LOG_INFO ("*************************************************\n");
   }
}

/********************************************************************
********************************************************************/
void ipaddrchar(void * ipaddrhex, void * ipaddrstr)
{
   uint32_t x1,x2,x3,x4,*xp;
   xp = (uint32_t *)ipaddrhex;
   x1 = ntohl(*xp++); x2 = ntohl(*xp++); x3 = ntohl(*xp++); x4 = ntohl(*xp);
   if (x1==0 && x2==0 && x3==0) {
      unsigned char *cp = (unsigned char *)xp;
      x1 = *cp++; x2 = *cp++; x3 = *cp++; x4 = *cp;
   }
   sprintf(ipaddrstr,"%d.%d.%d.%d",x1,x2,x3,x4);
}

/********************************************************************
********************************************************************/
char * FuncIDText (uint16_t funcid)
{
   switch (funcid)
   {
    /* Request */
    case ISNS_REG_DEV_ATTR_REQ     : return("Register Device Attribute Request");
    case ISNS_DEV_ATTR_QRY_REQ     : return("Device Attribute Query Request");
    case ISNS_DEV_GET_NXT_REQ      : return("Device Get Next Request");
    case ISNS_DEREG_DEV_REQ        : return("Deregister Device Request");
    case ISNS_SCN_REG_REQ          : return("SCN Register Request");
    case ISNS_SCN_DEREG_REQ        : return("SCN Deregister Request");
    case ISNS_SCN_EVENT            : return("SCN Event");
    case ISNS_SCN                  : return("State Change Notification");
    case ISNS_REG_DD_REQ           : return("Discovery Domain Register");
    case ISNS_DEREG_DD_REQ         : return("Discovery Domain Deregister");
    case ISNS_REG_DDS_REQ          : return("Discovery Domain Set Register");
    case ISNS_DEREG_DDS_REQ        : return("Discovery Domain Set Deregister");
    case ISNS_ESI                  : return("Entity Status Inquiry");
    case ISNS_HEART_BEAT           : return("Name Service Heartbeat");
    case ISNS_REQ_SW_ID_REQ        : return("Request Switch ID Request");
    case ISNS_REL_SW_ID_REQ        : return("Release Switch ID Request");
    case ISNS_GET_SW_ID_REQ        : return("Get Switch ID Request");
    case ISNS_DDS_GET_NXT_MEMBER_REQ: return("Get DDS Next Member Request");
    case ISNS_DD_GET_NXT_ISCSI_MEMBER_REQ: return("Get DDS Next Member Request");
    case ISNS_ENTITY_GET_NXT_PORTAL_REQ: return("Get Entity Next Portal Request");

    /* Response */
    case ISNS_REG_DEV_ATTR_RES     : return("Register Device Attribute Response");
    case ISNS_DEV_ATTR_QRY_RES     : return("Device Attribute Query Response");
    case ISNS_DEV_GET_NXT_RES      : return("Device Get Next Response");
    case ISNS_DEREG_DEV_RES        : return("Deregister Device Response");
    case ISNS_SCN_REG_RES          : return("SCN Register Response");
    case ISNS_SCN_DEREG_RES        : return("SCN Degregister Response");
    case ISNS_SCN_EVENT_RES        : return("SCN Event Response");
    case ISNS_SCN_RES              : return("SCN Response");
    case ISNS_REG_DD_RES           : return("Discovery Domain Register Response");
    case ISNS_DEREG_DD_RES         : return("Discovery Domain Deregister Response");
    case ISNS_REG_DDS_RES          : return("Discovery Domain Set Register Response");
    case ISNS_DEREG_DDS_RES        : return("Discovery Domain Set Deregister Response");
    case ISNS_ESI_RSP              : return("Entity Status Inquiry Response");
    case ISNS_REQ_SW_ID_RES        : return("Request Switch ID Response");
    case ISNS_REL_SW_ID_RES        : return("Release Switch ID Response");
    case ISNS_GET_SW_ID_RES        : return("Get Switch ID Response");
    case ISNS_DDS_GET_NXT_MEMBER_RES: return("Get DDS Next Member Response");
    case ISNS_DD_GET_NXT_ISCSI_MEMBER_RES: return("Get DDS Next Member Response");
    case ISNS_ENTITY_GET_NXT_PORTAL_RES: return("Get Portal Next Entity Response");

    /* Internal */
    case ESI_TIMEOUT_EVENT      : return("ESI Timeout Event");
    case FSM_TIMEOUT_EVENT      : return("FSM Timeout Event");
    case REQ_TIMEOUT_EVENT      : return("REQ Timeout Event");

    default                        : return("Unknown Message Type");
   }
}

/********************************************************************
********************************************************************/
char * isnsTagText (uint32_t tagid)
{
    switch (tagid)
    {
        case ISNS_DELIMITER          : return("Delimiter");
        case ISNS_ENTITY_ID          : return("Entity Identifier");
        case ISNS_ENTITY_TYPE        : return("Entity Protocol");
        case ISNS_MGMT_IP            : return("Mgmt IP Address");
        case ISNS_TIMESTAMP          : return("Timestamp");
        case ISNS_PROT_VER           : return("Protocol Version Range");
        case ISNS_ENTITY_PERIOD      : return("Entity Period");
        case ISNS_ENTITY_CERT        : return("Entity Certificate");
        case ISNS_PORTAL_IP          : return("Portal IP-Address");
        case ISNS_PORTAL_PORT        : return("Portal TCP/UDP Port");
        case ISNS_PORTAL_SYM_NAME    : return("Portal Symbolic Name");
        case ISNS_ESI_INTERVAL       : return("ESI Interval");
        case ISNS_ESI_PORT           : return("ESI/SCN UDP Port");
        case ISNS_PORTAL_CERT        : return("Portal Certificate");
        case ISNS_ISCSI_NODE_ID      : return("iSCSI Name");
        case ISNS_ISCSI_TYPE         : return("iSCSI Node Type");
        case ISNS_ISCSI_ALIAS        : return("iSCSI Node Alias");
        case ISNS_ISCSI_SCN_BITMAP   : return("iSCSI SCN Bitmap");
        case ISNS_ISCSI_CERT         : return("iSCSI Node Certificate");
        case ISNS_PORT_NAME          : return("Port Name");
        case ISNS_PORT_ID            : return("Port_ID");
        case ISNS_PORT_TYPE          : return("Port_Type");
        case ISNS_PORT_SYM_NAME      : return("Port_Symbolic Name");
        case ISNS_FABRIC_PORT_NAME   : return("FC Fabric Port Name");
        case ISNS_FC_HARD_ADDR       : return("FC Hard Address");
        case ISNS_FC_PORT_IP         : return("FC Port IP-Address");
        case ISNS_FC_COS             : return("FC Class of Service");
        case ISNS_FC4_TYPE           : return("FC FC-4 Types");
        case ISNS_FC4_DESC           : return("FC FC-4 Descriptor");
        case ISNS_FC4_FEATURE        : return("FC FC-4 Features");
        case ISNS_IFCP_SCN_BITMAP    : return("iFCP Node SCN bitmap");
        case ISNS_IFCP_NODE_CERT     : return("iFCP Node Certificate");
        case iSNS_FC4_TYPE_QUERY_KEY : return("FC-4 Type Code");
        case ISNS_NODE_NAME          : return("FC Device WWNN");
        case ISNS_NODE_SYM_NAME      : return("FC Device Sym Node Name");
        case ISNS_FC_NODE_IP         : return("FC Device IP-Address");
        case ISNS_FC_NODE_IPA        : return("FC Device IPA");
        case ISNS_FC_NODE_CERT       : return("FC Device Certificate");
        case ISNS_DDS_ID             : return("Discovery Domain Set ID");
        case ISNS_DDS_SYM_NAME       : return("Discovery Domain Set Symbolic Name");
        case ISNS_DDS_STATUS         : return("Discovery Domain Set Status");
        case ISNS_DD_ID              : return("Discovery Domain ID");
        case ISNS_DD_FEATURE_BITMAP  : return("Discovery Domain Feature Bitmap");
        case ISNS_DD_SYM_NAME        : return("Discovery Domain Symbolic Name");
        case ISNS_DD_ISCSI_MEMBER    : return("Discovery Domain iSCSI Member");
        case ISNS_DD_IFCP_MEMBER     : return("Discovery Domain iFCP Member");
        case ISNS_COMPANY_OUI        : return("Company OUI");
        case ISNS_ENTITY_IDX         : return("Entity Index");
        case ISNS_ISCSI_IDX          : return("iSCSI Index");
        case ISNS_PORTAL_IDX         : return("Portal Index");
        case ISNS_DD_ISCSI_MEMBER_IDX: return("Discovery Domain iSCSI Member Index");
        case ISNS_DD_ACTIVE          : return("Discovery Domain Active");
        case ISNS_NODE_ACTIVE        : return("Node Active");
        case ISNS_SCN_PORT           : return("SCN Port");
        case ISNS_PORTAL_GROUP       : return("Portal Group Tag (depreciated)");
        case ISNS_PORTAL_GROUP_TAG   : return("Portal Group Tag");
        case ISNS_PORTAL_GROUP_ISCSI_NAME : return("Portal Group ISCSI Name");
        case ISNS_PORTAL_GROUP_IP    : return("Portal Group IP Address");
        case ISNS_PORTAL_GROUP_PORT  : return("Portal Group TCP/UDP Port");
        case ISNS_PORTAL_SECURITY_BITMAP:  return("Portal Security Bitmap");
        default                      : return("Unknown TAG code");
    }
}

/********************************************************************
********************************************************************/
char * errorText(int errorCode)
{
    switch (errorCode)
    {
        case  0: return ("No error detected");
        case  1: return ("Unknown error.");
        case  2: return ("Message format error.");
        case  3: return ("Invalid registration.");
        case  4: return ("Requested ESI period too short.");
        case  5: return ("Invalid query.");
        case  6: return ("Authentication unknown.");
        case  7: return ("Authentication absent.");
        case  8: return ("Authentication failed.");
        case  9: return ("No such entry.");
        case 10: return ("Version not supported.");
        case 11: return ("Internal bus error.");
        case 12: return ("Busy now.");
        case 13: return ("Option not understood");
        case 14: return ("Invalid update.");
        case 15: return ("Message not supported");
        case 16: return ("SCN event rejected.");
        case 17: return ("SCN registration rejected.");
        case 18: return ("Attribute not implemented.");
        case 19: return ("SWITCH_ID not available.");
        case 20: return ("SWITCH_ID not allocated.");
        default: return ("Unknown error code.");
    }
}


/*********************************************************************/
/* Parses a PDU */
/*********************************************************************/
void
ParsePDU (char *buffer)
{
   ISNS_Attr    *p_attr;
   ISNS_Msg_Hdr *pdu;
   char         *ptr;
   uint16_t     pdufunc;
   uint16_t     pdulen;
   uint16_t     pduflags;
   uint32_t     attrtag; 
   uint32_t     attrlen; 
   int          i=0;
   int          *valword;

   pdu = (ISNS_Msg_Hdr *)buffer;
   __LOG_INFO("PDU Header: Version(%d) Func(%04X=%s) Len(%d) Flags(%04X) XID(%04X) Seq(%04X)\n",
              ntohs(pdu->version),
   pdufunc  = ntohs(pdu->type), FuncIDText(ntohs(pdu->type)),
   pdulen   = ntohs(pdu->msg_len),
   pduflags = ntohs(pdu->flags),
              ntohs(pdu->xid),
              ntohs(pdu->sequence));
   ptr = buffer + sizeof (ISNS_Msg_Hdr);
   if (pdufunc >= ISNS_START_VALID_RES_FUNC_ID)
   {
      __LOG_INFO ("Error Code = %08X, \"%s\"\n", ntohl (*(uint32_t *) ptr), errorText(ntohl (*(uint32_t *) ptr)));
      ptr += 4;
   }
   if (pdufunc == ISNS_HEART_BEAT)
   {
     ISNS_Hb * hb = (ISNS_Hb *)ptr;
     char     ipaddr[45];
     ipaddrchar(hb->ip_addr.v, ipaddr);
     __LOG_INFO("IP address = %s\n",ipaddr);
     __LOG_INFO("UDP port = %d\n",ntohs(hb->udp_port));
     __LOG_INFO("TCP port = %d\n",ntohs(hb->tcp_port));
     __LOG_INFO("Interval = %d\n",ntohl(hb->interval));
     __LOG_INFO("Counter = %d\n",ntohl(hb->counter));
   }
   else
   while (ptr < buffer + sizeof (ISNS_Msg_Hdr) + pdulen)
   {
      p_attr = (struct ISNS_attr *)ptr;
      attrtag = ntohl (p_attr->tag);
      attrlen = ntohl (p_attr->len);
      __LOG_INFO("%08X %08X ",attrtag,attrlen);
      if (attrlen==0) __LOG_INFO("Tag: %s (empty)\n", isnsTagText(attrtag));
      else switch (attrtag)
      {
      case ISNS_ENTITY_PERIOD      :
      case ISNS_PORTAL_PORT        :
      case ISNS_ESI_INTERVAL       :
      case ISNS_ESI_PORT           :
      case ISNS_DDS_ID             :
      case ISNS_DD_ID              :
      case ISNS_DD_FEATURE_BITMAP  :
         __LOG_INFO ("Tag: %s = %d\n", isnsTagText(attrtag), ntohl(p_attr->val.index));
         break;
      case ISNS_ENTITY_ID          :
      case ISNS_PORTAL_SYM_NAME    :
      case ISNS_DDS_SYM_NAME       :
      case ISNS_DD_SYM_NAME        :
      case ISNS_ISCSI_NODE_ID      :
      case ISNS_ISCSI_ALIAS        :
      case ISNS_PORT_NAME          :
      case ISNS_PORT_SYM_NAME      :
      case ISNS_FABRIC_PORT_NAME   :
      case ISNS_NODE_SYM_NAME      :
      case ISNS_DD_ISCSI_MEMBER    :
      case ISNS_DD_IFCP_MEMBER     :
         __LOG_INFO ("Tag: %s = \"%s\"\n", isnsTagText(attrtag), (char *)&p_attr->val);
         break;
      case ISNS_PORT_TYPE          :
         __LOG_INFO ("Tag: %s = %08X (%s)\n", isnsTagText(attrtag),
                 ntohl(p_attr->val.index), ntohl(p_attr->val.index)?"TCP":"UDP");
         break;
      case ISNS_MGMT_IP            :
      case ISNS_PORTAL_IP          :
      case ISNS_FC_PORT_IP         :
      case ISNS_FC_NODE_IP         :
         {
           char     ipaddr[45];
           ipaddrchar(&p_attr->val,&ipaddr);
           __LOG_INFO ("Tag: %s = %s\n", isnsTagText(attrtag),ipaddr);
         }
         break;
      case ISNS_TIMESTAMP          :
      {
         time_t timevalue;
         timevalue = ntohl(p_attr->val.timestamp.t_time);
         __LOG_INFO ("Tag: %s = %s", isnsTagText(attrtag),
                 ctime(&timevalue));
         break;
      }
      case ISNS_PROT_VER           :
         __LOG_INFO ("Tag: %s = %04X-%04X\n", isnsTagText(attrtag),
                 ntohs(p_attr->val.prot_ver.ver_min),ntohs(p_attr->val.prot_ver.ver_max));
         break;
      case ISNS_ISCSI_TYPE         :
         {
            char iscsi_type_str[256]={0};
            uint32_t type = ntohl(p_attr->val.index);
            if (type & ISNS_ISCSI_TYPE_TARGET)
               strcat(iscsi_type_str, " Target");
            if (type & ISNS_ISCSI_TYPE_INITIATOR)
               strcat(iscsi_type_str, " Initiator");
            if (type & ISNS_ISCSI_TYPE_CONTROL)
               strcat(iscsi_type_str, " Control");
            if (type & ISNS_ISCSI_TYPE_REMOTE)
               strcat(iscsi_type_str, " Remote");

            __LOG_INFO ("Tag: %s = %08X (%s)\n", isnsTagText(attrtag),
                   type, iscsi_type_str);
         }
         break;
      case ISNS_ENTITY_TYPE        :
         {
           char * entity_type_str;
           switch (ntohl(p_attr->val.index))
           {
             case ENTITY_TYPE_ISCSI: entity_type_str="iSCSI"; break;
             case ENTITY_TYPE_IFCP : entity_type_str="iFCP";  break;
             default: entity_type_str="unrecognized Entity type";
           }
           __LOG_INFO ("Tag: %s = %08X (%s)\n", isnsTagText(attrtag),
                   ntohl(p_attr->val.index), entity_type_str);
         }
         break;
      case ISNS_DDS_STATUS:
         __LOG_INFO ("Tag: %s = %08X (%s)\n", isnsTagText(attrtag), ntohl(p_attr->val.index),
                 ntohl(p_attr->val.index)?"Enabled":"Disabled");
         break;
      case ISNS_ENTITY_CERT        :
      case ISNS_PORTAL_CERT        :
      case ISNS_ISCSI_SCN_BITMAP   :
         {
            uint32_t scn;
            char buffer[256]={0};
            scn = ntohl(p_attr->val.index);
            if (scn & ISNS_SCN_DETAILED)
               strcat(buffer, " Detailed SCN");
            if (scn & ISNS_SCN_MEMBER_ADDED)
               strcat(buffer, " Member Added");
            if (scn & ISNS_SCN_MEMBER_REMOVED)
               strcat(buffer, " Member Removed");
            if (scn & ISNS_SCN_OBJ_UPDATED)
               strcat(buffer, " Object Update");
            if (scn & ISNS_SCN_OBJ_ADDED)
               strcat(buffer, " Object Added");
            if (scn & ISNS_SCN_OBJ_REMOVED)
               strcat(buffer, " Object Removed");

         __LOG_INFO ("Tag: %s = %08X (%s)\n", isnsTagText(attrtag), ntohl(p_attr->val.index),
                 buffer);
         }
         break;
      case ISNS_ISCSI_CERT         :
      case ISNS_PORT_ID            :
      case ISNS_FC_HARD_ADDR       :
      case ISNS_FC_COS             :
      case ISNS_FC4_TYPE           :
      case ISNS_FC4_DESC           :
      case ISNS_FC4_FEATURE        :
      case ISNS_IFCP_SCN_BITMAP    :
         {
            uint32_t scn;
            char buffer[256]={0};
            scn = ntohl(p_attr->val.index);
            if (scn & ISNS_SCN_DETAILED)
               strcat(buffer, " Detailed SCN");
            if (scn & ISNS_SCN_MEMBER_ADDED)
               strcat(buffer, " Member Added");
            if (scn & ISNS_SCN_MEMBER_REMOVED)
               strcat(buffer, " Member Removed");
            if (scn & ISNS_SCN_OBJ_UPDATED)
               strcat(buffer, " Object Update");
            if (scn & ISNS_SCN_OBJ_ADDED)
               strcat(buffer, " Object Added");
            if (scn & ISNS_SCN_OBJ_REMOVED)
               strcat(buffer, " Object Removed");

         __LOG_INFO ("Tag: %s = %08X (%s)\n", isnsTagText(attrtag), ntohl(p_attr->val.index),
                 buffer);
         }
         break;
      case ISNS_IFCP_NODE_CERT     :
      case iSNS_FC4_TYPE_QUERY_KEY :
      case ISNS_NODE_NAME          :
      case ISNS_FC_NODE_CERT       :
      case ISNS_COMPANY_OUI        :
      default:
         __LOG_INFO ("Tag: %s = ", isnsTagText(attrtag));
         for (valword=(int *)&p_attr->val; (unsigned int) i<attrlen; i+=4) {
            __LOG_INFO("%08X ",ntohl(*valword++));
         }
         __LOG_INFO("\n");
         break;
      }
      ptr += (attrlen + 8);
   }
   __LOG_INFO("\n");
}

/*********************************************************************
*  print log message to console or system log 
**********************************************************************/
void
isns_log(int level, char *format, ...)
{
  va_list ptr;
  
  va_start(ptr,format);
  if (daemon_state == TRUE)
  {
    vsyslog(level,format,ptr);
  }
  else
  {
    switch (level)
    {
      case LOG_ERR:
         fprintf(stderr,"ERROR: ");
         break;
      case LOG_WARNING:
         fprintf(stderr,"WARNING: ");
         break;
      case LOG_DEBUG:
         fprintf(stderr,"Debug: ");
         break;
      case LOG_INFO:
         fprintf(stderr,"Info: ");
         break;
    }
    vfprintf(stderr, format, ptr);
  }
  va_end(ptr);
}

/*********************************************************************
*  isns_copy -- copy attribute with length checking on dst and src
**********************************************************************/
int
isns_copy(void *dst, int dst_size, void *src, int src_size)
{
   int cp_size = src_size;
   int rc = SUCCESS;

   if (dst == NULL)
   {
     __LOG_ERROR ("destination ptr is NULL");
     return ERROR;
   }
   if (src == NULL)
   {
     __LOG_ERROR ("source ptr is NULL");
     return ERROR;
   }
   
   if ( dst_size < src_size )
   {
     __LOG_ERROR ("destination size:%i is smaller than source:%i - setting dst_size=src_size",dst_size,src_size);
     cp_size = dst_size;
     rc = ERROR;
   }
   memset (dst,0,dst_size);
   memcpy (dst,src,cp_size);
   
   return rc;
}
