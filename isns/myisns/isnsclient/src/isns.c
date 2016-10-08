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


#define MAX_ESI_ENTRIES    (100)

static ISNS_CMD cmd;       /* Used by functions for msgs. */
int enableESIFlag;         /* Set to non-zero when enable ESI reponse. */

typedef struct _esi_table_entry
{
   char portal_name[256];
   int port;
   HANDLE t_hdle;
}
ESI_TBL_ENTRY;

/* ESI Table */
ESI_TBL_ENTRY esi_tble[MAX_ESI_ENTRIES];


/*********************************************************************/
/* Same as ISNSCreateHdr() but also zeros out the src field */
/*********************************************************************/
void
ISNSInsertQryHdr (ISNS_FUNC_ID func_id, ISNS_CMD * cmd)
{
   ISNSCreateHdr (func_id, cmd, sizeof(ISNS_CMD), 0);
   /* Insert a SRC ATTR of NULL */
   ISNSAppendAttr (cmd, 0, 0, NULL, 0);
}

/*********************************************************************/
/* Queries for DDS */
/*********************************************************************/
void
GetDDS (void)
{
   char buffer[MAX_PAYLOAD_SIZE];
   char cmdLine[256];
   int id;
   int key_type;

   printf ("Get DDS.\n");
   ISNSCreateHdr (ISNS_DEV_ATTR_QRY_REQ, &cmd, sizeof(cmd), 0);

   if (-1==GetSrc(&cmd, 0))
      return;

   /* Get Key */
   key_type = 0;
   if (0 !=
       GetInput (cmdLine, "Key Type", "*0:none 1:DDS ID 2:DD ID 3:iSCSI Node",
                 sizeof (cmdLine)))
   {
      key_type = atoi (cmdLine);
   }
   switch (key_type)
   {
   case 1:
      if (0 != GetInput (cmdLine, "DDS ID", NULL, sizeof (cmdLine)))
      {
         id = atoi (cmdLine);
         ISNSAppendAttr (&cmd, ISNS_DDS_ID, 4, NULL, id);
      }
      break;
   case 2:
      if (0 != GetInput (cmdLine, "DD ID", NULL, sizeof (cmdLine)))
      {
         id = atoi (cmdLine);
         ISNSAppendAttr (&cmd, ISNS_DD_ID, 4, NULL, id);
      }
      break;
   case 3:
      if (0 != GetInput (cmdLine, "iSCSI Node ID", NULL, sizeof (cmdLine)))
      {
         id = atoi (cmdLine);
         ISNSAppendAttr (&cmd, ISNS_ISCSI_NODE_ID, PAD4 (strlen (cmdLine)),
                         cmdLine, 0);
      }
      break;

   default:
         ISNSAppendAttr (&cmd, ISNS_DDS_ID, 0, NULL, 0);
      break;
   }

   /* Delimiter */
   ISNSAppendAttr (&cmd, 0, 0, NULL, 0);

   ISNSAppendAttr (&cmd, ISNS_DDS_ID, 0, NULL, 0);
   ISNSAppendAttr (&cmd, ISNS_DDS_SYM_NAME, 0, NULL, 0);
   ISNSAppendAttr (&cmd, ISNS_DDS_STATUS, 0, NULL, 0);
   ISNSAppendAttr (&cmd, ISNS_DD_ID, 0, NULL, 0);
   ISNSAppendAttr (&cmd, ISNS_DD_SYM_NAME, 0, NULL, 0);

   if (0 == ISNSSendCmd2 (&cmd, buffer, sizeof (buffer)))
      ParseDDSQuery (buffer);
}

/*********************************************************************/
/* Query for DD */
/*********************************************************************/
void
GetDD (void)
{
   char buffer[MAX_PAYLOAD_SIZE];
   char cmdLine[256];
   int id;
   int key_type;

   printf ("Get DD.\n");
   ISNSCreateHdr (ISNS_DEV_ATTR_QRY_REQ, &cmd, sizeof(cmd), 0);

   if (-1==GetSrc(&cmd, 0))
      return;

   /* Get Key */
   key_type = 0;
   if (0 !=
       GetInput (cmdLine, "Key Type", "*0:none 1:DDS ID 2:DD ID 3:iSCSI Node",
                 sizeof (cmdLine)))
   {
      key_type = atoi (cmdLine);
   }
   switch (key_type)
   {
   case 1:
      if (0 != GetInput (cmdLine, "DDS ID", NULL, sizeof (cmdLine)))
      {
         id = atoi (cmdLine);
         ISNSAppendAttr (&cmd, ISNS_DDS_ID, 4, NULL, id);
      }
      break;
   case 2:
      if (0 != GetInput (cmdLine, "DD ID", NULL, sizeof (cmdLine)))
      {
         id = atoi (cmdLine);
         ISNSAppendAttr (&cmd, ISNS_DD_ID, 4, NULL, id);
      }
      break;
   case 3:
      if (0 != GetInput (cmdLine, "iSCSI Node ID", NULL, sizeof (cmdLine)))
      {
         id = atoi (cmdLine);
         ISNSAppendAttr (&cmd, ISNS_ISCSI_NODE_ID, PAD4 (strlen (cmdLine)),
                         cmdLine, 0);
      }
      break;

   default:
         ISNSAppendAttr (&cmd, ISNS_DD_ID, 0, NULL, 0);
      break;
   }

   /* Delimiter */
   ISNSAppendAttr (&cmd, 0, 0, NULL, 0);

   ISNSAppendAttr (&cmd, ISNS_DD_ID, 0, NULL, 0);
   ISNSAppendAttr (&cmd, ISNS_DD_SYM_NAME, 0, NULL, 0);
   ISNSAppendAttr (&cmd, ISNS_DD_FEATURE_BITMAP, 0, NULL, 0);
   ISNSAppendAttr (&cmd, ISNS_DD_ISCSI_MEMBER, 0, NULL, 0);
   ISNSAppendAttr (&cmd, ISNS_DD_IFCP_MEMBER, 0, NULL, 0);
   ISNSAppendAttr (&cmd, ISNS_DD_ISCSI_MEMBER_IDX, 0, NULL, 0);
   ISNSAppendAttr (&cmd, ISNS_DDS_ID, 0, NULL, 0);
   ISNSAppendAttr (&cmd, ISNS_DDS_SYM_NAME, 0, NULL, 0);

   if (0 == ISNSSendCmd2 (&cmd, buffer, sizeof (buffer)))
      ParseDDQuery (buffer);
}

