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

/*
 * This file contains source code for managing the responses back to clients.
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/basetype.h>
#include <sys/error.h>
#include <sys/list.h>
#include "iSNStypes.h"
#include "iSNS.h"
#include "iSNSmsg.h"
#include "iSNSdebug.h"

/*********************************************************************
_ISNSAppendAttr

Appends the attribute to the message in the TLV format.
*********************************************************************/
int
ISNSAppendAttr (ISNS_Msg * msg, int tag, int size, char *p_value, int value)
{
   ISNS_Attr *attr;

   if (msg == NULL)
   {
      __LOG_ERROR ("Message is NULL");
      return (ERROR);
   }

   /* add room for error code */
   if (msg->hdr.msg_len == 0)
   {
      msg->hdr.msg_len = 4;
      msg->hdr.msg_len += ISNS_SIZEOF_TAG;  // add delimiter
   }

   /* Jump to the end */
   attr = (ISNS_Attr *)((char *) &msg->payload + msg->hdr.msg_len);

   /* insert */
   attr->tag = tag;
   attr->len = size;

   if (p_value)
   {
      __ISNS_COPY (&attr->val, sizeof(attr->val), p_value, attr->len);
   }
   else if (attr->len == 8)
   {
      /* Special time format */
      attr->val.timestamp.t_pad = 0;
      attr->val.timestamp.t_time = htonl (value);
   }
   else
   {
      *(uint32_t *) & attr->val = value;
   }

   msg->hdr.msg_len += attr->len + ISNS_SIZEOF_TAG;
   attr = (ISNS_Attr *)((char *) attr + attr->len + ISNS_SIZEOF_TAG);

   return (ISNS_NO_ERR);
}

int
ISNSAppendTimeStamp ( ISNS_Msg *msg )
{
   ISNS_Attr *attr;
   time_t    t;

   if ( msg == NULL )
      return ( ERROR );

   /* Jump to end */
   attr = (ISNS_Attr *)((char *) &msg->payload + msg->hdr.msg_len);

   /* AppendTime */
   attr->tag = ISNS_TIMESTAMP;
   attr->len = 8;
   attr->val.timestamp.t_pad = 0;
   attr->val.timestamp.t_time = htonl (time(&t));
   msg->hdr.msg_len += attr->len + ISNS_SIZEOF_TAG;

   return (SUCCESS);
}

/*********************************************************************
_ISNSAppendKey

Appends the attribute to the message in the TLV format.
*********************************************************************/
int
ISNSAppendKey (ISNS_Msg * msg, int tag, int size, char *p_value, int value)
{
   ISNS_Attr *attr;

   if (msg == NULL)
      return (ERROR);

   if (msg->hdr.msg_len == 0 &&
       msg->hdr.type != ISNS_SCN && msg->hdr.type != ISNS_ESI && msg->hdr.type != ISNS_SCN_REG_REQ)
   {
      msg->hdr.msg_len = 4;     /* Error Code */
   }

   /* Jump to end */
   attr = (ISNS_Attr *)((char *) &msg->payload + msg->hdr.msg_len);

   /* insert the key(s) */
   attr->tag = tag;
   attr->len = size;

   if (p_value)
   {
      __ISNS_COPY (&attr->val, sizeof(attr->val), p_value, attr->len);
   }
   else if (attr->len == 8)
   {
      /* Special time format */
      attr->val.timestamp.t_pad = 0;
      attr->val.timestamp.t_time = htonl (value);
   }
   else
   {
      *(uint32_t *) & attr->val = value;
   }

   msg->hdr.msg_len += attr->len + ISNS_SIZEOF_TAG;

   return (ISNS_NO_ERR);
}

/*********************************************************************
Finds a tag in the response msg.
*********************************************************************/
ISNS_Attr *
ISNSFindAttrInResponseMsg ( int key, ISNS_Msg *msg )
{
   ISNS_Attr *attr;

   /* Find Delimiter */
   attr = (ISNS_Attr *)((char *) &msg->payload + 4);
   while ((char *)attr < (char *)&msg->payload + msg->hdr.msg_len)
   {
      if ( attr->tag == key )
         return ( attr );
      attr = (ISNS_Attr *)((char *) attr + attr->len + ISNS_SIZEOF_TAG);
   }

   return ( NULL );
}


/*********************************************************************
Finds a tag in the response msg.
*********************************************************************/
ISNS_Attr *
ISNSFindKeyInResponseMsg (int key, ISNS_Msg * msg)
{
   ISNS_Attr *attr;

   /* Find Delimiter */
   attr = (ISNS_Attr *)((char *) &msg->payload + 4);
   while ((char *)attr < (char *)&msg->payload + msg->hdr.msg_len
          && attr->tag != ISNS_DELIMITER)
   {
      if (attr->tag == key)
         return (attr);
      attr = (ISNS_Attr *)((char *) attr + attr->len + ISNS_SIZEOF_TAG);
   }

   return (NULL);
}

/*********************************************************************
_ISNSAppendTLV

Appends a TLV to a spot in memory at an offset.
*********************************************************************/
int
ISNSAppendTLV(void *ptr, int *offset, int tag,
              int len, char *p_value, int value)
{
   ISNS_Attr *attr;

   attr = (ISNS_Attr *)((char *)ptr+(*offset));

   /* insert */
   attr->tag = tag;
   attr->len = len;

   if (p_value)
   {
      __ISNS_COPY (&attr->val, sizeof(attr->val), p_value, attr->len);
   }
   else
   {
      *(uint32_t *) & attr->val = value;
   }
   *offset += ISNS_SIZEOF_TAG + attr->len;

   return (ISNS_NO_ERR);
}

