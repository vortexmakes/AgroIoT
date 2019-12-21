/**
 *  \file       test_StatQue.c
 *  \brief      Unit test for status queue's adapter.
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
#include <string.h>
#include "unity.h"
#include "Config.h"
#include "settings.h"
#include "Mock_ffile.h"
#include "Mock_ffdata.h"
#include "Mock_rkhassert.h"
#include "Mock_rkhport.h"
#include "Mock_IOStatus.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
static const Config cfgDft =
{
    ACC_LIMIT_DFT,
    BR_LIMIT_DFT,
    1,
    CONNECTION_DOMAIN_DFT,
    CONNECTION_PORT_DFT,
    MAP_TIME_ON_RUNNING_DFT,
    MAP_TIME_ON_STOPPED_DFT,
    CONN_TIME_DFT,
    DEV_POLL_CYCLE_TIME_DFT,
    MAX_NUM_FRAMES_TO_SEND_DFT,
    UPDATE_GPS_TIME,
    DFT_DIG_OUT_DFT,
    MAX_NUM_STORE_ON_STOPPED_DFT,
    MAX_NUM_STORE_ON_RUNNING_DFT,
    SIM_PIN_NUMBER_DFT,
    MAX_NUM_CONNNORESP_RETRIES_DFT,
    CONNECTION_STATUS_PERIOD_DFT,
    REOPEN_DELAY_DFT,
    CONNECT_TRY_DELAY_DFT,
    MAX_NUM_CONNECT_RETRIES_DFT,
    CONFIG_TRY_DELAY_DFT
};

/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
static Config cfgFile =
{
    ACC_LIMIT_DFT,
    BR_LIMIT_DFT,
    1,
    "\"11.2.333.44\"",
    "11223",
    3,
    60,
    60,
    2,
    100,
    2,
    0,
    240,
    100,
    9474,
    2,
    2,
    5,
    5,
    3,
    3
};

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
static void
MockAssertCallback(const char* const file, int line, int cmock_num_calls)
{
    TEST_PASS();
}

/* ---------------------------- Global functions --------------------------- */
void
setUp(void)
{
}

void
tearDown(void)
{
}

void
test_InitWithoutStoredSettings(void)
{
    Config *cfg, cfgErased;

    cfg = (Config *)0;
    cfgErased.status = 0xff;

    ffile_seek_Expect(FFD1, 0);
    ffile_random_access_ExpectAndReturn(FFD1, READ_ACCESS, 0, 1, 1);
    ffile_random_access_IgnoreArg_preg();
    ffile_random_access_ReturnThruPtr_preg(&cfgErased);
    rkh_enter_critical_Expect();
    ffile_seek_Expect(FFD1, 0);
    ffile_random_access_ExpectAndReturn(FFD1, WRITE_ACCESS, 0, 1, 1);
    ffile_random_access_IgnoreArg_preg();
    ffile_sync_Expect();
    rkh_exit_critical_Expect();

    cfg = Config_init();

    TEST_ASSERT_NOT_NULL(cfg);
    TEST_ASSERT_EQUAL(cfgDft.accLimit, cfg->accLimit);
    TEST_ASSERT_EQUAL(cfgDft.brLimit, cfg->brLimit);
    TEST_ASSERT_EQUAL(cfgDft.status, cfg->status);
    TEST_ASSERT_EQUAL_STRING(cfgDft.connectionDomain, cfg->connectionDomain);
    TEST_ASSERT_EQUAL_STRING(cfgDft.connectionPort, cfg->connectionPort);
    TEST_ASSERT_EQUAL(cfgDft.mapTimeOnRunning, cfg->mapTimeOnRunning);
    TEST_ASSERT_EQUAL(cfgDft.mapTimeOnStopped, cfg->mapTimeOnStopped);
    TEST_ASSERT_EQUAL(cfgDft.mapTimeOnStopped, cfg->mapTimeOnStopped);
    TEST_ASSERT_EQUAL(cfgDft.devPollCycleTime, cfg->devPollCycleTime);
    TEST_ASSERT_EQUAL(cfgDft.maxNumFramesToSend, cfg->maxNumFramesToSend);
    TEST_ASSERT_EQUAL(cfgDft.updateGPSTime, cfg->updateGPSTime);
    TEST_ASSERT_EQUAL(cfgDft.dftDigOut, cfg->dftDigOut);
    TEST_ASSERT_EQUAL(cfgDft.maxNumStoreOnStopped, cfg->maxNumStoreOnStopped);
    TEST_ASSERT_EQUAL(cfgDft.maxNumStoreOnRunning, cfg->maxNumStoreOnRunning);
    TEST_ASSERT_EQUAL(cfgDft.SIMPinNumber, cfg->SIMPinNumber);
    TEST_ASSERT_EQUAL(cfgDft.maxNumConnNoRespRetries, 
                      cfg->maxNumConnNoRespRetries);
    TEST_ASSERT_EQUAL(cfgDft.connectionStatusPeriod, 
                      cfg->connectionStatusPeriod);
    TEST_ASSERT_EQUAL(cfgDft.reopenDelay, cfg->reopenDelay);
    TEST_ASSERT_EQUAL(cfgDft.connectTryDelay, cfg->connectTryDelay);
    TEST_ASSERT_EQUAL(cfgDft.maxNumConnectRetries, cfg->maxNumConnectRetries);
    TEST_ASSERT_EQUAL(cfgDft.configTryDelay, cfg->configTryDelay);
}

