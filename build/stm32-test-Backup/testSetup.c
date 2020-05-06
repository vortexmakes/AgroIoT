/**
 *  \file       testSetup.c
 *  \brief      .
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2018.05.17  DaBa  v1.0.00  Initial version
  */

/* -------------------------------- Authors -------------------------------- */
/*
 *  DaBa  Dario Baliña       db@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */
#include "cubemx.h"
#include "stdio.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
extern ApplicationTypeDef Appli_state;
static FATFS USBDISKFatFs;    /* File system object for USB disk logical drive */
static char USBDISKPath[4];   /* USB Host logical drive path */

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
static void
usbDisk_init(void)
{
    MX_FATFS_Init();
    USBDISKFatFs.fs_type = 0;
}

/* ---------------------------- Global functions --------------------------- */
/*
 * Unity printout on UART5 at CubeMX configuration (115200 8N1)
 */
int
__io_putchar(int ch)
{
	while(HAL_UART_GetState(&huart5) != HAL_UART_STATE_READY);

	HAL_UART_Transmit_DMA(&huart5, (uint8_t *)(&ch), 1);

	return ch;
}

/*
 * Pre main() call Unity hook for hardware initialization.
 */
void
suiteSetUp(void)
{
    HAL_Init();

    SystemClock_Config();
    MX_GPIO_Init();
    MX_DMA_Init();
    MX_UART5_Init();
    MX_CAN1_Init();
    MX_ADC1_Init();
    MX_SPI3_Init();
    MX_USB_HOST_Init();

    usbDisk_init();

    printf("Detecting USBMassStorage device...\r");
    while(Appli_state != APPLICATION_READY)
        MX_USB_HOST_Process();

    if(f_mount(&USBDISKFatFs, (TCHAR const*)USBDISKPath, 0) != FR_OK)
    {
         printf("Error mounting disk\r");
         printf("test ABORTED\r");
         __asm volatile	("	bkpt 0x00FF\n" );
    }
}

/*
 * Post main() call Unity.
 */
int
suiteTearDown(int num_failures)
{
	while(1)
		;

	return 0;
}

/* ------------------------------ End of file ------------------------------ */
