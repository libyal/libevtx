/*
 * The event record definition of a Windows XML Event Log (EVTX) file
 *
 * Copyright (C) 2011-2017, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This software is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this software.  If not, see <http://www.gnu.org/licenses/>.
 */

#if !defined( _EVTX_EVENT_RECORD_H )
#define _EVTX_EVENT_RECORD_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct evtx_event_record_header evtx_event_record_header_t;

struct evtx_event_record_header
{
	/* The file signature
	 * Consists of 4 bytes
	 * Consists of: "\x2a\x2a\x00\x00"
	 */
	uint8_t signature[ 4 ];

	/* The size
	 * Consists 4 bytes
	 */
	uint8_t size[ 4 ];

	/* The identifier
	 * Consists 8 bytes
	 */
	uint8_t identifier[ 8 ];

	/* The written time
	 * Consists 8 bytes
	 * Contains a filetime
	 */
	uint8_t written_time[ 8 ];
};

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _EVTX_EVENT_RECORD_H ) */