void
test_InitWithStoredSettings(void)
{
    Config *cfg;

    cfg = (Config *)0;
    cfgFile.status = 1;

    ffile_seek_Expect(FFD1, 0);
    ffile_random_access_ExpectAndReturn(FFD1, READ_ACCESS, 0, 1, 1);
    ffile_random_access_IgnoreArg_preg();
    ffile_random_access_ReturnThruPtr_preg(&cfgFile);

    cfg = Config_init();

    TEST_ASSERT_NOT_NULL(cfg);
    TEST_ASSERT_EQUAL(cfgFile.accLimit, cfg->accLimit);
    TEST_ASSERT_EQUAL(cfgFile.brLimit, cfg->brLimit);
    TEST_ASSERT_EQUAL(cfgFile.status, cfg->status);
    TEST_ASSERT_EQUAL_STRING(cfgFile.connectionDomain, cfg->connectionDomain);
    TEST_ASSERT_EQUAL_STRING(cfgFile.connectionPort, cfg->connectionPort);
    TEST_ASSERT_EQUAL(cfgFile.mapTimeOnRunning, cfg->mapTimeOnRunning);
    TEST_ASSERT_EQUAL(cfgFile.mapTimeOnStopped, cfg->mapTimeOnStopped);
    TEST_ASSERT_EQUAL(cfgFile.mapTimeOnStopped, cfg->mapTimeOnStopped);
    TEST_ASSERT_EQUAL(cfgFile.devPollCycleTime, cfg->devPollCycleTime);
    TEST_ASSERT_EQUAL(cfgFile.maxNumFramesToSend, cfg->maxNumFramesToSend);
    TEST_ASSERT_EQUAL(cfgFile.updateGPSTime, cfg->updateGPSTime);
    TEST_ASSERT_EQUAL(cfgFile.dftDigOut, cfg->dftDigOut);
    TEST_ASSERT_EQUAL(cfgFile.maxNumStoreOnStopped, cfg->maxNumStoreOnStopped);
    TEST_ASSERT_EQUAL(cfgFile.maxNumStoreOnRunning, cfg->maxNumStoreOnRunning);
    TEST_ASSERT_EQUAL(cfgFile.SIMPinNumber, cfg->SIMPinNumber);
    TEST_ASSERT_EQUAL(cfgFile.maxNumConnNoRespRetries, 
                      cfg->maxNumConnNoRespRetries);
    TEST_ASSERT_EQUAL(cfgFile.connectionStatusPeriod, 
                      cfg->connectionStatusPeriod);
    TEST_ASSERT_EQUAL(cfgFile.reopenDelay, cfg->reopenDelay);
    TEST_ASSERT_EQUAL(cfgFile.connectTryDelay, cfg->connectTryDelay);
    TEST_ASSERT_EQUAL(cfgFile.maxNumConnectRetries, cfg->maxNumConnectRetries);
    TEST_ASSERT_EQUAL(cfgFile.configTryDelay, cfg->configTryDelay);
}

