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
#include "util.h"
#include "parse.h"

#ifndef HEX_DUMP_BYTES_PER_LINE
#define HEX_DUMP_BYTES_PER_LINE         (16)
#endif

int hexDumpFlag;
int parserFlag=FALSE;

/***************************************************
_DumpHex: Prints out the contents of a ptr in hex.
****************************************************/
void
DumpHex (void *data, int size)
{
   int count = 0;
   int count2 = 0;
   char ascii[HEX_DUMP_BYTES_PER_LINE + 1];

   if (hexDumpFlag)
   {
      while (count < size)
      {
         printf ("%2.2x ", *((unsigned char *) data + count));

         if ((*((char *) data + count) >= 0x20)
             && (*((char *) data + count) <= 0x7E))
         {
            ascii[count2] = *((char *) data + count);
         }
         else
         {
            ascii[count2] = 0x2E;
         }

         count++;
         count2++;
         if ((count % HEX_DUMP_BYTES_PER_LINE) == 0)
         {
            ascii[HEX_DUMP_BYTES_PER_LINE] = '\0';
            printf ("*%s*\n", ascii);
            count2 = 0;
         }
      }
      ascii[count2] = '\0';
      if ((count % HEX_DUMP_BYTES_PER_LINE) != 0)
      {
         while ((count % HEX_DUMP_BYTES_PER_LINE) != 0)
         {
            count++;
            printf ("   ");
         }
         printf ("*%s", ascii);
         while (count2 < HEX_DUMP_BYTES_PER_LINE)
         {
            count2++;
            printf (" ");
         }
         printf ("*\n");
      }

      printf ("\n");
   }

   if (parserFlag)
      ParsePDU(data);
}

/***********************************************************************/
/* Pauses until a key is pressed. */
/***********************************************************************/
void
WaitForKey (void)
{
   printf ("Press ENTER to continue.\n");
   getchar ();
}

/***********************************************************************/
/* Converts hex to ascii */
/***********************************************************************/
char *
HexToAscii (char *srcbuffer, int size, char *dstbuffer)
{
   unsigned char *ptr;
   int count;
   unsigned int two;

   ptr = (unsigned char *)srcbuffer;
   count = 0;
   while (ptr < (unsigned char *) srcbuffer + size)
   {
      two = *(unsigned char *) ptr;
      if (two < 0x10)
      {
         dstbuffer[count] = '0';
         itoa (two, &dstbuffer[count + 1], 16);
      }
      else
      {
         itoa (two, &dstbuffer[count], 16);
      }
      ptr = ptr + 1;
      count = count + 2;
   }
   return dstbuffer;
}

/***********************************************************************/
/* Converts the time to hex-ascii */
/***********************************************************************/
void
GetHexTime (char *buffer)
{
   char cstr[256];
   char two[3];
   time_t t;

   memset (cstr, 0, sizeof (cstr));
   time (&t);
   strcpy (cstr, ctime (&t) + 11);
   printf ("Time: %s", ctime (&t) + 11);

   memset (two, 0, sizeof (two));
   memcpy (two, cstr, 2);
   sscanf (two, "%x", (int *)&buffer[0]);

   memset (two, 0, sizeof (two));
   memcpy (two, cstr + 3, 2);
   sscanf (two, "%x", (int *)&buffer[1]);

   memset (two, 0, sizeof (two));
   memcpy (two, cstr + 6, 2);
   sscanf (two, "%x", (int *)&buffer[2]);

   memset (two, 0, sizeof (two));
   memcpy (two, cstr + 9, 2);
   sscanf (two, "%x", (int *)&buffer[3]);

   memset (two, 0, sizeof (two));
   memcpy (two, cstr + 12, 2);
   sscanf (two, "%x", (int *)&buffer[4]);
}

/***********************************************************************/
/* Prompts for input.  The input should be in ascii-hex.  Stores it in
   hex. */
