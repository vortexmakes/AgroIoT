/**
 *  \file ubx.h
 *
 *	This module contains Ublox protocol frames for initial GPS setup
 */
/* -------------------------- Development history -------------------------- */
/*
 *  2018.09.05  DaBa  v1.0.00   Initial version
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  DaBa  Dario Baliña db@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* --------------------------------- Module -------------------------------- */
#ifndef __UBX_H__
#define __UBX_H__

/* ----------------------------- Include files ----------------------------- */
#include "epoch.h"

/* ---------------------- External C language linkage ---------------------- */
#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------- Macros -------------------------------- */
/* -------------------------------- Constants ------------------------------ */
#define UBX_GSA_OFF {0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0xF0, 0x02, \
                     0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x32}

#define UBX_VTG_OFF {0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0xF0, 0x05, \
                     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x46}

#define UBX_GSV_OFF {0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0xF0, 0x03, \
                     0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x39}

#define UBX_GLL_OFF {0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0xF0, 0x01, \
                     0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x2B}

#define UBX_GGA_OFF {0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0xF0, 0x00, \
                     0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x24}

#define UBX_RMC_2HZ {0xB5, 0x62, 0x06, 0x08, 0x06, 0x00, 0xF4, 0x01, \
                     0x01, 0x00, 0x00, 0x00, 0x0A, 0x75}

#define UBX_INTERCMD_TIME   RKH_TIME_MS(1000)

#define RMC_PERIOD_TIMEOUT  RKH_TIME_MS(3000)

/* ------------------------------- Data types ------------------------------ */
typedef struct
{
    const unsigned char *cmd;
    int size;
}UBXCmd_t;


/* -------------------------- External variables --------------------------- */
/* -------------------------- Function prototypes -------------------------- */
/* -------------------- External C language linkage end -------------------- */
#ifdef __cplusplus
}
#endif

/* ------------------------------ Module end ------------------------------- */
#endif

/* ------------------------------ End of file ------------------------------ */
