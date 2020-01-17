/**
 *  \file       trace_io_msd.c
 *  \brief      USB Mass Storage trace support 
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
#include "rtime.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
#define TRACE_FILE_MAX_SIZE     (100*1024)

/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
FATFS USBDISKFatFs;    /* File system object for USB disk logical drive */
FIL File;              /* File object */
char USBDISKPath[4];   /* USB Host logical drive path */
char fileName[25];

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
#include "ff.h"
#include "StatQue.h"
#include "GStatus.h"



void
dumpStatQueueToMsd(void)
{
	FIL dumpFile;
	GPS_STR from;
    uint32_t byteswritten;

    while(bsp_usbDeviceStatus() != UsbHostClassReady);

    if(f_open(&dumpFile, "StatQueue.dump", FA_CREATE_ALWAYS | FA_WRITE) != FR_OK)
    {
        //Error_Handler();
        return;
    }

    while( StatQue_read(&from) == 0)
    {
        f_write(&dumpFile, &from, sizeof(GPS_STR), (void *)&byteswritten);
    }

    f_sync(&dumpFile);

    f_close(&dumpFile);
}

/* ---------------------------- Global functions --------------------------- */
void
trace_msd_init(void)
{
    USBDISKFatFs.fs_type = 0;
}

void
trace_msd_write(uint8_t *pData, uint16_t Size)
{
    FRESULT res;
    uint32_t byteswritten;
    FSIZE_t fileSize;

    if(bsp_usbDeviceStatus() != UsbHostClassReady)
    {
        USBDISKFatFs.fs_type = 0;
        File.obj.fs = NULL;
        return;
    }

    if(USBDISKFatFs.fs_type == 0)
    {
        if(f_mount(&USBDISKFatFs, (TCHAR const*)USBDISKPath, 0) != FR_OK)
        {
            //Error_Handler();
            return;
        }
    }

    //dumpStatQueueToMsd();

    if(File.obj.fs == NULL)
    {
        sprintf(fileName,"trc%lu.bin", epoch_get());

        if(f_open(&File, fileName, FA_CREATE_ALWAYS | FA_WRITE) != FR_OK)
        {
            //Error_Handler();
            File.obj.fs = NULL;
            return;
        }
    }

    if((USBDISKFatFs.fs_type != 0) && (File.obj.fs != NULL))
    {
        res = f_write(&File, pData, Size, (void *)&byteswritten);

        if((byteswritten != Size) || (res != FR_OK))
        {
            //Error_Handler();
            f_close(&File);
            File.obj.fs = NULL;
            return;
        }
        
        fileSize = f_size(&File);
        if(fileSize > TRACE_FILE_MAX_SIZE)
        {
            f_close(&File);
            File.obj.fs = NULL;
            return;
        }
    }
}

/* ------------------------------ File footer ------------------------------ */