/***********************************************************************/
int
GetHexInput (char *cmdLine, char *prompt, char *cDefault, int c_size)
{
   char buffer[256];
   char buffer2[8];
   char two[3];
   char *ptr;
   int count;
   char *sptr;
   char *result;

   memset (buffer, 0, sizeof (buffer));
   memset (buffer2, 0, sizeof (buffer2));
   memset (cmdLine, 0, c_size);
   printf ("%s [%s] >", prompt,
           cDefault != NULL ? HexToAscii (cDefault, 8, buffer2) : "(none)");
   result = fgets (buffer, sizeof(buffer), stdin);
   sptr = strtok(buffer, TOKENS);
   ptr = sptr;
   count = 0;
   if (ptr)
   {
       while (*ptr)
           {
           memset (two, 0, sizeof (two));
           memcpy (two, ptr, 2);
           sscanf (two, "%x", (int *)&cmdLine[count]);
           count++;
           ptr = (char *) ptr + 2;
       }
   }
   
   return count;
}

/***********************************************************************/
/* Prompts for an ascii-input */
/***********************************************************************/
int
GetInput (char *cmdLine, char *prompt, char *cDefault, int c_size)
{
   int rval;
   char *sptr;
   char *result;

   memset (cmdLine, 0, c_size);
   printf ("%s [%s] >", prompt, cDefault != NULL ? cDefault : "(none)");
   result = fgets (cmdLine, c_size, stdin);
   sptr = strtok(cmdLine, TOKENS);
   rval = sptr?strlen (sptr):0;

   return (rval);
}

/***********************************************************************/
/* Prompts for yes/no.  Returns TRUE if yes. */
/***********************************************************************/
int
GetYesNo (char *cmdLine, char *prompt, int dvalue, int c_size)
{
   int rval;
   char *sptr;
   char *result;

   memset (cmdLine, 0, c_size);
   printf ("%s [%s] >", prompt, dvalue == TRUE ? "*y/n" : "*n/y");
   result = fgets (cmdLine, c_size, stdin);
   sptr = strtok(cmdLine, TOKENS);

   if (sptr == NULL || strlen (sptr) == 0)
      rval = dvalue;
   else if ((0 == strcmp (cmdLine, "Yes")) ||
            (0 == strcmp (cmdLine, "y")) ||
            (0 == strcmp (cmdLine, "yes")) || (0 == strcmp (cmdLine, "YES")))
      rval = TRUE;
   else
      rval = FALSE;

   return (rval);
}

/***********************************************************************/
/* Prints the commnd and returns input. */
/***********************************************************************/
int
GetCommand (void)
{
   char buffer[256]={0};
   int item;
   GetInput(buffer, "Please select an option", "0 for Help", sizeof(buffer));

   item = atoi (buffer);
   return (item);
}

/*********************************************************************/
/* Get the =SRC field */
/*********************************************************************/
int
GetSrc (ISNS_CMD * p_cmd, int flags)
{
   char cmdLine[256];
   int src_type;

   src_type = 0;
   if (0 !=  GetInput (cmdLine,"SRC type",
                 "*1:Control Node 2:iSCSI Node 3:iFCP Node", sizeof (cmdLine)))
   {
      src_type = atoi (cmdLine);
   }

   switch (src_type)
   {
   case 0:
      ISNSAppendAttr (p_cmd, ISNS_ISCSI_NODE_ID, PAD4 (strlen (CONTROL_NODE)),CONTROL_NODE, 0);
      break;     

   case 1:
      if (0 ==  GetInput (cmdLine, "SRC: Control Node Id", "(none)",sizeof (cmdLine)))
      {
         printf ("***ERROR: You must enter an Node Id.\n");
         return (-1);
      }

      ISNSAppendAttr (p_cmd, ISNS_ISCSI_NODE_ID, PAD4 (strlen (cmdLine)),cmdLine, 0);

      break;

   case 2:
      if (0 == GetInput (cmdLine, "SRC: iSCSI Node Id", "(none)", sizeof (cmdLine)))
      {
         printf ("***ERROR: You must enter an iSCSI Node Id.\n");
         return (-1);
      }

      ISNSAppendAttr (p_cmd, ISNS_ISCSI_NODE_ID, PAD4 (strlen (cmdLine)),cmdLine, 0);

      break;

   case 3:
      if (0 ==  GetHexInput (cmdLine, "SRC: iFCP Node WWPN", NULL, sizeof (cmdLine)))
      {
         printf ("***ERROR: You must enter an iFCP Node WWPN.\n");
         return (-1);
      }
      ISNSAppendAttr (p_cmd, ISNS_PORT_NAME, 8, cmdLine, 0);

      break;
   default:
      return (-1);
   }

   return (0);
}


