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
#include "Backup.h"
#include "ff.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
static DIR dir;
static FILINFO file;
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

/* ------------------------------ End of file ------------------------------ */
