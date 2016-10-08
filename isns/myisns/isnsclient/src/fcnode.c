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

extern int replaceFlag;

void
DeRegisterNode (void)
{
   char cmdLine[256];
   int c_size;
   int nodeFlag;
   ISNS_CMD cmd;

   printf ("DeRegistering Port.\n");

   /* Register Portal */
   ISNSCreateHdr (ISNS_DEREG_DEV_REQ, &cmd, sizeof (cmd), 0);

   ISNSAppendAttr (&cmd, 0, 0, NULL, 0);

   /* Get iFCP Node */
   nodeFlag = FALSE;
   do
   {
      c_size = GetHexInput (cmdLine, "FC Node WWNN", NULL, sizeof (cmdLine));
      if (c_size != 0)
      {
         ISNSAppendAttr (&cmd, ISNS_NODE_NAME, 8, cmdLine, 0);
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

static int currentNode = 0;
void
RegisterNode (void)
{
   int len;
   char buffer[MAX_PAYLOAD_SIZE];
   ISNS_CMD cmd;

   currentNode++;
   printf ("Registering Node Name.\n");
   ISNSCreateHdr (ISNS_REG_DEV_ATTR_REQ, &cmd, sizeof (cmd), 0);

   memset (buffer, 0, sizeof (buffer));
   strcpy (buffer, "WWNN ");
   itoa (currentNode, (char *) buffer + strlen (buffer), sizeof (buffer));

   ISNSAppendAttr (&cmd, ISNS_NODE_NAME, 8, buffer, 0);
   ISNSAppendAttr (&cmd, 0, 0, NULL, 0);
   ISNSAppendAttr (&cmd, ISNS_NODE_NAME, 8, buffer, 0);

   memset (buffer, 0, sizeof (buffer));
   strcpy (buffer, "Node Sym Name ");
   itoa (currentNode, (char *) buffer + strlen (buffer), sizeof (buffer));
   len = PAD4 (strlen (buffer));

   ISNSAppendAttr (&cmd, ISNS_NODE_SYM_NAME, len, buffer, 0);
   ISNSAppendAttr (&cmd, ISNS_FC_NODE_IP, 16, "FC NODE IP000000", 0);
   ISNSAppendAttr (&cmd, ISNS_FC_NODE_IPA, 8, "FC NODE0", 0);

   ISNSSendCmd (&cmd);
}

void
GetAllNodes (void)
{
   ISNS_CMD cmd;

   /* Query for all Node */
   printf ("Query for all Node Name.\n");
   ISNSInsertQryHdr (ISNS_DEV_ATTR_QRY_REQ, &cmd);
   ISNSAppendAttr (&cmd, 0, 0, NULL, 0);
   ISNSAppendAttr (&cmd, ISNS_NODE_NAME, 8, NULL, 0);
   ISNSAppendAttr (&cmd, ISNS_NODE_SYM_NAME, 4, NULL, 0);
   ISNSAppendAttr (&cmd, ISNS_FC_NODE_IP, 16, NULL, 0);
   ISNSAppendAttr (&cmd, ISNS_FC_NODE_IPA, 8, NULL, 0);

   ISNSSendCmd (&cmd);
}

void
ParseFCNodeQuery (char *buffer)
{
   char buffer2[MAX_PAYLOAD_SIZE];
   ISNS_Attr *p_attr;
   ISNS_HDR *pdu;
   char *ptr;

   pdu = (struct isns_hdr *)buffer;
   ptr = buffer + sizeof (ISNS_HDR) + 4;
   while (ptr < (char *) buffer + sizeof (ISNS_HDR) + pdu->len)
   {
      p_attr = (struct ISNS_attr *)ptr;
      p_attr->tag = htonl (p_attr->tag);
      p_attr->len = htonl (p_attr->len);
      switch (p_attr->tag)
      {
      case ISNS_NODE_NAME:
         memset (buffer2, 0, sizeof (buffer2));
         printf ("---------------------------------\n");
         printf ("FC WWNN : %s\n",
                 HexToAscii ((char *)&p_attr->val, p_attr->len, buffer2));
         break;
      case ISNS_NODE_SYM_NAME:
         printf ("Sym Name: %s\n",(char *)&p_attr->val);
         break;
      case ISNS_PORT_TYPE:
         p_attr->val.etype = ntohl (p_attr->val.etype);
         printf ("Port Type  : %#x\n", p_attr->val.etype);
         break;
      case ISNS_FC_NODE_IP:
         printf ("FC PORT IP--->\n");
         DumpHex (&p_attr->val, p_attr->len);
         break;
      case ISNS_FC_NODE_IPA:
         printf ("FC PORT IP--->\n");
         DumpHex (&p_attr->val, p_attr->len);
         break;
      case ISNS_FC_NODE_CERT:
         printf ("FC PORT IP--->\n");
         DumpHex (&p_attr->val, p_attr->len);
         break;
      default:
         break;
      }
      ptr = (char *) ptr + p_attr->len + 8;
   }
}

void
RegFCNode (void)
{
   char cmdLine[256];
   char nodeID[256];
   char nodeAlias[256];
   int c_size;
   ISNS_CMD cmd;

   printf ("Registering FC Node.\n");
   ISNSCreateHdr (ISNS_REG_DEV_ATTR_REQ, &cmd, sizeof (cmd), replaceFlag?ISNS_FLAG_REPLACE_REG:0);

   /* Delimiter */
   ISNSAppendAttr (&cmd, 0, 0, NULL, 0);

   /* Get FC Node */
   nodeID[0] = 0x05;
   GetHexTime ((char *) nodeID + 1);
   c_size = GetHexInput (cmdLine, "FC Node WWNN", nodeID, sizeof (cmdLine));
   if (c_size != 0)
   {
      memcpy (nodeID, cmdLine, sizeof (cmdLine));
   }

   ISNSAppendAttr (&cmd, ISNS_NODE_NAME, 8, nodeID, 0);

   /* Get Port Sym Name */
   memset (nodeAlias, 0, sizeof (nodeAlias));
   strcpy (nodeAlias, "Port Name");
   c_size =
      GetInput (cmdLine, "FC Device Sym Name", nodeAlias, sizeof (cmdLine));
   if (c_size != 0)
   {
      memcpy (nodeAlias, cmdLine, sizeof (cmdLine));
   }
   ISNSAppendAttr (&cmd, ISNS_NODE_SYM_NAME, PAD4 (strlen (nodeAlias)),
                   nodeAlias, 0);

   ISNSSendCmd (&cmd);
}

void
GetFCNode (void)
{
   char buffer[MAX_PAYLOAD_SIZE];
   ISNS_CMD cmd;

   printf ("Querying for All FC Node.\n");
   ISNSCreateHdr (ISNS_DEV_ATTR_QRY_REQ, &cmd, sizeof (cmd), 0);

   /* SRC */
   ISNSAppendAttr (&cmd, 0, 0, NULL, 0);

   /* DELIM */
   ISNSAppendAttr (&cmd, 0, 0, NULL, 0);

   /* Get FC Node */
   ISNSAppendAttr (&cmd, ISNS_NODE_NAME, 0, NULL, 0);
   ISNSAppendAttr (&cmd, ISNS_NODE_SYM_NAME, 0, NULL, 0);

   if (0 == ISNSSendCmd2 (&cmd, buffer, sizeof (buffer)))
   {
      ParseFCNodeQuery (buffer);
   }
}


void
UpdateFCNode (void)
{
   char cmdLine[256];
   ISNS_CMD cmd;

   printf ("Updating FC Node.\n");
   ISNSCreateHdr (ISNS_REG_DEV_ATTR_REQ, &cmd, sizeof (cmd),
                  0);

   /* Get iFCP Node */
   if (0 != GetHexInput (cmdLine, "FC Node WWNN", NULL, sizeof (cmdLine)))
   {
      ISNSAppendAttr (&cmd, ISNS_NODE_NAME, 8, cmdLine, 0);
   }

   /* Delimiter */
   ISNSAppendAttr (&cmd, 0, 0, NULL, 0);

   /* Get Port Sym Name */
   if (0 != GetInput (cmdLine, "iFCP Port Sym Name", NULL, sizeof (cmdLine)))
   {
      ISNSAppendAttr (&cmd, ISNS_NODE_SYM_NAME, PAD4 (strlen (cmdLine)),
                      cmdLine, 0);
   }

   ISNSSendCmd (&cmd);
}



