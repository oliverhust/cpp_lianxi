  /* Copyright (C) 1996-2002, 2003, 2004 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, write to the Free
   Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
   02111-1307 USA.  */

/* All data returned by the network data base library are supplied in
   host order and returned in network order (suitable for use in
   system calls).  */

#ifndef _NETDB_H
#define _NETDB_H 1

#include <features.h>

#ifndef _SYS_BASETYPE_H_
#include <sys/basetype.h>
#endif
#ifndef _SYS_ERROR_H_
#include <sys/error.h>
#endif
#ifndef _SYS_IN_H_
#include <sys/in.h>
#endif

#include <stdint.h> 
#if defined __USE_MISC && defined __UCLIBC_HAS_RPC__ 
/* This is necessary to make this include file properly replace the
   Sun version.  */
# include <rpc/netdb.h>
#endif

#ifdef __USE_GNU
# define __need_sigevent_t
# include <bits/siginfo.h>
# define __need_timespec
# include <time.h>
#endif

#include <bits/netdb.h>

/* Absolute file name for network data base files.  */
#define _PATH_HEQUIV    "/etc/hosts.equiv"
#define _PATH_HOSTS    "/etc/hosts"
#define _PATH_NETWORKS    "/etc/networks"
#define _PATH_NSSWITCH_CONF "/etc/nsswitch.conf"
#define _PATH_PROTOCOLS    "/etc/protocols"
#define _PATH_SERVICES    "/etc/services"


__BEGIN_DECLS

#ifdef __USE_XOPEN2K
/* Highest reserved Internet port number.  */
# define IPPORT_RESERVED	1024
#endif

#ifdef __USE_GNU
/* Scope delimiter for getaddrinfo(), getnameinfo().  */
# define SCOPE_DELIMITER	'%'
#endif

#if defined __USE_MISC || defined __USE_GNU
/* Print error indicated by `h_errno' variable on standard error.  STR
   if non-null is printed before the error string.  */
extern void herror (__const char *__str) __THROW;
#endif

/* Open host data base files and mark them as staying open even after
   a later search if STAY_OPEN is non-zero.

   This function is a possible cancellation point and therefore not
   marked with __THROW.  */
extern void sethostent (int __stay_open);

/* Close host data base files and clear `stay open' flag.

   This function is a possible cancellation point and therefore not
   marked with __THROW.  */
extern void endhostent (void);

/* Get next entry from host data base file.  Open data base if
   necessary.

   This function is a possible cancellation point and therefore not
   marked with __THROW.  */
extern struct hostent *gethostent (void);

#ifdef __USE_MISC
/* Reentrant versions of the functions above.  The additional
   arguments specify a buffer of BUFLEN starting at BUF.  The last
   argument is a pointer to a variable which gets the value which
   would be stored in the global variable `herrno' by the
   non-reentrant functions.

   These functions are not part of POSIX and therefore no official
   cancellation point.  But due to similarity with an POSIX interface
   or due to the implementation they are cancellation points and
   therefore not marked with __THROW.  */
extern int gethostent_r (struct hostent *__restrict __result_buf,
             char *__restrict __buf, size_t __buflen,
             struct hostent **__restrict __result,
             int *__restrict __h_errnop);
#endif	/* misc */


/* Open network data base files and mark them as staying open even
   after a later search if STAY_OPEN is non-zero.

   This function is a possible cancellation point and therefore not
   marked with __THROW.  */
extern void setnetent (int __stay_open);

/* Close network data base files and clear `stay open' flag.

   This function is a possible cancellation point and therefore not
   marked with __THROW.  */
extern void endnetent (void);

/* Get next entry from network data base file.  Open data base if
   necessary.

   This function is a possible cancellation point and therefore not
   marked with __THROW.  */
extern struct netent *getnetent (void);

/* Return entry from network data base which address match NET and
   type TYPE.

   This function is a possible cancellation point and therefore not
   marked with __THROW.  */
extern struct netent *getnetbyaddr (uint32_t __net, int __type);

/* Return entry from network data base for network with NAME.

   This function is a possible cancellation point and therefore not
   marked with __THROW.  */
extern struct netent *getnetbyname (__const char *__name);

