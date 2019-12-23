/**
 *  \file       Config.h
 *  \brief      Specifies the configurtion module.
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
/* --------------------------------- Module -------------------------------- */
#ifndef __CONFIG_H__
#define __CONFIG_H__

/* ----------------------------- Include files ----------------------------- */
#include "rkhtype.h"
#include "IOStatus.h"

/* ---------------------- External C language linkage ---------------------- */
#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------- Macros -------------------------------- */
/* -------------------------------- Constants ------------------------------ */
#define IP_LENGTH           23
#define PORT_LENGTH         5

/* ------------------------------- Data types ------------------------------ */
typedef struct Config Config;
struct Config
{
    /**
     * Indicates the value in which the system has suddenly accelerated
     */
    rui8_t accLimit;

    /**
     * Indicates the value in which the system has suddenly braked
     */
    rui8_t brLimit;

    /**
     * If status is set to 0, the configuration is set to default
     */
    rui8_t status;

    /**
     * Remote server's IP address
     */
    char connectionDomain[IP_LENGTH + 1];

    /**
     * Remote server's TCP/IP port number
     */
    char connectionPort[PORT_LENGTH + 1];

    /**
     * The mapping is performed by collecting the different
     * status of the system, including its location, every T seconds,
     * which is called mapping time. This period depends on the
     * mapping state (Stopped or Running). If it is in Stopped, it will store 
     * its status every mapTimeOnStopped seconds, otherwise it will do every 
     * mapTimeOnRunning seconds.
     */
    rui8_t mapTimeOnRunning;

    /**
     * Find out more information in mapTimeOnRunning
     */
    rui8_t mapTimeOnStopped;

    /**
     * The system connects to a remote server in order to send it messages
     * every connTime seconds. 
     */
    rui8_t connTime;

    /**
     * Every devPollCycleTime seconds a polling cycle starts over the RS485.
     */
    rui8_t devPollCycleTime;

    /**
     * Max. number of frames (status) to send in a multiframe message.
     */
    rui8_t maxNumFramesToSend;

    /**
     * The location is received by the GPS device every T seconds
     */
    rui8_t updateGPSTime;

    /**
     * Default logic state of digital outputs
     */
    DigOut dftDigOut;

    /**
     * Max. number of status storage in mapping stopped state before 
     * synchronize the file system.
     */
    rui8_t maxNumStoreOnStopped;

    /**
     * Max. number of status storage in mapping running state before
     * synchronize the file system.
     */
    rui8_t maxNumStoreOnRunning;

    /**
     * SIM pin number
     */
    rui16_t SIMPinNumber;

    /**
     */
    rui8_t maxNumConnNoRespRetries;

    /**
     * Specifies the TCP connection status's polling period in seconds
     */
    rui8_t connectionStatusPeriod;

    /**
     * Specifies time delay between server close and reopen in seconds
     */
    rui8_t reopenDelay;

    /**
     * Specifies time delay between server connection attemps in seconds
     */
    rui8_t connectTryDelay;

    /**
     * Specifies time delay between server connection attemps
     */
    rui8_t maxNumConnectRetries;

    /**
     * Specifies time delay after configuring error amd before 
     * restarting connection in seconds
     */
    rui8_t configTryDelay;
};

/* -------------------------- External variables --------------------------- */
/* -------------------------- Function prototypes -------------------------- */
Config *Config_init(void);
Config *config_read(void);
Config *Config_get(void);
void Config_set(Config *cfg);

/** Setters and getters */
void Config_setAccLimit(rui8_t value);
rui8_t Config_getAccLimit(void);
void Config_setBrLimit(rui8_t value);
rui8_t Config_getBrLimit(void);
void Config_setDefault(rui8_t value);
rui8_t Config_getDefault(void);
void Config_setConnectionDomain(char *value);
void Config_getConnectionDomain(char *value);
void Config_setConnectionPort(char *value);
void Config_getConnectionPort(char *value);
void Config_setMapTimeOnRunning(rui8_t value);
rui8_t Config_getMapTimeOnRunning(void);
void Config_setMapTimeOnStopped(rui8_t value);
rui8_t Config_getMapTimeOnStopped(void);
void Config_setConnTime(rui8_t value);
rui8_t Config_getConnTime(void);
void Config_setDevPollCycleTime(rui8_t value);
rui8_t Config_getDevPollCycleTime(void);
void Config_setMaxNumFramesToSend(rui16_t value);
rui16_t Config_getMaxNumFramesToSend(void);
void Config_setUpdateGPSTime(rui8_t value);
rui8_t Config_getUpdateGPSTime(void);
void Config_setDftDigOut(DigOut value);
DigOut Config_getDftDigOut(void);
void Config_setMaxNumStoreOnStopped(rui16_t value);
rui16_t Config_getMaxNumStoreOnStopped(void);
void Config_setMaxNumStoreOnRunning(rui16_t value);
rui16_t Config_getMaxNumStoreOnRunning(void);
void Config_setSIMPinNumber(rui16_t value);
rui16_t Config_getSIMPinNumber(void);
void Config_setMaxNumConnNoRespRetries(rui8_t value);
rui8_t Config_getMaxNumConnNoRespRetries(void);
void Config_setConnectionStatusPeriod(rui8_t value);
rui8_t Config_getConnectionStatusPeriod(void);
void Config_setReopenDelay(rui8_t value);
rui8_t Config_getReopenDelay(void);
void Config_setConnectTryDelay(rui8_t value);
rui8_t Config_getConnectTryDelay(void);
void Config_setMaxNumConnectRetries(rui8_t value);
rui8_t Config_getMaxNumConnectRetries(void);
void Config_setConfigTryDelay(rui8_t value);
rui8_t Config_getConfigTryDelay(void);

/* -------------------- External C language linkage end -------------------- */
#ifdef __cplusplus
}
#endif

/* ------------------------------ Module end ------------------------------- */
#endif

/* ------------------------------ End of file ------------------------------ */
