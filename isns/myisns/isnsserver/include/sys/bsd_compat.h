/*****************************************************************************
 Copyright (c) 2009, Hangzhou H3C Technologies Co., Ltd. All rights reserved.
------------------------------------------------------------------------------
                            bsd_compat.h
  Project Code: Comware Leopard
   Module Name: Socket
  Date Created: 2009/8/6
        Author: J03845
   Description: 协议栈从BSD移植，使用了很多BSD定义的基础结构和接口。为保持BSD
                原有风格，在移植时对基础功能做了适配。只有协议栈模块需要使用
                此文件中定义的结构和接口。其他模块不使用。

------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
  2009/8/6    J03845           Created for Leopard
*****************************************************************************/

#ifndef _SYS_BSD_COMPAT_H_
#define _SYS_BSD_COMPAT_H_

#ifdef __cplusplus
extern "C"{
#endif

#include <linux/version.h>

/* 定义MID */


/* 此头文件只在内核中被引用 */
#ifdef __KERNEL__
#ifndef _LINUX_TYPES_H
#include <linux/types.h>
#endif
#ifndef _LINUX_MM_H
#include <linux/mm.h>
#endif
#ifndef __LINUX_SEMAPHORE_H
#include <linux/semaphore.h>
#endif
#ifndef _LINUX_FS_H
#include <linux/fs.h>
#endif
#ifndef __LINUX_FILE_H
#include <linux/file.h>
#endif
#ifndef __LINUX_FDTABLE_H
#include <linux/fdtable.h>
#endif
#ifndef _LINUX_POLL_H
#include <linux/poll.h>
#endif
#ifndef _LINUX_RANDOM_H
#include <linux/random.h>
#endif
#ifndef _SYS_MBUF_H_
#include <sys/mbuf.h>
#endif
extern struct mtx Giant;

/*
 * define pseudo types for BSD socket
 */
struct filedesc {
    char f;
};

struct proc {
    struct filedesc *p_fd;   /* open files */
};

struct ucred {
    char u;
    int cr_uid;
};

struct rusage_bsd {
    long    ru_msgsnd;      /* messages sent */
    long    ru_msgrcv;      /* messages received */
};

struct thread {
    int td_retval[2];       /* return values */
    struct proc *td_proc;   /* Associated process. */
    struct ucred *td_ucred; /* Reference to credentials. */
    struct rusage_bsd td_ru;    /* rusage information */
};

extern struct thread g_stThread;
extern struct proc g_stProc;
extern struct ucred g_stUcred;

#define curthread (&g_stThread)

#define thread0 g_stThread;

/*
 * KASSERT
 */

#ifdef _DEBUG_VERSION
#define KASSERT(exp, msg) \
do { \
    if (unlikely(!(exp))) \
    { \
        (void) printk msg; \
    } \
} while (BOOL_FALSE)
#else
#define KASSERT(exp, msg)  
#endif

/*
 * mtx
 * When moved to Linux, we use spin lock to replace mtx
 */

struct mtx
{
    spinlock_t mtx_spinlock; /* Linux Spin Lock */
    int mtx_opts;            /* MTX flags: such as MTX_DEF, MTX_RECURSE */
    u_int mtx_recurse;       /* Number of recursive holds. */
    void *mtx_owner;         /* Mtx owner, may be thread or interrupt. */
};

#define MA_NOTOWNED 0x00000000
#define MA_OWNED    0x00000001

/*
 * Mutex types and options passed to mtx_init().  MTX_QUIET and MTX_DUPOK
 * can also be passed in.
 */
#define MTX_DEF        0x00000000    /* DEFAULT (sleep) lock */ 
#define MTX_SPIN       0x00000001    /* Spin lock (disables interrupts) */
#define MTX_RECURSE    0x00000004    /* Option: lock allowed to recurse */
#define MTX_NOWITNESS  0x00000008    /* Don't do any witness checking. */
#define MTX_NOPROFILE  0x00000020    /* Don't profile this lock */
/*
 * Option flags passed to certain lock/unlock routines, through the use
 * of corresponding mtx_{lock,unlock}_flags() interface macros.
 */
#define MTX_QUIET      0x00000040    /* Don't log a mutex event */
#define MTX_DUPOK      0x00000080    /* Don't log a duplicate acquire */

#define mtx_owned(mtx) ((mtx)->mtx_owner == (void *)current_thread_info())

#define mtx_assert(m, what) \
do { \
    switch (what) \
    { \
        case MA_OWNED: \
            KASSERT(mtx_owned(m), ("MTX is not owned at %s:%d", __func__, __LINE__)); \
            break; \
        case MA_NOTOWNED: \
            KASSERT(!mtx_owned(m), ("MTX is owned at %s:%d", __func__, __LINE__)); \
            break; \
        default: \
            break; \
    } \
} while (BOOL_FALSE)

void mtx_init(struct mtx *m, const char *name, const char *type, int opts);
void mtx_lock(struct mtx *m);
void mtx_unlock(struct mtx *m);
void mtx_destroy(struct mtx *m);


/* sx lock compat. */
#define sx semaphore

#define sx_init(sx, desc) init_MUTEX(sx)

#define sx_destroy(sx)  void(sx)

#define sx_xlock(sx) down(sx)

#define sx_xunlock(sx) up(sx)

extern int sx_xlock_sig(struct sx *sb_sx);
extern int sx_try_xlock(struct sx *sb_sx);

/* Hayek: Use Linux's rw Lock to replace BSD's rw Lock */
struct rwlock {
    rwlock_t rw_linux;     /* Linux rw lock struct */
    u_int rw_flag;         /* flags */
    u_int rw_recurse;      /* recurse number */
    struct task_struct *rw_owntask;  /* write lock's owner */
};

/* Flags passed to rw_assert. */
#define RA_LOCKED         0x00000001  /* Lock is at least share locked. */
#define RA_RLOCKED        0x00000002  /* Lock is exactly share locked. */
#define RA_WLOCKED        0x00000004  /* Lock is exclusively locked. */
#define RA_UNLOCKED       0x00000000  /* Lock is unlocked. */
#define RA_RECURSED       0x00000008  /* Lock is recursed. */
#define RA_NOTRECURSED    0x00000010  /* Lock is not recursed. */

/*
 * Options passed to rw_init_flags().
 */
#define RW_DUPOK      0x01
#define RW_NOPROFILE  0x02
#define RW_NOWITNESS  0x04
#define RW_QUIET      0x08
#define RW_RECURSE    0x10

void rw_init(struct rwlock *rw, const char *name);
void rw_init_flags(struct rwlock *rw, const char *name, int opts);
void rw_wlock(struct rwlock *rw);
void rw_wunlock(struct rwlock *rw);
int rw_wowned(struct rwlock *rw);
void rw_rlock(struct rwlock *rw);
void rw_runlock(struct rwlock *rw);

/* Hayek: Now, we cannot support assert. */
#define rw_assert(rw, flag) do { \
    (VOID) rw; \
    (VOID) flag; \
} while (BOOL_FALSE)

