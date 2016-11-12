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

/* Modeled after gdbm functions */

/* Protection for multiple includes. */
#ifndef _NDB_H_
#define _NDB_H_

typedef struct {
    char  *dptr;
    int   dsize;
} datum;

/* The file information header. This is good enough for most applications. */
typedef struct {int dummy[10];} *NDB_FILE;

typedef int ndb_error;      /* For compatibilities sake. */


/* Determine if the C(++) compiler requires complete function prototype  */
#ifndef __P
#if defined(__STDC__) || defined(__cplusplus) || defined(c_plusplus)
#define __P(x) x
#else
#define __P(x) ()
#endif
#endif

extern NDB_FILE ndb_open __P((char *, int, int, int, void (*)()));
extern void ndb_close __P((NDB_FILE));
extern int ndb_store __P((NDB_FILE, datum, datum, int));
extern datum ndb_fetch __P((uint32_t key_type, datum));
extern int ndb_delete __P((uint32_t, datum));
extern datum ndb_firstkey __P((uint32_t key_type));
extern datum ndb_nextkey __P((uint32_t key_type, datum));

extern datum ndb_fetch_sns ();
extern datum ndb_firstkey_isns (uint32_t key_type, char *key);
extern datum ndb_nextkey_isns (uint32_t key_type, datum key, char *pkey);

#endif
