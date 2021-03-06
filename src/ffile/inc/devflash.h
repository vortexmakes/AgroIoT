/**
 *  \file       devflash.h
 *  \brief
 */

/* -------------------------- Development history -------------------------- */
/*
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  LeFr  Leandro Francucci lf@vortexmakes.com
 *  DaBa  Dario Balina      db@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* --------------------------------- Module -------------------------------- */
#ifndef __DEVFLASH_H__
#define __DEVFLASH_H__

/* ----------------------------- Include files ----------------------------- */
#include "ffile.h"

/* ---------------------- External C language linkage ---------------------- */
#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------- Macros -------------------------------- */
#if FF_INTERNAL_FLASH == 1
#define devflash_cmp_pages(pga0, pga1) \
    (memcmp((pga0), (pga1), RF_SIZE_EFF_PAGE) == 0)
#else
/*
 * To avoid the usage of two buffers for comparing two pages, in
 * external flash device the directory is unconditionally backuped.
 */
#define devflash_cmp_pages(pga0, pga1)  ((ffui8_t)1)
#endif

/* -------------------------------- Constants ------------------------------ */
/* ------------------------------- Data types ------------------------------ */
typedef struct PageRes PageRes;
struct PageRes
{
    ffui8_t result;
    ffui16_t checksum;
};

/* -------------------------- External variables --------------------------- */
/* -------------------------- Function prototypes -------------------------- */
ffui8_t *devflash_restore_directory(ffui8_t *status);
void devflash_write_data(SA_T desta, SPG_T destp, ffui8_t *data,
                         ffui16_t size);
void devflash_read_data(SA_T desta, SPG_T destp, ffui8_t *data,
                        ffui16_t size);
void devflash_copy_page_from_buff(SA_T dest_addr);
void devflash_format_pages(SPG_T start_page, SPG_T qty);
PageRes devflash_verify_page(SPG_T page);
ffui8_t *devflash_page_dump(SPG_T page);
void devflash_page_dirty(SPG_T page);
void devflash_read_page(SPG_T page);
int devflash_is_ready_to_save_in_flash(void);
void devflash_setInvalidPage(void);
void devflash_copy_page(SPG_T destp, SPG_T srcp);

/* -------------------- External C language linkage end -------------------- */
#ifdef __cplusplus
}
#endif

/* ------------------------------ Module end ------------------------------- */
#endif

/* ------------------------------ End of file ------------------------------ */
