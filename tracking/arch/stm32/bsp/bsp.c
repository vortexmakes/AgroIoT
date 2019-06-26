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
 *  DaBa  Dario Bali�a       db@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */
#include <stdio.h>
#include "rkh.h"
#include "cubemx.h"
#include "bsp.h"

#include "signals.h"
#include "modpwr.h"
#include "dIn.h"
#include "dOut.h"
#include "modmgr.h"
#include "modcmd.h"
#include "conmgr.h"
#include "mTime.h"
#include "epoch.h"
#include "genled.h"

RKH_THIS_MODULE

/* ----------------------------- Local macros ------------------------------ */
#define BlinkLed(b)  HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, b)
#define ModemCTS(b)  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, b)

/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
static rui8_t bsp;
static RKH_TS_T tstamp;
static ModCmdRcvHandler gsmCmdParser;
static GpsRcvHandler    gpsParser;
static SIMSelect_t      simSelect;

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
    MX_USB_OTG_HS_HCD_Init();
    MX_USART6_UART_Init();
    MX_USART1_UART_Init();
    MX_CAN1_Init();
    MX_ADC1_Init();
    MX_SPI3_Init();

    modPwr_init();
    dIn_init();
    dOut_init();

    ffile_init();
}

void
bsp_timeTick(void)
{
    ++tstamp;
    mTime_tick();
}

RKH_TS_T
rkh_trc_getts(void)
{
    return tstamp;
}

static uint8_t uart1RxBuff[10];
static uint8_t uart2RxBuff[10];
static uint8_t uart3RxBuff[10];

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
    if (UartHandle->Instance == USART1)
    {
        tplink_rx_isr(uart1RxBuff[0]);

        HAL_UART_Receive_IT(&huart1, uart1RxBuff, 1);
    }
    else if (UartHandle->Instance == USART2)
    {
        if (gsmCmdParser != NULL)
        {
            gsmCmdParser(uart2RxBuff[0]);
        }

        HAL_UART_Receive_IT(&huart2, uart2RxBuff, 1);
    }
    else if (UartHandle->Instance == USART3)
    {
        if (gpsParser != NULL)
        {
            gpsParser(uart3RxBuff[0]);
        }

        HAL_UART_Receive_IT(&huart3, uart3RxBuff, 1);
    }
}

void
bsp_serial_open(int ch)
{
    (void)ch;

    switch(ch)
    {
        case GSM_PORT:
            gsmCmdParser = ModCmd_init();
            MX_USART2_UART_Init();
            HAL_UART_Receive_IT(&huart2, uart2RxBuff, 1);
            ModemCTS(1);
            break;

        case GPS_PORT:
            gpsParser = NULL;
            MX_USART3_UART_Init();
            HAL_UART_Receive_IT(&huart3, uart3RxBuff, 1);
			break;

		case TPSENS_PORT:
            MX_USART1_UART_Init();
            HAL_UART_Receive_IT(&huart1, uart1RxBuff, 1);
			break;

		default:
			break;
    }
        
    RKH_TR_FWK_ACTOR(&bsp, "bsp");
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
        bsp_serial_putchar(ch, *p);
        ++p;
    }
}

void
bsp_serial_putnchar(int ch, unsigned char *p, ruint ndata)
{
    while(ndata && (ndata-- != 0))
    {
        bsp_serial_putchar(ch, *p);
        ++p;
    }
}

void
bsp_serial_putchar(int ch, unsigned char c)
{
    UART_HandleTypeDef *pUart;

    switch(ch)
    {
        case GSM_PORT:
            pUart = &huart2;
            break;

        case GPS_PORT:
            pUart = &huart3;
			break;

		case TPSENS_PORT:
			break;

		default:
			break;
    }
    
    HAL_UART_Transmit(pUart, &c, 1, 100);
}

void 
bsp_GSMModemFound(void)
{
    set_led(LED_GSM, LSTAGE1);
}

void 
bsp_SIMReady(void)
{
    set_led(LED_GSM, LSTAGE2);
}

void 
bsp_SIMSelect(SIMSelect_t sim)
{
    simSelect = sim;

    HAL_GPIO_WritePin(SIM_SELECT_GPIO_Port,
                              SIM_SELECT_Pin, 
                              sim == MainSIM ? 0 : 1);

    set_led(LED_SIM, simSelect ? LSTAGE2 : LSTAGE1);
}

void
bsp_SIMChange(void)
{
    simSelect = (simSelect == MainSIM) ? SecSIM : MainSIM;

    bsp_SIMSelect(simSelect);
}

void
bsp_regStatus(Status_t status)
{
    set_led(LED_GSM, status == RegisteredSt ? LSTAGE3 : LSTAGE2);
}

void 
bsp_netStatus(Status_t status)
{
    set_led(LED_GSM, status == ConnectedSt ? LSTAGE4 : LSTAGE3);
}

void 
bsp_sendFail(void)
{
    set_led(LED_GSM, LSTAGE4);
}

void 
bsp_sendOk(void)
{

}

void
bsp_recvFail(void)
{
    set_led(LED_GSM, LSTAGE4);
}

void 
bsp_recvOk(void)
{
    set_led(LED_GSM, LIT);
}

void 
bsp_GPSStatus(char status)
{
    set_led(LED_GPS, status == RMC_StatusValid ? LIT : LSTAGE2);
}

void
bsp_gpsParserHandler_set(void *p)
{
    gpsParser = (GpsRcvHandler)p;
}

/* ------------------------------ File footer ------------------------------ */
