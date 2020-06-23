/**
 *  \file       YCommandFormat.c
 *  \brief      ...
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2020.04.17  DaBa  v1.0.00  Initial version
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  DaBa  Dario Baliña db@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */
#include <string.h>
#include <stdlib.h>
#include "rkh.h"
#include "YCommandFormat.h"
#include "IpValid.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
typedef struct YCmdFormat
{
	YCmdRes (*fmt)(YCmdParserData *pCmd, char *pData);
    ruint min;
    ruint max;
} YCmdFormat;

/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
static YCmdRes fmt_none(YCmdParserData *pCmd, char *pData);
static YCmdRes fmt_serverIp(YCmdParserData *pCmd, char *pData);
static YCmdRes fmt_string(YCmdParserData *pCmd, char *pData);
static YCmdRes fmt_rui8(YCmdParserData *pCmd, char *pData);
static YCmdRes fmt_rui16(YCmdParserData *pCmd, char *pData);

static const YCmdFormat fmtTable[YCmdNum] =
{
    { fmt_serverIp, 7, IP_LENGTH },     // YCmdServerIp
    { fmt_string, 1, PORT_LENGTH},      // YCmdServerPort
    { fmt_rui16, 1, 4 },                // YCmdConnectionTime
    { fmt_rui16, 1, 4 },                // YCmdGpsTime
    { fmt_rui8, 1, 2 },                 // YCmdAccLimit
    { fmt_rui8, 1, 2 },                 // YCmdBreakLimit
    { fmt_rui8, 1, 1 },                 // YCmdStatus
    { fmt_rui8, 1, 1 },                 // YCmdSetOut1
    { fmt_rui8, 1, 1 },                 // YCmdSetOut2
    { fmt_rui8, 1, 1 },                 // YCmdSetOut3
    { fmt_rui8, 1, 1 },                 // YCmdSetOut4
    { fmt_rui8, 1, 1 },                 // YCmdSetOut5
    { fmt_rui8, 1, 1 },                 // YCmdSetOut6
    { fmt_none, 1, 1 },                 // YCmdReset
    { fmt_rui8, 1, 1 },                 // YCmdSampleTime
    { fmt_none, 1, 1 }                  // YCmdDataFormat
};

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
static
rInt
checkLen(char *p, ruint min, ruint max)
{
    ruint l = strlen(p);

    if(l < min || l > max)
        return -1;
    return 0;
}

static
YCmdRes
fmt_none(YCmdParserData *pCmd, char *pData)
{
    return YCmdOk;
}

static
YCmdRes
fmt_serverIp(YCmdParserData *pCmd, char *pData)
{
    if(IpValid_check(pData))
    {
        strcpy(pCmd->data.serverIp, pData);
        return YCmdOk;
    }
    
    return YCmdWrongFormat;
}

static
YCmdRes
fmt_string(YCmdParserData *pCmd, char *pData)
{
    strcpy(pCmd->data.serverIp, pData);

    return YCmdOk;
}

static
YCmdRes
fmt_rui16(YCmdParserData *pCmd, char *pData)
{
    pCmd->data._rui16 = (rui16_t)atoi(pData);

    return YCmdOk;
}

static
YCmdRes
fmt_rui8(YCmdParserData *pCmd, char *pData)
{
    pCmd->data._rui8 = (rui8_t)atoi(pData);

    return YCmdOk;
}

/* ---------------------------- Global functions --------------------------- */
YCmdRes
YCommandFormat_format(YCmdParserData *pCmd, char *p)
{
    const YCmdFormat *pFmt;

    if(pCmd->id >= YCmdNum)
        return YCmdWrongFormat;

    pFmt = &fmtTable[pCmd->id];
    
    if(checkLen(p, pFmt->min, pFmt->max) < 0)
        return YCmdWrongLen;

    if((*pFmt->fmt)(pCmd, p) < 0)
        return YCmdWrongFormat;

    return YCmdOk;
}

/* ------------------------------ End of file ------------------------------ */
