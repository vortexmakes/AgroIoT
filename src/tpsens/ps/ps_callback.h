/*
 *	file: ps_callback.h
 *	Last updated for version: 1.0.00
 *	Date of the last update:  Oct 20, 2011
 *
 *  Copyright (C) 2010 Leandro Francucci. All rights reserved.
 *
 *  RKH is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  RKH is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with RKH, see copying.txt file.
 *
 * Contact information:
 * e-mail:			francuccilea@gmail.com
 */

/**
 *  \file ps_callback.h
 *	\brief
 *  Defines Protocol scheduler's callbacks
 *
 *  PROSENS uses the callback mechanism to notify events
 *  to application code. Each of callbacks could be enabled or
 *  disabled from the pscfg.h file using the preprocessor directives.
 */

#ifndef __PS_CALLBACK_H__
#define __PS_CALLBACK_H__

#include "ps.h"

/**
 *  /brief
 *  This function is called when the protocol starts a new polling cycle.
 */
void ps_onStartCycle(void);

/**
 *  /brief
 *  This function is called when the protocol stops the polling.
 */
void ps_onStop(void);

/**
 *  /brief
 *  This function is called when the protocol ends a polling cycle.
 *  Use ps_restart() function to start a new polling cycle.
 */

void ps_onEndCycle(void);

/**
 *  Station's callbacks.
 *
 *  PROSENS uses the callback mechanism to notify events
 *  to application code. Each of callbacks could be enabled or
 *  disabled from the pscfg.h file using the preprocessor directives.
 */

/**
 *  /brief
 *  This function is called when the station has sent data to the master.
 *
 *  Every station could be defined its own action on this situation,
 *  to do that, use the station parameter within this function.
 */
void ps_onStationRecv(ST_T station, PS_PLBUFF_T *pb);

/**
 *  /brief
 *  This function is called when the station is out of polling.
 *
 *  Every station could be defined its own action on this situation,
 *  to do that, use the station parameter within this function.
 */
void ps_onStationStop(ST_T station, uchar reason);

/**
 *  /brief
 *  This function is called when the station is added into the polling.
 *
 *  Every station could be defined its own action on this situation,
 *  to do that, use the station parameter within this function.
 */
void ps_onStationRun(ST_T station);

/**
 *  /brief
 *  If no reply is received, the response timeout expires, and this
 *  function is called.
 *  The maximum number of retries is defined by means of
 *  PS_NUM_RETRIES option.
 *
 *  Every station could be defined its own action on this situation,
 *  to do that, use the station parameter within this function.
 */
void ps_onStationTout(ST_T station);

/**
 *  /brief
 *  This function is called when the master sends the request command to
 *  the station. Every station could be defined its own request command,
 *  to do that, use the station parameter within this function.
 *
 *  Therefore, use PS_PLBUFF_T structure to store the request command
 *  payload. Thus, when invoking the ps_onStationReq() callback the protocol
 *  scheduler will send to station the defined request command.
 *
 *	/returns
 *	The pointer to the request command.
 */
PS_PLBUFF_T *ps_onStationReq(ST_T station);

#endif
/* ------------------------------ End of file ------------------------------ */
