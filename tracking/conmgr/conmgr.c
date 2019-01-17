/**
 *  \file       conmgr.c
 *  \brief      Implementation of connection and protocol manager.
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2018.05.15  DaBa  v1.0.01  Initial version
 *  2018.05.07  LeFr  v1.0.00  Initial version
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  DaBa  Dario Bali�a      db@vortexmakes.com
 *  LeFr  Leandro Francucci lf@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */
#include "rkh.h"
#include "rkhfwk_pubsub.h"
#include <string.h>
#include "conmgr.h"
#include "modpwr.h"
#include "modmgr.h"
#include "modcmd.h"
#include "signals.h"
#include "topics.h"
#include "rtime.h"
#include "bsp.h"

/* ----------------------------- Local macros ------------------------------ */
/* ......................... Declares active object ........................ */
typedef struct ConMgr ConMgr;

/* ................... Declares states and pseudostates .................... */
RKH_DCLR_BASIC_STATE ConMgr_inactive, ConMgr_sync,
                ConMgr_init, ConMgr_pin, ConMgr_setPin, ConMgr_enableNetTime,
                ConMgr_getImei, ConMgr_cipShutdown, ConMgr_setManualGet,
                ConMgr_waitReg, ConMgr_unregistered, ConMgr_failure,
                ConMgr_waitNetClockSync, ConMgr_localTime,
                ConMgr_setAPN, ConMgr_enableGPRS,
                ConMgr_checkIP, ConMgr_waitRetryConfig, ConMgr_waitingServer,
                ConMgr_idle, ConMgr_waitPrompt, ConMgr_waitOk,
                ConMgr_receiving, ConMgr_restarting, ConMgr_wReopen,
                ConMgr_waitRetryConnect, ConMgr_disconnecting;

RKH_DCLR_COMP_STATE ConMgr_active, ConMgr_initialize, ConMgr_registered,
                    ConMgr_configure, ConMgr_connecting, ConMgr_connected,
                    ConMgr_sending;
RKH_DCLR_FINAL_STATE ConMgr_activeFinal, ConMgr_initializeFinal, 
                     ConMgr_configureFinal, ConMgr_sendingFinal;
RKH_DCLR_COND_STATE ConMgr_checkSyncTry, ConMgr_checkConfigTry,
                    ConMgr_checkConnectTry;

/* ........................ Declares initial action ........................ */
static void init(ConMgr *const me, RKH_EVT_T *pe);

/* ........................ Declares effect actions ........................ */
static void open(ConMgr *const me, RKH_EVT_T *pe);
static void close(ConMgr *const me, RKH_EVT_T *pe);
static void initializeInit(ConMgr *const me, RKH_EVT_T *pe);
static void storeImei(ConMgr *const me, RKH_EVT_T *pe);
static void localTimeGet(ConMgr *const me, RKH_EVT_T *pe);
static void rtimeSync(ConMgr *const me, RKH_EVT_T *pe);
static void configureInit(ConMgr *const me, RKH_EVT_T *pe);
static void configTry(ConMgr *const me, RKH_EVT_T *pe);
static void requestIp(ConMgr *const me, RKH_EVT_T *pe);
static void connectInit(ConMgr *const me, RKH_EVT_T *pe);
static void connectTry(ConMgr *const me, RKH_EVT_T *pe);
static void socketOpen(ConMgr *const me, RKH_EVT_T *pe);
static void socketClose(ConMgr *const me, RKH_EVT_T *pe);
static void readData(ConMgr *const me, RKH_EVT_T *pe);
static void sendRequest(ConMgr *const me, RKH_EVT_T *pe);
static void flushData(ConMgr *const me, RKH_EVT_T *pe);
static void sendOk(ConMgr *const me, RKH_EVT_T *pe);
static void recvOk(ConMgr *const me, RKH_EVT_T *pe);
static void sendFail(ConMgr *const me, RKH_EVT_T *pe);
static void recvFail(ConMgr *const me, RKH_EVT_T *pe);
static void resetParser(ConMgr *const me, RKH_EVT_T *pe);

/* ......................... Declares entry actions ........................ */
static void sendSync(ConMgr *const me);
static void sendInit(ConMgr *const me);
static void checkPin(ConMgr *const me);
static void setPin(ConMgr *const me);
static void netTimeEnable(ConMgr *const me);
static void getImei(ConMgr *const me);
static void cipShutdown(ConMgr *const me);
static void unregEntry(ConMgr *const me);
static void failureEntry(ConMgr *const me);
static void setupManualGet(ConMgr *const me);
static void waitNetClockSyncEntry(ConMgr *const me);
static void waitRetryConfigEntry(ConMgr *const me);
static void setupAPN(ConMgr *const me);
static void startGPRS(ConMgr *const me);
static void wReopenEntry(ConMgr *const me);
static void waitRetryConnEntry(ConMgr *const me);
static void getConnStatus(ConMgr *const me);
static void connectingEntry(ConMgr *const me);
static void socketConnected(ConMgr *const me);
static void idleEntry(ConMgr *const me);

