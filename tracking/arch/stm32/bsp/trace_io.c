/**
 *  \file       trace_io.c
 *  \brief      Serial support for Tracking-STM32
 *
 *  \ingroup    bsp
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2019.01.31  DaBa  v2.4.05
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  DaBa  Dario Baliña     db@vortexmakes.com
 */
/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */
#include "rkh.h"
#include "bsp.h"
#include "cubemx.h"

#if RKH_CFG_TRC_EN == 1
/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
#define TRC_COM_PORT        &huart5 /* CubeMX allocated handler */
#define TRC_UART_Init()     MX_UART5_Init()
#define TRC_BAUD_RATE               /* CubeMX setups baudrate */
#define BSP_TS_RATE_HZ      (1000/RKH_CFG_FWK_TICK_RATE_HZ)

/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
UART_HandleTypeDef UartHandle;

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ---------------------------- Global functions --------------------------- */
void
rkh_trc_open(void)
{
    rkh_trc_init();

    TRC_UART_Init();

    RKH_TRC_SEND_CFG(BSP_TS_RATE_HZ);
}



void
rkh_trc_close(void)
{
}

void
rkh_trc_flush(void)
{
    rui8_t *blk;
    TRCQTY_T nbytes;
    RKH_SR_ALLOC();

    FOREVER
    {
        nbytes = 128;

        RKH_ENTER_CRITICAL_();
        blk = rkh_trc_get_block(&nbytes);
        RKH_EXIT_CRITICAL_();

        if ((blk != (rui8_t *)0))
        {
            HAL_UART_Transmit(TRC_COM_PORT, blk, nbytes, 100);
        }
        else
        {
            break;
        }
    }
}
#endif

/* ------------------------------ File footer ------------------------------ */
