/*
 *	file: ffport.c - Visual Studio 2008 port
 *	Last updated for version: 1.0.00
 *	Date of the last update:  Nov 20, 2012
 *
 * 	Copyright (C) 2010 Leandro Francucci. All rights reserved.
 *
 * 	RKH is free software: you can redistribute it and/or modify
 * 	it under the terms of the GNU General Public License as published by
 * 	the Free Software Foundation, either version 3 of the License, or
 * 	(at your option) any later version.
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
 * RKH web site:	http://
 * e-mail:			francuccilea@gmail.com
 */

/*
 * 	ffport.c
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ffile.h"


#define RETRIES				2
#define EQUAL				0


static ffui8_t image_cell = 0xFF;
static FILE *fout;


void 
flash_init( void )
{
	long i;

	if( ( fout = fopen( "build/test/out/image", "r+b" ) ) == NULL )
    {
		if( ( fout = fopen( "build/test/out/image", "w+b" ) ) == NULL )
		{
			perror( "Can't open input file image\n" );
			exit( EXIT_FAILURE );
		}
		else
		{
			/*printf( "Created the image file\n" );*/
			/* ...emulates the erased flash memory device */
			for( i = 0 ; i < FF_FLASH_SIZE ; ++i )
				fwrite( &image_cell, sizeof( image_cell ), 1, fout );
		}
    }

	if( ferror( fout ) )
	{
		perror( "Error in image file" );
		exit( EXIT_FAILURE );
	}	
}


void 
flash_close( void )
{
	fclose( fout );
}


void
flash_write_data( unsigned long pto, const void *pfrom, int qty )
{
	long loc;

	loc = (long)pto - (long)RF_FB_ADDRESS; 		/* position location */
	fseek( fout, loc, SEEK_SET );
	fwrite( pfrom, 1, qty, fout );
	if( ferror( fout ) )
	{
		perror( "Error in write image file" );
		exit( EXIT_FAILURE );
	}	
}


void
flash_read_data( void *pto, unsigned long pfrom, int qty )
{
	long loc;

	loc = (long)pfrom - (long)RF_FB_ADDRESS; 	/* position location */
	fseek( fout, loc, SEEK_SET );
	fread( pto, 1, qty, fout );
	if( ferror( fout ) )
	{
		perror( "Error in write image file" );
		exit( EXIT_FAILURE );
	}
}


void
flash_read_page( void *pto, unsigned long pfrom )
{
	flash_read_data( pto, pfrom, FF_PHYS_BLOCK_SIZE );
}


void
flash_write_page( unsigned long pto, const void *pfrom )
{
	flash_write_data( pto, pfrom, FF_PHYS_BLOCK_SIZE );
}
