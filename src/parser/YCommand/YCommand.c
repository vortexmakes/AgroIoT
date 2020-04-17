/**
 *  \file       YCommand.c
 *  \brief      ...
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2020.04.17  Daba  v1.0.00  Initial version
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  DaBa  Dario Baliña db@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */
#include <stdlib.h>
#include <string.h>
#include "rkh.h"
#include "YCommand.h"
#include "signals.h"
#include "events.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
ruint YCommandParser;

/* ---------------------------- Local variables ---------------------------- */
SSP_DCLR_NORMAL_NODE rootYCommandParser;
SSP_DCLR_TRN_NODE socketIndex, id, security, data;

/* ----------------------- Local function prototypes ----------------------- */
static void indexInit(unsigned char pos);
static void indexCollect(unsigned char c);
static void idInit(unsigned char pos);
static void idCollect(unsigned char c);
static void securityInit(unsigned char pos);
static void securityCollect(unsigned char c);
static void dataInit(unsigned char pos);
static void dataCollect(unsigned char c);
static void process(unsigned char pos);

/* ---------------------------- Local functions ---------------------------- */

SSP_CREATE_NORMAL_NODE(rootYCommandParser);
SSP_CREATE_BR_TABLE(rootYCommandParser)
SSPBR("!3",    indexInit, &socketIndex),
SSPBR("Im:",   idInit,    &id),
SSP_END_BR_TABLE

SSP_CREATE_TRN_NODE(socketIndex, indexCollect);
SSP_CREATE_BR_TABLE(socketIndex)
SSPBR(",",    idInit, &id),
SSP_END_BR_TABLE

SSP_CREATE_TRN_NODE(id, idCollect);
SSP_CREATE_BR_TABLE(id)
SSPBR(",",    securityInit, &security),
SSP_END_BR_TABLE

SSP_CREATE_TRN_NODE(security, securityCollect);
SSP_CREATE_BR_TABLE(security)
SSPBR(",",    dataInit, &data),
SSP_END_BR_TABLE

SSP_CREATE_TRN_NODE(data, dataCollect);
SSP_CREATE_BR_TABLE(data)
SSPBR(";",    process, &rootYCommandParser),
SSP_END_BR_TABLE

static void
indexInit(unsigned char pos)
{
    (void)pos;
}

static void
indexCollect(unsigned char c)
{
}

static void
idInit(unsigned char pos)
{
    (void)pos;
}

static void
idCollect(unsigned char c)
{
}

static void
securityInit(unsigned char pos)
{
    (void)pos;
}

static void
securityCollect(unsigned char c)
{
}

static void
dataInit(unsigned char pos)
{
    (void)pos;
}

static void
dataCollect(unsigned char c)
{
}

static void
process(unsigned char pos)
{
    (void)pos;
}

/* ---------------------------- Global functions --------------------------- */
/* ------------------------------ End of file ------------------------------ */