void
test_InitWithFileError(void)
{
    Config *cfg;

    cfg = (Config *)0;

    ffile_seek_Expect(FFD1, 0);
    ffile_random_access_ExpectAndReturn(FFD1, READ_ACCESS, 0, 1, 0);
    ffile_random_access_IgnoreArg_preg();
    rkh_enter_critical_Expect();
    ffile_seek_Expect(FFD1, 0);
    ffile_random_access_ExpectAndReturn(FFD1, WRITE_ACCESS, 0, 1, 1);
    ffile_random_access_IgnoreArg_preg();
    ffile_sync_Expect();
    rkh_exit_critical_Expect();

    cfg = Config_init();

    TEST_ASSERT_NOT_NULL(cfg);
    TEST_ASSERT_EQUAL(cfgDft.accLimit, cfg->accLimit);
    TEST_ASSERT_EQUAL(cfgDft.brLimit, cfg->brLimit);
    TEST_ASSERT_EQUAL(cfgDft.status, cfg->status);
    TEST_ASSERT_EQUAL_STRING(cfgDft.connectionDomain, cfg->connectionDomain);
    TEST_ASSERT_EQUAL_STRING(cfgDft.connectionPort, cfg->connectionPort);
    TEST_ASSERT_EQUAL(cfgDft.mapTimeOnRunning, cfg->mapTimeOnRunning);
    TEST_ASSERT_EQUAL(cfgDft.mapTimeOnStopped, cfg->mapTimeOnStopped);
    TEST_ASSERT_EQUAL(cfgDft.mapTimeOnStopped, cfg->mapTimeOnStopped);
    TEST_ASSERT_EQUAL(cfgDft.devPollCycleTime, cfg->devPollCycleTime);
    TEST_ASSERT_EQUAL(cfgDft.maxNumFramesToSend, cfg->maxNumFramesToSend);
    TEST_ASSERT_EQUAL(cfgDft.updateGPSTime, cfg->updateGPSTime);
    TEST_ASSERT_EQUAL(cfgDft.dftDigOut, cfg->dftDigOut);
    TEST_ASSERT_EQUAL(cfgDft.maxNumStoreOnStopped, cfg->maxNumStoreOnStopped);
    TEST_ASSERT_EQUAL(cfgDft.maxNumStoreOnRunning, cfg->maxNumStoreOnRunning);
    TEST_ASSERT_EQUAL(cfgDft.SIMPinNumber, cfg->SIMPinNumber);
    TEST_ASSERT_EQUAL(cfgDft.maxNumConnNoRespRetries, 
                      cfg->maxNumConnNoRespRetries);
    TEST_ASSERT_EQUAL(cfgDft.connectionStatusPeriod, 
                      cfg->connectionStatusPeriod);
    TEST_ASSERT_EQUAL(cfgDft.reopenDelay, cfg->reopenDelay);
    TEST_ASSERT_EQUAL(cfgDft.connectTryDelay, cfg->connectTryDelay);
    TEST_ASSERT_EQUAL(cfgDft.maxNumConnectRetries, cfg->maxNumConnectRetries);
    TEST_ASSERT_EQUAL(cfgDft.configTryDelay, cfg->configTryDelay);
}

