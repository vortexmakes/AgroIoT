/**
 *  \file       rfile.h
 *  \brief
 */

/* -------------------------- Development history -------------------------- */
/*
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  LeFr  Leandro Francucci  lf@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* --------------------------------- Module -------------------------------- */
#ifndef __RFILE_H__
#define __RFILE_H__

/* ----------------------------- Include files ----------------------------- */
/**
 *  Application specific configuration options.
 */
#include "ffilecfg.h"

/**
 *  Specific definitions to the platform being used.
 */
#include "ffplat.h"
#include "fftype.h"

/* ---------------------- External C language linkage ---------------------- */
#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------- Macros -------------------------------- */
/**
 *  \brief
 *	Macro that creates a new flash file structure into directory.
 *
 *  \param fd	file descriptor. Represents a new flash file into directory.
 *  \param 't   type of flash file. It can have the following values:
 *              QFILE_TYPE or RFILE_TYPE. The file type allows to determine
 *              whether the file access is queue or random.
 *  \param np   number of pages. It's defines the size of file.
 *  \param bp   begin page.
 *	\param sr   size of register. Number of bytes each register in the file
 *				will require. Registers are stored by copy, not by reference,
 *				so this is the number of bytes that will be stored for each
 *				written register. Each register on the file must be the same
 *				size.
 */
#define CREATE_FFILE(fd, t, np, bp, sr) \
    { \
        fd, t, 0, np, bp, \
        (SA_T)(((SA_T)(bp) * FF_PHYS_BLOCK_SIZE) + RF_FB_ADDRESS), \
        (ffui16_t)sr, (NR_T)(RF_SIZE_EFF_PAGE / (ffui16_t)(sr)), \
        (NR_T)((RF_SIZE_EFF_PAGE / (ffui16_t)(sr)) * np), \
        0, 0, 0 \
    }

/**
 *  \brief
 *  Macro that creates the default directory.
 */
#define CREATE_DIR          const FFILE_T defdir[]

/**
 *  \brief
 *  Macro that declares the default directory.
 *
 *  \param nf	number of files.
 */
#define DECLARE_DIR(nf)     const FFILE_T defdir[nf]

/* -------------------------------- Constants ------------------------------ */
/**
 *  Sets the physical size of page in bytes.
 */
#define RF_SIZE_PHY_PAGE        FF_PHYS_BLOCK_SIZE

/**
 *	Number of flash memory sectors (files and directory).
 */
#define FF_MAX_NSECTOR          (FF_NSECTOR - (FF_DIR_BACKUP + 1))

/**
 *  Sets the invalid page number. It cannot be accesed.
 */
#define FF_INVALID_PAGE         FF_MAX_NSECTOR

/**
 *  Sets the memory flash size in bytes.
 */
#define FF_FLASH_SIZE           (FF_MAX_NSECTOR * FF_PHYS_BLOCK_SIZE)

/**
 *  Each page stores a 16-bits checksum. It's stored in the last
 *  2 position of page. Thus, the effective storage	area of a page
 *  is calculated as:
 *
 *  RF_SIZE_EFF_PAGE = FF_PHYS_BLOCK_SIZE - RF_SIZE_CHECKSUM
 */
#define RF_SIZE_CHECKSUM        2   /* in bytes */
#define RF_SIZE_EFF_PAGE        (FF_PHYS_BLOCK_SIZE - RF_SIZE_CHECKSUM)
#define RF_PAGE_CHECKSUM_POS    RF_SIZE_EFF_PAGE

/**
 *  Generally, the erased flash page sets all bits to one. Therefore,
 *  the checksum of recently erased page is calculated as:
 */
#define RF_ERASE_CHECKSUM       (ffui16_t) ~(RF_SIZE_EFF_PAGE * 0xFF)

#define RF_DIR_MAIN_PAGE        0
#if FF_DIR_BACKUP == 1
#define RF_DIR_BACK_PAGE        (RF_DIR_MAIN_PAGE + 1)
#else
#define RF_DIR_BACK_PAGE        (RF_DIR_MAIN_PAGE + 0)
#endif
#define RF_FILE_PAGE_BASE       (RF_DIR_BACK_PAGE + 1)

/**
 *  The address base of storage area is automatically calculated.
 */
#define RF_DIR_MAIN_FB_ADDRESS \
    ((SA_T)((SA_T)(RF_DIR_MAIN_PAGE * FF_PHYS_BLOCK_SIZE) + RF_FB_ADDRESS))

#define RF_DIR_BACK_FB_ADDRESS \
    ((SA_T)((SA_T)(RF_DIR_BACK_PAGE * FF_PHYS_BLOCK_SIZE) + RF_FB_ADDRESS))

