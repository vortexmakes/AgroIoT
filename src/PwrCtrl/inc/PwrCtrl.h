/**
 *  \file       PwrCtrl.h
 *  \brief      Specifies Power Fail and Shutdown control module
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2020.03.30  DaBa  v1.0.00  Initial version
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  DaBa  Darío Baliña  db@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* --------------------------------- Module -------------------------------- */
#ifndef __PWRCTRL_H__
#define __PWRCTRL_H__

/* ----------------------------- Include files ----------------------------- */
/* ---------------------- External C language linkage ---------------------- */
#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------- Macros -------------------------------- */
/* -------------------------------- Constants ------------------------------ */
/* ------------------------------- Data types ------------------------------ */
/*
 * Power Control states
 */
typedef enum
{
    PowerON,
    PowerOFF,
}PwrCtrl_t;

/* -------------------------- External variables --------------------------- */
/* -------------------------- Function prototypes -------------------------- */
void PwrCtrl_init(void);
void PwrCtrl_powerFailCheck(void);
void PwrCtrl_toPowerOff(void);

/* -------------------- External C language linkage end -------------------- */
#ifdef __cplusplus
}
#endif

/* ------------------------------ Module end ------------------------------- */
#endif

/* ------------------------------ End of file ------------------------------ */
