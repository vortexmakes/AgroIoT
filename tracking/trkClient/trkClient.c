/**
 *  \file       trkClient.c
 *  \brief      Yipies Tracking Client.
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2018.06.05  DaBa  v1.0.00   Initial version
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  DaBa  Dario Baliña db@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */
#include <stdio.h>
#include <string.h>
#include "rkh.h"
#include "rkhtmr.h"
#include "signals.h"
#include "trkClient.h"
#include "conmgr.h"
#include "epoch.h"
#include "date.h"

/* ----------------------------- Local macros ------------------------------ */
/* ......................... Declares active object ........................ */
typedef struct TrkClient TrkClient;

/* ................... Declares states and pseudostates .................... */
RKH_DCLR_BASIC_STATE Sync_Idle; 
/* ........................ Declares initial action ........................ */
static void init(TrkClient *const me, RKH_EVT_T *pe);

/* ........................ Declares effect actions ........................ */
/* ......................... Declares entry actions ........................ */
/* ......................... Declares exit actions ......................... */
/* ............................ Declares guards ............................ */
/* ........................ States and pseudostates ........................ */
RKH_CREATE_BASIC_STATE(Client_Idle, NULL, NULL, RKH_ROOT, NULL);
RKH_CREATE_TRANS_TABLE(Client_Idle)
    RKH_TRREG(evActivate, NULL, NULL, &Client_Idle),
RKH_END_TRANS_TABLE

/* ............................. Active object ............................. */
struct TrkClient
{
    RKH_SMA_T ao;               /* Base structure */
    rui8_t sendbuf[2048];  /* sendbuf should be large enough to hold */
                            /* multiple whole mqtt messages */
    rui8_t recvbuf[1024];  /* recvbuf should be large enough any whole */
                            /* mqtt message expected to be received */
};

RKH_SMA_CREATE(TrkClient, trkClient, 2, HCAL, &Client_Idle, init, NULL);
RKH_SMA_DEF_PTR(trkClient);

/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ............................ Initial action ............................. */
static void
init(TrkClient *const me, RKH_EVT_T *pe)
{
	(void)pe;

    RKH_TR_FWK_AO(me);
    RKH_TR_FWK_QUEUE(&RKH_UPCAST(RKH_SMA_T, me)->equeue);
    RKH_TR_FWK_STATE(me, &Client_Idle);

}

/* ............................ Effect actions ............................. */
/* ............................. Entry actions ............................. */
/* ............................. Exit actions .............................. */
/* ................................ Guards ................................. */
/* ---------------------------- Global functions --------------------------- */
/* ------------------------------ End of file ------------------------------ */
