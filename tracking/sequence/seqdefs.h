/*
 * 	seqdefs.h
 */

#ifndef __SEQDEFS_H__
#define __SEQDEFS_H__

/*
 * 	Led codes
 */

enum
{
	NO_LIT,
        GPKG_NOTON_FLASH = NO_LIT,
        SMSCMD_NOTQUEUED = NO_LIT,
	LIT,  
        LINE_BATT = LIT,
		SIGNAL_STRENGTH = LIT,
        GPS_VALID_PKG = LIT,
        GPKG_ON_FLASH = LIT,
        SMSCMD_QUEUED = LIT, 
	LINE_BATTCHG,
	    NOSRV = LINE_BATTCHG,
	LINE_NOBATT, 
	NOLINE_BATT,
	NOLINE_BATTDIS,
	LSTAGE1,
	LSTAGE2,
        GPS_INV_PKG = LSTAGE2,
	LSTAGE3,
	LSTAGE4,
	LFAIL,
	  GPS_FAILURE = LFAIL,
	NUM_LEDS
};


/*
 * 	Addressed by main led sequences
 */

enum
{
	NUM_TOTAL_LEDS = NUM_LEDS
};

/*
 * 	Minor indices
 */

#define LED_POWER       0x02
#define LED_GSM         0x04
#define LED_STORAGE     0x08
#define LED_GPS         0x10
#define LED_WORK        0x20
#define LED_SIM         0x40
#define NUM_LEDS_CHN    6
	
/*
 * 	Major indices
 */

typedef enum
{
	MAJOR_LED,
	NUM_MAJOR
} MAJOR_ENUM;

#define NUM_SEQ_CHANNELS NUM_LEDS_CHN

#define __BURST_INTERRUPT__ 

#endif
