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
/* This file contains sample code for iSCSI devices                    */

#include "isns.h"
#include "iscsi.h"
#include "comm.h"
#include "util.h"
#include "parse.h"

static ISNS_CMD cmd; /* Used for msgs */
extern int replaceFlag;

/**********************************************************************
This will register iSCSI Nodes.
**********************************************************************/
void
RegisterISCSI_I (void)
{
   char cmdLine[256];
   char nodeID[256];
   char eid[256];
   char nodeAlias[256];
   time_t t;
   int type;
   int rval;

   printf ("Registering ISCSI Node.\n");
   ISNSCreateHdr (ISNS_REG_DEV_ATTR_REQ, &cmd, sizeof (cmd), replaceFlag?ISNS_FLAG_REPLACE_REG:0);

   /* Get Entity ID */
   memset (eid, 0, sizeof (eid));

   if (-1==GetSrc(&cmd,0))
      return;

   if ( (rval = GetInput(cmdLine, "Entity ID", NULL, sizeof(cmdLine))) )
   {
      strcpy (eid, cmdLine);
      ISNSAppendAttr (&cmd, ISNS_ENTITY_ID, PAD4 (strlen (eid)), eid, 0);

   }

   /* delimiter */
   ISNSAppendAttr (&cmd, 0, 0, NULL, 0);

   if (rval == 0)
   {
      /* Get Version */
      {
         SOIP_Ver ver;
         memset (&ver, 0, sizeof (ver));

         if (0 !=
             GetInput (cmdLine, "Entity Period", NULL, sizeof (cmdLine)))
         {
            ISNSAppendAttr ((struct cmd *)&cmd, ISNS_ENTITY_PERIOD, 4, NULL, atoi(cmdLine));
         }

         if (0 !=
             GetInput (cmdLine, "Protocol Version Max", "(none)",
                       sizeof (cmdLine)))
         {
            ver.max = atoi (cmdLine);
         }

         if (0 !=
             GetInput (cmdLine, "Protocol Version Min", "(none)",
                       sizeof (cmdLine)))
         {
            ver.min = atoi (cmdLine);
         }

         if (ver.max || ver.min)
         {
            ver.max = htons (ver.max);
            ver.min = htons (ver.min);

            ISNSAppendAttr ((struct cmd *)&cmd, ISNS_PROT_VER, 4, (char *)&ver, 0);
         }
      }
   }

   /* Get iSCSI Node ID */
   do
   {
      memset (nodeID, 0, sizeof (nodeID));
      strcpy (nodeID, "isns.iscsi.node.");
      time (&t);
      strncat (nodeID + strlen (nodeID), ctime (&t) + 11, 8);
      if (GetInput(cmdLine, "ID", nodeID, sizeof(cmdLine))!= 0)
      {
         memset (nodeID, 0, sizeof (nodeID));
         strcpy (nodeID, cmdLine);
      }
      ISNSAppendAttr (&cmd, ISNS_ISCSI_NODE_ID, PAD4 (strlen (nodeID)),
                      nodeID, 0);

      /* Get Alias */
      memset (nodeAlias, 0, sizeof (nodeAlias));
      strcpy (nodeAlias, nodeID);

      if (GetInput(cmdLine, "Alias", nodeID, sizeof(cmdLine))!= 0)
      {
         memset (nodeAlias, 0, sizeof (nodeAlias));
         strcpy (nodeAlias, cmdLine);
      }
      ISNSAppendAttr (&cmd, ISNS_ISCSI_ALIAS, PAD4 (strlen (nodeAlias)),
                      nodeAlias, 0);

      /* Get Type */
      type = 1;
      if (GetInput(cmdLine, "Node Type Bitmask", "*1:Target, 2:Initiator, 3:Target/Initiator", sizeof(cmdLine)) != 0)
      {
         type = atoi (cmdLine);
      }

      ISNSAppendAttr (&cmd, ISNS_ISCSI_TYPE, 4, NULL, type);
   }
   while (GetYesNo (cmdLine, "More iSCSI Node?", FALSE, sizeof (cmdLine)));

   ISNSSendCmd (&cmd);
}

