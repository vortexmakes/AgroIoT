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

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
typedef struct YCmdFormat
{
    YCmd_t (*fmt)(YCommand *pCmd, const struct YCmdFormat *pFmt);
    ruint min;
    ruint max;
}YCmdFormat;

/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
static YCommandParser yCmdParser;

/* ----------------------- Local function prototypes ----------------------- */
static YCmd_t fmt_none(YCommand *pCmd, const YCmdFormat *pFmt);
static YCmd_t fmt_serverIp(YCommand *pCmd, const YCmdFormat *pFmt);
static YCmd_t fmt_string(YCommand *pCmd, const YCmdFormat *pFmt);
static YCmd_t fmt_rui8(YCommand *pCmd, const YCmdFormat *pFmt);

static const YCmdFormat fmtTable[YCmdNum] =
{
    { fmt_serverIp, 7, IP_LENGTH },
    { fmt_string, 1, PORT_LENGTH},
    { fmt_rui8, 1, 2 },
    { fmt_rui8, 1, 2 },
    { fmt_rui8, 1, 2 },
    { fmt_rui8, 1, 2 },
    { fmt_rui8, 1, 2 },
    { fmt_rui8, 1, 2 },
    { fmt_rui8, 1, 2 },
    { fmt_rui8, 1, 2 },
    { fmt_rui8, 1, 2 },
    { fmt_rui8, 1, 2 },
    { fmt_rui8, 1, 2 },
    { fmt_none, 0, 0 },
    { fmt_rui8, 1, 2 },
    { fmt_none, 0, 0 }
};


/* ---------------------------- Local functions ---------------------------- */

static
YCmd_t
fmt_none(YCommand *pCmd, const YCmdFormat *pFmt)
{
    return YCmdWrongFormat;
}

static
YCmd_t
fmt_serverIp(YCommand *pCmd, const YCmdFormat *pFmt)
{
    return YCmdWrongFormat;
}

static
YCmd_t
fmt_string(YCommand *pCmd, const YCmdFormat *pFmt)
{
    return YCmdWrongFormat;
}

static
YCmd_t
fmt_rui8(YCommand *pCmd, const YCmdFormat *pFmt)
{
    return YCmdWrongFormat;
}

/* ---------------------------- Global functions --------------------------- */
YCmd_t
YCommand_parse(YCommand *pCmd, char *p, ruint size)
{
    YCmd_t cmd;
    const YCmdFormat *pFmt;

    if(p == NULL || size == 0)
    {
        return YCmdUnknown;
    }
 
    if(YCommandParser_search(&yCmdParser, p, size) < 0)
        return YCmdUnknown;

    if(YCommandParser_securityCheck(&yCmdParser, YCOMMAND_SECURITY_KEY_DFT) < 0)
    {
        return YCmdInvalidKey;
    }

    cmd = YCommandParser_getId(&yCmdParser);

    memset(pCmd, 0, sizeof(YCommand));

    strcpy(pCmd->index, YCommandParser_getIndex(&yCmdParser));
    pCmd->id = YCommandParser_getId(&yCmdParser);
    
    pFmt = &fmtTable[cmd];
    
    return (*pFmt->fmt)(pCmd, pFmt);
}

/* ------------------------------ End of file ------------------------------ */
