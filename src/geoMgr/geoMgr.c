/**
 *  \file       geoMgr.c
 *  \brief      Geolocalization Context Manager.
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2018.06.05  DaBa  v1.0.00   Initial version
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  DaBa  Dario Baliña db@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "rkh.h"
#include "rkhfwk_pubsub.h"
#include "rkhtmr.h"
#include "bsp.h"
#include "signals.h"
#include "events.h"
#include "topic.h"
#include "geoMgr.h"
#include "rmc.h"
#include "ubx.h"
#include "Geo.h"
#include "Config.h"

/* ----------------------------- Local macros ------------------------------ */
/* ......................... Declares active object ........................ */
typedef struct GeoMgr GeoMgr;

/* ................... Declares states and pseudostates .................... */
RKH_DCLR_BASIC_STATE GeoMgr_Configure, GeoMgr_Failure, GeoMgr_WaitTimeSync,
                     GeoMgr_Active, GeoMgr_Void; 
RKH_DCLR_COMP_STATE  GeoMgr_OnTimeSync; 
RKH_DCLR_CHOICE_STATE GeoMgr_ChoiceInit, GeoMgr_ChoiceTimeSync, 
                      GeoMgr_ChoiceActive;
                    
/* ........................ Declares initial action ........................ */
static void init(GeoMgr *const me, RKH_EVT_T *pe);

/* ........................ Declares effect actions ........................ */
static void configInc(GeoMgr *const me, RKH_EVT_T *pe);
static void startRMCParser(GeoMgr *const me, RKH_EVT_T *pe);
static void restartTimer(GeoMgr *const me, RKH_EVT_T *pe);
static void publishRmc(GeoMgr *const me, RKH_EVT_T *pe);
static void publishInvRmc(GeoMgr *const me, RKH_EVT_T *pe);

/* ......................... Declares entry actions ........................ */
static void configInit(GeoMgr *const me);
static void configSend(GeoMgr *const me);
static void startWaitSync(GeoMgr *const me);
static void ontimeEntry(GeoMgr *const me);
static void turnsDetect(GeoMgr *const me);

/* ......................... Declares exit actions ......................... */
static void waitSyncExit(GeoMgr *const me);

/* ............................ Declares guards ............................ */
rbool_t chkConfigNotEnd(GeoMgr *const me, RKH_EVT_T *pe);
rbool_t checkRMCTime(GeoMgr *const me, RKH_EVT_T *pe);
rbool_t chkRMCActive(GeoMgr *const me, RKH_EVT_T *pe);

/* ........................ States and pseudostates ........................ */
RKH_CREATE_BASIC_STATE(GeoMgr_Configure, configSend, NULL, RKH_ROOT, NULL);
RKH_CREATE_TRANS_TABLE(GeoMgr_Configure)
    RKH_TRREG(evTimeout, NULL, configInc, &GeoMgr_ChoiceInit),
RKH_END_TRANS_TABLE

RKH_CREATE_CHOICE_STATE(GeoMgr_ChoiceInit);
RKH_CREATE_BRANCH_TABLE(GeoMgr_ChoiceInit)
    RKH_BRANCH(chkConfigNotEnd,  NULL,   &GeoMgr_Configure),
    RKH_BRANCH(ELSE,             startRMCParser,   &GeoMgr_WaitTimeSync),
RKH_END_BRANCH_TABLE

RKH_CREATE_BASIC_STATE(GeoMgr_WaitTimeSync, startWaitSync, 
                                            waitSyncExit, RKH_ROOT, NULL);
RKH_CREATE_TRANS_TABLE(GeoMgr_WaitTimeSync)
    RKH_TRREG(evTimeout, NULL, NULL, &GeoMgr_Failure),
    RKH_TRREG(evRMC, NULL, NULL, &GeoMgr_ChoiceTimeSync),
RKH_END_TRANS_TABLE

RKH_CREATE_BASIC_STATE(GeoMgr_Failure, NULL, NULL, RKH_ROOT, NULL);
RKH_CREATE_TRANS_TABLE(GeoMgr_Failure)
RKH_END_TRANS_TABLE

