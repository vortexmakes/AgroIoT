/*
 *  devflash.c
 */

#include <string.h>
#include "ffile.h"
#include "devflash.h"

#define devflash_is_changed_page(pg) \
    ((pg) != cpage)

#if FF_INTERNAL_FLASH == 1
    #define devflash_cmp_pages(pga0, pga1) \
    (memcmp((pga0), (pga1), RF_SIZE_EFF_PAGE) == 0)
#else
/*
 * To avoid the usage of two buffers for comparing two pages, in
 * external flash device the directory is unconditionally backuped.
 */
    #define devflash_cmp_pages(pga0, pga1) \
    ((ffui8_t)1)
#endif

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
static SA_T main_page_addr, back_page_addr, page_address;
static ffui8_t rmain, rback;
static ffui16_t main_check, back_check;
static SPG_T cpage = FF_INVALID_PAGE;

/*
 *  Recovery true tables:
 *
 *  rmain: result of checksum from main page.
 *  rback: result of checksum from backup page.
 *
 *  rmain		|	rback		|	Process
 *  ---------------------------------------------------
 *  CHECK_BAD	|	CHECK_BAD	|	DIR_BAD
 *  CHECK_BAD	|	CHECK_OK	|	DIR_RECOVERY
 *  CHECK_OK	|	CHECK_BAD	|	DIR_BACKUP
 *  CHECK_OK	|	CHECK_OK	|	-> next true table
 *
 *  Checksum compare true table:
 *  ---------------------------------------------------
 *  NOT_MATCH	|	DIR_BACKUP
 *  MATCH		|	-> next true table
 *
 *  Content compare true table:
 *  ---------------------------------------------------
 *	NOT_MATCH	|	DIR_BACKUP
 *  MATCH		|	DIR_OK
 */

#if FF_DIR_BACKUP == 1
static ffui8_t  proc_page_in_error(void),
proc_page_recovery(void),
proc_page_backup(void),
proc_page_cmp(void);
#endif

#if FF_DIR_BACKUP == 1
static const RECPROC_T recovery[] =
{
    proc_page_in_error,
    proc_page_recovery,
    proc_page_backup,
    proc_page_cmp
};
#endif

static
void
devflash_set_page_address(SPG_T page)
{
    cpage = page;
    page_address = (SA_T)(((SA_T)page * RF_SIZE_PHY_PAGE) + RF_FB_ADDRESS);
}

#if FF_DIR_BACKUP == 1
static
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

#if FF_DIR_BACKUP == 1
static
ffui8_t
proc_page_in_error(void)
{
/*	set_led(LED_FLASH,LSTAGE4); */
    return DIR_BAD;
}

static
ffui8_t
proc_page_recovery(void)
{
/*	set_led(LED_FLASH,LSTAGE1); */
    devflash_copy_page(RF_DIR_MAIN_PAGE, RF_DIR_BACK_PAGE);
    return DIR_RECOVERY;
}

static
ffui8_t
proc_page_backup(void)
{
/*	set_led(LED_FLASH,LSTAGE2); */
    devflash_copy_page(RF_DIR_BACK_PAGE, RF_DIR_MAIN_PAGE);
    return DIR_BACKUP;
}

static
ffui8_t
proc_page_cmp(void)
{
/*	set_led(LED_FLASH,LSTAGE3); */
    if (main_check != back_check)
    {
        return proc_page_backup();
    }

    if (devflash_cmp_pages(main_page_addr, back_page_addr))
    {
        return DIR_OK;
    }

    return proc_page_backup();
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

static
ffui8_t
devflash_verify_page_checksum(ffui8_t *pfrom, ffui16_t checksum)
{
    return ((PAGE_BUFF_T*)pfrom)->checksum == checksum ? CHECK_OK : CHECK_BAD;
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

ffui8_t
devflash_verify_page(SPG_T page)
{
    devflash_read_page_direct(page);
    main_check = devflash_calculate_page_checksum(page_buff.data);
    rmain = devflash_verify_page_checksum(page_buff.data, main_check);
    return rmain == CHECK_OK ? PAGE_OK : PAGE_BAD;
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

ffui8_t *
devflash_restore_directory(ffui8_t *status)
{
    ffui8_t r;

    cpage = FF_INVALID_PAGE;
    devflash_read_page_direct(RF_DIR_MAIN_PAGE);
    main_check = devflash_calculate_page_checksum(page_buff.data);
    rmain = devflash_verify_page_checksum(page_buff.data, main_check);

#if FF_DIR_BACKUP == 1
    devflash_read_page_direct(RF_DIR_BACK_PAGE);
    back_check = devflash_calculate_page_checksum(page_buff.data);
    rback = devflash_verify_page_checksum(page_buff.data, back_check);

    r = 0;
    r = (rmain << 1) | rback;
    *status = (*recovery[r])();
#else
    r = 0;
    *status = (rmain == CHECK_OK) ? DIR_OK : DIR_BAD;
#endif
    return page_buff.data;
}

int
devflash_is_ready_to_save_in_flash(void)
{
    return 1;
}

/* ------------------------------ End of file ------------------------------ */