/*****************************************************
This will update an existing iSCSI Node.
*****************************************************/
void
UpdateISCSI (void)
{
   char cmdLine[256];
   int type;

   printf ("Updating ISCSI Node.\n");
   ISNSCreateHdr (ISNS_REG_DEV_ATTR_REQ, &cmd, sizeof (cmd),
                  0);

   /* Get iSCSI Node ID */
   if (0 == GetInput (cmdLine, "iSCSI Node", NULL, sizeof (cmdLine)))
   {
      printf ("***ERROR: You must enter an iSCSI Node.\n");
      return;
   }
   ISNSAppendAttr (&cmd, ISNS_ISCSI_NODE_ID, PAD4 (strlen (cmdLine)), cmdLine,
                   0);

   ISNSAppendAttr (&cmd, 0, 0, NULL, 0);

   /* Get Alias */
   if (0 != GetInput (cmdLine, "iSCSI Node Alias", NULL, sizeof (cmdLine)))
   {
      ISNSAppendAttr (&cmd, ISNS_ISCSI_ALIAS, PAD4 (strlen (cmdLine)),
                      cmdLine, 0);
   }

   /* Get Type */
   if (0 != GetInput (cmdLine, "Node Type Bitmask", "1:Target, 2:Initiator, 3:Target/Initiator", sizeof (cmdLine)))
   {
      type = atoi (cmdLine);
      if (type < 1 || type > 3)
      {
         printf ("***ERROR: Type is wrong.");
         exit (-1);
      }

      ISNSAppendAttr (&cmd, ISNS_ISCSI_TYPE, 4, NULL, type);
   }

   ISNSSendCmd (&cmd);
}

