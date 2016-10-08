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
/***********************************************************************/
/* Deregisters an entity */
/***********************************************************************/
void
DeRegisterEntity (void)
{   
   char cmdLine[256];
   int c_size;
   int flag;
   ISNS_CMD cmd;

   printf ("DeRegistering Entity.\n");

   /* Register Portal */
   ISNSCreateHdr (ISNS_DEREG_DEV_REQ, &cmd, sizeof (cmd), 0);


   /* Get SRC */
   if (-1 == GetSrc (&cmd, 0))
      return;

   /* Add Delimiter */
   ISNSAppendAttr (&cmd, 0, 0, NULL, 0);

   /* Get Node */
   flag = FALSE;
   do
   {
      c_size = GetInput (cmdLine, "Entity ID", "(none)", sizeof (cmdLine));
      if (c_size != 0)
      {
         ISNSAppendAttr (&cmd, ISNS_ENTITY_ID, PAD4 (strlen (cmdLine)),
                         cmdLine, 0);
         flag = TRUE;
      }
   }
   while (c_size != 0);

   if (!flag)
   {
      printf ("***ERROR: At least one node is required.\n");
      return;
   }

   ISNSSendCmd (&cmd);
}

/***********************************************************************/
/* Queries for entities */
/***********************************************************************/
void
GetEntities (void)
{
   int c_size;
   char buffer[MAX_PAYLOAD_SIZE];
   char cmdLine[256];
   ISNS_CMD cmd;
   int key_type = 0;
   int added = FALSE;

   printf ("Query For Entities.\n");
   ISNSCreateHdr (ISNS_DEV_ATTR_QRY_REQ, &cmd, sizeof (cmd), 0);

   /* Src */
   if (-1 == GetSrc (&cmd, 0))
      return;


   if (0 != GetInput (cmdLine, "Key Type",
                 "*0:Entity Id 1: Entity Index", sizeof (cmdLine)))
   {
      key_type = atoi (cmdLine);
   }

   do
   {
      switch (key_type)
      {
         case 1:
            c_size = GetInput (cmdLine, "Enitity ID", "(All)", sizeof (cmdLine));
            if (c_size != 0)
            {
               ISNSAppendAttr (&cmd, ISNS_ENTITY_IDX, ISNS_INDEX_SIZE,
                               NULL, atoi(cmdLine));
            }
            break;
         default:
            c_size = GetInput (cmdLine, "Enitity ID", "(All)", sizeof (cmdLine));
            if (c_size != 0)
            {
               ISNSAppendAttr (&cmd, ISNS_ENTITY_ID, PAD4 (strlen (cmdLine)),
                               cmdLine, 0);
               added = TRUE;
            }
            else
            if (!added)
               ISNSAppendAttr (&cmd, ISNS_ENTITY_ID, 0, NULL, 0);

      }
   }
   while (c_size != 0);

   /* Delimiter */
   ISNSAppendAttr (&cmd, 0, 0, NULL, 0);

   ISNSAppendAttr (&cmd, ISNS_ENTITY_ID, 0, NULL, 0);
   ISNSAppendAttr (&cmd, ISNS_ENTITY_IDX, 0, NULL, 0);
   ISNSAppendAttr (&cmd, ISNS_ENTITY_TYPE, 0, NULL, 0);
   ISNSAppendAttr (&cmd, ISNS_MGMT_IP, 0, NULL, 0);
   ISNSAppendAttr (&cmd, ISNS_TIMESTAMP, 0, NULL, 0);
   ISNSAppendAttr (&cmd, ISNS_PROT_VER, 0, NULL, 0);
   ISNSAppendAttr (&cmd, ISNS_ENTITY_PERIOD, 0, NULL, 0);
   ISNSAppendAttr (&cmd, ISNS_PORTAL_IP, 0, NULL, 0);
   ISNSAppendAttr (&cmd, ISNS_PORTAL_PORT, 0, NULL, 0);

   if (0 == ISNSSendCmd2 (&cmd, buffer, sizeof (buffer)))
      ParseEntityQuery (buffer);
}

