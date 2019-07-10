/**
 *  \file       eeprom.h
 *  \brief      EEPROM M95xxx device controller.
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
#ifndef __EEPROM_H__
#define __EEPROM_H__

/* ----------------------------- Include files ----------------------------- */
#include "stdint.h"

/* ---------------------- External C language linkage ---------------------- */
#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------- Macros -------------------------------- */
/* -------------------------------- Constants ------------------------------ */
/* ------------------------------- Data types ------------------------------ */
/* -------------------------- External variables --------------------------- */
/* -------------------------- Function prototypes -------------------------- */
void eeprom_init(void);

void eeprom_write(uint8_t *p, uint16_t addr, uint16_t qty);
void eeprom_read(uint8_t *p, uint16_t addr, uint16_t qty);

uint8_t eeprom_readStatusRegister(void);
void eeprom_writeStatusRegister(uint8_t regval);

/* -------------------- External C language linkage end -------------------- */
#ifdef __cplusplus
}
#endif

/* ------------------------------ Module end ------------------------------- */
#endif
