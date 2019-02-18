/*
 *	file: fft.h - Visual Studio 2008 port
 *	Last updated for version: 1.0.00
 *	Date of the last update:  Nov 20, 2012
 *
 *  Copyright (C) 2010 Leandro Francucci. All rights reserved.
 *
 *  RKH is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  RKH is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with RKH, see copying.txt file.
 *
 * Contact information:
 * e-mail:			francuccilea@gmail.com
 */

/*
 *  fft.h
 */

#ifndef __FFT_H__
#define __FFT_H__

/*
 *  Portable data types.
 *
 *  The FFILE uses a set of integer quantities. That maybe
 *  machine or compiler	dependent.
 *
 *  Note:
 *
 *  The 'FFUInt' and 'FFInt' will normally be the natural size
 *  for a particular machine. These types designates an integer
 *  type that is usually fastest to operate with among all integer
 *  types.
 */

typedef signed char ffi8_t;
typedef signed short ffi16_t;
typedef signed long ffi32_t;
typedef unsigned char ffui8_t;
typedef unsigned short ffui16_t;
typedef unsigned long ffui32_t;

typedef unsigned int FFUInt;
typedef signed int FFInt;

#endif
/* ------------------------------ End of file ------------------------------ */
