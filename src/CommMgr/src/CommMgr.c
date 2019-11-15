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
RKH_CREATE_BASIC_STATE(CommMgr_Idle, NULL, NULL, RKH_ROOT, NULL);
RKH_CREATE_BASIC_STATE(CommMgr_WaitSync, CommMgr_enWaitSync, 
                       CommMgr_exWaitSync, &CommMgr_Active, NULL);
RKH_CREATE_BASIC_STATE(CommMgr_SendingStatus, CommMgr_enSendingStatus, 
                       NULL, &CommMgr_Active, NULL);
RKH_CREATE_BASIC_STATE(CommMgr_ReceivingAck, CommMgr_enReceivingAck, 
                       NULL, &CommMgr_Active, NULL);
RKH_CREATE_BASIC_STATE(CommMgr_SendingEndOfHist, CommMgr_enSendingEndOfHist, 
                       NULL, &CommMgr_Active, NULL);
RKH_CREATE_BASIC_STATE(CommMgr_SendingHist, CommMgr_enSendingHist, 
                       NULL, &CommMgr_Active, NULL);
RKH_CREATE_BASIC_STATE(CommMgr_EndCycle, NULL, NULL, &CommMgr_Active, NULL);

RKH_CREATE_COMP_REGION_STATE(CommMgr_Active, NULL, NULL, RKH_ROOT, 
                             &CommMgr_WaitSync, NULL, RKH_NO_HISTORY, 
                             NULL, NULL, NULL, NULL);

RKH_CREATE_TRANS_TABLE(CommMgr_Idle)
	RKH_TRREG(evNetConnected, NULL, CommMgr_IdleToActiveExt1, &CommMgr_Active),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(CommMgr_Active)
	RKH_TRREG(evNetDisconnected, NULL, NULL, &CommMgr_Idle),
	RKH_TRINT(evGStatus, NULL, CommMgr_ActiveToActiveLoc0),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(CommMgr_WaitSync)
	RKH_TRREG(evTout0, NULL, NULL, &CommMgr_SendingStatus),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(CommMgr_SendingStatus)
	RKH_TRREG(evSent, NULL, NULL, &CommMgr_ReceivingAck),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(CommMgr_ReceivingAck)
	RKH_TRREG(evReceived, NULL, CommMgr_ReceivingAckToC1Ext6, &CommMgr_C1),
	RKH_TRREG(evRecvFail, NULL, CommMgr_ReceivingAckToEndCycleExt7, &CommMgr_EndCycle),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(CommMgr_SendingEndOfHist)
	RKH_TRREG(evSent, NULL, NULL, &CommMgr_ReceivingAck),
	RKH_TRREG(evSendFail, NULL, CommMgr_SendingEndOfHistToEndCycleExt9, &CommMgr_EndCycle),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(CommMgr_SendingHist)
	RKH_TRREG(evSent, NULL, CommMgr_SendingHistToC0Ext12, &CommMgr_C0),
	RKH_TRREG(evSendFail, NULL, CommMgr_SendingHistToEndCycleExt13, &CommMgr_EndCycle),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(CommMgr_EndCycle)
RKH_END_TRANS_TABLE

RKH_CREATE_CHOICE_STATE(CommMgr_C0);
RKH_CREATE_CHOICE_STATE(CommMgr_C1);
RKH_CREATE_CHOICE_STATE(CommMgr_C2);
RKH_CREATE_CHOICE_STATE(CommMgr_C3);

RKH_CREATE_BRANCH_TABLE(CommMgr_C0)
	RKH_BRANCH(CommMgr_isCondC0ToSendingHist11, NULL, &CommMgr_SendingHist),
	RKH_BRANCH(ELSE, NULL, &CommMgr_SendingEndOfHist),
RKH_END_BRANCH_TABLE

RKH_CREATE_BRANCH_TABLE(CommMgr_C1)
	RKH_BRANCH(CommMgr_isCondC1ToC215, NULL, &CommMgr_C2),
	RKH_BRANCH(ELSE, CommMgr_C1ToEndCycleExt14, &CommMgr_EndCycle),
RKH_END_BRANCH_TABLE

RKH_CREATE_BRANCH_TABLE(CommMgr_C2)
	RKH_BRANCH(CommMgr_isCondC2ToSendingStatus16, NULL, &CommMgr_SendingStatus),
	RKH_BRANCH(ELSE, NULL, &CommMgr_C3),
RKH_END_BRANCH_TABLE

RKH_CREATE_BRANCH_TABLE(CommMgr_C3)
	RKH_BRANCH(CommMgr_isCondC3ToSendingHist18, CommMgr_C3ToSendingHistExt18, &CommMgr_SendingHist),
	RKH_BRANCH(ELSE, NULL, &CommMgr_EndCycle),
RKH_END_BRANCH_TABLE


/* ............................. Active object ............................. */
RKH_SMA_CREATE(CommMgr, commMgr, 0, HCAL, &CommMgr_Idle, CommMgr_ToIdleExt0, NULL);
RKH_SMA_DEF_PTR(commMgr);

/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ---------------------------- Global functions --------------------------- */
/* ------------------------------ End of file ------------------------------ */
