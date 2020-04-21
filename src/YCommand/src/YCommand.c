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
#include "rkh.h"
#include "YCommand.h"
#include "YCommandParser.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
TypeOfCmd
YCommand_parse(YCommand *pCmd, char *p, ruint size)
{
    TypeOfCmd cmd;

    if(p == NULL || size == 0)
    {
        return TypeOfCmdUnknown;
    }
 
    if(YCommandParser_search(p, size) < 0)
        return TypeOfCmdUnknown;

    if(YCommandParser_securityCheck(COMMAND_SECURITY_KEY_DFT) < 0)
    {
        return TypeOfCmdInvalidKey;
    }

    cmd = YCommandParser_getId();

    return cmd;
}


/* ---------------------------- Global functions --------------------------- */
/* ------------------------------ End of file ------------------------------ */