#ifdef __USE_MISC
/* Reentrant versions of the functions above.  The additional
   arguments specify a buffer of BUFLEN starting at BUF.  The last
   argument is a pointer to a variable which gets the value which
   would be stored in the global variable `herrno' by the
   non-reentrant functions.

   These functions are not part of POSIX and therefore no official
   cancellation point.  But due to similarity with an POSIX interface
   or due to the implementation they are cancellation points and
   therefore not marked with __THROW.  */
extern int getnetent_r (struct netent *__restrict __result_buf,
         char *__restrict __buf, size_t __buflen,
         struct netent **__restrict __result,
         int *__restrict __h_errnop);

extern int getnetbyaddr_r (uint32_t __net, int __type,
               struct netent *__restrict __result_buf,
               char *__restrict __buf, size_t __buflen,
               struct netent **__restrict __result,
               int *__restrict __h_errnop);

extern int getnetbyname_r (__const char *__restrict __name,
               struct netent *__restrict __result_buf,
               char *__restrict __buf, size_t __buflen,
               struct netent **__restrict __result,
               int *__restrict __h_errnop);
#endif	/* misc */


/* Description of data base entry for a single service.  */
struct servent
{
  char *s_name;            /* Official service name.  */
  char **s_aliases;        /* Alias list.  */
  int s_port;            /* Port number.  */
  char *s_proto;        /* Protocol to use.  */
};

/* Open service data base files and mark them as staying open even
   after a later search if STAY_OPEN is non-zero.

   This function is a possible cancellation point and therefore not
   marked with __THROW.  */
extern void setservent (int __stay_open);

/* Close service data base files and clear `stay open' flag.

   This function is a possible cancellation point and therefore not
   marked with __THROW.  */
extern void endservent (void);

/* Get next entry from service data base file.  Open data base if
   necessary.

   This function is a possible cancellation point and therefore not
   marked with __THROW.  */
extern struct servent *getservent (void);

/* Return entry from network data base for network with NAME and
   protocol PROTO.

   This function is a possible cancellation point and therefore not
   marked with __THROW.  */
extern struct servent *getservbyname (__const char *__name,
                      __const char *__proto);

/* Return entry from service data base which matches port PORT and
   protocol PROTO.

   This function is a possible cancellation point and therefore not
   marked with __THROW.  */
extern struct servent *getservbyport (int __port, __const char *__proto);


#ifdef __USE_MISC
/* Reentrant versions of the functions above.  The additional
   arguments specify a buffer of BUFLEN starting at BUF.

   These functions are not part of POSIX and therefore no official
   cancellation point.  But due to similarity with an POSIX interface
   or due to the implementation they are cancellation points and
   therefore not marked with __THROW.  */
extern int getservent_r (struct servent *__restrict __result_buf,
             char *__restrict __buf, size_t __buflen,
             struct servent **__restrict __result);

extern int getservbyname_r (__const char *__restrict __name,
                __const char *__restrict __proto,
                struct servent *__restrict __result_buf,
                char *__restrict __buf, size_t __buflen,
                struct servent **__restrict __result);

extern int getservbyport_r (int __port, __const char *__restrict __proto,
                struct servent *__restrict __result_buf,
                char *__restrict __buf, size_t __buflen,
                struct servent **__restrict __result);
#endif /* misc */


/* Description of data base entry for a single service.  */
struct protoent
{
  char *p_name;            /* Official protocol name.  */
  char **p_aliases;        /* Alias list.  */
  int p_proto;            /* Protocol number.  */
};

/* Open protocol data base files and mark them as staying open even
   after a later search if STAY_OPEN is non-zero.

   This function is a possible cancellation point and therefore not
   marked with __THROW.  */
extern void setprotoent (int __stay_open);

/* Close protocol data base files and clear `stay open' flag.

   This function is a possible cancellation point and therefore not
   marked with __THROW.  */
extern void endprotoent (void);

/* Get next entry from protocol data base file.  Open data base if
   necessary.

   This function is a possible cancellation point and therefore not
   marked with __THROW.  */
extern struct protoent *getprotoent (void);

/* Return entry from protocol data base for network with NAME.

   This function is a possible cancellation point and therefore not
   marked with __THROW.  */
extern struct protoent *getprotobyname (__const char *__name);

/* Return entry from protocol data base which number is PROTO.

   This function is a possible cancellation point and therefore not
   marked with __THROW.  */
extern struct protoent *getprotobynumber (int __proto);


