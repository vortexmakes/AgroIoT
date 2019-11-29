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
static Config cfgFile =
{
    ACLIMIT_DFT,
    BRLIMIT_DFT,
    1,
    "\"111.22.33.444\"",
    "44500",
    70,
    100,
    9,
    1,
    2
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
    TEST_ASSERT_EQUAL(cfgDft.brlimit, cfg->brlimit);
    TEST_ASSERT_EQUAL(cfgDft.status, cfg->status);
    TEST_ASSERT_EQUAL_STRING(cfgDft.ip, cfg->ip);
    TEST_ASSERT_EQUAL_STRING(cfgDft.port, cfg->port);
    TEST_ASSERT_EQUAL(cfgDft.conntime, cfg->conntime);
    TEST_ASSERT_EQUAL(cfgDft.totacttime, cfg->totacttime);
    TEST_ASSERT_EQUAL(cfgDft.gpsttime, cfg->gpsttime);
    TEST_ASSERT_EQUAL(cfgDft.digOut, cfg->digOut);
    TEST_ASSERT_EQUAL(cfgDft.smptime, cfg->smptime);
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
    TEST_ASSERT_EQUAL(cfgFile.brlimit, cfg->brlimit);
    TEST_ASSERT_EQUAL(cfgFile.status, cfg->status);
    TEST_ASSERT_EQUAL_STRING(cfgFile.ip, cfg->ip);
    TEST_ASSERT_EQUAL_STRING(cfgFile.port, cfg->port);
    TEST_ASSERT_EQUAL(cfgFile.conntime, cfg->conntime);
    TEST_ASSERT_EQUAL(cfgFile.totacttime, cfg->totacttime);
    TEST_ASSERT_EQUAL(cfgFile.gpsttime, cfg->gpsttime);
    TEST_ASSERT_EQUAL(cfgFile.digOut, cfg->digOut);
    TEST_ASSERT_EQUAL(cfgFile.smptime, cfg->smptime);
}

void
test_InitWithFileError(void)
{
    Config *cfg, cfgErased;

    cfg = (Config *)0;
    cfgErased.status = 0xff;

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
    TEST_ASSERT_EQUAL(cfgDft.brlimit, cfg->brlimit);
    TEST_ASSERT_EQUAL(cfgDft.status, cfg->status);
    TEST_ASSERT_EQUAL_STRING(cfgDft.ip, cfg->ip);
    TEST_ASSERT_EQUAL_STRING(cfgDft.port, cfg->port);
    TEST_ASSERT_EQUAL(cfgDft.conntime, cfg->conntime);
    TEST_ASSERT_EQUAL(cfgDft.totacttime, cfg->totacttime);
    TEST_ASSERT_EQUAL(cfgDft.gpsttime, cfg->gpsttime);
    TEST_ASSERT_EQUAL(cfgDft.digOut, cfg->digOut);
    TEST_ASSERT_EQUAL(cfgDft.smptime, cfg->smptime);
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
    Config *cfg, cfgErased;

    cfgErased.status = 0xff;
    rkh_enter_critical_Expect();
    ffile_seek_Expect(FFD1, 0);
    ffile_random_access_ExpectAndReturn(FFD1, WRITE_ACCESS, &cfgErased, 1, 1);
    ffile_sync_Expect();
    rkh_exit_critical_Expect();

    Config_set(&cfgErased);

    cfg = (Config *)0;
    rkh_enter_critical_Expect();
    rkh_assert_Expect("Config", 0);
    rkh_assert_IgnoreArg_file();
    rkh_assert_IgnoreArg_line();
    rkh_assert_StubWithCallback(MockAssertCallback);

    cfg = Config_get();
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
test_SetGetMappingTime(void)
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
    Config_setMappingTime(time);
    cfg = Config_get();
    TEST_ASSERT_EQUAL(time, cfg->smptime);

    time = 0;
    time = Config_getMappingTime();
    TEST_ASSERT_EQUAL(time, cfg->smptime);
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
    TEST_ASSERT_EQUAL(out, cfg->digOut);

    out = 0;
    out = Config_getDftDigOut();
    TEST_ASSERT_EQUAL(out, cfg->digOut);
}

void
test_SetGetUpdateLocTime(void)
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
    Config_setUpdateLocTime(time);
    cfg = Config_get();
    TEST_ASSERT_EQUAL(time, cfg->gpsttime);

    time = 0;
    time = Config_getUpdateLocTime();
    TEST_ASSERT_EQUAL(time, cfg->gpsttime);
}

