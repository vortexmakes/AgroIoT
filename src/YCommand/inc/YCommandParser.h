/**
 *  \file       YCommandParser.h
 *  \brief      Yipies Command Parser.
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2020.04.17  Daba  v1.0.00  Initial version
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  DaBa  Dario Balina db@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* --------------------------------- Module -------------------------------- */
#ifndef __YCOMMANDPARSER_H__
#define __YCOMMANDPARSER_H__

/* ----------------------------- Include files ----------------------------- */
#include "rkh.h"
#include "ssp.h"

/* ---------------------- External C language linkage ---------------------- */
#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------- Macros -------------------------------- */
/* -------------------------------- Constants ------------------------------ */
SSP_DCLR_NORMAL_NODE rootYCommandParser;

/* ------------------------------- Data types ------------------------------ */
/* -------------------------- External variables --------------------------- */
/* -------------------------- Function prototypes -------------------------- */
rInt YCommandParser_search(char *p, ruint size);
rInt YCommandParser_securityCheck(char *pkey);
ruint YCommandParser_getId(void);

/* -------------------- External C language linkage end -------------------- */
#ifdef __cplusplus
}
#endif

/* ------------------------------ Module end ------------------------------- */
#endif

/* ------------------------------ End of file ------------------------------ */
