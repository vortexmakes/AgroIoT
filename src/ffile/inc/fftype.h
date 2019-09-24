/**
 *  \file       fftype.h
 *  \brief      This file defines the data types that uses FFILE.
 *
 *  The FFILE uses a set of integer quantities. That maybe machine or compiler
 *  dependent. These types must be defined in \b fft.h file. The following
 *  listing shows the required data type definitions:
 *
 *  \code
 *  // Denotes a signed integer type with a width of exactly 8 bits
 *  typedef signed char     ffi8_t;
 *
 *  // Denotes a signed integer type with a width of exactly 16 bits
 *  typedef signed short    ffi16_t;
 *
 *  // Denotes a signed integer type with a width of exactly 32 bits
 *  typedef signed long		ffi32_t;
 *
 *  // Denotes an unsigned integer type with a width of exactly 8 bits
 *  typedef unsigned char   ffui8_t;
 *
 *  // Denotes an unsigned integer type with a width of exactly 16 bits
 *  typedef unsigned short  ffui16_t;
 *
 *  // Denotes an unsigned integer type with a width of exactly 32 bits
 *  typedef unsigned long	ffui32_t;
 *
 *  // Denotes an unsigned integer type that is usually fastest to operate with
 *  // among all integer types.
 *  typedef unsigned int	HUInt;
 *
 *  // Denotes a signed integer type that is usually fastest to operate with
 *  // among all integer types.
 *  typedef signed int		HInt;
 *  \endcode
 *
 *  Next, each \b fft.h file must be referenced from \b fftype.h header
 *  file, located in \\include directory.  The next listing shows an example
 *  of \b fftype.h, where __CFV1CW63__, and __W32STVC08__ are used to
 *	instruct the C/C++ compiler to include header files from the specific
 *	FFILE port directory. The key point of the design is that all
 *	platform-independent FFILE source files include the same \b fftype.h
 *	header file as the application source files.
 *
 *	\code
 *	#ifdef __W32STVC08__
 *		#include "..\portable\80x86\win32\vc08\fft.h"
 *	#endif
 *	...
 *	\endcode
 *
 *	The idea behind conditional compilation is that a \b fft.h can be
 *	selectively compiled, depending upon whether a specific value has been
 *	defined.
 *
 *	\note
 *	The path of platform-dependent file must be relative.
 */

/* -------------------------- Development history -------------------------- */
/*
 */

/* -------------------------------- Authors -------------------------------- */
/*
 */

/* --------------------------------- Notes --------------------------------- */
/* --------------------------------- Module -------------------------------- */
#ifndef __FFTYPE_H__
#define __FFTYPE_H__

/* ----------------------------- Include files ----------------------------- */
#ifdef __W32VC08__
    #include "..\portable\80x86\win32\vc08\fft.h"
#endif

#ifdef __S08GB60CW63__
    #include "..\portable\s08gb60\cw6_3\internal\fft.h"
#endif

#ifdef __S08GB60CW63DF__
    #include "..\portable\s08gb60\cw6_3\dataflash\fft.h"
#endif

#ifdef __DATAFLASH__
    #include "../portable/dataflash/fft.h"
#endif

#ifdef __FFILETEST__
    #include "../portable/test/fft.h"
#endif

/* ---------------------- External C language linkage ---------------------- */
#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------- Macros -------------------------------- */
/* -------------------------------- Constants ------------------------------ */
/* ------------------------------- Data types ------------------------------ */
/* -------------------------- External variables --------------------------- */
/* -------------------------- Function prototypes -------------------------- */
/* -------------------- External C language linkage end -------------------- */
#ifdef __cplusplus
}
#endif

/* ------------------------------ Module end ------------------------------- */
#endif

/* ------------------------------ End of file ------------------------------ */
