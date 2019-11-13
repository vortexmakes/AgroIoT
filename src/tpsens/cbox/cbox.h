/**
 *  \file       cbox.h
 *  \brief      Module to handle cbox singleton
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2018.05.17  DaBa  v1.0.00  Initial version
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  DaBa  Dario Balina       db@vortexmakes.com
 *  LeFr  Leandro Francucci  lf@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* --------------------------------- Module -------------------------------- */
#ifndef __CBOX_H__
#define __CBOX_H__

/* ----------------------------- Include files ----------------------------- */
#include "rkhtype.h"

/* ---------------------- External C language linkage ---------------------- */
#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------- Macros -------------------------------- */
/* -------------------------------- Constants ------------------------------ */
#define CBOX_NOT_PRESENT        0xFFFF
#define EQTYPE_HARVEST_WO_ACC   0xEEEE
#define EQTYPE_HARVEST_W_ACC    0x00FA
#define EQTYPE_HARVEST          0xFF05
#define EQTYPE_SAMPLER          0xFF06
#define EQTYPE_SPRAYER          0xFF07
#define EQTYPE_SPRAYERPRO       0xFF08
#define ADDR_CAUDALIMETRO       1

enum
{
    ADDR_NORIA,
    /* ADDR_CAUDALIMETRO, */
    NUM_SENSORS_BOX
};

enum
{
    TPSENS_NULL_CMD,

    CBOX_NULL = TPSENS_NULL_CMD,
    CBOX_READ_ALL,      /* Reads all CBOX parameters */

    NUM_CBOX_CMDS
};

/* ------------------------------- Data types ------------------------------ */
typedef struct ACCEL_T ACCEL_T;
struct ACCEL_T
{
    short x;
    short y;
    short z;
    unsigned char m;
};

typedef struct GRSENS_T GRSENS_T;
struct GRSENS_T
{
    unsigned short hoard;
    unsigned short pqty;
    unsigned short flow;
};

typedef struct CBOX_STR CBOX_STR;
struct CBOX_STR
{
    unsigned char cmd;
    unsigned char m;
    GRSENS_T h;
    ACCEL_T a;
    unsigned char hum;
};

/* -------------------------- External variables --------------------------- */
/* -------------------------- Function prototypes -------------------------- */
rbool_t cbox_isMoving(CBOX_STR *const me);

/* -------------------- External C language linkage end -------------------- */
#ifdef __cplusplus
}
#endif

/* ------------------------------ Module end ------------------------------- */
#endif

/* ------------------------------ End of file ------------------------------ */
