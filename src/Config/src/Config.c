/**
 *  \file       Config.c
 *  \brief      Implements the configurtion module.
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2018.09.06  DaBa  v1.0.00   Initial version
 *  2019.18.02  LeFr  v1.1.00   Reviewed to test it
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  DaBa  Dario Balina db@vortexmakes.com
 *  LeFr  Leandro Francucci  lf@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */
#include <string.h>
#include "rkhtype.h"
#include "rkhassert.h"
#include "rkhitl.h"     /* It's needed to include platform files */
#include "Config.h"
#include "settings.h"
#include "ffile.h"
#include "ffdata.h"
#include "IOStatus.h"

RKH_MODULE_NAME(Config)

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
enum
{
    IP_PARAM, PORT_PARAM
};

static const Config cfgDft =
{
    ACC_LIMIT_DFT,
    BR_LIMIT_DFT,
    1,
    IP_DFT,
    PORT_DFT,
    MAP_TIME_ON_RUNNING_DFT,
    MAP_TIME_ON_STOPPED_DFT,
    CONN_TIME_DFT,
    DEV_POLL_CYCLE_TIME_DFT,
    MAX_NUM_FRAMES_TO_SEND_DFT,
    UPDATE_GPS_TIME,
    DFT_DIG_OUT_DFT,
    MAX_NUM_STORE_ON_STOPPED_DFT,
    MAX_NUM_STORE_ON_RUNNING_DFT
};

/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
static Config cfgObj;       /* It is a shared resource! */

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
static void
setTCP(int param, char *value)
{
    Config *cfg;
    int size;

    size = (param == IP_PARAM) ? IP_LENGTH : PORT_LENGTH;
    RKH_ENSURE((value != (char *)0) && (strlen(value) <= size));
    cfg = Config_get();
    strcpy((param == IP_PARAM) ? cfg->ip : cfg->port, value);
    Config_set(cfg);
}

/* ---------------------------- Global functions --------------------------- */
Config *
Config_get(void)
{
    Config *cfg;
    RKH_SR_ALLOC();

    RKH_ENTER_CRITICAL_();
    RKH_ENSURE(cfgObj.status == 1);
    cfg = (Config *)&cfgObj;
    RKH_EXIT_CRITICAL_();
    return cfg;
}

Config *
Config_init(void)
{
    int res;
    Config *cfg;

    ffile_seek(FFD1, 0);
    res = ffile_random_access(FFD1, READ_ACCESS, &cfgObj, 1);
    if (res > 0)
    {
        if (cfgObj.status != 1)
        {
            cfg = (Config *)&cfgDft;
            Config_set(cfg);
        }
        else
        {
            cfg = &cfgObj;
        }
    }
    else
    {
        cfg = (Config *)&cfgDft;
        Config_set(cfg);
    }
    return cfg;
}

void
Config_set(Config *cfg)
{
    int res;
    RKH_SR_ALLOC();

    RKH_REQUIRE(cfg != (Config *)0);
    RKH_ENTER_CRITICAL_();
    ffile_seek(FFD1, 0);
    res = ffile_random_access(FFD1, WRITE_ACCESS, cfg, 1);
    RKH_ENSURE(res > 0);
    ffile_sync();
    cfgObj = *cfg;
    RKH_EXIT_CRITICAL_();
}

void
Config_setAccLimit(rui8_t value)
{
    Config *cfg;

    cfg = Config_get();
    cfg->accLimit = value;
    Config_set(cfg);
}

rui8_t
Config_getAccLimit(void)
{
    Config *cfg;

    cfg = Config_get();
    return cfg->accLimit;
}

void
Config_setBrLimit(rui8_t value)
{
    Config *cfg;

    cfg = Config_get();
    cfg->brLimit = value;
    Config_set(cfg);
}

rui8_t
Config_getBrLimit(void)
{
    Config *cfg;

    cfg = Config_get();
    return cfg->brLimit;
}

void
Config_setDefault(rui8_t value)
{
    Config *cfg;

    cfg = Config_get();
    cfg->status = value;
    Config_set(cfg);
}

