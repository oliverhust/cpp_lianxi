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
#include "isns.h"
#include "util.h"
#include "parse.h"

static int currentXid = 0;

/* Generates a new transaction id (xid) */
static int
GetXid (void)
{
   currentXid++;

   return (currentXid);

}

/*********************************************************************/
/* Zeros out the message and puts a hdr in */
/*********************************************************************/
void
ISNSCreateHdr (ISNS_FUNC_ID func_id, ISNS_CMD * cmd, int b_size, int flags)
{
   memset (cmd, 0, b_size);
   cmd->hdr.len = 0;
   cmd->hdr.flags =
      htons ((short)(flags | ISNS_FLAG_FIRST_PDU | ISNS_FLAG_LAST_PDU |
             ISNS_FLAG_SND_CLIENT));
   cmd->hdr.version = htons ((short)ISNSP_VERSION);
   cmd->hdr.func_id = htons ((short)func_id);
   cmd->hdr.xid = htons ((short)GetXid ());
}

/*********************************************************************/
/* Appends an attribute to a msg.  Does the necessary, byte 
conversions */
/*********************************************************************/
void
ISNSAppendAttr (ISNS_CMD * cmd, ISNS_TAG tag, int len, char *ptr, int val)
{
   ISNS_TLV_P tlvp = (ISNS_TLV_P)((char *) cmd + cmd->hdr.len + sizeof (ISNS_HDR));

   /* Attr */
   tlvp->attrib_id = htonl (tag);
   tlvp->attrib_len = htonl (len);
   if (ptr != NULL)
      memcpy (&tlvp->attrib_val, ptr, len);
   else
   {
      switch (len)
      {
      case 0:
         break;
      case 2:
         *(uint16_t *) & tlvp->attrib_val = htons ((short)val);
         break;
      case 4:
         *(uint32_t *) & tlvp->attrib_val = htonl (val);
         break;
      default:
         break;
      }
   }
   cmd->hdr.len += ISNS_TAG_LEN + ISNS_LEN_LEN + len;
}


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

