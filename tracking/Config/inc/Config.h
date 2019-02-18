/**
 *  \file       Config.h
 *  \brief      Specifies the configurtion module.
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2018.09.06  DaBa  v1.0.00   Initial version
 *  2019.18.02  LeFr  v1.1.00   Reviewed to test it
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  DaBa  Dario Balina db@vortexmakes.com
 *  LeFr  Leandro Francucci  lf@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* --------------------------------- Module -------------------------------- */
#ifndef __CONFIG_H__
#define __CONFIG_H__

/* ----------------------------- Include files ----------------------------- */
/* ---------------------- External C language linkage ---------------------- */
#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------- Macros -------------------------------- */
/* -------------------------------- Constants ------------------------------ */
/* ------------------------------- Data types ------------------------------ */
typedef struct Config Config;
struct Config
{
    rui8_t aclimit;
    rui8_t brlimit;
};

/* -------------------------- External variables --------------------------- */
/* -------------------------- Function prototypes -------------------------- */
Config *config_read(void);

/* -------------------- External C language linkage end -------------------- */
#ifdef __cplusplus
}
#endif

/* ------------------------------ Module end ------------------------------- */
#endif

/* ------------------------------ End of file ------------------------------ */
