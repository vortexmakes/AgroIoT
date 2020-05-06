/**
 *  \file       main.c
 *  \brief      
 */

/* -------------------------- Development history -------------------------- */
/* -------------------------------- Authors -------------------------------- */
/*
 *  LeFr  Leandro Francucci  lf@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */
#include <stdio.h>
#include "GStatus.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
typedef enum ByteOrder ByteOrder;
enum ByteOrder
{
    LittleEndian, BigEndian
};

/* ---------------------------- Local data types --------------------------- */
typedef struct FieldLayout FieldLayout;
struct FieldLayout
{
    int offset;
    int size;
};

typedef struct GeoLayout GeoLayout;
struct GeoLayout
{
    FieldLayout utc;
    FieldLayout status;
    FieldLayout latitude;
    FieldLayout latInd;
    FieldLayout longitude;
    FieldLayout longInd;
    FieldLayout speed;
    FieldLayout course;
    FieldLayout date;
};

typedef struct GStatusTypeLayout GStatusTypeLayout;
struct GStatusTypeLayout
{
    GeoLayout position;
    /*CBOX_STRLayout devData;
    IOStatusLayout ioStatus;
    BatChrStatusLayout batChrStatus;*/
};

typedef struct GStatusLayout GStatusLayout;
struct GStatusLayout
{
    GStatusTypeLayout data;
    FieldLayout checksum;
};

/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
static GStatusLayout layout =
{
    {
        {
            {0, 0},
            {7, 0},
            {9, 0},
            {20, 0},
            {22, 0},
            {34, 0},
            {36, 0},
            {44, 0},
            {48, 0}
        },
    },
    {}
};

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ---------------------------- Global functions --------------------------- */
int
main(int argc, char *argv[])
{
    printf("Hello world!\n");
    return 0;
}

/* ------------------------------ End of file ------------------------------ */
