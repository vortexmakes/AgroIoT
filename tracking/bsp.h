/**
 *  \file       bsp.h
 *  \brief      BSP for 80x86 OS win32
 *
 *  \ingroup    bsp
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2018.05.23  DaBa  v0.0.01  Initial version
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  LeFr  Leandro Francucci  lf@vortexmakes.com
 *  DaBa  Dario Baliña       dariosb@gmail.com
 */

/* --------------------------------- Module -------------------------------- */
#ifndef __BSP_H__
#define __BSP_H__

/* ----------------------------- Include files ----------------------------- */
#include "rkh.h"

/* ---------------------- External C language linkage ---------------------- */
#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------- Macros -------------------------------- */
/* -------------------------------- Constants ------------------------------ */
/*
 * Serial Ports channels identifiers
 */
enum
{
	GSM_PORT, 
	GPS_PORT,
	TPSENS_PORT,

  	NUM_CHANNELS
};

/*
 * User trace events id´s
 */
enum
{
    MODCMD_USR_TRACE = RKH_TE_USER,
};

/*
 * Sim Selection
 */
typedef enum
{
    MainSIM, 
    SecSIM
} SIMSelect_t;

/*
 * Status Led´s 
 */
typedef enum
{
    DisconnectedSt, 
    UnregisteredSt = DisconnectedSt,

    ConnectedSt,
    RegisteredSt = ConnectedSt,
} Status_t;



/* ------------------------------- Data types ------------------------------ */
/* -------------------------- External variables --------------------------- */
/* -------------------------- Function prototypes -------------------------- */
void bsp_init(int argc, char *argv[]);
void bsp_close(void);

void bsp_keyParser(int c);
void bsp_timeTick(void);

void bsp_serial_open(int ch);
void bsp_serial_close(int ch);
void bsp_serial_puts(int ch, char *p);
void bsp_serial_putnchar(int ch, unsigned char *p, ruint ndata);
void bsp_serial_putchar(int ch, unsigned char c);

void bsp_GSMModemFound(void);
void bsp_SIMReady(void);
void bsp_SIMSelect(SIMSelect_t sim);
void bsp_SIMChange();
void bsp_netStatus(Status_t status);
void bsp_regStatus(Status_t status);
void bsp_sendOk(void);
void bsp_sendFail(void);
void bsp_recvFail(void);
void bsp_recvOk(void);
void bsp_GPSStatus(char status);

void bsp_gpsParserHandler_set(void *p);

/* -------------------- External C language linkage end -------------------- */
#ifdef __cplusplus
}
#endif

/* ------------------------------ Module end ------------------------------- */
#endif

/* ------------------------------ File footer ------------------------------ */
