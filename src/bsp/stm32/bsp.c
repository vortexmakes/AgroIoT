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
#include <string.h>
#include "rkh.h"
#include "cubemx.h"
#include "bsp.h"

#include "signals.h"
#include "modpwr.h"
#include "dIn.h"
#include "dOut.h"
#include "modmgr.h"
#include "modcmd.h"
#include "tplhal.h"
#include "tplfsm.h"
#include "mTime.h"
#include "epoch.h"
#include "genled.h"
#include "ffile.h"
#include "Backup.h"
#include "trace_msd.h"

RKH_THIS_MODULE

/* ----------------------------- Local macros ------------------------------ */
#ifdef DEBUG
#define reset_now()		__asm volatile	("	bkpt 0x00FF\n" )
#else
#define reset_now()		NVIC_SystemReset()
#endif

#define BlinkLed(b)  HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, b)
#define ModemCTS(b)  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, b)
#define RS485_DIR(b) HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, b)
#define ExtPower()   HAL_GPIO_ReadPin(EXTPOWER_GPIO_Port, EXTPOWER_Pin)

/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
extern ApplicationTypeDef Appli_state;

/* ---------------------------- Local variables ---------------------------- */
static rui8_t bsp;
static RKH_TS_T tstamp;
static ModCmdRcvHandler gsmCmdParser;
static GpsRcvHandler    gpsParser;
static SIMSelect_t      simSelect;
static FATFS USBDISKFatFs;    /* File system object for USB disk logical drive */
static char USBDISKPath[4];   /* USB Host logical drive path */
static UsbHostStatus_t usbHostStatus = UsbHostClassDisconnect;
static UsbDiskStatus_t usbDiskStatus = UsbDiskNotReady;

static uint8_t uart1RxBuff[10];
static uint8_t uart2RxBuff[10];
static uint8_t uart3RxBuff[10];

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
static UART_HandleTypeDef *
getUartHandle(int ch)
{
    switch(ch)
    {
        case GSM_PORT:
            return &huart2;

        case GPS_PORT:
            return &huart3;

		case TPSENS_PORT:
            return &huart1;

		default:
            return NULL;
    }
}

static void
usbDisk_init(void)
{
    MX_FATFS_Init();
    USBDISKFatFs.fs_type = 0;
    usbHostStatus = UsbHostClassDisconnect;
    usbDiskStatus = UsbDiskNotReady;
}

/* ---------------------------- Global functions --------------------------- */
void
HAL_UART_TxCpltCallback(UART_HandleTypeDef *UartHandle)
{
    if (UartHandle->Instance == USART1)
    {
        tplink_tx_isr();
    }
}

void
HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
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
bsp_init(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    HAL_Init();

    SystemClock_Config();
    MX_GPIO_Init();
    MX_DMA_Init();
    MX_USART6_UART_Init();
    MX_CAN1_Init();
    MX_ADC1_Init();
    HAL_ADC_Start(&hadc1);
    MX_SPI3_Init();
    MX_USB_HOST_Init();
    MX_CRC_Init();

    modPwr_init();
    dIn_init();
    dOut_init();
    usbDisk_init();
}

void
bsp_reset(void)
{
    reset_now();
}

void
bsp_safeReset(void)
{
	ffile_sync();
    Backup_sync();
	RKH_TRC_FLUSH();
	trace_msd_close();
    reset_now();
}

void
bsp_timeTick(void)
{
    ++tstamp;
    mTime_tick();
	tplfsm_timer_isr();
}

RKH_TS_T
rkh_trc_getts(void)
{
    return epoch_get(); //tstamp;
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
    ruint len;

    len = strlen(p);
    bsp_serial_putnchar(ch, (unsigned char *)p, len);
}

void
bsp_serial_putnchar(int ch, unsigned char *p, ruint ndata)
{
    UART_HandleTypeDef *pUart;
    HAL_UART_StateTypeDef state;

    pUart = getUartHandle(ch);

    do
    {
        state = HAL_UART_GetState(pUart);
    }
    while((state == HAL_UART_STATE_BUSY_TX) ||
          (state == HAL_UART_STATE_BUSY_TX_RX));

    HAL_UART_Transmit_DMA(getUartHandle(ch), p, ndata);
}

void
bsp_serial_putchar(int ch, unsigned char c)
{
    HAL_UART_Transmit(getUartHandle(ch), &c, 1, 100);
}

void 
bsp_GSMModemFound(void)
{
    set_led(LED_GSM, SEQ_LSTAGE1);
}

void
bsp_set_battery(BatteryCtrl_t state)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	GPIO_InitStruct.Pin = PWROFF_Pin;

	if(state == Ready)
	{
		HAL_GPIO_WritePin(PWROFF_GPIO_Port, PWROFF_Pin, 0);
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init(PWROFF_GPIO_Port, &GPIO_InitStruct);
	}
	else
	{
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(PWROFF_GPIO_Port, &GPIO_InitStruct);
	}
}

