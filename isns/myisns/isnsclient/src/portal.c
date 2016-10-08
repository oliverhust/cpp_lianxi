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

/***********************************************************************/
/* DeRegisters a portal */
/***********************************************************************/
void
DeRegisterPortal (void)
{
   char cmdLine[256];
   int port;
   int ip;
   int portal_type;
   char b[256]={0};

   /* Insert Header */
   ISNSCreateHdr (ISNS_DEREG_DEV_REQ, &cmd, sizeof (cmd), 0);

   if (-1 == GetSrc (&cmd, 0))
      return;

   /* Add Delimiter */
   ISNSAppendAttr (&cmd, 0, 0, NULL, 0);

   ip = inet_addr (DEFAULT_IP_ADDR);
   if (0 !=
       GetInput ((char *)&cmdLine, "Portal IP", DEFAULT_IP_ADDR, sizeof (cmdLine)))
   {
      ip = inet_addr (cmdLine);
   }

   {
      char tempb[256];
      memset (tempb, 0, sizeof (tempb));
      memcpy (tempb + 12, &ip, sizeof (ip));
      ISNSAppendAttr (&cmd, ISNS_PORTAL_IP, 16, tempb, 0);
   }

   /* Get Portal Type */
   portal_type = 0;
   if (0 !=
       GetInput (cmdLine, "Portal Type", "*0: UDP 1: TCP", sizeof (cmdLine)))
   {
      portal_type = atoi (cmdLine);
   }

   /* Get Port */
   port = DEFAULT_PORTAL_PORT;
   itoa(DEFAULT_PORTAL_PORT, b, 10);
   if (0 != GetInput (cmdLine, "Portal Port", b, sizeof (cmdLine)))
   {
      port = atoi (cmdLine);
   }

   ISNSAppendAttr (&cmd, ISNS_PORTAL_PORT, 4, NULL,
                   port | (portal_type == 0 ? 0x10000 : 0));

   ISNSSendCmd (&cmd);
}

