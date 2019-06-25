/*
 *  devflash.h
 */

#ifndef __DEVFLASH_H__
#define __DEVFLASH_H__

#include "ffile.h"

enum
{
    DIR_OK, DIR_BAD, DIR_RECOVERY, DIR_BACKUP
};

ffui8_t *devflash_restore_directory(ffui8_t *status);
void devflash_write_data(SA_T desta, SPG_T destp, ffui8_t *data,
                         ffui16_t size);
void devflash_read_data(SA_T desta, SPG_T destp, ffui8_t *data,
                        ffui16_t size);
void devflash_copy_page_from_buff(SA_T dest_addr);
void devflash_format_pages(SPG_T start_page, SPG_T qty);
ffui8_t devflash_verify_page(SPG_T page);
ffui8_t *devflash_page_dump(SPG_T page);
void devflash_page_dirty(SPG_T page);
void devflash_read_page(SPG_T page);
int devflash_is_ready_to_save_in_flash(void);

#endif
/* ------------------------------ End of file ------------------------------ */
