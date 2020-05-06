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
#include "GStatus.h"

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

static
void
logOffsets(void)
{
	int dataOff;
		int positionOff;
		int devDataOff;
		int ioStatusOff;
		int BatchrStatusOff;

	dataOff = offsetof(GStatus, data);
		positionOff = dataOff + offsetof(GStatusType, position);
		devDataOff= dataOff + offsetof(GStatusType, devData);
		ioStatusOff= dataOff + offsetof(GStatusType, ioStatus);
		BatchrStatusOff= dataOff + offsetof(GStatusType, ioStatus);

	printf("GStatus members offsets\r\n", dataOff);
	printf("GStatus.data %d\r\n", dataOff);
		printf("GStatus.data.position %d\r\n", positionOff);
			printf("GStatus.data.position.utc %d\r\n", positionOff + offsetof(Geo, utc));
			printf("GStatus.data.position.status %d\r\n", positionOff + offsetof(Geo, status));
			printf("GStatus.data.position.latitude %d\r\n", positionOff + offsetof(Geo, latitude));
			printf("GStatus.data.position.latInd %d\r\n", positionOff + offsetof(Geo, latInd));
			printf("GStatus.data.position.longitude %d\r\n", positionOff + offsetof(Geo, longitude));
			printf("GStatus.data.position.longInd %d\r\n", positionOff + offsetof(Geo, longInd));
			printf("GStatus.data.position.speed %d\r\n", positionOff + offsetof(Geo, speed));
			printf("GStatus.data.position.course %d\r\n", positionOff + offsetof(Geo, course));
			printf("GStatus.data.position.date %d\r\n", positionOff + offsetof(Geo, date));
		printf("GStatus.data.devData %d\r\n", devDataOff);
			printf("GStatus.data.devData.cmd %d\r\n", devDataOff + offsetof(CBOX_STR, cmd));
			printf("GStatus.data.devData.m %d\r\n", devDataOff + offsetof(CBOX_STR, m));
			int hOff= devDataOff + offsetof(CBOX_STR, h);
			printf("GStatus.data.devData.h %d\r\n", hOff);
				printf("GStatus.data.devData.h.hoard %d\r\n", hOff + offsetof(GRSENS_T, hoard));
				printf("GStatus.data.devData.h.pqty %d\r\n", hOff + offsetof(GRSENS_T, pqty));
				printf("GStatus.data.devData.h.flow %d\r\n", hOff + offsetof(GRSENS_T, flow));
			int aOff= devDataOff + offsetof(CBOX_STR, a);
			printf("GStatus.data.devData.a %d\r\n", aOff);
				printf("GStatus.data.devData.a.x %d\r\n", aOff + offsetof(ACCEL_T, x));
				printf("GStatus.data.devData.a.y %d\r\n", aOff + offsetof(ACCEL_T, y));
				printf("GStatus.data.devData.a.z %d\r\n", aOff + offsetof(ACCEL_T, z));
				printf("GStatus.data.devData.a.m %d\r\n", aOff + offsetof(ACCEL_T, m));
			printf("GStatus.data.devData.hum %d\r\n", devDataOff + offsetof(CBOX_STR, hum));
		printf("GStatus.data.ioStatus %d\r\n", ioStatusOff);
			printf("GStatus.data.ioStatus.digIn %d\r\n", ioStatusOff + offsetof(IOStatus, digIn));
			printf("GStatus.data.ioStatus.digOuy %d\r\n", ioStatusOff + offsetof(IOStatus, digOut));
		printf("GStatus.data.batChrStatus %d\r\n", BatchrStatusOff);
	printf("GStatus.checksum %d\r\n", offsetof(GStatus, checksum));

	printf("\r\n", dataOff);
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

    logOffsets();

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
