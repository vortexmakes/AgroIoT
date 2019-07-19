/*
 * tplact.h
 */

#ifndef __TPLACT_H__
#define __TPLACT_H__

#include "mytypes.h"
#include "tplink.h"

MUInt proc_tpl_idle( MUInt input, uchar data );
MUInt proc_tpl_inesc( MUInt input, uchar data );
#if TPLINK_ADDRESSING_ON
MUInt proc_tpl_waddr( MUInt input, uchar data );
#endif
MUInt proc_tpl_wndatah( MUInt input, uchar data );
MUInt proc_tpl_wndatal( MUInt input, uchar data );
MUInt proc_tpl_indata( MUInt input, uchar data );
MUInt proc_tpl_wchk( MUInt input, uchar data );
MUInt proc_tpl_inxmit( MUInt input, uchar data );
MUInt proc_tpl_xmitesc( MUInt input, uchar data );
MUInt proc_tpl_xchk( MUInt input, uchar data );
MUInt proc_tpl_wxmit( MUInt input, uchar data );
MUInt proc_tpl_wxmitcmp( MUInt input, uchar data );

void start_xmit( TPLFRM_T *p );

#endif
