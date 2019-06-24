/*
 * 	seqtbl.c
 * 		Tables of sequences
 */

#include "mytypes.h"
#include "seqtbl.h"
#include "seqlog.h"
#include "seqdefs.h"


static const SEQ_T leds_t[] =
{
	{ UNIQ|NO_ARG,	NO_ARG,	0,		0,	0,		0,	ONE_SHOT		},		/*  NO_LIT */
	{ LED_ON,		NO_ARG,	1,		0,	1,		1,	LIT     		},		/*	LINE_BATT, SIGNAL_STRENGTH	*/

	{ LED_ON,		NO_ARG,	5,		15,	1,		20, LINE_BATTCHG	},		/*	LINE_BATTCHG,NOSVC				*/
	{ LED_ON,		NO_ARG,	6,		4,	3,		40, LINE_NOBATT		},		/*	LINE_NOBATT									*/
	{ LED_ON,		NO_ARG,	5,		1,	1,		40, NOLINE_BATT		},		/*	NOLINE_BATT									*/
	{ LED_ON,		NO_ARG,	5,		5,	1,		10, NOLINE_BATTDIS	},		/*	NOLINE_BATTDIS								*/

	{ LED_ON,		NO_ARG,	2,		1,	1,		20, LSTAGE1		    },		/* 	*/
	{ LED_ON,		NO_ARG,	2,		1,	2,		20, LSTAGE2		    },		/* 	*/
	{ LED_ON,		NO_ARG,	2,		1,	3,		20, LSTAGE3		    },		/* 	*/
	{ LED_ON,		NO_ARG,	2,		1,	4,		20, LSTAGE4		    },		/* 	*/
	{ LED_ON,		NO_ARG,	2,		2,	1,		4, 	LFAIL		    },		/* 	*/
};

const MAJOR_T majors_t[ NUM_MAJOR ] =
{
	{ set_hard_leds,	leds_t,		NUM_LEDS_CHN,	NUM_LEDS	}
};


