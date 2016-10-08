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

/*******************************************
  Command line support functions
*******************************************/

/*********************************************************************/
/* queryDDS */
/*********************************************************************/
void
queryDDS (char *id)
{
   char buffer[MAX_PAYLOAD_SIZE]={0};

   ISNSCreateHdr (ISNS_DEV_ATTR_QRY_REQ, &cmd, sizeof(cmd), 0);
   ISNSAppendAttr (&cmd, ISNS_ISCSI_NODE_ID, PAD4 (strlen (CONTROL_NODE)),CONTROL_NODE, 0);

   if (strlen(id))
   {
      int ddsid;
      ddsid = atoi(id);
      ISNSAppendAttr (&cmd, ISNS_DDS_ID, 4, NULL, ddsid);
      ISNSAppendAttr (&cmd, 0, 0, NULL, 0);
      ISNSAppendAttr (&cmd, ISNS_DDS_SYM_NAME, 0, NULL, 0);
      ISNSAppendAttr (&cmd, ISNS_DD_ID, 0, NULL, 0);
      ISNSAppendAttr (&cmd, ISNS_DD_SYM_NAME, 0, NULL, 0);
   }
   else
   {
     ISNSAppendAttr (&cmd, ISNS_DDS_ID, 0, NULL, 0);
     ISNSAppendAttr (&cmd, 0, 0, NULL, 0);
     ISNSAppendAttr (&cmd, ISNS_DDS_ID, 0, NULL, 0);
     ISNSAppendAttr (&cmd, ISNS_DDS_SYM_NAME, 0, NULL, 0);
     ISNSAppendAttr (&cmd, ISNS_DDS_STATUS, 0, NULL, 0);
     ISNSAppendAttr (&cmd, ISNS_DD_ID, 0, NULL, 0);
     ISNSAppendAttr (&cmd, ISNS_DD_SYM_NAME, 0, NULL, 0);
   }

   if (0 == ISNSSendCmd2 (&cmd, buffer, sizeof (buffer)))
      ParseDDSQuery (buffer);
}

/*********************************************************************/
/* queryDD */
/*********************************************************************/
void
queryDD (char *id)
{
   char buffer[MAX_PAYLOAD_SIZE]={0};

   ISNSCreateHdr (ISNS_DEV_ATTR_QRY_REQ, &cmd, sizeof(cmd), 0);
   ISNSAppendAttr (&cmd, ISNS_ISCSI_NODE_ID, PAD4 (strlen (CONTROL_NODE)),CONTROL_NODE, 0);

   if (strlen(id))
   {
     int ddid;
     ddid = atoi(id);
     ISNSAppendAttr (&cmd, ISNS_DD_ID, 4, NULL, ddid);
     ISNSAppendAttr (&cmd, 0, 0, NULL, 0);
     ISNSAppendAttr (&cmd, ISNS_DD_SYM_NAME, 0, NULL, 0);
     ISNSAppendAttr (&cmd, ISNS_DD_ISCSI_MEMBER, 0, NULL, 0);
     ISNSAppendAttr (&cmd, ISNS_DD_ISCSI_MEMBER_IDX, 0, NULL, 0);
   }
   else
   {
     ISNSAppendAttr (&cmd, ISNS_DD_ID, 0, NULL, 0);
     ISNSAppendAttr (&cmd, 0, 0, NULL, 0);
     ISNSAppendAttr (&cmd, ISNS_DD_ID, 0, NULL, 0);
     ISNSAppendAttr (&cmd, ISNS_DD_SYM_NAME, 0, NULL, 0);
     ISNSAppendAttr (&cmd, ISNS_DD_ISCSI_MEMBER, 0, NULL, 0);
     ISNSAppendAttr (&cmd, ISNS_DD_ISCSI_MEMBER_IDX, 0, NULL, 0);
  } 

   if (0 == ISNSSendCmd2 (&cmd, buffer, sizeof (buffer)))
      ParseDDQuery (buffer);
}

