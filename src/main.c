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
 *  DaBa  Dario Bali�a       db@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */
#include <string.h>
#include <stdio.h>
#include "rkh.h"
#include "rkhfwk_dynevt.h"
#include "rkhfwk_pubsub.h"
#include "bsp.h"
#include "BatChr.h"
#include "signals.h"
#include "PowerMgr.h"
#include "CommMgr.h"
#include "GsmMgr.h"
#include "ModMgr.h"
#include "geoMgr.h"
#include "ioChg.h"
#include "DeviceMgr.h"
#include "UsbMgr.h"
#include "sprayer.h"
#include "Sampler.h"
#include "Harvest.h"
#include "sim5320parser.h"
#include "ubxm8parser.h"
#include "cbox.h"
#include "Collector.h"
#include "CollectorAct.h"
#include "StatQue.h"
#include "ffport.h"
#include "ffile.h"
#include "ffdata.h"
#include "Config.h"
#include "mTime.h"
#include "sequence.h"
#include "epoch.h"
#include "Trace.h"

/* ----------------------------- Local macros ------------------------------ */

/*
 * Active Objects Queues
 */
#define POWERMGR_QSTO_SIZE	    4
#define COLLECTOR_QSTO_SIZE     8
#define MODMGR_QSTO_SIZE        8
#define GSMMGR_QSTO_SIZE        8
#define GEOMGR_QSTO_SIZE        4
#define DEVSRV_QSTO_SIZE        4
#define COMMMGR_QSTO_SIZE       16
#define USBMGR_QSTO_SIZE        4

#define SIZEOF_EP0_BLOCK        (2 * sizeof(RKH_EVT_T))
#define SIZEOF_EP0STO           (12 * SIZEOF_EP0_BLOCK)

#define SIZEOF_EP1_BLOCK        sizeof(SensorData)
#define SIZEOF_EP1STO           (12 * SIZEOF_EP1_BLOCK)

#define SIZEOF_EP2_BLOCK        sizeof(RmcEvt)
#define SIZEOF_EP2STO           (12 * SIZEOF_EP2_BLOCK)

#define SIZEOF_EP3_BLOCK        sizeof(ModMgrEvt)
#define SIZEOF_EP3STO           (12 * SIZEOF_EP3_BLOCK)

/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
static RKH_EVT_T *PowerMgr_qsto[POWERMGR_QSTO_SIZE];
static RKH_EVT_T *CommMgr_qsto[COMMMGR_QSTO_SIZE];
static RKH_EVT_T *GsmMgr_qsto[GSMMGR_QSTO_SIZE];
static RKH_EVT_T *ModMgr_qsto[MODMGR_QSTO_SIZE];
static RKH_EVT_T *GeoMgr_qsto[GEOMGR_QSTO_SIZE];
static RKH_EVT_T *DeviceMgr_qsto[DEVSRV_QSTO_SIZE];
static RKH_EVT_T *Collector_qsto[COLLECTOR_QSTO_SIZE];
static RKH_EVT_T *UsbMgt_qsto[USBMGR_QSTO_SIZE];
static rui8_t evPool0Sto[SIZEOF_EP0STO],
              evPool1Sto[SIZEOF_EP1STO],
			  evPool2Sto[SIZEOF_EP2STO],
              evPool3Sto[SIZEOF_EP3STO];

static RKH_ROM_STATIC_EVENT(evOpenObj, evOpen);
Device *sprayer, *sampler, *harvest;

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
static void
setupTraceFilters(void)
{
    RKH_FILTER_ON_GROUP(RKH_TRC_ALL_GROUPS);
    RKH_FILTER_ON_EVENT(RKH_TRC_ALL_EVENTS);
    //RKH_FILTER_OFF_EVENT(MODCMD_USR_TRACE);
    /*RKH_FILTER_OFF_GROUP_ALL_EVENTS(RKH_TG_USR); */
    /*RKH_FILTER_OFF_EVENT(RKH_TE_TMR_TOUT);*/
    RKH_FILTER_OFF_EVENT(RKH_TE_SM_STATE);
    RKH_FILTER_OFF_EVENT(RKH_TE_SMA_FIFO);
    RKH_FILTER_OFF_EVENT(RKH_TE_SMA_LIFO);
    RKH_FILTER_OFF_EVENT(RKH_TE_SMA_GET);
    RKH_FILTER_OFF_EVENT(RKH_TE_FWK_ASSERT);
    /*RKH_FILTER_OFF_EVENT(RKH_TE_MP_INIT);
    RKH_FILTER_OFF_EVENT(RKH_TE_MP_PUT);
    RKH_FILTER_OFF_EVENT(RKH_TE_MP_GET);*/
    /*RKH_FILTER_OFF_EVENT(RKH_TE_FWK_EPREG);*/
    /*RKH_FILTER_OFF_EVENT(RKH_TE_FWK_AE);*/
    /*RKH_FILTER_OFF_EVENT(RKH_TE_FWK_GC);*/
    /*RKH_FILTER_OFF_EVENT(RKH_TE_FWK_GCR);*/
    /*RKH_FILTER_OFF_EVENT(RKH_TE_SM_TS_STATE);*/
    /*RKH_FILTER_OFF_EVENT(RKH_TE_SM_DCH);*/
    RKH_FILTER_OFF_SMA(powerMgr);
    /*RKH_FILTER_OFF_SMA(modMgr);*/
    RKH_FILTER_OFF_SMA(gsmMgr);
    RKH_FILTER_OFF_SMA(geoMgr);
    RKH_FILTER_OFF_SMA(deviceMgr);
    RKH_FILTER_OFF_SMA(commMgr);
    RKH_FILTER_OFF_SMA(collector);
    RKH_FILTER_OFF_SMA(usbMgr);
    RKH_FILTER_OFF_ALL_SIGNALS();
    /*RKH_FILTER_ON_SMA(((Collector *)collector)->itsMapping);*/
    /*RKH_FILTER_ON_SIGNAL(evGeo);*/
    /*RKH_FILTER_ON_SIGNAL(evGStatus);*/
    /*RKH_FILTER_ON_SIGNAL(evNoDev);*/
}

