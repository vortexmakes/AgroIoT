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
Device * 
sprayer_ctor(int sectionThd)
{
    Sprayer *me = &sprayer;

    device_ctor((Device *)me, SPRAYER, (JobCond *)&sprayerJobCond, 
                sprayer_test);
    sprayerJobCond.sectionThd = sectionThd;
    return (Device *)&sprayer;
}

int 
sprayerSpy_getMaxNumSections(void)
{
    return sprayer.maxNumSections;
}

int 
sprayerSpy_getSection(void)
{
    return sprayer.section;
}

int 
sprayerSpy_getDosage(void)
{
    return sprayer.dosage;
}

int 
sprayerSpy_getSectionThd(void)
{
    return sprayerJobCond.sectionThd;
}

/* ------------------------------ End of file ------------------------------ */