void
test_Get(void)
{
    Config *cfg, cfgErased;

    cfg = (Config *)0;
    cfgErased.status = 0xff;
    ffile_seek_Expect(FFD1, 0);
    ffile_random_access_ExpectAndReturn(FFD1, READ_ACCESS, 0, 1, 1);
    ffile_random_access_IgnoreArg_preg();
    ffile_random_access_ReturnThruPtr_preg(&cfgErased);
    rkh_enter_critical_Expect();
    ffile_seek_Expect(FFD1, 0);
    ffile_random_access_ExpectAndReturn(FFD1, WRITE_ACCESS, 0, 1, 1);
    ffile_random_access_IgnoreArg_preg();
    ffile_sync_Expect();
    rkh_exit_critical_Expect();

    cfg = Config_init();

    TEST_ASSERT_NOT_NULL(cfg);
    TEST_ASSERT_EQUAL(cfgDft.status, cfg->status);

    cfg = (Config *)0;
    rkh_enter_critical_Expect();
    rkh_exit_critical_Expect();

    cfg = Config_get();

    TEST_ASSERT_NOT_NULL(cfg);
    TEST_ASSERT_EQUAL(cfgDft.status, cfg->status);
}

void
test_GetBeforeInitFile(void)
{
    Config cfgErased;

    cfgErased.status = 0xff;
    rkh_enter_critical_Expect();
    ffile_seek_Expect(FFD1, 0);
    ffile_random_access_ExpectAndReturn(FFD1, WRITE_ACCESS, &cfgErased, 1, 1);
    ffile_sync_Expect();
    rkh_exit_critical_Expect();

    Config_set(&cfgErased);

    rkh_enter_critical_Expect();
    rkh_assert_Expect("Config", 0);
    rkh_assert_IgnoreArg_file();
    rkh_assert_IgnoreArg_line();
    rkh_assert_StubWithCallback(MockAssertCallback);

    Config_get();
}

void
test_Set(void)
{
    rkh_enter_critical_Expect();
    ffile_seek_Expect(FFD1, 0);
    ffile_random_access_ExpectAndReturn(FFD1, WRITE_ACCESS, &cfgFile, 1, 1);
    ffile_sync_Expect();
    rkh_exit_critical_Expect();

    Config_set(&cfgFile);
}

void
test_SetFailsWrongArg(void)
{
    rkh_assert_Expect("Config", 0);
    rkh_assert_IgnoreArg_file();
    rkh_assert_IgnoreArg_line();
    rkh_assert_StubWithCallback(MockAssertCallback);

    Config_set((Config *)0);
}

void
test_SetFailsWrongFileAccess(void)
{
    rkh_enter_critical_Expect();
    ffile_seek_Expect(FFD1, 0);
    ffile_random_access_ExpectAndReturn(FFD1, WRITE_ACCESS, &cfgFile, 1, 0);
    rkh_assert_Expect("Config", 0);
    rkh_assert_IgnoreArg_file();
    rkh_assert_IgnoreArg_line();
    rkh_assert_StubWithCallback(MockAssertCallback);

    Config_set(&cfgFile);
}

void
test_SetGetAccLimit(void)
{
    rui8_t value;
    Config *cfg;

    rkh_enter_critical_Ignore();
    rkh_exit_critical_Ignore();
    rkh_enter_critical_Ignore();
    ffile_seek_Ignore();
    ffile_random_access_IgnoreAndReturn(1);
    ffile_sync_Ignore();
    rkh_exit_critical_Ignore();

    value = 4;
    Config_setAccLimit(value);
    cfg = Config_get();
    TEST_ASSERT_EQUAL(value, cfg->accLimit);

    value = 0;
    value = Config_getAccLimit();
    TEST_ASSERT_EQUAL(value, cfg->accLimit);
}

