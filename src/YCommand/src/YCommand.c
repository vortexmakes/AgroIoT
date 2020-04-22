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
#include "rkh.h"
#include "YCommand.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
typedef rInt (*yCmdFormat)(YCommand *pCmd, char *data);

/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
static YCommandParser yCmdParser;

/* ----------------------- Local function prototypes ----------------------- */
static rInt fmt_none(YCommand *pCmd, char *p);
static rInt fmt_serverIp(YCommand *pCmd, char *p);
static rInt fmt_serverPort(YCommand *pCmd, char *p);
static rInt fmt_rui8(YCommand *pCmd, char *p);

static const yCmdFormat formatTable[YCmdNum] =
{
    fmt_serverIp,
    fmt_serverPort,
    fmt_rui8,
    fmt_rui8,
    fmt_rui8,
    fmt_rui8,
    fmt_rui8,
    fmt_rui8,
    fmt_rui8,
    fmt_rui8,
    fmt_rui8,
    fmt_rui8,
    fmt_rui8,
    fmt_none,
    fmt_rui8,
    fmt_none
};


/* ---------------------------- Local functions ---------------------------- */
static
rInt
fmt_none(YCommand *pCmd, char *p)
{
    return 0;
}

static
rInt
fmt_serverIp(YCommand *pCmd, char *p)
{
    return 0;
}

static
rInt
fmt_serverPort(YCommand *pCmd, char *p)
{
    return 0;
}

static
rInt
fmt_rui8(YCommand *pCmd, char *p)
{
    return 0;
}

/* ---------------------------- Global functions --------------------------- */
YCmd_t
YCommand_parse(YCommand *pCmd, char *p, ruint size)
{
    YCmd_t cmd;

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

    strcpy(pCmd->index, YCommandParser_getIndex(&yCmdParser));
    pCmd->id = YCommandParser_getId(&yCmdParser);
    
    
    return cmd;
}

/* ------------------------------ End of file ------------------------------ */
