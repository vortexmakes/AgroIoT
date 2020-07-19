/**
 *  \file       AgroIoTVersion.c
 *  \brief      Defines the AgroIoT version in string format
 */

/* -------------------------- Development history -------------------------- */
/* -------------------------------- Authors -------------------------------- */
/*
 *  LeFr  Leandro Francucci  lf@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */
#include "rkhtype.h"
#include "AgroIoTVersion.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
const char agroIoTVersion[] = /* String describing the AgroIoT version */
{
    (char)((uint8_t)((AGROIOT_VERSION >> 20) & 0x0F) + (uint8_t)'0'),
    (char)((uint8_t)((AGROIOT_VERSION >> 16) & 0x0F) + (uint8_t)'0'),
    (char)((uint8_t)((AGROIOT_VERSION >> 12) & 0x0F) + (uint8_t)'0'),
    (char)((uint8_t)((AGROIOT_VERSION >>  8) & 0x0F) + (uint8_t)'0'),
    (char)((uint8_t)((AGROIOT_VERSION >>  4) & 0x0F) + (uint8_t)'0'),
    (char)((uint8_t)( AGROIOT_VERSION        & 0x0F) + (uint8_t)'0'),
    (char)'\0'
};

/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ---------------------------- Global functions --------------------------- */
/* ------------------------------ End of file ------------------------------ */
