/**
 *  \file       ConMgr.c
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
#include "ConMgr.h"
#include "ConMgrAct.h"

/* ----------------------------- Local macros ------------------------------ */
/* ........................ States and pseudostates ........................ */
RKH_CREATE_BASIC_STATE(inactive, NULL, NULL, RKH_ROOT, NULL);
RKH_CREATE_BASIC_STATE(sync, ConMgr_ensync, NULL, &initialize, NULL);
RKH_CREATE_BASIC_STATE(init, ConMgr_eninit, NULL, &initialize, NULL);
RKH_CREATE_BASIC_STATE(pin, ConMgr_enpin, NULL, &initialize, NULL);
RKH_CREATE_BASIC_STATE(setPin, ConMgr_ensetPin, NULL, &initialize, NULL);
RKH_CREATE_BASIC_STATE(enableNetTime, ConMgr_enenableNetTime, NULL, &initialize, NULL);
RKH_CREATE_BASIC_STATE(getImei, ConMgr_engetImei, NULL, &initialize, NULL);
RKH_CREATE_BASIC_STATE(cipShutdown, ConMgr_encipShutdown, NULL, &initialize, NULL);
RKH_CREATE_BASIC_STATE(setManualGet, ConMgr_ensetManualGet, NULL, &initialize, NULL);
RKH_CREATE_BASIC_STATE(failure, ConMgr_enfailure, ConMgr_exfailure, &Active, NULL);
RKH_CREATE_BASIC_STATE(unregistered, ConMgr_enunregistered, ConMgr_exunregistered, &Active, NULL);
RKH_CREATE_BASIC_STATE(localTime, NULL, NULL, &registered, NULL);
RKH_CREATE_BASIC_STATE(waitingServer, ConMgr_enwaitingServer, ConMgr_exwaitingServer, &connecting, NULL);
RKH_CREATE_BASIC_STATE(idle, ConMgr_enidle, ConMgr_exidle, &connected, NULL);
RKH_CREATE_BASIC_STATE(receiving, NULL, NULL, &connected, NULL);
RKH_CREATE_BASIC_STATE(getStatus, NULL, NULL, &connected, NULL);
RKH_CREATE_BASIC_STATE(waitOk, NULL, NULL, &sending, NULL);
RKH_CREATE_BASIC_STATE(waitPrompt, NULL, NULL, &sending, NULL);
RKH_CREATE_BASIC_STATE(restarting, NULL, NULL, &connecting, NULL);
RKH_CREATE_BASIC_STATE(wReopen, ConMgr_enwReopen, ConMgr_exwReopen, &connecting, NULL);
RKH_CREATE_BASIC_STATE(waitRetryConfig, ConMgr_enwaitRetryConfig, ConMgr_exwaitRetryConfig, &registered, NULL);
RKH_CREATE_BASIC_STATE(waitNetClockSync, ConMgr_enwaitNetClockSync, ConMgr_exwaitNetClockSync, &registered, NULL);
RKH_CREATE_BASIC_STATE(setAPN, ConMgr_ensetAPN, NULL, &configure, NULL);
RKH_CREATE_BASIC_STATE(enableNetwork, ConMgr_enenableNetwork, NULL, &configure, NULL);
RKH_CREATE_BASIC_STATE(checkIP, ConMgr_encheckIP, NULL, &configure, NULL);
RKH_CREATE_BASIC_STATE(getOper, ConMgr_engetOper, NULL, &configure, NULL);
RKH_CREATE_BASIC_STATE(waitRetryConnect, ConMgr_enwaitRetryConnect, ConMgr_exwaitRetryConnect, &registered, NULL);
RKH_CREATE_BASIC_STATE(disconnecting, NULL, NULL, &registered, NULL);
RKH_CREATE_BASIC_STATE(SMS, NULL, NULL, &registered, NULL);

RKH_CREATE_COMP_REGION_STATE(Active, NULL, NULL, RKH_ROOT, &initialize, ConMgr_ToinitializeExt4, RKH_NO_HISTORY, NULL, NULL, NULL, NULL);
RKH_CREATE_COMP_REGION_STATE(initialize, NULL, NULL, &Active, &sync, NULL, RKH_NO_HISTORY, NULL, NULL, NULL, NULL);
RKH_CREATE_COMP_REGION_STATE(registered, ConMgr_enregistered, ConMgr_exregistered, &Active, &waitNetClockSync, ConMgr_TowaitingServerExt34, RKH_NO_HISTORY, NULL, NULL, NULL, NULL);
RKH_CREATE_COMP_REGION_STATE(connecting, NULL, NULL, &registered, &waitingServer, ConMgr_TowaitingServerExt34, RKH_NO_HISTORY, NULL, NULL, NULL, NULL);
RKH_CREATE_COMP_REGION_STATE(connected, ConMgr_enconnected, ConMgr_exconnected, &connecting, &idle, NULL, RKH_NO_HISTORY, NULL, NULL, NULL, NULL);
RKH_CREATE_COMP_REGION_STATE(sending, NULL, NULL, &connected, &waitPrompt, NULL, RKH_NO_HISTORY, NULL, NULL, NULL, NULL);
RKH_CREATE_HISTORY_STORAGE(configure);
RKH_CREATE_COMP_REGION_STATE(configure, NULL, NULL, &registered, &getOper, ConMgr_TogetOperExt59, RKH_SHISTORY, NULL, NULL, NULL, RKH_GET_HISTORY_STORAGE(configure));

