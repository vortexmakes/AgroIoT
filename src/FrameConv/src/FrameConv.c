/**
 *  \file       FrameConv.c
 *  \brief      Implements a conversor from STM32's GStatus to x86's GStatus
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2020.06.05  LeFr  v1.0.00  Initial version
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  LeFr  Leandro Francucci  lf@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */
#include <endian.h> 
#include <string.h>
#include <stdio.h>
#include "FrameConv.h"
#include "GStatus.h"
#include "YFrame.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
typedef enum ByteOrder ByteOrder;
enum ByteOrder
{
    LittleEndian, BigEndian
};

/* ---------------------------- Local data types --------------------------- */
typedef struct PrimLayout PrimLayout;   /* Layout of primitive type */
struct PrimLayout
{
    int offset;
    int size;
};

typedef struct IOStatusLayout IOStatusLayout;
struct IOStatusLayout
{
    PrimLayout digIn;
    PrimLayout digOut;
};

typedef struct ACCELLayout ACCELLayout;
struct ACCELLayout
{
    PrimLayout x;
    PrimLayout y;
    PrimLayout z;
    PrimLayout m;
};
typedef struct GRSENSLayout GRSENSLayout;
struct GRSENSLayout
{
    PrimLayout hoard;
    PrimLayout pqty;
    PrimLayout flow;
};

typedef struct CBOX_STRLayout CBOX_STRLayout;
struct CBOX_STRLayout
{
    PrimLayout cmd;
    PrimLayout m;
    GRSENSLayout h;
    ACCELLayout a;
    PrimLayout hum;
};

typedef struct GeoLayout GeoLayout;
struct GeoLayout
{
    PrimLayout utc;
    PrimLayout status;
    PrimLayout latitude;
    PrimLayout latInd;
    PrimLayout longitude;
    PrimLayout longInd;
    PrimLayout speed;
    PrimLayout course;
    PrimLayout date;
};

typedef struct GStatusTypeLayout GStatusTypeLayout;
struct GStatusTypeLayout
{
    GeoLayout position;
    CBOX_STRLayout devData;
    IOStatusLayout ioStatus;
    PrimLayout batChrStatus;
};

typedef struct GStatusLayout GStatusLayout;
struct GStatusLayout
{
    GStatusTypeLayout data;
    PrimLayout checksum;
};

/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
static GStatusLayout layout =
{
    {                /* GStatusType::data */
        {            /* GeoLayout::position */
            {0, 7},  /* ::utc */
            {7, 2},  /* ::status */
            {9, 11}, /* ::latitude */
            {20, 2}, /* ::latInd */
            {22, 12},/* ::longitude */
            {34, 2}, /* ::longInd */
            {36, 8}, /* ::speed */
            {44, 4}, /* ::course */
            {48, 7}  /* ::date */
        },
        {            /* CBOX_STRLayout::devData */
            {56, 1}, /* ::cmd */
            {57, 1}, /* ::m */
            {        /* GRSENSLayout::h */
                {58, 2}, /* ::hoard */
                {60, 2}, /* ::pqty */
                {62, 2}  /* ::flow */
            },
            {        /* ACCELLayout::a */
                {64, 2}, /* ::x */
                {66, 2}, /* ::y */
                {68, 2}, /* ::z */
                {70, 1}  /* ::m */
            },
            {72, 1}  /* ::hum */
        },
        {            /* IOStatusLayout::ioStatus */
            {74, 1}, /* ::digIn */
            {75, 1}  /* ::digOut */
        },
        {76, 4}      /* ::batChrStatus */
    },
    {80, 4}          /* ::checksum */
};

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
#if 0
static uint32_t
toh(uint32_t value, ByteOrder from) /* From LE or BE to host order */
{
    uint32_t val;

    if (from == BigEndian)
    {
        val = be32toh(value);
    }
    else
    {
        val = le32toh(value);
    }
    return val;
}
#endif

/* ---------------------------- Global functions --------------------------- */
int
FrameConv_STM32ToX86(GStatus *to, uint8_t from[], size_t size)
{
    int result = 0;
    Geo *toPos;
    GeoLayout *fromPos;
    CBOX_STR *toDev;
    CBOX_STRLayout *fromDev;
    IOStatus *toIo;
    IOStatusLayout *fromIo;

    if ((to != (GStatus *)0) && 
        (from != (uint8_t *)0) && 
        (size >= SIZEOF_GSTATUS_STM32))
    {
        toPos = &to->data.position;
        fromPos = &layout.data.position;
        memcpy(toPos->utc, &from[fromPos->utc.offset], fromPos->utc.size);
        memcpy(toPos->status, &from[fromPos->status.offset], fromPos->status.size);
        memcpy(toPos->latitude, &from[fromPos->latitude.offset], fromPos->latitude.size);
        memcpy(toPos->latInd, &from[fromPos->latInd.offset], fromPos->latInd.size);
        memcpy(toPos->longitude, &from[fromPos->longitude.offset], fromPos->longitude.size);
        memcpy(toPos->longInd, &from[fromPos->longInd.offset], fromPos->longInd.size);
        memcpy(toPos->speed, &from[fromPos->speed.offset], fromPos->speed.size);
        memcpy(toPos->course, &from[fromPos->course.offset], fromPos->course.size);
        memcpy(toPos->date, &from[fromPos->date.offset], fromPos->date.size);

        toDev = &to->data.devData;
        fromDev = &layout.data.devData;
        memcpy(&toDev->cmd, &from[fromDev->cmd.offset], fromDev->cmd.size);
        memcpy(&toDev->m, &from[fromDev->m.offset], fromDev->m.size);
        memcpy(&toDev->h.hoard, &from[fromDev->h.hoard.offset], fromDev->h.hoard.size);
        memcpy(&toDev->h.pqty, &from[fromDev->h.pqty.offset], fromDev->h.pqty.size);
        memcpy(&toDev->h.flow, &from[fromDev->h.flow.offset], fromDev->h.flow.size);
        memcpy(&toDev->a.x, &from[fromDev->a.x.offset], fromDev->a.x.size);
        memcpy(&toDev->a.y, &from[fromDev->a.y.offset], fromDev->a.y.size);
        memcpy(&toDev->a.z, &from[fromDev->a.z.offset], fromDev->a.z.size);
        memcpy(&toDev->a.m, &from[fromDev->a.m.offset], fromDev->a.m.size);
        memcpy(&toDev->hum, &from[fromDev->hum.offset], fromDev->hum.size);

        toIo = &to->data.ioStatus;
        fromIo = &layout.data.ioStatus;
        memcpy(&toIo->digIn, &from[fromIo->digIn.offset], fromIo->digIn.size);
        memcpy(&toIo->digOut, &from[fromIo->digOut.offset], fromIo->digOut.size);

        memcpy(&to->data.batChrStatus, &from[layout.data.batChrStatus.offset], layout.data.batChrStatus.size);
        memcpy(&to->checksum, &from[layout.checksum.offset], layout.checksum.size);
    }
    else
    {
        result = 1;
    }

    return result;
}

int 
FrameConv_GStatusToFrame(uint8_t *to, GStatus *from, size_t *size)
{
    int result = 0;

    if ((to != (uint8_t *)0) && (from != (GStatus *)0) && 
        (size != (size_t *)0))
    {
        *size = YFrame_header(&from->data, to, 0, YFRAME_SGP_TYPE);
        *size += YFrame_data(&from->data, to + *size, YFRAME_SGP_TYPE);
    }
    else
    {
        result = 1;
    }
    return result;
}

/* ------------------------------ End of file ------------------------------ */