#ifdef __USE_MISC
/* Reentrant versions of the functions above.  The additional
   arguments specify a buffer of BUFLEN starting at BUF.

   These functions are not part of POSIX and therefore no official
   cancellation point.  But due to similarity with an POSIX interface
   or due to the implementation they are cancellation points and
   therefore not marked with __THROW.  */
extern int getprotoent_r (struct protoent *__restrict __result_buf,
              char *__restrict __buf, size_t __buflen,
              struct protoent **__restrict __result);

extern int getprotobyname_r (__const char *__restrict __name,
                 struct protoent *__restrict __result_buf,
                 char *__restrict __buf, size_t __buflen,
                 struct protoent **__restrict __result);

extern int getprotobynumber_r (int __proto,
                   struct protoent *__restrict __result_buf,
                   char *__restrict __buf, size_t __buflen,
                   struct protoent **__restrict __result);


/* Establish network group NETGROUP for enumeration.

   This function is not part of POSIX and therefore no official
   cancellation point.  But due to similarity with an POSIX interface
   or due to the implementation it is a cancellation point and
   therefore not marked with __THROW.  */
extern int setnetgrent (__const char *__netgroup);

/* Free all space allocated by previous `setnetgrent' call.

   This function is not part of POSIX and therefore no official
   cancellation point.  But due to similarity with an POSIX interface
   or due to the implementation it is a cancellation point and
   therefore not marked with __THROW.  */
extern void endnetgrent (void);

/* Get next member of netgroup established by last `setnetgrent' call
   and return pointers to elements in HOSTP, USERP, and DOMAINP.

   This function is not part of POSIX and therefore no official
   cancellation point.  But due to similarity with an POSIX interface
   or due to the implementation it is a cancellation point and
   therefore not marked with __THROW.  */
extern int getnetgrent (char **__restrict __hostp,
         char **__restrict __userp,
         char **__restrict __domainp);


/* Test whether NETGROUP contains the triple (HOST,USER,DOMAIN).

   This function is not part of POSIX and therefore no official
   cancellation point.  But due to similarity with an POSIX interface
   or due to the implementation it is a cancellation point and
   therefore not marked with __THROW.  */
extern int innetgr (__const char *__netgroup, __const char *__host,
            __const char *__user, __const char *domain);

/* Reentrant version of `getnetgrent' where result is placed in BUFFER.

   This function is not part of POSIX and therefore no official
   cancellation point.  But due to similarity with an POSIX interface
   or due to the implementation it is a cancellation point and
   therefore not marked with __THROW.  */
extern int getnetgrent_r (char **__restrict __hostp,
              char **__restrict __userp,
              char **__restrict __domainp,
              char *__restrict __buffer, size_t __buflen);
#endif /* misc */


#ifdef __USE_BSD
/* Call `rshd' at port RPORT on remote machine *AHOST to execute CMD.
   The local user is LOCUSER, on the remote machine the command is
   executed as REMUSER.  In *FD2P the descriptor to the socket for the
   connection is returned.  The caller must have the right to use a
   reserved port.  When the function returns *AHOST contains the
   official host name.

   This function is not part of POSIX and therefore no official
   cancellation point.  But due to similarity with an POSIX interface
   or due to the implementation it is a cancellation point and
   therefore not marked with __THROW.  */
extern int rcmd (char **__restrict __ahost, unsigned short int __rport,
         __const char *__restrict __locuser,
         __const char *__restrict __remuser,
         __const char *__restrict __cmd, int *__restrict __fd2p);

/* This is the equivalent function where the protocol can be selected
   and which therefore can be used for IPv6.

   This function is not part of POSIX and therefore no official
   cancellation point.  But due to similarity with an POSIX interface
   or due to the implementation it is a cancellation point and
   therefore not marked with __THROW.  */
extern int rcmd_af (char **__restrict __ahost, unsigned short int __rport,
            __const char *__restrict __locuser,
            __const char *__restrict __remuser,
            __const char *__restrict __cmd, int *__restrict __fd2p,
            sa_family_t __af);

/* Call `rexecd' at port RPORT on remote machine *AHOST to execute
   CMD.  The process runs at the remote machine using the ID of user
   NAME whose cleartext password is PASSWD.  In *FD2P the descriptor
   to the socket for the connection is returned.  When the function
   returns *AHOST contains the official host name.

   This function is not part of POSIX and therefore no official
   cancellation point.  But due to similarity with an POSIX interface
   or due to the implementation it is a cancellation point and
   therefore not marked with __THROW.  */