#define RF_FILE_FB_ADDRESS \
    ((SA_T)((SA_T)(RF_FILE_PAGE_BASE * FF_PHYS_BLOCK_SIZE) + RF_FB_ADDRESS))

/**
 *  Misc. macro that includes code to use.
 */
#define RF_PAGE_DUMP            0
#define RF_PAGE_DIRTY           1
#define RF_SET_DIR              1

#if FF_DEBUG == 1
    #include <stdio.h>

static const char *ftmsg[] =
{
    "queue", "random"
};

static const char *qomsg[] =
{
    "end", "beginning"
};

static const char *opmsg[] =
{
    "read", "write"
};

    #if FF_DIR_BACKUP == 1
static const char *dirmsg[] =
{
    "ok", "corrupted, set to default",
    "recovered from backup",
    "backuped (always on external flash)"
};

    #define FFDBG_RESTORE_DIR(r) \
    printf("FFILE initializing...%d pages\n\n", \
           FF_MAX_NSECTOR); \
    printf("Configure with directory backup\n"); \
    printf("Directory %s\n", dirmsg[(r)]); \
    printf("Found %d files on memory in %d pages\n\n", \
           NUM_FLASH_FILES, FF_NSECTOR)
    #else
    #define FFDBG_RESTORE_DIR(r) \
    printf("FFILE initializing...\n\n"); \
    printf("Configure without directory backup\n"); \
    printf("Directory ok\n"); \
    printf("Found %d files on memory\n\n", NUM_FLASH_FILES)
    #endif

    #define FFDBG_FILE_FORMAT(f) \
    printf("File %d formatted, %d/%d pages in error\n", \
           (f)->fd, \
           (f)->page_error, \
           (f)->num_pages)

    #define FFDBG_FILE_INFO(f) \
    do \
    { \
        printf("File #%d:\n", (f)->fd); \
        printf("\tType: %s (%d)\n", ftmsg[(f)->type], (f)->type); \
        printf("\tNumber of used registers: %d\n", (f)->qty); \
        printf("\tCurrent position: %d\n", (f)->pos); \
        printf("\tQueue in: %d\n", (f)->in); \
        printf("\tQueue out: %d\n", (f)->out); \
        printf("\tQueue qty: %d\n", (f)->qty); \
        printf("\tQueue pos_qty: %d\n", (f)->pos_qty); \
        printf("\tSize of register: %d\n", (f)->size_reg); \
        printf("\tNumber of registers: %d\n", (f)->num_regs); \
        printf("\tNumber of pages: %d\n", (f)->num_pages); \
        printf("\tPage in error: %d\n", (f)->page_error); \
    } \
    while (0)

    #define FFDBG_Q_OPEN_AS_RND(f, from) \
    do \
    { \
        printf("Open file %d as random from %s (qty = %d)\n", \
               (f)->fd, qomsg[(from)], \
               (f)->pos_qty); \
    } \
    while (0)

    #define FFDBG_SEEK(f) \
    printf("Seek file %d to position = %d\n", \
           (f)->fd, (f)->pos)

    #define FFDBG_Q_READ(f) \
    printf("Read file %d from position = %d\n", \
           (f)->fd, (f)->pos)

    #define FFDBG_Q_RND_READ(f) \
    printf("Read file %d from position = %d\n", \
           (f)->fd, (f)->pos)

    #define FFDBG_Q_EMPTY(f) \
    printf("File %d is empty\n", (f)->fd)

    #define FFDBG_Q_REMOVE(f) \
    printf("Remove from file %d from position = %d\n", \
           (f)->fd, (f)->out)

    #define FFDBG_Q_DELETE(f) \
    printf("Delete from file %d from position = %d\n", \
           (f)->fd, (f)->out)

    #define FFDBG_Q_FULL(f) \
    printf("File %d is full\n", (f)->fd)

    #define FFDBG_Q_INSERT(f) \
    printf("Insert in file %d in position = %d\n", \
           (f)->fd, (f)->in)

    #define FFDBG_RND_ACCESS(f, o) \
    printf("%s access to file %d in position = %d\n", \
           opmsg[(o)], (f)->fd, (f)->pos)

    #define FFDBG_EOF(f) \
    printf("Reached the end-of-file in file %d\n", (f)->fd)

    #define FFDBG_SYNC() \
    printf("Save directory (%d files) to disk\n", nfiles)

