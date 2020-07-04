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
#define TRACE_FILE_MAX_SIZE     (1024*1024)

/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
static FIL File;              /* File object */
static char fileName[25];
static ruint isopen = 0;

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ---------------------------- Global functions --------------------------- */
void
trace_msd_init(void)
{
    isopen = 1;
}

void
trace_msd_close(void)
{
	isopen = 0;

	if(File.obj.fs != NULL)
	{
		f_close(&File);
	}
}

void
trace_msd_write(uint8_t *pData, uint16_t Size)
{
    FRESULT res;
    uint32_t byteswritten;
    FSIZE_t fileSize;

    if(!isopen)
    	return;

    if(bsp_usbDiskStatus() != UsbDiskReady)
    {
        File.obj.fs = NULL;
        return;
    }

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

void
trace_msd_sync(void)
{
	if(File.obj.fs != NULL)
		f_sync(&File);
}

/* ------------------------------ File footer ------------------------------ */
