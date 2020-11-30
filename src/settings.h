/**
 *  \file settings.h
 *
 *	This module contains application constant declarations
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
/* --------------------------------- Module -------------------------------- */
#ifndef __SETTINGS_H__
#define __SETTINGS_H__

/* ----------------------------- Include files ----------------------------- */
#include "rkh.h"

/* ---------------------- External C language linkage ---------------------- */
#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------- Macros -------------------------------- */
/* -------------------------------- Constants ------------------------------ */
#define ACCELERATION_PERIOD             3
#define MAX_NFRAMES_TOSEND              100

/* Default configurations */
#define ACC_LIMIT_DFT                   6
#define BR_LIMIT_DFT                    20
#define CONNECTION_DOMAIN_DFT           "216.75.55.101"
#define CONNECTION_PORT_DFT             "33499"
#define MAP_TIME_ON_RUNNING_DFT         3
#define MAP_TIME_ON_STOPPED_DFT         60
#define CONN_TIME_DFT                   60
#define DEV_POLL_CYCLE_TIME_DFT         1000
#define MAX_NUM_FRAMES_TO_SEND_DFT      100
#define UPDATE_GPS_TIME                 2
#define DFT_DIG_OUT_DFT                 0
#define MAX_NUM_STORE_ON_STOPPED_DFT    240
#define MAX_NUM_STORE_ON_RUNNING_DFT    100
#define SIM_PIN_NUMBER_DFT              9474
#define MAX_NUM_CONNNORESP_RETRIES_DFT  2
#define CONNECTION_STATUS_PERIOD_DFT    5
#define REOPEN_DELAY_DFT                5
#define CONNECT_TRY_DELAY_DFT           5
#define MAX_NUM_CONNECT_RETRIES_DFT     3
#define CONFIG_TRY_DELAY_DFT            3
#define DIGIN_POLARITY_DFT              1   /* DigInPol::ActiveHigh (1) */

/* ------------------------------- Data types ------------------------------ */
/* -------------------------- External variables --------------------------- */
/* -------------------------- Function prototypes -------------------------- */
/* -------------------- External C language linkage end -------------------- */
#ifdef __cplusplus
}
#endif

/* ------------------------------ Module end ------------------------------- */
#endif

/* ------------------------------ End of file ------------------------------ */