char * FuncIDText (uint16_t funcid)
{
    switch (funcid)
    {
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
        default                        : return("");
    }
}

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
        case ISNS_PORTAL_GROUP_ISCSI_NAME   : return("Portal Group ISCSI Name");
        case ISNS_PORTAL_GROUP_IP       : return("Portal Group IP Address");
        case ISNS_PORTAL_GROUP_PORT     : return("Portal Group Port");
        case ISNS_PORTAL_GROUP_TAG      : return("Portal Group Tag");

        default                      : return("Unknown TAG code");
    }
}

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
   ISNS_Attr *p_attr;
   ISNS_HDR  *pdu;
   char      *ptr;
   uint16_t   pdufunc, pdulen, pduflags;
   uint32_t   attrtag, attrlen;
   int        i=0, *valword;

   pdu = (struct isns_hdr *)buffer;
   printf("PDU Header: Version(%d) Func(%04X=%s) Len(%d) Flags(%04X) XID(%04X) Seq(%04X)\n",
              ntohs(pdu->version),
   pdufunc  = ntohs(pdu->func_id), FuncIDText(ntohs(pdu->func_id)),
   pdulen   = ntohs(pdu->len    ),
   pduflags = ntohs(pdu->flags  ),
              ntohs(pdu->xid    ),
              ntohs(pdu->seq    ));
   ptr = buffer + sizeof (ISNS_HDR);
   if (pdufunc >= ISNS_START_VALID_RES_FUNC_ID)
   {
      printf ("Error Code = %08X, \"%s\"\n", ntohl (*(uint32_t *) ptr), errorText(ntohl (*(uint32_t *) ptr)));
      ptr += 4;
   }
   if (pdufunc == ISNS_HEART_BEAT)
   {
     SNS_Hb * hb = (SNS_Hb *)ptr;
     char     ipaddr[45];
     ipaddrchar(hb->ip_ptr,ipaddr);
     printf("IP address = %s\n",ipaddr);
     printf("UDP port = %d\n",ntohs(hb->udp_port));
     printf("TCP port = %d\n",ntohs(hb->tcp_port));
     printf("Interval = %d\n",ntohl(hb->interval));
     printf("Counter = %d\n",ntohl(hb->counter));
   }
   else
   while (ptr < buffer + sizeof (ISNS_HDR) + pdulen)
   {
      p_attr = (struct ISNS_attr *)ptr;
      attrtag = ntohl (p_attr->tag);
      attrlen = ntohl (p_attr->len);
      printf("%08X %08X ",attrtag,attrlen);
      if (attrlen==0) printf("Tag: %s (empty)\n", isnsTagText(attrtag));
      else switch (attrtag)
      {
      case ISNS_ENTITY_PERIOD      :
      case ISNS_PORTAL_PORT        :
      case ISNS_PORTAL_GROUP_PORT  :
      case ISNS_ESI_INTERVAL       :
      case ISNS_ESI_PORT           :
      case ISNS_DDS_ID             :
      case ISNS_DD_ID              :
      case ISNS_DD_FEATURE_BITMAP  :
         printf ("Tag: %s = %d\n", isnsTagText(attrtag), ntohl(p_attr->val.etype));
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
      case ISNS_PORTAL_GROUP_ISCSI_NAME :
         printf ("Tag: %s = \"%s\"\n", isnsTagText(attrtag),(char *)&p_attr->val);
         break;
      case ISNS_PORT_TYPE          :
         printf ("Tag: %s = %08X (%s)\n", isnsTagText(attrtag),
                 ntohl(p_attr->val.etype), ntohl(p_attr->val.etype)?"TCP":"UDP");
         break;
      case ISNS_MGMT_IP            :
      case ISNS_PORTAL_IP          :
      case ISNS_FC_PORT_IP         :
      case ISNS_FC_NODE_IP         :
      case ISNS_PORTAL_GROUP_IP    :
         {
           char     ipaddr[45];
           ipaddrchar(&p_attr->val,&ipaddr);
           printf ("Tag: %s = %s\n", isnsTagText(attrtag),ipaddr);
         }
         break;
      case ISNS_TIMESTAMP          :
      {  time_t timevalue;
         timevalue = ntohl(p_attr->val.timestamp.t_time);
         printf ("Tag: %s = %s", isnsTagText(attrtag),
                 ctime(&timevalue));
         break;
      }
      case ISNS_PROT_VER           :
         printf ("Tag: %s = %04X-%04X\n", isnsTagText(attrtag),
                 ntohs(p_attr->val.ver.min),ntohs(p_attr->val.ver.max));
         break;
      case ISNS_ISCSI_TYPE         :
         {
            char iscsi_type_str[256]={0};
            uint32_t type = ntohl(p_attr->val.etype);
            if (type & ISNS_ISCSI_TYPE_TARGET)
               strcat(iscsi_type_str, " Target");
            if (type & ISNS_ISCSI_TYPE_INITIATOR)
               strcat(iscsi_type_str, " Initiator");
            if (type & ISNS_ISCSI_TYPE_CONTROL)
               strcat(iscsi_type_str, " Control");
            if (type & ISNS_ISCSI_TYPE_REMOTE)
               strcat(iscsi_type_str, " Remote");

            printf ("Tag: %s = %08lX (%s)\n", isnsTagText(attrtag),
                  (long)type, iscsi_type_str);
         }
         break;
      case ISNS_ENTITY_TYPE        :
         {
           char * entity_type_str;
           switch (ntohl(p_attr->val.etype))
           {
             case ENTITY_TYPE_ISCSI: entity_type_str="iSCSI"; break;
             case ENTITY_TYPE_IFCP : entity_type_str="iFCP";  break;
             default: entity_type_str="unrecognized Entity type";
           }
           printf ("Tag: %s = %08X (%s)\n", isnsTagText(attrtag),
                   ntohl(p_attr->val.etype), entity_type_str);
         }
         break;
      case ISNS_DDS_STATUS:
         printf ("Tag: %s = %08X (%s)\n", isnsTagText(attrtag), ntohl(p_attr->val.etype),
                 ntohl(p_attr->val.etype)?"Enabled":"Disabled");
         break;
      case ISNS_ENTITY_CERT        :
      case ISNS_PORTAL_CERT        :
      case ISNS_ISCSI_SCN_BITMAP   :
      case ISNS_ISCSI_CERT         :
      case ISNS_PORT_ID            :
      case ISNS_FC_HARD_ADDR       :
      case ISNS_FC_COS             :
      case ISNS_FC4_TYPE           :
      case ISNS_FC4_DESC           :
      case ISNS_FC4_FEATURE        :
      case ISNS_IFCP_SCN_BITMAP    :
      case ISNS_IFCP_NODE_CERT     :
      case iSNS_FC4_TYPE_QUERY_KEY :
      case ISNS_NODE_NAME          :
      case ISNS_FC_NODE_CERT       :
      case ISNS_COMPANY_OUI        :
         printf ("Tag: %s = ", isnsTagText(attrtag));
         for (valword=(int *)&p_attr->val.etype, i = 0; i<attrlen; i+=4) {
            printf("%08X ",ntohl(*valword++));
         }
         printf("\n");
         break;
      default:
         printf ("Tag: %s \n", isnsTagText(attrtag));
         break;
      }
      ptr += (attrlen + 8);
   }
   printf("\n");
}


