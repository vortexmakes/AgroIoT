/**
 *  \file       Backup.c
 *  \brief      Implementation of system status backup 
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
#include <stdio.h>
#include "Backup.h"
#include "ff.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
#define FIRST_FILE_NAME         "00000.frm"
#define DIR_PATH                BACKUP_DIR_NAME"/"

/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
static DIR dir;
static FILINFO file;
static FIL fp;
static Backup backInfo;
static const Backup defBackInfo = {-1, -1, -1};

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ---------------------------- Global functions --------------------------- */
int
Backup_init(Backup *info)
{
    FRESULT fsResult;
    int initResult = 0, fileNumber;
    char *pName, name[12];

    fsResult = f_mkdir(BACKUP_DIR_NAME);
    backInfo = defBackInfo;
    if ((fsResult == FR_OK) || (fsResult == FR_EXIST))
    {
        backInfo.nFiles = 0;
        fsResult = f_findfirst(&dir, &file, BACKUP_DIR_NAME, "*.frm");
        while ((fsResult == FR_OK) && (file.fname[0] != 0))
        {
            ++backInfo.nFiles;
            backInfo.oldest = backInfo.newest = 0;
            strcpy(name, file.fname);   /* Get file name excluding */
            pName = strtok(name, ".");   /* its extension */
            if (pName != (char *)0)
            {
                fileNumber = atoi(pName);
                if (fileNumber < backInfo.oldest)
                {
                    backInfo.oldest = fileNumber;
                }
                if (fileNumber > backInfo.newest)
                {
                    backInfo.newest = fileNumber;
                }
            }
            fsResult = f_findnext(&dir, &file);
        }
        if (backInfo.nFiles > 0)
        {
            sprintf(backInfo.current, "%05d.frm", backInfo.newest);
        }
        f_closedir(&dir);
    }
    else
    {
        initResult = 1;
    }

    if (info != (Backup *)0)
    {
        *info = backInfo;
    }
    return initResult;
}

void
Backup_getInfo(Backup *info)
{
    if (info != (Backup *)0)
    {
        *info = backInfo;
    }
}

int
Backup_store(GStatus *status)
{
    FRESULT fsResult;
    int storeResult = 0;
    char path[24];
    UINT bytesWritten;

    if (status != (GStatus *)0)
    {
        /* Convert to frame? */
        strcpy(path, DIR_PATH);
        if (backInfo.nFiles == 0)
        {
            strcat(path, FIRST_FILE_NAME);
            fsResult = f_open(&fp, path, FA_CREATE_ALWAYS | 
                                         FA_WRITE | 
                                         FA_READ);
            if (fsResult == FR_OK)
            {
                backInfo.nFiles = 1;
                backInfo.oldest = backInfo.newest = 0;
                strcpy(backInfo.current, FIRST_FILE_NAME);
            }
            else
            {
                storeResult = 1;
            }
        }
        else
        {
            strcat(path, backInfo.current);
            fsResult = f_open(&fp, path, FA_OPEN_APPEND | FA_WRITE | FA_READ);
            if (fsResult == FR_OK)
            {
                if (f_size(&fp) >= (BACKUP_MAXNUMREGPERFILE * sizeof(GStatus)))
                {
                    sprintf(path, "%s/%05d.frm", BACKUP_DIR_NAME, 
                                                 backInfo.newest + 1);
                    fsResult = f_open(&fp, path, FA_CREATE_ALWAYS | 
                                                 FA_WRITE | 
                                                 FA_READ);
                    if (fsResult == FR_OK)
                    {
                        ++backInfo.nFiles;
                        ++backInfo.newest;
                        sprintf(backInfo.current, "%05d.frm", backInfo.newest);
                    }
                    else
                    {
                        storeResult = 1;
                    }
                }
            }
            else
            {
                storeResult = 1;
            }
        }
        if (storeResult == 0)
        {
            fsResult = f_write(&fp, status, sizeof(GStatus), &bytesWritten);
            if ((fsResult != FR_OK) || (bytesWritten != sizeof(GStatus)))
            {
                storeResult = 1;
            }
        }
    }
    else
    {
        storeResult = 1;
    }

    return storeResult;
}

/* ------------------------------ End of file ------------------------------ */
