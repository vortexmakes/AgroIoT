/*
 *	file: ps.h
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
 *  \file ps.h
 *	\brief
 *  PROSENS platform-independent interfaces.
 *
 *  The PROSENS protocol uses a bus master to poll slave devices distributed
 *  in multi-drop fashion on an RS485 serial bus.  A PROSENS slave is any
 *  peripheral device which processes information and sends its output to
 *  the master.  The slave forms a "passive station" on the network since it
 *  does not have bus access rights, and can only acknowledge received
 *  messages, or send response messages to the master upon request.
 *  It is important to note that all stations have the same priority,
 *  and all network communication originates from the master.
 *
 *  During startup, after a system reset, or upon return to power, a master
 *  will attempt to re-establish contact with all the slaves assigned to it
 *  before assuming the cyclic exchange of I/O data. This process is called
 *  "startup cycle", which will attempt to find out devices on the network.
 *  Each slave must already have a unique valid address from
 *  0-PS_MAX_ADDRESS_RANGE in order to communicate with the master.
 *
 *  \note
 *  This header file must be included in all modules (*.c files) that
 *  use PROSENS.
 */

#ifndef __PS_H__
#define __PS_H__

#include "mytypes.h"
#include "pscfg.h"

/**
 *	Message signals.
 */

enum
{
    /**
     *	Produced by protocol scheduler.
     *	Indicates that it's the turn of current station.
     */

    PS_SIG_SEND_NEXT,

    /**
     *	Produced by data link layer.
     *	Indicates that has been received a valid packet from the
     *	other station.
     */

    PS_SIG_RESPONSE,

    /**
     *	Produced by callback timer.
     *	If no reply is received, the response timeout expires, and an
     *	error is generated.
     *	The maximum number of retries is defined by means of
     *	PS_NUM_RETRIES option.
     */

    PS_SIG_TOUT,

    /**
     *	Produced by application level.
     *	Indicates that must be turn on the polling.
     */

    PS_SIG_RUN,

    /**
     *	Produced by protocol scheduler.
     */

    PS_SIG_ENABLE,

    /**
     *	Produced by protocol scheduler.
     */

    PS_SIG_DISABLE,

    PS_NUM_SIGNALS
};

/**
 *  \brief
 *  This codes Returned code from station's state machine on called
 *  the ps_dispatch_to_station() function.
 */

enum
{
    RET_UNAVAILABLE,
    RET_OK,
    RET_WAITRESP,
};

/**
 *  \brief
 *  Defines the data type of station address.
 */

typedef unsigned char ST_T;

/**
 *  \brief
 *  Used to store the payload to send.
 */

typedef struct
{
    uchar qty;
    uchar payload[PS_MAX_PAYLOAD_SIZE];
} PS_PLBUFF_T;

/**
 *  \brief
 *  Represents event messages without parameters.
 *
 *  An event can have associated parameters, allowing the event
 *  instance to convey not only the occurrence of some interesting
 *  incident but also quantitative information regarding that occurrence.
 *	Implementing the single inheritance in C is very simply by literally
 *	embedding the base structure, PSM_T in this case, as the first
 *	member of the derived structure.
 */

typedef struct
{
    /**
     *  Signal of the message instance.
     */

    uchar m;
} PSM_T;

/**
 *  \brief
 *  Describes the message to be directly dispatched to protocol scheduler.
 *  Therefore, the 'm' member must be set to 'PS_SIG_RESPONSE'.
 */

typedef struct
{
    PSM_T m;
    ST_T station;
    PS_PLBUFF_T buff;
} PSM_PKT_T;

/**
 *  \brief
 *  Describes the message to be directly dispatched to protocol scheduler.
 *  Therefore, the 'm' member must be set to 'PS_SIG_TOUT'.
 */

typedef struct
{
    PSM_T m;
} PSM_TOUT_T;

/**
 *  Application interfaces.
 */

/**
 *	\brief
 *	This function is used to print on console a formatted string
 *	useful in debug time.
 *
 *	\param out		selects the output.
 *	\param fmt		formatted string.
 */

void print_format(uchar out, char *fmt, ...);

/**
 *	\brief
 *	Inits the protocol module, and prepare it to be started.
 */

void ps_init(void);

/**
 *	\brief
 *	Starts the protocol scheduler. Generally, this function is invoked
 *	after of ps_init() calling.
 */

void ps_start(void);

/**
 *	\brief
 *	When ending the polling cycle the protocol must be started.
 */

void ps_restart(void);

/**
 *	\brief
 *	Stops the protocol scheduler.
 */

void ps_stop(void);

/**
 *  \brief
 *  Shows dynamic information of protocol.
 *
 *  \note
 *  To use it, must be set to one (1) the PS_EN_GET_INFO, PS_DEBUG and
 *  PS_PRINT_FORMAT preprocessor directives from pscfg.h file.
 */

void ps_get_info(void);

/**
 *	\brief
 *  This function forces a station to be included into the polling.
 *
 *	\param station	station address.
 */

void ps_start_station(ST_T station);

/**
 *	\brief
 *  This function forces a station to be excluded from the polling.
 *
 *	\param station	station address.
 */

void ps_stop_station(ST_T station);

/**
 *  \brief
 *  This function configures the station queue descriptor.
 *
 *  \param station  station address.
 *  \param qd		queue descriptor.
 */

void ps_setqueue_station(ST_T station, uchar qd);

/**
 *  \brief
 *  This function posts a message to the station's queue.
 *
 *  \param station	station address.
 *  \param pd		pointer to data to be sent.
 *  \param size		size of data [in bytes].
 */

void ps_send_station(ST_T station, PS_PLBUFF_T *pl);

/**
 *  \brief
 *	This function is called (DLL context) on frame timer expiration.
 */

void ps_timeout(void);

/**
 *  \brief
 *	This function is called (DLL context) on received packet from a station.
 *
 *  \param pm		pointer to message that contains a received packet
 *                  (protocol payload) from dll.
 */

void ps_rcv_packet(PSM_PKT_T *pm);

#endif
/* ------------------------------ End of file ------------------------------ */
