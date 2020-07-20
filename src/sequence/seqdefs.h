/*
 *  seqdefs.h
 */

#ifndef __SEQDEFS_H__
#define __SEQDEFS_H__

/*
 *  Led codes
 */

typedef enum
{
    SEQ_NO_LIT,
    SEQ_GPKG_NOTON_FLASH = SEQ_NO_LIT,
    SEQ_SMSCMD_NOTQUEUED = SEQ_NO_LIT,
    SEQ_LIT,
    SEQ_LINE_BATT = SEQ_LIT,
    SEQ_SIGNAL_STRENGTH = SEQ_LIT,
    SEQ_GPS_VALID_PKG = SEQ_LIT,
    SEQ_GPKG_ON_FLASH = SEQ_LIT,
    SEQ_SMSCMD_QUEUED = SEQ_LIT,
    SEQ_LINE_BATTCHG,
    SEQ_NOSRV = SEQ_LINE_BATTCHG,
    SEQ_LINE_NOBATT,
    SEQ_NOLINE_BATT,
    SEQ_NOLINE_BATTDIS,
    SEQ_LSTAGE1,
    SEQ_LSTAGE2,
    SEQ_GPS_INV_PKG = SEQ_LSTAGE2,
    SEQ_LSTAGE3,
    SEQ_LSTAGE4,
    SEQ_LFAIL,
    SEQ_GPS_FAILURE = SEQ_LFAIL,
    SEQ_NUM_LEDS
} SeqNo;

/*
 *  Addressed by main led sequences
 */

enum
{
    NUM_TOTAL_LEDS = SEQ_NUM_LEDS
};

/*
 *  Minor indices
 */

#define LED_POWER       0x02
#define LED_GSM         0x04
#define LED_STORAGE     0x08
#define LED_GPS         0x10
#define LED_WORK        0x20
#define LED_SIM         0x40
#define NUM_LEDS_CHN    6

/*
 *  Major indices
 */

typedef enum
{
    MAJOR_LED,
    NUM_MAJOR
} MAJOR_ENUM;

#define NUM_SEQ_CHANNELS NUM_LEDS_CHN

#define __BURST_INTERRUPT__

#endif
/* ------------------------------ End of file ------------------------------ */
