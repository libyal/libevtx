/*
 * The file header definition of a Windows XML Event Log (EVTX) file
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

#if !defined( _EVTX_FILE_HEADER_H )
#define _EVTX_FILE_HEADER_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct evtx_file_header evtx_file_header_t;

struct evtx_file_header
{
	/* The file signature
	 * Consists of 8 bytes
	 * Consists of: "ElfFile\x00"
	 */
	uint8_t signature[ 8 ];

	/* The first chunk number
	 * Consists of 8 bytes
	 */
	uint8_t first_chunk_number[ 8 ];

	/* The last chunk number
	 * Consists of 8 bytes
	 */
	uint8_t last_chunk_number[ 8 ];

	/* The next record identifier
	 * Consists of 8 bytes
	 */
	uint8_t next_record_identifier[ 8 ];

	/* The header size
	 * Consists of 4 bytes
	 */
	uint8_t header_size[ 4 ];

	/* The minor version
	 * Consists of 2 bytes
	 */
	uint8_t minor_version[ 2 ];

	/* The major version
	 * Consists of 2 bytes
	 */
	uint8_t major_version[ 2 ];

	/* The header block_size
	 * Consists of 2 bytes
	 */
	uint8_t header_block_size[ 2 ];

	/* The number of chunks
	 * Consists of 2 bytes
	 */
	uint8_t number_of_chunks[ 2 ];

	/* Unknown
	 * Consists 76 bytes
	 */
	uint8_t unknown1[ 76 ];

	/* The file flags
	 * Consists of 4 bytes
	 */
	uint8_t file_flags[ 4 ];

	/* Checksum
	 * Consists of 4 bytes
	 * Contains a CRC32 of bytes 0 to 120
	 */
	uint8_t checksum[ 4 ];
};

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _EVTX_FILE_HEADER_H ) */

