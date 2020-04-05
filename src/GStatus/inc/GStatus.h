/**
 *  \file       GStatus.h
 *  \brief      Specifies the interface of GStatus module.
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2019.25.01  LeFr  v1.0.00  Initial version
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  LeFr  Leandro Francucci  lf@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* --------------------------------- Module -------------------------------- */
#ifndef __GSTATUS_H__
#define __GSTATUS_H__

/* ----------------------------- Include files ----------------------------- */
#include "Geo.h"
#include "cbox.h"
#include "IOStatus.h"
#include "BatChr.h"
#include "Crc32.h"

/* ---------------------- External C language linkage ---------------------- */
#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------- Macros -------------------------------- */
/* -------------------------------- Constants ------------------------------ */
#define INOUT_LENGTH    4
#define ACCELBR_LENGHT  4

/* ------------------------------- Data types ------------------------------ */
typedef struct GStatusType GStatusType;
struct GStatusType
{
    Geo position;
    CBOX_STR devData;
    IOStatus ioStatus;
    BatChrStatus batChrStatus;
};

typedef struct GStatus GStatus;
struct GStatus
{
    GStatusType data;
    Crc32 checksum;
};

/* -------------------------- External variables --------------------------- */
/* -------------------------- Function prototypes -------------------------- */
/* -------------------- External C language linkage end -------------------- */
#ifdef __cplusplus
}
#endif

/* ------------------------------ Module end ------------------------------- */
#endif

/* ------------------------------ End of file ------------------------------ */
