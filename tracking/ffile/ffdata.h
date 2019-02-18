/*
 *  ffdata.h
 */

#ifndef __FFDATA_H__
#define __FFDATA_H__

#include "ffile.h"

/*
 *	When a file is first created, it's assigned an associated unique
 *	descriptor.
 */

enum
{
    FFD0, FFD1, NUM_FLASH_FILES
};

extern DECLARE_DIR(NUM_FLASH_FILES);

#endif
/* ------------------------------ End of file ------------------------------ */
