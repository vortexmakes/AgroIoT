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
 */

/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */
#include <string.h>
#include <stdio.h>
#include "YFrame.h"
#include "conmgr.h"
#include "BatChr.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
#define YFRAME_ID               "!"
#define YFRAME_MARK             "|"
#define YFRAME_SEPARATOR        ","
#define YFRAME_TERMINATOR       "#"
#define YFRAME_ACK              "!2|"

#define FLG_GPS_VALID           1
#define FLG_HISTORY             2
#define FLG_MOVING              4

#define YFRAME_ACK_LEN          (sizeof(YFRAME_ACK) - 1)

static const char *frameType[] = {"0", "1"};

/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ---------------------------- Global functions --------------------------- */
rInt
YFrame_getFlags(GStatus *from, rui8_t *flags, rInt type)
{
    rInt res;
    rui8_t flag;

    res = 0;
    flag = 0;
    if ((from != (GStatus *)0) && (flags != (rui8_t *)0))
    {
        flag |= (Geo_isValid(&from->position) == 1) ? FLG_GPS_VALID : 0;
        flag |= (type == YFRAME_MGP_TYPE) ? FLG_HISTORY : 0;
        flag |= (cbox_isMoving(&from->devData) == 1) ? FLG_MOVING : 0;
        flag |= (BatChr_getStatus() << 3);
        *flags = flag;
    }
    else
    {
        res = 1;
    }
    return res;
}

ruint
YFrame_header(GStatus *from, char *to, rInt nFrames, rInt type)
{
    ruint size;
    char *frame, temp[12];
    rui8_t flags;

    size = 0;
    frame = to;
    if ((from != (GStatus *)0) && (to != (char *)0))
    {
        strcpy(frame, YFRAME_ID);
        strcat(frame, frameType[type]);
        strcat(frame, YFRAME_MARK);
        if (type == YFRAME_MGP_TYPE)
        {
            sprintf(temp, "%04x", nFrames);
            strcat(frame, temp);
            strcat(frame, YFRAME_MARK);
            strcat(frame, ConMgr_Imei());
        }
        else
        {
            (rInt)YFrame_getFlags(from, &flags, YFRAME_SGP_TYPE);
            sprintf(temp, "%02x", flags);
            strcat(frame, temp);
            strcat(frame, ConMgr_Imei());
            strcat(frame, YFRAME_SEPARATOR);
        }
        size = strlen(frame);
    }
    return size;
}

ruint
YFrame_data(GStatus *from, char *to, rInt type)
{
    ruint size;
    char *frame, temp[12];
    Geo *position;
    IOStatus *io;
    CBOX_STR *dev;
    rui8_t flags;

    size = 0;
    frame = to;
    position = &from->position;
    io = &from->ioStatus;
    dev = &from->devData;

    if ((from != (GStatus *)0) && (to != (char *)0))
    {
        if (type == YFRAME_MGP_TYPE)
        {
            strcpy(frame, YFRAME_MARK);
            (rInt)YFrame_getFlags(from, &flags, YFRAME_MGP_TYPE);
            sprintf(temp, "%02x", flags);
            strcat(frame, temp);
            strcat(frame, YFRAME_SEPARATOR);
        }
        strcat(frame, position->utc);
        strcat(frame, YFRAME_SEPARATOR);
        strcat(frame, position->latInd);
        strcat(frame, position->latitude);
        strcat(frame, YFRAME_SEPARATOR);
        strcat(frame, position->longInd);
        strcat(frame, position->longitude);
        strcat(frame, YFRAME_SEPARATOR);
        strcat(frame, position->speed);
        strcat(frame, YFRAME_SEPARATOR);
        strcat(frame, position->course);
        strcat(frame, YFRAME_SEPARATOR);
        strcat(frame, position->date);
        strcat(frame, YFRAME_SEPARATOR);
        sprintf(temp, "%02X%02X,", io->digOut, io->digIn);
        strcat(frame, temp);
        sprintf(temp, "%04X,", dev->h.hoard);
        strcat(frame, temp);
        sprintf(temp, "%02X,", dev->h.pqty);
        strcat(frame, temp);
        sprintf(temp, "%02X,", dev->hum);
        strcat(frame, temp);
        sprintf(temp, "%04X,", (rui16_t)(dev->a.x));
        strcat(frame, temp);
        sprintf(temp, "%04X,", (rui16_t)(dev->a.y));
        strcat(frame, temp);
        sprintf(temp, "%04X,", (rui16_t)(dev->a.z));
        strcat(frame, temp);
        sprintf(temp, "%d", from->batChrStatus);
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
YFrame_parse(char *from)
{
    ruint size;
    char *str;
    TypeOfResp res;

    res = TypeOfRespUnknown;
    if (from != (char *)0)
    {
        /* Check Ack */
        size = strlen(from);
        if (size >= YFRAME_ACK_LEN)
        {
            str = (from + (size - YFRAME_ACK_LEN));
            if (strcmp(str, YFRAME_ACK) == 0)
            {
                res = TypeOfRespAck;
            }
        }
        else if (size == 0)
        {
            res = TypeOfRespEmpty;
        }

        /* Check Cmd */
        /* 636D643A XX ... XX 3B */
    }
    return res;
}

/* ------------------------------ End of file ------------------------------ */