void
test_SetGetBrakeLimit(void)
{
    rui8_t value;
    Config *cfg;

    rkh_enter_critical_Ignore();
    rkh_exit_critical_Ignore();
    rkh_enter_critical_Ignore();
    ffile_seek_Ignore();
    ffile_random_access_IgnoreAndReturn(1);
    ffile_sync_Ignore();
    rkh_exit_critical_Ignore();

    value = 4;
    Config_setBrLimit(value);
    cfg = Config_get();
    TEST_ASSERT_EQUAL(value, cfg->brLimit);

    value = 0;
    value = Config_getBrLimit();
    TEST_ASSERT_EQUAL(value, cfg->brLimit);
}

void
test_SetGetDefault(void)
{
    rui8_t value;
    Config *cfg;

    rkh_enter_critical_Ignore();
    rkh_exit_critical_Ignore();
    rkh_enter_critical_Ignore();
    ffile_seek_Ignore();
    ffile_random_access_IgnoreAndReturn(1);
    ffile_sync_Ignore();
    rkh_exit_critical_Ignore();

    value = 1;
    Config_setDefault(value);
    cfg = Config_get();
    TEST_ASSERT_EQUAL(value, cfg->status);

    value = 0;
    value = Config_getDefault();
    TEST_ASSERT_EQUAL(value, cfg->status);
}

void
test_SetIP(void)
{
    char connectionDomain[64];
    Config *cfg;

    rkh_enter_critical_Ignore();
    rkh_exit_critical_Ignore();
    rkh_enter_critical_Ignore();
    ffile_seek_Ignore();
    ffile_random_access_IgnoreAndReturn(1);
    ffile_sync_Ignore();
    rkh_exit_critical_Ignore();

    strcpy(connectionDomain, "8.8.8.8");
    Config_setConnectionDomain(connectionDomain);
    cfg = Config_get();
    TEST_ASSERT_EQUAL_STRING(connectionDomain, cfg->connectionDomain);

    rkh_assert_Expect("Config", 0);
    rkh_assert_IgnoreArg_file();
    rkh_assert_IgnoreArg_line();
    rkh_assert_StubWithCallback(MockAssertCallback);
    strcpy(connectionDomain, "deadbeaddeadbeaddeadbeaddeadbead");
    Config_setConnectionDomain(connectionDomain);

    rkh_assert_Expect("Config", 0);
    rkh_assert_IgnoreArg_file();
    rkh_assert_IgnoreArg_line();
    rkh_assert_StubWithCallback(MockAssertCallback);
    Config_setConnectionDomain((char *)0);
}

void
test_GetIP(void)
{
    char connectionDomain[64];
    Config *cfg;

    rkh_enter_critical_Ignore();
    rkh_exit_critical_Ignore();
    rkh_enter_critical_Ignore();
    ffile_seek_Ignore();
    ffile_random_access_IgnoreAndReturn(1);
    ffile_sync_Ignore();
    rkh_exit_critical_Ignore();

    Config_getConnectionDomain(connectionDomain);
    cfg = Config_get();
    TEST_ASSERT_EQUAL_STRING(connectionDomain, cfg->connectionDomain);

    rkh_assert_Expect("Config", 0);
    rkh_assert_IgnoreArg_file();
    rkh_assert_IgnoreArg_line();
    rkh_assert_StubWithCallback(MockAssertCallback);
    Config_getConnectionDomain((char *)0);
}

void
test_SetPort(void)
{
    char connectionPort[64];
    Config *cfg;

    rkh_enter_critical_Ignore();
    rkh_exit_critical_Ignore();
    rkh_enter_critical_Ignore();
    ffile_seek_Ignore();
    ffile_random_access_IgnoreAndReturn(1);
    ffile_sync_Ignore();
    rkh_exit_critical_Ignore();

    strcpy(connectionPort, "99999");
    Config_setConnectionPort(connectionPort);
    cfg = Config_get();
    TEST_ASSERT_EQUAL_STRING(connectionPort, cfg->connectionPort);

    rkh_assert_Expect("Config", 0);
    rkh_assert_IgnoreArg_file();
    rkh_assert_IgnoreArg_line();
    rkh_assert_StubWithCallback(MockAssertCallback);
    strcpy(connectionPort, "deadbeaddeadbeaddeadbeaddeadbead");
    Config_setConnectionPort(connectionPort);

    rkh_assert_Expect("Config", 0);
    rkh_assert_IgnoreArg_file();
    rkh_assert_IgnoreArg_line();
    rkh_assert_StubWithCallback(MockAssertCallback);
    Config_setConnectionPort((char *)0);
}

