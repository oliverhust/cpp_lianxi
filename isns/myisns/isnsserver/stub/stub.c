#ifdef __cplusplus
extern "C"{
#endif

#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/mman.h>

#include <limits.h>    /* for PAGESIZE */

#include "stub.h"

#ifndef PAGESIZE
#define PAGESIZE 4096
#endif

void stub_set(struct func_stub *stub, void *fn, void *fn_stub)
{
    stub->fn = fn;
    memcpy(&stub->buf, fn, 5);

    if(mprotect((void *)((int)fn & ~(PAGESIZE-1)), PAGESIZE * 2, PROT_READ | PROT_WRITE | PROT_EXEC)) {
        perror("Couldn't mprotect");
        exit(errno);
    }
    *(char *)fn = (char)0xE9;
    *(int *)((char *)fn +1) = (char *)fn_stub - (char *)fn - 5;
    if(mprotect((void *)((int)fn & ~(PAGESIZE-1)), PAGESIZE * 2, PROT_READ | PROT_EXEC)) {
        perror("Couldn't mprotect");
        exit(errno);
    }
}

void stub_reset(struct func_stub *stub)
{
    if(mprotect((void *)((int)stub->fn & ~(PAGESIZE-1)), PAGESIZE * 2, PROT_READ | PROT_WRITE | PROT_EXEC)) {
        perror("Couldn't mprotect");
        exit(errno);
    }
    memcpy(stub->fn, &stub->buf, 5);
    if(mprotect((void *)((int)stub->fn & ~(PAGESIZE-1)), PAGESIZE * 2, PROT_READ | PROT_EXEC)) {
        perror("Couldn't mprotect");
        exit(errno);
    }
}

#ifdef __cplusplus
}
#endif /* end of __cplusplus */
