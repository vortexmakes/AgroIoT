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
    ACLIMIT_DFT,
    BRLIMIT_DFT,
    1,
    IP_DFT,
    PORT_DFT,
    CONNTIME_DFT,
    TOTACTTIME,
    GPSTTIME,
    DIGOUT_DFT,
    SMPTIME_DFT
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
Config_setMappingTime(rui8_t value)
{
    Config *cfg;

    cfg = Config_get();
    cfg->smptime = value;
    Config_set(cfg);
}

rui8_t
Config_getMappingTime(void)
{
    Config *cfg;

    cfg = Config_get();
    return cfg->smptime;
}

void
Config_setDftDigOut(DigOut value)
{
    Config *cfg;

    cfg = Config_get();
    cfg->digOut = value;
    Config_set(cfg);
}

DigOut
Config_getDftDigOut(void)
{
    Config *cfg;

    cfg = Config_get();
    return cfg->digOut;
}

void
Config_setUpdateLocTime(rui8_t value)
{
    Config *cfg;

    cfg = Config_get();
    cfg->gpsttime = value;
    Config_set(cfg);
}

rui8_t
Config_getUpdateLocTime(void)
{
    Config *cfg;

    cfg = Config_get();
    return cfg->gpsttime;
}

void
Config_setConnPeriodTime(rui8_t value)
{
    Config *cfg;

    cfg = Config_get();
    cfg->conntime = value;
    Config_set(cfg);
}

rui8_t
Config_getConnPeriodTime(void)
{
    Config *cfg;

    cfg = Config_get();
    return cfg->conntime;
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
Config_setBrakeLimit(rui8_t value)
{
    Config *cfg;

    cfg = Config_get();
    cfg->brlimit = value;
    Config_set(cfg);
}

rui8_t
Config_getBrakeLimit(void)
{
    Config *cfg;

    cfg = Config_get();
    return cfg->brlimit;
}

/* ------------------------------ End of file ------------------------------ */
