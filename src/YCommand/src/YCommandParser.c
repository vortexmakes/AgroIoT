/**
 *  \file       YCommandParser.c
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
#include "ssp.h"
#include "YCommand.h"
#include "YCommandParser.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
SSP_DCLR_NORMAL_NODE rootYCommandParser;
SSP_DCLR_TRN_NODE socketIndex, id, security, data;

static YCommandParser *pYCmd;

static char *p;

/* ----------------------- Local function prototypes ----------------------- */
static void indexInit(unsigned char pos);
static void noIndexidInit(unsigned char pos);
static void indexCollect(unsigned char c);
static void idInit(unsigned char pos);
static void idCollect(unsigned char c);
static void securityInit(unsigned char pos);
static void securityCollect(unsigned char c);
static void dataInit(unsigned char pos);
static void dataCollect(unsigned char c);
static void found(unsigned char pos);

/* ---------------------------- Local functions ---------------------------- */

SSP_CREATE_NORMAL_NODE(rootYCommandParser);
SSP_CREATE_BR_TABLE(rootYCommandParser)
SSPBR("!3",    indexInit,       &socketIndex),
SSPBR("Im:",   noIndexidInit,   &id),
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
SSPBR(";",    found, &rootYCommandParser),
SSP_END_BR_TABLE

static void
fieldInsert(char *pb, int len, unsigned char c)
{
    if(p >= (pb + len))
    {
        return;
    }

    if(c == ',' || c == ';')
        *p = '\0';
    else
        *p = (char)c;

    ++p;
    *p = '\0';
};

static void
fieldInit(char *pb)
{
    p = pb;
    *p = '\0';
};

static void
indexInit(unsigned char pos)
{
    (void)pos;

    fieldInit(pYCmd->index);
}

static void
indexCollect(unsigned char c)
{
    fieldInsert(pYCmd->index, YCOMMAND_INDEX_LEN, c);
}

static void
noIndexidInit(unsigned char pos)
{
    (void)pos;

    fieldInit(pYCmd->index);
    fieldInit(pYCmd->id);
}

static void
idInit(unsigned char pos)
{
    (void)pos;

    fieldInit(pYCmd->id);
}

static void
idCollect(unsigned char c)
{
    fieldInsert(pYCmd->id, YCOMMAND_ID_LEN, c);
}

static void
securityInit(unsigned char pos)
{
    (void)pos;
    fieldInit(pYCmd->security);
}

static void
securityCollect(unsigned char c)
{
    fieldInsert(pYCmd->security, YCOMMAND_SECURITY_LEN, c);
}

static void
dataInit(unsigned char pos)
{
    (void)pos;

    fieldInit(pYCmd->data);
}

static void
dataCollect(unsigned char c)
{
    fieldInsert(pYCmd->data, YCOMMAND_DATA_LEN, c);
}

static void
found(unsigned char pos)
{
    (void)pos;

    pYCmd->result = YCommandFound;
}

YCmdPResult
YCommandParser_search(YCommandParser *me, char *p, ruint size)
{
    pYCmd = me;

    ssp_init(&pYCmd->parser, &rootYCommandParser);

    pYCmd->result = YCommandNotFound; 

    do
    {
        ssp_doSearch(&pYCmd->parser, *p++);
    }
    while(--size);

    return pYCmd->result;
}

YCmdPResult
YCommandParser_securityCheck(YCommandParser *me, char *pkey)
{
    if(strncmp(me->security, pkey, YCOMMAND_SECURITY_LEN) != 0)
    {
        return YCommandInvalidKey; 
    }

    return YCommandValidKey;
}

char *
YCommandParser_getIndex(YCommandParser *me)
{
   return me->index; 
}

char *
YCommandParser_getData(YCommandParser *me)
{
    return me->data;
}

ruint
YCommandParser_getId(YCommandParser *me)
{
    ruint id;
    
    id = atoi(me->id);

    return (id >= YCmdNum) ? YCmdUnknown : id;
}

/* ---------------------------- Global functions --------------------------- */
/* ------------------------------ End of file ------------------------------ */
