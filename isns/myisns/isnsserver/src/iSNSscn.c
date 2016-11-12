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
 * This file contains source code for managing the SCN - state change notification.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/basetype.h>
#include <sys/error.h>
#include <sys/assert.h>
#include <sys/list.h>
#include <sys/epoll.h>
#include <sys/in.h>

#include "../../include/iscsi_com.h"
#include "../../include/iscsi_event.h"
#include "../../include/iscsi_util.h"

#include "../../include/iscsi_basetype.h"
#include "../../include/iscsi_packet.h"
#include "../../include/iscsi_main.h"

#include "iSNStypes.h"
#include "iSNS.h"
#include "iSNSdb.h"
#include "iSNSobjects.h"
#include "iSNSmsg.h"
#include "iSNScomm.h"
#include "iSNSbuffer.h"
#include "iSNStbl.h"
#include "iSNSList.h"
#include "iSNSparse.h"
#include "iSNSquery.h"
#include "iSNSresponse.h"
#include "iSNSreg.h"
#include "iSNSdebug.h"

/* Global */
extern int isns_scn_msg_filter;
extern int isns_rsync_debug;
extern ISNS_LIST scn_list;
extern ISNS_LIST scn_callback_list;
extern ISNS_Msg *p_scn_msg_buffer;
extern ISNS_Msg *p_scn_all_msg_buffer;
extern ISNS_Msg_Descp *p_scn_md;
extern ISNS_Msg_Descp *p_rspMd;
extern dbStats iSNS_stats;
extern int sns_scn_debug;
extern int sns_cb_debug;
extern int sns_cb_msg_filter;
extern int sns_scn_msg_filter;
extern int sns_rsync_debug;
extern int SNSGetMgmtAddr (void);
extern int sns_esi_interval;
extern uint32_t *dlist_src; /*[MAX_DD_PER_LIST];*/
extern uint32_t *dlist_node; /*[MAX_DD_PER_LIST];*/
extern SOIP_Dd_Member *ddmem; /* [100]; */

/*
 * static common variables for gdbm fetches
 * entry is for common registrations and queries
 * scn_entry is for state change notifications
 * opn_entry is for access control
 */
static SOIP_DB_Entry scn_entry;
static SOIP_DB_Entry scn_entry2;
static SOIP_DB_Entry scn_entry3;

static ISNS_ATTR_VALS_CB *
ISNSAttrGetList (ISNS_Msg_Descp *p_md);

static void*
SNSAttrGetAttrValLocation (ISNS_Attr *sns_attr);

/*********************************************************************
*********************************************************************/
static int
send_iscsi_scn_md(char *p_node_name, ISNS_Msg_Descp *p_md)
{
   int               foundSCNPortal;
   SOIP_DB_Entry     lentry;
   SOIP_DB_Entry     lentry2;
   SOIP_Iscsi        *p_node;
   SOIP_Entity       *p_entity;
   SOIP_Portal       *p_portal = NULL;
   ISNS_LIST_NODE    *lnode;
   ISNS_ATTR_VALS_CB *attr_vals_cb;
   int               rval;

   if ( p_node_name == NULL)
      return (ERROR);

   rval = read_ISCSIObject(p_node_name, &p_node, &lentry);
   if (rval != SUCCESS)
     return rval;

   if (!p_node->scn_bitmap)
      return (SUCCESS);

   /* Determine how to send */
   if ( p_node->scn_callback )
   {
      /* exec callback */
      /* notify local devices of SCN */
      attr_vals_cb = ISNSAttrGetList(p_md);
      if (attr_vals_cb != NULL)
      {
             p_node->scn_callback(attr_vals_cb);
      }

      ISNSFreeBuffer ((char *)attr_vals_cb->attr_val_sptr);
      ISNSFreeBuffer ((char *)attr_vals_cb);
   }
   else if ( p_node->scn_sockfd )
   {
      /* TCP */
      p_md->cb.sock.socketType = ISNS_SOCKET_TYPE_TCP;
      p_md->cb.sock.sockfd = p_node->scn_sockfd;

      if ( SUCCESS != ISNSSendMsg (p_md) )
      {
         __DEBUG(sns_scn_debug & 1, (Warning: Failure sending SCN));
         p_node->scn_bitmap = 0;

         write_ISCSIObject(p_node_name, lentry);
      }
   }
   else
   {
      /* UDP */
      char buffer[256]={0};
      /* Pick a portal, send out a SCN */

      /* Fetch entity */
      memset (buffer, 0, sizeof (buffer));
      strncpy (buffer, p_node->entity_id.id, sizeof(buffer));
      read_EntityObject (buffer, &p_entity, &lentry);

      /* Fetch portal */
      foundSCNPortal = FALSE;
      lnode = NULL;
      while ((lnode=GetNextNode(&p_entity->iportal_list, lnode)))
      {
         rval = read_PortalObject(GetNodeData(lnode), &p_portal, &lentry2);
         if (rval != SUCCESS)
             return rval;

         /* Get first ESI/SCN Portal */
         if (p_portal->esi_port != 0)
         {
            foundSCNPortal = TRUE;
            break;
         }
      }

      /* Send out SCN */
      if (foundSCNPortal)
      {
         p_md->cb.sock.socketType = ISNS_SOCKET_TYPE_UDP;
         p_md->cb.sock.addr.sin_family = AF_INET;
         p_md->cb.sock.len = sizeof (p_md->cb.sock.addr);

         memcpy (&p_md->cb.sock.addr.sin_addr.s_addr,
                 p_portal->ip_addr.v + 12,
                 sizeof (p_md->cb.sock.addr.sin_addr.s_addr));

         p_md->cb.sock.addr.sin_port =
            htons ((short)(p_portal->esi_port & 0xFFFF));

         ISNSSendMsg (p_md);

      }
      else
      {
         __DEBUG (sns_scn_debug & 1, (No valid Portal to send SCN));
         return (ERROR);
      }

   }

   return (SUCCESS);
}