/***********************************************************************/
/* Register entities */
/***********************************************************************/
int
RegEntityI (void)
{
   char cmdLine[256];
   int ip;
   int type;
   ISNS_CMD cmd;

   printf ("Registering Entity.\n");
   /* Insert Header */
   printf("replaceFlag:%i\n",replaceFlag);
   printf("hdr replace:%i\n",replaceFlag?ISNS_FLAG_REPLACE_REG:0);


   ISNSCreateHdr (ISNS_REG_DEV_ATTR_REQ, &cmd, sizeof (cmd), replaceFlag?ISNS_FLAG_REPLACE_REG:0);

   if (-1==GetSrc(&cmd,0))
      return 0;

   /* Add Delimiter */
   ISNSAppendAttr (&cmd, 0, 0, NULL, 0);

   /* Get an entity ID */
   if (0 != GetInput (cmdLine, "Entity ID", "(none)", sizeof (cmdLine)))
   {
      ISNSAppendAttr (&cmd, ISNS_ENTITY_ID, PAD4 (strlen (cmdLine)), cmdLine,0);
   }

   /* Get Type */
   type = 2;
   if (0 !=
       GetInput (cmdLine, "Entity Type", "*2(iSCSI)/3(iFCP)",
                 sizeof (cmdLine)))
   {
      type = atoi (cmdLine);
   }
   ISNSAppendAttr (&cmd, ISNS_ENTITY_TYPE, 4, NULL, type);

   /* Get IP */
   ip = inet_addr (DEFAULT_IP_ADDR);
   if (0 !=
       GetInput (cmdLine, "Mgt Ip-Address", DEFAULT_IP_ADDR,
                 sizeof (cmdLine)))
   {
      ip = inet_addr (cmdLine);
   }

   {
      char tempb[256];
      memset (tempb, 0, sizeof (tempb));
      memcpy (tempb + 12, &ip, sizeof (ip));
      ISNSAppendAttr (&cmd, ISNS_MGMT_IP, 16, tempb, 0);
   }

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

         ISNSAppendAttr (&cmd, ISNS_PROT_VER, 4, (char *)&ver, 0);
      }
   }

   if (0 != GetInput (cmdLine, "Entity Period", "(none)", sizeof( cmdLine )))
   {
      int period;

      period = atoi (cmdLine);
      ISNSAppendAttr (&cmd, ISNS_ENTITY_PERIOD, 4, NULL, period);
   }

   ISNSSendCmd (&cmd);

   return (0);
}

/***********************************************************************/
/* Updates an existing entity */
/***********************************************************************/
int
UpdateEntity (void)
{
   char cmdLine[256];
   int ip;
   ISNS_CMD cmd;

   printf ("Updating Entity.\n");

   /* Insert Header */
   ISNSCreateHdr (ISNS_REG_DEV_ATTR_REQ, &cmd, sizeof (cmd),
                  0);

   /* Get an entity ID */
   if (0 == GetInput (cmdLine, "Entity ID", "(none)", sizeof (cmdLine)))
   {
      printf ("***ERROR: You must enter an Entity ID.\n");
      return (-1);
   }

   ISNSAppendAttr (&cmd, ISNS_ENTITY_ID, PAD4 (strlen (cmdLine)), cmdLine, 0);

   /* Add Delimiter */
   ISNSAppendAttr (&cmd, 0, 0, NULL, 0);

   /* Get IP */
   if (0 != GetInput (cmdLine, "Mgt Ip-Address", "(none)", sizeof (cmdLine)))
   {
      ip = inet_addr (cmdLine);

      {
         char tempb[256];
         memset (tempb, 0, sizeof (tempb));
         memcpy (tempb + 12, &ip, sizeof (ip));
         ISNSAppendAttr (&cmd, ISNS_MGMT_IP, 16, tempb, 0);
      }
   }

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

         ISNSAppendAttr (&cmd, ISNS_PROT_VER, 4, (char *)&ver, 0);
      }
   }

   if (0 != GetInput (cmdLine, "Entity Period", "(none)", sizeof( cmdLine )))
   {
      int period;

      period = atoi (cmdLine);
      ISNSAppendAttr (&cmd, ISNS_ENTITY_PERIOD, 4, NULL, period);
   }

   ISNSSendCmd (&cmd);
   return (0);
}




