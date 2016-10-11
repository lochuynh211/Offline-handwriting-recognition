/*
 *    line drawing sample program: main
 *
 *    Copyright (C) 2002 by Personal Media Corporation
 */

#include "drawsamp.h"
#include<sys/pinfo.h>

/*
 * main
 */
EXPORT ER main( INT ac, UB *av[] )
{
    T_CTSK    ctsk;               /* create task information */
    T_RTSK    rtsk;               /* task information */
    static ID tskid = -1;         /* task ID */

    if ( ac < 0 ) { /* in unload */
        if (tskid >= 0) {
            exitproc();
            while (tk_ref_tsk(tskid, &rtsk) >= E_OK) tk_dly_tsk(1);
        }
        goto ext;
    }

    /* create task */

    ctsk.exinf   = PRCTSK_EXINF;                /* extend information */
    ctsk.tskatr  = TA_HLNG | TA_RNG0 | TA_FPU;  /* task attribute */
    ctsk.task    = task;               /* task start address */
    ctsk.itskpri = PRI1;               /* task start priority */
    ctsk.stksz   = 1024 * 4;           /* stack size */

    tskid = tk_cre_tsk( &ctsk );       /* create task */

    if (tskid < E_OK) goto ext;        /* fail to create task */

    /* start task */

    tk_sta_tsk(tskid, 0);              /* start task */

    /* exit */

ext:

    return 0;
}

