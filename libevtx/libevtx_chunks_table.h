/*
 * Chunks table functions
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

#if !defined( _LIBEVTX_CHUNKS_TABLE_H )
#define _LIBEVTX_CHUNKS_TABLE_H

#include <common.h>
#include <types.h>

#include "libevtx_io_handle.h"
#include "libevtx_libbfio.h"
#include "libevtx_libcerror.h"
#include "libevtx_libfcache.h"
#include "libevtx_libfdata.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libevtx_chunks_table libevtx_chunks_table_t;

struct libevtx_chunks_table
{
	/* The IO handle
	 */
	libevtx_io_handle_t *io_handle;

	/* The chunks vector
	 */
	libfdata_vector_t *chunks_vector;

	/* The chunks cache
	 */
	libfcache_cache_t *chunks_cache;
};

int libevtx_chunks_table_initialize(
     libevtx_chunks_table_t **chunks_table,
     libevtx_io_handle_t *io_handle,
     libfdata_vector_t *chunks_vector,
     libfcache_cache_t *chunks_cache,
     libcerror_error_t **error );

int libevtx_chunks_table_free(
     libevtx_chunks_table_t **chunks_table,
     libcerror_error_t **error );

int libevtx_chunks_table_read_record(
     intptr_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfdata_list_element_t *list_element,
     libfcache_cache_t *cache,
     int data_range_file_index,
     off64_t data_range_offset,
     size64_t data_range_size,
     uint32_t data_range_flags,
     uint8_t read_flags,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBEVTX_CHUNKS_TABLE_H ) */

