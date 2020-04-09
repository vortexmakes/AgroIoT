/**
 *  \file       GsmMgrAccess.c
 *  \brief      Active object's action required interface implementations.
 */

/* -------------------------- Development history -------------------------- */
/*
 */

/* -------------------------------- Authors -------------------------------- */
/*
 */

/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */
#include <string.h>

#include "rkh.h"
#include "GsmMgr.h"
#include "GsMMgrAccess.h"
#include "bsp.h"
#include "ModCmd.h"
#include "modpwr.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ---------------------------- Global functions --------------------------- */
void
powerOn(void)
{
    modPwr_on();
    ModCmd_init();
}

void
powerOff(void)
{
    modPwr_off();
    bsp_SIMChange();
}

void
modemFound(void)
{
    bsp_GSMModemFound();
}

void
registered(void)
{
    bsp_regStatus(RegisteredSt);
}

void
unregistered(void)
{
    bsp_regStatus(UnregisteredSt);
}

void
stopSMS(void)
{

}

/* ------------------------------ End of file ------------------------------ */