/********************************************
This will query for iSCSI Node(s) attributes.
********************************************/
void
GetISCSI (void)
{
   char buffer[MAX_PAYLOAD_SIZE];
   char cmdLine[256];
   int c_size;
   int key_type;
   int ip;
   int port;

   /* Query for all Node */
   printf ("Query for ISCSI Nodes.\n");
   ISNSCreateHdr (ISNS_DEV_ATTR_QRY_REQ, &cmd, sizeof (cmd), 0);

   /* Append Src field*/
   if (-1 == GetSrc (&cmd, 0))
      return;

   /* Keys */
   key_type = 0;
   if (0 !=
       GetInput (cmdLine, "Key type (1:iSCSI, 2:Entity ID 3:Portal IP/Port 4:Node Type Bitmask 5:iSCSI Idx)>",
                 "(none)", sizeof (cmdLine)))
   {
      key_type = atoi (cmdLine);
      if (key_type == 1)
      {
         do
         {
            c_size =
               GetInput (cmdLine, "iSCSI Node ID >", "(none)",
                         sizeof (cmdLine));
            if (c_size != 0)
            {
               ISNSAppendAttr (&cmd, ISNS_ISCSI_NODE_ID,
                               PAD4 (strlen (cmdLine)), cmdLine, 0);
            }
         }
         while (c_size != 0);
      }
      else if (key_type == 2)
      {
         do
         {
            c_size =
               GetInput (cmdLine, "Entity ID >", "(none)", sizeof (cmdLine));
            if (c_size != 0)
            {
               ISNSAppendAttr (&cmd, ISNS_ENTITY_ID, PAD4 (strlen (cmdLine)),
                               cmdLine, 0);
            }
         }
         while (c_size != 0);
      }
      else if (key_type == 3)
      {
         c_size =
            GetInput (cmdLine, "Portal IP >", "(none)", sizeof (cmdLine));
         if (c_size != 0)
         {
            char b[16];
            memset (b, 0, sizeof (b));
            ip = inet_addr (cmdLine);
            memcpy ((char *) b + 12, &ip, sizeof (ip));
            ISNSAppendAttr (&cmd, ISNS_PORTAL_IP, 16, b, 0);
         }

         c_size =
            GetInput (cmdLine, "Portal Port >", "(none)", sizeof (cmdLine));
         if (c_size != 0)
         {
            port = atoi (cmdLine);
            ISNSAppendAttr (&cmd, ISNS_PORTAL_PORT, 4, NULL, port);
         }
      }
      else if (key_type == 4)
      {
         uint32_t iscsi_type=0;

         c_size = GetInput (cmdLine, "Node Type Bitmask", "1:Target, 2:Initiator, 3:Target/Initiator", sizeof (cmdLine));
         if (c_size != 0)
         {
            iscsi_type=atoi(cmdLine);
            ISNSAppendAttr (&cmd, ISNS_ISCSI_TYPE, 4, NULL, iscsi_type);
         }
      }
      else if (key_type == 5)
      {
         uint32_t iscsi_type=0;

         c_size = GetInput (cmdLine, "iSCSI Index", NULL, sizeof (cmdLine));
         if (c_size != 0)
         {
            iscsi_type=atoi(cmdLine);
            ISNSAppendAttr (&cmd, ISNS_ISCSI_IDX, 4, NULL, iscsi_type);
         }
      }
      else
      {
         printf ("***ERROR: Invalid key_type.\n");
         return;
      }
   }
   else
       ISNSAppendAttr (&cmd, ISNS_ISCSI_NODE_ID, 0, NULL, 0);


   /* Delimiter */
   ISNSAppendAttr (&cmd, 0, 0, NULL, 0);

   /* These are the operational attributes */
   /* These will be returned in a response */

   /* Note order does not matter, but to query for 
      an iSCSI device, put an iSCSI attribute as the
      the first op attr */
   ISNSAppendAttr (&cmd, ISNS_ISCSI_NODE_ID, 0, NULL, 0);
   ISNSAppendAttr (&cmd, ISNS_ISCSI_IDX, 0, NULL, 0);
   ISNSAppendAttr (&cmd, ISNS_ISCSI_TYPE, 0, NULL, 0);
   ISNSAppendAttr (&cmd, ISNS_ISCSI_ALIAS, 0, NULL, 0);
   ISNSAppendAttr (&cmd, ISNS_DD_ID, 0, NULL, 0);
   ISNSAppendAttr (&cmd, ISNS_ENTITY_ID, 0, NULL, 0);
   ISNSAppendAttr (&cmd, ISNS_ENTITY_IDX, 0, NULL, 0);
   ISNSAppendAttr (&cmd, ISNS_PROT_VER, 0, NULL, 0);
   ISNSAppendAttr (&cmd, ISNS_PORTAL_IDX, 0, NULL, 0);
   ISNSAppendAttr (&cmd, ISNS_PORTAL_IP, 0, NULL, 0);
   ISNSAppendAttr (&cmd, ISNS_PORTAL_PORT, 0, NULL, 0);
   ISNSAppendAttr (&cmd, ISNS_PORTAL_GROUP_ISCSI_NAME, 0, NULL, 0);
   ISNSAppendAttr (&cmd, ISNS_PORTAL_GROUP_PORT, 0, NULL, 0);
   ISNSAppendAttr (&cmd, ISNS_PORTAL_GROUP_IP, 0, NULL, 0);
   ISNSAppendAttr (&cmd, ISNS_PORTAL_GROUP_TAG, 0, NULL, 0);

   if (0 == ISNSSendCmd2 (&cmd, buffer, sizeof (buffer)))
   {
      /* This will parse the query rsp msg into 
         a human readable format. */
      ParseISCSIQuery (buffer);
   }
}

/***********************************************
This will deregister an iSCSI Node.
***********************************************/
void
DeRegisterISCSI (void)
{
   char cmdLine[256];

   /* Insert Header */
   ISNSCreateHdr (ISNS_DEREG_DEV_REQ, &cmd, sizeof (cmd), 0);

   /* Insert SRC */
   if (-1 == GetSrc (&cmd, 0))
      return;

   /* Delimiter */
   ISNSAppendAttr (&cmd, 0, 0, NULL, 0);

   /* Get iSCSI Node ID */
   if (0 == GetInput (cmdLine, "KEY: iSCSI Node Id", NULL, sizeof (cmdLine)))
   {
      printf ("***ERROR: required.\n");
      return;
   }

   ISNSAppendAttr (&cmd, ISNS_ISCSI_NODE_ID, PAD4 (strlen (cmdLine)), cmdLine, 0);


   ISNSSendCmd (&cmd);
}