/*********************************************************************/
/* Registers DDS */
/*********************************************************************/
void
RegDDS (void)
{
   static int sym_integer=0;
   char buffer[256]={0};
   char sym[256]={0};
   char cmdLine[256]={0};
   uint32_t status=0;
   int id;
   extern int replaceFlag;

   printf ("Registering DDS.\n");
   ISNSCreateHdr (ISNS_REG_DDS_REQ, &cmd, sizeof (cmd), replaceFlag?ISNS_FLAG_REPLACE_REG:0);

   /* Src */
   if (-1==GetSrc(&cmd, 0))
      return;

   /* Delimiter */
   ISNSAppendAttr (&cmd, 0, 0, NULL, 0);

   do
   {
      id = 0;
      /* Get DDS */
      if (0 != GetInput (cmdLine, "DDS ID", NULL, sizeof (cmdLine)))
      {
         id = atoi (cmdLine);
         ISNSAppendAttr (&cmd, ISNS_DDS_ID, 4, NULL, id);
      }
      else
      {
         ISNSAppendAttr (&cmd, ISNS_DDS_ID, 0, NULL, 0);
      }

      /* Get Sym Name */
      memset (buffer, 0, sizeof (buffer));
      strcpy (buffer, "DDS Sym Name ");
      if (id)
         itoa (id, buffer + strlen (buffer), 10);
      else
         itoa (sym_integer++, buffer + strlen (buffer), 10);

      if (0 == GetInput (cmdLine, "DDS Sym Name", buffer, sizeof (cmdLine)))
      {
         strcpy (cmdLine, buffer);
      }

      memset(sym, 0, sizeof(sym));
      strcpy (sym, cmdLine);
      ISNSAppendAttr (&cmd, ISNS_DDS_SYM_NAME, PAD4 (strlen (sym)), sym, 0);

      /* Get Status */
      status=0;
      if (0 != GetInput (cmdLine, "Enable", "*0/1", sizeof (cmdLine)))
      {
         if (1 == atoi (cmdLine))
            status = 0x1;
      }

      ISNSAppendAttr (&cmd, ISNS_DDS_STATUS, 4, NULL, status);
   }
   while (GetYesNo (cmdLine, "More DDS?", FALSE, sizeof (cmdLine)));

   ISNSSendCmd (&cmd);
}

/*********************************************************************/
/* Updates a DDS */
/*********************************************************************/
void
UpdateDDS (void)
{
   char cmdLine[256];
   uint32_t status = 0;
   int id;

   printf ("Updating DDS.\n");
   ISNSCreateHdr (ISNS_REG_DDS_REQ, &cmd, sizeof (cmd), 0);

   if (-1==GetSrc(&cmd, 0))
      return;

   /* Get DDS */
   if (0 == GetInput (cmdLine, "DDS ID", NULL, sizeof (cmdLine)))
   {
      printf ("***ERROR: Required.\n");
      return;
   }
   id = atoi (cmdLine);
   ISNSAppendAttr (&cmd, ISNS_DDS_ID, 4, NULL, id);

   /* Delimiter */
   ISNSAppendAttr (&cmd, 0, 0, NULL, 0);

   /* Get Sym Name */
   if (0 != GetInput (cmdLine, "DDS Sym Name", NULL, sizeof (cmdLine)))
   {
      ISNSAppendAttr (&cmd, ISNS_DDS_SYM_NAME, PAD4 (strlen (cmdLine)),
                      cmdLine, 0);
   }

   /* Get Status */
   if (0 != GetInput (cmdLine, "Status Enable", "*0/1", sizeof (cmdLine)))
   {
      status = atoi (cmdLine) == 1 ? 0x1 : 0;
      ISNSAppendAttr (&cmd, ISNS_DDS_STATUS, 4, NULL, status);
   }

   ISNSSendCmd (&cmd);
}
/*********************************************************************/
/* Deregisters a DDS */
/*********************************************************************/
void
DeRegDDS (void)
{
   char cmdLine[256];
   int id;

   printf ("DeRegistering DDS.\n");
   ISNSCreateHdr (ISNS_DEREG_DDS_REQ, &cmd, sizeof (cmd), 0);

   /* Get SRC */
   if (-1 == GetSrc (&cmd, 0))
      return;

   /* Get DDS */
   if (GetInput(cmdLine, "DDS ID", NULL, sizeof(cmdLine)) == 0)
   {
      printf ("***ERROR: You must enter a DDS ID.\n");
      return;
   }

   id = atoi (cmdLine);
   ISNSAppendAttr (&cmd, ISNS_DDS_ID, 4, NULL, id);

   /* Delimiter */
/*   ISNSAppendAttr (&cmd, 0, 0, NULL, 0); */

   ISNSSendCmd (&cmd);
}

