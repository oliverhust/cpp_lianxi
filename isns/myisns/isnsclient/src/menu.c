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

#include "iscsi.h"
#include "portal.h"
#include "entity.h"
#include "ifcp.h"
#include "fcnode.h"
#include "util.h"
#include "comm.h"
#include "menu.h"
#include "custom.h"

extern int hexDumpFlag;
extern int parserFlag;

void HexDumpToggle(void);
void ParserToggle(void);
void ReplaceSet(void);

/* Number of menu options */
#define MENU_SIZE             (48)
int replaceFlag = TRUE;
/***********************************************************************/
/* These are all the menu options.  The 1st field is the option name.
   2nd is a unique ID, 3rd is the function call. */
/***********************************************************************/
MENU_ITEM menu[MENU_SIZE] = {
   {"Register an Entity",        1,       (void *)RegEntityI},
   {"Register a Portal",         2,       RegPortalI},
   {"Register iSCSI Node",       3,       RegisterISCSI_I},
   {"Register iFCP Node",        4,       RegIFCP},
   {"Register FC Node",          5,       RegFCNode},
   {"Register DDS",              6,       RegDDS},
   {"Register DD",               7,       RegDD},

   {"Query for Entities",        10,      GetEntities},
   {"Query for Portals",         11,      GetPortals},
   {"Query for iSCSI Node",      12,      GetISCSI},
   {"Query for iFCP Node",       13,      GetIFCP},
   {"Query for FC Node",         14,      GetFCNode},
   {"Query for DDS",             15,      GetDDS},
   {"Query for DD",              16,      GetDD},
   {"Get Next Attr",             17,      GetNext},

   {"DeRegister an Entity",      20,      DeRegisterEntity},
   {"DeRegister a Portal",       21,      DeRegisterPortal},
   {"DeRegister an iSCSI Node",  22,      DeRegisterISCSI},
   {"DeRegister iFCP Node(s)",   23,      DeRegisterPort},
   {"DeRegister FC Node(s)",     24,      DeRegisterNode},
   {"DeRegister a DDS",          25,      DeRegDDS},
   {"DeRegister a DD",           26,      DeRegDD},

   {"Disable All ESI Response",  30,      DisableESI},
   {"Enable  All ESI Response",  31,      EnableESI},

   {"Add DD to DDS",             60,      Add_DD_to_DDS},
   {"Add ISCSI Node to DD",      61,      Add_ISCSI_to_DD},
   {"Add IFCP Node to DD",       62,      Add_IFCP_to_DD},
   {"Remove member from DD",     63,      Remove_Member_from_DD},
   {"Remove DD from DDS",        64,      Remove_DD_from_DDS},

   {"Register SCN",              70,      (void *)RegSCN},
   {"DeRegister SCN",            71,      DeRegSCN},
   {"Send SCN Event",            72,      Send_SCN_Event},

   {"Update Entity",             80,      (void *)UpdateEntity},
   {"Update Portal",             81,      UpdatePortal},
   {"Update iSCSI Node",         82,      UpdateISCSI},
   {"Update iFCP Node",          83,      UpdateIFCP},
   {"Update FC Node",            84,      UpdateFCNode},
   {"Update DDS",                85,      UpdateDDS},
   {"Update DD",                 86,      UpdateDD},

   {"Multi-Attr iSCSI Registration", 90,  MRegISCSI},
   {"Multi-Attr iFCP Registration", 91,   MRegIFCP},

   {"Custom Register 1",         100,     RegCustom1},
   {"Custom Register 2",         101,     RegCustom2},
   {"Custom Register 3",         102,     RegCustom3},

   {"Toggle Hex Dump On/Off",      300,     HexDumpToggle},
   {"Toggle PDU Parser On/Off",    301,     ParserToggle},
   {"Enable/Disable REPLACE FLAG", 302,     ReplaceSet}
};

void ReplaceSet(void)
{
   char cmdLine[ 256 ];
   if ( GetYesNo(cmdLine, "Enable REPLACE flag", 
        replaceFlag, sizeof(cmdLine)))
   {
      replaceFlag = TRUE;
   }
   else
   {
      replaceFlag = FALSE;
   }

   printf ("Replace bit is %s!\n", replaceFlag?"ENABLED":"DISABLED");
}

void HexDumpToggle(void)
{
   hexDumpFlag = hexDumpFlag?FALSE:TRUE;
   printf("Hex dump is now %s.\n", hexDumpFlag?"ON":"OFF");
}

void ParserToggle(void)
{
   parserFlag = parserFlag?FALSE:TRUE;
   printf("PDU Parser is now %s.\n", parserFlag?"ON":"OFF");
}

/***********************************************************************/
/* Displays the menu */
/***********************************************************************/
void
DisplayMenu (void)
{
   int i;
   printf ("Item\t Menu Name\n");
   printf ("----\t ---------\n");
   printf ("-1)\t exit.\n");
   printf ("0)\t Display Menu\n");
   for (i = 0; i < MENU_SIZE; i++)
   {
      printf ("%d)\t %s.\n", menu[i].id, menu[i].name);
   }
}

/***********************************************************************/
/* Prompt the user for a menu selection and calls the menu function. */
/***********************************************************************/
void
Interact (void)
{
   int i;
   int selection;
   DisplayMenu ();

   while (1)
   {
      selection = GetCommand ();
      if (selection == 0)
      {
         DisplayMenu ();
      }
      else if (selection == -1)
        break;
      else
      {
         for (i = 0; i < MENU_SIZE; i++)
         {
            if (selection == menu[i].id)
            {
               if (menu[i].func == NULL)
               {
                  printf ("Selection not implemented.\n");
                  break;
               }
               menu[i].func ();
               break;
            }
         }
      }
   }
}



