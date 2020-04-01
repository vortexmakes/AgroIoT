/**
 *  \file       SecString.c
 *  \brief      Implementation of secure string.
 */

/* -------------------------- Development history -------------------------- */
/*
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  LeFr  Leandro Francucci lf@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */
#include <string.h>
#include "SecString.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ---------------------------- Global functions --------------------------- */
char *
SecString_strchk(char *s, size_t num)
{
    char *pos;

    pos = memchr(s, '\0', num);
    return pos;
}

size_t 
SecString_strnlen(char *s, size_t maxlen)
{
    char *pos = memchr(s, '\0', maxlen);
    return (pos != (char *)0) ? (pos - s) : maxlen;
}

/* ------------------------------ End of file ------------------------------ */
