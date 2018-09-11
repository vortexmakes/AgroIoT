/**
 *  \file rmc.c
 *
 *	This module contains functions for manipulating RMC NMEA Frame,
 *	including functions for determining what the current time is convert
 *	geolocalization and check it´s reliability.
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2018.09.06  DaBa  v1.0.00   Initial version
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  DaBa  Dario Baliña db@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */
#include <string.h>

#include "rmc.h"
#include "epoch.h"

/* ----------------------------- Local macros ------------------------------ */
#define _2DIGIT_ATOI(b,o)   (((b)[o]-'0')*10 + ((b)[o+1] - '0'))

#define RMC_DATE_DAY_POS    0
#define RMC_DATE_MONTH_POS  2
#define RMC_DATE_YEAR_POS   4

#define RMC_DAY(date)       _2DIGIT_ATOI(date,RMC_DATE_DAY_POS)
#define RMC_MONTH(date)     _2DIGIT_ATOI(date,RMC_DATE_MONTH_POS)
#define RMC_YEAR(date)      (_2DIGIT_ATOI(date,RMC_DATE_YEAR_POS) + 2000)

#define RMC_UTC_HOUR_POS    0
#define RMC_UTC_MIN_POS     2
#define RMC_UTC_SEC_POS     4

#define RMC_HOUR(utc)       _2DIGIT_ATOI(utc,RMC_UTC_HOUR_POS)
#define RMC_MIN(utc)        _2DIGIT_ATOI(utc,RMC_UTC_MIN_POS)
#define RMC_SEC(utc)        _2DIGIT_ATOI(utc,RMC_UTC_SEC_POS)

/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
static Time rmcTime;

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ---------------------------- Global functions --------------------------- */
char
rmc_status(RMC_t *p)
{
	return (char)(p->status[0]);
}

int
rmc_timeUpdate(RMC_t *p)
{
	char *putc, *pdate;

	putc = p->utc;
	pdate = p->date;

	if (strlen(putc) == 0 || strlen(pdate) == 0)
	{
		memset(&rmcTime, 0, sizeof(rmcTime));
		return -1;
	}

    rmcTime.tm_year = RMC_YEAR(pdate);
    rmcTime.tm_mon = RMC_MONTH(pdate);
    rmcTime.tm_mday = RMC_DAY(pdate);
    rmcTime.tm_hour = RMC_HOUR(putc);
    rmcTime.tm_min = RMC_MIN(putc);
    rmcTime.tm_sec = RMC_SEC(putc);

	if ((rmcTime.tm_year < 1970) ||
		(rmcTime.tm_mon < 1) || (rmcTime.tm_mon > 12) ||
		(rmcTime.tm_mday < 1) || (rmcTime.tm_mday > 31) ||
		(rmcTime.tm_hour < 0) || (rmcTime.tm_hour > 23) ||
		(rmcTime.tm_min < 0) || (rmcTime.tm_min > 59) ||
		(rmcTime.tm_sec < 0) || (rmcTime.tm_sec > 59)
		)
		return -1;

	return 0;
}
/* ------------------------------ End of file ------------------------------ */