/********************************************************************
Sends a SCN to all the DD members of the node that's passed in.
********************************************************************/
int
send_iscsi_scn_to_members ( SOIP_Iscsi * p_node, char *p_src_name,
                            ISNS_Msg *p_scn_msg, uint32_t scn_bitmap )
{
   ISNS_Msg             *p_msg;
   time_t               t;
   int                  rval;
   ISNS_LIST_NODE       *ddnode;
   ISNS_LIST_NODE       *pnode;
   SOIP_Iscsi           *iscsi_node;
   int                  dd_id;
   SOIP_Dd              *p_dd;

   __DEBUG (sns_scn_debug &1, send_iscsi_scn_to_members);

   p_msg=&p_scn_md->msg;
   memset(p_scn_md, 0, sizeof(ISNS_Msg_Descp));

   if ( p_node == NULL )
   {
      /* Fetch the node */
      rval = read_ISCSIObject(p_src_name, &p_node, &scn_entry);
      if (rval != SUCCESS )
        return rval;
   }

   ddnode = NULL;
   while ( (ddnode = GetNextNode(&p_node->dd_id_list, ddnode)) )
   {
      dd_id = *(uint32_t *)GetNodeData (ddnode);

      rval = read_DDObject(dd_id, &p_dd, &scn_entry2);
      if (rval != SUCCESS)
        break;

      if ( !(p_dd->activeFlag) )
      {
          __DEBUG (sns_scn_debug &1, Discovery Domain is not Active:%i,p_dd->id);
          continue;
      }

      /* For each member */
      pnode = NULL;
      while ( (pnode = GetNextNode(&p_dd->member_list, pnode)) )
      {
        SOIP_Dd_Member *p_member;

        p_member=(SOIP_Dd_Member *)GetNodeData(pnode);

        if (p_member->type != ISNS_DD_ISCSI_MEMBER)
         continue;
        if ( !strcmp(p_node->id.v,p_member->node_id) )
        {
          continue;
        }

        /* Fetch Node */
        rval = read_ISCSIObject(p_member->node_id, &iscsi_node, &scn_entry3);
        if (rval != SUCCESS)
           return ERROR;

        if (!(iscsi_node->scn_bitmap & scn_bitmap))
        {
         continue;
        }

        memset(p_scn_md, 0, sizeof(ISNS_Msg_Descp));
        p_msg->hdr.type = ISNS_SCN;

        /* Add Dest Attribute */
        ISNSAppendKey (p_msg, ISNS_ISCSI_NODE_ID,
           PAD4 (strlen (p_member->node_id)), p_member->node_id, 0);

        /* Add Timestamp */
         ISNSAppendKey (p_msg, ISNS_TIMESTAMP, ISNS_TIMESTAMP_SIZE,
                     NULL, time (&t));

        /* Copy the src attribute buffer into the msg */
        memcpy((char *)&p_msg->payload+p_msg->hdr.msg_len,
            (char *)&p_scn_msg->payload, p_scn_msg->hdr.msg_len);

        p_msg->hdr.msg_len+=p_scn_msg->hdr.msg_len;

        /* Send SCN msg */
        send_iscsi_scn_md(p_member->node_id, p_scn_md);
     }
   }

   return ( SUCCESS );
}

