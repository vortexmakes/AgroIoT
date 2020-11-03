/**
 *  \file       AgroIoTVersion.h
 *  \brief      Defines the AgroIoT version
 */

/* -------------------------- Development history -------------------------- */
/* -------------------------------- Authors -------------------------------- */
/*
 *  LeFr  Leandro Francucci  lf@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* --------------------------------- Module -------------------------------- */
#ifndef __AGROIOTVERSION_H__
#define __AGROIOTVERSION_H__

/* ----------------------------- Include files ----------------------------- */
/* ---------------------- External C language linkage ---------------------- */
#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------- Macros -------------------------------- */
/* -------------------------------- Constants ------------------------------ */
/**
 *  \brief
 *	This macro expands to the binary representation of the AgroIoT version,
 *  which follows the semantic and format provided by https://semver.org/
 *
 *	The version number is composed by 0xABCCDD, where:
 *	- A (1-digit) denoted the major version
 *  - B (1-digit) denoted the minor version
 *	- C (2-digits) denoted the patch version
 *  - D (2-digits) denoted the beta version.
 *
 *  As a general rule, given a version number major.minor.patch, increment 
 *  the:
 *  1. major version when you make incompatible API changes,
 *  2. minor version when you add functionality in a backwards compatible 
 *     manner, and
 *  3. patch version when you make backwards compatible bug fixes.
 *  
 *  Additional labels for pre-release are available as extensions to the 
 *  major.minor.patch format.
 *
 *  For example, the code for version 2.2.04 is 0x220400 and the code 
 *  for version 2.2.04-beta.2 is 0x220402
 */
#define AGROIOT_VERSION         0x090010

/**
 *  \brief
 *  This macro indicates the release date as string in DDMMYY format.
 */
#define AGROIOT_RELEASE_DATE    "031120"

/* ------------------------------- Data types ------------------------------ */
/* -------------------------- External variables --------------------------- */
/**
 *  \brief
 *  String indicating the AgroIoT version.
 */
extern const char agroIoTVersion[];

/* -------------------------- Function prototypes -------------------------- */
/* -------------------- External C language linkage end -------------------- */
#ifdef __cplusplus
}
#endif

/* ------------------------------ Module end ------------------------------- */
#endif

/* ------------------------------ End of file ------------------------------ */
