/**
 *  \file       YCommandExec.c
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
#include "YCommandExec.h"
#include "Config.h"
#include "dOut.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
typedef struct YCmdExec
{
    rInt (*exec)(YCmdParserData *pCmd);
}YCmdExec;

/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
static rInt exec_cmdNotSupported(YCmdParserData *pCmd);
static rInt exec_serverIp(YCmdParserData *pCmd);
static rInt exec_serverPort(YCmdParserData *pCmd);
static rInt exec_connectionTime(YCmdParserData *pCmd);
static rInt exec_gpsTime(YCmdParserData *pCmd);
static rInt exec_accLimit(YCmdParserData *pCmd);
static rInt exec_breakLimit(YCmdParserData *pCmd);
static rInt exec_cmdStatus(YCmdParserData *pCmd);
static rInt exec_setOut1(YCmdParserData *pCmd);
static rInt exec_setOut2(YCmdParserData *pCmd);
static rInt exec_reset(YCmdParserData *pCmd);
static rInt exec_sampleTime(YCmdParserData *pCmd);
static rInt exec_dataFormat(YCmdParserData *pCmd);

/* ----------------------- Local function prototypes ----------------------- */
static const YCmdExec execTable[YCmdNum] =
{
    { exec_serverIp },       // YCmdServerIp
    { exec_serverPort },     // YCmdServerPort
    { exec_connectionTime }, // YCmdConnectionTime
    { exec_gpsTime },        // YCmdGpsTime
    { exec_accLimit },       // YCmdAccLimit
    { exec_breakLimit },     // YCmdBreakLimit
    { exec_cmdStatus },      // YCmdStatus
    { exec_setOut1 },        // YCmdSetOut1
    { exec_setOut2 },        // YCmdSetOut2
    { exec_cmdNotSupported },// YCmdSetOut3
    { exec_cmdNotSupported },// YCmdSetOut4
    { exec_cmdNotSupported },// YCmdSetOut5
    { exec_cmdNotSupported },// YCmdSetOut6
    { exec_reset },          // YCmdReset
    { exec_sampleTime },     // YCmdSampleTime
    { exec_dataFormat }      // YCmdDataFormat
};


/* ---------------------------- Local functions ---------------------------- */
static
rInt
exec_cmdNotSupported(YCmdParserData *pCmd)
{
    pCmd->p->reset = 0;
    return -1;
}

static
rInt
exec_serverIp(YCmdParserData *pCmd)
{
    Config_setConnectionDomain(pCmd->data.serverIp);
    pCmd->p->reset = 0;
    return 0;
}

static
rInt exec_serverPort(YCmdParserData *pCmd)
{
    Config_setConnectionPort(pCmd->data.serverPort);
    pCmd->p->reset = 0;
    return 0;
}

static 
rInt 
exec_connectionTime(YCmdParserData *pCmd)
{
    Config_setConnTime(pCmd->data.connTime);
    pCmd->p->reset = 0;
    return 0;
}

static
rInt 
exec_gpsTime(YCmdParserData *pCmd)
{
    Config_setUpdateGPSTime(pCmd->data.updateGPSTime);
    pCmd->p->reset = 0;
    return 0;
}

static 
rInt 
exec_accLimit(YCmdParserData *pCmd)
{
    Config_setAccLimit(pCmd->data.accLimit);
    pCmd->p->reset = 0;
    return 0;
}

static 
rInt 
exec_breakLimit(YCmdParserData *pCmd)
{
    Config_setBrLimit(pCmd->data.brLimit);
    pCmd->p->reset = 0;
    return 0;
}

static 
rInt 
exec_cmdStatus(YCmdParserData *pCmd)
{
    Config_setDefault(pCmd->data.status);
    pCmd->p->reset = 1;
    return 0;
}

static 
rInt 
exec_setOut1(YCmdParserData *pCmd)
{
    Config_setDftDigOut(pCmd->data.outValue);
    dOut_set(dOut1, pCmd->data.outValue, 1);
    pCmd->p->reset = 0;
    return 0;
}

static 
rInt 
exec_setOut2(YCmdParserData *pCmd)
{
    Config_setDftDigOut(pCmd->data.outValue);
    dOut_set(dOut2, pCmd->data.outValue, 1);
    pCmd->p->reset = 0;
    return 0;
}

static 
rInt 
exec_reset(YCmdParserData *pCmd)
{
    pCmd->p->reset = 1;
    return 0;
}

static 
rInt 
exec_sampleTime(YCmdParserData *pCmd)
{
    Config_setMapTimeOnRunning(pCmd->data.sampleTime);
    pCmd->p->reset = 0;
    return 0;
}

static 
rInt 
exec_dataFormat(YCmdParserData *pCmd)
{
    return 0;
}

/* ---------------------------- Global functions --------------------------- */
YCmdRes
YCommandExec_exec(YCmdParserData *pCmd)
{
    const YCmdExec *pExec;

    if(pCmd->id >= YCmdNum)
       return YCmdExecError;

    pExec = &execTable[pCmd->id];

    if((*pExec->exec)(pCmd) < 0)
        return YCmdExecError;

    return YCmdOk;    
}

/* ------------------------------ End of file ------------------------------ */
