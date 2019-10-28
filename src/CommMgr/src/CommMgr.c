/**
 *  \file       CommMgr.c
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
#include "CommMgr.h"
#include "CommMgrAct.h"

/* ----------------------------- Local macros ------------------------------ */
/* ........................ States and pseudostates ........................ */
RKH_CREATE_BASIC_STATE(idle, NULL, NULL, RKH_ROOT, NULL);
RKH_CREATE_BASIC_STATE(WaitSync, CommMgr_enWaitSync, CommMgr_exWaitSync, &active, NULL);
RKH_CREATE_BASIC_STATE(SendingStatus, CommMgr_enSendingStatus, NULL, &active, NULL);
RKH_CREATE_BASIC_STATE(ReceivingAck, CommMgr_enReceivingAck, NULL, &active, NULL);
RKH_CREATE_BASIC_STATE(SendingEndOfHist, CommMgr_enSendingEndOfHist, NULL, &active, NULL);
RKH_CREATE_BASIC_STATE(SendingHist, CommMgr_enSendingHist, NULL, &active, NULL);
RKH_CREATE_BASIC_STATE(EndCycle, NULL, NULL, &active, NULL);

RKH_CREATE_COMP_REGION_STATE(active, NULL, NULL, RKH_ROOT, &WaitSync, NULL, RKH_NO_HISTORY, NULL, NULL, NULL, NULL);

RKH_CREATE_TRANS_TABLE(idle)
	RKH_TRREG(evNetConnected, NULL, CommMgr_idleToactiveExt1, &active),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(active)
	RKH_TRREG(evNetDisconnected, NULL, NULL, &idle),
	RKH_TRINT(evGStatus, NULL, CommMgr_activeToactiveLoc0),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(WaitSync)
	RKH_TRREG(evTout0, NULL, NULL, &SendingStatus),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(SendingStatus)
	RKH_TRREG(evSent, NULL, NULL, &ReceivingAck),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(ReceivingAck)
	RKH_TRREG(evReceived, NULL, CommMgr_ReceivingAckToC1Ext6, &C1),
	RKH_TRREG(evRecvFail, NULL, CommMgr_ReceivingAckToEndCycleExt7, &EndCycle),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(SendingEndOfHist)
	RKH_TRREG(evSent, NULL, NULL, &ReceivingAck),
	RKH_TRREG(evSendFail, NULL, CommMgr_SendingEndOfHistToEndCycleExt9, &EndCycle),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(SendingHist)
	RKH_TRREG(evSent, NULL, CommMgr_SendingHistToC0Ext12, &C0),
	RKH_TRREG(evSendFail, NULL, CommMgr_SendingHistToEndCycleExt13, &EndCycle),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(EndCycle)
RKH_END_TRANS_TABLE

RKH_CREATE_CHOICE_STATE(C0);
RKH_CREATE_CHOICE_STATE(C1);
RKH_CREATE_CHOICE_STATE(C2);
RKH_CREATE_CHOICE_STATE(C3);

RKH_CREATE_BRANCH_TABLE(C0)
	RKH_BRANCH(CommMgr_isCondC0ToSendingHist11, NULL, &SendingHist),
	RKH_BRANCH(ELSE, NULL, &SendingEndOfHist),
RKH_END_BRANCH_TABLE

RKH_CREATE_BRANCH_TABLE(C1)
	RKH_BRANCH(CommMgr_isCondC1ToC215, NULL, &C2),
	RKH_BRANCH(ELSE, CommMgr_C1ToEndCycleExt14, &EndCycle),
RKH_END_BRANCH_TABLE

RKH_CREATE_BRANCH_TABLE(C2)
	RKH_BRANCH(CommMgr_isCondC2ToSendingStatus16, NULL, &SendingStatus),
	RKH_BRANCH(ELSE, NULL, &C3),
RKH_END_BRANCH_TABLE

RKH_CREATE_BRANCH_TABLE(C3)
	RKH_BRANCH(CommMgr_isCondC3ToSendingHist18, CommMgr_C3ToSendingHistExt18, &SendingHist),
	RKH_BRANCH(ELSE, NULL, &EndCycle),
RKH_END_BRANCH_TABLE


/* ............................. Active object ............................. */
RKH_SMA_CREATE(CommMgr, commMgr, 0, HCAL, &idle, CommMgr_ToidleExt0, NULL);
RKH_SMA_DEF_PTR(commMgr);

/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ---------------------------- Global functions --------------------------- */
/* ------------------------------ End of file ------------------------------ */
