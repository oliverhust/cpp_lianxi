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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/basetype.h>
#include <sys/error.h>
#include <sys/list.h>
#include "iSNStypes.h"
#include "iSNSdb.h"
#include "iSNSmsg.h"
#include "iSNSparse.h"
#include "iSNSdebug.h"

extern int isns_parse_debug;

/*** local function prototypes ***/
static int
ISNSProfileKeysAttr (ISNS_Msg * msg,
                     ISNS_Attr **attr_index, int *num_attrs,
                     ISNS_Attr **key_index, int *num_keys, ISNS_Attr **src_attr);

/*************************************************************
* ISNSParseMsg
*************************************************************/
int
ISNSParseMsg (ISNS_Msg * msg, ISNS_Attr **attr_index, ISNS_Attr **key_index,
             ISNS_Attr **src_attr)
{
   int ii;
   int parse_status = SUCCESS;
   int num_attrs;
   int num_keys;

   /* verify input parameters */
   if ((msg == NULL) || (attr_index == NULL))
      return (ERROR);

   for (ii = 0; ii < SNS_MAX_ATTRS; ii++)
      attr_index[ii] = NULL;

   for (ii = 0; ii < SNS_MAX_ATTRS; ii++)
      key_index[ii] = NULL;

   //if ((msg->hdr.type > ISNS_END_VALID_REQ_FUNC_ID)
   //    && (msg->hdr.type < ISNS_START_VALID_RES_FUNC_ID))
   //   return (SUCCESS);
//
   //if (msg->hdr.type == ISNS_HEART_BEAT)
   //   return (SUCCESS);

   switch (msg->hdr.type)
   {
   case ISNS_HEART_BEAT:
         return (SUCCESS);
   case ISNS_REG_DEV_ATTR_REQ:
   case ISNS_ESI:
   case ISNS_SCN:
   case ISNS_DEV_GET_NXT_REQ:
   case ISNS_REG_DDS_REQ:
   case ISNS_REG_DD_REQ:
   case ISNS_DEREG_DDS_REQ:
   case ISNS_DEREG_DD_REQ:
   case ISNS_SCN_EVENT:
   case ISNS_DEREG_DEV_REQ:
   case ISNS_SCN_REG_REQ:
   case ISNS_SCN_DEREG_REQ:
   case ISNS_DEV_ATTR_QRY_REQ:
   case ISNS_DDS_GET_NXT_MEMBER_REQ:
   case ISNS_DD_GET_NXT_ISCSI_MEMBER_REQ:
   case ISNS_ENTITY_GET_NXT_PORTAL_REQ:

   case ISNS_REG_DEV_ATTR_RES:
   case ISNS_DEV_ATTR_QRY_RES:
   case ISNS_DEV_GET_NXT_RES:
   case ISNS_DEREG_DEV_RES:
   case ISNS_SCN_REG_RES:
   case ISNS_SCN_DEREG_RES:
   case ISNS_SCN_EVENT_RES:
   case ISNS_SCN_RES:
   case ISNS_REG_DD_RES:
   case ISNS_DEREG_DD_RES:
   case ISNS_REG_DDS_RES:
   case ISNS_DEREG_DDS_RES:
   case ISNS_ESI_RSP:
   case ISNS_DDS_GET_NXT_MEMBER_RES:
   case ISNS_DD_GET_NXT_ISCSI_MEMBER_RES:
   case ISNS_ENTITY_GET_NXT_PORTAL_RES:
      parse_status = ISNSProfileKeysAttr (msg, attr_index, &num_attrs,
                                          key_index, &num_keys, (ISNS_Attr **)src_attr);
      if(parse_status)
         __LOG_ERROR ("ISNSParseMSG error for msg->hdr.type=0x%x\n",msg->hdr.type);
      break;
   default:
      //if (msg->hdr.type & 0x8000)
      //  return (SUCCESS);
      parse_status = ERROR;
      __LOG_ERROR ("ISNSParseMSG error for msg->hdr.type=0x%x unimplemented message type\n",msg->hdr.type);
      break;
   }

   return parse_status;
}

