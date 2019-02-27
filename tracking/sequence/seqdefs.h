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

enum
{
    LED_POWER,
    LED_GSM,
    LED_STORAGE,
    LED_GPS,
    LED_WORK,
    LED_SIM,
	NUM_LEDS_CHN
};
	
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
