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
    rui8_t aclimit;

    /**
     * Indicates the value in which the system has suddenly braked
     */
    rui8_t brlimit;

    /**
     * When status is set to 0 configuration is set to default
     */
    rui8_t status;

    /**
     * Remote server's IP address
     */
    char ip[IP_LENGTH + 1];

    /**
     * Remote server's TCP/IP port number
     */
    char port[PORT_LENGTH + 1];

    /**
     * The system connects to remote server in order to send it messages
     * every conntime seconds
     */
    rui8_t conntime;

    /**
     * Maximum time to register in the GSM network, open a TCP/IP socket,
     * and send a message to the remote server
     * Should it be deprecated?
     */
    rui8_t totacttime;

    /**
     * The location is received by the GPS device every T seconds
     */
    rui8_t gpsttime;

    /**
     * Default logic state of digital outputs
     */
    DigOut digOut;

    /**
     * The mapping is performed by collecting the different
     * status of the system, including its location, every T seconds,
     * which is called the mapping time. This period depends on the
     * mapping state (Stopped or Running). In Stopped state the system
     * stores its status every smptime.
     */
    rui8_t smptime;
};

/* -------------------------- External variables --------------------------- */
/* -------------------------- Function prototypes -------------------------- */
Config *Config_init(void);
Config *config_read(void);
Config *Config_get(void);
void Config_set(Config *cfg);

void Config_setMappingTime(rui8_t value);
rui8_t Config_getMappingTime(void);
void Config_setDftDigOut(DigOut value);
DigOut Config_getDftDigOut(void);
void Config_setUpdateLocTime(rui8_t value);
rui8_t Config_getUpdateLocTime(void);
void Config_setConnPeriodTime(rui8_t value);
rui8_t Config_getConnPeriodTime(void);
void Config_setIP(char *value);
void Config_getIP(char *value);
void Config_setPort(char *value);
void Config_getPort(char *value);
void Config_setDefault(rui8_t value);
rui8_t Config_getDefault(void);
void Config_setAccLimit(rui8_t value);
rui8_t Config_getAccLimit(void);
void Config_setBrakeLimit(rui8_t value);
rui8_t Config_getBrakeLimit(void);

/* -------------------- External C language linkage end -------------------- */
#ifdef __cplusplus
}
#endif

/* ------------------------------ Module end ------------------------------- */
#endif

/* ------------------------------ End of file ------------------------------ */