/********************************************************************
This will verify the len of tags to make sure they are correct.
********************************************************************/
int 
ISNSVerifyTag(int srcFlag, uint32_t tag, uint32_t len, void *ptr)
{
   int rval = TRUE;
   char src[4];

   if (srcFlag)
      strcpy(src,"SRC");
   else
      strcpy(src,"   ");

   switch ( tag )
   {
      case ISNS_DELIMITER:
         if (len != ISNS_DELIMITER_SIZE)
         {
          rval = FALSE;
          __LOG_WARNING ("%sTag:%i (%s) len=%i",src,tag,isnsTagText(tag),len);
         }
          __DEBUG (isns_parse_debug &1,%sTag:%i (%s) len=%i,src,tag,isnsTagText(tag),len);
         break;
      /* Variable Length */
      case ISNS_ENTITY_ID:
      case ISNS_ISCSI_NODE_ID:
      case ISNS_DDS_SYM_NAME:
      case ISNS_ISCSI_ALIAS:
      case ISNS_NODE_SYM_NAME:
      case ISNS_DD_SYM_NAME:
      case ISNS_PORT_SYM_NAME:
      case ISNS_PORTAL_SYM_NAME:
      case ISNS_DD_ISCSI_MEMBER:
      case ISNS_PORTAL_GROUP_ISCSI_NAME:
         if ( len > DD_SYM_NAME_SIZE)
         {
            rval = FALSE;
         }
         if ( len == 0)
            __DEBUG (isns_parse_debug &1,%sTag:%i (%s),src,tag,isnsTagText(tag));
         else
            __DEBUG (isns_parse_debug &1,%sTag:%i (%s) %s,src,tag,isnsTagText(tag),(char *)ptr);
         break;
       
      case ISNS_MGMT_IP:
      case ISNS_PORTAL_IP:
      case ISNS_PORTAL_GROUP_IP:
         if (len != ISNS_IP_SIZE && len != 0)
         {
            rval = FALSE;
            __LOG_WARNING ("%sTag:%i (%s) %s",src,tag,isnsTagText(tag),
        inet_ntoa(*(struct in_addr *)(ptr+12)) );
         }
         __DEBUG (isns_parse_debug &1,%sTag:%i (%s) %s,src,tag,isnsTagText(tag),
        inet_ntoa(*(struct in_addr *)(ptr+12)) );
         break;
      case ISNS_TIMESTAMP:
         if (len != ISNS_TIMESTAMP_SIZE && len != 0 )
         {
            rval = FALSE;
            __LOG_WARNING ("%sTag:%i (%s)",src,tag,isnsTagText(tag));
         }
         __DEBUG (isns_parse_debug &1,%sTag:%i (%s),src,tag,isnsTagText(tag));
         break;

      case ISNS_ENTITY_TYPE:
      case ISNS_PROT_VER:
      case ISNS_ENTITY_PERIOD:
      case ISNS_ENTITY_IDX:
      case ISNS_ENTITY_CERT:
      case ISNS_PORTAL_PORT:
      case ISNS_ESI_INTERVAL:
      case ISNS_ESI_PORT:
      case ISNS_SCN_PORT:
      case ISNS_PORTAL_IDX:
      case ISNS_ISCSI_TYPE:
      case ISNS_ISCSI_SCN_BITMAP:
      case ISNS_ISCSI_IDX:
      case ISNS_PORT_TYPE:
      case ISNS_DDS_ID:
      case ISNS_DDS_STATUS:
      case ISNS_DD_ID:
      case ISNS_DD_ISCSI_MEMBER_IDX:
      case ISNS_DD_ACTIVE:
      case ISNS_NODE_ACTIVE:
      case ISNS_PORT_ID:
      case ISNS_IFCP_SCN_BITMAP:
      case ISNS_DD_FEATURE_BITMAP:
      case ISNS_PORTAL_GROUP:
      case ISNS_PORTAL_GROUP_TAG:
      case ISNS_PORTAL_SECURITY_BITMAP:
      case ISNS_PORTAL_GROUP_PORT:
         if (len != 4 && len != 0)
         {
            rval = FALSE;
            if (len == 0)
               __LOG_WARNING ("%sTag:%i (%s)",src,tag,isnsTagText(tag));
            else
               __LOG_WARNING ("%sTag:%i (%s) %i",src,tag,isnsTagText(tag),*(uint32_t *)ptr);
         }
         else if( tag == ISNS_DD_ID && len == 4 && *(uint32_t *)ptr == 0 )
            rval = FALSE;

         if (len == 0)
           __DEBUG (isns_parse_debug &1,%sTag:%i (%s),src,tag,isnsTagText(tag));
         else
           __DEBUG (isns_parse_debug &1,%sTag:%i (%s) %i,src,tag,isnsTagText(tag),*(uint32_t *)ptr);
         break;


      case ISNS_PORT_NAME:
      case ISNS_NODE_NAME:
      case ISNS_DD_IFCP_MEMBER:
         if (len != 8 && len != 0)
         {
            rval = FALSE;
            if (len == 0)
              __LOG_WARNING ("%sTag:%i (%s)",src,tag,isnsTagText(tag));
            else
              __LOG_WARNING ("%sTag:%i (%s) %s",src,tag,isnsTagText(tag),(char *)ptr);
         }
         if (len == 0)
           __DEBUG (isns_parse_debug &1,%sTag:%i (%s),src,tag,isnsTagText(tag));
         else
           __DEBUG (isns_parse_debug &1,%sTag:%i (%s) %s,src,tag,isnsTagText(tag),(char *)ptr);
         break;

      case ISNS_FABRIC_PORT_NAME:
      case ISNS_FC_HARD_ADDR:
      case ISNS_FC_PORT_IP:
      case ISNS_FC_COS:
      case ISNS_FC4_TYPE:
      case ISNS_FC4_DESC:
      case ISNS_FC4_FEATURE:
      case ISNS_IFCP_NODE_CERT:
      case iSNS_FC4_TYPE_QUERY_KEY:

      case ISNS_FC_NODE_IP:
      case ISNS_FC_NODE_IPA:
      case ISNS_COMPANY_OUI:
      case ISNS_SCN_CALLBACK:
      case ISNS_PORTAL_CERT:
      case ISNS_ISCSI_CERT:
      case ISNS_FC_NODE_CERT:
         if (len == 0)
           __DEBUG (isns_parse_debug &1,%sTag:%i (%s),src,tag,isnsTagText(tag));
         else
           __DEBUG (isns_parse_debug &1,%sTag:%i (%s) %i,src,tag,isnsTagText(tag),*(uint32_t *)ptr);
    break;

      default:
           __DEBUG (isns_parse_debug &1,(Tag not Defined:%i),tag);
         break;
   }


   if (!rval)
      __LOG_WARNING ("VerifyTag rc:%i",rval);

   return (rval);
}

