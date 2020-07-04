/**
 *  \file       YCommandParser.h
 *  \brief      Yipies Command Parser.
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
#ifndef __YCOMMANDPARSER_H__
#define __YCOMMANDPARSER_H__

/* ----------------------------- Include files ----------------------------- */
#include "rkh.h"
#include "ssp.h"
#include "YCommand.h"
#include "Config.h"


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
    YAckFound,

    YCommandNotFound = -1,
    YCommandInvalidKey = YCommandNotFound,

} YCmdPResult;

/* ------------------------------- Data types ------------------------------ */
typedef struct
{
    SSP parser;
    char index[YCOMMAND_INDEX_LEN+1];
    char id[YCOMMAND_ID_LEN+1];
    char security[YCOMMAND_SECURITY_LEN+1];
    char data[YCOMMAND_DATA_LEN+1];
    YCmdPResult result;
} YCommandParser;

typedef union
{
    char serverIp[IP_LENGTH+1];
    char serverPort[PORT_LENGTH+1];

    rui16_t _rui16;
        rui16_t connTime;
        rui16_t updateGPSTime;

    rui8_t _rui8;
        rui8_t sampleTime;
        rui8_t accLimit;
        rui8_t brLimit;
        rui8_t status;
        rui8_t outValue;
} CmdData;

typedef struct
{
    YCommand *p;
    YCmd_t id;
    CmdData data;
} YCmdParserData;

/* -------------------------- External variables --------------------------- */
/* -------------------------- Function prototypes -------------------------- */
YCmdRes YCommandParser_parse(YCmdParserData *pCmd, char *p, ruint size);

/* -------------------- External C language linkage end -------------------- */
#ifdef __cplusplus
}
#endif

/* ------------------------------ Module end ------------------------------- */
#endif

/* ------------------------------ End of file ------------------------------ */
