/**
 *  \file       FsMgr.c
 *  \brief      Active object's action implementations.
 */

/* -------------------------- Development history -------------------------- */
/*
 */

/* -------------------------------- Authors -------------------------------- */
/*
 */

/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */
#include "rkh.h"
#include "signals.h"
#include "FsMgr.h"
#include "FsMgrAct.h"
#include "bsp.h"
#include "FsMgrActRequired.h"

/* ----------------------------- Local macros ------------------------------ */

/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ............................ Effect actions ............................. */
void 
FsMgr_DiskDisconnectedToDiskConnectedExt1(FsMgr *const me, RKH_EVT_T *pe)
{
	fsMount();
}

void 
FsMgr_FsActiveToC0Ext3(FsMgr *const me, RKH_EVT_T *pe)
{
	fsWrite();
}

void 
FsMgr_FsActiveToC0Ext4(FsMgr *const me, RKH_EVT_T *pe)
{
	fsRead();
}

void 
FsMgr_FsActiveToC0Ext5(FsMgr *const me, RKH_EVT_T *pe)
{
	fsOpen();
}

void 
FsMgr_FsActiveToC0Ext6(FsMgr *const me, RKH_EVT_T *pe)
{
	fsSync();
}

/* ............................. Entry actions ............................. */
/* ............................. Exit actions .............................. */
/* ................................ Guards ................................. */
rbool_t 
FsMgr_isCondC0ToFsActive9(FsMgr *const me, RKH_EVT_T *pe)
{
	return ((me->result == me->FR_OK)) ? true : false;
}

/* ---------------------------- Global functions --------------------------- */
/* ------------------------------ End of file ------------------------------ */