/***********************************************************************/
/* Queries for portals */
/***********************************************************************/
void
GetPortals (void)
{
   char buffer[MAX_PAYLOAD_SIZE];
   char cmdLine[256];
   int c_size;
   int portal_type;
   int key_type;

   ISNSCreateHdr (ISNS_DEV_ATTR_QRY_REQ, &cmd, sizeof (cmd), 0);

   /* Src */
   if (-1 == GetSrc (&cmd, 0))
      return;
   
   /* Key */
   key_type = 0;
   if (0 !=
       GetInput (cmdLine, "Key type (1:iSCSI, 2:Entity ID 3:Portal IP/Port 4:Portal Index)>",
                 "(none)", sizeof (cmdLine)))
   {
      key_type=atoi(cmdLine);
      switch (key_type)
      {
         case 1:
               c_size =
                  GetInput (cmdLine, "iSCSI Node ID >", "(none)",
                            sizeof (cmdLine));
               if (c_size != 0)
               {
                  ISNSAppendAttr (&cmd, ISNS_ISCSI_NODE_ID,
                                  PAD4 (strlen (cmdLine)), cmdLine, 0);
               }
               break;
         case 2:
               c_size =
                  GetInput (cmdLine, "Entity ID >", "(none)", sizeof (cmdLine));
               if (c_size != 0)
               {
                  ISNSAppendAttr (&cmd, ISNS_ENTITY_ID, PAD4 (strlen (cmdLine)),
                                  cmdLine, 0);
               }
               break;
         case 3:
            c_size = GetInput (cmdLine, "Portal IP", NULL, sizeof (cmdLine));
            if (c_size != 0)
            {
               char tempb[256];
               int ip;
               int port;

               ip = inet_addr (cmdLine);
               memset (tempb, 0, sizeof (tempb));
               memcpy (tempb + 12, &ip, sizeof (ip));
               ISNSAppendAttr (&cmd, ISNS_PORTAL_IP, 16, tempb, 0);

               portal_type = 0;
               if (GetInput
                   (cmdLine, "Portal Type", "*0:UDP 1:TCP", sizeof (cmdLine)))
               {
                  portal_type = atoi (cmdLine);
               }

               c_size = GetInput (cmdLine, "Portal PORT", NULL, sizeof (cmdLine));
               if (c_size == 0)
               {
                  printf ("***ERROR: You must enter a Portal Port.");
                  return;
               }
               port = atoi (cmdLine);
               ISNSAppendAttr (&cmd, ISNS_PORTAL_PORT, 4, NULL,
                               port | (portal_type == 0 ? 0x10000 : 0));
            }
            break;
         case 4:
            c_size = GetInput (cmdLine, "Portal Index", "(none)", sizeof (cmdLine));
            if (c_size != 0)
            {
               ISNSAppendAttr (&cmd, ISNS_PORTAL_IDX, ISNS_INDEX_SIZE,
                               NULL, atoi(cmdLine));
            }
            break;
         default:
               ISNSAppendAttr (&cmd, ISNS_PORTAL_IP, 0, NULL, 0);
            break;
      }
   }
   else
      ISNSAppendAttr (&cmd, ISNS_PORTAL_IP, 0, NULL, 0);


   ISNSAppendAttr (&cmd, 0, 0, NULL, 0);

   ISNSAppendAttr (&cmd, ISNS_PORTAL_IP, 0, NULL, 0);
   ISNSAppendAttr (&cmd, ISNS_PORTAL_IDX, 0, NULL, 0);
   ISNSAppendAttr (&cmd, ISNS_PORTAL_PORT, 0, NULL, 0);
   ISNSAppendAttr (&cmd, ISNS_PORTAL_SYM_NAME, 0, NULL, 0);
   ISNSAppendAttr (&cmd, ISNS_ESI_PORT, 0, NULL, 0);
   ISNSAppendAttr (&cmd, ISNS_ESI_INTERVAL, 0, NULL, 0);
   ISNSAppendAttr (&cmd, ISNS_ENTITY_ID, 0, NULL, 0);
   ISNSAppendAttr (&cmd, ISNS_ENTITY_IDX, 0, NULL, 0);

   if (0 == ISNSSendCmd2 (&cmd, buffer, sizeof (buffer)))
   {
      ParsePortalQuery (buffer);
   }
}

