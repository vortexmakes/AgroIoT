/**
 *  \file       FileMgr.c
 *  \brief      ...
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
#include <ftw.h>
#include <errno.h>
#include "Backup.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
#define FRAME_DIR_PATH          "./test/support/ff"

/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
static GStatus status = 
{
    {
        {
            "000000", {'V'}, {"38.0030396"}, {"-"}, {"057.3266218"}, {"-"},
            {"000.000"}, {"000"}, "000000"
        },
        {0, 0, {0, 0, 0}, {0, 0, 0}, 0},
        {0, 0},
        0
    },
    0
};
static char filePath[128];
static char dirPath[64];

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ---------------------------- Global functions --------------------------- */
static int 
unlink_cb(const char *fpath, const struct stat *sb, int typeflag, 
          struct FTW *ftwbuf)
{
    int rv = remove(fpath);

    if (rv)
    {
        perror(fpath);
    }

    return rv;
}

int 
FileMgr_rmrf(void)
{
    return nftw(dirPath, unlink_cb, 64, FTW_DEPTH | FTW_PHYS);
}

void
FileMgr_createFiles(int nFiles)
{
    int i;
    FILE *file;

    mkdir(dirPath, 0777);
    for (i = 0; i < nFiles; ++i)
    {
        sprintf(filePath, "%s/%05d.frm", dirPath, i);
        file = fopen(filePath, "w+");
        if (file == NULL)
        {
            printf("%s\n", strerror(errno));
            break;
        }
        fclose(file);
    }
}

void 
FileMgr_cd(char *path)
{
    sprintf(dirPath, "%s/%s", FRAME_DIR_PATH, path);
}

void
FileMgr_fillFile(char *path)
{
    FILE *file;
    int i;

    sprintf(filePath, "%s/%s/%s", FRAME_DIR_PATH, BACKUP_DIR_NAME, path);
    file = fopen(filePath, "w+");
    if (file != NULL)
    {
        for (i = 0; i < BACKUP_MAXNUMREGPERFILE; ++i)
        {
            fwrite(&status, BACKUP_SIZEOF_REG, 1, file);
        }
        fflush(file);
        fclose(file);
    }
}

/* ------------------------------ End of file ------------------------------ */
