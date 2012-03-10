/*
 * Chunk functions
 *
 * Copyright (c) 2011-2012, Joachim Metz <jbmetz@users.sourceforge.net>
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

#include <liberror.h>

#include "libevtx_array_type.h"
#include "libevtx_io_handle.h"
#include "libevtx_libbfio.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libevtx_chunk_value libevtx_chunk_value_t;

struct libevtx_chunk_value
{
	/* The reference to the chunk value data
	 */
	uint8_t *data;

	/* The chunk value size
	 */
	uint16_t size;

	/* The offset of the chunk value
	 */
	uint16_t offset;

	/* The flags of the value
	 */
	uint8_t flags;
};

typedef struct libevtx_chunk libevtx_chunk_t;

struct libevtx_chunk
{
	/* The chunk data
	 */
	uint8_t *data;

	/* The chunk data size
	 */
	size_t data_size;

	/* The chunk values array
	 */
	libevtx_array_t *values_array;
};

int libevtx_chunk_value_initialize(
     libevtx_chunk_value_t **chunk_value,
     liberror_error_t **error );

int libevtx_chunk_value_free(
     libevtx_chunk_value_t **chunk_value,
     liberror_error_t **error );

int libevtx_chunk_initialize(
     libevtx_chunk_t **chunk,
     liberror_error_t **error );

int libevtx_chunk_free(
     libevtx_chunk_t **chunk,
     liberror_error_t **error );

int libevtx_chunk_read(
     libevtx_chunk_t *chunk,
     libevtx_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     liberror_error_t **error );

int libevtx_chunk_get_number_of_values(
     libevtx_chunk_t *chunk,
     uint16_t *number_of_values,
     liberror_error_t **error );

int libevtx_chunk_get_value(
     libevtx_chunk_t *chunk,
     uint16_t value_index,
     libevtx_chunk_value_t **chunk_value,
     liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