void
test_SetGetConnPeriodTime(void)
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
    Config_setConnPeriodTime(time);
    cfg = Config_get();
    TEST_ASSERT_EQUAL(time, cfg->conntime);

    time = 0;
    time = Config_getConnPeriodTime();
    TEST_ASSERT_EQUAL(time, cfg->conntime);
}

void
test_SetIP(void)
{
    char ip[64];
    Config *cfg;

    rkh_enter_critical_Ignore();
    rkh_exit_critical_Ignore();
    rkh_enter_critical_Ignore();
    ffile_seek_Ignore();
    ffile_random_access_IgnoreAndReturn(1);
    ffile_sync_Ignore();
    rkh_exit_critical_Ignore();

    strcpy(ip, "8.8.8.8");
    Config_setIP(ip);
    cfg = Config_get();
    TEST_ASSERT_EQUAL_STRING(ip, cfg->ip);

    rkh_assert_Expect("Config", 0);
    rkh_assert_IgnoreArg_file();
    rkh_assert_IgnoreArg_line();
    rkh_assert_StubWithCallback(MockAssertCallback);
    strcpy(ip, "deadbeaddeadbeaddeadbeaddeadbead");
    Config_setIP(ip);

    rkh_assert_Expect("Config", 0);
    rkh_assert_IgnoreArg_file();
    rkh_assert_IgnoreArg_line();
    rkh_assert_StubWithCallback(MockAssertCallback);
    Config_setIP((char *)0);
}

void
test_GetIP(void)
{
    char ip[64];
    Config *cfg;

    rkh_enter_critical_Ignore();
    rkh_exit_critical_Ignore();
    rkh_enter_critical_Ignore();
    ffile_seek_Ignore();
    ffile_random_access_IgnoreAndReturn(1);
    ffile_sync_Ignore();
    rkh_exit_critical_Ignore();

    Config_getIP(ip);
    cfg = Config_get();
    TEST_ASSERT_EQUAL_STRING(ip, cfg->ip);

    rkh_assert_Expect("Config", 0);
    rkh_assert_IgnoreArg_file();
    rkh_assert_IgnoreArg_line();
    rkh_assert_StubWithCallback(MockAssertCallback);
    Config_getIP((char *)0);
}

void
test_SetPort(void)
{
    char port[64];
    Config *cfg;

    rkh_enter_critical_Ignore();
    rkh_exit_critical_Ignore();
    rkh_enter_critical_Ignore();
    ffile_seek_Ignore();
    ffile_random_access_IgnoreAndReturn(1);
    ffile_sync_Ignore();
    rkh_exit_critical_Ignore();

    strcpy(port, "99999");
    Config_setPort(port);
    cfg = Config_get();
    TEST_ASSERT_EQUAL_STRING(port, cfg->port);

    rkh_assert_Expect("Config", 0);
    rkh_assert_IgnoreArg_file();
    rkh_assert_IgnoreArg_line();
    rkh_assert_StubWithCallback(MockAssertCallback);
    strcpy(port, "deadbeaddeadbeaddeadbeaddeadbead");
    Config_setPort(port);

    rkh_assert_Expect("Config", 0);
    rkh_assert_IgnoreArg_file();
    rkh_assert_IgnoreArg_line();
    rkh_assert_StubWithCallback(MockAssertCallback);
    Config_setPort((char *)0);
}

void
test_GetPort(void)
{
    char port[64];
    Config *cfg;

    rkh_enter_critical_Ignore();
    rkh_exit_critical_Ignore();
    rkh_enter_critical_Ignore();
    ffile_seek_Ignore();
    ffile_random_access_IgnoreAndReturn(1);
    ffile_sync_Ignore();
    rkh_exit_critical_Ignore();

    Config_getPort(port);
    cfg = Config_get();
    TEST_ASSERT_EQUAL_STRING(port, cfg->port);

    rkh_assert_Expect("Config", 0);
    rkh_assert_IgnoreArg_file();
    rkh_assert_IgnoreArg_line();
    rkh_assert_StubWithCallback(MockAssertCallback);
    Config_getPort((char *)0);
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
    Config_setBrakeLimit(value);
    cfg = Config_get();
    TEST_ASSERT_EQUAL(value, cfg->brlimit);

    value = 0;
    value = Config_getBrakeLimit();
    TEST_ASSERT_EQUAL(value, cfg->brlimit);
}

/* ------------------------------ End of file ------------------------------ */
