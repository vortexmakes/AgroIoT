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
    YCmdOk,
    YAck,

    YCmdUnknown = -1,
    YCmdInvalidKey = -2,
    YCmdWrongLen = -3,
    YCmdWrongFormat = -4,
    YCmdExecError = -5
} YCmdRes;

#define YCOMMAND_SECURITY_KEY_DFT    "123"

/* ------------------------------- Data types ------------------------------ */
typedef struct
{
    char index[YCOMMAND_INDEX_LEN+1]; // GPRS command index number [1 .. 11] 
    rui8_t reset;                     // After command execution reset flag
} YCommand;

/* -------------------------- External variables --------------------------- */
/* -------------------------- Function prototypes -------------------------- */
/*
 *  \brief
 *  Yipies Command message parser (SMS or GPRS) key validation and execution
 *  message format: 
 *        
 *        < SMS or Gprs Header,CmdId,SecurityKey,CmdData; >
 *
 *  Gprs Header: "!3<index>" where index is an integer expresed in char
 *                        1 to 11 bytes len. Used for command execution 
 *                        confirmation (Gprs only)
 *  SMS Header:  "Im:"
 *  CmdId:       Command identification number [1 to 2 bytes len]
 *  SecurityKey: Security Key [3 bytes len], is hardcoded to (123)
 *  CmdData:     Command data buffer 1 to 32 bytes len
 *
 *  \param pCmd: destination YCommand pointer where fill command args.
 *  \param p: received message buffer (null or not null terminated).
 *  \param size: size of message.
 *
 *  \return
 *      - If is ACK message returns YAck.
 *      - If is command message and its execution success returns 
 *        YCmdOk, filling pCmd with command args (id, index and reset).
 *      - Else returns the error code according to YCmdRes.
 */
YCmdRes YCommand_parseAndExec(YCommand *pCmd, char *p, ruint size);

/* -------------------- External C language linkage end -------------------- */
#ifdef __cplusplus
}
#endif

/* ------------------------------ Module end ------------------------------- */
#endif

/* ------------------------------ End of file ------------------------------ */
