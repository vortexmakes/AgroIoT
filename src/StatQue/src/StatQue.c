/**
 *  \file       StatQue.c
 *  \brief      Implements the status queue's adapter.
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2019.18.02  LeFr  v1.0.00  Initial version
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  LeFr  Leandro Francucci  lf@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */
#include "StatQue.h"
#include "ffile.h"
#include "ffdata.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ---------------------------- Global functions --------------------------- */
rui16_t
StatQue_init(void)
{
    return ffile_queue_open_as_random(FFD0, OPEN_FROM_BEGINNING);
}

rui16_t
StatQue_getNumElem(void)
{
    return (rui16_t)ffile_tell(FFD0);
}

rbool_t
StatQue_isEmpty(void)
{
    return (ffile_tell(FFD0) == (ffui16_t)0);
}

rInt
StatQue_remove(GPS_STR *elem)
{
    rInt res;

    res = ffile_queue_remove(FFD0, elem);
    return (res == FQFILE_OK) ? 0 : 1;
}

rInt
StatQue_delete(rui16_t *nElem)
{
    rInt res;

    res = ffile_queue_delete(FFD0, (NR_T *)nElem);
    return (res == FQFILE_OK) ? 0 : 1;
}

rInt
StatQue_read(GPS_STR *elem)
{
    rInt res;

    res = ffile_queue_random_read(FFD0, READ_FORWARD, elem);
    return (res == FQFILE_OK) ? 0 : 1;
}

rInt
StatQue_put(GPS_STR *elem)
{
    rInt res;

    res = ffile_queue_insert(FFD0, elem);
    return (res == FQFILE_OK) ? 0 : 1;
}

/* ------------------------------ End of file ------------------------------ */