RKH_CREATE_CHOICE_STATE(GeoMgr_ChoiceTimeSync);
RKH_CREATE_BRANCH_TABLE(GeoMgr_ChoiceTimeSync)
    RKH_BRANCH(checkRMCTime,   NULL, &GeoMgr_OnTimeSync),
    RKH_BRANCH(ELSE,           NULL, &GeoMgr_WaitTimeSync),
RKH_END_BRANCH_TABLE

RKH_CREATE_COMP_REGION_STATE(GeoMgr_OnTimeSync, ontimeEntry, NULL,
                             RKH_ROOT, &GeoMgr_ChoiceActive, NULL,
                             RKH_NO_HISTORY, NULL, NULL, NULL, NULL);
RKH_CREATE_TRANS_TABLE(GeoMgr_OnTimeSync)
	RKH_TRREG(evRMC, NULL, restartTimer, &GeoMgr_OnTimeSync),
    RKH_TRREG(evTimeout, NULL, NULL, &GeoMgr_Failure),
RKH_END_TRANS_TABLE


RKH_CREATE_CHOICE_STATE(GeoMgr_ChoiceActive);
RKH_CREATE_BRANCH_TABLE(GeoMgr_ChoiceActive)
    RKH_BRANCH(chkRMCActive, publishRmc,     &GeoMgr_Active),
    RKH_BRANCH(ELSE,         publishInvRmc,  &GeoMgr_Void),
RKH_END_BRANCH_TABLE

RKH_CREATE_BASIC_STATE(GeoMgr_Active, turnsDetect, NULL, &GeoMgr_OnTimeSync, NULL);
RKH_CREATE_TRANS_TABLE(GeoMgr_Active)
RKH_END_TRANS_TABLE

RKH_CREATE_BASIC_STATE(GeoMgr_Void, NULL, NULL, &GeoMgr_OnTimeSync, NULL);
RKH_CREATE_TRANS_TABLE(GeoMgr_Void)
RKH_END_TRANS_TABLE

/* ............................. Active object ............................. */
struct GeoMgr
{
    RKH_SMA_T ao;       /* Base structure */
    RKH_TMR_T timer;    
    UBXCmd_t *pInitCmd;
    Rmc rmc;
    rui8_t count;
    int cog;
};

RKH_SMA_CREATE(GeoMgr, geoMgr, 2, HCAL, &GeoMgr_Configure, init, NULL);
RKH_SMA_DEF_PTR(geoMgr);

/* ------------------------------- Constants ------------------------------- */
#define GPS_ALIVE_TIME  RMC_PERIOD_TIMEOUT
#define GEO_INVALID_GEOSTAMP    \
                { \
                    {GEO_INVALID_UTC}, {RMC_StatusInvalid}, \
                    {GEO_INVALID_LATITUDE}, {GEO_INVALID_LATITUDE_IND}, \
                    {GEO_INVALID_LONGITUDE}, {GEO_INVALID_LONGITUDE_IND}, \
                    {GEO_INVALID_SPEED}, {GEO_INVALID_COURSE}, \
                    {GEO_INVALID_DATE} \
                }

/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
static const unsigned char gsaOff[] = UBX_GSA_OFF;
static const unsigned char vtgOff[] = UBX_VTG_OFF;
static const unsigned char gsvOff[] = UBX_GSV_OFF;
static const unsigned char gllOff[] = UBX_GLL_OFF;
static const unsigned char ggaOff[] = UBX_GGA_OFF;
static const unsigned char rmc2Hz[] = UBX_RMC_2HZ;

static UBXCmd_t InitCmds[] =
{
    { gsaOff, sizeof(gsaOff) },
    { vtgOff, sizeof(vtgOff) },
    { gsvOff, sizeof(gsvOff) },
    { gllOff, sizeof(gllOff) },
    { ggaOff, sizeof(ggaOff) },
//    { rmc2Hz, sizeof(rmc2Hz) },
    { NULL, 0 }
};