/*********************************************************************/
/* regDDS */
/*********************************************************************/
void
regDDS (char * sym)
{
   uint32_t status=0;
   extern int replaceFlag;

   ISNSCreateHdr (ISNS_REG_DDS_REQ, &cmd, sizeof (cmd), replaceFlag?ISNS_FLAG_REPLACE_REG:0);

   ISNSAppendAttr (&cmd, ISNS_ISCSI_NODE_ID, PAD4 (strlen (CONTROL_NODE)),CONTROL_NODE, 0);

   /* Delimiter */
   ISNSAppendAttr (&cmd, 0, 0, NULL, 0);
   ISNSAppendAttr (&cmd, ISNS_DDS_ID, 0, NULL, 0);
   ISNSAppendAttr (&cmd, ISNS_DDS_SYM_NAME, PAD4 (strlen (sym)), sym, 0);
   status = 0x1;
   ISNSAppendAttr (&cmd, ISNS_DDS_STATUS, 4, NULL, status);

   ISNSSendCmd (&cmd);
}

/*********************************************************************/
/* regDDSMember */
/*********************************************************************/
void
regDDSMember (char * ddsid, char * ddid )
{
   int dds_id;
   int dd_id;

   dds_id = atoi (ddsid);
   dd_id = atoi (ddid);

   ISNSCreateHdr (ISNS_REG_DDS_REQ, &cmd, sizeof (cmd), 0);
   ISNSAppendAttr (&cmd, ISNS_ISCSI_NODE_ID, PAD4 (strlen (CONTROL_NODE)),CONTROL_NODE, 0);
   ISNSAppendAttr (&cmd, ISNS_DDS_ID, 4, NULL, dds_id);
   ISNSAppendAttr (&cmd, 0, 0, NULL, 0);
   ISNSAppendAttr (&cmd, ISNS_DD_ID, 4, NULL, dd_id);

   ISNSSendCmd (&cmd);
}

/*********************************************************************/
/* regDDMember */
/*********************************************************************/
void
regDDMember (char * ddid, char * iscsi_node )
{
   int dd_id;

   dd_id = atoi (ddid);

   ISNSCreateHdr (ISNS_REG_DD_REQ, &cmd, sizeof (cmd), 0);
   ISNSAppendAttr (&cmd, ISNS_ISCSI_NODE_ID, PAD4 (strlen (CONTROL_NODE)),CONTROL_NODE, 0);
   ISNSAppendAttr (&cmd, ISNS_DD_ID, 4, NULL, dd_id);
   ISNSAppendAttr (&cmd, 0, 0, NULL, 0);

   ISNSAppendAttr (&cmd, ISNS_DD_ISCSI_MEMBER, PAD4 (strlen(iscsi_node)),
              iscsi_node, 0);

   ISNSSendCmd (&cmd);
}

/*********************************************************************/
/* regDD */
/*********************************************************************/
void
regDD (char * sym)
{
   extern int replaceFlag;

   ISNSCreateHdr (ISNS_REG_DD_REQ, &cmd, sizeof (cmd), replaceFlag?ISNS_FLAG_REPLACE_REG:0);

   ISNSAppendAttr (&cmd, ISNS_ISCSI_NODE_ID, PAD4 (strlen (CONTROL_NODE)),CONTROL_NODE, 0);

   /* Delimiter */
   ISNSAppendAttr (&cmd, 0, 0, NULL, 0);
   ISNSAppendAttr (&cmd, ISNS_DD_ID, 0, NULL, 0);
   ISNSAppendAttr (&cmd, ISNS_DD_SYM_NAME, PAD4 (strlen (sym)), sym, 0);

   ISNSSendCmd (&cmd);
}

/*********************************************************************/
/*********************************************************************/
void
regISCSI (char * sym, char *id)
{
   extern int replaceFlag;

   ISNSCreateHdr (ISNS_REG_DEV_ATTR_REQ, &cmd, sizeof (cmd), replaceFlag?ISNS_FLAG_REPLACE_REG:0);

   ISNSAppendAttr (&cmd, ISNS_ISCSI_NODE_ID, PAD4 (strlen (CONTROL_NODE)),CONTROL_NODE, 0);

   ISNSAppendAttr (&cmd, ISNS_ENTITY_ID, PAD4(strlen(id)),id,0);

   ISNSAppendAttr (&cmd, 0, 0, NULL, 0);
   ISNSAppendAttr (&cmd, ISNS_ISCSI_NODE_ID, PAD4(strlen(sym)),sym,0);
   ISNSAppendAttr (&cmd, ISNS_ISCSI_TYPE, 4, NULL, 1);

   ISNSSendCmd (&cmd);
}