/* ......................... Declares exit actions ......................... */
static void unregExit(ConMgr *const me);
static void waitNetClockSyncExit(ConMgr *const me);
static void wReopenExit(ConMgr *const me);
static void waitRetryConnExit(ConMgr *const me);
static void failureExit(ConMgr *const me);
static void connectingExit(ConMgr *const me);
static void socketDisconnected(ConMgr *const me);
static void idleExit(ConMgr *const me);

/* ............................ Declares guards ............................ */
rbool_t checkSyncTry(ConMgr *const me, RKH_EVT_T *pe);
rbool_t checkConfigTry(ConMgr *const me, RKH_EVT_T *pe);
rbool_t checkConnectTry(ConMgr *const me, RKH_EVT_T *pe);

/* ........................ States and pseudostates ........................ */
RKH_CREATE_BASIC_STATE(ConMgr_inactive, NULL, NULL, RKH_ROOT, NULL);
RKH_CREATE_TRANS_TABLE(ConMgr_inactive)
    RKH_TRINT(evSend, NULL, sendFail),
    RKH_TRINT(evRecv, NULL, recvFail),
    RKH_TRREG(evOpen, NULL, open, &ConMgr_active),
RKH_END_TRANS_TABLE

RKH_CREATE_COMP_REGION_STATE(ConMgr_active, NULL, NULL, RKH_ROOT, 
                             &ConMgr_initialize, NULL,
                             RKH_NO_HISTORY, NULL, NULL, NULL, NULL);
RKH_CREATE_TRANS_TABLE(ConMgr_active)
    RKH_TRINT(evSend, NULL, sendFail),
    RKH_TRINT(evRecv, NULL, recvFail),
    RKH_TRCOMPLETION(NULL, NULL, &ConMgr_inactive),
    RKH_TRREG(evClose, NULL, close, &ConMgr_inactive),
RKH_END_TRANS_TABLE

RKH_CREATE_COMP_REGION_STATE(ConMgr_initialize, NULL, NULL, &ConMgr_active, 
                             &ConMgr_sync, initializeInit,
                             RKH_NO_HISTORY, NULL, NULL, NULL, NULL);
RKH_CREATE_TRANS_TABLE(ConMgr_initialize)
    RKH_TRCOMPLETION(NULL, NULL, &ConMgr_unregistered),
    RKH_TRREG(evNoResponse, NULL, NULL, &ConMgr_failure),
RKH_END_TRANS_TABLE

RKH_CREATE_BASIC_STATE(ConMgr_sync, sendSync, NULL, &ConMgr_initialize, NULL);
RKH_CREATE_TRANS_TABLE(ConMgr_sync)
    RKH_TRREG(evOk,         NULL, NULL, &ConMgr_init),
    RKH_TRREG(evNoResponse, NULL, NULL, &ConMgr_checkSyncTry),
RKH_END_TRANS_TABLE

RKH_CREATE_COND_STATE(ConMgr_checkSyncTry);
RKH_CREATE_BRANCH_TABLE(ConMgr_checkSyncTry)
    RKH_BRANCH(checkSyncTry,   NULL,   &ConMgr_sync),
    RKH_BRANCH(ELSE,           NULL,   &ConMgr_failure),
RKH_END_BRANCH_TABLE

RKH_CREATE_BASIC_STATE(ConMgr_init, sendInit, NULL, &ConMgr_initialize, NULL);
RKH_CREATE_TRANS_TABLE(ConMgr_init)
    RKH_TRREG(evOk,         NULL, NULL, &ConMgr_pin),
RKH_END_TRANS_TABLE

RKH_CREATE_BASIC_STATE(ConMgr_pin, checkPin, NULL, &ConMgr_initialize, NULL);
RKH_CREATE_TRANS_TABLE(ConMgr_pin)
    RKH_TRREG(evSimPin,     NULL, NULL, &ConMgr_setPin),
    RKH_TRREG(evSimError,   NULL, NULL, &ConMgr_failure),
    RKH_TRREG(evSimReady,   NULL, NULL, &ConMgr_enableNetTime),
RKH_END_TRANS_TABLE

RKH_CREATE_BASIC_STATE(ConMgr_setPin, setPin, NULL, &ConMgr_initialize, NULL);
RKH_CREATE_TRANS_TABLE(ConMgr_setPin)
    RKH_TRREG(evOk,         NULL, NULL,   &ConMgr_pin),
RKH_END_TRANS_TABLE

RKH_CREATE_BASIC_STATE(ConMgr_enableNetTime, netTimeEnable, NULL, 
                                                    &ConMgr_initialize, NULL);