/*****************************************************
This will add an iSCSI node to a DD.
*****************************************************/
void
Add_ISCSI_to_DD (void)
{
   char cmdLine[256];
   int dd_id;
   int iscsi_flag;
   int c_size;

   printf ("Adding ISCSI to DD.\n");
   ISNSCreateHdr (ISNS_REG_DD_REQ, &cmd, sizeof (cmd), 0);

   /* Insert SRC */
   if (-1 == GetSrc (&cmd, 0))
      return;

   /* Get DD ID */
   if (GetInput(cmdLine, "DD ID", NULL, sizeof(cmdLine)) == 0)
   {
      printf ("***ERROR: You must enter a DD ID.\n");
      return;
   }

   dd_id = atoi (cmdLine);
   /* Insert as a key */
   ISNSAppendAttr (&cmd, ISNS_DD_ID, 4, NULL, dd_id);

   /* Delimiter */
   ISNSAppendAttr (&cmd, 0, 0, NULL, 0);


   /* Get iSCSI Node ID */
   iscsi_flag = FALSE;
   do
   {
      /* You can put multiple members in single msg. */
      c_size = GetInput (cmdLine, "iSCSI Node ID", NULL, sizeof (cmdLine));
      if (c_size != 0)
      {
         ISNSAppendAttr (&cmd, ISNS_DD_ISCSI_MEMBER, PAD4 (strlen (cmdLine)),
                         cmdLine, 0);
         iscsi_flag = TRUE;
      }
   }
   while (c_size != 0);

   if (iscsi_flag)
      ISNSSendCmd (&cmd);
   else
      printf ("***ERROR: You must enter at least ONE iSCSI Node.\n");
}

