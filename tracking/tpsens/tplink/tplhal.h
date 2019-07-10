/*
 * tplhal.h
 */

#ifndef __TPLHAL_H__
#define __TPLHAL_H__

#include "mytypes.h"
#include "tplcfg.h"
#include "rkh.h"

void init_tpl_hal( void );
void deinit_tpl_hal( void );

void tplink_rx_isr( unsigned char byte );
void tplink_tx_isr( void );

void tpl_choke_xmit( uchar data );
void tpl_xmit( uchar data );
void tpl_stop_xmit( void );
void tpl_eoftx( void );

#define tpl_enter_critical()   RKH_SR_ALLOC(); RKH_ENTER_CRITICAL_()
#define tpl_exit_critical()    RKH_EXIT_CRITICAL_()

#if __TPLINK_DBG__ || TREO_DBG

#include <stdio.h>
#define mprintf(x)	printf x
#else
#define mprintf(x)	(void)0
#endif

#endif
