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
 *  DaBa  Dario Balina db@vortexmakes.com
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
/* ------------------------------- Data types ------------------------------ */
/* -------------------------- External variables --------------------------- */
/* -------------------------- Function prototypes -------------------------- */
void dfspi_init(void);

void dfspi_select_channel(void);
void dfspi_deselect_channel(void);

void dfspi_write_byte(uchar b);
void dfspi_read_byte(uchar *p);

void dfspi_write(uchar *p, uint qty);
void dfspi_read(uchar *p, uint qty);

/* -------------------- External C language linkage end -------------------- */
#ifdef __cplusplus
}
#endif

/* ------------------------------ Module end ------------------------------- */
#endif
