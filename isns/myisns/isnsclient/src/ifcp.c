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
#include "comm.h"
#include "util.h"
#include "parse.h"


static ISNS_CMD cmd;
extern int replaceFlag;

/***********************************************************************
Parses an iFCP query response msg.
***********************************************************************/
void
ParseIFCPQuery (char *buffer)
{
   char buffer2[MAX_PAYLOAD_SIZE];
   ISNS_Attr *p_attr;
   ISNS_HDR *pdu;
   char *ptr;

   pdu = (struct isns_hdr *)buffer;
   ptr = (char *)buffer + sizeof (ISNS_HDR) + 4;
   while (ptr < (char *) buffer + sizeof (ISNS_HDR) + pdu->len)
   {
      p_attr = (struct ISNS_attr *)ptr;
      p_attr->tag = htonl (p_attr->tag);
      p_attr->len = htonl (p_attr->len);
      switch (p_attr->tag)
      {
      case ISNS_PORT_NAME:
         memset (buffer2, 0, sizeof (buffer2));
         printf ("---------------------------------\n");
         printf ("iFCP WWPN  : %s\n",
                 HexToAscii ((char *)&p_attr->val, p_attr->len, buffer2));
         break;
      case ISNS_PORT_SYM_NAME:
         printf ("Alias      : %s\n",(char *)&p_attr->val);
         break;
      case ISNS_PORT_TYPE:
         p_attr->val.etype = ntohl (p_attr->val.etype);
         printf ("Port Type  : %#x\n", p_attr->val.etype);
         break;
      case ISNS_FABRIC_PORT_NAME:
         printf ("Fabric Port--->\n");
         DumpHex (&p_attr->val, p_attr->len);
         break;
      case ISNS_FC_HARD_ADDR:
         printf ("Hard Addr--->\n");
         DumpHex (&p_attr->val, p_attr->len);
         break;
      case ISNS_FC_PORT_IP:
         printf ("FC PORT IP--->\n");
         DumpHex (&p_attr->val, p_attr->len);
         break;
      case ISNS_FC_COS:
         printf ("FC COS--->\n");
         DumpHex (&p_attr->val, p_attr->len);
         break;
      case ISNS_FC4_TYPE:
         printf ("FC4 Type--->\n");
         DumpHex (&p_attr->val, p_attr->len);
         break;
      case ISNS_FC4_DESC:
         printf ("FC4 Descriptor--->\n");
         DumpHex (&p_attr->val, p_attr->len);
         break;
      case ISNS_FC4_FEATURE:
         printf ("FC4 Feature--->\n");
         DumpHex (&p_attr->val, p_attr->len);
         break;
      case ISNS_IFCP_SCN_BITMAP:
         printf ("iFCP SCN Bitmap: %#x.\n", *(char *) &p_attr->val);
         break;
      case ISNS_DD_ID:
         p_attr->val.etype = ntohl (p_attr->val.etype);
         printf ("DD ID      : %d\n", p_attr->val.etype);
         break;
      case ISNS_ENTITY_ID:
         printf ("Entity ID  : %s\n",(char *)&p_attr->val);
         break;
      default:
         break;
      }
      ptr = (char *) ptr + p_attr->len + 8;
   }
}

