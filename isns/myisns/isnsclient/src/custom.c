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
#ifdef SNS_LINUX
#include <getopt.h>
#else
#include "getarg.h"
#endif
#include "iscsi.h"
#include "portal.h"
#include "entity.h"
#include "ifcp.h"
#include "fcnode.h"
#include "util.h"
#include "comm.h"
#include "parse.h"

#include "custom.h"

static ISNS_CMD cmd;
extern int replaceFlag;

/* Custom registrations */
void
RegCustom1 (void)
{
   char buffer[ 256 ];
   memset (buffer, 0, sizeof (buffer));

   ISNSCreateHdr (ISNS_REG_DEV_ATTR_REQ, &cmd, sizeof (cmd), replaceFlag?ISNS_FLAG_REPLACE_REG:0);

   /* Add Delimiter */
   ISNSAppendAttr (&cmd, 0, 0, NULL, 0);
   memcpy (buffer, "Entity 1", sizeof (buffer));
   ISNSAppendAttr (&cmd, ISNS_ENTITY_ID, PAD4 (strlen (buffer)), buffer, 0);
   ISNSAppendAttr (&cmd, ISNS_ENTITY_TYPE, 4, NULL, 1);
   ISNSAppendAttr (&cmd, ISNS_ENTITY_CERT, 4, "1234", 0);

   ISNSSendCmd (&cmd);
}

void
RegCustom2 (void)
{
   char buffer[256];

   memset (buffer, 0, sizeof (buffer));

   /* Register Entity 1 */
   ISNSCreateHdr (ISNS_REG_DEV_ATTR_REQ, &cmd, sizeof (cmd), replaceFlag?ISNS_FLAG_REPLACE_REG:0);

   /* Add Delimiter */
   ISNSAppendAttr (&cmd, 0, 0, NULL, 0);
   memcpy (buffer, "Entity 1", sizeof (buffer));
   ISNSAppendAttr (&cmd, ISNS_ENTITY_ID, PAD4 (strlen (buffer)), buffer, 0);
   ISNSAppendAttr (&cmd, ISNS_ENTITY_TYPE, 4, NULL, 1);

   /* Register iSCSI Node 1 */
   memset (buffer, 0, sizeof (buffer));
   strcpy (buffer, "iSCSI Node 1");
   ISNSAppendAttr (&cmd, ISNS_ISCSI_NODE_ID, PAD4 (strlen (buffer)), buffer,
                   0);
   ISNSAppendAttr (&cmd, ISNS_ISCSI_TYPE, 4, NULL, 1);
   ISNSAppendAttr (&cmd, ISNS_ISCSI_SCN_BITMAP, 4, NULL, 0x1F);

   /* Register iSCSI Node 2 */
   memset (buffer, 0, sizeof (buffer));
   strcpy (buffer, "iSCSI Node 2");
   ISNSAppendAttr (&cmd, ISNS_ISCSI_NODE_ID, PAD4 (strlen (buffer)), buffer,
                   0);
   ISNSAppendAttr (&cmd, ISNS_ISCSI_TYPE, 4, NULL, 1);
   ISNSAppendAttr (&cmd, ISNS_ISCSI_SCN_BITMAP, 4, NULL, 0x1f);

   /* Register iSCSI Node 3 */
   memset (buffer, 0, sizeof (buffer));
   strcpy (buffer, "iSCSI Node 3");
   ISNSAppendAttr (&cmd, ISNS_ISCSI_NODE_ID, PAD4 (strlen (buffer)), buffer,
                   0);
   ISNSAppendAttr (&cmd, ISNS_ISCSI_TYPE, 4, NULL, 1);
   ISNSAppendAttr (&cmd, ISNS_ISCSI_SCN_BITMAP, 4, NULL, 0x1f);

   /* Register iSCSI Node 4 */
   memset (buffer, 0, sizeof (buffer));
   strcpy (buffer, "iSCSI Node 4");
   ISNSAppendAttr (&cmd, ISNS_ISCSI_NODE_ID, PAD4 (strlen (buffer)), buffer,
                   0);
   ISNSAppendAttr (&cmd, ISNS_ISCSI_TYPE, 4, NULL, 1);
   ISNSAppendAttr (&cmd, ISNS_ISCSI_SCN_BITMAP, 4, NULL, 0x1f);

   ISNSSendCmd (&cmd);
   WaitForKey ();

   /* Register DD */
   ISNSCreateHdr (ISNS_REG_DD_REQ, &cmd, sizeof (cmd), 0);
   ISNSAppendAttr (&cmd, 0, 0, NULL, 0);

   /* Delimiter */
   ISNSAppendAttr (&cmd, 0, 0, NULL, 0);
   ISNSAppendAttr (&cmd, ISNS_DD_ID, 4, NULL, 1);
   memset (buffer, 0, sizeof (buffer));
   strcpy (buffer, "DD 1");
   ISNSAppendAttr (&cmd, ISNS_DD_SYM_NAME, PAD4 (strlen (buffer)), buffer, 0);
   memset (buffer, 0, sizeof (buffer));
   strcpy (buffer, "iSCSI Node 1");
   ISNSAppendAttr (&cmd, ISNS_DD_ISCSI_MEMBER, PAD4 (strlen (buffer)), buffer,
                   0);
   memset (buffer, 0, sizeof (buffer));
   strcpy (buffer, "iSCSI Node 2");
   ISNSAppendAttr (&cmd, ISNS_DD_ISCSI_MEMBER, PAD4 (strlen (buffer)), buffer,
                   0);
   strcpy (buffer, "iSCSI Node 3");
   ISNSAppendAttr (&cmd, ISNS_DD_ISCSI_MEMBER, PAD4 (strlen (buffer)), buffer,
                   0);
   memset (buffer, 0, sizeof (buffer));
   strcpy (buffer, "iSCSI Node 4");
   ISNSAppendAttr (&cmd, ISNS_DD_ISCSI_MEMBER, PAD4 (strlen (buffer)), buffer,
                   0);

   ISNSSendCmd (&cmd);
   WaitForKey ();

   ISNSCreateHdr (ISNS_REG_DD_REQ, &cmd, sizeof (cmd), 0);

   ISNSAppendAttr (&cmd, 0, 0, NULL, 0);
   ISNSAppendAttr (&cmd, 0, 0, NULL, 0);
   ISNSAppendAttr (&cmd, ISNS_DD_ID, 4, NULL, 2);
   memset (buffer, 0, sizeof (buffer));
   strcpy (buffer, "DD 2");
   ISNSAppendAttr (&cmd, ISNS_DD_SYM_NAME, PAD4 (strlen (buffer)), buffer, 0);

   ISNSSendCmd (&cmd);
   WaitForKey ();

   /* Register DDS */
   printf ("Registering DDS.\n");
   ISNSCreateHdr (ISNS_REG_DDS_REQ, &cmd, sizeof (cmd), 0);
   ISNSAppendAttr (&cmd, 0, 0, NULL, 0);
   ISNSAppendAttr (&cmd, 0, 0, NULL, 0);

   ISNSAppendAttr (&cmd, ISNS_DDS_ID, 4, NULL, 1);
   memset (buffer, 0, sizeof (buffer));
   strcpy (buffer, "DDS 1");
   ISNSAppendAttr (&cmd, ISNS_DDS_SYM_NAME, PAD4 (strlen (buffer)), buffer,
                   0);
   ISNSAppendAttr (&cmd, ISNS_DDS_STATUS, 4, NULL, 0);
   ISNSAppendAttr (&cmd, ISNS_DD_ID, 4, NULL, 1);

   ISNSSendCmd (&cmd);
   WaitForKey ();

   ISNSCreateHdr (ISNS_REG_DDS_REQ, &cmd, sizeof (cmd), 0);

   ISNSAppendAttr (&cmd, 0, 0, NULL, 0);
   ISNSAppendAttr (&cmd, 0, 0, NULL, 0);
   ISNSAppendAttr (&cmd, ISNS_DDS_ID, 4, NULL, 2);
   memset (buffer, 0, sizeof (buffer));
   strcpy (buffer, "DDS 2");
   ISNSAppendAttr (&cmd, ISNS_DDS_SYM_NAME, PAD4 (strlen (buffer)), buffer,
                   0);
   ISNSAppendAttr (&cmd, ISNS_DDS_STATUS, 4, NULL, 0);
   ISNSAppendAttr (&cmd, ISNS_DD_ID, 4, NULL, 2);

   ISNSSendCmd (&cmd);
   WaitForKey ();
}

