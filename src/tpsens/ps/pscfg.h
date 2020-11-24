/*
 *	file: pscfg.h
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
 *  \file pscfg.h
 *	\brief
 *	Protocol configuration file.
 *
 *  This file adapts and configures PROSENS by means of compiler definitions
 *  and macros allowing to restrict the resources consumed by PROSENS.
 *  Adjusting this definitions allows to reduce the ROM and RAM consumption,
 *  and to enhance the system performance in a substantial manner.
 */

#ifndef __PSCFG_H__
#define __PSCFG_H__

#include "cbox.h"

/**
 *  Polling configurations.
 */

#define PS_NUM_STATIONS                 NUM_SENSORS_BOX
#define PS_NUM_STARTUP_CYCLES           2
#define PS_NUM_RETRIES                  4
#define PS_MAX_PAYLOAD_SIZE             16
#define PS_MAX_REQUEST_SIZE             1

/**
 *  Debug configurations.
 */

#define PS_DEBUG                        0
#define PS_PRINT_FORMAT                 0
#define PS_PRINT_FORMAT_SIZE            64

/**
 *  Callback configurations.
 */

#define PS_EN_ON_STARTCYCLE             1   /* If it's enabled (1)  */
#define PS_EN_ON_ENDCYCLE               1
#define PS_EN_ON_STOP                   1

#define PS_EN_ON_ST_STOP                1
#define PS_EN_ON_ST_RUN                 1
#define PS_EN_ON_ST_TOUT                0

/**
 *  Facility and service configurations.
 */

#define PS_EN_STOP                      1
#define PS_EN_RESTART                   1
#define PS_EN_GET_INFO                  0
#define PS_EN_START_STATION             1
#define PS_EN_STOP_STATION              0
#define PS_EN_SETQUEUE_STATION          0

#endif
/* ------------------------------ End of file ------------------------------ */
