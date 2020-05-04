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
static Backup backInfo = {0, 0, 0, "null", 0, 1};
static char name[12];
static char path[24];

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ---------------------------- Global functions --------------------------- */
BackupCode 
Backup_init(Backup *info)
{
    FRESULT fsResult;
    BackupCode initResult = Backup_Ok; 
    int fileNumber;
    char *pName;
    uint32_t oldest;

    backInfo.nFiles = backInfo.newest = 0;
    backInfo.error = Backup_Ok;
    backInfo.nWrites = 0;
    oldest = 99999;
    fsResult = f_mkdir(BACKUP_DIR_NAME);
    if ((fsResult == FR_OK) || (fsResult == FR_EXIST))
    {
        backInfo.nFiles = 0;
        fsResult = f_findfirst(&dir, &file, BACKUP_DIR_NAME, "*.frm");
        while ((fsResult == FR_OK) && (file.fname[0] != 0))
        {
            ++backInfo.nFiles;
            strcpy(name, file.fname);   /* Get file name excluding */
            pName = strtok(name, ".");  /* its extension */
            if (pName != (char *)0)
            {
                fileNumber = atoi(pName);
                if (fileNumber < oldest)
                {
                    oldest = fileNumber;
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
            sprintf(backInfo.current, "%05u.frm", backInfo.newest);
            strcpy(path, DIR_PATH);
            strcat(path, backInfo.current);
            fsResult = f_open(&fp, path, FA_OPEN_APPEND | FA_WRITE | FA_READ);
            if (fsResult != FR_OK)
            {
                backInfo.error = initResult = Backup_OpenFileError;
            }
        }
        else
        {
            oldest = 0;
        }
        f_closedir(&dir);
    }
    else
    {
        backInfo.error = initResult = Backup_DirError;
        oldest = 0;
    }

    backInfo.oldest = oldest;
    if (info != (Backup *)0)
    {
        *info = backInfo;
    }
    return initResult;
}

BackupCode  
Backup_deinit(Backup *info)
{
    backInfo.error = Backup_NoInit;
    if (info != (Backup *)0)
    {
        *info = backInfo;
    }
    return Backup_Ok;
}

void
Backup_getInfo(Backup *info)
{
    if (info != (Backup *)0)
    {
        *info = backInfo;
    }
}

BackupCode 
Backup_store(GStatus *status)
{
    FRESULT fsResult;
    BackupCode storeResult = Backup_Ok;
    UINT bytesWritten;
    int nFiles;
    uint32_t oldest;

    if ((status != (GStatus *)0) && (backInfo.error == Backup_Ok))
    {
        /* Convert to frame? */
        strcpy(path, DIR_PATH);
        if (backInfo.nFiles == 0)
        {
            /* Create the first file */
            strcat(path, FIRST_FILE_NAME);
            fsResult = f_open(&fp, path, FA_CREATE_ALWAYS | 
                                         FA_WRITE | 
                                         FA_READ);
            if (fsResult == FR_OK)
            {
                backInfo.nWrites = 0;
                backInfo.nFiles = 1;
                backInfo.oldest = backInfo.newest = 0;
                strcpy(backInfo.current, FIRST_FILE_NAME);
            }
            else
            {
                storeResult = Backup_FailToCreateFirstFile;
            }
        }
        else
        {
            /* Does it align the file size to BACKUP_SIZEOF_REG? */
            if (f_size(&fp) >= (BACKUP_MAXNUMREGPERFILE * 
                                BACKUP_SIZEOF_REG))
            {
                f_close(&fp);
                nFiles = backInfo.nFiles;
                oldest = backInfo.oldest;
                if (backInfo.nFiles >= BACKUP_MAXNUMFILES)
                {
                    /* Recycle the oldest file */
                    sprintf(path, "%s%05u.frm", DIR_PATH, backInfo.oldest);
                    f_unlink(path);
                    ++oldest;
                }
                else
                {
                    ++nFiles;
                }
                strcpy(path, DIR_PATH);
                sprintf(name, "%05u.frm", backInfo.newest + 1);
                strcat(path, name);
                fsResult = f_open(&fp, path, FA_CREATE_ALWAYS | 
                                             FA_WRITE | 
                                             FA_READ);
                if (fsResult == FR_OK)
                {
                    backInfo.nWrites = 0;
                    backInfo.nFiles = nFiles;
                    ++backInfo.newest;
                    backInfo.oldest = oldest;
                    strcpy(backInfo.current, name);
                }
                else
                {
                    storeResult = Backup_FailToCreateNewFile;
                }
            }
        }
        if (storeResult == 0)
        {
            fsResult = f_write(&fp, status, BACKUP_SIZEOF_REG, &bytesWritten);
            if ((fsResult != FR_OK) || (bytesWritten != BACKUP_SIZEOF_REG))
            {
                storeResult = Backup_WriteError;
            }
            else
            {
                ++backInfo.nWrites;
                if (backInfo.nWrites >= BACKUP_NUMWRITES)
                {
                    backInfo.nWrites = 0;
                    f_sync(&fp);
                }
            }
        }
    }
    else
    {
        storeResult = Backup_WrongArgsInitError;
    }

    return storeResult;
}

/* ------------------------------ End of file ------------------------------ */