/*
 * Kernel Timer for BSD code
 * Hayek: In BSD, callout is used as timer. When moved to Linux, we use
 * timer_list to replace callout.
 */
struct callout {
    struct timer_list c_tl;           /* Linux timer list struct */
    void *c_arg;               /* function argument */
    void (*c_func)(void *);    /* function to call */
    struct mtx *c_mtx;         /* mutex to lock */
    int c_flags;               /* state of this entry */
};

#define CALLOUT_LOCAL_ALLOC    0x0001 /* was allocated from callfree */
#define CALLOUT_ACTIVE         0x0002 /* callout is currently active */
#define CALLOUT_PENDING        0x0004 /* callout is waiting for timeout */
#define CALLOUT_MPSAFE         0x0008 /* callout handler is mp safe */
#define CALLOUT_RETURNUNLOCKED 0x0010 /* handler returns with mtx unlocked */

void callout_init(struct callout *, int);
void callout_init_mtx(struct callout *, struct mtx *, int);
int callout_reset(struct callout *, int, void (*)(void *), void *);
int _callout_stop_safe(struct callout *, int);

#define callout_active(c)     ((c)->c_flags & CALLOUT_ACTIVE)
#define callout_deactivate(c) ((c)->c_flags &= ~CALLOUT_ACTIVE)
#define callout_pending(c)    (timer_pending(&((c)->c_tl)))
#define callout_drain(c)      _callout_stop_safe(c, 1)
#define callout_stop(c)       _callout_stop_safe(c, 0)

/* TCP timestamps are only 32-bits, this causes a slight */
#define TICKS_USEC (1000000 / HZ)   /* microseconds per hz */

/*Start:add by linchangwang:for tcp timestamps*/
/*for tcp syncookie timestamp*/
#define time_uptime (get_seconds())   /* UTC in second */
/*End:add by linchangwang for tcp timestamps*/