RKH_CREATE_TRANS_TABLE(ConMgr_enableNetTime)
    RKH_TRREG(evOk,         NULL, NULL, &ConMgr_getImei),
RKH_END_TRANS_TABLE

RKH_CREATE_BASIC_STATE(ConMgr_getImei, getImei, NULL, &ConMgr_initialize, NULL);
RKH_CREATE_TRANS_TABLE(ConMgr_getImei)
    RKH_TRREG(evImei,       NULL, storeImei, &ConMgr_cipShutdown),
RKH_END_TRANS_TABLE

RKH_CREATE_BASIC_STATE(ConMgr_cipShutdown, cipShutdown, NULL, &ConMgr_initialize, NULL);
RKH_CREATE_TRANS_TABLE(ConMgr_cipShutdown)
    RKH_TRREG(evOk,         NULL, NULL, &ConMgr_setManualGet),
RKH_END_TRANS_TABLE

RKH_CREATE_BASIC_STATE(ConMgr_setManualGet, setupManualGet, NULL, 
                                                    &ConMgr_initialize, NULL);
RKH_CREATE_TRANS_TABLE(ConMgr_setManualGet)
    RKH_TRREG(evOk,         NULL, NULL, &ConMgr_initializeFinal),
RKH_END_TRANS_TABLE

RKH_CREATE_COMP_REGION_STATE(ConMgr_registered, NULL, NULL, &ConMgr_active, 
                             &ConMgr_waitNetClockSync, NULL,
                             RKH_NO_HISTORY, NULL, NULL, NULL, NULL);
RKH_CREATE_TRANS_TABLE(ConMgr_registered)
    RKH_TRREG(evNoReg, NULL, NULL,   &ConMgr_unregistered),
RKH_END_TRANS_TABLE

RKH_CREATE_BASIC_STATE(ConMgr_unregistered, unregEntry, unregExit,
                            &ConMgr_active, NULL);
RKH_CREATE_TRANS_TABLE(ConMgr_unregistered)
    RKH_TRREG(evTimeout,  NULL,    NULL, &ConMgr_failure),
    RKH_TRREG(evReg, NULL, NULL,   &ConMgr_registered),
RKH_END_TRANS_TABLE

RKH_CREATE_BASIC_STATE(ConMgr_failure, failureEntry, failureExit, &ConMgr_active, NULL);
RKH_CREATE_TRANS_TABLE(ConMgr_failure)
    RKH_TRREG(evTimeout, NULL,  NULL, &ConMgr_active),
RKH_END_TRANS_TABLE

RKH_CREATE_BASIC_STATE(ConMgr_waitNetClockSync, 
                            waitNetClockSyncEntry, waitNetClockSyncExit,
                            &ConMgr_registered, NULL);
RKH_CREATE_TRANS_TABLE(ConMgr_waitNetClockSync)
    RKH_TRREG(evTimeout,       NULL, localTimeGet, &ConMgr_localTime),
    RKH_TRREG(evNetClockSync,  NULL, localTimeGet, &ConMgr_localTime),
RKH_END_TRANS_TABLE

RKH_CREATE_BASIC_STATE(ConMgr_localTime, NULL, NULL, &ConMgr_registered, NULL);
RKH_CREATE_TRANS_TABLE(ConMgr_localTime)
    RKH_TRREG(evLocalTime,     NULL, rtimeSync,  &ConMgr_configure),
    RKH_TRREG(evNoResponse,    NULL, NULL,       &ConMgr_configure),
RKH_END_TRANS_TABLE

RKH_CREATE_HISTORY_STORAGE(ConMgr_configure);
RKH_CREATE_COMP_REGION_STATE(ConMgr_configure, NULL, NULL, &ConMgr_registered, 
                             &ConMgr_setAPN, configureInit,
                             RKH_SHISTORY, NULL, NULL, NULL,
                             RKH_GET_HISTORY_STORAGE(ConMgr_configure));
RKH_CREATE_TRANS_TABLE(ConMgr_configure)
    RKH_TRCOMPLETION(NULL, connectInit, &ConMgr_connecting),
    RKH_TRREG(evNoResponse, NULL, NULL, &ConMgr_checkConfigTry),
RKH_END_TRANS_TABLE

RKH_CREATE_BASIC_STATE(ConMgr_setAPN, setupAPN, NULL, 
                                                    &ConMgr_configure, NULL);
RKH_CREATE_TRANS_TABLE(ConMgr_setAPN)
    RKH_TRREG(evOk,         NULL, NULL, &ConMgr_enableGPRS),
RKH_END_TRANS_TABLE

RKH_CREATE_BASIC_STATE(ConMgr_enableGPRS, startGPRS, NULL, 
                                                    &ConMgr_configure, NULL);