/********************************************************************
* Functions to parse and print query results 
*********************************************************************/

/***********************************************************************/
/* Parses an entity query response */
/***********************************************************************/
void
ParseEntityQuery (char *buffer)
{
   ISNS_Attr *p_attr;
   ISNS_HDR *pdu;
   char *ptr;

   pdu = (struct isns_hdr *)buffer;
   ptr = buffer + sizeof (ISNS_HDR) + 4;
   while (ptr < buffer + sizeof (ISNS_HDR) + pdu->len)
   {
      p_attr = (struct ISNS_attr *)ptr;
      p_attr->tag = htonl (p_attr->tag);
      p_attr->len = htonl (p_attr->len);
      switch (p_attr->tag)
      {
      case ISNS_ENTITY_ID:
         printf ("---------------------------------\n");
         printf ("Entity ID  : %s\n",(char *)&p_attr->val);
         break;
      case ISNS_ENTITY_TYPE:
         p_attr->val.etype = ntohl (p_attr->val.etype);
         printf ("Entity Type: %d\n", p_attr->val.etype);
         break;
      case ISNS_MGMT_IP:
         {
            struct in_addr ip;
            ip.s_addr = *(uint32_t *) ((char *) &p_attr->val + 12);
            printf ("Mgmt IP: %s.\n", inet_ntoa (ip));
         }
         break;
      case ISNS_TIMESTAMP:
      {
         time_t timevalue;
         timevalue = ntohl (p_attr->val.timestamp.t_time);
         printf ("TimeStamp: %s", ctime (&timevalue));
         break;
      }
      case ISNS_PROT_VER:
         p_attr->val.ver.max = ntohs (p_attr->val.ver.max);
         p_attr->val.ver.min = ntohs (p_attr->val.ver.min);
         printf ("Version: Max=%d, Min=%d.\n", p_attr->val.ver.max,
                 p_attr->val.ver.min);
         break;
      case ISNS_ENTITY_PERIOD:
         p_attr->val.etype = ntohl (p_attr->val.etype);
         printf ("Period: %d secs.\n", p_attr->val.etype);
         break;
      case ISNS_ENTITY_IDX:
         p_attr->val.etype = ntohl (p_attr->val.etype);
         printf ("Entity Index: %d.\n", p_attr->val.etype);
         break;
      case ISNS_DELIMITER:
         break;
      default:
         break;
      }
      ptr = (char *) ptr + p_attr->len + 8;
   }
}

