/**
 *  \file       Crc32.c
 *  \brief      Implementation of CRC-32 using STM32.
 */

/* -------------------------- Development history -------------------------- */
/*
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  LeFr  Leandro Francucci lf@vortexmakes.com
 *  DaBa  Darío Baliña db@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */
#include "Crc32.h"
#include "bsp.h"
#include "crc.h"
#include "GStatus.h"
#include <string.h>

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
static uint32_t buffer[sizeof(GStatusType)+1];

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ---------------------------- Global functions --------------------------- */
void
Crc32_init(void)
{
}

Crc32
Crc32_calc(const uint8_t *message, size_t nBytes, Crc32 init)
{
	uint i;

	for(i = 0; i < nBytes; ++i, ++message)
		buffer[i] = *message;

	return HAL_CRC_Calculate(&hcrc, buffer, nBytes);
}

/* ------------------------------ End of file ------------------------------ */
