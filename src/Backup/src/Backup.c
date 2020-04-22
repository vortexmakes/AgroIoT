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

    fsResult = f_mkdir(BACKUP_FRMDIR);
    backInfo = defBackInfo;
    if ((fsResult == FR_OK) || (fsResult == FR_EXIST))
    {
        backInfo.nFiles = 0;
        fsResult = f_findfirst(&dir, &file, BACKUP_FRMDIR, "*.frm");
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
        /*f_closedir(&dir);*/
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

    if (status != (GStatus *)0)
    {
        /* Convert to frame? */
        if (backInfo.nFiles == 0)
        {
            sprintf(path, "%s/00000.frm", BACKUP_FRMDIR);
            fsResult = f_open(&fp, path, FA_CREATE_ALWAYS | 
                                         FA_WRITE | 
                                         FA_READ);
            if (fsResult == FR_OK)
            {
                backInfo.nFiles = 1;
                backInfo.oldest = backInfo.newest = 0;
                strcpy(backInfo.current, "00000.frm");
            }
            else
            {
                storeResult = 1;
            }
        }
        else
        {
        }
    }
    else
    {
        storeResult = 1;
    }

    return storeResult;
}

/* ------------------------------ End of file ------------------------------ */
