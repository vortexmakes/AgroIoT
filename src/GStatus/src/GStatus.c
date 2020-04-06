/**
 *  \file       GStatus.c
 *  \brief      Implements general status module
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2018.08.02  LeFr  v1.0.00  Initial version
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  LeFr  Leandro Francucci  lf@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */
#include "GStatus.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ---------------------------- Global functions --------------------------- */
void 
GStatus_setChecksum(GStatus *const me)
{
    if (me != (GStatus *)0)
    {
        Crc32_init();
        me->checksum = Crc32_calc((const uint8_t *)&me->data, 
                                  sizeof(GStatusType), 
                                  0xffffffff);
    }
}

bool 
GStatus_checkValidity(GStatus *const me)
{
    Crc32 checksum;
    bool result = 0;

    if (me != (GStatus *)0)
    {
        Crc32_init();
        checksum = Crc32_calc((const uint8_t *)&me->data, 
                              sizeof(GStatusType), 
                              0xffffffff);
        if (checksum == me->checksum)
        {
            result = true;
        }
    }
    return result;
}

/* ------------------------------ End of file ------------------------------ */