static int
ISNSProfileKeysAttr (ISNS_Msg * msg,
                     ISNS_Attr **attr_index, int *num_attrs,
                     ISNS_Attr **key_index, int *num_keys, ISNS_Attr **src_attr)
{
   int ii, jj;
   int len;
   int sns_attr_len;
   ISNS_Attr *attr;
   ISNS_Attr *ptr;
   int offset;

   /* 
    * verify input parameters 
    */
   if ((msg == NULL) || (msg->hdr.msg_len > sizeof (ISNS_Msg)))
   {
      __LOG_ERROR ("Invalid msg header length: %i\n",msg->hdr.msg_len);
      return (ERROR);
   }

   __DEBUG (isns_parse_debug &1,%s:0x%x flags:%x msg_len:%i, FuncIDText(msg->hdr.type), msg->hdr.type, msg->hdr.flags, msg->hdr.msg_len);

   /* Store Keys */
   if (msg->hdr.type & 0x8000)  // response messages with status code 
   {
      __DEBUG (isns_parse_debug &1,"response status code:0x%x",msg->payload);
      offset = 4;
   }
   else
      offset = 0;

   ptr = attr = (ISNS_Attr *)((char *)&msg->payload+offset);

   if (src_attr != NULL)
   {
      if (!ISNSVerifyTag(TRUE,ptr->tag, ptr->len, (void *)&ptr->val))
         return (ERROR);

      /* Store the src attr */
      *src_attr = attr;

      /* Skip the src attr */
      ptr = (struct ISNS_attr *)((char *) attr + attr->len + ISNS_SIZEOF_TAG);
      offset = attr->len + ISNS_SIZEOF_TAG;
   }

   jj = 0;
   while ((char *)ptr < (char *) &msg->payload + msg->hdr.msg_len)
   {
      if (!ISNSVerifyTag(FALSE,ptr->tag, ptr->len, (void *)&ptr->val))
         return (ERROR);

      if (ptr->tag == ISNS_DELIMITER)
      {
         ptr = (struct ISNS_attr *)((char *) ptr + ISNS_SIZEOF_TAG);
         offset += ISNS_SIZEOF_TAG;
         break;
      }
      key_index[jj] = ptr;
      jj++;
      offset += ISNS_SIZEOF_TAG + ptr->len;
      ptr = (struct ISNS_attr *)((char *) ptr + ISNS_SIZEOF_TAG + ptr->len);
   }
   *num_keys = jj;

   /* Skip to the Operational Attributes */
   ii = 0;
   len = offset;
   sns_attr_len = msg->hdr.msg_len;
   attr = (ISNS_Attr *) ptr;

   /*
    * Go through the message's attributes and parse them
    */
   jj = 0;
   while (len < sns_attr_len && ii < SNS_MAX_ATTRS)
   {
      /*
       * Find a match with any required or optional attributes
       * for that op_code.
       */
      if (!ISNSVerifyTag(FALSE,attr->tag, attr->len, (void *)&attr->val))
         return (ERROR);

      attr_index[ii] =  attr;
      ii++;

      len += ISNS_SIZEOF_TAG + attr->len;
      attr = (ISNS_Attr *) ((char *) attr + ISNS_SIZEOF_TAG + attr->len);
   }

   *num_attrs = ii;
   return SUCCESS;
}

