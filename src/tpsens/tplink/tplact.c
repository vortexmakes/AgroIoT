/*
 * tplact.c
 */

#include <string.h>
#include "tplact.h"
#include "tplfsm.h"
#include "tpltbl.h"
#include "tplcfg.h"
#include "tpldefs.h"
#include "tplhal.h"
#include "tplink.h"

static TPLFRM_T tpl;

static uchar *pl;
static ushort curr_dcnt, check;
static uchar check_sum, curr_escape;

static const uchar esc_codes[TPLINK_NUM_ESC_CODES - 1] = {TPL_SOF, TPL_ESC};

#define store_addr(x)           tpl.addr = (x)
#define store_dcnt_h(x)         tpl.qty = (ushort)((x) << 8)
#define store_dcnt_l(x)         tpl.qty |= (x)
#define fits_in_pload_size()    (tpl.qty <= TPLINK_MAX_PLOAD_SIZE)

#define store_data(x)           *pl++ = (x)

#define save_escaped(x)       curr_escape = (uchar)((x) & TPLINK_ESC_MASK)
#define get_esc_code(x)         esc_codes[(x) - 1]
#define xmit_escape()           tpl_xmit(TPL_ESC)
#define xmit_esc_code()         tpl_xmit(curr_escape)

#define add_check_sum(x)        check_sum += (x)
#define store_chk_h(x)          check = (ushort)((x) << 8)
#define store_chk_l(x)          check |= (x)
#define xmit_chk_h()            tpl_xmit((uchar)((check & 0xFF00) >> 8))
#define xmit_chk_l()            tpl_xmit((uchar)(check & 0x00FF))
#define verify_check()          (check == calc_check(check_sum))

#define is_rcv_inframe()        (++curr_dcnt <= tpl.qty)
#define is_xmit_inframe()       (++curr_dcnt <= tpl.qty)

#define init_pload() \
    ;           pl = tpl.pload;                             \
    curr_dcnt = tpl.qty = check = check_sum = 0;
void
start_xmit(TPLFRM_T *p)
{
#if TPLINK_ADDRESSING_ON
#if (TPLINK_DEV_TYPE == TPLINK_DEV_SLAVE)
    tpl.addr = get_slave_addr();
#else
    tpl.addr = p->addr | TPLINK_COMFLOW_MSK;
#endif
#endif

    pl = tpl.pload;
    *pl++ = (uchar)((p->qty & 0xFF00) >> 8);
    *pl++ = (uchar)(p->qty & 0x00FF);
    memcpy(pl, p->pload, p->qty);

    init_pload();

    tpl.qty = (ushort)(p->qty + 2);

    tpl_choke_xmit(TPL_SOF);
    tplfsm_stop_timer();
#if ((TPLINK_DEV_TYPE == TPLINK_DEV_MASTER) && (TPLINK_VAR_FRMTOUT))
    curr_toutfrm = p->toutfrm;
#endif
}

static
void
start_rcv(void)
{
    init_pload();
}

static
MUInt
is_esc_code(uchar data)
{
    switch (data)
    {
        case TPLINK_ESC_SOF:
        case TPLINK_ESC_ESC:
            return 1;

        default:
            return 0;
    }
}

static
MUInt
need_escape(uchar data)
{
    switch (data)
    {
        case TPL_SOF:
        case TPL_ESC:
            return 1;

        default:
            return 0;
    }
}

/*
 * TPLINK FSM Proc Functions
 */

MUInt
proc_tpl_idle(MUInt input, uchar data)
{
    switch (input)
    {
        case TPLINK_RCV:
            if (data != TPL_SOF)
            {
                return TPL_IDLE;
            }

#if TPLINK_ADDRESSING_ON
            start_rcv();
            tplfsm_kick_timer(TPLINK_INTER_BYTE_TIME);
            return TPL_WADDR;
#else
            start_rcv();
            tplfsm_kick_timer(TPLINK_INTER_BYTE_TIME);
            return TPL_WNDATAH;
#endif

        case TPLINK_XMIT:
#if TPLINK_ADDRESSING_ON
            tpl_xmit(tpl.addr);
#else
            proc_tpl_inxmit(TPLINK_XMIT, 0);
#endif
            tplfsm_stop_timer();
            return TPL_IN_XMIT;

        case TPLINK_TOUT:
#if (TPLINK_DEV_TYPE == TPLINK_DEV_MASTER)
            tplink_ontout();
            tplfsm_stop_timer();
#endif
            return TPL_IDLE;

        default:
            return TPL_IDLE;
    }
}

