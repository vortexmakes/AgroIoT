/*
 * tplink.c
 */

#include "mytypes.h"
#include "tplink.h"
#include "tplfsm.h"
#include "tpltbl.h"
#include "tplact.h"
#include "tplhal.h"
#include <string.h>

void
tplink_init(void)
{
    init_tplfsm(TPL_IDLE);

    init_tpl_hal();
}

void
tplink_deinit(void)
{
    deinit_tpl_hal();
}

MInt
tplink_send_frame(TPLFRM_T *p)
{
#ifdef __TPLINK_DBG__
    ushort i;
    uchar *pd;
#endif

    if (p->qty > TPLINK_MAX_PLOAD_SIZE)
    {
        return -TPLINK_ERR_PLOAD_SIZE_EXCEED;
    }

    tpl_enter_critical();

    if (get_tplfsm_state() >= TPL_IN_XMIT)
    {
        tpl_exit_critical();
        return -TPLINK_ERR_BUSY;
    }

#ifdef __TPLINK_DBG__
    #if (TPLINK_ADDRESSING_ON) && (TPLINK_DEV_TYPE == TPLINK_DEV_MASTER)
    mprintf(("%s: station = %02d, qty = %02d, buff =",
             __FUNCTION__, p->addr, p->qty));
    #else
    mprintf(("%s: qty = %02d, buff =",
             __FUNCTION__, p->qty));

    #endif

    for (i = p->qty, pd = p->pload; i; --i, ++pd)
        mprintf((" %X", *pd));

    mprintf(("\n"));
#endif

    start_xmit(p);

    tpl_exit_critical();

    return TPLINK_OK;
}

/* ------------------------------ End of file ------------------------------ */
