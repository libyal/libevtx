/*
 * The chunk definition of a Windows XML Event Log (EVTX) file
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

#if !defined( _EVTX_CHUNK_H )
#define _EVTX_CHUNK_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct evtx_chunk_header evtx_chunk_header_t;

struct evtx_chunk_header
{
	/* The file signature
	 * Consists of 8 bytes
	 * Consists of: "ElfChnk\x00"
	 */
	uint8_t signature[ 8 ];

	/* The first event record number
	 * Consists of 8 bytes
	 */
	uint8_t first_event_record_number[ 8 ];

	/* The last event record number
	 * Consists of 8 bytes
	 */
	uint8_t last_event_record_number[ 8 ];

	/* The first event record identifier
	 * Consists of 8 bytes
	 */
	uint8_t first_event_record_identifier[ 8 ];

	/* The last event record identifier
	 * Consists of 8 bytes
	 */
	uint8_t last_event_record_identifier[ 8 ];

	/* The header size
	 * Consists of 4 bytes
	 */
	uint8_t header_size[ 4 ];

	/* The last event record offset
	 * Consists 4 bytes
	 */
	uint8_t last_event_record_offset[ 4 ];

	/* The free space offset
	 * Consists 4 bytes
	 */
	uint8_t free_space_offset[ 4 ];

	/* Event records checksum
	 * Consists 4 bytes
	 */
	uint8_t event_records_checksum[ 4 ];

	/* Unknown
	 * Consists 64 bytes
	 */
	uint8_t unknown1[ 64 ];

	/* Unknown
	 * Consists 4 bytes
	 */
	uint8_t unknown2[ 4 ];

	/* Checksum
	 * Consists of 4 bytes
	 * Contains a CRC32 of bytes 0 to 120 and 128 to 512
	 */
	uint8_t checksum[ 4 ];
};

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _EVTX_CHUNK_H ) */

