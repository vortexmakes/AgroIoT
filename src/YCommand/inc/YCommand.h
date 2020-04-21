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

/* ---------------------- External C language linkage ---------------------- */
#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------- Macros -------------------------------- */
/* -------------------------------- Constants ------------------------------ */
#define COMMAND_INDEX_LEN       11
#define COMMAND_ID_LEN          2
#define COMMAND_SECURITY_LEN    3
#define COMMAND_DATA_LEN        32

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

#define COMMAND_SECURITY_KEY_DFT    "1234"

/* ------------------------------- Data types ------------------------------ */
typedef union
{
    char raw[COMMAND_DATA_LEN+1];
    char serverIp[COMMAND_DATA_LEN+1];
    char serverPort[COMMAND_DATA_LEN+1];
}cmdData;

typedef struct
{
    char index[COMMAND_INDEX_LEN+1];
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