/*********************************************************************
Processes a SCN Event request.
*********************************************************************/
int
ISNS_Process_SCN_Event (ISNS_Msg *p_msg)
{
   ISNS_Key        *key;
   ISNS_Attr       *p_attr; /* query attr, response attr */
   ISNS_Attr       *src_attr;
   ISNS_Attr       *attr_indx[SNS_MAX_ATTRS];
   ISNS_Attr       *key_indx[SNS_MAX_ATTRS];


   ISNSParseMsg( p_msg, attr_indx, key_indx, &src_attr);
   key = (ISNS_Key *)(key_indx[0]);
   if (key == NULL)
   {
    __DEBUG (sns_scn_debug & 1, (SCN Rejected:No Key));
      return (ISNS_SCN_EVENT_REJECTED_ERR);
   }

   p_attr = (ISNS_Attr *)(attr_indx[0]);
   if (p_attr == NULL)
   {
    __DEBUG (sns_scn_debug & 1, (SCN Rejected:No Op Attr));
      return (ISNS_SCN_EVENT_REJECTED_ERR);
   }

   switch ( key->tag )
   {
      case ISNS_ISCSI_NODE_ID:
         if ( p_attr->tag != ISNS_ISCSI_SCN_BITMAP )
         {
          __DEBUG (sns_scn_debug & 1, (SCN Rejected:Wrong Op Attr %u),
                     p_attr->tag);
            return ( ISNS_SCN_EVENT_REJECTED_ERR );
         }

         //if (SUCCESS != send_iscsi_scn_to_members (NULL, (char *)&key->val, *(uint32_t *) &p_attr->val),0)
           // return ( ISNS_SCN_EVENT_REJECTED_ERR );
         break;

      case ISNS_PORT_NAME:
         if (p_attr->tag != ISNS_IFCP_SCN_BITMAP)
         {
          __DEBUG (sns_scn_debug & 1, (SCN Rejected:Wrong Op Attr %u),
                     p_attr->tag);
            return ( ISNS_SCN_EVENT_REJECTED_ERR );
         }

         //if (SUCCESS != send_iscsi_scn_to_members (NULL, (char *)&key->val, *(uint32_t *) &p_attr->val),0)
           // return ( ISNS_SCN_EVENT_REJECTED_ERR );

         break;

      default:
       __DEBUG (sns_scn_debug & 1, (SCN Rejected:Invalid Key %u),
                  key->tag);
         return ( ISNS_SCN_EVENT_REJECTED_ERR );
   }

   return ( SUCCESS );
}


/********************************************************************
********************************************************************/
int
ISNS_Process_SCN ( ISNS_Msg_Descp *p_md, ISNS_Msg *p_rsp_msg )
{
    ISNS_LIST_NODE                 *cnode;
    ISNS_Attr                 *p_attr;
    ISNS_ATTR_VALS_CB         *attr_vals_cb;
    SOIP_Scn_Callback_Entry   *p_scn;

    p_attr = (ISNS_Attr *)&p_md->msg.payload;

    /* Go through SCN */
    /* If Dest Matches, exec scn callback */
    cnode = NULL;
    while ((cnode = GetNextNode(&scn_callback_list, cnode)))
    {
        /* For each SCN Entry */
        p_scn = (SOIP_Scn_Callback_Entry *)GetNodeData(cnode);

        /* Does SCN match, dest */
        if (0 != strcmp((char *)&p_attr->val, p_scn->name.v))
            continue;

        /* determine how to send scn */
         if ( p_scn->scn_callback_func )
         {
            /* exec callback */
            /* notify local devices of SCN */
            attr_vals_cb = ISNSAttrGetList(p_md);
            if ( attr_vals_cb != NULL )
            {
                   p_scn->scn_callback_func( attr_vals_cb );
            }

            ISNSFreeBuffer ( (char *)attr_vals_cb->attr_val_sptr );
            ISNSFreeBuffer ( (char *)attr_vals_cb );
         }
         else
         {
             /* ignore the rest */
             __LOG_WARNING ("Not implemented.\n");
         }

         /* Fill in rsp */
         ISNSAppendKey( p_rsp_msg, ISNS_ISCSI_NODE_ID,
             PAD4( strlen( p_scn->name.v ) ), p_scn->name.v, 0 );
         break;
    }
    return (SUCCESS);
}