/*********************************************************************/
/* Removes a DD from a DDS */
/*********************************************************************/
void
Remove_DD_from_DDS (void)
{
   char cmdLine[256];
   int id;
   int c_size;
   int flag;

   printf ("DeRegister DD from DDS.\n");
   ISNSCreateHdr (ISNS_DEREG_DDS_REQ, &cmd, sizeof (cmd), 0);

   /* Insert SRC */
   if (-1 == GetSrc (&cmd, 0))
      return;

   /* Get DDS */
   if (GetInput(cmdLine, "DDS ID", NULL, sizeof(cmdLine)) == 0)
   {
      printf ("***ERROR: You must enter a DDS ID.\n");
      return;
   }

   id = atoi (cmdLine);
   ISNSAppendAttr (&cmd, ISNS_DDS_ID, 4, NULL, id);

   /* Delimiter */
   ISNSAppendAttr (&cmd, 0, 0, NULL, 0);

   flag = FALSE;
   do
   {
      c_size = GetInput (cmdLine, "DD ID", "(none)", sizeof (cmdLine));
      if (0 != c_size)
      {
         ISNSAppendAttr (&cmd, ISNS_DD_ID, 4, NULL, atoi (cmdLine));
         flag = TRUE;
      }
   }
   while (c_size);

   if (!flag)
   {
      printf ("***ERROR: You must enter at least 1 DD.\n");
      return;
   }

   ISNSSendCmd (&cmd);
}

