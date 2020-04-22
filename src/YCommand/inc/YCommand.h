/**
 *  \file       YCommand.h
 *  \brief      Yipies Command .
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
#include "ssp.h"
#include "YCommandParser.h"
#include "Config.h"

/* ---------------------- External C language linkage ---------------------- */
#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------- Macros -------------------------------- */
/* -------------------------------- Constants ------------------------------ */
typedef enum TypeOfCmd TypeOfCmd;
enum TypeOfCmd
{
    TypeOfCmdServerIp,
    TypeOfCmdServerPort,
    TypeOfCmdConnectionTime,
    TypeOfCmdGpsTime,
    TypeOfCmdAccLimit,
    TypeOfCmdBreakLimit,
    TypeOfCmdStatus,
    TypeOfCmdSetOut1,
    TypeOfCmdSetOut2,
    TypeOfCmdSetOut3,
    TypeOfCmdSetOut4,
    TypeOfCmdSetOut5,
    TypeOfCmdSetOut6,
    TypeOfCmdReset,
    TypeOfCmdSampleTime,
    TypeOfCmdDataFormat,

    TypeOfCmdInvalidKey,

    TypeOfCmdUnknown
};

#define YCOMMAND_SECURITY_KEY_DFT    "1234"

/* ------------------------------- Data types ------------------------------ */
typedef union
{
    char serverIp[IP_LENGTH+1];
    char serverPort[PORT_LENGTH+1];
    rui8_t connTime;
    rui8_t updateGPSTime;
    rui8_t accLimit;
    rui8_t brLimit;
    rui8_t status;
    rui8_t outValue;
    rui8_t sampleTime;
}cmdData;

typedef struct
{
    char index[YCOMMAND_INDEX_LEN+1];
    rui8_t id;
    cmdData data;
}YCommand;

/* -------------------------- External variables --------------------------- */
/* -------------------------- Function prototypes -------------------------- */
TypeOfCmd YCommand_parse(YCommand *pCmd, char *p, ruint size);

/* -------------------- External C language linkage end -------------------- */
#ifdef __cplusplus
}
#endif

/* ------------------------------ Module end ------------------------------- */
#endif

/* ------------------------------ End of file ------------------------------ */