RKH_CREATE_TRANS_TABLE(inactive)
	RKH_TRREG(evOpen, NULL, ConMgr_inactiveToActiveExt1, &Active),
	RKH_TRINT(evRecv, NULL, ConMgr_inactiveToinactiveLoc0),
	RKH_TRINT(evSend, NULL, ConMgr_inactiveToinactiveLoc1),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(Active)
	RKH_TRREG(evClose, NULL, ConMgr_ActiveToinactiveExt2, &inactive),
	RKH_TRCOMPLETION(NULL, NULL, &inactive),
	RKH_TRINT(evSend, NULL, ConMgr_ActiveToActiveLoc2),
	RKH_TRINT(evRecv, NULL, ConMgr_ActiveToActiveLoc3),
	RKH_TRINT(evSigLevel, NULL, ConMgr_ActiveToActiveLoc4),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(initialize)
	RKH_TRREG(evNoResponse, NULL, NULL, &failure),
	RKH_TRREG(evError, NULL, NULL, &failure),
	RKH_TRCOMPLETION(NULL, NULL, &failure),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(sync)
	RKH_TRREG(evNoResponse, NULL, NULL, &C0),
	RKH_TRREG(evError, NULL, NULL, &C0),
	RKH_TRREG(evOk, NULL, NULL, &init),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(init)
	RKH_TRREG(evOk, NULL, NULL, &pin),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(pin)
	RKH_TRREG(evSimError, NULL, NULL, &initialize_Final),
	RKH_TRREG(evSimPin, NULL, NULL, &setPin),
	RKH_TRREG(evSimReady, NULL, NULL, &enableNetTime),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(setPin)
	RKH_TRREG(evOk, NULL, NULL, &pin),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(enableNetTime)
	RKH_TRREG(evOk, NULL, NULL, &getImei),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(getImei)
	RKH_TRREG(evImei, NULL, NULL, &cipShutdown),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(cipShutdown)
	RKH_TRREG(evOk, NULL, NULL, &setManualGet),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(setManualGet)
	RKH_TRREG(evOk, NULL, NULL, &unregistered),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(failure)
	RKH_TRREG(evTout0, NULL, NULL, &Active),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(unregistered)
	RKH_TRREG(evTout1, NULL, NULL, &failure),
	RKH_TRREG(evReg, NULL, NULL, &registered),
	RKH_TRINT(evRegStatusTout, NULL, ConMgr_unregisteredTounregisteredLoc16),
	RKH_TRINT(evNoReg, NULL, ConMgr_unregisteredTounregisteredLoc17),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(registered)
	RKH_TRREG(evNoReg, NULL, NULL, &unregistered),
	RKH_TRREG(evError, NULL, ConMgr_registeredToregistered_FinalExt25, &registered_Final),
	RKH_TRCOMPLETION(NULL, NULL, &failure),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(localTime)
	RKH_TRREG(evLocalTime, NULL, ConMgr_localTimeToconfigureExt29, &configure),
	RKH_TRREG(evNoResponse, NULL, NULL, &configure),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(connecting)
	RKH_TRREG(evNoResponse, NULL, NULL, &C1),
	RKH_TRREG(evClosed, NULL, NULL, &C1),
	RKH_TRREG(evIPStatus, NULL, NULL, &C1),
	RKH_TRREG(evError, NULL, NULL, &C1),
	RKH_TRREG(evIPStart, NULL, NULL, &configure),
	RKH_TRREG(evIPInitial, NULL, NULL, &configure),
	RKH_TRREG(evIPGprsAct, NULL, NULL, &configure),
	RKH_TRREG(evClose, NULL, ConMgr_connectingTodisconnectingExt33, &disconnecting),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(waitingServer)
	RKH_TRREG(evTout2, NULL, ConMgr_waitingServerTowaitingServerExt35, &waitingServer),
	RKH_TRREG(evConnected, NULL, NULL, &connected),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(connected)
	RKH_TRREG(evRestart, NULL, ConMgr_connectedTorestartingExt37, &restarting),
	RKH_TRREG(evClosed, NULL, NULL, &connecting),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(idle)
	RKH_TRREG(evTout3, NULL, ConMgr_idleTogetStatusExt40, &getStatus),
	RKH_TRREG(evSend, NULL, ConMgr_idleTosendingExt41, &sending),
	RKH_TRREG(evRecv, NULL, ConMgr_idleToreceivingExt42, &receiving),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(receiving)
	RKH_TRREG(evNoResponse, NULL, ConMgr_receivingToidleExt43, &idle),
	RKH_TRREG(evError, NULL, ConMgr_receivingToidleExt43, &idle),
	RKH_TRREG(evOk, NULL, ConMgr_receivingToidleExt44, &idle),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(getStatus)
	RKH_TRREG(evNoResponse, ConMgr_isCondgetStatusToidle45, ConMgr_getStatusToidleExt45, &idle),
	RKH_TRREG(evConnected, NULL, ConMgr_getStatusToidleExt46, &idle),
	RKH_TRINT(evSend, NULL, ConMgr_getStatusTogetStatusLoc22),
	RKH_TRINT(evRecv, NULL, ConMgr_getStatusTogetStatusLoc22),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(sending)
	RKH_TRREG(evNoResponse, NULL, ConMgr_sendingToidleExt47, &idle),
	RKH_TRREG(evError, NULL, ConMgr_sendingToidleExt47, &idle),
	RKH_TRCOMPLETION(NULL, NULL, &idle),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(waitOk)
	RKH_TRREG(evOk, NULL, ConMgr_waitOkTosending_FinalExt50, &sending_Final),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(waitPrompt)
	RKH_TRREG(evOk, NULL, ConMgr_waitPromptTowaitOkExt51, &waitOk),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(restarting)
	RKH_TRREG(evDisconnected, NULL, NULL, &wReopen),
	RKH_TRREG(evNoResponse, NULL, NULL, &wReopen),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(wReopen)
	RKH_TRREG(evTout4, NULL, NULL, &connecting),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(waitRetryConfig)
	RKH_TRREG(evTout5, NULL, ConMgr_waitRetryConfigToHConfigureExt54, &HConfigure),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(waitNetClockSync)
	RKH_TRREG(evNetClockSync, NULL, ConMgr_waitNetClockSyncTolocalTimeExt55, &localTime),
	RKH_TRREG(evTout6, NULL, NULL, &configure),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(configure)
	RKH_TRREG(evNoResponse, NULL, NULL, &C2),
	RKH_TRCOMPLETION(NULL, ConMgr_configureToconnectingExt58, &connecting),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(setAPN)
	RKH_TRREG(evOk, NULL, NULL, &enableNetwork),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(enableNetwork)
	RKH_TRREG(evOk, NULL, NULL, &checkIP),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(checkIP)
	RKH_TRREG(evIPStatus, NULL, NULL, &configure_Final),
	RKH_TRREG(evIP, NULL, NULL, &checkIP),
	RKH_TRREG(evIPInitial, NULL, NULL, &checkIP),
	RKH_TRREG(evIPStart, NULL, NULL, &checkIP),
	RKH_TRREG(evIPGprsAct, NULL, ConMgr_checkIPTocheckIPExt64, &checkIP),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(getOper)
	RKH_TRREG(evOper, NULL, ConMgr_getOperTosetAPNExt65, &setAPN),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(waitRetryConnect)
	RKH_TRREG(evTout7, NULL, ConMgr_waitRetryConnectToconnectingExt68, &connecting),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(disconnecting)
	RKH_TRREG(evDisconnected, NULL, NULL, &Active_Final),
	RKH_TRREG(evNoResponse, NULL, NULL, &Active_Final),
