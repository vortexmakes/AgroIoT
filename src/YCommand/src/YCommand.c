/**
 *  \file       YCommand.c
 *  \brief      ...
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2020.04.17  Daba  v1.0.00  Initial version
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
#include "YCommand.h"
#include "IpValid.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
typedef struct YCmdFormat
{
    YCmd_t (*fmt)(YCommand *pCmd, char *pData);
    ruint min;
    ruint max;
}YCmdFormat;

/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
static YCommandParser yCmdParser;

/* ----------------------- Local function prototypes ----------------------- */
static YCmd_t fmt_none(YCommand *pCmd, char *pData);
static YCmd_t fmt_serverIp(YCommand *pCmd, char *pData);
static YCmd_t fmt_string(YCommand *pCmd, char *pData);
static YCmd_t fmt_rui8(YCommand *pCmd, char *pData);
static YCmd_t fmt_rui16(YCommand *pCmd, char *pData);

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
YCmd_t
fmt_none(YCommand *pCmd, char *pData)
{
    return pCmd->id;
}

static
YCmd_t
fmt_serverIp(YCommand *pCmd, char *pData)
{
    if(IpValid_check(pData))
    {
        strcpy(pCmd->data.serverIp, pData);
    }
    else
    {
        pCmd->id = YCmdWrongFormat;
    }

    return pCmd->id;
}

static
YCmd_t
fmt_string(YCommand *pCmd, char *pData)
{
    strcpy(pCmd->data.serverIp, pData);

    return pCmd->id;
}

static
YCmd_t
fmt_rui16(YCommand *pCmd, char *pData)
{
    pCmd->data._rui16 = (rui16_t)atoi(pData);

    return pCmd->id;
}

static
YCmd_t
fmt_rui8(YCommand *pCmd, char *pData)
{
    pCmd->data._rui8 = (rui8_t)atoi(pData);

    return pCmd->id;
}

/* ---------------------------- Global functions --------------------------- */
YCmd_t
YCommand_parse(YCommand *pCmd, char *p, ruint size)
{
    YCmd_t cmd;
    const YCmdFormat *pFmt;
    char *pData;

    if(pCmd == NULL || p == NULL || size == 0)
    {
        return YCmdUnknown;
    }

    memset(pCmd, 0, sizeof(YCommand));
 
    if(YCommandParser_search(&yCmdParser, p, size) < 0)
        return YCmdUnknown;

    cmd = YCommandParser_getId(&yCmdParser);
    if(cmd == YCmdUnknown)
    {
        return YCmdUnknown;
    }

    if(YCommandParser_securityCheck(&yCmdParser, YCOMMAND_SECURITY_KEY_DFT) < 0)
    {
        return YCmdInvalidKey;
    }

    pData = YCommandParser_getData(&yCmdParser);
    pFmt = &fmtTable[cmd];
    
    if(checkLen(pData, pFmt->min, pFmt->max) < 0)
        return YCmdWrongLen;

    pCmd->id = YCommandParser_getId(&yCmdParser);
    cmd = (*pFmt->fmt)(pCmd, pData);

    if(cmd >= 0)
    {
        strcpy(pCmd->index, YCommandParser_getIndex(&yCmdParser));
    }
    
    return cmd; 
}

/* ------------------------------ End of file ------------------------------ */