/*********************************************************************/
/*********************************************************************/
void
regEntity (char * sym)
{
   extern int replaceFlag;
   int type;

   ISNSCreateHdr (ISNS_REG_DEV_ATTR_REQ, &cmd, sizeof (cmd), replaceFlag?ISNS_FLAG_REPLACE_REG:0);

   ISNSAppendAttr (&cmd, ISNS_ISCSI_NODE_ID, PAD4 (strlen (CONTROL_NODE)),CONTROL_NODE, 0);

   /* Delimiter */
   ISNSAppendAttr (&cmd, 0, 0, NULL, 0);
   ISNSAppendAttr (&cmd, ISNS_ENTITY_ID, PAD4 (strlen (sym)), sym, 0);
   type = 2;
   ISNSAppendAttr (&cmd, ISNS_ENTITY_TYPE, 4, NULL, type);

   ISNSSendCmd (&cmd);
}

/*********************************************************************/
/*********************************************************************/
void
regPortal (char * ip_address, char * id, char *port_value)
{
   extern int replaceFlag;
   int port;
   int portal_type = 1;
   int ip;
   char tempb[256];

   port = atoi(port_value);
printf("port_value:%s\n",port_value);

   ISNSCreateHdr (ISNS_REG_DEV_ATTR_REQ, &cmd, sizeof (cmd), replaceFlag?ISNS_FLAG_REPLACE_REG:0);

   ISNSAppendAttr (&cmd, ISNS_ISCSI_NODE_ID, PAD4 (strlen (CONTROL_NODE)),CONTROL_NODE, 0);

   ISNSAppendAttr (&cmd, ISNS_ENTITY_ID, PAD4(strlen(id)),id,0);

   ISNSAppendAttr (&cmd, 0, 0, NULL, 0);

printf("ipaddress:%s\n",ip_address);
   ip = inet_addr(ip_address);
   memset (tempb, 0, sizeof(tempb));
   tempb[10] = tempb[11] = 0xff;
   memcpy(tempb + 12, &ip, sizeof(ip));
   ISNSAppendAttr (&cmd, ISNS_PORTAL_IP, 16, tempb, 0);
   ISNSAppendAttr (&cmd, ISNS_PORTAL_PORT, 4, NULL, port | (portal_type == 0 ? 0x10000: 0));

   ISNSSendCmd (&cmd);
}

/*********************************************************************/
/* deregisters a DDS */
/*********************************************************************/
void
deregDDS (char *ddsid)
{
   int id;

   ISNSCreateHdr (ISNS_DEREG_DDS_REQ, &cmd, sizeof (cmd), 0);
   ISNSAppendAttr (&cmd, ISNS_ISCSI_NODE_ID, PAD4 (strlen (CONTROL_NODE)),CONTROL_NODE, 0);

   id = atoi (ddsid);
   ISNSAppendAttr (&cmd, ISNS_DDS_ID, 4, NULL, id);

   ISNSSendCmd (&cmd);
}

/*********************************************************************/
/* deregisters a DD */
/*********************************************************************/
void
deregDD (char *ddid)
{
   int id;

   ISNSCreateHdr (ISNS_DEREG_DD_REQ, &cmd, sizeof (cmd), 0);
   ISNSAppendAttr (&cmd, ISNS_ISCSI_NODE_ID, PAD4 (strlen (CONTROL_NODE)),CONTROL_NODE, 0);

   id = atoi (ddid);
   ISNSAppendAttr (&cmd, ISNS_DD_ID, 4, NULL, id);

   ISNSSendCmd (&cmd);
}

/*********************************************************************/
/*********************************************************************/
void
deregISCSI (char *iscsi)
{
   ISNSCreateHdr (ISNS_DEREG_DEV_REQ, &cmd, sizeof (cmd), 0);
   ISNSAppendAttr (&cmd, ISNS_ISCSI_NODE_ID, PAD4 (strlen (CONTROL_NODE)),CONTROL_NODE, 0);

   ISNSAppendAttr (&cmd, 0, 0, NULL, 0);
   ISNSAppendAttr (&cmd, ISNS_ISCSI_NODE_ID, PAD4(strlen(iscsi)),iscsi, 0);

   ISNSSendCmd (&cmd);
}

/*********************************************************************/
/*********************************************************************/
void
deregEntity (char *entity)
{
   ISNSCreateHdr (ISNS_DEREG_DEV_REQ, &cmd, sizeof (cmd), 0);
   ISNSAppendAttr (&cmd, ISNS_ISCSI_NODE_ID, PAD4 (strlen (CONTROL_NODE)),CONTROL_NODE, 0);

   ISNSAppendAttr (&cmd, 0, 0, NULL, 0);
   ISNSAppendAttr (&cmd, ISNS_ENTITY_ID, PAD4(strlen(entity)),entity, 0);

   ISNSSendCmd (&cmd);
}