RKH_END_TRANS_TABLE

RKH_CREATE_TRANS_TABLE(SMS)
	RKH_TRREG(evStopSMS, NULL, NULL, &registered_Final),
RKH_END_TRANS_TABLE

RKH_CREATE_CHOICE_STATE(C0);
RKH_CREATE_CHOICE_STATE(C1);
RKH_CREATE_CHOICE_STATE(C2);

RKH_CREATE_BRANCH_TABLE(C0)
	RKH_BRANCH(ConMgr_isCondC0Tosync10, NULL, &sync),
	RKH_BRANCH(ELSE, NULL, &initialize_Final),
RKH_END_BRANCH_TABLE

RKH_CREATE_BRANCH_TABLE(C1)
	RKH_BRANCH(ConMgr_isCondC1TowaitRetryConnect27, NULL, &waitRetryConnect),
	RKH_BRANCH(ELSE, ConMgr_C1Toregistered_FinalExt28, &registered_Final),
RKH_END_BRANCH_TABLE

RKH_CREATE_BRANCH_TABLE(C2)
	RKH_BRANCH(ConMgr_isCondC2TowaitRetryConfig66, NULL, &waitRetryConfig),
	RKH_BRANCH(ELSE, ConMgr_C2Toregistered_FinalExt67, &registered_Final),
RKH_END_BRANCH_TABLE


/* ............................. Active object ............................. */
RKH_SMA_CREATE(ConMgr, conMgr, 0, HCAL, &inactive, ConMgr_ToinactiveExt0, NULL);
RKH_SMA_DEF_PTR(conMgr);

/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ---------------------------- Global functions --------------------------- */
/* ------------------------------ End of file ------------------------------ */
