/*
 *	file: psport.h
 *	Last updated for version: 1.0.00
 *	Date of the last update:  October 20, 2011
 *
 * 	Copyright (C) 2010 Leandro Francucci. All rights reserved.
 *
 * 	RKH is free software: you can redistribute it and/or modify
 * 	it under the terms of the GNU General Public License as published by
 * 	the Free Software Foundation, either version 3 of the License, or
 * 	(at your option) any later version.
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
 * 	\file psport.h
 *	\brief
 *
 *	Defines platform independant interfaces.
 */


#ifndef __PSPORT_H__
#define __PSPORT_H__


#include <stdio.h>
#include "mytypes.h"
#include "ps.h"
#include "rkh.h"
#include "tplink.h"


/*
 *	##### Include dependant files #####
 */



/*
 *	##### Defines the critical section interface #####
 */

#define ps_enter_critical()		RKH_ENTER_CRITICAL_()
#define ps_exit_critical()		RKH_EXIT_CRITICAL_()


/*
 *	##### Defines the data link layer interface #####
 */
#define dll_init()				tplink_init()
#define dll_send_frame(s, p)	tplink_send_frame((s),(p)->payload,(p)->qty)


/*
 *	##### Defines the queue service interface #####
 */


/**
 * 	\brief
 * 	Defines the data type of queue descriptor.
 */

#define PSQD_T						uchar


/**
 * 	\brief
 * 	This macro query the station queue.
 *
 * 	\param q	station queue descriptor.
 * 	\return
 *
 * 	'1' if queue is empty, otherwise '0'.
 */

#define PSQ_IS_EMPTY( q )			1

/**
 *
 * 	\brief
 *	Inserts an element on a queue. 
 *
 *	The element is queued by copy, not by reference.
 *
 * 	\param q	station queue descriptor.
 * 	\param p 	pointer to the element that is to be placed on the queue.  
 *				The size of the elements the queue will hold was defined 
 *				when the queue was created.
 *
 * 	\return
 *
 * 	PSQ_OK if the element was successfully inserted, 
 *	otherwise error code.
 */

#define PSQ_INSERT( q, p );	


/**
 * 	\brief
 *	Read an element from a queue without remove it. 
 *
 *	The element is received by copy so a buffer of adequate size 
 *	must be provided. The number of bytes copied into the buffer was 
 *	defined when the queue was created.
 *
 * 	\param q	station queue descriptor.
 * 	\param p	pointer to the buffer into which the received item will be 
 * 				copied.
 *
 * 	\return
 *
 * 	PSQ_OK if an element was successfully readed from the 
 * 	queue, otherwise error code.
 */

#define PSQ_READ( q, p );


/**
 * 	\brief
 * 	Removes an element from a queue. 
 *
 * 	The element is received by copy so a buffer of adequate size 
 * 	must be provided. The number of bytes copied into the buffer was 
 * 	defined when the queue was created.
 *
 * 	\param q	station queue descriptor.
 * 	\param p 	pointer to the buffer into which the received item will be 
 * 				copied.
 *
 * 	\return
 *
 * 	PSQ_OK if an element was successfully removed from the 
 * 	queue, otherwise error code.
 */

#define PSQ_REMOVE( q, b );


/**
 * 	\brief
 * 	Depletes a station queue.
 *
 * 	\param q	station queue descriptor.
 */

#define PSQ_DEPLETE( q );


/*
 *	##### Defines debug interface #####
 */

#define psput_string(ch,x)			put_string(ch,(x))
#define psput_char(ch,x)			put_char(ch,(x))

#if PS_DEBUG == 1
#define psdprint(x)					ctprintf##x
#else
#define psdprint(x)
#endif


#endif
