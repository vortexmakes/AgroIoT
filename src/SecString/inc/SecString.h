/**
 *  \file       SecString.h
 *  \brief      Specification of secure string module.
 */

/* -------------------------- Development history -------------------------- */
/*
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  LeFr  Leandro Francucci lf@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* --------------------------------- Module -------------------------------- */
#ifndef __SECSTRING_H__
#define __SECSTRING_H__

/* ----------------------------- Include files ----------------------------- */
/* ---------------------- External C language linkage ---------------------- */
#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------- Macros -------------------------------- */
/* -------------------------------- Constants ------------------------------ */
/* ------------------------------- Data types ------------------------------ */
/* -------------------------- External variables --------------------------- */
/* -------------------------- Function prototypes -------------------------- */
/**
 *  Searches within the first num bytes of the string pointed by s for the 
 *  first occurrence of '\0', and returns a pointer to it.
 *
 *  \return
 *  A pointer to the first occurrence of '\0' in the string pointed by s.
 *  If the '\0' is not found, the function returns a null pointer.
 */
char *SecString_strchk(char *s, size_t num);

/* -------------------- External C language linkage end -------------------- */
#ifdef __cplusplus
}
#endif

/* ------------------------------ Module end ------------------------------- */
#endif

/* ------------------------------ End of file ------------------------------ */