/*********************************************************************/
/*********************************************************************/
void
deregPortal (char *ip_address, char * port_value)
{
   extern int replaceFlag;
   int port;
   int portal_type = 1;
   int ip;
   char tempb[256];

   port = atoi(port_value);

   ISNSCreateHdr (ISNS_DEREG_DEV_REQ, &cmd, sizeof (cmd), 0);
   ISNSAppendAttr (&cmd, ISNS_ISCSI_NODE_ID, PAD4 (strlen (CONTROL_NODE)),CONTROL_NODE, 0);
   ISNSAppendAttr (&cmd, 0, 0, NULL, 0);

   ip = inet_addr(ip_address);
   memset (tempb, 0, sizeof(tempb));
   tempb[10] = tempb[11] = 0xff;
   memcpy(tempb + 12, &ip, sizeof(ip));
   ISNSAppendAttr (&cmd, ISNS_PORTAL_IP, 16, tempb, 0);
   ISNSAppendAttr (&cmd, ISNS_PORTAL_PORT, 4, NULL, port | (portal_type == 0 ? 0x10000: 0));

   ISNSSendCmd (&cmd);
}

/*********************************************************************/
/* Removes a DD from a DDS */
/*********************************************************************/
void
deregDDSMember (char *ddsid, char *ddid)
{
   int dds_id;
   int dd_id;

   ISNSCreateHdr (ISNS_DEREG_DDS_REQ, &cmd, sizeof (cmd), 0);
   ISNSAppendAttr (&cmd, ISNS_ISCSI_NODE_ID, PAD4 (strlen (CONTROL_NODE)),CONTROL_NODE, 0);

   dds_id = atoi (ddsid);
   ISNSAppendAttr (&cmd, ISNS_DDS_ID, 4, NULL, dds_id);

   ISNSAppendAttr (&cmd, 0, 0, NULL, 0);
 
   dd_id = atoi (ddid);
   ISNSAppendAttr (&cmd, ISNS_DD_ID, 4, NULL, dd_id);

   ISNSSendCmd (&cmd);
}

/*********************************************************************/
/* Removes a DD from a DDS */
/*********************************************************************/
void
deregDDMember (char *ddid, char * iscsi_node)
{
   int dd_id;

   ISNSCreateHdr (ISNS_DEREG_DD_REQ, &cmd, sizeof (cmd), 0);
   ISNSAppendAttr (&cmd, ISNS_ISCSI_NODE_ID, PAD4 (strlen (CONTROL_NODE)),CONTROL_NODE, 0);

   dd_id = atoi (ddid);
   ISNSAppendAttr (&cmd, ISNS_DD_ID, 4, NULL, dd_id);

   ISNSAppendAttr (&cmd, 0, 0, NULL, 0);
 
   ISNSAppendAttr (&cmd, ISNS_DD_ISCSI_MEMBER, PAD4(strlen(iscsi_node)),
              iscsi_node, 0);

   ISNSSendCmd (&cmd);
}

/********************************************
This will query for iSCSI Node(s) attributes.
********************************************/
void
queryISCSI (char * id)
{
   char buffer[MAX_PAYLOAD_SIZE]={0};

   ISNSCreateHdr (ISNS_DEV_ATTR_QRY_REQ, &cmd, sizeof (cmd), 0);
   ISNSAppendAttr (&cmd, ISNS_ISCSI_NODE_ID, PAD4 (strlen (CONTROL_NODE)),CONTROL_NODE, 0);

   if (strlen(id) )
   {
     ISNSAppendAttr (&cmd, ISNS_ISCSI_IDX, ISNS_INDEX_SIZE, NULL, atoi(id));

     ISNSAppendAttr (&cmd, 0, 0, NULL, 0);
     ISNSAppendAttr (&cmd, ISNS_ISCSI_NODE_ID, 0, NULL, 0);
     ISNSAppendAttr (&cmd, ISNS_ISCSI_TYPE, 0, NULL, 0);
   }
   else
   {
     ISNSAppendAttr (&cmd, ISNS_ISCSI_NODE_ID, 0, NULL, 0);

     ISNSAppendAttr (&cmd, 0, 0, NULL, 0);
     ISNSAppendAttr (&cmd, ISNS_ISCSI_NODE_ID, 0, NULL, 0);
     ISNSAppendAttr (&cmd, ISNS_ISCSI_TYPE, 0, NULL, 0);
   }

   if (0 == ISNSSendCmd2 (&cmd, buffer, sizeof (buffer)))
   {
      ParseISCSIQuery (buffer);
   }
}

