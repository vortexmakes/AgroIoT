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
    rInt (*exec)(YCmdParserData *pcmd);
}YCmdExec;

/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
static rInt exec_serverIp(YCmdParserData *pcmd);
static rInt exec_serverPort(YCmdParserData *pcmd);
static rInt exec_connectionTime(YCmdParserData *pcmd);
static rInt exec_gpsTime(YCmdParserData *pcmd);
static rInt exec_accLimit(YCmdParserData *pcmd);
static rInt exec_breakLimit(YCmdParserData *pcmd);
static rInt exec_cmdStatus(YCmdParserData *pcmd);
static rInt exec_setOut1(YCmdParserData *pcmd);
static rInt exec_setOut2(YCmdParserData *pcmd);
static rInt exec_setOut3(YCmdParserData *pcmd);
static rInt exec_setOut4(YCmdParserData *pcmd);
static rInt exec_setOut5(YCmdParserData *pcmd);
static rInt exec_setOut6(YCmdParserData *pcmd);
static rInt exec_reset(YCmdParserData *pcmd);
static rInt exec_sampleTime(YCmdParserData *pcmd);
static rInt exec_dataFormat(YCmdParserData *pcmd);

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
exec_serverIp(YCmdParserData *pcmd)
{
    return 0;
}

static
rInt exec_serverPort(YCmdParserData *pcmd)
{
    return 0;
}

static 
rInt 
exec_connectionTime(YCmdParserData *pcmd)
{
    return 0;
}

static
rInt 
exec_gpsTime(YCmdParserData *pcmd)
{
    return 0;
}

static 
rInt 
exec_accLimit(YCmdParserData *pcmd)
{
    return 0;
}

static 
rInt 
exec_breakLimit(YCmdParserData *pcmd)
{
    return 0;
}

static 
rInt 
exec_cmdStatus(YCmdParserData *pcmd)
{
    return 0;
}

static 
rInt 
exec_setOut1(YCmdParserData *pcmd)
{
    return 0;
}

static 
rInt 
exec_setOut2(YCmdParserData *pcmd)
{
    return 0;
}

static 
rInt 
exec_setOut3(YCmdParserData *pcmd)
{
    return 0;
}

static 
rInt 
exec_setOut4(YCmdParserData *pcmd)
{
    return 0;
}

static 
rInt 
exec_setOut5(YCmdParserData *pcmd)
{
    return 0;
}

static 
rInt 
exec_setOut6(YCmdParserData *pcmd)
{
    return 0;
}

static 
rInt 
exec_reset(YCmdParserData *pcmd)
{
    return 0;
}

static 
rInt 
exec_sampleTime(YCmdParserData *pcmd)
{
    return 0;
}

static 
rInt 
exec_dataFormat(YCmdParserData *pcmd)
{
    return 0;
}

/* ---------------------------- Global functions --------------------------- */
YCmdRes
YCommandExec_exec(YCmdParserData *pcmd)
{
    
}

/* ------------------------------ End of file ------------------------------ */