RKH_CREATE_TRANS_TABLE(ConMgr_enableGPRS)
    RKH_TRREG(evOk,         NULL, NULL, &ConMgr_checkIP),
RKH_END_TRANS_TABLE

RKH_CREATE_BASIC_STATE(ConMgr_checkIP, getConnStatus, NULL, 
                                                    &ConMgr_configure, NULL);
RKH_CREATE_TRANS_TABLE(ConMgr_checkIP)
    RKH_TRREG(evIPGprsAct,  requestIp,    NULL, &ConMgr_checkIP),
    RKH_TRREG(evIP,         NULL,         NULL, &ConMgr_checkIP),
    RKH_TRREG(evIPInitial,  NULL,         NULL, &ConMgr_checkIP),
    RKH_TRREG(evIPStart,    NULL,         NULL, &ConMgr_checkIP),
    RKH_TRREG(evIPStatus,   NULL,         NULL, &ConMgr_configureFinal),
RKH_END_TRANS_TABLE

RKH_CREATE_COND_STATE(ConMgr_checkConfigTry);
RKH_CREATE_BRANCH_TABLE(ConMgr_checkConfigTry)
    RKH_BRANCH(checkConfigTry,   NULL, &ConMgr_waitRetryConfig),
    RKH_BRANCH(ELSE,           NULL,   &ConMgr_failure),
RKH_END_BRANCH_TABLE

RKH_CREATE_BASIC_STATE(ConMgr_waitRetryConfig, waitRetryConfigEntry, NULL,
                                                    &ConMgr_registered, NULL);
RKH_CREATE_TRANS_TABLE(ConMgr_waitRetryConfig)
    RKH_TRREG(evTimeout,  NULL,    configTry, &ConMgr_configureHist),
RKH_END_TRANS_TABLE

RKH_CREATE_COMP_REGION_STATE(ConMgr_connecting, NULL, NULL, 
                             &ConMgr_registered, &ConMgr_waitingServer,
                             socketOpen,
                             RKH_NO_HISTORY, NULL, NULL, NULL, NULL);
RKH_CREATE_TRANS_TABLE(ConMgr_connecting)
    RKH_TRREG(evClose,  NULL,  socketClose, &ConMgr_disconnecting),
    RKH_TRREG(evNoResponse, NULL, NULL, &ConMgr_checkConnectTry),
    RKH_TRREG(evClosed,     NULL,  NULL, &ConMgr_checkConnectTry),
RKH_END_TRANS_TABLE

RKH_CREATE_BASIC_STATE(ConMgr_waitingServer, connectingEntry, connectingExit,
                                                    &ConMgr_connecting, NULL);
RKH_CREATE_TRANS_TABLE(ConMgr_waitingServer)
    RKH_TRREG(evTimeout,    NULL,  getConnStatus, &ConMgr_waitingServer),
    RKH_TRREG(evConnected,  NULL,  NULL, &ConMgr_connected),
RKH_END_TRANS_TABLE

RKH_CREATE_COMP_REGION_STATE(ConMgr_connected, 
                             socketConnected, socketDisconnected, 
                             &ConMgr_connecting, &ConMgr_idle, NULL,
                             RKH_NO_HISTORY, NULL, NULL, NULL, NULL);
RKH_CREATE_TRANS_TABLE(ConMgr_connected)
    RKH_TRREG(evClosed,       NULL,  NULL,        &ConMgr_connecting),
    RKH_TRREG(evRestart,      NULL,  socketClose, &ConMgr_restarting),
RKH_END_TRANS_TABLE

RKH_CREATE_BASIC_STATE(ConMgr_idle, idleEntry, idleExit,
                                                &ConMgr_connected, NULL);
RKH_CREATE_TRANS_TABLE(ConMgr_idle)
    RKH_TRINT(evNoResponse, NULL, resetParser),
    RKH_TRREG(evTimeout, NULL,  getConnStatus,  &ConMgr_idle),
    RKH_TRREG(evSend,    NULL,  sendRequest,    &ConMgr_sending),
    RKH_TRREG(evRecv,    NULL,  readData,       &ConMgr_receiving),
RKH_END_TRANS_TABLE

RKH_CREATE_COMP_REGION_STATE(ConMgr_sending, NULL, NULL, 
                             &ConMgr_connected, &ConMgr_waitPrompt, NULL,
                             RKH_NO_HISTORY, NULL, NULL, NULL, NULL);
RKH_CREATE_TRANS_TABLE(ConMgr_sending)
    RKH_TRCOMPLETION(NULL, NULL, &ConMgr_idle),
    RKH_TRREG(evNoResponse, NULL, sendFail, &ConMgr_idle),
RKH_END_TRANS_TABLE