/***********************************************************************/
/* Registers a portal */
/***********************************************************************/
void
RegPortalI (void)
{
   char cmdLine[256];
   char eid[256];
   char sym[256];
   int port;
   int ip;
   int esi_interval;
   int port_type;
   int portal_group;
   char b[256]={0};
   int flag;

   /* Insert Header */
   printf ("Registering Portal.\n");
   ISNSCreateHdr (ISNS_REG_DEV_ATTR_REQ, &cmd, sizeof (cmd), replaceFlag?ISNS_FLAG_REPLACE_REG:0);

   /* Get Entity ID */
   flag = FALSE;

   if (-1 ==GetSrc(&cmd,0))
     return;


   if (0  != GetInput (cmdLine, "Entity ID", "(none)", sizeof (cmdLine)))
   {
      memset (eid, 0, sizeof (eid));
      strcpy (eid, cmdLine);
      ISNSAppendAttr (&cmd, ISNS_ENTITY_ID, PAD4 (strlen (cmdLine)), cmdLine, 0);
      flag = TRUE;
   }

   if (!flag)
   {
      printf("Error: You must enter at least one Entity ID.\n");
      return;
   }

   /* Add Delimiter */
   ISNSAppendAttr (&cmd, 0, 0, NULL, 0);

   /* Get IP */
   ip = inet_addr (DEFAULT_IP_ADDR);
   if (0 !=
       GetInput (cmdLine, "Portal IP", DEFAULT_IP_ADDR, sizeof (cmdLine)))
   {
      ip = inet_addr (cmdLine);
   }

   {
      char tempb[256];
      memset (tempb, 0, sizeof (tempb));
      tempb[10] = tempb[11] = 0xff;
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
      memset (cmdLine, 0, sizeof (cmdLine));
      ISNSAppendAttr (&cmd, ISNS_PORTAL_GROUP_TAG, 4, NULL, portal_group);
   }

   /* Get Interval */
   esi_interval = 0;
   if (0 != GetInput (cmdLine, "ESI Interval", "0", sizeof (cmdLine)))
   {
      esi_interval = atoi (cmdLine);
      ISNSAppendAttr (&cmd, ISNS_ESI_INTERVAL, 4, NULL, esi_interval);

      /* Get ESI Port */
      port = 0;
      if (0 != GetInput (cmdLine, "ESI/SCN UDP Port", NULL, sizeof (cmdLine)))
      {
         port = atoi (cmdLine);
      }

      ISNSAppendAttr (&cmd, ISNS_ESI_PORT, 4, NULL, port);

      if (port && esi_interval)
      {
#ifndef SNS_LINUX
         DWORD junk;
#endif
         LISTENER_CB_P p_cb = NULL;
         HANDLE hdle;

#ifdef SNS_LINUX
         hdle = (void *)malloc (sizeof (pthread_t));
#else
         p_cb = malloc (sizeof (LISTENER_CB));
#endif

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

   ISNSSendCmd (&cmd);

}

/***********************************************************************/
/* Updates an existing portal */
/***********************************************************************/
void
UpdatePortal (void)
{
   char cmdLine[256];
   char sym[256]={0};
   int port;
   int ip;
   int esi_interval;
   int port_type;

   /* Insert Header */
   printf ("Updating Portal.\n");
   ISNSCreateHdr (ISNS_REG_DEV_ATTR_REQ, &cmd, sizeof (cmd),
                  0);

   /* Get IP */
   if (0 == GetInput (cmdLine, "Portal IP", NULL, sizeof (cmdLine)))
   {
      printf ("***ERROR: You must enter an IP address.\n");
      return;
   }

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
   if (0 == GetInput (cmdLine, "Portal Port", NULL, sizeof (cmdLine)))
   {
      printf ("***ERROR: You must enter a port.\n");
      return;
   }

   port = atoi (cmdLine);
   ISNSAppendAttr (&cmd, ISNS_PORTAL_PORT, 4, NULL,
                   port | (port_type == 1 ? 0 : 0x10000));

   /* Add Delimiter */
   ISNSAppendAttr (&cmd, 0, 0, NULL, 0);

   /* Get an Portal Sym Name */
   if (0 != GetInput (cmdLine, "Portal Sym Name", "(none)", sizeof (cmdLine)))
   {
      ISNSAppendAttr (&cmd, ISNS_PORTAL_SYM_NAME, PAD4 (strlen (cmdLine)),
                      cmdLine, 0);
      memset (sym, 0, sizeof (sym));
      strcpy (cmdLine, sym);
   }

   /* Get Interval */
   esi_interval = 0;
   if (0 != GetInput (cmdLine, "ESI Interval", NULL, sizeof (cmdLine)))
   {
      esi_interval = atoi (cmdLine);
      ISNSAppendAttr (&cmd, ISNS_ESI_INTERVAL, 4, NULL, esi_interval);

      /* Get ESI Port */
      port = 0;
      if (0 != GetInput (cmdLine, "ESI/SCN UDP Port", NULL, sizeof (cmdLine)))
      {
         port = atoi (cmdLine);
      }

      ISNSAppendAttr (&cmd, ISNS_ESI_PORT, 4, NULL, port);

      if (port && esi_interval)
      {
#ifndef SNS_LINUX
         DWORD junk;
#endif
         LISTENER_CB_P p_cb = NULL;
         HANDLE hdle;

#ifdef SNS_LINUX
         hdle = (void *)malloc (sizeof (pthread_t));
#else
         p_cb = malloc (sizeof (LISTENER_CB));
#endif

         memset (p_cb, 0, sizeof (LISTENER_CB));
         memset (p_cb->eid, 0, sizeof (p_cb->eid));
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

   ISNSSendCmd (&cmd);
}