/***********************************************************************
Example of a multi attribute iFCP Node registration.
***********************************************************************/
void
MRegIFCP (void)
{
   char cmdLine[256];
   char eid[256];
   char sym[256];
   int port;
   int ip;
   int esi_interval;
   int port_type;
   int portal_group;
   char nodeAlias[256];
   int c_size;
   char b[256]={0};

   printf ("Multi-attr iFCP Node Registration.\n");
   ISNSCreateHdr (ISNS_REG_DEV_ATTR_REQ, &cmd, sizeof (cmd), 0);

   /* Get Entity ID */
   memset (eid, 0, sizeof (eid));

   if (GetInput(cmdLine, "Entity ID", NULL, sizeof(cmdLine)) != 0)
   {
      strcpy (eid, cmdLine);
      ISNSAppendAttr (&cmd, ISNS_ENTITY_ID, PAD4 (strlen (eid)), eid, 0);
   }

   ISNSAppendAttr (&cmd, 0, 0, NULL, 0);

   if (strlen (cmdLine) == 0)
   {
      /* Get Version */
      {
         SOIP_Ver ver;
         memset (&ver, 0, sizeof (ver));

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

   do
   {
      /* Get iFCP Node */
      if (0 ==
          GetHexInput (cmdLine, "iFCP Node WWPN", NULL, sizeof (cmdLine)))
         break;

      ISNSAppendAttr (&cmd, ISNS_PORT_NAME, 8, cmdLine, 0);

      /* Get Port Sym Name */
      memset (nodeAlias, 0, sizeof (nodeAlias));
      strcpy (nodeAlias, "Port Name");
      c_size =
         GetInput (cmdLine, "iFCP Port Sym Name", nodeAlias,
                   sizeof (cmdLine));
      if (c_size != 0)
      {
         memcpy (nodeAlias, cmdLine, sizeof (cmdLine));
      }
      ISNSAppendAttr (&cmd, ISNS_PORT_SYM_NAME, PAD4 (strlen (nodeAlias)),
                      nodeAlias, 0);

      /* Get Port ID */
      if (0 != GetInput (cmdLine, "iFCP Port ID", NULL, sizeof (cmdLine)))
      {
         ISNSAppendAttr (&cmd, ISNS_PORT_ID, 4, cmdLine, 0);
      }

      /* Get Port Type */
      if (0 != GetInput (cmdLine, "iFCP Port Type", NULL, sizeof (cmdLine)))
      {
         ISNSAppendAttr (&cmd, ISNS_PORT_TYPE, 4, NULL, atoi (cmdLine));
      }

      if (0 != GetHexInput (cmdLine, "FC Node WWNN", NULL, sizeof (cmdLine)))
      {
         ISNSAppendAttr (&cmd, ISNS_NODE_NAME, 8, cmdLine, 0);
      }
   }
   while (1);

   /* Get IP */
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
            printf ("***ERROR: Invalid portal group.\n");
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

         /* Get ESI Port */
         ISNSAppendAttr (&cmd, ISNS_ESI_INTERVAL, 4, NULL, esi_interval);

         port = 0;
         if (0 != GetInput (cmdLine, "ESI/SCN Port", NULL, sizeof (cmdLine)))
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

/***********************************************************************
Registers iFCP nodes.
***********************************************************************/
void
RegIFCP (void)
{
   char cmdLine[256];
   char nodeID[256];
   char eid[256];
   char nodeAlias[256];
   time_t t;
   int c_size;

   printf ("Registering iFCP Node.\n");
   ISNSCreateHdr (ISNS_REG_DEV_ATTR_REQ, &cmd, sizeof (cmd), replaceFlag?ISNS_FLAG_REPLACE_REG:0);

   /* Get Entity ID */
   c_size = GetInput (eid, "Entity ID", "(none)", sizeof (eid));
   if (c_size != 0)
   {
      ISNSAppendAttr (&cmd, ISNS_ENTITY_ID, PAD4 (strlen (eid)), eid, 0);
   }

   /* Delimiter */
   ISNSAppendAttr (&cmd, 0, 0, NULL, 0);

   if (strlen (eid) == 0)
   {
      /* Get Version */
      {
         SOIP_Ver ver;
         memset (&ver, 0, sizeof (ver));

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

   /* Get iFCP Node */
   do
   {
      memset (nodeID, 0, sizeof (nodeID));
      strcpy (nodeID, "iFCP ");
      time (&t);
      nodeID[0] = 0x02;
      GetHexTime ((char *) nodeID + 1);
      c_size =
         GetHexInput (cmdLine, "iFCP Node WWPN", nodeID, sizeof (cmdLine));
      if (c_size != 0)
      {
         memcpy (nodeID, cmdLine, sizeof (cmdLine));
      }
      ISNSAppendAttr (&cmd, ISNS_PORT_NAME, 8, nodeID, 0);

      /* Get Port Sym Name */
      memset (nodeAlias, 0, sizeof (nodeAlias));
      strcpy (nodeAlias, "Port Name");
      c_size =
         GetInput (cmdLine, "iFCP Port Sym Name", nodeAlias,
                   sizeof (cmdLine));
      if (c_size != 0)
      {
         memcpy (nodeAlias, cmdLine, sizeof (cmdLine));
      }
      ISNSAppendAttr (&cmd, ISNS_PORT_SYM_NAME, PAD4 (strlen (nodeAlias)),
                      nodeAlias, 0);

      /* Get Port ID */
      if (0 != GetInput (cmdLine, "iFCP Port ID", NULL, sizeof (cmdLine)))
      {
         ISNSAppendAttr (&cmd, ISNS_PORT_ID, 4, cmdLine, 0);
      }

      /* Get Port Type */
      if (0 != GetInput (cmdLine, "iFCP Port Type", NULL, sizeof (cmdLine)))
      {
         ISNSAppendAttr (&cmd, ISNS_PORT_TYPE, 4, NULL, atoi (cmdLine));
      }

      if (0 != GetHexInput (cmdLine, "FC Node WWNN", NULL, sizeof (cmdLine)))
      {
         ISNSAppendAttr (&cmd, ISNS_NODE_NAME, 8, cmdLine, 0);
      }
   }
   while (GetYesNo (cmdLine, "More iFCP Node?", FALSE, sizeof (cmdLine)));

   ISNSSendCmd (&cmd);
}

/***********************************************************************
Update an iFCP node.
***********************************************************************/
void
UpdateIFCP (void)
{
   char cmdLine[256];

   printf ("Updating iFCP Node.\n");
   ISNSCreateHdr (ISNS_REG_DEV_ATTR_REQ, &cmd, sizeof (cmd),
                  0);

   /* Get iFCP Node */
   if (0 != GetHexInput (cmdLine, "iFCP Node WWPN", NULL, sizeof (cmdLine)))
   {
      ISNSAppendAttr (&cmd, ISNS_PORT_NAME, 8, cmdLine, 0);
   }

   /* Delimiter */
   ISNSAppendAttr (&cmd, 0, 0, NULL, 0);

   /* Get Port Sym Name */
   if (0 != GetInput (cmdLine, "iFCP Port Sym Name", NULL, sizeof (cmdLine)))
   {
      ISNSAppendAttr (&cmd, ISNS_PORT_SYM_NAME, PAD4 (strlen (cmdLine)),
                      cmdLine, 0);
   }

   ISNSSendCmd (&cmd);
}

/***********************************************************************
Query for iFCP node(s).
***********************************************************************/
void
GetIFCP (void)
{
   char buffer[MAX_PAYLOAD_SIZE];
   char cmdLine[256];
   int c_size;
   int key_type;
   int ip;
   int port;

   /* Query for Node */
   printf ("Query for IFCP Nodes.\n");
   ISNSCreateHdr (ISNS_DEV_ATTR_QRY_REQ, &cmd, sizeof (cmd), 0);

   /* Src */
   if (-1 == GetSrc (&cmd, 0))
      return;

   /* Keys */
   key_type = 0;
   if (0 !=
       GetInput (cmdLine,
                 "Key type (1:iFCP, 2:Entity ID 3:Portal IP/Port 4:FC Node WWNN 5:iFCP Index)>",
                 "(none)", sizeof (cmdLine)))
   {
      key_type = atoi (cmdLine);
      if (key_type == 1)
      {
         do
         {
            c_size =
               GetHexInput (cmdLine, "iFCP Node WWPN", NULL,
                            sizeof (cmdLine));
            if (c_size != 0)
            {
               ISNSAppendAttr (&cmd, ISNS_PORT_NAME, 8, cmdLine, 0);
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
         c_size =
            GetHexInput (cmdLine, "FC Node WWNN", NULL, sizeof (cmdLine));
         if (c_size != 0)
         {
            ISNSAppendAttr (&cmd, ISNS_NODE_NAME, 8, cmdLine, 0);
         }
      }
      else if (key_type == 5)
      {
         c_size =
            GetInput (cmdLine, "Portal Idx", "(none)", sizeof (cmdLine));
         if (c_size != 0)
         {
            port = atoi (cmdLine);
            ISNSAppendAttr (&cmd, ISNS_PORTAL_IDX, 4, NULL, port);
         }
      }
      else
      {
         printf ("***ERROR: Invalid key_type.\n");
         return;
      }
   }

   ISNSAppendAttr (&cmd, 0, 0, NULL, 0);
   ISNSAppendAttr (&cmd, ISNS_PORT_NAME, 0, NULL, 0);
   ISNSAppendAttr (&cmd, ISNS_ENTITY_ID, 0, NULL, 0);
   ISNSAppendAttr (&cmd, ISNS_ENTITY_IDX, 0, NULL, 0);
   ISNSAppendAttr (&cmd, ISNS_PORT_ID, 0, NULL, 0);
   ISNSAppendAttr (&cmd, ISNS_PORT_TYPE, 0, NULL, 0);
   ISNSAppendAttr (&cmd, ISNS_PORT_SYM_NAME, 0, NULL, 0);
   ISNSAppendAttr (&cmd, ISNS_DD_ID, 0, NULL, 0);

   ISNSAppendAttr (&cmd, ISNS_FABRIC_PORT_NAME, 0, NULL, 0);
   ISNSAppendAttr (&cmd, ISNS_FC_HARD_ADDR, 0, NULL, 0);
   ISNSAppendAttr (&cmd, ISNS_FC_PORT_IP, 0, NULL, 0);
   ISNSAppendAttr (&cmd, ISNS_FC_COS, 0, NULL, 0);
   ISNSAppendAttr (&cmd, ISNS_FC4_TYPE, 0, NULL, 0);
//   ISNSAppendAttr(&cmd, ISNS_FC4_DESC, 0, NULL, 0);
//   ISNSAppendAttr(&cmd, ISNS_FC4_FEATURE, 0, NULL, 0);
   ISNSAppendAttr (&cmd, ISNS_IFCP_SCN_BITMAP, 0, NULL, 0);
   ISNSAppendAttr (&cmd, ISNS_IFCP_NODE_CERT, 0, NULL, 0);

   if (0 == ISNSSendCmd2 (&cmd, buffer, sizeof (buffer)))
   {
      ParseIFCPQuery (buffer);
   }
}

/***********************************************************************
Adds one or more iFCP Nodes to a DD.
***********************************************************************/
void
Add_IFCP_to_DD (void)
{
   char cmdLine[256];
   int dd_id;
   int iscsi_flag;
   int c_size;

   printf ("Adding IFCP to DD.\n");
   ISNSCreateHdr (ISNS_REG_DD_REQ, &cmd, sizeof (cmd), 0);

   /* SRC */
   ISNSAppendAttr (&cmd, 0, 0, NULL, 0);

   /* Get DD ID */
   if (0==GetInput(cmdLine, "DD ID", NULL, sizeof(cmdLine)))
   {
      printf ("***ERROR: You must enter a DD.\n");
      return;
   }

   dd_id = atoi (cmdLine);
   ISNSAppendAttr (&cmd, ISNS_DD_ID, 4, NULL, dd_id);

   /* Delimiter */
   ISNSAppendAttr (&cmd, 0, 0, NULL, 0);


   /* Get iFCP Node WWPN */
   iscsi_flag = FALSE;
   do
   {
      c_size =
         GetHexInput (cmdLine, "iFCP Node WWPN", NULL, sizeof (cmdLine));
      if (c_size != 0)
      {
         ISNSAppendAttr (&cmd, ISNS_DD_IFCP_MEMBER, 8, cmdLine, 0);
         iscsi_flag = TRUE;
      }
   }
   while (c_size != 0);

   if (iscsi_flag)
      ISNSSendCmd (&cmd);
   else
      printf ("***ERROR: You must enter at least ONE iFCP Node.\n");
}

/***********************************************************************
Deregisters one or more iFCP node.
***********************************************************************/
void
DeRegisterPort (void)
{
   char cmdLine[256];
   int c_size;
   int nodeFlag;

   printf ("DeRegistering iFCP Node.\n");

   ISNSCreateHdr (ISNS_DEREG_DEV_REQ, &cmd, sizeof (cmd), 0);

   if (-1 == GetSrc (&cmd, 0))
      return;

   /* Get iFCP Node */
   nodeFlag = FALSE;
   do
   {
      c_size =
         GetHexInput (cmdLine, "iFCP Node WWPN", NULL, sizeof (cmdLine));
      if (c_size != 0)
      {
         ISNSAppendAttr (&cmd, ISNS_PORT_NAME, 8, cmdLine, 0);
         nodeFlag = TRUE;
      }
   }
   while (c_size != 0);

   if (!nodeFlag)
   {
      printf ("***ERROR: At least one node is required.\n");
      return;
   }

   ISNSSendCmd (&cmd);
}



