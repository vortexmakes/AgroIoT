/**
 *  \file       dfspi.h
 *  \brief      Dataflash SPI interface
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2019.06.27  DaBa  v1.0.00  Initial version
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  DaBa  Dario Baliña db@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* --------------------------------- Module -------------------------------- */

#ifndef __DFSPI_H__
#define __DFSPI_H__

/* ----------------------------- Include files ----------------------------- */
#include "mytypes.h"

/* ---------------------- External C language linkage ---------------------- */
#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------- Macros -------------------------------- */
/* -------------------------------- Constants ------------------------------ */
#define DFLASH_BASE				1
#define DF_BOUNCE_TIME 			(50/SLEEP_BASE)
#define DF_RELEASE_TIME 		(1000/DFLASH_BASE)

/* ------------------------------- Data types ------------------------------ */
/* -------------------------- External variables --------------------------- */
/* -------------------------- Function prototypes -------------------------- */
void dfspi_init(void);

void dfspi_select_channel(void);
void dfspi_deselect_channel(void);

void dfspi_send_byte(uchar b);
uchar dfspi_get_byte(void);

/* -------------------- External C language linkage end -------------------- */
#ifdef __cplusplus
}
#endif

/* ------------------------------ Module end ------------------------------- */
#endif
