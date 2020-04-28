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
typedef enum
{
    YCommandFound,
    YCommandValidKey = YCommandFound,

    YCommandNotFound = -1,
    YCommandInvalidKey = YCommandNotFound,

}YCmdPResult;

#define YCOMMAND_INDEX_LEN       11
#define YCOMMAND_ID_LEN          2
#define YCOMMAND_SECURITY_LEN    3
#define YCOMMAND_DATA_LEN        32

/* ------------------------------- Data types ------------------------------ */
typedef struct
{
    SSP parser;
    char index[YCOMMAND_INDEX_LEN+1];
    char id[YCOMMAND_ID_LEN+1];
    char security[YCOMMAND_SECURITY_LEN+1];
    char data[YCOMMAND_DATA_LEN+1];
    YCmdPResult result;
}YCommandParser;

/* -------------------------- External variables --------------------------- */
/* -------------------------- Function prototypes -------------------------- */
YCmdPResult YCommandParser_search(YCommandParser *me, char *p, ruint size);
YCmdPResult YCommandParser_securityCheck(YCommandParser *me, char *pkey);
ruint YCommandParser_getId(YCommandParser *me);
char * YCommandParser_getIndex(YCommandParser *me);
char * YCommandParser_getData(YCommandParser *me);

/* -------------------- External C language linkage end -------------------- */
#ifdef __cplusplus
}
#endif

/* ------------------------------ Module end ------------------------------- */
#endif

/* ------------------------------ End of file ------------------------------ */
