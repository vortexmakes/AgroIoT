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

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
typedef struct YCmdExec
{
    rInt (*exec)(YCommand *pcmd);
}YCmdExec;

/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
static rInt exec_serverIp(YCommand *pcmd);
static rInt exec_serverPort(YCommand *pcmd);
static rInt exec_connectionTime(YCommand *pcmd);
static rInt exec_gpsTime(YCommand *pcmd);
static rInt exec_accLimit(YCommand *pcmd);
static rInt exec_breakLimit(YCommand *pcmd);
static rInt exec_cmdStatus(YCommand *pcmd);
static rInt exec_setOut1(YCommand *pcmd);
static rInt exec_setOut2(YCommand *pcmd);
static rInt exec_setOut3(YCommand *pcmd);
static rInt exec_setOut4(YCommand *pcmd);
static rInt exec_setOut5(YCommand *pcmd);
static rInt exec_setOut6(YCommand *pcmd);
static rInt exec_reset(YCommand *pcmd);
static rInt exec_sampleTime(YCommand *pcmd);
static rInt exec_dataFormat(YCommand *pcmd);

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
    { exec_setOut3 },        // YCmdSetOut3
    { exec_setOut4 },        // YCmdSetOut4
    { exec_setOut5 },        // YCmdSetOut5
    { exec_setOut6 },        // YCmdSetOut6
    { exec_reset },          // YCmdReset
    { exec_sampleTime },     // YCmdSampleTime
    { exec_dataFormat }      // YCmdDataFormat
};


/* ---------------------------- Local functions ---------------------------- */
static
rInt
exec_serverIp(YCommand *pcmd)
{
    return 0;
}

static
rInt exec_serverPort(YCommand *pcmd)
{
    return 0;
}

static 
rInt 
exec_connectionTime(YCommand *pcmd)
{
    return 0;
}

static
rInt 
exec_gpsTime(YCommand *pcmd)
{
    return 0;
}

static 
rInt 
exec_accLimit(YCommand *pcmd)
{
    return 0;
}

static 
rInt 
exec_breakLimit(YCommand *pcmd)
{
    return 0;
}

static 
rInt 
exec_cmdStatus(YCommand *pcmd)
{
    return 0;
}

static 
rInt 
exec_setOut1(YCommand *pcmd)
{
    return 0;
}

static 
rInt 
exec_setOut2(YCommand *pcmd)
{
    return 0;
}

static 
rInt 
exec_setOut3(YCommand *pcmd)
{
    return 0;
}

static 
rInt 
exec_setOut4(YCommand *pcmd)
{
    return 0;
}

static 
rInt 
exec_setOut5(YCommand *pcmd)
{
    return 0;
}

static 
rInt 
exec_setOut6(YCommand *pcmd)
{
    return 0;
}

static 
rInt 
exec_reset(YCommand *pcmd)
{
    return 0;
}

static 
rInt 
exec_sampleTime(YCommand *pcmd)
{
    return 0;
}

static 
rInt 
exec_dataFormat(YCommand *pcmd)
{
    return 0;
}

/* ---------------------------- Global functions --------------------------- */
YCmd_t
YCommand_exec(YCommand *pcmd)
{
    
}

/* ------------------------------ End of file ------------------------------ */
