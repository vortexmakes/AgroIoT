/*
 *	seqlog.c
 */

#include "mytypes.h"
#include "seqlog.h"

void
set_hard_leds( unsigned short minor, MUInt arga, MUInt argb )
{
    (void)minor;
    (void)arga;
    (void)argb;
  
#ifdef __PRINTF__
	printf( "set_hard_leds: %u, %d, %d\n", minor, arga, argb );
#endif
}