RKH_CREATE_BASIC_STATE(ConMgr_waitPrompt, NULL, NULL, &ConMgr_sending, NULL);
RKH_CREATE_TRANS_TABLE(ConMgr_waitPrompt)
    RKH_TRREG(evOk, NULL,  flushData, &ConMgr_waitOk),
RKH_END_TRANS_TABLE

RKH_CREATE_BASIC_STATE(ConMgr_waitOk, NULL, NULL, &ConMgr_sending, NULL);
RKH_CREATE_TRANS_TABLE(ConMgr_waitOk)
    RKH_TRREG(evOk, NULL,  sendOk, &ConMgr_sendingFinal),    
RKH_END_TRANS_TABLE

RKH_CREATE_BASIC_STATE(ConMgr_receiving, NULL, NULL, &ConMgr_connected, NULL);
RKH_CREATE_TRANS_TABLE(ConMgr_receiving)
    RKH_TRREG(evOk, NULL,  recvOk, &ConMgr_idle),
	RKH_TRREG(evNoResponse, NULL, recvFail, &ConMgr_idle),
RKH_END_TRANS_TABLE

RKH_CREATE_BASIC_STATE(ConMgr_restarting, NULL, NULL, &ConMgr_connecting, NULL);
RKH_CREATE_TRANS_TABLE(ConMgr_restarting)
    RKH_TRREG(evDisconnected, NULL,  NULL, &ConMgr_wReopen),
	RKH_TRREG(evNoResponse, NULL, NULL, &ConMgr_wReopen),
RKH_END_TRANS_TABLE

RKH_CREATE_BASIC_STATE(ConMgr_wReopen, wReopenEntry, wReopenExit, 
                                                        &ConMgr_connecting, NULL);
RKH_CREATE_TRANS_TABLE(ConMgr_wReopen)
    RKH_TRREG(evTimeout, NULL,  NULL, &ConMgr_connecting),
RKH_END_TRANS_TABLE

RKH_CREATE_COND_STATE(ConMgr_checkConnectTry);
RKH_CREATE_BRANCH_TABLE(ConMgr_checkConnectTry)
    RKH_BRANCH(checkConnectTry,  NULL, &ConMgr_waitRetryConnect),
    RKH_BRANCH(ELSE,             NULL, &ConMgr_failure),
RKH_END_BRANCH_TABLE

RKH_CREATE_BASIC_STATE(ConMgr_waitRetryConnect,
                                        waitRetryConnEntry, waitRetryConnExit,
                                        &ConMgr_connecting, NULL);
RKH_CREATE_TRANS_TABLE(ConMgr_waitRetryConnect)
    RKH_TRREG(evTimeout, NULL,  connectTry, &ConMgr_connecting),
RKH_END_TRANS_TABLE

RKH_CREATE_BASIC_STATE(ConMgr_disconnecting, NULL, NULL,
                                                    &ConMgr_registered, NULL);
RKH_CREATE_TRANS_TABLE(ConMgr_disconnecting)
    RKH_TRREG(evDisconnected,  NULL,    NULL, &ConMgr_activeFinal),
    RKH_TRREG(evNoResponse,    NULL,    NULL, &ConMgr_activeFinal),
RKH_END_TRANS_TABLE

/* ............................. Active object ............................. */
struct ConMgr
{
    RKH_SMA_T ao;       /* base structure */
    RKH_TMR_T timer;
    rui8_t retryCount; 
    SendEvt *psend;
};

RKH_SMA_CREATE(ConMgr, conMgr, 1, HCAL, &ConMgr_inactive, init, NULL);
RKH_SMA_DEF_PTR(conMgr);

/* ------------------------------- Constants ------------------------------- */
#define IMEI_SNR_OFFSET  8

/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
static RKH_STATIC_EVENT(e_tout, evToutDelay);
static RKH_ROM_STATIC_EVENT(e_Open, evOpen);
static RKH_ROM_STATIC_EVENT(e_Close, evClose);
static RKH_ROM_STATIC_EVENT(e_NetConnected, evNetConnected);
static RKH_ROM_STATIC_EVENT(e_NetDisconnected, evNetDisconnected);
static RKH_ROM_STATIC_EVENT(e_Sent,     evSent);
static RKH_ROM_STATIC_EVENT(e_SendFail, evSendFail);
static RKH_ROM_STATIC_EVENT(e_RecvFail, evRecvFail);

ReceivedEvt e_Received;

