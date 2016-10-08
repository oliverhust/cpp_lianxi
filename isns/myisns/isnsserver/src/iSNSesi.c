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
 * This file contains source code for managing the ESI - Entity State Inquiry.
 *
 */
#include "iSNS.h"
#include "iSNSmsg.h"
#include "iSNSList.h"
#include "iSNSdb.h"
#include "iSNSesi.h"
#include "iSNSresponse.h"
#include "iSNSreg.h"
#include "iSNStypes.h"
#include "iSNScomm.h"
#include "iSNSbuffer.h"
#include "iSNStbl.h"
#include "iSNSparse.h"
#include "iSNSdebug.h"
#include "iSNSobjects.h"


extern int isns_esi_debug;
extern int sns_esi_interval;
extern int esi_sent_limit;

/*
 * static common variables for gdbm fetches
 * entry is for common registrations and queries
 * scn_entry is for state change notifications
 * opn_entry is for access control
 */
static SOIP_DB_Entry entry;     /* top level fetches ONLY */
static SOIP_DB_Entry entry2;    /* secondary fetches */

ISNS_Msg_Descp    *p_rspMd;

/*********************************************************************
Sends out one or more ESI(s).
*********************************************************************/
int
ISNS_SendESI (void)
{
   static int        esi_xid = 1;
   int               rval;
   SOIP_Portal       *p_portal;
   SOIP_Portal_Key   db_portal;
   ISNS_Msg_Descp    *p_md;
   time_t            mytime;
   ISNS_DBKey          read_key = { 0 };

   __DEBUG (isns_esi_debug &1, ISNS_SendESI);

   p_md = p_rspMd;
   memset( p_md, 0, sizeof( ISNS_Msg_Descp ) );

   if (isns_role != ISNS_PRIMARY_SERVER)
   {
      return ( ISNS_NO_ERR );
   }

   __DEBUG (isns_esi_debug &1, process each portal record);

   read_key.tag = PORTAL_ID_KEY;

   while ( SNSdbGetNextOfKey (&read_key) == SUCCESS )
   {
      __DEBUG (isns_esi_debug &1, Next Portal Record found);
      __ISNS_COPY(&db_portal, sizeof(SOIP_Portal_Key), &read_key.val.portal_key, sizeof(SOIP_Portal_Key));
      rval = read_PortalObject(&db_portal, &p_portal, &entry2);
      if (rval != SUCCESS)
         return rval;

      /* check to see if an interval is set > 0 */
      if ( p_portal->esi_interval == 0 )
      {
         __DEBUG (isns_esi_debug &1, esi_interval not set);
         continue;
      }

      /* check to see if timer interval expired */      
      time ( &mytime );
      if (p_portal->esi_timer > 0 && (p_portal->esi_timer + p_portal->esi_interval) > mytime)
      {
         __DEBUG (isns_esi_debug &1, esi_timer plus interval not reached yet);
         __DEBUG (isns_esi_debug &1, esi_timer:%ld,p_portal->esi_timer);
         __DEBUG (isns_esi_debug &1, esi_interval:%i,p_portal->esi_interval);
         __DEBUG (isns_esi_debug &1, mytime:%ld,mytime);
         continue;
      }

      p_portal->esi_timer = mytime;
      p_portal->esiSent++;
      rval = write_PortalObject(&db_portal, entry2);

      __DEBUG (isns_esi_debug &1, esi_timer Timer Expired esiSend:%i,p_portal->esiSent);

      if ( p_portal->esiSent < esi_sent_limit )
      {
         __DEBUG (isns_esi_debug & 2, (Sending ESI to Entity %s), p_portal->entity_id.id);

         /* Send a msg to the client & update the counter */
         memset (&p_md->msg, 0, sizeof (ISNS_Msg));
         p_md->msg.hdr.type = ISNS_ESI;

         ISNSAppendKey (&p_md->msg, ISNS_ENTITY_ID,
                     PAD4 (strlen (p_portal->entity_id.id)), p_portal->entity_id.id, 0);

         ISNSAppendKey ( &p_md->msg, ISNS_PORTAL_IP, ISNS_IP_SIZE,
                         p_portal->ip_addr.v, 0);

         ISNSAppendKey ( &p_md->msg, ISNS_PORTAL_PORT, ISNS_PORTAL_PORT_SIZE,
                         NULL, p_portal->ip_port);

         memcpy ( &p_md->cb.sock.addr.sin_addr.s_addr, p_portal->ip_addr.v + 12,
                  sizeof (p_md->cb.sock.addr.sin_addr.s_addr));

         /* Determine how to send */
         if ( p_portal->esi_port )
         {
            p_md->msg.hdr.xid = esi_xid++;
            p_md->cb.sock.socketType = ISNS_SOCKET_TYPE_UDP;
            p_md->cb.sock.addr.sin_port = htons ((short)(p_portal->esi_port & 0xFFFF));
         }
         else
         {
            p_md->msg.hdr.xid = esi_xid++;
            p_md->cb.sock.socketType = ISNS_SOCKET_TYPE_TCP;
            p_md->cb.sock.sockfd = p_portal->esi_sockfd;
         }

         __DEBUG (isns_esi_debug & 2, (Call ISNSSendMsg to Send ESI to Entity %s), p_portal->entity_id.id);
         rval = ISNSSendMsg ( p_md );
      }

      else
      {
         __DEBUG (isns_esi_debug &1,esi timed out - remove portal record);
         __DEBUG (isns_esi_debug &1,portal ip_addr:%s,inet_ntoa(*(struct in_addr *)(p_portal->ip_addr.v+12)) );
         __DEBUG (isns_esi_debug &1,port:%i,p_portal->ip_port);

         /* remove portal */
         memset (&db_portal, 0, sizeof (db_portal));
         memcpy (db_portal.ip_addr.v, p_portal->ip_addr.v, ISNS_IP_SIZE);
         db_portal.ip_port=p_portal->ip_port;

         SNSRemovePortalEntry (&db_portal, NULL, NULL);
      }
   }

   return ( ISNS_NO_ERR );
}

