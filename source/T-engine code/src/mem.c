/*
 *    line drawing sample program
 *    stack management
 *    Copyright (C) 2002 by Personal Media Corporation
 */

#include "drawsamp.h"

LOCAL int memsiz;
LOCAL int memused;
LOCAL H *memtop = NULL;

/*
 * initialize memory
 */
EXPORT void ini_stk( void )
{
    if (memtop != NULL) Kfree(memtop);
    memused = 0; memsiz = 0;
    memtop = NULL;
}

/*
 * push to stack
 */
EXPORT void psh_stk( H x )
{
    memused++;
    if (memused > memsiz) {
        if (memsiz > 0) memsiz = 2 * memsiz; else memsiz = 1;
        memtop = Krealloc(memtop, memsiz * sizeof(H));
    }
    memtop[memused - 1] = x;
}

/*
 * get from stack
 */
EXPORT H pop_stk( void )
{
    if (memused <= 0) return -32768;
    return memtop[--memused];
}