/***********************************************
This will parse the response msg from an iSCSI
query msg.
***********************************************/
void
ParseISCSIQuery (char *buffer)
{
   ISNS_Attr *p_attr;
   ISNS_HDR *pdu;
   char *ptr;

   pdu = (struct isns_hdr *)buffer;
   ptr = buffer + sizeof (ISNS_HDR) + 4 /* Error Code Length */;

   while (ptr < (char *) buffer + sizeof (ISNS_HDR) + pdu->len)
   {
      p_attr = (struct ISNS_attr *)ptr;
      p_attr->tag = htonl (p_attr->tag);
      p_attr->len = htonl (p_attr->len);
      /*Attributes can come back in any order */
      switch (p_attr->tag)
      {
      case ISNS_ISCSI_NODE_ID:
         printf ("---------------------------------\n");
         printf ("iSCSI ID  : %s\n",(char *)&p_attr->val);
         break;
      case ISNS_ISCSI_IDX:
         p_attr->val.etype = ntohl (p_attr->val.etype);
         printf ("iSCSI Index: %d\n", p_attr->val.etype);
         break;
      case ISNS_ISCSI_TYPE:
         p_attr->val.etype = ntohl (p_attr->val.etype);
         printf ("Type: %#x (", p_attr->val.etype);
         if (p_attr->val.etype & 1)
            printf ("Target");
         if (p_attr->val.etype & 2)
            printf ("Initiator");
         printf (")\n");
         break;
      case ISNS_ISCSI_ALIAS:
         printf ("Alias      : %s\n",(char *)&p_attr->val);
         break;
      case ISNS_DD_ID:
         p_attr->val.etype = ntohl (p_attr->val.etype);
         printf ("DD ID      : %d\n", p_attr->val.etype);
         break;
      case ISNS_ENTITY_ID:
         printf ("Entity ID  : %s\n",(char *)&p_attr->val);
         break;
      case ISNS_ENTITY_IDX:
         p_attr->val.etype = ntohl (p_attr->val.etype);
         printf ("Entity Idx : %d\n", p_attr->val.etype);
         break;
      case ISNS_PORTAL_IP:
         {
            struct in_addr ip;
            ip.s_addr = *(uint32_t *) ((char *) &p_attr->val + 12);
            printf ("Portal IP     : %s.\n", inet_ntoa (ip));
         }
         break;
      case ISNS_PORTAL_PORT:
         p_attr->val.etype = ntohl (p_attr->val.etype);
         printf ("Portal Port    : %d.\n", p_attr->val.etype & 0xFFFF);
         printf ("Portal Type    : %s.\n",
                 p_attr->val.etype & 0x10000 ? "UDP" : "TCP");
         break;

      case ISNS_PORTAL_IDX:
         p_attr->val.etype = ntohl (p_attr->val.etype);
         printf ("Portal Idx : %d\n", p_attr->val.etype);
         break;
      case ISNS_PROT_VER:
         p_attr->val.ver.max = ntohs (p_attr->val.ver.max);
         p_attr->val.ver.min = ntohs (p_attr->val.ver.min);
         printf ("Version: Max=%d, Min=%d.\n", p_attr->val.ver.max,
                 p_attr->val.ver.min);
         break;
      default:
         break;
      }
      ptr = (char *) ptr + p_attr->len + 8/* Size of tag and length fields*/;
   }
}

