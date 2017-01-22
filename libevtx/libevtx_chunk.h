/*
 * Chunk functions
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

#if !defined( _LIBEVTX_CHUNK_H )
#define _LIBEVTX_CHUNK_H

#include <common.h>
#include <types.h>

#include "libevtx_io_handle.h"
#include "libevtx_libbfio.h"
#include "libevtx_libcdata.h"
#include "libevtx_libcerror.h"
#include "libevtx_record_values.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libevtx_chunk libevtx_chunk_t;

struct libevtx_chunk
{
	/* The chunk data
	 */
	uint8_t *data;

	/* The chunk data size
	 */
	size_t data_size;

	/* The (chunk) file offset
	 */
	off64_t file_offset;

	/* The records array
	 */
	libcdata_array_t *records_array;

	/* The recovered records array
	 */
	libcdata_array_t *recovered_records_array;

	/* Various flags
	 */
	uint8_t flags;
};

int libevtx_chunk_initialize(
     libevtx_chunk_t **chunk,
     libcerror_error_t **error );

int libevtx_chunk_free(
     libevtx_chunk_t **chunk,
     libcerror_error_t **error );

int libevtx_chunk_read(
     libevtx_chunk_t *chunk,
     libevtx_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     libcerror_error_t **error );

int libevtx_chunk_get_number_of_records(
     libevtx_chunk_t *chunk,
     uint16_t *number_of_records,
     libcerror_error_t **error );

int libevtx_chunk_get_record(
     libevtx_chunk_t *chunk,
     uint16_t record_index,
     libevtx_record_values_t **record_values,
     libcerror_error_t **error );

int libevtx_chunk_get_number_of_recovered_records(
     libevtx_chunk_t *chunk,
     uint16_t *number_of_records,
     libcerror_error_t **error );

int libevtx_chunk_get_recovered_record(
     libevtx_chunk_t *chunk,
     uint16_t record_index,
     libevtx_record_values_t **record_values,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBEVTX_CHUNK_H ) */

