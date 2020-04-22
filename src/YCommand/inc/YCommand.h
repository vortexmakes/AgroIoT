/**
 *  \file       YCommand.h
 *  \brief      Yipies Command.
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2020.04.17  Daba  v1.0.00  Initial version
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  DaBa  Dario Balina db@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* --------------------------------- Module -------------------------------- */
#ifndef __YCOMMAND_H__
#define __YCOMMAND_H__

/* ----------------------------- Include files ----------------------------- */
#include "rkh.h"
#include "YCommandParser.h"
#include "Config.h"

/* ---------------------- External C language linkage ---------------------- */
#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------- Macros -------------------------------- */
/* -------------------------------- Constants ------------------------------ */
typedef enum
{
    YCmdServerIp,
    YCmdServerPort,
    YCmdConnectionTime,
    YCmdGpsTime,
    YCmdAccLimit,
    YCmdBreakLimit,
    YCmdStatus,
    YCmdSetOut1,
    YCmdSetOut2,
    YCmdSetOut3,
    YCmdSetOut4,
    YCmdSetOut5,
    YCmdSetOut6,
    YCmdReset,
    YCmdSampleTime,
    YCmdDataFormat,

    YCmdNum,

    YCmdUnknown = -1,
    YCmdInvalidKey = -2,
    YCmdWrongLen = -3,
    YCmdWrongFormat = -4

}YCmd_t;

#define YCOMMAND_SECURITY_KEY_DFT    "123"

/* ------------------------------- Data types ------------------------------ */
typedef union
{
    char serverIp[IP_LENGTH+1];
    char serverPort[PORT_LENGTH+1];
    rui8_t connTime;
    rui8_t updateGPSTime;
    rui8_t sampleTime;
    rui8_t accLimit;
    rui8_t brLimit;
    rui8_t status;
    rui8_t outValue;
}cmdData;

typedef struct
{
    char index[YCOMMAND_INDEX_LEN+1];
    YCmd_t id;
    cmdData data;
}YCommand;

/* -------------------------- External variables --------------------------- */
/* -------------------------- Function prototypes -------------------------- */
YCmd_t YCommand_parse(YCommand *pCmd, char *p, ruint size);

/* -------------------- External C language linkage end -------------------- */
#ifdef __cplusplus
}
#endif

/* ------------------------------ Module end ------------------------------- */
#endif

/* ------------------------------ End of file ------------------------------ */
