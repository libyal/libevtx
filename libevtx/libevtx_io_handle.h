/*
 * Input/Output (IO) handle functions
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

#if !defined( _LIBEVTX_IO_HANDLE_H )
#define _LIBEVTX_IO_HANDLE_H

#include <common.h>
#include <types.h>

#include "libevtx_libbfio.h"
#include "libevtx_libcerror.h"
#include "libevtx_libfcache.h"
#include "libevtx_libfdata.h"

#if defined( __cplusplus )
extern "C" {
#endif

extern const uint8_t *evtx_file_signature;

typedef struct libevtx_io_handle libevtx_io_handle_t;

struct libevtx_io_handle
{
	/* The major version
	 */
	uint16_t major_version;

	/* The minor version
	 */
	uint16_t minor_version;

	/* The number of chunks
	 */
	uint16_t number_of_chunks;

	/* The chunks data offset
	 */
	off64_t chunks_data_offset;

	/* The chunks data size
	 */
	size64_t chunks_data_size;

	/* The file flags
	 */
	uint32_t file_flags;

	/* The chunk size
	 */
	uint32_t chunk_size;

	/* Various flags
	 */
	uint8_t flags;

	/* The first record identifier
	 */
	uint64_t first_record_identifier;

	/* The last record identifier
	 */
	uint64_t last_record_identifier;

	/* The codepage of the ASCII strings
	 */
	int ascii_codepage;

	/* Value to indicate if abort was signalled
	 */
	int abort;
};

int libevtx_io_handle_initialize(
     libevtx_io_handle_t **io_handle,
     libcerror_error_t **error );

int libevtx_io_handle_free(
     libevtx_io_handle_t **io_handle,
     libcerror_error_t **error );

int libevtx_io_handle_clear(
     libevtx_io_handle_t *io_handle,
     libcerror_error_t **error );

int libevtx_io_handle_read_file_header(
     libevtx_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     libcerror_error_t **error );

int libevtx_io_handle_read_chunk(
     libevtx_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfdata_vector_t *vector,
     libfcache_cache_t *cache,
     int element_index,
     int element_data_file_index,
     off64_t element_data_offset,
     size64_t element_data_size,
     uint32_t element_data_flags,
     uint8_t read_flags,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBEVTX_IO_HANDLE_H ) */

