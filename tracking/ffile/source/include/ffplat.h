/*
 *	file: ffplat.h
 *	Last updated for version: 1.0.00
 *	Date of the last update:  Nov 20, 2012
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
 *  \file ffplat.h
 *	\brief
 *	FFILE platform-dependent interface.
 *
 *  All software components of the FFILE, contain a platform abstraction
 *  layer. It is an indirection layer that hides the differences in
 *  hardware and software environments in which FFILE operates so that the
 *  FFILE source code does not need to be changed to run in a different
 *  environment. Instead, all the changes required to adapt FFILE are confined
 *  to this layer. It's basically composed by a few files, \b rkhplat.h and
 *  rkhtype.h, which include references to the actual platform-dependent
 *  files like \b ffport.h and \b rkht.h. The platform-specific code for
 *  the FFILE port is defined by \b rkhport.c. Not all FFILE ports require this
 *  file.
 *
 *  As said above, each platform, compiler or processor supported by FFILE must
 *  have its own platform-dependent file, called \b rkhport.h by FFILE
 *  convention.
 *  Next, each \b rkhport.h file must be referenced from rkhplat.h header
 *  file, located in \\include directory.  The next listing shows an example
 *  of \b rkhplat.h, where __CFV1CW63__, and __W32STVC08__ are used to
 *	instruct the C/C++ compiler to include header files from the specific
 *	FFILE port directory. The key point of the design is that all
 *	platform-independent FFILE source files include the same \b rkhplat.h
 *	header file as the application source files.
 *
 *	\code
 *	#ifdef __CFV1CW63__
 *		#include "..\portable\cfv1\rkhs\cw6_3\rkhport.h"
 *	#endif
 *
 *	#ifdef __W32STVC08__
 *		#include "..\portable\80x86\win32_st\vc08\rkhport.h"
 *	#endif
 *	...
 *	\endcode
 *
 *	The idea behind conditional compilation is that a \b rkhport.h can be
 *	selectively compiled, depending upon whether a specific value has been
 *	defined.
 *
 *	\note
 *	The path of platform-dependent file must be relative.
 */

#ifndef __FFPLAT_H__
#define __FFPLAT_H__

#ifdef __W32VC08__
    #include "..\portable\80x86\win32\vc08\ffport.h"
#endif

#ifdef __S08GB60CW63__
    #include "..\portable\s08gb60\cw6_3\internal\ffport.h"
#endif

#ifdef __S08GB60CW63DF__
    #include "..\portable\s08gb60\cw6_3\dataflash\ffport.h"
#endif

#endif
/* ------------------------------ End of file ------------------------------ */
