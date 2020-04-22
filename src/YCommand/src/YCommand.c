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
    return YCmdWrongFormat;
}

#include "stdio.h"
static
YCmd_t
fmt_serverIp(YCommand *pCmd, char *pData)
{
    printf("server ip:%d\r\n",pCmd->id);
    if(IpValid_check(pData))
    {
        printf("is valid\r\n");
        strcpy(pCmd->data.serverIp, pData);
    }
    else
    {
        pCmd->id = YCmdWrongFormat;
    }

    printf("%s\r\n", pCmd->data.serverIp);
    printf("%s\r\n", pData);
    printf("server ip:%d\r\n",pCmd->id);
    return pCmd->id;
}

static
YCmd_t
fmt_string(YCommand *pCmd, char *pData)
{
    return YCmdWrongFormat;
}

static
YCmd_t
fmt_rui8(YCommand *pCmd, char *pData)
{
    return YCmdWrongFormat;
}

/* ---------------------------- Global functions --------------------------- */
YCmd_t
YCommand_parse(YCommand *pCmd, char *p, ruint size)
{
    YCmd_t cmd;
    const YCmdFormat *pFmt;
    char *pData;

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
    pData = YCommandParser_getData(&yCmdParser);
    pFmt = &fmtTable[cmd];
    
    if(checkLen(pData, pFmt->min, pFmt->max) < 0)
        return YCmdWrongLen;

    memset(pCmd, 0, sizeof(YCommand));
    
    pCmd->id = YCommandParser_getId(&yCmdParser);
    cmd = (*pFmt->fmt)(pCmd, pData);

    if(cmd > 0)
    {
        strcpy(pCmd->index, YCommandParser_getIndex(&yCmdParser));
    }
    
    return cmd; 
}

/* ------------------------------ End of file ------------------------------ */