int
SNSConvertPayloadNTOH (ISNS_Msg_Descp * msg)
{
   void *ptr;

   ptr = msg->msg.payload.payload_data;

   if (msg->cb.sender != ISNS_REMOTE)
   {
      return (SUCCESS);
   }

   if (msg->msg.hdr.msg_len == 0)
   {
      return (SUCCESS);
   }

   if (msg->msg.hdr.type == ISNS_HEART_BEAT)
   {
      msg->msg.payload.heartbeat.tcp_port =
         ntohs (msg->msg.payload.heartbeat.tcp_port);
      msg->msg.payload.heartbeat.udp_port =
         ntohs (msg->msg.payload.heartbeat.udp_port);
      msg->msg.payload.heartbeat.counter =
         ntohl (msg->msg.payload.heartbeat.counter);
      msg->msg.payload.heartbeat.interval =
         ntohl (msg->msg.payload.heartbeat.interval);
      return (SUCCESS);
   }

   switch (msg->msg.hdr.type)
   {
   case ISNS_SCN_RES:
   case ISNS_ESI_RSP:
      msg->msg.payload.error_code = ntohl (msg->msg.payload.error_code);
      ptr = (char *) ptr + 4;
   default:
      while ( ptr <
              (void *)(msg->msg.payload.payload_data +
              msg->msg.hdr.msg_len))
      {
         ISNS_Attr *pattr = (ISNS_Attr *)ptr;

         if ((pattr->tag < 5000) && (pattr->tag != 0))
         {
            __LOG_WARNING ("**** Warning: Tag is incorrect tag=0x%x (%u).\n",
                    pattr->tag, pattr->tag);

         }
         else
         {
            pattr->tag = ntohl (pattr->tag);
         }

         //__DEBUG (isns_parse_debug &1,"Processing msg type=0x%x, tag=%i",msg->msg.hdr.type,pattr->tag);

         /* length */
         ptr = (void *)(ptr + 4);
         if ((pattr->len < 5000) && (pattr->len != 0))
         {
            __LOG_WARNING ("Warning: Length is incorrect tag=0x%x, len=0x%x.", pattr->tag, pattr->len);

         }
         else
         {
            pattr->len = ntohl (pattr->len);
         }
         ptr = (void *)(ptr + 4);
         switch (pattr->tag)
         {
            /* These we convert. */
         case ISNS_ENTITY_IDX:
         case ISNS_ISCSI_IDX:
         case ISNS_PORTAL_IDX:
         case ISNS_DD_ISCSI_MEMBER_IDX:
         case ISNS_DDS_ID:
         case ISNS_DD_ID:
         case ISNS_ENTITY_TYPE:
         case ISNS_ESI_INTERVAL:
         case ISNS_ESI_PORT:
         case ISNS_COMPANY_OUI:
         case ISNS_PORTAL_PORT:
         case ISNS_PORT_ID:
         case ISNS_PORT_TYPE:
         case ISNS_DDS_STATUS:
         case ISNS_ISCSI_SCN_BITMAP:
         case ISNS_IFCP_SCN_BITMAP:
         case ISNS_ISCSI_TYPE:
         case ISNS_ENTITY_PERIOD:
         case ISNS_DD_FEATURE_BITMAP:
         case ISNS_PORTAL_SECURITY_BITMAP:
         case ISNS_PORTAL_GROUP_PORT:
         case ISNS_PORTAL_GROUP_TAG:
         case ISNS_PORTAL_GROUP:
            if (pattr->len != 0)
            {
               *(uint32_t *) & pattr->val =
                  ntohl (*(uint32_t *) & pattr->val);
            }
            ptr = (uint8_t *) ptr + pattr->len;
            break;
         case ISNS_PROT_VER:
            if (pattr->len != 0)
            {
               pattr->val.prot_ver.ver_max = htons (pattr->val.prot_ver.ver_max);
               pattr->val.prot_ver.ver_min = htons (pattr->val.prot_ver.ver_min);
            }
            ptr = (uint8_t *) ptr + pattr->len;
            break;
            /* These we leave alone. */
         case ISNS_PORT_NAME:
         case ISNS_DDS_SYM_NAME:
         case ISNS_DD_SYM_NAME:
         case ISNS_DD_ISCSI_MEMBER:
         case ISNS_DD_IFCP_MEMBER:
         case ISNS_FC4_TYPE:
         case ISNS_PORTAL_IP:
         case ISNS_FC_NODE_IP:
         case ISNS_FC_NODE_IPA:
         case ISNS_NODE_SYM_NAME:
         case ISNS_ENTITY_ID:
         case ISNS_DELIMITER:
         case ISNS_FC4_FEATURE:
         case ISNS_NODE_NAME:
         case ISNS_PORTAL_SYM_NAME:
         case ISNS_ISCSI_NODE_ID:
         case ISNS_FABRIC_PORT_NAME:
         case ISNS_FC4_DESC:
         case ISNS_FC_NODE_CERT:
         case ISNS_MGMT_IP:
         case ISNS_TIMESTAMP:
         case ISNS_FC_HARD_ADDR:
         case ISNS_FC_PORT_IP:
         case ISNS_FC_COS:
         case ISNS_PORT_SYM_NAME:
         case ISNS_ENTITY_CERT:
         case ISNS_PORTAL_CERT:
         case ISNS_IFCP_NODE_CERT:
         case ISNS_ISCSI_CERT:
         case ISNS_ISCSI_ALIAS:
         case ISNS_SCN_PORT:
         case ISNS_PORTAL_GROUP_ISCSI_NAME:
         case ISNS_PORTAL_GROUP_IP:
            ptr = (uint8_t *) ptr + pattr->len;
            break;

         default:
            __LOG_ERROR ("Warning: Unknown tag.  tag=0x%x.",pattr->tag);
            ptr = (uint8_t *) ptr + msg->msg.hdr.msg_len;
            return (ERROR);
            break;
         }                      /* End switch */
      }
      break;
   }
   return (SUCCESS);
}

