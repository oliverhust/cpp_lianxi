/*****************************************************************************
 Copyright (c) 2009, Hangzhou H3C Technologies Co., Ltd. All rights reserved.
------------------------------------------------------------------------------
                            domain.h
  Project Code: Comware V7
   Module Name: Socket
  Date Created: 2009/8/6
        Author: J03845
   Description: domain头文件，从BSD移植。内核协议栈使用

------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
  2009/8/6    J03845           Created for V7
*****************************************************************************/

#ifndef _SYS_DOMAIN_H_
#define _SYS_DOMAIN_H_

#ifdef __cplusplus
extern "C"{
#endif

/* 此头文件只在内核中被引用 */
#ifdef __KERNEL__
#ifndef _SYS_BSD_COMPAT_H_
#include <sys/bsd_compat.h>
#endif
/*
 * Structure per communications domain.
 */

/*
 * Forward structure declarations for function prototypes [sic].
 */

struct domain{
    int     dom_family;        /* AF_xxx */
    char    *dom_name;
    void    (*dom_init)        /* initialize domain data structures */
        (void);
    int    (*dom_externalize)    /* externalize access rights */
        (struct mbuf *, struct mbuf **);
    void    (*dom_dispose)        /* dispose of internalized rights */
        (struct mbuf *);
    struct    protosw *dom_protosw, *dom_protoswNPROTOSW;
    struct    domain *dom_next;
    int    (*dom_rtattach)        /* initialize routing table */
        (void **, int);
    int     dom_rtoffset;        /* an arg to rtattach, in bits */
    int     dom_maxrtkey;        /* for routing layer */
    void    *(*dom_ifattach)(struct ifnet *);
    void    (*dom_ifdetach)(struct ifnet *, void *); /* af-dependent data on ifnet */
                    /* af-dependent data on ifnet */
};


extern struct pr_usrreqs nousrreqs;
extern unsigned long net_add_domain(void *);
extern VOID net_get_domain(u_char *paucDomain);
extern BOOL_T net_support_domain(const u_char *paucDom, INT iDomain);
extern int max_hdr;
extern int max_linkhdr;
extern int max_protohdr;
extern unsigned long domaininit(void *);
extern unsigned long domainfinalize(void *);
extern unsigned long domainservinit(void *dummy);
extern unsigned long domainrun(void *dummy);

extern unsigned long domainminiservinit(void *dummy);
extern u_long sv_eventcb(u_int evtype, void *datap);

/* Begin: Added by mengli for Socket-Dist-Project on 2009-08-25. */
extern int pfevnotifyall(int event, void *datap);
extern BOOL_T pfmsgrcv(int family, int protocol, u_long channel, void *datap, u_int datalen);
extern int pfevnotifytospec(int family, int protocol, int event, void *datap);
extern int pffindnextpull(short *family, short *protocol);
extern int pfpullspec(int family, int protocol, u_long *len, u_long *privatecontex, void **buf);
extern void pffasttimoproc(void);
extern void pfslowtimoproc(void);
extern void pftimoinit(void);
extern void pftimouninit(void);
/* End: Added by mengli for Socket-Dist-Project on 2009-08-25. */

/*
#define DOMAIN_SET(name) \
    SYSINIT(domain_ ## name, SI_SUB_PROTO_DOMAIN, SI_ORDER_SECOND, net_add_domain, & name ## domain)
*/

#endif /* __KERNEL__ */

#ifdef __cplusplus
}
#endif  /* end of __cplusplus */

#endif  /* _SYS_DOMAIN_H_ */

