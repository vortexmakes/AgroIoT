/**
 *  \file       ff.c
 *  \brief      Trivial stub for fatfs on Linux platform
 */

/* -------------------------- Development history -------------------------- */
/*
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  LeFr  Leandro Francucci lf@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <errno.h>
#include <ftw.h>
#include "ff.h"
#include "findfile.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
#define FRAME_DIR_PATH          "./test/support/ff"

/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ---------------------------- Global functions --------------------------- */
FRESULT 
f_open(FIL* fp, const TCHAR* path, BYTE mode)
{
    return FR_DISK_ERR;
}

FRESULT 
f_close (FIL* fp)
{
    return FR_DISK_ERR;
}

FRESULT 
f_write (FIL* fp, const void* buff, UINT btw, UINT* bw)
{
    return FR_DISK_ERR;
}

FRESULT 
f_sync (FIL* fp)
{
    return FR_DISK_ERR;
}

FRESULT 
f_closedir (DIR* dp)
{
    closeDir();
    return FR_OK;
}

FRESULT 
f_findfirst (DIR* dp, FILINFO* fno, const TCHAR* path, const TCHAR* pattern)
{
    FRESULT result = FR_OK;
    char *fileName;
    char dirPath[64];

    sprintf(dirPath, "%s/%s", FRAME_DIR_PATH, path);
    fileName = findFirstFile((char *)dirPath);
    if (fileName != NULL)
    {
        strcpy(fno->fname, fileName);
    }
    else
    {
        fno->fname[0] = '\0';
        result = FR_DISK_ERR;
    }
    return result;
}

FRESULT 
f_findnext (DIR* dp, FILINFO* fno)
{
    FRESULT result = FR_OK;
    char *fileName;

    fileName = findNextFile();
    if (fileName != NULL)
    {
        strcpy(fno->fname, fileName);
    }
    else
    {
        fno->fname[0] = '\0';
        result = FR_DISK_ERR;
    }
    return result;
}

FRESULT 
f_mkdir(const TCHAR* path)
{
    int status;
    FRESULT result = FR_OK;
    char dirPath[64];

    sprintf(dirPath, "%s/%s", FRAME_DIR_PATH, path);
    status = mkdir(dirPath, 0777);
    if (status < 0)
    {
        if (errno == EEXIST)
        {
            result = FR_EXIST;
        }
        else
        {
            result = FR_DISK_ERR;
        }
    }
    return result;
}

FRESULT 
f_unlink (const TCHAR* path)
{
    return FR_DISK_ERR;
}

/* ------------------------------ End of file ------------------------------ */
