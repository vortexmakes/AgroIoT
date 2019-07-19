/*
 *	file: fftype.h
 *	Last updated for version: 1.0.00
 *	Date of the last update:  Feb 28, 2012
 *
 *  Copyright (C) 2010 Leandro Francucci. All rights reserved.
 *
 *  FFILE is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  FFILE is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with FFILE, see copying.txt file.
 *
 * Contact information:
 * e-mail:			francuccilea@gmail.com
 */

/**
 *  \file fftype.h
 *	\brief
 *	This file defines the data types that uses FFILE.
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

#ifndef __FFTYPE_H__
#define __FFTYPE_H__

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

#endif
/* ------------------------------ End of file ------------------------------ */
