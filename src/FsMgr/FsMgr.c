/**
 *  \file       FsMgr.c
 *  \brief      Active object implementation.
 */

/* -------------------------- Development history -------------------------- */
/*
 */

/* -------------------------------- Authors -------------------------------- */
/*
 */

/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */
#include "rkhsma.h"
#include "signals.h"
#include "FsMgr.h"
#include "FsMgrAct.h"

/* ----------------------------- Local macros ------------------------------ */
/* ........................ States and pseudostates ........................ */
RKH_CREATE_BASIC_STATE(DiskDisconnected, NULL, NULL, RKH_ROOT, NULL);
RKH_CREATE_BASIC_STATE(FsActive, NULL, NULL, &DiskConnected, NULL);
RKH_CREATE_BASIC_STATE(FsError, NULL, NULL, &DiskConnected, NULL);

RKH_CREATE_COMP_REGION_STATE(DiskConnected, NULL, NULL, RKH_ROOT, &C0, NULL, RKH_NO_HISTORY, NULL, NULL, NULL, NULL);

RKH_CREATE_TRANS_TABLE(DiskDisconnected)
	RKH_TRREG(evDiskConnected, NULL, FsMgr_DiskDisconnectedToDiskConnectedExt1, &DiskConnected),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(DiskConnected)
	RKH_TRREG(evDiskDisconnected, NULL, NULL, &DiskDisconnected),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(FsActive)
	RKH_TRREG(evWrite, NULL, FsMgr_FsActiveToC0Ext3, &C0),
	RKH_TRREG(evRead, NULL, FsMgr_FsActiveToC0Ext4, &C0),
	RKH_TRREG(evOpen, NULL, FsMgr_FsActiveToC0Ext5, &C0),
	RKH_TRREG(evSync, NULL, FsMgr_FsActiveToC0Ext6, &C0),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(FsError)
RKH_END_TRANS_TABLE

RKH_CREATE_CHOICE_STATE(C0);

RKH_CREATE_BRANCH_TABLE(C0)
	RKH_BRANCH(FsMgr_isCondC0ToFsActive9, NULL, &FsActive),
	RKH_BRANCH(ELSE, NULL, &FsError),
RKH_END_BRANCH_TABLE


/* ............................. Active object ............................. */
RKH_SMA_CREATE(FsMgr, fsMgr, 0, HCAL, &DiskDisconnected, NULL, NULL);
RKH_SMA_DEF_PTR(fsMgr);

/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ---------------------------- Global functions --------------------------- */
/* ------------------------------ End of file ------------------------------ */
