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
#define printOffSize(m,po,o) \
        {                   \
            printf("%-27s%5d%8d\r\n", #m, po+o, sizeof(m)); \
        }

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
gstatusOffsetSize(void)
{
    GStatus gs;

	printf("GStatus members offsets and sizes\r\n");

    printf("%-27s%-10s%-10s\r\n", "attribute", "offset", "size");

    printOffSize(gs, 0, 0);
    printOffSize(gs.data, 0, offsetof(GStatus, data));
	int dataOff = offsetof(GStatus, data);

    printOffSize(gs.data.position, dataOff, offsetof(GStatusType, position));
    int posOff = dataOff + offsetof(GStatusType, position);

    printOffSize(gs.data.position.utc, posOff, offsetof(Geo, utc));
    printOffSize(gs.data.position.status, posOff, offsetof(Geo, status));
    printOffSize(gs.data.position.latitude, posOff, offsetof(Geo, latitude));
    printOffSize(gs.data.position.latInd, posOff, offsetof(Geo, latInd));
    printOffSize(gs.data.position.longitude, posOff, offsetof(Geo, longitude));
    printOffSize(gs.data.position.longInd, posOff, offsetof(Geo, longInd));
    printOffSize(gs.data.position.speed, posOff, offsetof(Geo, speed));
    printOffSize(gs.data.position.course, posOff, offsetof(Geo, course));
    printOffSize(gs.data.position.date, posOff, offsetof(Geo, date));

	printOffSize(gs.data.devData, dataOff, offsetof(GStatusType, devData));
	int devDataOff= dataOff + offsetof(GStatusType, devData);

	printOffSize(gs.data.devData.cmd, devDataOff, offsetof(CBOX_STR, cmd));
	printOffSize(gs.data.devData.m, devDataOff, offsetof(CBOX_STR, m));

	printOffSize(gs.data.devData.h, devDataOff, offsetof(CBOX_STR, h));
	int hOff= devDataOff + offsetof(CBOX_STR, h);

	printOffSize(gs.data.devData.h.hoard, hOff, offsetof(GRSENS_T, hoard));
	printOffSize(gs.data.devData.h.pqty, hOff, offsetof(GRSENS_T, pqty));
	printOffSize(gs.data.devData.h.flow, hOff, offsetof(GRSENS_T, flow));

	printOffSize(gs.data.devData.a, devDataOff, offsetof(CBOX_STR, a))
	int aOff= devDataOff + offsetof(CBOX_STR, a);

	printOffSize(gs.data.devData.a.x, aOff, offsetof(ACCEL_T, x));
	printOffSize(gs.data.devData.a.y, aOff, offsetof(ACCEL_T, y));
	printOffSize(gs.data.devData.a.z, aOff, offsetof(ACCEL_T, z));
	printOffSize(gs.data.devData.a.m, aOff, offsetof(ACCEL_T, m));

	printOffSize(gs.data.devData.hum, devDataOff, offsetof(CBOX_STR, hum));
		
    printOffSize(gs.data.ioStatus, dataOff, offsetof(GStatusType, ioStatus));
	int iosOff= dataOff + offsetof(GStatusType, ioStatus);

	printOffSize(gs.data.ioStatus.digIn, iosOff, offsetof(IOStatus, digIn));
	printOffSize(gs.data.ioStatus.digOut, iosOff, offsetof(IOStatus, digOut));

    printOffSize(gs.data.batChrStatus, dataOff, offsetof(GStatusType, batChrStatus));

	printOffSize(gs.checksum, 0, offsetof(GStatus, checksum));

	printf("\r\n");
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

    gstatusOffsetSize();

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
