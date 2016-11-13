#include <sys/basetype.h>
#include <sys/epoll.h>

/* EPOLL ¸¨ÖúÍ·ÎÄ¼ş */

typedef ULONG (*epoll_callback_t) (IN UINT uiEvent, IN VOID *pHandle);

struct _ut_epoll_event
{
    UINT32 events;  /* Epoll events */
    epoll_data_t data;    /* User data variable */
    epoll_callback_t callback;
} __attribute__ ((__packed__));

#define epoll_event _ut_epoll_event


