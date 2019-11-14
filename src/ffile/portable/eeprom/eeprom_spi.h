/**
 *  \file       eeprom_spi.h
 *  \brief      EEPROM SPI interface
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

#ifndef __EEPROM_SPI_H__
#define __EEPROM_SPI_H__

/* ----------------------------- Include files ----------------------------- */
#include "mytypes.h"

/* ---------------------- External C language linkage ---------------------- */
#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------- Macros -------------------------------- */
#define EEPROM_CS_HIGH()    eeprom_spi_deselect()
#define EEPROM_CS_LOW()     eeprom_spi_select()

/* -------------------------------- Constants ------------------------------ */
/* ------------------------------- Data types ------------------------------ */
/* -------------------------- External variables --------------------------- */
/* -------------------------- Function prototypes -------------------------- */
void eeprom_spi_init(void);

void eeprom_spi_select(void);
void eeprom_spi_deselect(void);

void eeprom_spi_write_byte(uchar b);
void eeprom_spi_read_byte(uchar *p);

void eeprom_spi_write(uchar *p, uint qty);
void eeprom_spi_read(uchar *p, uint qty);

/* -------------------- External C language linkage end -------------------- */
#ifdef __cplusplus
}
#endif

/* ------------------------------ Module end ------------------------------- */
#endif