/*
 * Process
 */
#define PCATCH   0x00000001
#define PSOCK    0x00000002

/* 
 * Memory
 */

/*
 * flags to malloc.
 */
#define M_NOWAIT    GFP_ATOMIC      /* do not block */
#define M_WAITOK    GFP_KERNEL      /* ok to block */
#define M_ZERO      __GFP_ZERO      /* bzero the allocation */

void bcopy(const void *src, void *dst, size_t len);

void bzero(void *b, size_t len);

extern int copyin(const void __user *from, void *to, size_t len);

extern int copyout(const void *from, void __user *to, size_t len);

extern int copyiniov(struct iovec *iovp, u_int iovcnt, struct iovec **iov, int error);

#define HASH_NOWAIT    0x00000001
#define HASH_WAITOK    0x00000002
void hashdestroy(void *, u_long);
void *hashinit(int count, u_long *hashmark);
void *hashinit_flags(int count, u_long *hashmask, int flags);

/* 
 * Item constructor
 *
 * Arguments:
 *    item  A pointer to the memory which has been allocated.
 *    arg   The arg field passed to uma_zalloc_arg
 *    size  The size of the allocated item
 *    flags See zalloc flags
 * 
 * Returns:
 *    0      on success
 *      errno  on failure
 *
 * Discussion:
 *    The constructor is called just before the memory is returned
 *    to the user. It may block if necessary.
 */
typedef int (*uma_ctor)(void *mem, int size, void *arg, int flags);

/*
 * Item destructor
 *
 * Arguments:
 *    item  A pointer to the memory which has been allocated.
 *    size  The size of the item being destructed.
 *    arg   Argument passed through uma_zfree_arg
 * 
 * Returns:
 *    Nothing
 *
 * Discussion:
 *    The destructor may perform operations that differ from those performed
 *    by the initializer, but it must leave the object in the same state.
 *    This IS type stable storage.  This is called after EVERY zfree call.
 */
typedef void (*uma_dtor)(void *mem, int size, void *arg);

/* 
 * Item initializer
 *
 * Arguments:
 *    item  A pointer to the memory which has been allocated.
 *    size  The size of the item being initialized.
 *    flags See zalloc flags
 * 
 * Returns:
 *    0      on success
 *      errno  on failure
 *
 * Discussion:
 *    The initializer is called when the memory is cached in the uma zone. 
 *    this should be the same state that the destructor leaves the object in.
 */
typedef int (*uma_init)(void *mem, int size, int flags);

/*
 * Item discard function
 *
 * Arguments:
 *     item  A pointer to memory which has been 'freed' but has not left the 
 *          zone's cache.
 *    size  The size of the item being discarded.
 *
 * Returns:
 *    Nothing
 *
 * Discussion:
 *    This routine is called when memory leaves a zone and is returned to the
 *    system for other uses.  It is the counter part to the init function.
 */
typedef void (*uma_fini)(void *mem, int size);

/*
 * Definitions for uma_zcreate flags
 *
 * These flags share space with UMA_ZFLAGs in uma_int.h.  Be careful not to
 * overlap when adding new features.  0xf0000000 is in use by uma_int.h.
 */
#define UMA_ZONE_PAGEABLE  0x0001    /* Return items not fully backed by
                                        physical memory XXX Not yet */
#define UMA_ZONE_ZINIT     0x0002    /* Initialize with zeros */
#define UMA_ZONE_STATIC    0x0004    /* Staticly sized zone */
#define UMA_ZONE_OFFPAGE   0x0008    /* Force the slab structure allocation
                                        off of the real memory */
#define UMA_ZONE_MALLOC    0x0010    /* For use by malloc(9) only! */
#define UMA_ZONE_NOFREE    0x0020    /* Do not free slabs of this type! */
#define UMA_ZONE_MTXCLASS  0x0040    /* Create a new lock class */
#define UMA_ZONE_VM        0x0080    /*
                                      * Used for internal vm datastructures
                                      * only.
                                      */
#define UMA_ZONE_HASH      0x0100    /*
                                      * Use a hash table instead of caching
                                      * information in the vm_page.
                                      */
#define UMA_ZONE_SECONDARY 0x0200    /* Zone is a Secondary Zone */
#define UMA_ZONE_REFCNT    0x0400    /* Allocate refcnts in slabs */
#define UMA_ZONE_MAXBUCKET 0x0800    /* Use largest buckets */

