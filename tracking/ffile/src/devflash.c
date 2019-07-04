/*
 *  devflash.c
 */

#include <string.h>
#include "ffile.h"
#include "devflash.h"

#define devflash_is_changed_page(pg) \
    ((pg) != cpage)

enum
{
    CHECK_BAD, CHECK_OK
};

typedef struct
{
    ffui8_t data[RF_SIZE_EFF_PAGE];  /* 510 */
    ffui16_t checksum; /* 2 */
} PAGE_BUFF_T;

typedef ffui8_t (*RECPROC_T)(void);

static PAGE_BUFF_T page_buff;
static SA_T page_address;
static SPG_T cpage = FF_INVALID_PAGE;

static
void
devflash_set_page_address(SPG_T page)
{
    cpage = page;
    page_address = (SA_T)(((SA_T)page * RF_SIZE_PHY_PAGE) + RF_FB_ADDRESS);
}

#if FF_DIR_BACKUP == 1
void
devflash_copy_page(SPG_T destp, SPG_T srcp)
{
    FFILE_WATCHDOG();

    if (devflash_is_ready_to_save_in_flash())
    {
        devflash_read_page(srcp);
        devflash_set_page_address(destp);
        flash_write_page(page_address, &page_buff);
    }

    FFILE_WATCHDOG();
}
#endif

static
ffui16_t
devflash_calculate_page_checksum(ffui8_t *address)
{
    ffui16_t check, i;
    ffui8_t *p;

    p = (ffui8_t*)address;
    FFILE_WATCHDOG();

    for (check = 0, i = RF_SIZE_EFF_PAGE; i; --i)
        check += *p++;

    return ~check;
}

void
devflash_read_page(SPG_T page)
{
    if (devflash_is_changed_page(page))
    {
        devflash_set_page_address(page);
        flash_read_page(&page_buff, page_address);
    }
}

static
void
devflash_read_page_direct(SPG_T page)
{
#if FF_INTERNAL_FLASH == 1
    if (devflash_is_changed_page(page))
    {
        devflash_set_page_address(page);
        flash_read_page(&page_buff, page_address);
    }
#else
    devflash_read_page(page);
#endif
}

void
devflash_format_pages(SPG_T start_page, SPG_T qty)
{
    FFILE_WATCHDOG();

    if (devflash_is_ready_to_save_in_flash())
    {
        devflash_set_page_address(start_page);

        memset(&page_buff, 0xFF, RF_SIZE_PHY_PAGE);
        page_buff.checksum = RF_ERASE_CHECKSUM;

        for (; cpage < qty + start_page;
             ++cpage, page_address += RF_SIZE_PHY_PAGE)
            flash_write_page(page_address, &page_buff);
    }
}

void
devflash_write_data(SA_T desta, SPG_T destp, ffui8_t *data, ffui16_t size)
{
    FFILE_WATCHDOG();

    if (devflash_is_ready_to_save_in_flash())
    {
        devflash_read_page(destp);
        memcpy(page_buff.data + desta, data, size);
        page_buff.checksum = devflash_calculate_page_checksum(page_buff.data);
        flash_write_page(page_address, &page_buff);
    }
}

void
devflash_read_data(SA_T desta, SPG_T destp, ffui8_t *data, ffui16_t size)
{
    devflash_read_page_direct(destp);
    memcpy(data, page_buff.data + desta, size);
}

PageRes
devflash_verify_page(SPG_T page)
{
    PageRes res;

    devflash_read_page_direct(page);
    res.checksum = devflash_calculate_page_checksum(page_buff.data);
    res.result = (page_buff.checksum == res.checksum) ? PAGE_OK : PAGE_BAD;
    return res;
}

#if RF_PAGE_DUMP == 1
ffui8_t *
devflash_page_dump(SPG_T page)
{
    devflash_read_page(page);
    return page_buff.data;
}
#endif

#if RF_PAGE_DIRTY == 1
void
devflash_page_dirty(SPG_T page)
{
    devflash_read_page(page);
    page_buff.checksum = ~page_buff.checksum;
    flash_write_page(page_address, &page_buff);
}
#endif

void
devflash_copy_page_from_buff(SA_T dest_addr)
{
    if (devflash_is_ready_to_save_in_flash())
    {
        flash_write_page(dest_addr, &page_buff);
    }
}

int
devflash_is_ready_to_save_in_flash(void)
{
    return 1;
}

void 
devflash_setInvalidPage(void)
{
    cpage = FF_INVALID_PAGE;
}

/* ------------------------------ End of file ------------------------------ */
