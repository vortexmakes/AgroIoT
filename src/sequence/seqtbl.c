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
		/* NO_LIT */
	{ UNIQ|NO_ARG,	NO_ARG,	0,  0,	0,  0,	ONE_SHOT            },

		/* LINE_BATT, SIGNAL_STRENGTH */
	{ LED_ON,		NO_ARG,	1,  0,	1,  1,	SEQ_LIT             },

		/* LINE_BATTCHG,NOSVC */
	{ LED_ON,		NO_ARG,	5,  15,	1,  20, SEQ_LINE_BATTCHG    },

		/* LINE_NOBATT */
	{ LED_ON,		NO_ARG,	6,  4,	3,  40, SEQ_LINE_NOBATT     },

		/* NOLINE_BATT */
	{ LED_ON,		NO_ARG,	5,  1,	1,  40, SEQ_NOLINE_BATT     },

		/* NOLINE_BATTDIS */
	{ LED_ON,		NO_ARG,	5,  5,	1,  10, SEQ_NOLINE_BATTDIS  },

	{ LED_ON,		NO_ARG,	2,  1,	1,  20, SEQ_LSTAGE1         },
	{ LED_ON,		NO_ARG,	2,  1,	2,  20, SEQ_LSTAGE2         },
	{ LED_ON,		NO_ARG,	2,  1,	3,  20, SEQ_LSTAGE3         },
	{ LED_ON,		NO_ARG,	2,  1,	4,  20, SEQ_LSTAGE4         },
	{ LED_ON,		NO_ARG,	2,  2,	1,  4, 	SEQ_LFAIL           },
};

const MAJOR_T majors_t[ NUM_MAJOR ] =
{
	{ set_hard_leds,	leds_t,		NUM_LEDS_CHN,	SEQ_NUM_LEDS	}
};