MUInt
proc_tpl_inesc(MUInt input, uchar data)
{
    switch (input)
    {
        case TPLINK_RCV:
            if (is_esc_code(data))
            {
                data = get_esc_code(data);
                tplfsm_kick_timer(TPLINK_INTER_BYTE_TIME);
                return ret_tplst(TPLINK_RETESC, data);
            }
            else
            {
                return TPL_IDLE;
            }

        case TPLINK_TOUT:
            mprintf(("##TPLDBG: RCV Tout\n"));
            tplink_ontout();
            return TPL_IDLE;

        default:
            tplfsm_stop_timer();
            tpl_stop_xmit();
            return TPL_IDLE;
    }
}

#define is_from_master(x) \
    ((x & TPLINK_COMFLOW_MSK) == \
     (TPLINK_COMFLOW_MSK))
#define is_from_last_slave(x)   ((x) == (tpl.addr & ~TPLINK_COMFLOW_MSK))
#define is_my_slave_addr(x) \
    (((x & ~TPLINK_COMFLOW_MSK) == get_slave_addr()) ||   \
     ((x & ~TPLINK_COMFLOW_MSK) == TPLINK_BROADCAST_ADDR))

#if TPLINK_ADDRESSING_ON
MUInt
proc_tpl_waddr(MUInt input, uchar data)
{
    switch (input)
    {
        case TPLINK_RCV:
            if (data == TPL_ESC)
            {
                return TPL_IDLE;
            }
            else
            {
#if (TPLINK_DEV_TYPE == TPLINK_DEV_SLAVE)
                if (is_from_master(data) && is_my_slave_addr(data))
                {
                    store_addr(data);
                    tplfsm_kick_timer(TPLINK_INTER_BYTE_TIME);
                    return TPL_WNDATAH;
                }
                else
                {
                    return TPL_IDLE;
                }
#else
                if (is_from_last_slave(data))
                {
                    tplfsm_kick_timer(TPLINK_INTER_BYTE_TIME);
                    store_addr(data);
                    return TPL_WNDATAH;
                }
                else
                {
                    return TPL_IDLE;
                }
#endif
            }

        case TPLINK_TOUT:
            mprintf(("##TPLDBG: RCV Tout\n"));
            tplink_ontout();
            return TPL_IDLE;

        default:
            tplfsm_stop_timer();
            tpl_stop_xmit();
            return TPL_IDLE;
    }
}
#endif

MUInt
proc_tpl_wndatah(MUInt input, uchar data)
{
    switch (input)
    {
        case TPLINK_RCV:
            if (data == TPL_ESC)
            {
                tplfsm_kick_timer(TPLINK_INTER_BYTE_TIME);
                return call_tplst(TPL_IN_ESC, TPL_WNDATAH);
            }
            else
            {
                case TPLINK_RETESC:
                {
                    tplfsm_kick_timer(TPLINK_INTER_BYTE_TIME);
                    add_check_sum(data);
                    store_dcnt_h(data);
                    return TPL_WNDATAL;
                }

                case TPLINK_TOUT:
                    mprintf(("##TPLDBG: RCV Tout\n"));
            }
            tplink_ontout();
            return TPL_IDLE;

        default:
            tplfsm_stop_timer();
            tpl_stop_xmit();
            return TPL_IDLE;
    }
}

MUInt
proc_tpl_wndatal(MUInt input, uchar data)
{
    switch (input)
    {
        case TPLINK_RCV:
            if (data == TPL_ESC)
            {
                tplfsm_kick_timer(TPLINK_INTER_BYTE_TIME);
                return call_tplst(TPL_IN_ESC, TPL_WNDATAL);
            }
            else
            {
                case TPLINK_RETESC:
                {
                    tplfsm_kick_timer(TPLINK_INTER_BYTE_TIME);
                    add_check_sum(data);
                    store_dcnt_l(data);
                    if (!fits_in_pload_size())
                    {
                        tpl.qty = TPLINK_MAX_PLOAD_SIZE;
                    }

                    return TPL_IN_DATA;
                }

                case TPLINK_TOUT:
                    mprintf(("##TPLDBG: RCV Tout\n"));
            }
            tplink_ontout();
            return TPL_IDLE;

        default:
            tplfsm_stop_timer();
            tpl_stop_xmit();
            return TPL_IDLE;
    }
}

