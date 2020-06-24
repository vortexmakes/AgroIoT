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
#include "YCommandParser.h"
#include "YCommandExec.h"
#include "IpValid.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ---------------------------- Global functions --------------------------- */
YCmdRes
YCommand_parseAndExec(YCommand *pCmd, char *p, ruint size)
{
    YCmdParserData cmd;
    YCmdRes r;

    if(pCmd == NULL || p == NULL || size == 0)
    {
        return YCmdUnknown;
    }

    memset(pCmd, 0, sizeof(YCommand));
 
    cmd.p = pCmd;
    memset(&(cmd.data), 0, sizeof(CmdData));

    r = YCommandParser_parse(&cmd, p, size);
    if((r < 0) || (r == YAck))
        return r;

    if(YCommandExec_exec(&cmd) < 0)
    {
        return YCmdExecError; 
    }

    return YCmdOk;
}

/* ------------------------------ End of file ------------------------------ */