/***********************************************************************/
/* Parses a portal response */
/***********************************************************************/
void
ParsePortalQuery (char *buffer)
{
   ISNS_Attr *p_attr;
   ISNS_HDR *pdu;
   char *ptr;

   pdu = (struct isns_hdr *)buffer;
   ptr = buffer + sizeof (ISNS_HDR) + 4;
   while (ptr < buffer + sizeof (ISNS_HDR) + pdu->len)
   {
      p_attr = (struct ISNS_attr *)ptr;
      p_attr->tag = htonl (p_attr->tag);
      p_attr->len = htonl (p_attr->len);
      switch (p_attr->tag)
      {
      case ISNS_PORTAL_IP:
         {
            struct in_addr ip;
            printf ("---------------------------------\n");
            ip.s_addr = *(uint32_t *) ((char *) &p_attr->val + 12);
            printf ("Portal IP     : %s.\n", inet_ntoa (ip));
         }
         break;
      case ISNS_PORTAL_PORT:
         p_attr->val.etype = ntohl (p_attr->val.etype);
         printf ("Portal Port    : %d.\n", p_attr->val.etype & 0xFFFF);
         printf ("Portal Type    : %s.\n",
                 p_attr->val.etype & 0x10000 ? "UDP" : "TCP");
         break;
      case ISNS_PORTAL_SYM_NAME:
         printf ("Portal Sym Name: %s\n",(char *)&p_attr->val);
         break;
      case ISNS_ENTITY_ID:
         printf ("Entity ID  : %s\n",(char *)&p_attr->val);
         break;
      case ISNS_ENTITY_IDX:
         p_attr->val.etype = ntohl (p_attr->val.etype);
         printf ("Entity Index: %d.\n", p_attr->val.etype);
         break;
      case ISNS_ESI_PORT:
         p_attr->val.etype = ntohl (p_attr->val.etype);
         printf ("ESI/SCN UDP Port    : %d.\n", p_attr->val.etype & 0xFFFF);
         break;
      case ISNS_ESI_INTERVAL:
         p_attr->val.etype = ntohl (p_attr->val.etype);
         printf ("ESI Interval : %d.\n", p_attr->val.etype);
         break;
      case ISNS_PORTAL_IDX:
         p_attr->val.etype = ntohl (p_attr->val.etype);
         printf ("Portal Index: %d.\n", p_attr->val.etype);
         break;
      case ISNS_DELIMITER:
         break;
      default:
         break;
      }
      ptr = (char *) ptr + p_attr->len + 8;
   }
}


/*********************************************************************/
/* Parses an ESI */
/*********************************************************************/
void
ParseESI (char *buffer)
{
   ISNS_Attr *p_attr;
   ISNS_HDR *pdu;
   char *ptr;

   printf ("Parsing ESI.\n");
   pdu = (struct isns_hdr *)buffer;
   ptr = buffer + sizeof (ISNS_HDR);
   pdu->len = htons (pdu->len);
   while (ptr < (char *) buffer + sizeof (ISNS_HDR) + pdu->len)
   {
      p_attr = (struct ISNS_attr *)ptr;
      p_attr->tag = ntohl (p_attr->tag);
      p_attr->len = ntohl (p_attr->len);
      switch (p_attr->tag)
      {
      case ISNS_ENTITY_ID:
         printf ("---------------------------------\n");
         printf ("Entity ID  : %s\n",(char *)&p_attr->val);
         break;
      case ISNS_PORTAL_IP:
         {
            struct in_addr ip;
            ip.s_addr = *(uint32_t *) ((char *) &p_attr->val + 12);
            printf ("Portal IP     : %s.\n", inet_ntoa (ip));
         }
         break;
      case ISNS_PORTAL_PORT:
         p_attr->val.etype = ntohl (p_attr->val.etype);
         printf ("Portal Port    : %d.\n", p_attr->val.etype & 0xFFFF);
         printf ("Portal Port Type : %s.\n",
                 p_attr->val.etype & 0x10000 ? "UDP" : "TCP");
         break;
      default:
         break;
      }
      ptr = (char *) ptr + p_attr->len + 8;
   }
}