#else
    #define FFDBG_RESTORE_DIR(r)          (void)0
    #define FFDBG_FILE_FORMAT(f)          (void)0
    #define FFDBG_FILE_INFO(f)            (void)0
    #define FFDBG_SEEK(f)                 (void)0
    #define FFDBG_RND_ACCESS(f, o)        (void)0
    #define FFDBG_EOF(f)                  (void)0
    #define FFDBG_Q_OPEN_AS_RND(f, from)  (void)0
    #define FFDBG_Q_READ(f)               (void)0
    #define FFDBG_Q_EMPTY(f)              (void)0
    #define FFDBG_Q_REMOVE(f)             (void)0
    #define FFDBG_Q_DELETE(f)             (void)0
    #define FFDBG_Q_FULL(f)               (void)0
    #define FFDBG_Q_INSERT(f)             (void)0
    #define FFDBG_Q_RND_READ(f)           (void)0
    #define FFDBG_SYNC()                  (void)0
#endif

enum
{
    PAGE_BAD, PAGE_OK
};

enum
{
    QFILE_TYPE, RFILE_TYPE
};

typedef enum DirId DirId;
enum DirId
{
    DirMainId, DirBackupId
};

/* ------------------------------- Data types ------------------------------ */
/**
 *  Defines the file handler.
 */
typedef FFUInt FFD_T;

/**
 *  \brief
 *  Specify the size of the number of register in FFILE. The valid values
 *  [in bits] are 8, 16 or 32. Default is 16.
 */
#if FF_SIZEOF_NREGS == 8
typedef ffui8_t NR_T;
#elif FF_SIZEOF_NREGS == 16
typedef ffui16_t NR_T;
#elif FF_SIZEOF_NREGS == 32
typedef ffui32_t NR_T;
#else
typedef ffui16_t NR_T;
#endif

/**
 *  \brief
 *  This data type defines the maximum number of sectors that the flash
 *  memory device can contain.
 *
 *	The valid values [in bits] are 8, 16 or 32. Default is 8. This type is
 *	configurable via the preprocessor switch FF_MAX_NSECTOR.
 */
#if FF_MAX_NSECTOR > 0 && FF_MAX_NSECTOR <= 256
typedef ffui8_t SPG_T;
typedef ffui16_t SA_T;
#elif FF_MAX_NSECTOR > 256 && FF_MAX_NSECTOR <= 65536
typedef ffui16_t SPG_T;
typedef ffui32_t SA_T;
#else
typedef ffui8_t SPG_T;
typedef ffui32_t SA_T;
#endif

typedef struct FFILE_T FFILE_T;
struct FFILE_T
{
    /**
     *  File descriptor.
     */
    unsigned fd : 4;

    /**
     *  Type of file access: QFILE_TYPE, RFILE_TYPE.
     */
    unsigned type : 4;

    /**
     *  Number of page in error.
     */
    SPG_T page_error;

    /**
     *  Number of pages.
     */
    SPG_T num_pages;

    /**
     *  First page relative to directory page.
     */
    SPG_T begin_page;

    /**
     *  Absolute base address.
     */
    SA_T start_address;

    /**
     *  Register size in bytes.
     */
    ffui16_t size_reg;

    /*
     *  Number of register per page.
     */
    NR_T num_regs_per_page;

    /**
     *  Maximum number of registers.
     */
    NR_T num_regs;

    /**
     *  Points to the next place of stored register.
     */
    NR_T in;

    /**
     *  Points to the free next place.
     */
    NR_T out;

    /**
     *  Number of registers currently in the file.
     */
    NR_T qty;

    /**
     *  Points to currently position.
     */
    NR_T pos;

    /**
     *  Number of registers currently in the random file.
     */
    NR_T pos_qty;
};

typedef struct
{
    FFILE_T *pf;
    unsigned op : 1;
    unsigned tacc : 1;
    unsigned res : 6;
    void *ptofrom;
    ffui16_t currpos;
    ffui16_t rqty;
} RACC_T;

/* -------------------------- External variables --------------------------- */
/* -------------------------- Function prototypes -------------------------- */
void rfile_init_directory(ffui8_t mode);
void rfile_update_directory(FFILE_T *pf);
void rfile_file_format(FFILE_T *pf);
void rfile_access_register(RACC_T *pra);
FFILE_T *rfile_get_file(FFD_T fd);
void rfile_page_dirty(SPG_T page);
FFILE_T *rfile_restore_directory(ffui8_t *status);
void rfile_getDirtyDir(DirId dir);

/* -------------------- External C language linkage end -------------------- */
#ifdef __cplusplus
}
#endif

/* ------------------------------ Module end ------------------------------- */
#endif

/* ------------------------------ End of file ------------------------------ */
