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
#include "priorities.h"
#include "signals.h"
#include "CommMgr.h"
#include "CommMgrAct.h"

/* ----------------------------- Local macros ------------------------------ */
/* ........................ States and pseudostates ........................ */
RKH_CREATE_BASIC_STATE(Idle, CommMgr_enIdle, NULL, RKH_ROOT, NULL);
RKH_CREATE_BASIC_STATE(WaitSync, CommMgr_enWaitSync, CommMgr_exWaitSync, &Active, NULL);
RKH_CREATE_BASIC_STATE(SendingStatus, CommMgr_enSendingStatus, NULL, &Current, NULL);
RKH_CREATE_BASIC_STATE(ReceivingStatusAck, CommMgr_enReceivingStatusAck, NULL, &Current, NULL);
RKH_CREATE_BASIC_STATE(SendingEndOfHist, CommMgr_enSendingEndOfHist, NULL, &History, NULL);
RKH_CREATE_BASIC_STATE(SendingHist, CommMgr_enSendingHist, NULL, &History, NULL);
RKH_CREATE_BASIC_STATE(ReceivingMsgAck, CommMgr_enReceivingMsgAck, NULL, &History, NULL);
RKH_CREATE_BASIC_STATE(SendingStartOfHist, CommMgr_enSendingStartOfHist, NULL, &History, NULL);
RKH_CREATE_BASIC_STATE(SendingCmdAck, CommMgr_enSendingCmdAck, NULL, &Command, NULL);
RKH_CREATE_BASIC_STATE(Disconnected, CommMgr_enDisconnected, NULL, RKH_ROOT, NULL);

RKH_CREATE_COMP_REGION_STATE(Active, NULL, NULL, RKH_ROOT, &Current, NULL, RKH_NO_HISTORY, NULL, NULL, NULL, NULL);
RKH_CREATE_COMP_REGION_STATE(Current, NULL, NULL, &Active, &SendingStatus, NULL, RKH_NO_HISTORY, NULL, NULL, NULL, NULL);
RKH_CREATE_COMP_REGION_STATE(History, NULL, NULL, &Active, &C1, CommMgr_ToC1Ext16, RKH_NO_HISTORY, NULL, NULL, NULL, NULL);
RKH_CREATE_COMP_REGION_STATE(Command, NULL, NULL, &Active, &SendingCmdAck, NULL, RKH_NO_HISTORY, NULL, NULL, NULL, NULL);

RKH_CREATE_TRANS_TABLE(Idle)
	RKH_TRREG(evNetConnected, NULL, NULL, &Active),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(Active)
	RKH_TRREG(evNetDisconnected, NULL, NULL, &Disconnected),
	RKH_TRINT(evGStatus, NULL, CommMgr_ActiveToActiveLoc0),
	RKH_TRREG(evTerminate, NULL, NULL, &CommMgrFinal),
    RKH_TRCOMPLETION(NULL, NULL, &CommMgrFinal),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(WaitSync)
	RKH_TRREG(evTout0, NULL, NULL, &Current),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(Current)
	RKH_TRREG(evSendFail, NULL, CommMgr_CurrentToWaitSyncExt5, &WaitSync),
	RKH_TRREG(evRecvFail, NULL, CommMgr_CurrentToWaitSyncExt6, &WaitSync),
	RKH_TRCOMPLETION(NULL, NULL, &WaitSync),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(SendingStatus)
	RKH_TRREG(evSent, NULL, CommMgr_SendingStatusToReceivingiStatusAckExt37, &ReceivingStatusAck),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(ReceivingStatusAck)
	RKH_TRREG(evReceived, NULL, CommMgr_ReceivingStatusAckToC0Ext9, &C0),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(History)
	RKH_TRREG(evRecvFail, NULL, CommMgr_HistoryToWaitSyncExt13, &WaitSync),
	RKH_TRREG(evSendFail, NULL, CommMgr_HistoryToWaitSyncExt14, &WaitSync),
    RKH_TRCOMPLETION(NULL, NULL, &WaitSync),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(SendingEndOfHist)
	RKH_TRREG(evSent, NULL, CommMgr_SendingEndOfHistToReceivingMsgAckExt34, &ReceivingMsgAck),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(SendingHist)
	RKH_TRREG(evSent, NULL, CommMgr_SendingHistToC2Ext18, &C2),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(ReceivingMsgAck)
	RKH_TRREG(evReceived, NULL, CommMgr_ReceivingMsgAckToC3Ext19, &C3),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(SendingStartOfHist)
	RKH_TRREG(evSent, NULL, CommMgr_SendingStartOfHistToSendingHistExt30, &SendingHist),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(Command)
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(SendingCmdAck)
	RKH_TRREG(evSent, NULL, NULL, &C7),
	RKH_TRREG(evSendFail, NULL, NULL, &C7),
	RKH_TRINT(evTerminate, NULL, NULL),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(Disconnected)
	RKH_TRREG(evNetConnected, NULL, NULL, &Active),