MUInt
proc_tpl_indata(MUInt input, uchar data)
{
    switch (input)
    {
        case TPLINK_RCV:
            if (data == TPL_ESC)
            {
                tplfsm_kick_timer(TPLINK_INTER_BYTE_TIME);
                return call_tplst(TPL_IN_ESC, TPL_IN_DATA);
            }
            else
            {
                case TPLINK_RETESC:
                {
                    tplfsm_kick_timer(TPLINK_INTER_BYTE_TIME);
                    if (!is_rcv_inframe())
                    {
                        store_chk_h(data);
                        return TPL_WCHK;
                    }
                    store_data(data);
                    add_check_sum(data);
                    return TPL_IN_DATA;
                }

                case TPLINK_TOUT:
                    mprintf(("##TPLDBG: RCV Tout\n"));
            }
            tplink_ontout();
            return TPL_IDLE;

        default:
            tplfsm_stop_timer();
            tpl_stop_xmit();
            return TPL_IDLE;
    }
}

MUInt
proc_tpl_wchk(MUInt input, uchar data)
{
    switch (input)
    {
        case TPLINK_RCV:
            store_chk_l(data);
            if (verify_check())
            {
                mprintf(("##TPLDBG: Valid Frame RCV\n"));
                tplink_onrcv(&tpl);
                tplfsm_stop_timer();
            }
            else
            {
                mprintf(("##TPLDBG: Invalid Frame Checksum\n"));
                tplink_onchkerr();
                tplfsm_stop_timer();
            }
            return TPL_IDLE;

        case TPLINK_TOUT:
            mprintf(("##TPLDBG: RCV Tout\n"));
            tplink_ontout();

        default:
            tplfsm_stop_timer();
            tpl_stop_xmit();
            return TPL_IDLE;
    }
}

MUInt
proc_tpl_inxmit(MUInt input, uchar data)
{
    (void)data;

    switch (input)
    {
        case TPLINK_XMIT:
            if (is_xmit_inframe())
            {
                if (need_escape(*pl))
                {
                    save_escaped(*pl);
                    add_check_sum(*pl);
                    xmit_escape();
                    return TPL_XMIT_ESC;
                }
                add_check_sum(*pl);
                tpl_xmit(*pl++);
                return TPL_IN_XMIT;
            }
            else
            {
                check = calc_check(check_sum);
                xmit_chk_h();
                return TPL_XCHK;
            }
        default:
            tpl_stop_xmit();
            return TPL_IDLE;
    }
}

MUInt
proc_tpl_xmitesc(MUInt input, uchar data)
{
    (void)data;

    switch (input)
    {
        case TPLINK_XMIT:
            xmit_esc_code();
            ++pl;
            return TPL_IN_XMIT;

        default:
            tpl_stop_xmit();
            return TPL_IDLE;
    }
}

MUInt
proc_tpl_xchk(MUInt input, uchar data)
{
    (void)data;

    switch (input)
    {
        case TPLINK_XMIT:
            xmit_chk_l();
            return TPL_WXMIT;

        default:
            tpl_stop_xmit();
            return TPL_IDLE;
    }
}

MUInt
proc_tpl_wxmit(MUInt input, uchar data)
{
    (void)data;

    switch (input)
    {
        case TPLINK_XMIT:
            tpl_stop_xmit();
#if TPLINK_BSP_SUPPORT_TC_IRQ == 1
            return TPL_WXMITCMP;
#else
            tpl_eoftx();
            tplink_onxmit_cmp();
#if (TPLINK_DEV_TYPE == TPLINK_DEV_MASTER)
#if (TPLINK_VAR_FRMTOUT)
            tplfsm_kick_timer(curr_toutfrm);
#else
            tplfsm_kick_timer(TPLINK_FRMTOUT_DFT);
#endif
#endif
            return TPL_IDLE;
#endif
        default:
            return TPL_IDLE;
    }
}

MUInt
proc_tpl_wxmitcmp(MUInt input, uchar data)
{
    (void)data;

    switch (input)
    {
        case TPLINK_XMIT:
            tpl_eoftx();
            tplink_onxmit_cmp();
#if (TPLINK_DEV_TYPE == TPLINK_DEV_MASTER)
#if (TPLINK_VAR_FRMTOUT)
            tplfsm_kick_timer(curr_toutfrm);
#else
            tplfsm_kick_timer(TPLINK_FRMTOUT_DFT);
#endif
#endif

        default:
            return TPL_IDLE;
    }
}

/* ------------------------------ End of file ------------------------------ */