/*********************************************************************/
/* Parses a DDS Query */
/*********************************************************************/
void
ParseDDSQuery (char *buffer)
{
   ISNS_Attr *p_attr;
   ISNS_HDR *pdu;
   char *ptr;

   pdu = (struct isns_hdr *)buffer;
   ptr = buffer + sizeof (ISNS_HDR) + 4;
   while (ptr < buffer + sizeof (ISNS_HDR) + pdu->len)
   {
      p_attr = (struct ISNS_attr *)ptr;
      p_attr->tag = ntohl (p_attr->tag);
      p_attr->len = ntohl (p_attr->len);
      switch (p_attr->tag)
      {
      case ISNS_DDS_ID:
         printf ("---------------------------------\n");
         p_attr->val.etype = ntohl (p_attr->val.etype);
         printf ("DDS ID  : %d\n", p_attr->val.etype);
         break;
      case ISNS_DDS_SYM_NAME:
         printf ("DDS Sym Name : %s\n",(char *)&p_attr->val);
         break;
      case ISNS_DDS_STATUS:
         p_attr->val.etype = ntohl (p_attr->val.etype);
         printf ("Status: %s\n", p_attr->val.etype?"Active":"Disabled");
         break;
      case ISNS_DD_ID:
         p_attr->val.etype = ntohl (p_attr->val.etype);
         printf ("   DD ID : %d\n", p_attr->val.etype);
         break;
      case ISNS_DD_SYM_NAME:
         printf ("   DD Sym Name : %s\n",(char *)&p_attr->val);
         break;
      case ISNS_DD_ACTIVE:
         p_attr->val.etype = ntohl (p_attr->val.etype);
         printf ("Status: %s\n", p_attr->val.etype?"Active":"Disabled");
      default:
         break;
      }
      ptr = (char *) ptr + p_attr->len + 8;
   }
}

/*********************************************************************/
/* Parses a DD Query Response */
/*********************************************************************/
void
ParseDDQuery (char *buffer)
{
   ISNS_Attr *p_attr;
   ISNS_HDR *pdu;
   char *ptr;

   pdu = (struct isns_hdr *)buffer;
   ptr = buffer + sizeof (ISNS_HDR) + 4;
   if (pdu->len > MAX_PAYLOAD_SIZE)
      printf("ERROR pdu->len:%d\n",pdu->len);

   while (ptr < (buffer + sizeof (ISNS_HDR) + pdu->len) )
   {
      p_attr = (struct ISNS_attr *)ptr;
      p_attr->tag = ntohl (p_attr->tag);
      p_attr->len = ntohl (p_attr->len);
      switch (p_attr->tag)
      {
      case ISNS_DD_ID:
         printf ("---------------------------------\n");
         p_attr->val.etype = ntohl (p_attr->val.etype);
         printf ("DD ID  : %d\n", p_attr->val.etype);
         break;
      case ISNS_DD_SYM_NAME:
         printf ("DD Sym Name : %s\n",(char *)&p_attr->val);
         break;
      case ISNS_DDS_ID:
         p_attr->val.etype = ntohl (p_attr->val.etype);
         printf ("   DDS ID : %d\n", p_attr->val.etype);
         break;
      case ISNS_DDS_SYM_NAME:
         printf ("   DDS Sym Name : %s\n",(char *)&p_attr->val);
         break;
      case ISNS_DD_ISCSI_MEMBER_IDX:
         p_attr->val.etype = ntohl (p_attr->val.etype);
         printf ("   DD iSCSI Member Index  : %d\n", p_attr->val.etype);
         break;
      case ISNS_DD_ISCSI_MEMBER:
         printf ("   DD iSCSI Member : %s\n",(char *)&p_attr->val);
         break;
      case ISNS_DD_IFCP_MEMBER:
         {
            char buffer2[256];
            memset (buffer2, 0, sizeof (buffer2));
            printf ("   iFCP WWPN  : %s\n",
                    HexToAscii ((char *)&p_attr->val, p_attr->len, buffer2));
            break;
         }
      default:
         break;
      }
      ptr = (char *) ptr + p_attr->len + 8;
   }
}