static RKH_ROM_STATIC_EVENT(toutEvt, evTimeout);
static RKH_ROM_STATIC_EVENT(turnEvt, evTurn);

static RKHROM GeoEvt geoInvalidEvt =
{
    RKH_INIT_STATIC_EVT( evGeoInvalid ),
    GEO_INVALID_GEOSTAMP
};

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ............................ Initial action ............................. */
static void
init(GeoMgr *const me, RKH_EVT_T *pe)
{
	(void)pe;

    RKH_TR_FWK_AO(me);
    RKH_TR_FWK_TIMER(&me->timer);

    RKH_TR_FWK_QUEUE(&RKH_UPCAST(RKH_SMA_T, me)->equeue);
    RKH_TR_FWK_STATE(me, &GeoMgr_Configure);
    RKH_TR_FWK_STATE(me, &GeoMgr_ChoiceInit);
    RKH_TR_FWK_STATE(me, &GeoMgr_WaitTimeSync);
    RKH_TR_FWK_STATE(me, &GeoMgr_Failure);
    RKH_TR_FWK_STATE(me, &GeoMgr_ChoiceTimeSync);
    RKH_TR_FWK_STATE(me, &GeoMgr_OnTimeSync);
    RKH_TR_FWK_STATE(me, &GeoMgr_ChoiceActive);
    RKH_TR_FWK_STATE(me, &GeoMgr_Active);
    RKH_TR_FWK_STATE(me, &GeoMgr_Void);

    RKH_TMR_INIT(&me->timer, &toutEvt, NULL);

    bsp_serial_open(GPS_PORT);

	configInit(me);

    me->cog = -1;
}

/* ............................ Effect actions ............................. */
static void
configInc(GeoMgr *const me, RKH_EVT_T *pe)
{
	(void)pe;
	
    ++me->pInitCmd;
}

static void
startRMCParser(GeoMgr *const me, RKH_EVT_T *pe)
{
	(void)me;
	(void)pe;
	
    bsp_gpsParserHandler_set((void *)gps_parserInit());
}

static void
restartTimer(GeoMgr *const me, RKH_EVT_T *pe)
{
	(void)me;
	(void)pe;
	
    rkh_tmr_stop(&me->timer);
    RKH_TMR_ONESHOT(&me->timer, RKH_UPCAST(RKH_SMA_T, me), GPS_ALIVE_TIME);
}

void
rmcPos2Degrees( char *dest, char *org, int size, int degOffset )
{
    char *pdeg, *pmin;
    char min[10];
    int val;

    pdeg = strtok(org, ".");
    pmin = strtok(NULL, ".");

    snprintf(min, sizeof(min), "%s%s", 
            pdeg != NULL ? pdeg + degOffset : "",
            pmin != NULL ? pmin: "" );

    val = atol(min);
    val = (val*100)/60;

    *(org + degOffset) = '\0';

    snprintf(dest, size, "%s.%07u", org, val );
}

