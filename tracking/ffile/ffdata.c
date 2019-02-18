/*
 *  ffdata.c
 */

#include "ffile.h"
#include "ffdata.h"
#include "gps.h"

/*
 *  The memory layout of file flash system is wired, the
 *  first two pages stores the directory and its backup, the remains
 *  pages stores the files.
 *
 *  Example of typical memory layout:
 *
 *  Storage		-	Page ix
 *
 *  -----------		RF_DIR_MAIN_PAGE = 0
 *  0 - dir
 *  -----------		RF_DIR_MAIN_PAGE = 1
 *  1 - dir back
 *  -----------		FILE0_BEGIN_PAGE = RF_FILE_PAGE_BASE = 2
 *  2 - file 0
 *  3 -	file 0		FILE0_NUMBER_PAGES = 3
 *  4 - file 0
 *  -----------		FILE1_BEGIN_PAGE = FILE0_BEGIN_PAGE + FILE0_NUMBER_PAGES
 *  5 - file 1
 *  6 -	file 1		FILE1_NUMBER_PAGES = 3
 *  7 - file 1
 *  -----------		FILE2_BEGIN_PAGE = FILE1_BEGIN_PAGE + FILE1_NUMBER_PAGES
 *  8 - file 1
 *  9 -	file 1		FILE2_NUMBER_PAGES = 3
 *  10- file 1
 *  -----------		...
 */

/*
 * FFx_NUM_PAGES sum must be equal to FF_NSECTOR - 2.
 * Two pages are used to store directory and its backup.
 */

#define FF0_NUM_PAGES           (FF_NSECTOR - 3) /* 8190 - 3= 8187 */
#define FF1_NUM_PAGES           1

#define FF0_BEGIN_PAGE          RF_FILE_PAGE_BASE /* 2 */
#define FF1_BEGIN_PAGE          (FF0_NUM_PAGES + FF0_BEGIN_PAGE) /* 8187 + 2 =
                                                                  * 8189 */

/*
 *	Files directory. Any software module intending to use a flash file
 *	must first create a file structure into directory. When a file
 *	is first created, it's assigned an associated unique descriptor,
 *	access type, a number of flash pages, a begin page, and size of
 *	register. Therefore, each file of that must be defined by means
 *	'CREATE_FFILE' macro. The file descriptor are defined in 'ffdata.h'
 *	file.
 */

CREATE_DIR =
{
    CREATE_FFILE(FFD0,
                 QFILE_TYPE,
                 FF0_NUM_PAGES,   /* 8187 */
                 FF0_BEGIN_PAGE,   /* 2 */
                 sizeof(GPS_STR)),
    CREATE_FFILE(FFD1,
                 RFILE_TYPE,
                 FF1_NUM_PAGES,   /* 1 */
                 FF1_BEGIN_PAGE,   /* 8189 */
                 sizeof(CONF_STR))
};
/* ------------------------------ End of file ------------------------------ */