/* ---------------------------- Global functions --------------------------- */
int
main(int argc, char *argv[])
{
	RKH_SR_ALLOC();
    InitMode mode;
    MInt flashCheck;

    bsp_init(argc, argv);

    bsp_setAllLeds(1);

    epoch_init();
    init_seqs();
    mTime_init();
    Trace_init();

#if (_DO_FIRST_FORMAT_ == 1)
    ffile_init(CleanAndRestoreMode);
    flashCheck = flash_verify_device();
    if(flashCheck != FLASH_OK)
    {
    	switch(flashCheck)
    	{
    		case FLASH_MUST_POWERCYCLE:
    			bsp_setErrLeds(1);
    			break;
    		case FLASH_UNKNOWN:
    			bsp_setErrLeds(2);
    			break;
    		default:
    			bsp_setErrLeds(3);
				break;
    	}
    	for(;;);
    }

    StatQue_init();
    Config_init();
    Config_setDefault(0);
    bsp_setAllLeds(0);
    for(;;);
#else

#if (_CHECK_FORMATING_START_ == 1)
    mode = (bsp_getDigIn(dIn1) == 0) ? CleanAndRestoreMode : RestoreMode;
#else
    mode = RestoreMode;
#endif
    ffile_init(mode);
    StatQue_init();
    Config_init();

    dIn_init();
    dOut_init();

    rkh_fwk_init();
    rkh_pubsub_init();

    setupTraceFilters();

    RKH_TRC_OPEN();

    GsmMgr_ctor();
    sprayer = sprayer_ctor(0);
    sampler = Sampler_ctor();
    harvest = Harvest_ctor(0);
    Collector_ctor();
    signals_publishSymbols();

    RKH_TR_FWK_ACTOR(&inChg, "inChg");
    RKH_TR_FWK_ACTOR(&sim5320parser, "sim5320parser");
    RKH_TR_FWK_ACTOR(&ubxm8parser, "ubxm8parser");
    RKH_TR_FWK_ACTOR(&main, "main");

    RKH_TR_FWK_EPOOL(0, "evPool0");
    RKH_TR_FWK_EPOOL(1, "evPool1");
    RKH_TR_FWK_EPOOL(2, "evPool2");
    RKH_TR_FWK_EPOOL(3, "evPool3");

    rkh_dynEvt_init();
    rkh_fwk_registerEvtPool(evPool0Sto, SIZEOF_EP0STO, SIZEOF_EP0_BLOCK);
    rkh_fwk_registerEvtPool(evPool1Sto, SIZEOF_EP1STO, SIZEOF_EP1_BLOCK);
    rkh_fwk_registerEvtPool(evPool2Sto, SIZEOF_EP2STO, SIZEOF_EP2_BLOCK);
    rkh_fwk_registerEvtPool(evPool3Sto, SIZEOF_EP3STO, SIZEOF_EP3_BLOCK);

    RKH_SMA_ACTIVATE(powerMgr, PowerMgr_qsto, POWERMGR_QSTO_SIZE, 0, 0);

    RKH_SMA_ACTIVATE(gsmMgr, GsmMgr_qsto, GSMMGR_QSTO_SIZE, 0, 0);
    RKH_SMA_ACTIVATE(modMgr, ModMgr_qsto, MODMGR_QSTO_SIZE, 0, 0);
    RKH_SMA_ACTIVATE(geoMgr, GeoMgr_qsto, GEOMGR_QSTO_SIZE, 0, 0);
    RKH_SMA_ACTIVATE(deviceMgr, DeviceMgr_qsto, DEVSRV_QSTO_SIZE, 0, 0);
    RKH_SMA_ACTIVATE(commMgr, CommMgr_qsto, COMMMGR_QSTO_SIZE, 0, 0);
    RKH_SMA_ACTIVATE(collector, Collector_qsto, COLLECTOR_QSTO_SIZE, 0, 0);
    RKH_SMA_ACTIVATE(usbMgr, UsbMgt_qsto, USBMGR_QSTO_SIZE, 0, 0);

    RKH_SMA_POST_FIFO(gsmMgr, &evOpenObj, &main);
    RKH_SMA_POST_FIFO(deviceMgr, &evOpenObj, &main);
    RKH_SMA_POST_FIFO(usbMgr, &evOpenObj, &main);

    BatChr_init();

    bsp_setAllLeds(0);

    rkh_fwk_enter();

#endif

    return 0;
}

/* ------------------------------ End of file ------------------------------ */