RKH_END_TRANS_TABLE

RKH_CREATE_CHOICE_STATE(C0);
RKH_CREATE_CHOICE_STATE(C1);
RKH_CREATE_CHOICE_STATE(C2);
RKH_CREATE_CHOICE_STATE(C3);
RKH_CREATE_CHOICE_STATE(C4);
RKH_CREATE_CHOICE_STATE(C5);
RKH_CREATE_CHOICE_STATE(C6);
RKH_CREATE_CHOICE_STATE(C7);
RKH_CREATE_CHOICE_STATE(C8);

RKH_CREATE_BRANCH_TABLE(C0)
	RKH_BRANCH(CommMgr_isCondC0ToHistory11, NULL, &History),
	RKH_BRANCH(CommMgr_isCondC0ToC628, CommMgr_C0ToC6Ext35, &C6),
	RKH_BRANCH(CommMgr_isCondC0ToCommand42, NULL, &Command),
	RKH_BRANCH(ELSE, CommMgr_C0ToCurrentFinalExt12, &CurrentFinal),
RKH_END_BRANCH_TABLE

RKH_CREATE_BRANCH_TABLE(C1)
	RKH_BRANCH(CommMgr_isCondC1ToSendingStartOfHist20, NULL, &SendingStartOfHist),
	RKH_BRANCH(ELSE, NULL, &HistoryFinal),
RKH_END_BRANCH_TABLE

RKH_CREATE_BRANCH_TABLE(C2)
	RKH_BRANCH(CommMgr_isCondC2ToSendingEndOfHist23, NULL, &SendingEndOfHist),
	RKH_BRANCH(ELSE, CommMgr_C2ToSendingHistExt31, &SendingHist),
RKH_END_BRANCH_TABLE

RKH_CREATE_BRANCH_TABLE(C3)
	RKH_BRANCH(CommMgr_isCondC3ToC425, CommMgr_C3ToC4Ext25, &C4),
	RKH_BRANCH(CommMgr_isCondC3ToC529, CommMgr_C3ToC5Ext32, &C5),
	RKH_BRANCH(CommMgr_isCondC3ToCommand43, NULL, &Command),
	RKH_BRANCH(ELSE, CommMgr_C3ToHistoryFinalExt24, &HistoryFinal),
RKH_END_BRANCH_TABLE

RKH_CREATE_BRANCH_TABLE(C4)
	RKH_BRANCH(CommMgr_isCondC4ToCurrent26, NULL, &Current),
	RKH_BRANCH(ELSE, CommMgr_C4ToC1Ext27, &C1),
RKH_END_BRANCH_TABLE

RKH_CREATE_BRANCH_TABLE(C5)
	RKH_BRANCH(CommMgr_isCondC5ToHistoryFinal33, CommMgr_C5ToHistoryFinalExt38, &HistoryFinal),
	RKH_BRANCH(ELSE, NULL, &ReceivingMsgAck),
RKH_END_BRANCH_TABLE

RKH_CREATE_BRANCH_TABLE(C6)
	RKH_BRANCH(CommMgr_isCondC6ToCurrentFinal36, CommMgr_C6ToCurrentFinalExt39, &CurrentFinal),
	RKH_BRANCH(ELSE, NULL, &ReceivingStatusAck),
RKH_END_BRANCH_TABLE

RKH_CREATE_BRANCH_TABLE(C7)
	RKH_BRANCH(CommMgr_isCondC7ToActiveFinal40, CommMgr_C7ToActiveFinalExt41, &ActiveFinal),
	RKH_BRANCH(ELSE, NULL, &C8),
RKH_END_BRANCH_TABLE

RKH_CREATE_BRANCH_TABLE(C8)
	RKH_BRANCH(CommMgr_isCondC8ToHistory44, NULL, &History),
	RKH_BRANCH(ELSE, NULL, &C4),
RKH_END_BRANCH_TABLE

RKH_CREATE_FINAL_STATE(CommMgrFinal, RKH_ROOT);

/* ............................. Active object ............................. */
RKH_SMA_CREATE(CommMgr, commMgr, CommMgrPrio, HCAL, &Idle, CommMgr_ToIdleExt0, NULL);
RKH_SMA_DEF_PTR(commMgr);

/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ---------------------------- Global functions --------------------------- */
/* ------------------------------ End of file ------------------------------ */