/* Definitions for align */
#define UMA_ALIGN_PTR    (int)(sizeof(void *))   /* Alignment fit for ptr */
#define UMA_ALIGN_LONG   (int)(sizeof(long))     /* "" long */
#define UMA_ALIGN_INT    (int)(sizeof(int))      /* "" int */
#define UMA_ALIGN_SHORT  (int)(sizeof(short))    /* "" short */
#define UMA_ALIGN_CHAR   (int)(sizeof(char))     /* "" char */
#define UMA_ALIGN_CACHE  (int)(0)                /* Cache line size align */

/*
 * Zone management structure 
 *
 * TODO: Optimize for cache line size
 *
 */
#define UMA_MAXNAME_LEN  24 
struct uma_zone {
    char        uz_name[UMA_MAXNAME_LEN];  /* name of uma zone */
    size_t      size;       /* Size of memory */
    uma_ctor    uz_ctor;    /* Constructor for each allocation */
    uma_dtor    uz_dtor;    /* Destructor */
    uma_init    uz_init;    /* Initializer for each item */
    uma_fini    uz_fini;    /* Discards memory */
    struct kmem_cache *uz_cachep;  /* Cache pointer */
    atomic_t    uz_curitem; /* Current number */
    atomic_t    uz_maxitem; /* Max number */
};

typedef struct uma_zone * uma_zone_t;

uma_zone_t uma_zcreate(char *name, size_t size, uma_ctor ctor, uma_dtor dtor,
        uma_init uminit, uma_fini fini, int align, u_int32_t flags);

void uma_zdestroy(uma_zone_t zone);

void *uma_zalloc(uma_zone_t zone, int flags);

void uma_zfree(uma_zone_t zone, void *item);

void uma_zone_set_max(uma_zone_t zone, int nitems);

/*
 * MBUF
 */

#define MT_DATA     MBUF_DATA_DATA      /* dynamic (data) allocation */
#define MT_SONAME   MBUF_DATA_NAME      /* socket name */
#define MT_CONTROL  MBUF_DATA_CONTROL   /* extra-data protocol message */
#define MT_OOBDATA  MBUF_DATA_OOBDATA   /* oob data */

/* mbuf flags */
#define M_EXT        0x0001    /* has associated external storage */
#define M_PKTHDR     0x0002    /* start of record */
#define M_EOR        0x0004    /* end of record */


/*BSD的mbuf操作转定义成leopard相关操作*/
#define mbuf tagMBuf 

#define mtod(m,t) MBUF_BTOD(m, t)

#define m_freem(m) \
    do { \
        if (NULL != m) { \
            (void)MBUF_Destroy(m); \
        } \
    } while (BOOL_FALSE)

#define M_PREPEND(m, plen, how) \
    do { \
        ULONG ulRet; \
        ulRet = MBUF_PREPEND(m, plen, MID_SOCKET, how); \
        if (ERROR_SUCCESS != ulRet) \
        { \
            (void)MBUF_Destroy(m); \
            m = NULL; \
        } \
    } while (BOOL_FALSE)


#define M_DONTWAIT  M_NOWAIT
#define M_TRYWAIT   M_WAITOK
#define M_WAIT      M_WAITOK

/* Length to m_copy to copy all. */
#define M_COPYALL   1000000000

extern struct mbuf *m_copym(struct mbuf *m, int off0, int len, int wait);
extern struct mbuf *m_pullup(struct mbuf *n, int len);
extern void m_adj(struct mbuf *m, int len);
#define m_copy(m, o, l) m_copym((m), (o), (l), M_DONTWAIT)

#define m_get(how, type) MBUF_Create(0, 0, type, MID_SOCKET, how)


/*
 * File system
 */
int falloc(IN struct path *pstPath, 
           IN mode_t stMode,
           IN const struct file_operations *pstfOper, 
           OUT struct file **ppstFile, 
           OUT int *piFd);
void fdclose(struct filedesc *fdp, struct file *fp, int idx, struct thread *td);
int fdrop(struct file *fp, struct thread *td);

/*
 * UIO types
 */
enum uio_rw { UIO_READ, UIO_WRITE };

