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

    TypeOfCmdUnknown
};

/* ------------------------------- Data types ------------------------------ */
typedef struct YCommand
{
    SSP parser;
    char index[COMMAND_INDEX_LEN+1];
    char id[COMMAND_ID_LEN+1];
    char security[COMMAND_SECURITY_LEN+1];
    char data[COMMAND_DATA_LEN+1];
    ruint command;
}YCommand;

/* -------------------------- External variables --------------------------- */
/* -------------------------- Function prototypes -------------------------- */
void YCommandParser_init(YCommand *p);
TypeOfCmd YCommandParser_search(char *p, ruint size);

/* -------------------- External C language linkage end -------------------- */
#ifdef __cplusplus
}
#endif

/* ------------------------------ Module end ------------------------------- */
#endif

/* ------------------------------ End of file ------------------------------ */