/*********************************************************************
Processes an ESI response.
*********************************************************************/
int
ISNS_ProcessESI ( ISNS_Msg_Descp *p_md )
{
   ISNS_Key             *p_attr;
   SOIP_Entity          *p_entity;
   SOIP_Portal          *p_portal;
   time_t               mytime;
   SOIP_Portal_Key      db_portal;
   int                  i;
   int                  len;
   ISNS_Attr            *attr_indx[SNS_MAX_ATTRS];
   ISNS_Attr            *key_indx[SNS_MAX_ATTRS];
   int                  rval;
   SOIP_Entity_Id       db_entity_id;

   __DEBUG (isns_esi_debug &1, ISNS_ProcessESI);

   ISNSParseMsg( &p_md->msg, attr_indx, key_indx, NULL );
   i = ISNSFindTag (0, ISNS_ENTITY_ID, key_indx);
   if (i == -1)
   {
      __DEBUG (isns_esi_debug & 1, (Entity ID tag not found in msg));
      return ( ERROR );
   }
   p_attr = (ISNS_Key *)(key_indx[ i ]);

   memset(db_entity_id.id,0,sizeof(SOIP_Entity_Id));
   strncpy(db_entity_id.id,p_attr->val.entity_id.id,sizeof(SOIP_Entity_Id));

   rval = read_EntityObject(db_entity_id.id, &p_entity, &entry);
   if (rval != SUCCESS)
      return (ERROR);

   /* Update the time stamp */
   len = sizeof (mytime);
   time ( &mytime );
   p_entity->timestamp.t_pad = 0;
   p_entity->timestamp.t_time = mytime;
   __DEBUG (isns_esi_debug &1,(Time in seconds since UTC 1/1/70:  %ld),mytime);
   __DEBUG (isns_esi_debug &1,(UNIX time and date: %s),ctime(&mytime) );

   /* Store Entry */
   write_EntityObject(db_entity_id.id, entry);

   memset(&db_portal,0,sizeof(SOIP_Portal_Key));

   /* Fetch the portal IP */
   i = ISNSFindTag (0, ISNS_PORTAL_IP, key_indx);
   if ( i == -1 )
   {
      __DEBUG (isns_esi_debug & 1, (Portal IP tag not found in msg));
      return ( ERROR );
   }
   p_attr = (ISNS_Key *)(key_indx[i]);
   memcpy(db_portal.ip_addr.v, p_attr->val.ip_addr.v, ISNS_IP_SIZE);

   i = ISNSFindTag (0, ISNS_PORTAL_PORT, key_indx);
   if (i == -1)
   {
      __DEBUG (isns_esi_debug & 1, (Portal PORT tag not found in msg));
      return ( ERROR );
   }
   p_attr = (ISNS_Key *)(key_indx[i]);
   db_portal.ip_port=p_attr->val.ip_port;

   rval = read_PortalObject(&db_portal, &p_portal, &entry);
   if (rval != SUCCESS)
    return rval;

   p_portal->esiSent = 0;

   /* Store Portal */
   rval = write_PortalObject(&db_portal, entry);

   return ( ISNS_NO_ERR );
}