static void
publishRmc(GeoMgr *const me, RKH_EVT_T *pe)
{
    Rmc *pRmc;
    GeoEvt *geoEvt;
    Geo *pGps;
    char *pchr;

	(void)me;
	(void)pe;
	
    bsp_GPSStatus(RMC_StatusValid);

    pRmc = &(((RmcEvt *)(pe))->rmc);

    me->rmc = *pRmc;

    geoEvt = RKH_ALLOC_EVT(GeoEvt, evGeo, &geoMgr);
    pGps = &(geoEvt->gps);

    memset(pGps, 0, sizeof(Geo));

    /* utc: like NMEA utc, decimals discard */
    strncpy(pGps->utc, pRmc->utc, UTC_LENGTH);

    /* status: like NMEA A=Valid, V=Invalid  */
    *(pGps->status) = RMC_StatusValid;

    /* latitude: convert NMEA DDMM.MMMMM to DD.DDDDDD */
    rmcPos2Degrees(pGps->latitude, pRmc->latitude,
                        LATITUDE_LENGTH, RMC_LATITUDE_DEG_LEN);

    /* Indicator: 'N'->'+' / 'S'->'-' */
    *pGps->latInd = *(pRmc->northingIndicator) == 'N' ? '+' : '-';

    /* longitude: convert NMEA DDDMM.MMMMM to DDD.DDDDDD */
    rmcPos2Degrees(pGps->longitude, pRmc->longitude,
                        LONGITUDE_LENGTH, RMC_LONGITUDE_DEG_LEN);

    /* Indicator: 'E'->'+' / 'W'->'-' */
    *pGps->longInd = *(pRmc->eastingIndicator) == 'E' ? '+' : '-';

    /* speed: like NMEA sog [knots] */
    strncpy(pGps->speed, pRmc->sog, SPEED_LENGTH);

    /* course: like NMEA cog [degrees], decimals discarded */
    pchr = strtok(pRmc->cog, ".");
    strncpy(pGps->course, (pchr != NULL) ? pchr : "", COURSE_LENGTH);

    /* date: like NMEA date [degrees], decimals discarded */
    strncpy(pGps->date, pRmc->date, DATE_LENGTH);

    topic_publish(status, geoEvt, me);
}


static void
publishInvRmc(GeoMgr *const me, RKH_EVT_T *pe)
{
	(void)pe;

    bsp_GPSStatus(RMC_StatusInvalid);

    topic_publish(status, &geoInvalidEvt, me);
}

/* ............................. Entry actions ............................. */
static void
configInit(GeoMgr *const me)
{
    me->pInitCmd = InitCmds;
}

static void
configSend(GeoMgr *const me)
{
	ubx_sendCmd(me->pInitCmd->cmd, me->pInitCmd->size);
	RKH_TMR_ONESHOT(&me->timer, RKH_UPCAST(RKH_SMA_T, me), UBX_INTERCMD_TIME);
}

static void
startWaitSync(GeoMgr *const me)
{
    RKH_TMR_ONESHOT(&me->timer, RKH_UPCAST(RKH_SMA_T, me), GPS_ALIVE_TIME);
}

static void
ontimeEntry(GeoMgr *const me)
{
    RKH_TMR_ONESHOT(&me->timer, RKH_UPCAST(RKH_SMA_T, me), GPS_ALIVE_TIME);
}

static void
turnsDetect(GeoMgr *const me)
{
    int currSog, currCog, diff, cog;

    currSog = atol(me->rmc.sog);
    currCog = atol(me->rmc.cog);

    if(me->cog < 0)
        me->cog = currCog;

    if(!(me->count % ACCELERATION_PERIOD) && 
        (currSog > Config_getAccLimit()))
    {
		diff = abs(currCog - me->cog);
		cog = diff < (360 - diff) ? diff : (360 - diff);
		me->cog = currCog;

		if(cog > Config_getBrakeLimit())
        {
            topic_publish(status, &turnEvt, me);
        }
    }
}

/* ............................. Exit actions .............................. */
static void
waitSyncExit(GeoMgr *const me)
{
    rkh_tmr_stop(&me->timer);
}

/* ................................ Guards ................................. */
rbool_t
chkConfigNotEnd(GeoMgr *const me, RKH_EVT_T *pe)
{
    (void)pe;
    
    return (me->pInitCmd->cmd != NULL) ? RKH_TRUE : RKH_FALSE;
}

rbool_t
checkRMCTime(GeoMgr *const me, RKH_EVT_T *pe)
{
	(void)me;
    
    return (rmc_timeUpdate(&((RmcEvt *)pe)->rmc) >= 0) ? RKH_TRUE : RKH_FALSE;
}

rbool_t
chkRMCActive(GeoMgr *const me, RKH_EVT_T *pe)
{
	(void)me;
    
    return (rmc_status(&((RmcEvt *)pe)->rmc) == RMC_StatusValid) ?
                                            RKH_TRUE : RKH_FALSE;
}

/* ---------------------------- Global functions --------------------------- */
/* ------------------------------ End of file ------------------------------ */
