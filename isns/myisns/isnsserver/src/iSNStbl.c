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
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/basetype.h>
#include <sys/error.h>
#include "iSNStypes.h"
#include "iSNStbl.h"
#include "iSNSdebug.h"

static SCN_ENTRY *scnTbl[MAX_SCN_ENTRIES];
static ESI_ENTRY *esiTbl[MAX_ESI_ENTRIES];


static uint32_t nESI;
static uint32_t nSCN;


static uint32_t curIndex;

int sns_tbl_debug=0xFF;

void InitAllTables(void)
{
   InitESITable();
   InitSCNTable();
}

void InitESITable(void)
{
   memset(esiTbl, 0, sizeof(esiTbl));
}

void InitSCNTable(void)
{
    memset(scnTbl, 0, sizeof(scnTbl));
    nSCN = 0;
}

int
UpdateESIEntry(char *p_ip, int port, int sockfd, 
               char *id, int interval)
{
   int i;

   if (interval==0)
   {
      /* Remove Portal Entry */
      i=FindESIEntry(p_ip, (uint32_t)port);
      if (i==-1)
      {
         return (SUCCESS);
      }

      free(esiTbl[i]);
      esiTbl[i]=NULL;
      nESI--;

   }
   else
   {
      /* Check for dup */
      i=FindESIEntry(p_ip, (uint32_t)port);
      if (i==-1)
      {
         /* Find Empty Spot */
         for (i=0; i<MAX_ENTRIES; i++)
         {
            if (!(esiTbl[i]))
            {
               /* Create a new entry */
               esiTbl[i]=malloc(sizeof(ESI_ENTRY));
               memset(esiTbl[i], 0, sizeof(ESI_ENTRY));

               strcpy(esiTbl[i]->entity_id, id);
               esiTbl[i]->port=port;

               esiTbl[i]->sockfd=sockfd;

               memcpy(esiTbl[i]->ip_ptr, p_ip, ISNS_IP_SIZE);

               break;
            }
         }
      }

      nESI++;
   }
   return (SUCCESS);
}

int FindESIEntry(char *p_ip, uint32_t port)
{
   int i=-1;

   for (i=0; i<MAX_ESI_ENTRIES; i++)
   {
       
      if (esiTbl[i] && 0==memcmp(p_ip, esiTbl[i]->ip_ptr, ISNS_IP_SIZE)
         && esiTbl[i]->port==port)
      {
         return (i);
      }
   }

   return (-1);
}

void GetFirstESIEntry(ESI_ENTRY **p_esi)
{
   if (nESI==0)
   {
      *p_esi=NULL;
      return;
   }

   for (curIndex=0; curIndex<MAX_ESI_ENTRIES; curIndex++)
   {
      if (esiTbl[curIndex])
      {
         *p_esi=esiTbl[curIndex];
         break;
      }
   }
}

void GetNextESIEntry(ESI_ENTRY **p_esi)
{
   *p_esi=NULL;
   curIndex++;
   for (; curIndex<MAX_ESI_ENTRIES; curIndex++)
   {
      if (esiTbl[curIndex])
      {
         *p_esi=esiTbl[curIndex];
         break;
      }
   }
}

int
FindSCNEntry(int type, char *p_name)
{
   int i;
   for (i=0; i<MAX_SCN_ENTRIES; i++)
   {
      switch (type)
      {
         case ISNS_ENTITY_ID:
         case ISNS_ISCSI_NODE_ID:
            if (scnTbl[i] && scnTbl[i]->type==type 
               && 0==strcmp(scnTbl[i]->name, p_name))
            {
               return (i);
            }
            break;
         case ISNS_PORT_NAME:
            if (scnTbl[i] && scnTbl[i]->type==type 
               && 0==memcmp(scnTbl[i]->name, p_name, ISNS_PORT_NAME_SIZE))
            {
               return (i);
            }
         default:
            return (-1);
      }
   }

   return (-1);
}

int
AddSCNEntry(int type, char *p_name)
{
   int i;
   /* Check to see if it is already in Table */
   if (FindSCNEntry(type, p_name)!=-1)
   {
      DEBUG_0(sns_tbl_debug & 1, (SCN already in table));
      return (SUCCESS);
   }

   /* Find Empty slot */
   for (i=0; i<MAX_SCN_ENTRIES; i++)
   {
      if (!(scnTbl[i]))
      {
         switch (type)
         {
            case ISNS_ISCSI_NODE_ID:
            case ISNS_ENTITY_ID:               
               scnTbl[i]=malloc(sizeof(SCN_ENTRY));
               memset(scnTbl[i], 0, sizeof(SCN_ENTRY));
               scnTbl[i]->type=type;
               strcpy(scnTbl[i]->name, p_name);
               nSCN++;
               break;
            case ISNS_PORT_NAME:
               scnTbl[i]=malloc(sizeof(SCN_ENTRY));
               memset(scnTbl[i], 0, sizeof(SCN_ENTRY));
               scnTbl[i]->type=type;
               isns_copy (scnTbl[i]->name, sizeof(scnTbl[i]->name, p_name, ISNS_PORT_NAME_SIZE);
               nSCN++;
               break;
            default:
               DEBUG_1(sns_tbl_debug & 1, (Unknown type %d), type);
               return (ERROR);
               break;
         }
         break;
      }
   }

   return (SUCCESS);
}

int
RemoveSCNEntry(int type, char *p_name)
{
   int i;
   /* Check to see if it is already in the Table */
   if (FindSCNEntry(type, p_name)==-1)
   {
      return (SUCCESS);
   }

   /* Find Empty slot */
   for (i=0; i<MAX_SCN_ENTRIES; i++)
   {
      if (scnTbl[i] && scnTbl[i]->type==type)
      {
         if (type==ISNS_ENTITY_ID || type==ISNS_ISCSI_NODE_ID)
         {
            if (0==strcmp(scnTbl[i]->name, p_name))
            {
               free(scnTbl[i]);
               scnTbl[i]=NULL;
               nSCN--;
               return(SUCCESS);
            }
         } else if (type==ISNS_PORT_NAME)
         {
            if (0==memcmp(scnTbl[i]->name, p_name, ISNS_PORT_NAME))
            {
               free(scnTbl[i]);
               scnTbl[i]=NULL;
               nSCN--;
               return(SUCCESS);
            }
         }
         else
         {
            DEBUG_1(sns_tbl_debug & 1, (Unknown type %d), type);
            return (ERROR);
         }
     }
   }

   DEBUG_0(sns_tbl_debug & 1, (Unable to find entry));
   return (ERROR);
}

void GetFirstSCNEntry(char **id, int type)
{
   if (nSCN==0)
   {
      *id=NULL;
      return;
   }

   for (curIndex=0; curIndex<MAX_SCN_ENTRIES; curIndex++)
   {
      if (scnTbl[curIndex] && scnTbl[curIndex]->type==type)
      {
         *id=scnTbl[curIndex]->name;
         break;
      }
   }
}

void GetNextSCNEntry(char **id, int type)
{
   *id=NULL;
   curIndex++;
   for (; curIndex<MAX_SCN_ENTRIES; curIndex++)
   {
      if (scnTbl[curIndex] && scnTbl[curIndex]->type==type)
      {
         *id=scnTbl[curIndex]->name;
         break;
      }
   }
}
