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

} YCmd_t;

#define YCOMMAND_SECURITY_KEY_DFT    "123"

/* ------------------------------- Data types ------------------------------ */
typedef union
{
    char serverIp[IP_LENGTH+1];
    char serverPort[PORT_LENGTH+1];

    rui16_t _rui16;
        rui16_t connTime;
        rui16_t updateGPSTime;

    rui8_t _rui8;
        rui8_t sampleTime;
        rui8_t accLimit;
        rui8_t brLimit;
        rui8_t status;
        rui8_t outValue;
} cmdData;

typedef struct
{
    char index[YCOMMAND_INDEX_LEN+1];
    YCmd_t id;
    cmdData data;
} YCommand;

/* -------------------------- External variables --------------------------- */
/* -------------------------- Function prototypes -------------------------- */
/*
 *  \brief
 *  Yipies Command message parser (SMS or GPRS) and key validation
 *  message format: 
 *        
 *        < SMS or Gprs Header,CmdId,SecurityKey,CmdData; >
 *
 *  Gprs Header: "!3<index>" where index is an integer expresed in char
 *                        1 to 11 bytes len. Used for command execution 
 *                        confirmation (Gprs only)
 *  SMS Header:  "Im:"
 *  CmdId:       Command identification number [1 to 2 bytes len]
 *  SecurityKey: Security Key [3 bytes len]
 *  CmdData:     Command data buffer 1 to 32 bytes len
 *
 *  \param pCmd: destination YCommand pointer.
 *  \param p: received message buffer (null or not null terminated).
 *  \param size: size of message.
 *
 *  \return
 *      - If parse success returns command id according to YCmd_t 
 *                  and fill pCmd with command data args.
 *      - If error returns error code according to YCmd_t.
 */
YCmd_t YCommand_parse(YCommand *pCmd, char *p, ruint size);

/* -------------------- External C language linkage end -------------------- */
#ifdef __cplusplus
}
#endif

/* ------------------------------ Module end ------------------------------- */
#endif

/* ------------------------------ End of file ------------------------------ */