static char Imei[IMEI_BUF_SIZE];

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ............................ Initial action ............................. */
static void
init(ConMgr *const me, RKH_EVT_T *pe)
{
	(void)pe;

    rkh_pubsub_subscribe(ConnectionTopic, RKH_UPCAST(RKH_SMA_T, me));
    tpModURC_subscribe(me);

    RKH_TR_FWK_AO(me);

    RKH_TR_FWK_TIMER(&me->timer);
    RKH_TR_FWK_STATE(me, &ConMgr_inactive);
    RKH_TR_FWK_STATE(me, &ConMgr_active);
    RKH_TR_FWK_STATE(me, &ConMgr_initialize);
    RKH_TR_FWK_STATE(me, &ConMgr_sync);
    RKH_TR_FWK_STATE(me, &ConMgr_checkSyncTry);
	RKH_TR_FWK_STATE(me, &ConMgr_init);
    RKH_TR_FWK_STATE(me, &ConMgr_pin);
    RKH_TR_FWK_STATE(me, &ConMgr_setPin);
    RKH_TR_FWK_STATE(me, &ConMgr_enableNetTime);
    RKH_TR_FWK_STATE(me, &ConMgr_getImei);
    RKH_TR_FWK_STATE(me, &ConMgr_cipShutdown);
    RKH_TR_FWK_STATE(me, &ConMgr_initializeFinal);
    RKH_TR_FWK_STATE(me, &ConMgr_registered);
    RKH_TR_FWK_STATE(me, &ConMgr_unregistered);
    RKH_TR_FWK_STATE(me, &ConMgr_failure);
    RKH_TR_FWK_STATE(me, &ConMgr_waitNetClockSync);
    RKH_TR_FWK_STATE(me, &ConMgr_localTime);
    RKH_TR_FWK_STATE(me, &ConMgr_configure);
    RKH_TR_FWK_STATE(me, &ConMgr_configureHist);
    RKH_TR_FWK_STATE(me, &ConMgr_setManualGet);
    RKH_TR_FWK_STATE(me, &ConMgr_setAPN);
    RKH_TR_FWK_STATE(me, &ConMgr_enableGPRS);
    RKH_TR_FWK_STATE(me, &ConMgr_checkIP);
    RKH_TR_FWK_STATE(me, &ConMgr_checkConfigTry);
    RKH_TR_FWK_STATE(me, &ConMgr_waitRetryConfig);
    RKH_TR_FWK_STATE(me, &ConMgr_configureFinal);
    RKH_TR_FWK_STATE(me, &ConMgr_connecting);
    RKH_TR_FWK_STATE(me, &ConMgr_waitingServer);
    RKH_TR_FWK_STATE(me, &ConMgr_connected);
    RKH_TR_FWK_STATE(me, &ConMgr_idle);
    RKH_TR_FWK_STATE(me, &ConMgr_sending);
    RKH_TR_FWK_STATE(me, &ConMgr_waitPrompt);
    RKH_TR_FWK_STATE(me, &ConMgr_waitOk);
    RKH_TR_FWK_STATE(me, &ConMgr_sendingFinal);
    RKH_TR_FWK_STATE(me, &ConMgr_receiving);
    RKH_TR_FWK_STATE(me, &ConMgr_restarting);
    RKH_TR_FWK_STATE(me, &ConMgr_wReopen);
    RKH_TR_FWK_STATE(me, &ConMgr_waitRetryConnect);
    RKH_TR_FWK_STATE(me, &ConMgr_checkConnectTry);
    RKH_TR_FWK_STATE(me, &ConMgr_disconnecting);
    RKH_TR_FWK_TIMER(&me->timer);

    RKH_TMR_INIT(&me->timer, &e_tout, NULL);
    me->retryCount = 0;
}

/* ............................ Effect actions ............................. */
static void
open(ConMgr *const me, RKH_EVT_T *pe)
{
    (void)pe;
    (void)me;

    RKH_SMA_POST_FIFO(modMgr, &e_Open, conMgr);

    modPwr_on();
}

static void
close(ConMgr *const me, RKH_EVT_T *pe)
{
    (void)pe;
    (void)me;

    RKH_SMA_POST_FIFO(modMgr, &e_Close, conMgr);

    modPwr_off();
}

static void
initializeInit(ConMgr *const me, RKH_EVT_T *pe)
{
    (void)pe;

    me->retryCount = 0;
}

static void
storeImei(ConMgr *const me, RKH_EVT_T *pe)
{
    ImeiEvt *p;

	(void)me;

    p = RKH_UPCAST(ImeiEvt, pe);
    strcpy(Imei, p->buf);
}

static void
localTimeGet(ConMgr *const me, RKH_EVT_T *pe)
{
	(void)me;
	(void)pe;

    ModCmd_getLocalTime();
}

static void 
rtimeSync(ConMgr *const me, RKH_EVT_T *pe)
{
	(void)me;

    rtime_set(&(RKH_UPCAST(LocalTimeEvt, pe)->time));
}

static void
configureInit(ConMgr *const me, RKH_EVT_T *pe)
{
    (void)pe;

    me->retryCount = 0;
}

static void
configTry(ConMgr *const me, RKH_EVT_T *pe)
{
    (void)pe;

    ++me->retryCount;
}

