/*
 * psport.c
 */

#include <string.h>
#include "ps.h"
#include "psport.h"
#include "tplink.h"

static TPLFRM_T frm;

void
dll_send_frame(ST_T s, PS_PLBUFF_T *p)
{
    frm.addr = (uchar)s;
    frm.qty = p->qty <= TPLINK_MAX_PLOAD_SIZE ?
              p->qty : TPLINK_MAX_PLOAD_SIZE;

    memcpy(frm.pload, p->payload, frm.qty);
    tplink_send_frame(&frm);
}
/* ------------------------------ End of file ------------------------------ */