void
test_GetConnectionPort(void)
{
    char connectionPort[64];
    Config *cfg;

    rkh_enter_critical_Ignore();
    rkh_exit_critical_Ignore();
    rkh_enter_critical_Ignore();
    ffile_seek_Ignore();
    ffile_random_access_IgnoreAndReturn(1);
    ffile_sync_Ignore();
    rkh_exit_critical_Ignore();

    Config_getConnectionPort(connectionPort);
    cfg = Config_get();
    TEST_ASSERT_EQUAL_STRING(connectionPort, cfg->connectionPort);

    rkh_assert_Expect("Config", 0);
    rkh_assert_IgnoreArg_file();
    rkh_assert_IgnoreArg_line();
    rkh_assert_StubWithCallback(MockAssertCallback);
    Config_getConnectionPort((char *)0);
}

void
test_SetGetMapTimeOnRunning(void)
{
    rui8_t time;
    Config *cfg;

    rkh_enter_critical_Ignore();
    rkh_exit_critical_Ignore();
    rkh_enter_critical_Ignore();
    ffile_seek_Ignore();
    ffile_random_access_IgnoreAndReturn(1);
    ffile_sync_Ignore();
    rkh_exit_critical_Ignore();

    time = 4;
    Config_setMapTimeOnRunning(time);
    cfg = Config_get();
    TEST_ASSERT_EQUAL(time, cfg->mapTimeOnRunning);

    time = 0;
    time = Config_getMapTimeOnRunning();
    TEST_ASSERT_EQUAL(time, cfg->mapTimeOnRunning);
}

void
test_SetGetMapTimeOnStopped(void)
{
    rui8_t time;
    Config *cfg;

    rkh_enter_critical_Ignore();
    rkh_exit_critical_Ignore();
    rkh_enter_critical_Ignore();
    ffile_seek_Ignore();
    ffile_random_access_IgnoreAndReturn(1);
    ffile_sync_Ignore();
    rkh_exit_critical_Ignore();

    time = 4;
    Config_setMapTimeOnStopped(time);
    cfg = Config_get();
    TEST_ASSERT_EQUAL(time, cfg->mapTimeOnStopped);

    time = 0;
    time = Config_getMapTimeOnStopped();
    TEST_ASSERT_EQUAL(time, cfg->mapTimeOnStopped);
}

void
test_SetGetConnTime(void)
{
    rui8_t time;
    Config *cfg;

    rkh_enter_critical_Ignore();
    rkh_exit_critical_Ignore();
    rkh_enter_critical_Ignore();
    ffile_seek_Ignore();
    ffile_random_access_IgnoreAndReturn(1);
    ffile_sync_Ignore();
    rkh_exit_critical_Ignore();

    time = 4;
    Config_setConnTime(time);
    cfg = Config_get();
    TEST_ASSERT_EQUAL(time, cfg->connTime);

    time = 0;
    time = Config_getConnTime();
    TEST_ASSERT_EQUAL(time, cfg->connTime);
}

void
test_SetGetDevPollCycleTime(void)
{
    rui8_t time;
    Config *cfg;

    rkh_enter_critical_Ignore();
    rkh_exit_critical_Ignore();
    rkh_enter_critical_Ignore();
    ffile_seek_Ignore();
    ffile_random_access_IgnoreAndReturn(1);
    ffile_sync_Ignore();
    rkh_exit_critical_Ignore();

    time = 4;
    Config_setDevPollCycleTime(time);
    cfg = Config_get();
    TEST_ASSERT_EQUAL(time, cfg->devPollCycleTime);

    time = 0;
    time = Config_getDevPollCycleTime();
    TEST_ASSERT_EQUAL(time, cfg->devPollCycleTime);
}

