/**
 *  \file       sprayer.c
 *  \brief      Implementation of sprayer concrete module
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2018.17.10  LeFr  v1.0.00  ---
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  LeFr  Leandro Francucci  lf@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */
#include "unity.h"
#include "device.h"
#include "sprayer.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
typedef struct SprayerJobCond SprayerJobCond;
struct SprayerJobCond
{
    JobCond base;
    int sectionThd;
};

typedef struct Sprayer Sprayer;
struct Sprayer
{
    Device base;
    int maxNumSections;
    int section;
    int dosage;
};

/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
static SprayerJobCond sprayerJobCond;
static Sprayer sprayer;

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
static int 
sprayer_test(JobCond *const me)
{
    return 1;
}

/* ---------------------------- Global functions --------------------------- */
void 
sprayer_ctor(int sectionThd)
{
    Sprayer *me = &sprayer;
    SprayerJobCond *jc;

    device_ctor((Device *)me, SPRAYER, (JobCond *)&sprayerJobCond, 
                sprayer_test);
}

/* ------------------------------ End of file ------------------------------ */