ExtPwr_t
bsp_get_ExtPower(void)
{
#if 0
    return ExtPower();
#else
    uint32_t value;

    value = HAL_ADC_GetValue(&hadc1);
    HAL_ADC_Start(&hadc1);

    if(value < 90)
        return 0;
    else
        return 1;
#endif
}

void 
bsp_SIMReady(void)
{
    set_led(LED_GSM, SEQ_LSTAGE2);
}

void 
bsp_SIMSelect(SIMSelect_t sim)
{
    HAL_GPIO_WritePin(SIM_SELECT_GPIO_Port,
                              SIM_SELECT_Pin, 
                              sim == MainSIM ? 0 : 1);
}

void
bsp_SIMChange(void)
{
    simSelect = (simSelect == MainSIM) ? SecSIM : MainSIM;

    set_led(LED_SIM, simSelect == MainSIM ? SEQ_NO_LIT : SEQ_LIT);
    bsp_SIMSelect(simSelect);
}

void
bsp_regStatus(Status_t status)
{
    set_led(LED_GSM, status == RegisteredSt ? SEQ_LSTAGE3 : SEQ_LSTAGE2);
}

void 
bsp_netStatus(Status_t status)
{
    set_led(LED_GSM, status == ConnectedSt ? SEQ_LSTAGE4 : SEQ_LSTAGE3);
}

void 
bsp_sendFail(void)
{
    set_led(LED_GSM, SEQ_LSTAGE4);
}

void 
bsp_sendOk(void)
{

}

void
bsp_recvFail(void)
{
    set_led(LED_GSM, SEQ_LSTAGE4);
}

void 
bsp_recvOk(void)
{
    set_led(LED_GSM, SEQ_LIT);
}

void 
bsp_GPSStatus(char status)
{
    set_led(LED_GPS, status == RMC_StatusValid ? SEQ_LIT : SEQ_LSTAGE2);
}

void
bsp_gpsParserHandler_set(void *p)
{
    gpsParser = (GpsRcvHandler)p;
}

void
bsp_RS485_DIR(ruint val)
{
    RS485_DIR(val);
}

void
bsp_setDigOut(DigOutSignalId out, ruint val)
{
    switch(out)
    {
        case dOut1:
            HAL_GPIO_WritePin(OUTPUT1_GPIO_Port, OUTPUT1_Pin, val);
            return;

        case dOut2:
            HAL_GPIO_WritePin(OUTPUT2_GPIO_Port, OUTPUT2_Pin, val);
            return;

        default:
            return;
    }
}

ruint
bsp_getDigIn(DigInSignalId in)
{
    switch(in)
    {
        case dIn1:
            return HAL_GPIO_ReadPin(INPUT1_GPIO_Port, INPUT1_Pin);

        case dIn2:
            return HAL_GPIO_ReadPin(INPUT2_GPIO_Port, INPUT2_Pin);

        default:
            return 0;
    }
}

void
bsp_DigInPullSelect(DigInPullSelectId ps, uint8_t val)
{
    switch(ps)
    {
        case pullSelect1:
            HAL_GPIO_WritePin(PULL_SELECT1_GPIO_Port, PULL_SELECT1_Pin, val);
            break;

        case pullSelect2:
            HAL_GPIO_WritePin(PULL_SELECT2_GPIO_Port, PULL_SELECT2_Pin, val);
            break;

        default:
            break;
    }
}

void
bsp_usbDeviceEnable(void)
{
    usbHostStatus = UsbHostClassReady;

    if(USBDISKFatFs.fs_type == 0)
    {
        if(f_mount(&USBDISKFatFs, (TCHAR const*)USBDISKPath, 0) != FR_OK)
        {
            //Error_Handler();
            return;
        }
        usbDiskStatus = UsbDiskReady;
    }
}

void
bsp_usbDeviceDisable(void)
{
    usbHostStatus = UsbHostClassDisconnect;
    usbDiskStatus = UsbDiskNotReady;
    USBDISKFatFs.fs_type = 0;
    f_mount(0, "", 0);
}

UsbHostStatus_t
bsp_usbDeviceStatus(void)
{
    return usbHostStatus;
}

UsbDiskStatus_t
bsp_usbDiskStatus(void)
{
    return usbDiskStatus;
}

UsbHostStatus_t
bsp_usbHostStatus(void)
{
    if(Appli_state == APPLICATION_READY)
        return UsbHostClassReady; 

    return UsbHostClassDisconnect;
}

void
bsp_usbHostProcess(void)
{
//    MX_USB_HOST_Process();
}

void
bsp_setAllLeds(uint8_t val)
{
    HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, val);
    HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, val);
    HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, val);
    HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, val);
    HAL_GPIO_WritePin(LED5_GPIO_Port, LED5_Pin, val);
    HAL_GPIO_WritePin(LED6_GPIO_Port, LED6_Pin, val);
}

void
bsp_setErrLeds(uint8_t err)
{
	bsp_setAllLeds(0);

	switch(err)
	{
		case 1:
			HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, 1);
			break;
		case 2:
			HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, 1);
			break;
		case 3:
			HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, 1);
			HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, 1);
			break;

		default:
			break;
	}
}

/* ------------------------------ File footer ------------------------------ */
