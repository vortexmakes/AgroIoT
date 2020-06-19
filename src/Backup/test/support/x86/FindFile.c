/**
 *  \file       FindFile.c
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
#include <dirent.h>

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
static DIR *dp;

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ---------------------------- Global functions --------------------------- */
char *
FindFile_findFirstFile(char *path)
{
    struct dirent *dir;
    char *file;

    file = NULL;
    if ((dp = opendir(path)) != NULL)
    {
        while ((dir = readdir(dp)) != NULL)
        {
            if ((strcmp(dir->d_name, ".") == 0) ||
                    (strcmp(dir->d_name, "..") == 0))
            {
                continue;
            }
            else
            {
                file = dir->d_name;
                break;
            }
        }
    }
    return file;
}

char *
FindFile_findNextFile(void)
{
    struct dirent *dir;
    char *file;

    file = NULL;
    while ((dir = readdir(dp)) != NULL)
    {
        if ((strcmp(dir->d_name, ".") == 0) ||
            (strcmp(dir->d_name, "..") == 0))
        {
            continue;
        }
        else
        {
            file = dir->d_name;
            break;
        }
    }
    return file;
}

void 
FindFile_closeDir(void)
{
    closedir(dp);
}

/* ------------------------------ End of file ------------------------------ */