static void 
requestIp(ConMgr *const me, RKH_EVT_T *pe)
{
    (void)pe;
    (void)me;

    ModCmd_requestIP();
}

static void
connectInit(ConMgr *const me, RKH_EVT_T *pe)
{
    (void)pe;

    me->retryCount = 0;
}

static void
connectTry(ConMgr *const me, RKH_EVT_T *pe)
{
    (void)pe;

    ++me->retryCount;
}

static void
socketOpen(ConMgr *const me, RKH_EVT_T *pe)
{
    (void)me;
    (void)pe;

    ModCmd_connect(CONNECTION_PROT, CONNECTION_DOMAIN, CONNECTION_PORT);
}

static void
socketClose(ConMgr *const me, RKH_EVT_T *pe)
{
    (void)me;
    (void)pe;

    ModCmd_disconnect();
}

static void 
readData(ConMgr *const me, RKH_EVT_T *pe)
{
    (void)pe;
    (void)me;

    RKH_SET_STATIC_EVENT(RKH_UPCAST(RKH_EVT_T, &e_Received), evReceived);
    ModCmd_readData();
}

static void
sendRequest(ConMgr *const me, RKH_EVT_T *pe)
{
    (void)me;

    me->psend = RKH_UPCAST(SendEvt, pe);

    ModCmd_sendDataRequest((rui16_t)(me->psend->size));
}

static void
flushData(ConMgr *const me, RKH_EVT_T *pe)
{
    (void)pe;

    ModCmd_sendData(me->psend->buf, me->psend->size);
}

static void
sendOk(ConMgr *const me, RKH_EVT_T *pe)
{
    (void)pe;
    (void)me;

    rkh_pubsub_publish(ConnectionTopic, RKH_UPCAST(RKH_EVT_T, &e_Sent), 
                                     RKH_UPCAST(RKH_SMA_T, me));
}

static void
recvOk(ConMgr *const me, RKH_EVT_T *pe)
{
    (void)pe;
    (void)me;

    rkh_pubsub_publish(ConnectionTopic, RKH_UPCAST(RKH_EVT_T, &e_Received), 
                                     RKH_UPCAST(RKH_SMA_T, me));
}

static void
sendFail(ConMgr *const me, RKH_EVT_T *pe)
{
    (void)pe;
    (void)me;

    rkh_pubsub_publish(ConnectionTopic, RKH_UPCAST(RKH_EVT_T, &e_SendFail), 
                                     RKH_UPCAST(RKH_SMA_T, me));
	ModCmd_init();
}

static void
recvFail(ConMgr *const me, RKH_EVT_T *pe)
{
    (void)pe;
    (void)me;

    rkh_pubsub_publish(ConnectionTopic, RKH_UPCAST(RKH_EVT_T, &e_RecvFail), 
                                     RKH_UPCAST(RKH_SMA_T, me));
	ModCmd_init();
}

static void
resetParser(ConMgr *const me, RKH_EVT_T *pe)
{
    (void)pe;
    (void)me;

	ModCmd_init();
}

/* ............................. Entry actions ............................. */
static void
sendSync(ConMgr *const me)
{
    ++me->retryCount;

    ModCmd_sync();
}

static void
sendInit(ConMgr *const me)
{
    (void)me;

    ModCmd_initStr();
}

static void
checkPin(ConMgr *const me)
{
    (void)me;

    ModCmd_getPinStatus();
}

static void
setPin(ConMgr *const me)
{
    (void)me;

    ModCmd_setPin(SIM_PIN_NUMBER);
}

static void
netTimeEnable(ConMgr *const me)
{
    (void)me;

    ModCmd_enableNetTime();
}

static void
getImei(ConMgr *const me)
{
    (void)me;

    ModCmd_getImei();
}

static void
cipShutdown(ConMgr *const me)
{
    (void)me;

    ModCmd_cipShutdown();
}

static void 
unregEntry(ConMgr *const me)
{
    ModCmd_getRegStatus();

    RKH_SET_STATIC_EVENT(&e_tout, evTimeout);
    RKH_TMR_ONESHOT(&me->timer, RKH_UPCAST(RKH_SMA_T, me), REGISTRATION_TIME);
}

static void 
failureEntry(ConMgr *const me)
{
    RKH_SET_STATIC_EVENT(&e_tout, evTimeout);
    RKH_TMR_ONESHOT(&me->timer, RKH_UPCAST(RKH_SMA_T, me), FAILURE_TRY_DELAY);
    modPwr_off();
}

static void
waitRetryConfigEntry(ConMgr *const me)
{
    RKH_SET_STATIC_EVENT(&e_tout, evTimeout);
    RKH_TMR_ONESHOT(&me->timer, RKH_UPCAST(RKH_SMA_T, me), CONFIG_TRY_DELAY);
}

static void
setupManualGet(ConMgr *const me)
{
    (void)me;

    ModCmd_setManualGet();
}
   