rui8_t
Config_getDefault(void)
{
    Config *cfg;

    cfg = Config_get();
    return cfg->status;
}

void
Config_setIP(char *value)
{
    setTCP(IP_PARAM, value);
}

void
Config_getIP(char *value)
{
    Config *cfg;

    RKH_ENSURE(value != (char *)0);
    cfg = Config_get();
    strcpy(value, cfg->ip);
}

void
Config_setPort(char *value)
{
    setTCP(PORT_PARAM, value);
}

void
Config_getPort(char *value)
{
    Config *cfg;

    RKH_ENSURE(value != (char *)0);
    cfg = Config_get();
    strcpy(value, cfg->port);
}

void
Config_setMapTimeOnRunning(rui8_t value)
{
    Config *cfg;

    cfg = Config_get();
    cfg->mapTimeOnRunning = value;
    Config_set(cfg);
}

rui8_t
Config_getMapTimeOnRunning(void)
{
    Config *cfg;

    cfg = Config_get();
    return cfg->mapTimeOnRunning;
}

void
Config_setMapTimeOnStopped(rui8_t value)
{
    Config *cfg;

    cfg = Config_get();
    cfg->mapTimeOnStopped = value;
    Config_set(cfg);
}

rui8_t
Config_getMapTimeOnStopped(void)
{
    Config *cfg;

    cfg = Config_get();
    return cfg->mapTimeOnStopped;
}

void
Config_setConnTime(rui8_t value)
{
    Config *cfg;

    cfg = Config_get();
    cfg->connTime = value;
    Config_set(cfg);
}

rui8_t
Config_getConnTime(void)
{
    Config *cfg;

    cfg = Config_get();
    return cfg->connTime;
}

void
Config_setDevPollCycleTime(rui8_t value)
{
    Config *cfg;

    cfg = Config_get();
    cfg->devPollCycleTime = value;
    Config_set(cfg);
}

rui8_t
Config_getDevPollCycleTime(void)
{
    Config *cfg;

    cfg = Config_get();
    return cfg->devPollCycleTime;
}

void
Config_setMaxNumFramesToSend(rui8_t value)
{
    Config *cfg;

    cfg = Config_get();
    cfg->maxNumFramesToSend = value;
    Config_set(cfg);
}

rui8_t
Config_getMaxNumFramesToSend(void)
{
    Config *cfg;

    cfg = Config_get();
    return cfg->maxNumFramesToSend;
}

void
Config_setUpdateGPSTime(rui8_t value)
{
    Config *cfg;

    cfg = Config_get();
    cfg->updateGPSTime = value;
    Config_set(cfg);
}

rui8_t
Config_getUpdateGPSTime(void)
{
    Config *cfg;

    cfg = Config_get();
    return cfg->updateGPSTime;
}

void
Config_setDftDigOut(DigOut value)
{
    Config *cfg;

    cfg = Config_get();
    cfg->dftDigOut = value;
    Config_set(cfg);
}

DigOut
Config_getDftDigOut(void)
{
    Config *cfg;

    cfg = Config_get();
    return cfg->dftDigOut;
}

void
Config_setMaxNumStoreOnStopped(DigOut value)
{
    Config *cfg;

    cfg = Config_get();
    cfg->maxNumStoreOnStopped = value;
    Config_set(cfg);
}

rui8_t
Config_getMaxNumStoreOnStopped(void)
{
    Config *cfg;

    cfg = Config_get();
    return cfg->maxNumStoreOnStopped;
}

void
Config_setMaxNumStoreOnRunning(rui8_t value)
{
    Config *cfg;

    cfg = Config_get();
    cfg->maxNumStoreOnRunning = value;
    Config_set(cfg);
}

rui8_t
Config_getMaxNumStoreOnRunning(void)
{
    Config *cfg;

    cfg = Config_get();
    return cfg->maxNumStoreOnRunning;
}

/* ------------------------------ End of file ------------------------------ */