void
test_SetGetMaxNumFramesToSend(void)
{
    rui16_t time;
    Config *cfg;

    rkh_enter_critical_Ignore();
    rkh_exit_critical_Ignore();
    rkh_enter_critical_Ignore();
    ffile_seek_Ignore();
    ffile_random_access_IgnoreAndReturn(1);
    ffile_sync_Ignore();
    rkh_exit_critical_Ignore();

    time = 4;
    Config_setMaxNumFramesToSend(time);
    cfg = Config_get();
    TEST_ASSERT_EQUAL(time, cfg->maxNumFramesToSend);

    time = 0;
    time = Config_getMaxNumFramesToSend();
    TEST_ASSERT_EQUAL(time, cfg->maxNumFramesToSend);
}

void
test_SetGetUpdateGPSTime(void)
{
    rui8_t time;
    Config *cfg;

    rkh_enter_critical_Ignore();
    rkh_exit_critical_Ignore();
    rkh_enter_critical_Ignore();
    ffile_seek_Ignore();
    ffile_random_access_IgnoreAndReturn(1);
    ffile_sync_Ignore();
    rkh_exit_critical_Ignore();

    time = 4;
    Config_setUpdateGPSTime(time);
    cfg = Config_get();
    TEST_ASSERT_EQUAL(time, cfg->updateGPSTime);

    time = 0;
    time = Config_getUpdateGPSTime();
    TEST_ASSERT_EQUAL(time, cfg->updateGPSTime);
}

void
test_SetGetDftDigOut(void)
{
    DigOut out;
    Config *cfg;

    rkh_enter_critical_Ignore();
    rkh_exit_critical_Ignore();
    rkh_enter_critical_Ignore();
    ffile_seek_Ignore();
    ffile_random_access_IgnoreAndReturn(1);
    ffile_sync_Ignore();
    rkh_exit_critical_Ignore();

    out = 4;
    Config_setDftDigOut(out);
    cfg = Config_get();
    TEST_ASSERT_EQUAL(out, cfg->dftDigOut);

    out = 0;
    out = Config_getDftDigOut();
    TEST_ASSERT_EQUAL(out, cfg->dftDigOut);
}

void
test_SetMaxNumStoreOnStopped(void)
{
    rui16_t out;
    Config *cfg;

    rkh_enter_critical_Ignore();
    rkh_exit_critical_Ignore();
    rkh_enter_critical_Ignore();
    ffile_seek_Ignore();
    ffile_random_access_IgnoreAndReturn(1);
    ffile_sync_Ignore();
    rkh_exit_critical_Ignore();

    out = 4;
    Config_setMaxNumStoreOnStopped(out);
    cfg = Config_get();
    TEST_ASSERT_EQUAL(out, cfg->maxNumStoreOnStopped);

    out = 0;
    out = Config_getMaxNumStoreOnStopped();
    TEST_ASSERT_EQUAL(out, cfg->maxNumStoreOnStopped);
}

void
test_SetMaxNumStoreOnRunning(void)
{
    rui16_t out;
    Config *cfg;

    rkh_enter_critical_Ignore();
    rkh_exit_critical_Ignore();
    rkh_enter_critical_Ignore();
    ffile_seek_Ignore();
    ffile_random_access_IgnoreAndReturn(1);
    ffile_sync_Ignore();
    rkh_exit_critical_Ignore();

    out = 4;
    Config_setMaxNumStoreOnRunning(out);
    cfg = Config_get();
    TEST_ASSERT_EQUAL(out, cfg->maxNumStoreOnRunning);

    out = 0;
    out = Config_getMaxNumStoreOnRunning();
    TEST_ASSERT_EQUAL(out, cfg->maxNumStoreOnRunning);
}

/* ------------------------------ End of file ------------------------------ */
