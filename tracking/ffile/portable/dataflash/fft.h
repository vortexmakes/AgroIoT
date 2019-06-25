/**
 *  \file       fft.h
 *  \brief      Data types definitions for FFile dataflash port.
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2019.06.25  DaBa  v1.0.00   Initial version
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  DaBa  Dario Baliña db@vortexmakes.com
 */

#ifndef __FFT_H__
#define __FFT_H__


/*
 * 	Portable data types.
 *
 * 	The FFILE uses a set of integer quantities. That maybe 
 * 	machine or compiler	dependent.
 *
 * 	Note:
 *
 * 	The 'FFUInt' and 'FFInt' will normally be the natural size 
 * 	for a particular machine. These types designates an integer 
 * 	type that is usually fastest to operate with among all integer 
 * 	types.
 */

typedef signed char 	ffi8_t;
typedef signed short 	ffi16_t;
typedef signed long		ffi32_t;
typedef unsigned char 	ffui8_t;
typedef unsigned short 	ffui16_t;
typedef unsigned long	ffui32_t;

typedef unsigned char	FFUInt;
typedef signed char		FFInt;


#endif
