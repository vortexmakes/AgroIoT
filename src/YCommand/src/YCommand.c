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
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
static YCommandParser yCmdParser;

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
 
    if(YCommandParser_search(&yCmdParser, p, size) < 0)
        return TypeOfCmdUnknown;

    if(YCommandParser_securityCheck(&yCmdParser, YCOMMAND_SECURITY_KEY_DFT) < 0)
    {
        return TypeOfCmdInvalidKey;
    }

    cmd = YCommandParser_getId(&yCmdParser);

    strcpy(pCmd->index, YCommandParser_getIndex(&yCmdParser));
    pCmd->id = YCommandParser_getId(&yCmdParser);
    

    return cmd;
}


/* ---------------------------- Global functions --------------------------- */
/* ------------------------------ End of file ------------------------------ */
