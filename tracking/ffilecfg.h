/**
 *  file: ffilecfg.h
 *	Last updated for version: 1.0.00
 *	Date of the last update:  Feb 13, 2012
 */

/**
 *  \file ffilecfg.h
 *
 *	\brief
 *	Each FFILE application must have its own configuration file, called
 *	ffilecfg.h. This file adapts and configures FFILE by means of compiler
 *	definitions and macros.
 *
 *  This header file must be included in all modules
 *  (*.c files) that use FFILE.
 */

#ifndef __FFILECFG_H__

#include "ffplat.h"

/**
 *	Size of sector flash in bytes.
 */

#define FF_PHYS_BLOCK_SIZE      PHYS_BLOCK_SIZE /* 512 */

/**
 *	Number of flash memory sectors for files only.
 */

#define FF_NSECTOR              (NUM_PHYS_BLOCKS - (FF_DIR_BACKUP + 1)) /* 8192
                                                                         * -(1+1))=
                                                                         * 8190 */

/**
 *	Max. number of registers per flash memory sector.
 */

#define FF_SIZEOF_NREGS         16

/*
 *  Sets the flash base address of storage area.
 */

#define RF_FB_ADDRESS           (SA_T)(FLASH_BASE_ADDRESS) /* 0x0000 */

/**
 *  Sets the type of flash memory access.
 */

#define FF_INTERNAL_FLASH       0

/**
 */

#define FF_DIR_BACKUP           1

/**
 */

#define FF_QUEUE_FILE           1

/**
 */

#define FF_AUTO_FILE_FORMAT     1

/**
 */

#define FF_FILE_FORMAT          1

/**
 */

#define FF_TELL                 1

/**
 */

#define FF_QUEUE_RND_READ       1

/**
 */

#define FF_QUEUE_READ           0

/**
 */

#define FF_SEEK                 1

/**
 */

#define FF_IS_CORRUPTED         0

/**
 */

#define FF_CHECK_EOF            0

/**
 */

#define FF_GET_FILE_INFO        0

/**
 */

#define FF_DEBUG                0

#endif
/* ------------------------------ End of file ------------------------------ */
