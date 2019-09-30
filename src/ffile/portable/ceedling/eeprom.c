/**
 *  \file       eeprom.c
 *  \brief
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2019.25.07  LeFr  v1.0.00  Initial version
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  LeFr  Leandro Francucci lf@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "eeprom.h"
#include "ffile.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
static ffui8_t image_cell = 0xFF;
static FILE *fout;

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ---------------------------- Global functions --------------------------- */
void
eeprom_init(void)
{
	long i;

	if ((fout = fopen( "build/test/out/image-eeprom", "r+b")) == NULL)
    {
		if ((fout = fopen( "build/test/out/image-eeprom", "w+b")) == NULL)
		{
			perror("Can't open input file image-eeprom\n");
			exit(EXIT_FAILURE);
		}
		else
		{
			/* ...emulates the erased eeprom memory device */
			for (i = 0 ; i < 256; ++i)
            {
				fwrite(&image_cell, sizeof(image_cell), 1, fout);
            }
		}
    }

	if (ferror(fout))
	{
		perror("Error in image-eeprom file");
		exit(EXIT_FAILURE);
	}	
	fflush(fout);
}

void
eeprom_write(uint8_t *p, uint16_t addr, uint16_t qty)
{
	fseek(fout, addr, SEEK_SET);
	fwrite(p, 1, qty, fout);
	if (ferror(fout))
	{
		perror("Error in write image-eeprom file");
		exit(EXIT_FAILURE);
	}	
	fflush(fout);
}

void
eeprom_read(uint8_t* p, uint16_t addr, uint16_t qty)
{
	fseek(fout, addr, SEEK_SET);
	fread(p, 1, qty, fout);
	if (ferror(fout))
	{
		perror("Error in write image-eeprom file");
		exit(EXIT_FAILURE);
	}
}

uint8_t
eeprom_readStatusRegister(void)
{
    return 0;
}

void
eeprom_writeStatusRegister(uint8_t regval)
{
}

/* ------------------------------ End of file ------------------------------ */

