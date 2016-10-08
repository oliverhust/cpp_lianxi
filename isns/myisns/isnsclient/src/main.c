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

#ifdef SNS_LINUX
#include <pthread.h>
#else
#include "getarg.h"
#endif

#include "isns.h"
#include "iscsi.h"
#include "portal.h"
#include "entity.h"
#include "ifcp.h"
#include "fcnode.h"
#include "util.h"
#include "comm.h"
#include "menu.h"

extern void CommandLineDispatch (char , char *, char *, char *,char *);

/***************************************
 Globals
 **************************************/
char p_ip[256];
extern int isns_port;
extern int enableESIFlag;
extern char multicast_addr[20];

/********************************
 Locals
********************************/
static char query[100]="";
static char reg[100]="";
static char dereg[100]="";
static char idvalue[100]="";
static char idvaluemember[256]="";
static char id3[256]="";

/*****************
   Usage Message
*****************/
char Usage[] = "\
Usage: isnsadm [options] \n\
Options:\n\
 -h               Listen for L2 heartbeat from an iSNS.\n\
 -?               Help.\n\
\n\
 -a ip            iSNS Address.\n\
 -p port          Port used for sending. Default is port 3205.\n\
 -q type          Query\n\
                      type: dds\n\
                      type: dd\n\
                      type: iscsi\n\
                      type: entity\n\
                      type: portal\n\
 -r type -n name -m member -v portvalue \n\
                  Register\n\
                      type:dds        name:id\n\
                      type:dd         name:id\n\
                      type:ddsmember  name:ddsid member:ddid\n\
                      type:ddmember   name:ddid  member:iscsinode\n\
                      type:entity     name:id\n\
                      type:iscsi      name:iqnvalue member:entityid\n\
                      type:portal     name:ipvalue  member:entityid -v port\n\
 -d type -n name -m member\n\
                  Deregister\n\
                      type:dds        name=index\n\
                      type:dd         name=index\n\
                      type:ddsmember  name=ddsid  member:ddid\n\
                      type:ddmember   name:ddid   member:iscsinode\n\
                      type:entity     name:id\n\
                      type:iscsi      name:iqnvalue\n\
                      type:portal     name:ipvalue member:port\n\
\n\
";

/***********************************************************************/
int
main (int argc, char **argv)
{
   int l3_hb_flag;
   int hb_flag;
   int c;
   int tcpFlag;
   memset (p_ip, 0, sizeof (p_ip));

   printf ("IETF iSNS Open Source Client, v%s.\n", ISNS_VERSION);

   if (argc < 1)
   {
      fprintf (stderr, Usage);
      exit (0);
   }

   /* init ESI */
   InitESITable ();

   /* Default Values */
   isns_port = ISNS_SERVER_PORT;
   //tcpFlag = FALSE;
   tcpFlag = TRUE;
   enableESIFlag = TRUE;
   hb_flag = FALSE;
   l3_hb_flag = FALSE;

   /* Parse Command Line arguments */
   while (optind != argc)
   {
      c = getopt (argc, argv, "htd:p:a:m:q:r:n:e:v:");

      switch (c)
      {
      case EOF:
         optarg = argv[optind];
         optind++;
         break;
      case 't':
         tcpFlag = TRUE;
         break;

      case 'p':
         isns_port = atoi (optarg);
         break;

      case 'a':
         strcpy (p_ip, optarg);
         break;

      case 'h':
         hb_flag=TRUE;
         break;

      case 'c':
         l3_hb_flag=TRUE;
         strcpy (multicast_addr, optarg);
         break;

      case 'q':
     strcpy (query, optarg);
     break;

      case 'r':
     strcpy (reg, optarg);
     break;

      case 'd':
     strcpy (dereg, optarg);
     break;

      case 'n':
     strcpy (idvalue, optarg);
     break;

      case 'm':
     strcpy (idvaluemember, optarg);
     break;

      case 'v':
     strcpy (id3, optarg);
     break;

      case '?':
      default:
         fprintf (stderr, Usage);
         exit (0);
      }
   }

   if (0 == strlen (p_ip) && hb_flag==FALSE && l3_hb_flag==FALSE)
   {
      fprintf (stderr, Usage);
      exit (0);
   }

   /* Init the Communications sockets */
   if (-1==InitComm(hb_flag, l3_hb_flag, tcpFlag))
      exit(-1);

   if (strcmp(query,""))
      CommandLineDispatch ('q',query,idvalue,idvaluemember,id3);
   else if (strcmp(reg,""))
   {
      if ( 0 == strlen(idvalue))
        printf ("-n parameter required\n");
      else
        CommandLineDispatch ('r',reg,idvalue,idvaluemember,id3);
   }
   else if (strcmp(dereg,""))
   {
      if ( 0 == strlen(idvalue))
        printf ("-n parameter required\n");
      else
        CommandLineDispatch ('d',dereg,idvalue,idvaluemember,id3);
   }

   return (0);
}