void
SNSConvertPayloadHTON (ISNS_Msg * msg)
{
   int len;
   void *ptr = msg->payload.payload_data;

   if (msg->hdr.type == ISNS_HEART_BEAT)
   {
      msg->payload.heartbeat.tcp_port =
         ntohs (msg->payload.heartbeat.tcp_port);
      msg->payload.heartbeat.udp_port =
         ntohs (msg->payload.heartbeat.udp_port);
      msg->payload.heartbeat.interval =
         htonl (msg->payload.heartbeat.interval);
      msg->payload.heartbeat.counter = htonl (msg->payload.heartbeat.counter);
      return;
   }

   /* Check to see if this is a response */
   if (msg->hdr.type & 0x8000)
   {
      /* Convert the Error CODE */
      *(uint32_t *) ptr = htonl (*(uint32_t *) ptr);
      ptr = (char *) ptr + 4;
   }

   while ( ptr <
          (void *)(msg->payload.payload_data + msg->hdr.msg_len))
   {
      ISNS_Attr *pattr = (ISNS_Attr *)ptr;
      int tag;

      if (pattr->tag > 5000)
      {
         __LOG_ERROR ("Warning: Tag is incorrect.  tag=0x%x",pattr->tag);
         tag = ntohl (pattr->tag);
      }
      else
      {
         tag = pattr->tag;
         pattr->tag = ntohl (pattr->tag);
      }

      /* length */
      ptr = (void *)(ptr + 4);
      if (pattr->len > 5000)
      {
         __LOG_ERROR("Warning: Length is incorrect. tag=0x%x, len=0x%x",pattr->tag,pattr->len);
         len = htonl (pattr->len);
      }
      else
      {
         len = pattr->len;
         pattr->len = ntohl (pattr->len);
      }
      ptr = (void *)(ptr + 4);
      switch (tag)
      {
         /* These we convert. */
      case ISNS_ENTITY_IDX:
      case ISNS_ISCSI_IDX:
      case ISNS_PORTAL_IDX:
      case ISNS_DD_ISCSI_MEMBER_IDX:
      case ISNS_DDS_ID:
      case ISNS_DD_ID:
      case ISNS_ENTITY_TYPE:
      case ISNS_ESI_PORT:
      case ISNS_ESI_INTERVAL:
      case ISNS_COMPANY_OUI:
      case ISNS_PORTAL_PORT:
      case ISNS_PORTAL_GROUP_PORT:
      case ISNS_PORTAL_GROUP_TAG:
      case ISNS_PORT_TYPE:
      case ISNS_DDS_STATUS:
      case ISNS_ISCSI_SCN_BITMAP:
      case ISNS_IFCP_SCN_BITMAP:
      case ISNS_ISCSI_TYPE:
      case ISNS_ENTITY_PERIOD:
      case ISNS_DD_FEATURE_BITMAP:
         *(uint32_t *) & pattr->val = ntohl (*(uint32_t *) & pattr->val);
         ptr = (uint8_t *) ptr + len;
         break;
      case ISNS_PROT_VER:
         pattr->val.prot_ver.ver_min = htons (pattr->val.prot_ver.ver_min);
         pattr->val.prot_ver.ver_max = htons (pattr->val.prot_ver.ver_max);
         ptr = (uint8_t *) ptr + len;
         break;
         /* These we leave alone. */
      case ISNS_PORT_NAME:
      case ISNS_DDS_SYM_NAME:
      case ISNS_DD_SYM_NAME:
      case ISNS_DD_ISCSI_MEMBER:
      case ISNS_DD_IFCP_MEMBER:
      case ISNS_FC4_TYPE:
      case ISNS_PORTAL_IP:
      case ISNS_PORTAL_GROUP_IP:
      case ISNS_PORT_ID:
      case ISNS_FC_NODE_IP:
      case ISNS_FC_NODE_IPA:
      case ISNS_NODE_SYM_NAME:
      case ISNS_ENTITY_ID:
      case ISNS_DELIMITER:
      case ISNS_FC4_FEATURE:
      case ISNS_NODE_NAME:
      case ISNS_PORTAL_SYM_NAME:
      case ISNS_ISCSI_NODE_ID:
      case ISNS_FABRIC_PORT_NAME:
      case ISNS_FC4_DESC:
      case ISNS_FC_NODE_CERT:
      case ISNS_MGMT_IP:
      case ISNS_TIMESTAMP:
      case ISNS_FC_HARD_ADDR:
      case ISNS_FC_PORT_IP:
      case ISNS_FC_COS:
      case ISNS_PORT_SYM_NAME:
      case ISNS_ENTITY_CERT:
      case ISNS_PORTAL_CERT:
      case ISNS_IFCP_NODE_CERT:
      case ISNS_ISCSI_CERT:
      case ISNS_ISCSI_ALIAS:
      case ISNS_PORTAL_GROUP_ISCSI_NAME:
         ptr = (uint8_t *) ptr + len;
         break;

      default:
         __LOG_WARNING ("Warning: Unknown tag. tag=0x%x",pattr->tag);
         ptr = (uint8_t *) ptr + msg->hdr.msg_len;
         break;
      }                         /* End switch */
   }
   return;
}

int
SNSParseTransOpcode (int op_code)
{
   return (-1);
}

/*********************************************************************
Finds a tag.
*********************************************************************/
int
ISNSFindTag (int startIndex, int key, ISNS_Attr **attr_indx)
{
   int ii;
   ISNS_Attr *attr;

   for (ii = startIndex; ii < SNS_MAX_ATTRS && attr_indx[ii]; ii++)
   {
      attr = (ISNS_Attr *) attr_indx[ii];
      if (attr->tag == key)
      {
         return (ii);
      }
   }
   return (-1);
}