extern int rexec (char **__restrict __ahost, int __rport,
          __const char *__restrict __name,
          __const char *__restrict __pass,
          __const char *__restrict __cmd, int *__restrict __fd2p);

/* This is the equivalent function where the protocol can be selected
   and which therefore can be used for IPv6.

   This function is not part of POSIX and therefore no official
   cancellation point.  But due to similarity with an POSIX interface
   or due to the implementation it is a cancellation point and
   therefore not marked with __THROW.  */
extern int rexec_af (char **__restrict __ahost, int __rport,
             __const char *__restrict __name,
             __const char *__restrict __pass,
             __const char *__restrict __cmd, int *__restrict __fd2p,
             sa_family_t __af);

/* Check whether user REMUSER on system RHOST is allowed to login as LOCUSER.
   If SUSER is not zero the user tries to become superuser.  Return 0 if
   it is possible.

   This function is not part of POSIX and therefore no official
   cancellation point.  But due to similarity with an POSIX interface
   or due to the implementation it is a cancellation point and
   therefore not marked with __THROW.  */
extern int ruserok (__const char *__rhost, int __suser,
            __const char *__remuser, __const char *__locuser);

/* This is the equivalent function where the protocol can be selected
   and which therefore can be used for IPv6.

   This function is not part of POSIX and therefore no official
   cancellation point.  But due to similarity with an POSIX interface
   or due to the implementation it is a cancellation point and
   therefore not marked with __THROW.  */
extern int ruserok_af (__const char *__rhost, int __suser,
               __const char *__remuser, __const char *__locuser,
               sa_family_t __af);

/* Try to allocate reserved port, returning a descriptor for a socket opened
   at this port or -1 if unsuccessful.  The search for an available port
   will start at ALPORT and continues with lower numbers.

   This function is not part of POSIX and therefore no official
   cancellation point.  But due to similarity with an POSIX interface
   or due to the implementation it is a cancellation point and
   therefore not marked with __THROW.  */
extern int rresvport (int *__alport);

/* This is the equivalent function where the protocol can be selected
   and which therefore can be used for IPv6.

   This function is not part of POSIX and therefore no official
   cancellation point.  But due to similarity with an POSIX interface
   or due to the implementation it is a cancellation point and
   therefore not marked with __THROW.  */
extern int rresvport_af (int *__alport, sa_family_t __af);
#endif


/* Extension from POSIX.1g.  */
#ifdef __USE_POSIX
# ifdef __USE_GNU
/* Structure used as control block for asynchronous lookup.  */
struct gaicb
{
  const char *ar_name;        /* Name to look up.  */
  const char *ar_service;    /* Service name.  */
  const struct addrinfo *ar_request; /* Additional request specification.  */
  struct addrinfo *ar_result;    /* Pointer to result.  */
  /* The following are internal elements.  */
  int __return;
  int __unused[5];
};

/* Lookup mode.  */
#  define GAI_WAIT	0
#  define GAI_NOWAIT	1
# endif
#endif	/* POSIX */

#ifdef __USE_GNU
/* Enqueue ENT requests from the LIST.  If MODE is GAI_WAIT wait until all
   requests are handled.  If WAIT is GAI_NOWAIT return immediately after
   queueing the requests and signal completion according to SIG.

   This function is not part of POSIX and therefore no official
   cancellation point.  But due to similarity with an POSIX interface
   or due to the implementation it is a cancellation point and
   therefore not marked with __THROW.  */
extern int getaddrinfo_a (int __mode, struct gaicb *__list[__restrict_arr],
              int __ent, struct sigevent *__restrict __sig);

/* Suspend execution of the thread until at least one of the ENT requests
   in LIST is handled.  If TIMEOUT is not a null pointer it specifies the
   longest time the function keeps waiting before returning with an error.

   This function is not part of POSIX and therefore no official
   cancellation point.  But due to similarity with an POSIX interface
   or due to the implementation it is a cancellation point and
   therefore not marked with __THROW.  */
extern int gai_suspend (__const struct gaicb *__const __list[], int __ent,
         __const struct timespec *__timeout);

/* Get the error status of the request REQ.  */
extern int gai_error (struct gaicb *__req) __THROW;

/* Cancel the requests associated with GAICBP.  */
extern int gai_cancel (struct gaicb *__gaicbp) __THROW;
#endif /* GNU */

__END_DECLS

#endif /* netdb.h */
