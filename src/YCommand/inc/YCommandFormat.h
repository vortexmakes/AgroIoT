/**
 *  \file       YCommandFormat.h
 *  \brief      Yipies Command Format.
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2020.04.17  DaBa  v1.0.00  Initial version
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  DaBa  Dario Balina db@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* --------------------------------- Module -------------------------------- */
#ifndef __YCOMMANDFORMAT_H__
#define __YCOMMANDFORMAT_H__

/* ----------------------------- Include files ----------------------------- */
#include "YCommand.h"
#include "YCommandParser.h"

/* ---------------------- External C language linkage ---------------------- */
#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------- Macros -------------------------------- */
/* -------------------------------- Constants ------------------------------ */
/* ------------------------------- Data types ------------------------------ */
/* -------------------------- External variables --------------------------- */
/* -------------------------- Function prototypes -------------------------- */
YCmdRes YCommandFormat_format(YCmdParserData *pCmd, char *p);

/* -------------------- External C language linkage end -------------------- */
#ifdef __cplusplus
}
#endif

/* ------------------------------ Module end ------------------------------- */
#endif

/* ------------------------------ End of file ------------------------------ */
