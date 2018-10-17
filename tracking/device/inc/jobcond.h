/**
 *  \file       jobcond.h
 *  \brief      Specifies the interface of JobCond module
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2018.16.10  LeFr  v1.0.00  Initial version
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  LeFr  Leandro Francucci  lf@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* --------------------------------- Module -------------------------------- */
#ifndef __JOBCOND_H__
#define __JOBCOND_H__

/* ----------------------------- Include files ----------------------------- */
/* ---------------------- External C language linkage ---------------------- */
#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------- Macros -------------------------------- */
/* -------------------------------- Constants ------------------------------ */
/* ------------------------------- Data types ------------------------------ */
typedef struct JobCond JobCond;
typedef int (*TestOper)(JobCond *const me);

struct JobCond
{
    TestOper test;
};

/* -------------------------- External variables --------------------------- */
/* -------------------------- Function prototypes -------------------------- */
void jobcond_ctor(JobCond *const me, TestOper testOper);

/* -------------------- External C language linkage end -------------------- */
#ifdef __cplusplus
}
#endif

/* ------------------------------ Module end ------------------------------- */
#endif

/* ------------------------------ End of file ------------------------------ */
