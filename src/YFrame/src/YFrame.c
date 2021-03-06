/**
 *  \file       YFrame.c
 *  \brief      Implements Yipies frame (remote protocol) module
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2018.05.02  LeFr  v1.0.00  Initial version
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  LeFr  Leandro Francucci  lf@vortexmakes.com
 *  DaBa  Darío Baliña       db@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */
#include <string.h>
#include <stdio.h>
#include "YFrame.h"
#include "GsmMgr.h"
#include "BatChr.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
#define YFRAME_ID               "!"
#define YFRAME_MARK             "|"
#define YFRAME_SEPARATOR        ","
#define YFRAME_TERMINATOR       "#"
#define YFRAME_ACK              "!2|"
#define YFRAME_CACK             "!4|"

#define FLG_GPS_VALID           1
#define FLG_HISTORY             2
#define FLG_MOVING              4
#define FLG_LINE	            8

#define YFRAME_ACK_LEN          (sizeof(YFRAME_ACK) - 1)

static const char *frameType[] = {"0", "1"};
static const char empty[] = "";

/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ---------------------------- Global functions --------------------------- */
rInt
YFrame_getFlags(GStatusType *from, rui8_t *flags, rInt type)
{
    rInt res;
    rui8_t flag;

    res = 0;
    flag = 0;
    if ((from != (GStatusType *)0) && (flags != (rui8_t *)0))
    {
        flag |= (Geo_isValid(&from->position) == 1) ? FLG_GPS_VALID : 0;
        flag |= (type == YFRAME_MGP_TYPE) ? FLG_HISTORY : 0;
        flag |= (cbox_isMoving(&from->devData) == 1) ? FLG_MOVING : 0;
        flag |= (from->batChrStatus == LINE_BATT) ? FLG_LINE : 0;
        *flags = flag;
    }
    else
    {
        res = 1;
    }
    return res;
}

ruint
YFrame_header(GStatusType *from, char *to, rInt nFrames, rInt type)
{
    ruint size;
    char *frame, temp[12];
    rui8_t flags;

    size = 0;
    frame = to;
    if ((from != (GStatusType *)0) && (to != (char *)0))
    {
        strcpy(frame, YFRAME_ID);
        strcat(frame, frameType[type]);
        strcat(frame, YFRAME_MARK);
        if (type == YFRAME_MGP_TYPE)
        {
            sprintf(temp, "%04x", nFrames);
            strcat(frame, temp);
            strcat(frame, YFRAME_MARK);
            strcat(frame, GsmMgr_getImei());
        }
        else
        {
            (rInt)YFrame_getFlags(from, &flags, YFRAME_SGP_TYPE);
            sprintf(temp, "%02x", flags);
            strcat(frame, temp);
            strcat(frame, GsmMgr_getImei());
            strcat(frame, YFRAME_SEPARATOR);
        }
        size = strlen(frame);
    }
    return size;
}

ruint
YFrame_data(GStatusType *from, char *to, rInt type)
{
    ruint size;
    char *frame, temp[12];
    Geo *position;
    IOStatus *io;
    CBOX_STR *dev;
    rui8_t flags;
    char *value;

    size = 0;
    frame = to;
    position = &from->position;
    io = &from->ioStatus;
    dev = &from->devData;


    if ((from != (GStatusType *)0) && (to != (char *)0))
    {
        if (type == YFRAME_MGP_TYPE)
        {
            strcpy(frame, YFRAME_MARK);
            (rInt)YFrame_getFlags(from, &flags, YFRAME_MGP_TYPE);
            sprintf(temp, "%02x", flags);
            strcat(frame, temp);
            strcat(frame, YFRAME_SEPARATOR);
        }
        value = Geo_getUtc(position);
        strcat(frame, (value == NULL) ? empty : value);
        strcat(frame, YFRAME_SEPARATOR);
        value = Geo_getLatInd(position);
        strcat(frame, (value == NULL) ? empty : value);
        value = Geo_getLatitude(position);
        strcat(frame, (value == NULL) ? empty : value);
        strcat(frame, YFRAME_SEPARATOR);
        value = Geo_getLongInd(position);
        strcat(frame, (value == NULL) ? empty : value);
        value = Geo_getLongitude(position);
        strcat(frame, (value == NULL) ? empty : value);
        strcat(frame, YFRAME_SEPARATOR);
        value = Geo_getSpeed(position);
        strcat(frame, (value == NULL) ? empty : value);
        strcat(frame, YFRAME_SEPARATOR);
        value = Geo_getCourse(position);
        strcat(frame, (value == NULL) ? empty : value);
        strcat(frame, YFRAME_SEPARATOR);
        value = Geo_getDate(position);
        strcat(frame, (value == NULL) ? empty : value);
        strcat(frame, YFRAME_SEPARATOR);
        sprintf(temp, "%02X%02X,", io->digOut, io->digIn);
        strcat(frame, temp);
        sprintf(temp, "%04X,", dev->h.hoard);
        strcat(frame, temp);
        sprintf(temp, "%02X,", (uint8_t)dev->h.pqty);
        strcat(frame, temp);
        sprintf(temp, "%02X,", dev->hum);
        strcat(frame, temp);
        sprintf(temp, "%04X,", (rui16_t)(dev->a.x));
        strcat(frame, temp);
        sprintf(temp, "%04X,", (rui16_t)(dev->a.y));
        strcat(frame, temp);
        sprintf(temp, "%04X,", (rui16_t)(dev->a.z));
        strcat(frame, temp);
        sprintf(temp, "%01X", (uint8_t)from->batChrStatus);
        strcat(frame, temp);
        size = strlen(frame);
    }
    return size;
}

ruint
YFrame_multipleTail(char *to)
{
    ruint size;
    char *frame;

    size = 0;
    frame = to;
    if (to != (char *)0)
    {
        strcpy(frame, YFRAME_TERMINATOR);
        size = strlen(frame);
    }
    return size;
}

TypeOfResp
YFrame_parse(char *from, ruint size, YCommand *cmd)
{
    TypeOfResp res;
    YCmdRes yCmdRes;

    yCmdRes = YCommand_parseAndExec(cmd, from, size);
    if (yCmdRes == YCmdOk)
    {
        res = TypeOfRespCmd;
    }
    else if (yCmdRes == YAck)
    {
        res = TypeOfRespAck;
    }
    else
    {
        res = TypeOfRespUnknown;
    }
    return res;
}

ruint 
YFrame_getCmdAck(YCommand *from, char *to)
{
    ruint size;
    char *frame;

    size = 0;
    frame = to;
    if ((from != (YCommand *)0) && (to != (char *)0))
    {
        strcpy(frame, YFRAME_CACK);
        strcat(frame, GsmMgr_getImei());
        strcat(frame, YFRAME_SEPARATOR);
        strcat(frame, from->index);
        strcat(frame, YFRAME_SEPARATOR);
        size = strlen(frame);
    }
    return size;
}

/* ------------------------------ End of file ------------------------------ */
