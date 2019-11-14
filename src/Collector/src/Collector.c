/**
 *  \file       collector.c
 *  \brief      Implementation of Collector module.
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2018.11.22  LeFr  v1.0.00  Initial version
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  LeFr  Leandro Francucci  lf@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */
#include "rkhsma.h"
#include "signals.h"
#include "Collector.h"
#include "CollectorAct.h"

/* ----------------------------- Local macros ------------------------------ */
/* ........................ Declares initial action ........................ */
/* ........................ Declares effect actions ........................ */
/* ......................... Declares entry actions ........................ */
/* ......................... Declares exit actions ......................... */
/* ............................ Declares guards ............................ */
/* ........................ States and pseudostates ........................ */
RKH_CREATE_COMP_REGION_STATE(DevStatus_Active, Collector_enActive,
                             Collector_exActive, RKH_ROOT,
                             &DevStatus_DevNotConnected, NULL,
                             RKH_NO_HISTORY, NULL, NULL, NULL, NULL);
RKH_CREATE_TRANS_TABLE(DevStatus_Active)
RKH_TRINT(evGeo, NULL, Collector_updatePosition),
RKH_TRINT(evGeoInvalid, NULL, Collector_updatePosition),
RKH_TRINT(evDigOutChanged, NULL, Collector_updateDigOut),
RKH_TRINT(evTimeout, NULL, Collector_publishCurrStatus),
RKH_END_TRANS_TABLE

RKH_CREATE_BASIC_STATE(DevStatus_DevNotConnected,
                       Collector_initAndTestDevNull,
                       NULL, &DevStatus_Active, NULL);
RKH_CREATE_TRANS_TABLE(DevStatus_DevNotConnected)
RKH_TRINT(evDigInChanged, NULL, Collector_updateDigInTestDevNull),
RKH_TRREG(evDevData, NULL, Collector_updateAndTestDevData,
          &DevStatus_DevConnected),
RKH_END_TRANS_TABLE

RKH_CREATE_BASIC_STATE(DevStatus_DevConnected, NULL,
                       NULL, &DevStatus_Active, NULL);
RKH_CREATE_TRANS_TABLE(DevStatus_DevConnected)
RKH_TRINT(evDevData, NULL, Collector_updateAndTestDevData),
RKH_TRINT(evDigInChanged, NULL, Collector_updateDigIn),
RKH_TRREG(evNoDev, NULL, NULL, &DevStatus_DevNotConnected),
RKH_END_TRANS_TABLE

RKH_CREATE_COMP_REGION_STATE(Mapping_Active, NULL, NULL, RKH_ROOT,
                             &Mapping_Stopped, NULL,
                             RKH_NO_HISTORY, NULL, NULL, NULL, NULL);
RKH_CREATE_TRANS_TABLE(Mapping_Active)
RKH_END_TRANS_TABLE

RKH_CREATE_BASIC_STATE(Mapping_Stopped, NULL,
                       NULL, &Mapping_Active, NULL);
RKH_CREATE_TRANS_TABLE(Mapping_Stopped)
RKH_TRREG(evToutSyncStopped, NULL, Mapping_storeStatus, &Mapping_C1),
RKH_TRREG(evMapping, NULL, NULL, &Mapping_Running),
RKH_END_TRANS_TABLE

RKH_CREATE_CHOICE_STATE(Mapping_C1);
RKH_CREATE_BRANCH_TABLE(Mapping_C1)
RKH_BRANCH(Mapping_isSyncDirOnStopped, Mapping_syncDir, &Mapping_Stopped),
RKH_BRANCH(ELSE, NULL, &Mapping_Stopped),
RKH_END_BRANCH_TABLE

RKH_CREATE_BASIC_STATE(Mapping_Running, NULL,
                       NULL, &Mapping_Active, NULL);
RKH_CREATE_TRANS_TABLE(Mapping_Running)
RKH_TRREG(evToutSyncRunning, NULL, Mapping_storeStatus, &Mapping_C3),
RKH_TRREG(evNoMapping, NULL, NULL, &Mapping_C2),
RKH_END_TRANS_TABLE

RKH_CREATE_CHOICE_STATE(Mapping_C2);
RKH_CREATE_BRANCH_TABLE(Mapping_C2)
RKH_BRANCH(Mapping_isSyncDirOnStopped, Mapping_syncDir, &Mapping_Stopped),
RKH_BRANCH(ELSE, NULL, &Mapping_Stopped),
RKH_END_BRANCH_TABLE

RKH_CREATE_CHOICE_STATE(Mapping_C3);
RKH_CREATE_BRANCH_TABLE(Mapping_C3)
RKH_BRANCH(Mapping_isSyncDirOnRunning, Mapping_syncDir, &Mapping_Running),
RKH_BRANCH(ELSE, NULL, &Mapping_Running),
RKH_END_BRANCH_TABLE

/* ............................. Active object ............................. */
RKH_SMA_CREATE(Collector, collector, 3, HCAL, NULL, Collector_init, NULL);
RKH_SMA_DEF_PTR(collector);
RKH_SM_CONST_CREATE(mapping, 0, HCAL, &Mapping_Active, NULL, NULL);

/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ............................ Initial action ............................. */
/* ............................ Effect actions ............................. */
/* ............................. Entry actions ............................. */
/* ............................. Exit actions .............................. */
/* ................................ Guards ................................. */
/* ---------------------------- Global functions --------------------------- */
/* ------------------------------ End of file ------------------------------ */
