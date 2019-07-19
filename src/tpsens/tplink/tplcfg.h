/*
 * tplcfg.h
 *
 *	<SOF> <addr> <nDataH> <nDataL> <Data-n> …. <Data-0> <CHK-H> <CHK-L>
 *
 *	SOF: 0xF1
 *
 *	ESC: 0xF2
 *	ESC_MASK: 0x0F
 *		Escape codes:
 *			0x01 – SOF
 *			0x02 – ESC 
 *
 *	Checksum:
 *		mod-256, from nData..Data-n splitted each nibble in 2 bytes, 
 *		right justified plus CHECK_OFFSET (0xA0), then these are bounded to 
 *		0xA0 - 0xAF. 
 *		Then checksum bytes never has to be escaped.
 */	

#ifndef __TPLCFG_H__
#define __TPLCFG_H__

#include "mytypes.h"

#define __TPLINK_DBG__	0

#ifndef TPLINK_DEV_TYPE
#define TPLINK_DEV_TYPE			TPLINK_DEV_MASTER	/* Device type */
#endif


#ifndef TPLINK_ADDRESSING_ON
#define TPLINK_ADDRESSING_ON	1		/* 1/0 Enable/Disable Protocol Slaves Addressing */
#endif


#ifndef TPLINK_SLAVE_ADDRESS
#define TPLINK_SLAVE_ADDRESS	0x00	/* Device Address [0x00 .. 0x7F] */
#endif

#ifndef TPLINK_MAX_PLOAD_SIZE
#define TPLINK_MAX_PLOAD_SIZE	512		/* Max Payload Size */
#endif

#define TPLINK_SOF				0xF1	/* Start of Frame "MARK" */
#define TPLINK_ESC				0xF2	/* Escape character */

#define TPLINK_BSP_SUPPORT_TC_IRQ	0	/*	On Allmost embedded platforms based on MCU we port
										 	TPLINK to the lower level hardware accesing to MCU registers,
										 	in this case, depending of MCU, we may obtain an IRQ when 
											Transmition	is Complete (TC).
										 	In the other hand, for example in Windows Environment, 
											the bsp don't inform to us when the transmision is really
											completed.
											Then in order to bring a rule to set this option, 
											except Windows Platforms, this option is set to 1.
										*/

#define TPLINK_BASE_TIME		10		/* Tplink base time tick */

#ifndef TPLINK_INTER_BYTE_TIME
#define TPLINK_INTER_BYTE_TIME	(40/TPLINK_BASE_TIME)	/* Inter Byte timeout in ms */
#endif

#ifndef TPLINK_VAR_FRMTOUT
#define TPLINK_VAR_FRMTOUT		0		/* Variable Frame Timout */
#endif

#if ( TPLINK_VAR_FRMTOUT == 0 )
#ifndef TPLINK_FRMTOUT_DFT
#define TPLINK_FRMTOUT_DFT		(1000/TPLINK_BASE_TIME) /* Inter Frame timeout in ms */
#endif
#endif

#define __USE_TPLINK_ONXMIT_CMP_CBF__	0			/* Enable/Disable transmit complete call back function */
#define __USE_TPLINK_ONCHERR_CBF__		1			/* Enable/Disable check sum error call back function */

#endif