static ISNS_ATTR_VALS_CB *
ISNSAttrGetList (ISNS_Msg_Descp *p_md)
{
    ISNS_Key       *p_key;
    char          *loc_ptr1;
    uint16_t      sns_attr_len;
    ISNS_ATTR_VALS     *p_aval;
    ISNS_ATTR_VALS_CB  *p_attr_vals_cb;
    void          *val_location;
    int           num_entries;
    int           display_debug;
    uint16_t      len;

    display_debug = 0;
    if ((sns_cb_debug == 3) && (p_md->msg.hdr.type != ISNS_SCN))
       display_debug = (sns_cb_msg_filter == p_md->msg.hdr.type);

    /*
     * Initialize the vars and go to the start
     * of the message..
     */
    p_aval         = NULL;
    p_attr_vals_cb = NULL;
    len            = 0;

    if (p_md->msg.hdr.type == ISNS_SCN)
    {
       /* No error Code */
       p_key   = (ISNS_Key *)&p_md->msg.payload;
       sns_attr_len   = p_md->msg.hdr.msg_len;
    }
    else
    {
       /* Error Code */
       p_key   = (ISNS_Key *)&p_md->msg.payload+ISNS_ERROR_CODE_SIZE;
       sns_attr_len   = p_md->msg.hdr.msg_len-ISNS_ERROR_CODE_SIZE;
    }
    __DEBUG (display_debug, (Attributes length : 0x%x), sns_attr_len);

    /*
     * Go through the message's attributes and parse them into
     * the Attribute list. NOTE: All the pointers are allocated
     * here including the storage for the attribute values.
     */
    num_entries = 0;
    loc_ptr1 = (char *)p_key;
    p_aval   = (void *)ISNSAllocBuffer (sns_attr_len);

    while (((char *)loc_ptr1 - (char *)p_key) < sns_attr_len)
    {
        if (p_aval == NULL)
        {
           __LOG_WARNING("AttrGetList out of aval buffs (size:%i)",
                    sns_attr_len);
           return (NULL);
        }

        /* Set the VALUE location for the attributes */
        val_location = SNSAttrGetAttrValLocation((ISNS_Attr *)loc_ptr1);
        len = ((ISNS_Attr *)loc_ptr1)->len;

        if (val_location == NULL)
        {
            __LOG_WARNING("AttrGetList id not found (tag:%i, msg id:%i, xid:%i)",
                     ((ISNS_Attr *)loc_ptr1)->tag, p_md->msg.hdr.type,
                     p_md->msg.hdr.xid);
            ISNSFreeBuffer((char *)p_aval);
            p_aval = NULL;
            break;
        }

        p_aval[num_entries].attr_id  = ((ISNS_Attr *)loc_ptr1)->tag;
        p_aval[num_entries].attr_len = len;
        p_aval[num_entries].attr_val = val_location;

        /* Word Align values */
        if (len % 4)
           len += 4 - (len % 4);

        /* Debugging */
        if ((sns_scn_debug == 3) && (p_md->msg.hdr.type == ISNS_SCN))
        {
           if (p_aval[num_entries].attr_id == RSCN_EVENT)
              display_debug = (sns_scn_msg_filter ==
                               *(int *)p_aval[num_entries].attr_val);
        }
        __DEBUG (display_debug, (attr id %u, len %d),
                 p_aval[num_entries].attr_id,
                 p_aval[num_entries].attr_len);

        /* End Debugging */
         loc_ptr1  = (char *) loc_ptr1 + ISNS_SIZEOF_TAG + len;
         num_entries++;
    }
    /*
     * Set the attribute value control block.
     */
    if (p_aval != NULL)
    {
        p_attr_vals_cb = (ISNS_ATTR_VALS_CB *)
          ISNSAllocBuffer(sizeof (ISNS_ATTR_VALS_CB));

        if (p_attr_vals_cb == NULL)
        {
           __LOG_WARNING("AttrGetList out of cb buffs (size:%i)",
                    (int)sizeof (ISNS_ATTR_VALS_CB));
        }
        else
        {
           p_attr_vals_cb->num_entries = num_entries;
           p_attr_vals_cb->total_size = sns_attr_len;
           p_attr_vals_cb->attr_val_sptr = p_aval;
        }
    }
    return (p_attr_vals_cb);
}

static void*
SNSAttrGetAttrValLocation (ISNS_Attr *sns_attr)
{
    if (sns_attr->tag < ISNS_END_VALID_TAG)
    {
       return ((void *)&sns_attr->val);
    }

    return (NULL);
}