/*********************************************************************/
/* Registers a DD */
/*********************************************************************/
void
RegDD (void)
{
   static int sym_integer=0;
   char buffer[MAX_PAYLOAD_SIZE];
   char cmdLine[256];
   int id;

   printf ("Registering DD.\n");
   ISNSCreateHdr (ISNS_REG_DD_REQ, &cmd, sizeof (cmd), 0);

   /* SRC */
   if (-1==GetSrc(&cmd, 0))
      return;

   /* Delimiter */
   ISNSAppendAttr (&cmd, 0, 0, NULL, 0);

   /* Get DD */
   do
   {
      id =0;
      if (0==GetInput(cmdLine, "DD ID", NULL, sizeof(cmdLine)))
      {
         ISNSAppendAttr (&cmd, ISNS_DD_ID, 0, NULL, 0);
      }
      else
      {
         id = atoi (cmdLine);
         ISNSAppendAttr (&cmd, ISNS_DD_ID, 4, NULL, id);
      }

      /* Get Sym Name */
      memset (buffer, 0, sizeof (cmdLine));
      strcpy (buffer, "DD Sym Name ");
      if (id)
         itoa (id, buffer + strlen (buffer), 10);
      else
         itoa (sym_integer++, buffer + strlen (buffer), 10);

      if (0 == GetInput (cmdLine, "DD Sym Name", buffer, sizeof (cmdLine)))
      {
         strcpy (cmdLine, buffer);
      }

      ISNSAppendAttr (&cmd, ISNS_DD_SYM_NAME, PAD4 (strlen (cmdLine)),
                      cmdLine, 0);

      if (0 != GetInput (cmdLine, "DD Feature Bitmap", NULL, sizeof (cmdLine)))
      {
         ISNSAppendAttr (&cmd, ISNS_DD_FEATURE_BITMAP, ISNS_DD_FEATURE_BITMAP_SIZE,
                         NULL, atoi(cmdLine));
      }
   }
   while (GetYesNo (cmdLine, "More DD?", FALSE, sizeof (cmdLine)));

   ISNSSendCmd (&cmd);
}
/*********************************************************************/
/* Updates a DD */
/*********************************************************************/
void
UpdateDD (void)
{
   char cmdLine[256];
   int id;

   printf ("Updating DD.\n");
   ISNSCreateHdr (ISNS_REG_DD_REQ, &cmd, sizeof (cmd), 0);

   if (-1==GetSrc(&cmd, 0))
      return;

   if (GetInput (cmdLine, "DD ID", NULL, sizeof (cmdLine)))
   {
      id = atoi (cmdLine);
      ISNSAppendAttr (&cmd, ISNS_DD_ID, 4, NULL, id);
   }
   else
   {
      printf ("***ERROR: Required.\n");
   }

   /* Delimiter */
   ISNSAppendAttr (&cmd, 0, 0, NULL, 0);

   /* Get Sym Name */
   if (GetInput (cmdLine, "DD Sym Name", NULL, sizeof (cmdLine)))
   {
      ISNSAppendAttr (&cmd, ISNS_DD_SYM_NAME, PAD4 (strlen (cmdLine)),
                      cmdLine, 0);
   }

   ISNSSendCmd (&cmd);
}
/*********************************************************************/
/* Deregisters a DD */
/*********************************************************************/
void
DeRegDD (void)
{
   char cmdLine[256];
   int id;

   printf ("DeReg DD.\n");
   ISNSCreateHdr (ISNS_DEREG_DD_REQ, &cmd, sizeof (cmd), 0);

   /* Insert SRC */
   if (-1 == GetSrc (&cmd, 0))
      return;

   if (0==GetInput(cmdLine, "DD ID", NULL, sizeof(cmdLine)))
   {
      printf ("***ERROR: You must enter a DD.\n");
      return;
   }

   id = atoi (cmdLine);
   ISNSAppendAttr (&cmd, ISNS_DD_ID, 4, NULL, id);

   /* Delimiter */
   ISNSAppendAttr (&cmd, 0, 0, NULL, 0);

   ISNSSendCmd (&cmd);
}
/*********************************************************************/
/* Removes a member from a DD */
/*********************************************************************/
void
Remove_Member_from_DD (void)
{
   char cmdLine[256];
   int id;
   int mem_type;
   int flag;
   int c_size;

   printf ("Deregister DD Member.\n");
   ISNSCreateHdr (ISNS_DEREG_DD_REQ, &cmd, sizeof (cmd), 0);

   /* Insert SRC */
   if (-1 == GetSrc (&cmd, 0))
      return;

   if (0==GetInput(cmdLine, "DD ID", NULL, sizeof(cmdLine)))
   {
      printf ("***ERROR: You must enter a DD.\n");
      return;
   }

   id = atoi (cmdLine);
   ISNSAppendAttr (&cmd, ISNS_DD_ID, 4, NULL, id);

   /* Delimiter */
   ISNSAppendAttr (&cmd, 0, 0, NULL, 0);

   /* Get Member */
   mem_type = 1;
   if (0 !=
       GetInput (cmdLine, "DD MEMBER type (1:iSCSI 2:iFCP)", "(none)",
                 sizeof (cmdLine)))
   {
      mem_type = atoi (cmdLine);
   }

   flag = FALSE;
   do
   {
      switch (mem_type)
      {
      case (1):
         c_size =
            GetInput (cmdLine, "iSCSI Node ID", "(none)", sizeof (cmdLine));
         if (c_size != 0)
         {
            ISNSAppendAttr (&cmd, ISNS_DD_ISCSI_MEMBER,
                            PAD4 (strlen (cmdLine)), cmdLine, 0);
            flag = TRUE;
         }
         break;
      case (2):
         c_size = GetInput (cmdLine, "iFCP WWPN", "(none)", sizeof (cmdLine));
         if (c_size != 0)
         {
            ISNSAppendAttr (&cmd, ISNS_DD_IFCP_MEMBER, 8, cmdLine, 0);
            flag = TRUE;
         }
         break;
      default:
         return;

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

/*********************************************************************/
/* Adds a DD to a DDS */
/*********************************************************************/
void
Add_DD_to_DDS (void)
{
   char cmdLine[256];
   int dd_id;
   int c_size;
   int dd_flag;

   printf ("Adding DD(s) to DDS.\n");
   ISNSCreateHdr (ISNS_REG_DDS_REQ, &cmd, sizeof (cmd), 0);

   /* SRC */
   if (-1 == GetSrc (&cmd, 0))
      return;

   ISNSAppendAttr (&cmd, 0, 0, NULL, 0);

   /* Get DDS ID */
   if (GetInput (cmdLine, "DDS ID", NULL, sizeof (cmdLine)) == 0)
   {
      printf ("***ERROR: You must enter a DDS ID.\n");
      return;
   }

   dd_id = atoi (cmdLine);
   ISNSAppendAttr (&cmd, ISNS_DDS_ID, 4, NULL, dd_id);

   /* Delimiter */
   ISNSAppendAttr (&cmd, 0, 0, NULL, 0);

   /* Get DD ID */
   dd_flag = FALSE;
   do
   {
      c_size = GetInput (cmdLine, "DD ID", NULL, sizeof (cmdLine));
      if (c_size)
      {
         dd_id = atoi (cmdLine);
         ISNSAppendAttr (&cmd, ISNS_DD_ID, 4, NULL, dd_id);
         dd_flag = TRUE;
      }
   }
   while (c_size != 0);

   if (!dd_flag)
   {
      printf ("***ERROR: You must enter at least one DD ID.\n");
      return;
   }

   ISNSSendCmd (&cmd);
}

/*********************************************************************/
/* Disable an ESI */
/*********************************************************************/
void
DisableESI (void)
{
   enableESIFlag = FALSE;
}

/*********************************************************************/
/* Enable an ESI */
/*********************************************************************/
void
EnableESI (void)
{
   enableESIFlag = TRUE;
}

/*********************************************************************/
/* Sends a SCN Event */
/*********************************************************************/
void
Send_SCN_Event (void)
{
   int scn_bitmap_type=0;
   int scn_bitmap=0;
   char cmdLine[256];
   int key_type;
   int c_size;

   printf ("Sending SCN Event.\n");
   ISNSCreateHdr (ISNS_SCN_EVENT, &cmd, sizeof (cmd), 0);

   /* Get SRC */
   if (-1 == GetSrc (&cmd, 0))
      return;

   key_type = 3;
   if (0 != GetInput (cmdLine, "Key type (1:iFCP, 2:Entity ID *3:iSCSI)",
                      "(none)", sizeof (cmdLine)))
   {
      key_type = atoi (cmdLine);
      if (key_type == 1)
      {
         c_size =
            GetHexInput (cmdLine, "iFCP Node WWPN", NULL, sizeof (cmdLine));
         if (c_size == 0)
         {
            printf ("***ERROR: You must enter a node id.\n");
            return;
         }

         ISNSAppendAttr (&cmd, ISNS_PORT_NAME, 8, cmdLine, 0);

         scn_bitmap_type = ISNS_ISCSI_SCN_BITMAP;
      }
//      else if (key_type == 2)
//      {
//         c_size =
//            GetInput (cmdLine, "Entity ID", "(none)", sizeof (cmdLine));
//         if (c_size == 0)
//         {
//            printf ("***ERROR: You must enter a node id.\n");
//            return;
//         }
//
//         ISNSAppendAttr (&cmd, ISNS_ENTITY_ID, PAD4 (strlen (cmdLine)),
//                         cmdLine, 0);
//
//         scn_bitmap_type = ISNS_ENTITY_SCN_BITMAP;
//      }
      else if (key_type == 3)
      {
         c_size =
            GetInput (cmdLine, "iSCSI Node ID", "(none)", sizeof (cmdLine));
         if (c_size == 0)
         {
            printf ("***ERROR: You must enter a node id.\n");
            return;
         }

         ISNSAppendAttr (&cmd, ISNS_ISCSI_NODE_ID, PAD4 (strlen (cmdLine)),
                         cmdLine, 0);

         scn_bitmap_type = ISNS_ISCSI_SCN_BITMAP;
      }
      else
      {
         printf ("***ERROR: Invalid key_type.\n");
         return;
      }
   }
   ISNSAppendAttr (&cmd, 0, 0, NULL, 0);

   scn_bitmap = 0x1F;
   c_size = GetInput (cmdLine, "SCN Bitmap", "(0x1F)", sizeof (cmdLine));
   scn_bitmap = atoi (cmdLine);

   ISNSAppendAttr (&cmd, scn_bitmap_type, 4, NULL, scn_bitmap);

   ISNSSendCmd (&cmd);
}

/*********************************************************************/
/* Sends an ESI response */
/*********************************************************************/
int
Send_ESI_Response (ISNS_CMD * p_cmd, int size)
{
   char rbuffer[MAX_PAYLOAD_SIZE];
   ISNS_CMD *p_rcmd;

   if (FALSE == enableESIFlag)
   {
      printf ("ESI Response is disabled.\n");
      return (0);
   }

   /* Verify Msg */
   if (ntohs (p_cmd->hdr.func_id) != ISNS_ESI)
   {
      printf ("***ERROR: Expected ESI, recv %#x.\n", p_cmd->hdr.func_id);
      return (-1);
   }

   /* Send response */
   memset (rbuffer, 0, sizeof (rbuffer));
   ISNSCreateHdr (ISNS_ESI_RSP, (struct cmd *)rbuffer, sizeof (rbuffer), 0);
   p_rcmd = (struct cmd *)rbuffer;
   p_rcmd->hdr.len = ntohs (p_cmd->hdr.len) + 4;
   p_rcmd->hdr.xid = p_cmd->hdr.xid;
   memcpy ((char *) rbuffer + sizeof (ISNS_HDR) + 4,
           (char *) p_cmd + sizeof (ISNS_HDR), ntohs (p_cmd->hdr.len));

   ParseESI ((char *)p_cmd);

   printf ("Rsp --->\n");
   ISNSJustSendCmd ((struct cmd *)rbuffer);

   return (0);
}

/*********************************************************************/
/* Process a SCN and sends a response */
/*********************************************************************/
void
Process_SCN (ISNS_CMD * p_msg, int size)
{
   ISNS_Attr *p_attr;
   char *ptr;

//   printf ("RCV SCN.\n");
   DumpHex (p_msg, size);
   ptr = (char *) p_msg + sizeof (ISNS_HDR);
   printf ("---------------------------------\n");
   while (ptr < (char *) p_msg + sizeof (ISNS_HDR) + ntohs (p_msg->hdr.len))
   {
      p_attr = (struct ISNS_attr *)ptr;
      p_attr->tag = ntohl (p_attr->tag);
      p_attr->len = ntohl (p_attr->len);
      switch (p_attr->tag)
      {
      case ISNS_PORT_NAME:
         {
            char buffer2[256] = { 0 };
            memset (buffer2, 0, sizeof (buffer2));
            printf ("iFCP WWPN  : %s\n",
                    HexToAscii ((char *)&p_attr->val, p_attr->len, buffer2));
            break;
         }
      case ISNS_ENTITY_ID:
      case ISNS_ISCSI_NODE_ID:
         printf ("iSCSI ID  : %s\n",(char *)&p_attr->val);
         break;
      case ISNS_TIMESTAMP:
      {
         time_t time_value;
         time_value = ntohl (p_attr->val.timestamp.t_time);
         printf ("TimeStamp: %s", ctime (&time_value));
         break;
      }
      case ISNS_IFCP_SCN_BITMAP:
      case ISNS_ISCSI_SCN_BITMAP:
         p_attr->val.etype=ntohl(p_attr->val.etype);
         printf ("Bitmap: %#x.\n", p_attr->val.etype);
         break;
      default:
         break;
      }
      ptr = (char *) ptr + p_attr->len + 8;
   }


   /* Send a response */
   {
      ISNSCreateHdr(ISNS_SCN_RES, &cmd, sizeof(cmd), 0);

      /* Append Dest Addr */
      p_attr = (struct ISNS_attr *)((char *) p_msg + sizeof (ISNS_HDR));

      /* Insert error code */
      cmd.hdr.len=4;

      cmd.hdr.xid=p_msg->hdr.xid;

      ISNSAppendAttr(&cmd, p_attr->tag, p_attr->len, (char *)&p_attr->val, 0);

      printf("Sending SCN rsp-->\n");
      ISNSJustSendCmd ((struct cmd *)&cmd);
   }

}

/*********************************************************************/
/* Registers a SCN */
/*********************************************************************/
int
RegSCN (void)
{
   char cmdLine[256];
   int src_type;
   int scn_bitmap;

   /* Register SCN */
   printf ("Register SCN.\n");

   ISNSCreateHdr (ISNS_SCN_REG_REQ, &cmd, sizeof (cmd), 0);

   if (-1 == GetSrc (&cmd, 0))
      return (-1);

   /* Src Type */
   if (0 == GetInput (cmdLine, "Node type (1:iSCSI, 2:iFCP 3:Entity ID)>",
                      "(req)", sizeof (cmdLine)))
   {
      printf ("***ERROR: You must enter a node type.\n");
      return (-1);
   }

   src_type = atoi (cmdLine);
   scn_bitmap = 0x1F;
   if (src_type == 1)
   {
      if (0 ==
          GetInput (cmdLine, "KEY: iSCSI Node ID", "(none)",
                    sizeof (cmdLine)))
      {
         return (-1);
      }

      ISNSAppendAttr (&cmd, ISNS_ISCSI_NODE_ID, PAD4 (strlen (cmdLine)),
                      cmdLine, 0);

      if (0 !=
          GetInput (cmdLine, "Event Bitmap (dec)", "0x1F", sizeof (cmdLine)))
      {
         scn_bitmap = atoi (cmdLine);
      }

      ISNSAppendAttr (&cmd, 0, 0, NULL, 0);

      ISNSAppendAttr (&cmd, ISNS_ISCSI_SCN_BITMAP, 4, NULL, scn_bitmap);
   }
   else if (src_type == 2)
   {
      if (0 ==
          GetHexInput (cmdLine, "KEY: iFCP WWPN", NULL, sizeof (cmdLine)))
      {
         return (-1);
      }

      ISNSAppendAttr (&cmd, ISNS_PORT_NAME, 8, cmdLine, 0);

      if (0 !=
          GetInput (cmdLine, "Event Bitmap (int)", "0x1F", sizeof (cmdLine)))
      {
         scn_bitmap = atoi (cmdLine);
      }

      ISNSAppendAttr (&cmd, 0, 0, NULL, 0);

      ISNSAppendAttr (&cmd, ISNS_IFCP_SCN_BITMAP, 4, NULL, scn_bitmap);
   }
   else if (src_type == 3)
   {
//    if (0 ==
//        GetInput (cmdLine, "KEY: Entity ID", "(none)",
//                  sizeof (cmdLine)))
//    {
//       return (-1);
//    }
//
//    ISNSAppendAttr (&cmd, ISNS_ENTITY_ID, PAD4 (strlen (cmdLine)), cmdLine,
//                    0);
//
//    if (0 !=
//        GetInput (cmdLine, "Event Bitmap (int)", "0x1F", sizeof (cmdLine)))
//    {
//       scn_bitmap = atoi (cmdLine);
//    }
//
//    ISNSAppendAttr (&cmd, 0, 0, NULL, 0);
//
//    ISNSAppendAttr (&cmd, ISNS_ENTITY_SCN_BITMAP, 4, NULL, scn_bitmap);

   }
   else
   {
      printf ("***ERROR: Invalid Node type.\n");
      return (-1);
   }

   ISNSSendCmd (&cmd);
   return (0);
}
/*********************************************************************/
/* Deregisters a SCN */
/*********************************************************************/
void
DeRegSCN (void)
{
   char cmdLine[256];
   int src_type;

   /* Register SCN */
   printf ("DeRegister SCN.\n");

   ISNSCreateHdr (ISNS_SCN_DEREG_REQ, &cmd, sizeof (cmd), 0);


   /* Src Type */
   if (0 == GetInput (cmdLine, "SRC type (1:iSCSI, 2:iFCP 3:Entity ID)>",
                      "(none)", sizeof (cmdLine)))
   {
      printf ("***ERROR: You must enter a SRC type.\n");
      return;
   }

   src_type = atoi (cmdLine);
   if (src_type == 1)
   {
      if (0 ==
          GetInput (cmdLine, "SRC: iSCSI Node ID", "(none)",
                    sizeof (cmdLine)))
      {
         return;
      }

      ISNSAppendAttr (&cmd, ISNS_ISCSI_NODE_ID, PAD4 (strlen (cmdLine)),
                      cmdLine, 0);

      if (0 ==
          GetInput (cmdLine, "KEY: iSCSI Node ID", "(none)",
                    sizeof (cmdLine)))
      {
         return;
      }

      ISNSAppendAttr (&cmd, ISNS_ISCSI_NODE_ID, PAD4 (strlen (cmdLine)),
                      cmdLine, 0);

   }
   else if (src_type == 2)
   {
      if (0 ==
          GetInput (cmdLine, "SRC: iFCP WWPN", "(none)", sizeof (cmdLine)))
      {
         return;
      }

      ISNSAppendAttr (&cmd, ISNS_PORT_NAME, 8, cmdLine, 0);

      if (0 ==
          GetInput (cmdLine, "KEY: iFCP WWPN", "(none)", sizeof (cmdLine)))
      {
         return;
      }

      ISNSAppendAttr (&cmd, ISNS_PORT_NAME, 8, cmdLine, 0);

   }
   else if (src_type == 3)
   {
      if (0 ==
          GetInput (cmdLine, "SRC: Entity ID", "(none)",
                    sizeof (cmdLine)))
      {
         return;
      }

      ISNSAppendAttr (&cmd, ISNS_ENTITY_ID, PAD4 (strlen (cmdLine)), cmdLine,
                      0);

      if (0 ==
          GetInput (cmdLine, "KEY: Entity ID", "(none)",
                    sizeof (cmdLine)))
      {
         return;
      }

      ISNSAppendAttr (&cmd, ISNS_ENTITY_ID, PAD4 (strlen (cmdLine)), cmdLine,
                      0);

   }
   else
   {
      printf ("***ERROR: Invalid src type.\n");
      return;
   }

   ISNSSendCmd (&cmd);
}

/*********************************************************************/
/* Initializes the ESI Table.  This table contains the ESI thread
   handles and other relevant information. */
/*********************************************************************/
void
InitESITable (void)
{
   memset (esi_tble, 0, sizeof (esi_tble));
}

/*********************************************************************/
/* Adds an ESI Entry to the table */
/*********************************************************************/
void
AddESIPortal (char *p_name, int port, HANDLE hdle)
{
   int i;
   for (i = 0; i < MAX_ESI_ENTRIES; i++)
   {
      if (esi_tble[i].t_hdle == 0)
      {
         strcpy (esi_tble[i].portal_name, p_name);
         esi_tble[i].port = port;
         esi_tble[i].t_hdle = hdle;
         break;
      }
   }
}

/*********************************************************************/
/* Kills all ESI Threads */
/*********************************************************************/
void
KillESIThreads (void)
{
   int i;
   printf ("Disabling ALL ESI reponses.\n");
   for (i = 0; i < MAX_ESI_ENTRIES; i++)
   {
      if (esi_tble[i].t_hdle != 0)
      {
#ifdef SNS_LINUX
         pthread_cancel(*(esi_tble[i].t_hdle));
         free(esi_tble[i].t_hdle);
         esi_tble[i].t_hdle = 0;
#else
         TerminateThread (esi_tble[i].t_hdle, -1);
#endif
      }
   }
}

/*********************************************************************/
/* Implements a Get Next */
/*********************************************************************/
void
GetNext(void)
{
   static int currentTag=0;
   static int currentLen=0;
   static char currentValue[256]={0};

   static int currentTag2=0;
   static int currentLen2=0;
   static char currentValue2[256]={0};

   char buffer[MAX_PAYLOAD_SIZE]={0};

   char cmdLine[256]={0};
   int type=0;

   /* Register SCN */
   printf ("Get Next Attr.\n");

   ISNSCreateHdr (ISNS_DEV_GET_NXT_REQ, &cmd, sizeof (cmd), 0);

   if (-1==GetSrc(&cmd, 0))
      return;

   /* Enter Type */
   if (0!=GetInput(cmdLine, "Object type", "*0:Get Next 1:iSCSI ID 2:Entity ID\
3:iFCP 4:Portal IP/PORT 5:iSCSI Idx 6:Entity Idx \
7:Portal Idx 8:DD ID 9:DDS ID", sizeof(cmdLine)))
   {
      type=atoi(cmdLine);
   }

   switch (type)
   {
      case (0):
         if (currentTag==0)
         {
            printf("Object Type needed for first iteration.\n");
            return;
         }
         else
         {
            ISNSAppendAttr(&cmd, currentTag, currentLen, currentValue, 0);
            if (currentTag==ISNS_PORTAL_IP || currentTag==ISNS_PORTAL_PORT)
            {
               ISNSAppendAttr(&cmd, currentTag2, currentLen2, currentValue2, 0);
            }
         }
         break;
      case (1):
         if (0 == GetInput (cmdLine, "KEY: iSCSI Node Id", "(none)",
                       sizeof (cmdLine)))
         {
            ISNSAppendAttr (&cmd, ISNS_ISCSI_NODE_ID, 0,
                            NULL, 0);
            ISNSAppendAttr (&cmd, 0, 0, NULL, 0);
            ISNSAppendAttr (&cmd, ISNS_ISCSI_ALIAS, 0, NULL, 0);
         }
         else
         {
            ISNSAppendAttr (&cmd, ISNS_ISCSI_NODE_ID, PAD4 (strlen (cmdLine)),
                         cmdLine, 0);
         }

         break;
      case (2):
         if (0 == GetInput (cmdLine, "KEY: Entity ID", "(none)", sizeof (cmdLine)))
         {
            ISNSAppendAttr (&cmd, ISNS_ENTITY_ID, 0,
                            NULL, 0);
         }
         else
         {
            ISNSAppendAttr (&cmd, ISNS_ENTITY_ID, PAD4 (strlen (cmdLine)), 
                        cmdLine, 0);
         }
         break;
      case (3):
         if (0 == GetHexInput (cmdLine, "KEY: iFCP Node WWPN", NULL,
                          sizeof (cmdLine)))
         {
            ISNSAppendAttr (&cmd, ISNS_PORT_NAME, 0,
                            NULL, 0);
            ISNSAppendAttr (&cmd, 0, 0, NULL, 0);
            ISNSAppendAttr (&cmd, ISNS_PORT_SYM_NAME, 0, NULL, 0);
         }
         else
         {
            ISNSAppendAttr (&cmd, ISNS_PORT_NAME, 8, cmdLine, 0);
         }
         break;
      case (4):
         /* Get IP */
         if (0 != GetInput (cmdLine, "Portal IP", NULL, sizeof (cmdLine)))
         {
            int port;
            int port_type;
            int ip;

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
         }
         else
         {
            ISNSAppendAttr (&cmd, ISNS_PORTAL_IP, 0, NULL, 0);
            ISNSAppendAttr (&cmd, ISNS_PORTAL_PORT, 0, NULL, 0);
            ISNSAppendAttr (&cmd, 0, 0, NULL, 0);
            ISNSAppendAttr (&cmd, ISNS_PORTAL_SYM_NAME, 0, NULL, 0);
         }
         break;
      case 5:
         if (0 == GetInput (cmdLine, "KEY: iSCSI Index", "(none)",
                       sizeof (cmdLine)))
         {
            ISNSAppendAttr (&cmd, ISNS_ISCSI_IDX, 0,
                            NULL, 0);
         }
         else
         {
            ISNSAppendAttr (&cmd, ISNS_ISCSI_IDX, ISNS_INDEX_SIZE,
                         NULL, atoi(cmdLine));
         }
         ISNSAppendAttr (&cmd, 0, 0, NULL, 0);
         ISNSAppendAttr (&cmd, ISNS_ISCSI_ALIAS, 0, NULL, 0);
         break;
      case 6:
         if (0 == GetInput (cmdLine, "KEY: Entity Index", "(none)", sizeof (cmdLine)))
         {
            ISNSAppendAttr (&cmd, ISNS_ENTITY_IDX, 0,
                            NULL, 0);
         }
         else
         {
            ISNSAppendAttr (&cmd, ISNS_ENTITY_IDX, ISNS_INDEX_SIZE, 
                        NULL, atoi(cmdLine));
         }
         break;
      case 7:
         if (0 != GetInput (cmdLine, "Key: Portal Index", NULL, sizeof (cmdLine)))
         {
            ISNSAppendAttr (&cmd, ISNS_PORTAL_IDX, ISNS_INDEX_SIZE, NULL, atoi(cmdLine));
         }
         else
         {
            ISNSAppendAttr (&cmd, ISNS_PORTAL_IDX, ISNS_INDEX_SIZE, NULL, 0);
         }
            ISNSAppendAttr (&cmd, 0, 0, NULL, 0);
            ISNSAppendAttr (&cmd, ISNS_PORTAL_SYM_NAME, 0, NULL, 0);
         break;
      case 8:
         if (0 != GetInput (cmdLine, "Key: DD ID", NULL, sizeof (cmdLine)))
         {
            ISNSAppendAttr (&cmd, ISNS_DD_ID, ISNS_INDEX_SIZE, NULL, atoi(cmdLine));
         }
         else
         {
            ISNSAppendAttr (&cmd, ISNS_DD_ID, 0, NULL, 0);
         }

         ISNSAppendAttr (&cmd, 0, 0, NULL, 0);
         ISNSAppendAttr (&cmd, ISNS_DD_ID, 0, NULL, 0);
         break;
      case 9:
         if (0 != GetInput (cmdLine, "Key: DDS ID", NULL, sizeof (cmdLine)))
         {
            ISNSAppendAttr (&cmd, ISNS_DDS_ID, ISNS_INDEX_SIZE, NULL, atoi(cmdLine));
         }
         else
         {
            ISNSAppendAttr (&cmd, ISNS_DDS_ID, 0, NULL, 0);
         }

         ISNSAppendAttr (&cmd, 0, 0, NULL, 0);
         ISNSAppendAttr (&cmd, ISNS_DDS_ID, 0, NULL, 0);
         break;
      default:
         printf("Invalid type.\n");
         return;
         break;
   }

   /* Call ParseGetNext */
   if (0 == ISNSSendCmd2 (&cmd, buffer, sizeof (buffer)))
   {
      ISNS_Attr *p_attr;

      p_attr=(struct ISNS_attr *)((char *)buffer+sizeof(ISNS_HDR)+4);

      memset(currentValue, 0, sizeof(currentValue));
      memcpy(currentValue, &p_attr->val, ntohl(p_attr->len));
      currentTag=ntohl(p_attr->tag);
      currentLen=ntohl(p_attr->len);

      if (currentTag==ISNS_PORTAL_IP || currentTag==ISNS_PORTAL_PORT)
      {
         p_attr=(struct ISNS_attr *)((char *)p_attr + 8 + ntohl(p_attr->len));
         memset(currentValue2, 0, sizeof(currentValue2));
         memcpy(currentValue2, &p_attr->val, ntohl(p_attr->len));
         currentTag2=ntohl(p_attr->tag);
         currentLen2=ntohl(p_attr->len);
      }
   }
   else
   {
      currentTag=0;
   }
}