void
RegCustom3 (void)
{
   char buffer[256];
   memset (buffer, 0, sizeof (buffer));

   /* Register Entity 1 */
   ISNSCreateHdr (ISNS_REG_DEV_ATTR_REQ, &cmd, sizeof (cmd), 0);

   /* Add Delimiter */
   ISNSAppendAttr (&cmd, 0, 0, NULL, 0);
   memcpy (buffer, "Entity 1", sizeof (buffer));
   ISNSAppendAttr (&cmd, ISNS_ENTITY_ID, PAD4 (strlen (buffer)), buffer, 0);
   ISNSAppendAttr (&cmd, ISNS_ENTITY_TYPE, 4, NULL, 2);

   /* Register iFCP Node 1 */
   memset (buffer, 0, sizeof (buffer));
   strcpy (buffer, "PORTNAM1");
   ISNSAppendAttr (&cmd, ISNS_PORT_NAME, 8, buffer, 0);
   ISNSAppendAttr (&cmd, ISNS_PORT_ID, 4, "PID1", 1);
   ISNSAppendAttr (&cmd, ISNS_PORT_TYPE, 4, NULL, 0xFF12);
   ISNSAppendAttr (&cmd, ISNS_FABRIC_PORT_NAME, 8, "FP_NAME1", 0);
   ISNSAppendAttr (&cmd, ISNS_FC_HARD_ADDR, 4, "HRD1", 0);
   {
      char tempip[16];
      memset (tempip, 0, sizeof (tempip));
      *(uint32_t *) & tempip[12] = inet_addr (DEFAULT_IP_ADDR);
      ISNSAppendAttr (&cmd, ISNS_FC_PORT_IP, 16, tempip, 0);
   }
   ISNSAppendAttr (&cmd, ISNS_FC_COS, 4, "COS 1", 0);

   ISNSAppendAttr (&cmd, ISNS_IFCP_SCN_BITMAP, 4, NULL, 0x1f);

   ISNSSendCmd (&cmd);
   WaitForKey ();
   return;
}