/*******************************************************************
Sample Multi Attribute ISCSI registration.
This sample will register one or more iSCSI Nodes along with one
or more Portals.  An entity may also be created during the
registration process.
*******************************************************************/
void
MRegISCSI (void)
{
   char cmdLine[256];
   char eid[256];
   char sym[256];
   int port;
   int ip;
   int esi_interval;
   int port_type;
   int portal_group;
   int type;
   char b[256]={0};

   printf ("Multi-attr ISCSI Node Registration.\n");
   ISNSCreateHdr (ISNS_REG_DEV_ATTR_REQ, &cmd, sizeof (cmd), 0);

   /* Get Entity ID */
   memset (eid, 0, sizeof (eid));

   if (GetInput(cmdLine, "Entity ID", NULL, sizeof(cmdLine)) != 0)
   {
      strcpy (eid, cmdLine);
      ISNSAppendAttr (&cmd, ISNS_ENTITY_ID, PAD4 (strlen (eid)), eid, 0);
   }

   /* Delimiter */
   /* This seperates the keys from the op attr */
   ISNSAppendAttr (&cmd, 0, 0, NULL, 0);


   /* Get all iSCSI Nodes to be registered*/
   do
   {
      /* Get iSCSI Node ID */
      if (0 == GetInput (cmdLine, "iSCSI Node", NULL, sizeof (cmdLine)))
         break;

      ISNSAppendAttr (&cmd, ISNS_ISCSI_NODE_ID, PAD4 (strlen (cmdLine)),
                      cmdLine, 0);

      /* Get Alias */
      if (0 != GetInput (cmdLine, "iSCSI Alias", NULL, sizeof (cmdLine)))
      {
         ISNSAppendAttr (&cmd, ISNS_ISCSI_ALIAS, PAD4 (strlen (cmdLine)),
                         cmdLine, 0);
      }

      /* Get Type */
      type = 1;
      if (0 != GetInput (cmdLine, "Node Type Bitmask", "1:Target, 2:Initiator, 3:Target/Initiator", sizeof (cmdLine)))
      {
         type = atoi (cmdLine);
         if (type < 1 || type > 3)
         {
            printf ("***ERROR: Type is wrong.");
            return;
         }
      }
      ISNSAppendAttr (&cmd, ISNS_ISCSI_TYPE, 4, NULL, type);
   }
   while (1);

   /* Get all portals to be registered */
   do
   {
      if (0 == GetInput (cmdLine, "Portal IP", NULL, sizeof (cmdLine)))
         break;

      ip = inet_addr (cmdLine);

      {
         char tempb[256];
         memset (tempb, 0, sizeof (tempb));
         memcpy (tempb + 12, &ip, sizeof (ip));
         ISNSAppendAttr (&cmd, ISNS_PORTAL_IP, 16, tempb, 0);
      }

      /* Get Port Type */
      port_type = 0;
      if (0 !=
          GetInput (cmdLine, "Portal Type", "*0:UDP 1:TCP", sizeof (cmdLine)))
      {
         port_type = atoi (cmdLine);
         if (port_type > 1)
         {
            printf ("***ERROR: Port type is wrong.\n");
            return;
         }
      }

      /* Get Port */
      port = DEFAULT_PORTAL_PORT;
      itoa(DEFAULT_PORTAL_PORT, b, 10);
      if (0 != GetInput (cmdLine, "Portal Port", b, sizeof (cmdLine)))
      {
         port = atoi (cmdLine);
      }
      ISNSAppendAttr (&cmd, ISNS_PORTAL_PORT, 4, NULL,
                      port | (port_type == 1 ? 0 : 0x10000));

      /* Get an Portal Sym Name */
      memset (sym, 0, sizeof (sym));
      if (0 != GetInput (cmdLine, "Portal Sym Name", NULL, sizeof (cmdLine)))
      {
         ISNSAppendAttr (&cmd, ISNS_PORTAL_SYM_NAME, PAD4 (strlen (cmdLine)),
                         cmdLine, 0);
         strcpy (cmdLine, sym);
      }

      /* Get Portal Group */
      if (0 != GetInput (cmdLine, "Portal Group Tag", NULL, sizeof (cmdLine)))
      {
         portal_group = atoi (cmdLine);
         if (portal_group > 255 || portal_group == 0)
         {
            printf ("***ERROR: Invalid portal group tag.\n");
            return;
         }
         memset (cmdLine, 0, sizeof (cmdLine));
         ISNSAppendAttr (&cmd, ISNS_PORTAL_GROUP_TAG, 4, NULL, portal_group);
      }

      /* Get Interval */
      esi_interval = 0;
      if (0 != GetInput (cmdLine, "ESI Interval", "(0)", sizeof (cmdLine)))
      {
         esi_interval = atoi (cmdLine);
         ISNSAppendAttr (&cmd, ISNS_ESI_INTERVAL, 4, NULL, esi_interval);

         /* Get ESI Port */
         port = 0;
         if (0 != GetInput (cmdLine, "ESI/SCN UDP Port", NULL, sizeof (cmdLine)))
         {
            port = atoi (cmdLine);
            ISNSAppendAttr (&cmd, ISNS_ESI_PORT, 4, NULL, port);
         }

         if (port && esi_interval)
         {
#ifndef SNS_LINUX
            DWORD junk;
#endif
            LISTENER_CB_P p_cb;
            HANDLE hdle;

#ifdef SNS_LINUX
            hdle = (void *)malloc (sizeof (pthread_t));
#endif
            p_cb = (void *)malloc (sizeof (LISTENER_CB));

            memset (p_cb, 0, sizeof (LISTENER_CB));
            strcpy (p_cb->eid, eid);
            p_cb->port = port;
            p_cb->ip = ip;

#ifdef SNS_LINUX
            if (pthread_create (hdle, NULL, (void*) (ESIListener), p_cb) != 0)
               printf ("Could not create ESI Listener thread\n");
            else
#else
            hdle = CreateThread ((LPSECURITY_ATTRIBUTES)0, 0, (LPTHREAD_START_ROUTINE)ESIListener, p_cb, 0, &junk);
#endif
            AddESIPortal (sym, port, hdle);

         }
      }
   }
   while (1);

   ISNSSendCmd (&cmd);
}




