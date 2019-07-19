/*
 * 	dftype.h
 * 		Data Flash types
 * 		definitions
 */

#ifndef __DFTYPE_H__
#define __DFTYPE_H__

#ifdef __DF_AT45DB161D__

/* For Atmel AT45DB161 */

#define DF_DEVICE_ID			0x0B
#define DF_BADDR_BITS			9
#define DF_NUM_SECTORS			17
#define DF_PAGE_SIZE			512
#define DF_PAGE_COUNT			4096
#define DF_PAGE_NUM_PER_BLOCK	8

#endif

#ifdef __DF_AT45DB321D__
/* For Atmel AT45DB321D */

#define DF_DEVICE_ID			0x0D
#define DF_BADDR_BITS			9
#define DF_NUM_SECTORS			65
#define DF_PAGE_SIZE			512
#define DF_PAGE_COUNT			8192
#define DF_PAGE_NUM_PER_BLOCK	8

#endif

#define DENSITY_POS			2
#define DENSITY_MASK		0x0f
#define POWER2_PSIZE_MASK	0x01


#endif
