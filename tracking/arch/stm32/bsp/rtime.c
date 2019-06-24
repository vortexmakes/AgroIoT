/**
 *  \file       rtime.c
 *  \brief      Implementation of RTC abstraction for STM32 bsp.
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2018.05.17  DaBa  v1.0.00  Initial version
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  DaBa  Dario Baliña       db@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */
#include "rtime.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
static Time t;

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ---------------------------- Global functions --------------------------- */
void
rtime_init(void)
{
    /* 
     * TODO: initialize RTC
     *
     */
}

Time *
rtime_get(void)
{
    /* 
     * TODO: Read RTC
     *
     * Ex:
        rtcRead(&rtc);

        t.tm_sec = rtc.sec;
        t.tm_min = rtc.min;
        t.tm_hour = rtc.hour;
        t.tm_mday = rtc.mday;
        t.tm_mon = rtc.month;
        t.tm_year = rtc.year;
        t.tm_wday = rtc.wday;
        t.tm_isdst = 0;
     */
    return &t;
}

void
rtime_set(Time *pt)
{
    /*
     * TODO: Write RTC
     *
     * Ex:
     *
     *    rtc.sec = pt->tm_sec;
   	   	  rtc.min = pt->tm_min;
   	   	  rtc.hour = pt->tm_hour;
          rtc.wday = pt->tm_wday;
          rtc.mday = pt->tm_mday;
          rtc.month = pt->tm_mon;
          rtc.year = pt->tm_year;

          rtcWrite(&rtc);
     */

}

/* ------------------------------ End of file ------------------------------ */