static void
waitNetClockSyncEntry(ConMgr *const me)
{
    RKH_SET_STATIC_EVENT(&e_tout, evTimeout);
    RKH_TMR_ONESHOT(&me->timer, RKH_UPCAST(RKH_SMA_T, me), WAIT_NETCLOCK_TIME);
}

static void
setupAPN(ConMgr *const me)
{
    (void)me;

    ModCmd_setupAPN(CONNECTION_APN, CONNECTION_USER, CONNECTION_PASSWORD);
}
   
static void
startGPRS(ConMgr *const me)
{
    (void)me;

    ModCmd_startGPRS();
}

static void
getConnStatus(ConMgr *const me)
{
    (void)me;

    ModCmd_getConnStatus();
}

static void
connectingEntry(ConMgr *const me)
{
    (void)me;

    RKH_SET_STATIC_EVENT(&e_tout, evTimeout);
    RKH_TMR_ONESHOT(&me->timer, RKH_UPCAST(RKH_SMA_T, me), CONNSTATUS_PERIOD);
}

static void
socketConnected(ConMgr *const me)
{
    (void)me;

    rkh_pubsub_publish(ConnectionTopic, RKH_UPCAST(RKH_EVT_T, &e_NetConnected), 
                                     RKH_UPCAST(RKH_SMA_T, me));
    bsp_netStatus(ConnectedSt);
}

static void
wReopenEntry(ConMgr *const me)
{
    (void)me;

    RKH_SET_STATIC_EVENT(&e_tout, evTimeout);
    RKH_TMR_ONESHOT(&me->timer, RKH_UPCAST(RKH_SMA_T, me), REOPEN_DELAY);
}

static void
waitRetryConnEntry(ConMgr *const me)
{
    (void)me;

    RKH_SET_STATIC_EVENT(&e_tout, evTimeout);
    RKH_TMR_ONESHOT(&me->timer, RKH_UPCAST(RKH_SMA_T, me), CONNECT_TRY_DELAY);
}

static void
idleEntry(ConMgr *const me)
{
    (void)me;

    RKH_SET_STATIC_EVENT(&e_tout, evTimeout);
    RKH_TMR_ONESHOT(&me->timer, RKH_UPCAST(RKH_SMA_T, me), CONNSTATUS_PERIOD);
}


/* ............................. Exit actions ............................. */
static void 
unregExit(ConMgr *const me)
{
    rkh_tmr_stop(&me->timer);
}

static void
waitNetClockSyncExit(ConMgr *const me)
{
    rkh_tmr_stop(&me->timer);
}

static void
failureExit(ConMgr *const me)
{
    (void)me;

    modPwr_on();
    ModCmd_init();
    rkh_tmr_stop(&me->timer);
}

static void
wReopenExit(ConMgr *const me)
{
    (void)me;

    rkh_tmr_stop(&me->timer);
}

static void
waitRetryConnExit(ConMgr *const me)
{
    (void)me;

    ModCmd_init();
    rkh_tmr_stop(&me->timer);
}

static void
connectingExit(ConMgr *const me)
{
    (void)me;

    rkh_tmr_stop(&me->timer);
}

static void
socketDisconnected(ConMgr *const me)
{
    (void)me;

    rkh_pubsub_publish(ConnectionTopic, RKH_UPCAST(RKH_EVT_T, &e_NetDisconnected), 
                                     RKH_UPCAST(RKH_SMA_T, me));
    bsp_netStatus(DisconnectedSt);
}

static void
idleExit(ConMgr *const me)
{
    (void)me;

    rkh_tmr_stop(&me->timer);
}

/* ................................ Guards ................................. */
rbool_t
checkSyncTry(ConMgr *const me, RKH_EVT_T *pe)
{
    (void)pe;
    
    return (me->retryCount < MAX_SYNC_RETRY) ? RKH_TRUE : RKH_FALSE;
}

rbool_t
checkConfigTry(ConMgr *const me, RKH_EVT_T *pe)
{
    (void)pe;
    
    return (me->retryCount < MAX_CONFIG_RETRY) ? RKH_TRUE : RKH_FALSE;
}

rbool_t
checkConnectTry(ConMgr *const me, RKH_EVT_T *pe)
{
    (void)pe;
    
    return (me->retryCount < MAX_CONNECT_RETRY) ? RKH_TRUE : RKH_FALSE;
}

/* ---------------------------- Global functions --------------------------- */
ReceivedEvt *
ConMgr_ReceiveDataGetRef(void)
{
    return &e_Received;
}

char *
ConMgr_Imei(void)
{
    return Imei;
}

char *
ConMgr_ImeiSNR(void)
{
    return (Imei + IMEI_SNR_OFFSET);
}

/* ------------------------------ End of file ------------------------------ */
