/**
 *  \file       main.c
 *  \brief      Example application.
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2018.05.17  DaBa  v1.0.00  Initial version
  */

/* -------------------------------- Authors -------------------------------- */
/*
 *  DaBa  Dario Baliña       db@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */
#include <string.h>
#include <stdio.h>
#include "rkh.h"
#include "rkhfwk_dynevt.h"
#include "rkhfwk_pubsub.h"
#include "bsp.h"
#include "signals.h"
#include "CommMgr.h"
#include "conmgr.h"
#include "modmgr.h"
#include "geoMgr.h"
#include "ioChg.h"
#include "DeviceMgr.h"
#include "sprayer.h"
#include "sim5320parser.h"
#include "ubxm8parser.h"
#include "cbox.h"
#include "CollectorAct.h"

#include "ffile.h"
#include "Config.h"
#include "mTime.h"
#include "sequence.h"
#include "epoch.h"

/* ----------------------------- Local macros ------------------------------ */
#define COMMMGR_QSTO_SIZE	16
#define CONMGR_QSTO_SIZE    8
#define MODMGR_QSTO_SIZE    4
#define GEOMGR_QSTO_SIZE    4
#define DEVSRV_QSTO_SIZE    4

#define SIZEOF_EP0STO       16
#define SIZEOF_EP0_BLOCK    sizeof(RKH_EVT_T)

#define SIZEOF_EP1_BLOCK    sizeof(RmcEvt)
#define SIZEOF_EP1STO       (16*SIZEOF_EP1_BLOCK)

#define SIZEOF_EP2_BLOCK    sizeof(ModMgrEvt)
#define SIZEOF_EP2STO       (16*SIZEOF_EP2_BLOCK)

/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
static RKH_EVT_T *CommMgr_qsto[COMMMGR_QSTO_SIZE];
static RKH_EVT_T *ConMgr_qsto[CONMGR_QSTO_SIZE];
static RKH_EVT_T *ModMgr_qsto[MODMGR_QSTO_SIZE];
static RKH_EVT_T *GeoMgr_qsto[GEOMGR_QSTO_SIZE];
static RKH_EVT_T *DeviceMgr_qsto[DEVSRV_QSTO_SIZE];
static rui8_t evPool0Sto[SIZEOF_EP0STO], 
              evPool1Sto[SIZEOF_EP1STO], 
              evPool2Sto[SIZEOF_EP2STO];

static RKH_ROM_STATIC_EVENT(e_Open, evOpen);
Device *sprayer;

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
static void
setupTraceFilters(void)
{
    RKH_FILTER_ON_GROUP(RKH_TRC_ALL_GROUPS);
    RKH_FILTER_ON_EVENT(RKH_TRC_ALL_EVENTS);
	RKH_FILTER_OFF_EVENT(MODCMD_USR_TRACE);
	//RKH_FILTER_OFF_GROUP_ALL_EVENTS(RKH_TG_USR);
    RKH_FILTER_OFF_EVENT(RKH_TE_TMR_TOUT);
    RKH_FILTER_OFF_EVENT(RKH_TE_SM_STATE);
    //RKH_FILTER_OFF_EVENT(RKH_TE_SMA_FIFO);
    //RKH_FILTER_OFF_EVENT(RKH_TE_SMA_LIFO);
    //RKH_FILTER_OFF_EVENT(RKH_TE_SM_TS_STATE);
    RKH_FILTER_OFF_EVENT(RKH_TE_SM_DCH);
    //RKH_FILTER_OFF_SMA(modMgr);
    RKH_FILTER_OFF_SMA(conMgr);
	//RKH_FILTER_OFF_SMA(geoMgr);
	//RKH_FILTER_OFF_SMA(deviceMgr);
	//RKH_FILTER_OFF_SMA(commMgr);
	RKH_FILTER_OFF_ALL_SIGNALS();
}

/* ---------------------------- Global functions --------------------------- */
int
main(int argc, char *argv[])
{
    bsp_init(argc, argv);

    epoch_init();
    init_seqs();
    mTime_init();
    ffile_init();
    Config_init();

    rkh_fwk_init();
    rkh_pubsub_init();

    setupTraceFilters();

    RKH_TRC_OPEN();

    sprayer = sprayer_ctor(0);
    Collector_ctor();
    signals_publishSymbols();

    RKH_TR_FWK_ACTOR(&inChg, "inChg");
    RKH_TR_FWK_ACTOR(&sim5320parser, "sim5320parser");
    RKH_TR_FWK_ACTOR(&ubxm8parser, "ubxm8parser");
	
    rkh_dynEvt_init();
    rkh_fwk_registerEvtPool(evPool0Sto, SIZEOF_EP0STO, SIZEOF_EP0_BLOCK);
    rkh_fwk_registerEvtPool(evPool1Sto, SIZEOF_EP1STO, SIZEOF_EP1_BLOCK);
    rkh_fwk_registerEvtPool(evPool2Sto, SIZEOF_EP2STO, SIZEOF_EP2_BLOCK);

    RKH_SMA_ACTIVATE(conMgr, ConMgr_qsto, CONMGR_QSTO_SIZE, 0, 0);
    RKH_SMA_ACTIVATE(modMgr, ModMgr_qsto, MODMGR_QSTO_SIZE, 0, 0);
	RKH_SMA_ACTIVATE(geoMgr, GeoMgr_qsto, GEOMGR_QSTO_SIZE, 0, 0);
    RKH_SMA_ACTIVATE(deviceMgr, DeviceMgr_qsto, DEVSRV_QSTO_SIZE, 0, 0);

    RKH_SMA_ACTIVATE(commMgr, CommMgr_qsto, COMMMGR_QSTO_SIZE, 0, 0);

    RKH_SMA_POST_FIFO(conMgr, &e_Open, 0);
	RKH_SMA_POST_FIFO(deviceMgr, &e_Open, 0);

    rkh_fwk_enter();

    RKH_TRC_CLOSE();
    return 0;
}

/* ------------------------------ End of file ------------------------------ */