/********************************************
queryEntity
********************************************/
void
queryEntity (void)
{
   char buffer[MAX_PAYLOAD_SIZE]={0};

   ISNSCreateHdr (ISNS_DEV_ATTR_QRY_REQ, &cmd, sizeof (cmd), 0);
   ISNSAppendAttr (&cmd, ISNS_ISCSI_NODE_ID, PAD4 (strlen (CONTROL_NODE)),CONTROL_NODE, 0);
   ISNSAppendAttr (&cmd, ISNS_ENTITY_ID, 0, NULL, 0);
   ISNSAppendAttr (&cmd, 0, 0, NULL, 0);
   ISNSAppendAttr (&cmd, ISNS_ENTITY_ID, 0, NULL, 0);

   if (0 == ISNSSendCmd2 (&cmd, buffer, sizeof (buffer)))
   {
      ParseEntityQuery (buffer);
   }
}

/********************************************
queryPortal
********************************************/
void
queryPortal (void)
{
   char buffer[MAX_PAYLOAD_SIZE]={0};

   ISNSCreateHdr (ISNS_DEV_ATTR_QRY_REQ, &cmd, sizeof (cmd), 0);
   ISNSAppendAttr (&cmd, ISNS_ISCSI_NODE_ID, PAD4 (strlen (CONTROL_NODE)),CONTROL_NODE, 0);
   ISNSAppendAttr (&cmd, ISNS_PORTAL_IP, 0, NULL, 0);

   ISNSAppendAttr (&cmd, 0, 0, NULL, 0);
   ISNSAppendAttr (&cmd, ISNS_PORTAL_IP, 0, NULL, 0);
   ISNSAppendAttr (&cmd, ISNS_PORTAL_PORT, 0, NULL, 0);
   ISNSAppendAttr (&cmd, ISNS_ENTITY_ID, 0, NULL, 0);

   if (0 == ISNSSendCmd2 (&cmd, buffer, sizeof (buffer)))
   {
      ParsePortalQuery (buffer);
   }
}

/*********************************************************************
  commandLineDispatch
*********************************************************************/
void CommandLineDispatch (char function, char *command, char *id, char *id2, char *id3)
{
  
  switch (function)
  {
    case 'q':
        if(0 == strcmp(command,"dds"))
           queryDDS(id); 

        else if ( 0 == strcmp(command,"dd"))
           queryDD(id);

        else if ( 0 == strcmp(command,"iscsi"))
           queryISCSI(id);

        else if ( 0 == strcmp(command,"entity"))
           queryEntity();

        else if ( 0 == strcmp(command,"portal"))
           queryPortal();
 
        else
           printf("invalid query type\n");

    break;
    case 'r':
        if(0 == strcmp(command,"dds"))
           regDDS(id); 

        else if ( 0 == strcmp(command,"dd"))
           regDD(id);

        else if ( 0 == strcmp(command,"ddsmember"))
           regDDSMember(id,id2);

        else if ( 0 == strcmp(command,"ddmember"))
           regDDMember(id,id2);

        else if ( 0 == strcmp(command,"iscsi"))
           regISCSI(id,id2);

        else if ( 0 == strcmp(command,"entity"))
           regEntity(id);

        else if ( 0 == strcmp(command,"portal"))
           regPortal(id,id2,id3);

        else
           printf("invalid register type\n");

    break;
    case 'd':
        if(0 == strcmp(command,"dds"))
           deregDDS(id); 

        else if ( 0 == strcmp(command,"dd"))
           deregDD(id);

        else if ( 0 == strcmp(command,"ddsmember"))
           deregDDSMember (id,id2);

        else if ( 0 == strcmp(command,"ddmember"))
           deregDDMember (id,id2);

        else if ( 0 == strcmp(command,"iscsi"))
           deregISCSI(id);

        else if ( 0 == strcmp(command,"entity"))
           deregEntity(id);

        else if ( 0 == strcmp(command,"portal"))
           deregPortal(id,id2);

        else
           printf("invalid deregister type\n");

        break;
  }
}