/* Segment flag values. */
enum uio_seg {
    UIO_USERSPACE,    /* from user data space */
    UIO_SYSSPACE,     /* from system space */
    UIO_NOCOPY        /* don't copy, already in object */
};
struct uio {
    struct iovec *uio_iov;        /* scatter/gather list */
    int    uio_iovcnt;            /* length of scatter/gather list */
    off_t  uio_offset;            /* offset in target object */
    int    uio_resid;             /* remaining bytes to process */
    int    uio_realLen;           /* real length of the packet */
    enum   uio_seg uio_segflg;    /* address space */
    enum   uio_rw uio_rw;         /* operation */
    struct thread *uio_td;        /* owner */
};

int uiomove(void *cp, int n, struct uio *uio);

struct mbuf *m_uiotombuf(struct uio *uio, int how, int len, int align, int flags);

int m_apply(struct mbuf *m, int off, int len,
    int (*f)(void *, void *, u_int), void *arg);
/*
 * SOCKET types
 */
#define MCLBYTES     512     /* size of an mbuf datablock */
#define MSIZE        256     /* size of an mbuf */
#define MHLEN        256     /* BSD:data len w/pkthdr */
#define nmbclusters  1024
#define MLEN         MSIZE   /* normal data len */
/*
 * IFNET
 */
struct ifnet
{
    IF_INDEX ifindex;
};


/*
 * Math
 */
/* Macros for min/max. */
#define howmany(x, y)    (((x)+((y)-1))/(y))
#ifndef rounddown
#define rounddown(x, y)    (((x)/(y))*(y))
#endif
#ifndef roundup
#define roundup(x, y)    ((((x)+((y)-1))/(y))*(y))  /* to any y */
#endif
#define roundup2(x, y)    (((x)+((y)-1))&(~((y)-1))) /* if y is powers of two */
#define powerof2(x)    ((((x)-1)&(x))==0)

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

static inline int imax(int a, int b) { return (a > b ? a : b); }
static inline int imin(int a, int b) { return (a < b ? a : b); }
static inline long lmin(long a, long b) { return (a < b ? a : b); }
static inline u_long ulmin(u_long a, u_long b) { return (a < b ? a : b); }

/*random32 - pseudo random number generator*/
#if (LINUX_VERSION_CODE < KERNEL_VERSION(3,1,0))
#define arc4random() random32()
#else
#define arc4random() prandom_u32()
#endif
#define read_random(buf,count) get_random_bytes(buf, count)


/* 
 * jailed
 */
/*
 * IPv4 and IPv6 privileges.
 */
#define PRIV_NETINET_RESERVEDPORT    490    /* Bind low port number. */
#define PRIV_NETINET_IPFW    491    /* Administer IPFW firewall. */
#define PRIV_NETINET_DIVERT    492    /* Open IP divert socket. */
#define PRIV_NETINET_PF        493    /* Administer pf firewall. */
#define PRIV_NETINET_DUMMYNET    494    /* Administer DUMMYNET. */
#define PRIV_NETINET_CARP    495    /* Administer CARP. */
#define PRIV_NETINET_MROUTE    496    /* Administer multicast routing. */
#define PRIV_NETINET_RAW    497    /* Open netinet raw socket. */
#define PRIV_NETINET_GETCRED    498    /* Query netinet pcb credentials. */
#define PRIV_NETINET_ADDRCTRL6    499    /* Administer IPv6 address scopes. */
#define RIV_NETINET_ND6    500    /* Administer IPv6 neighbor disc. */
#define PRIV_NETINET_SCOPE6    501    /* Administer IPv6 address scopes. */
#define PRIV_NETINET_ALIFETIME6    502    /* Administer IPv6 address lifetimes. */
#define PRIV_NETINET_IPSEC    503    /* Administer IPSEC. */
#define PRIV_NETINET_REUSEPORT    504    /* Allow [rapid] port/address reuse. */
#define PRIV_NETINET_SETHDROPTS 505 /* Set certain IPv4/6 header options. */

extern int jailed(struct ucred *cred);
extern void crfree(struct ucred *cr);
extern int priv_check(struct thread *td, int priv);
extern u_int32_t prison_getip(struct ucred *cred);
extern int prison_ip(struct ucred *cred, int flag, u_int32_t *ip);
extern int priv_check_cred(struct ucred *cred, int priv, int flags);
extern void prison_remote_ip(struct ucred *cred, int flag, u_int32_t *ip);
extern struct ucred *crhold(struct ucred *cr);
extern int jail_socket_unixiproute_only;

#define TUNABLE_INT_FETCH(path, var) 

#endif /* __KERNEL__ */

#ifdef __cplusplus
}
#endif  /* end of __cplusplus */

#endif /* _SYS_BSD_COMPAT_H_ */

