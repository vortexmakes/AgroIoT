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
#include "FindFile.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
#define FRAME_DIR_PATH      "./test/support/ff"
#define APLUS               (0x30 | 0x01 | 0x02)
#define WPLUS               (0x08 | 0x01 | 0x02)

/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
static FILE *file;
static char filePath[64], dirPath[64];

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ---------------------------- Global functions --------------------------- */
FRESULT 
f_open(FIL* fp, const TCHAR* path, BYTE mode)
{
    FRESULT result = FR_OK;
    int pos, size;

    sprintf(filePath, "%s/%s", FRAME_DIR_PATH, path);
    file = fopen(filePath, (mode == WPLUS) ? "w+": "a+");
    if (file == NULL)
    {
        result = FR_DISK_ERR;
    }
    else
    {
        pos = ftell(file);
        fseek(file, 0, SEEK_END);
        size = ftell(file);
        fseek(file, pos, SEEK_CUR);
        fp->obj.objsize = size;
    }
    return result;
}

FRESULT 
f_close (FIL* fp)
{
    fclose(file);
    return FR_OK;
}

FRESULT 
f_write (FIL* fp, const void* buff, UINT btw, UINT* bw)
{
    int bytesWritten;

    bytesWritten = fwrite(buff, 1, btw, file);
    if (bw != (UINT *)0)
    {
        *bw = bytesWritten;
    }
    return FR_OK;
}

FRESULT 
f_sync (FIL* fp)
{
    fflush(file);
    return FR_OK;
}

FRESULT 
f_closedir (DIR* dp)
{
    FindFile_closeDir();
    return FR_OK;
}

FRESULT 
f_findfirst (DIR* dp, FILINFO* fno, const TCHAR* path, const TCHAR* pattern)
{
    FRESULT result = FR_OK;
    char *fileName;

    sprintf(dirPath, "%s/%s", FRAME_DIR_PATH, path);
    fileName = FindFile_findFirstFile((char *)dirPath);
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

    fileName = FindFile_findNextFile();
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
    sprintf(filePath, "%s/%s", FRAME_DIR_PATH, path);
    remove(filePath);
    return FR_OK;
}

/* ------------------------------ End of file ------------------------------ */
