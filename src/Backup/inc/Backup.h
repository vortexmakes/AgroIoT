/**
 *  \file       Backup.h
 *  \brief      Specification of system status backup
 */

/* -------------------------- Development history -------------------------- */
/*
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  LeFr  Leandro Francucci lf@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* --------------------------------- Module -------------------------------- */
#ifndef __BACKUP_H__
#define __BACKUP_H__

/* ----------------------------- Include files ----------------------------- */
#include "GStatus.h"

/* ---------------------- External C language linkage ---------------------- */
#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------- Macros -------------------------------- */
/* -------------------------------- Constants ------------------------------ */
#define BACKUP_DIR_NAME             "frames"
#define BACKUP_MAXNUMFILES          40
#define BACKUP_MAXNUMREGPERFILE     15600 /* 13[h]*60[m/h]*60[s/m]/3[s] */
#define BACKUP_SIZEOF_REG           sizeof(GStatus)
#define BACKUP_NUMWRITES            1200

/* ------------------------------- Data types ------------------------------ */
typedef enum BackupCode BackupCode;
enum BackupCode
{
    Backup_Ok, 
    Backup_NoInit,
    Backup_OpenFileError,
    Backup_DirError,
    Backup_WrongArgsInitError,
    Backup_WriteError,
    Backup_FailToCreateFirstFile,
    Backup_FailToCreateNewFile
};

typedef enum FileState FileState;
enum FileState
{
    BackupOpen, BackupClosed
};

typedef struct Backup Backup;
struct Backup
{
    int nFiles;
    uint32_t oldest;
    uint32_t newest;
    char current[12];
    FileState state;
    uint32_t nWrites;
    BackupCode error;
};

/* -------------------------- External variables --------------------------- */
/* -------------------------- Function prototypes -------------------------- */
BackupCode Backup_init(Backup *info);
BackupCode Backup_deinit(Backup *info);
void Backup_getInfo(Backup *info);
BackupCode Backup_store(GStatus *status);
BackupCode Backup_sync(void);

/* -------------------- External C language linkage end -------------------- */
#ifdef __cplusplus
}
#endif

/* ------------------------------ Module end ------------------------------- */
#endif

/* ------------------------------ End of file ------------------------------ */
