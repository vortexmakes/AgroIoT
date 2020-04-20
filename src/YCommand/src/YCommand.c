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

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
static YCommand yCmd;

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */

YCommand_parse(char *p, ruint size)
{
    TypeOfCmd cmd;

    if(p == NULL || size == 0)
    {
        return 0;
    }

    YCommandParser_init(&yCmd);
    
    cmd = CommandParser_search(p, size);
        
    return cmd;
}


/* ---------------------------- Global functions --------------------------- */
/* ------------------------------ End of file ------------------------------ */
