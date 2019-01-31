/**
 *  \file       bsp.c
 *  \brief      BSP for Tracking STM32
 *
 *  \ingroup    bsp
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2019.01.31  DaBa  v0.0.01  Initial version
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  DaBa  Dario Baliña       db@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */
#include <stdio.h>
#include "rkh.h"
#include "cubemx.h"
//#include "uartisr.h"
#include "bsp.h"

#include "signals.h"
#include "modpwr.h"
#include "modmgr.h"
#include "modcmd.h"
#include "conmgr.h"
#include "mTime.h"
#include "epoch.h"
#include "rtime.h"

RKH_THIS_MODULE

/* ----------------------------- Local macros ------------------------------ */
#define BlinkLed(b) HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, b)

/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
static RKH_TS_T tstamp;
static ModCmdRcvHandler cmdParser;

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ---------------------------- Global functions --------------------------- */
void
bsp_init(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    HAL_Init();

    SystemClock_Config();
    MX_GPIO_Init();
    MX_USART2_UART_Init();
    MX_USART3_UART_Init();
    MX_USB_OTG_HS_HCD_Init();
    MX_USART6_UART_Init();
    MX_USART1_UART_Init();
    MX_SPI1_Init();
    MX_CAN1_Init();
    MX_ADC1_Init();
    MX_SPI3_Init();

    modPwr_init();

//    rtime_init();
}

void
bsp_timeTick(void)
{
    ++tstamp;
    modPwr_ctrl();
//    mTime_tick();
}

RKH_TS_T
rkh_trc_getts(void)
{
    return tstamp;
}

static
void
gsm_rx_isr( unsigned char byte )
{
    cmdParser(byte);
}

void
bsp_serial_open(int ch)
{
    (void)ch;

    cmdParser = ModCmd_init();
    /* 
     * TODO: Configure Serial Port
     *
     * Ex: 
     * uartConfig(UART_232, 19200);
     * uartIsr_rxEnable(UART_232, gsm_rx_isr);
     *
     */
}

void
bsp_serial_close(int ch)
{
    (void)ch;
}

void
bsp_serial_puts(int ch, char *p)
{
    while(*p!='\0')
    {
        /* 
         * TODO: put char on  Serial Port
         * Ex: 
         * uartWriteByte(UART_232, *p);
         */
        ++p;
    }
}

void
bsp_serial_putnchar(int ch, unsigned char *p, ruint ndata)
{
    while(ndata && (ndata-- != 0))
    {
        /* 
         * TODO: put char on  Serial Port
         * Ex: 
         * uartWriteByte(UART_232, *p);
         */
        ++p;
    }
}

/* ------------------------------ File footer ------------------------------ */
