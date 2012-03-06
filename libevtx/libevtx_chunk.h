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

#include "libevtx_io_handle.h"
#include "libevtx_libbfio.h"
#include "libevtx_libfcache.h"
#include "libevtx_libfdata.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libevtx_chunk libevtx_chunk_t;

struct libevtx_chunk
{
	/* TODO implement something useful */
	int dummy;
};

int libevtx_chunk_initialize(
     libevtx_chunk_t **chunk,
     liberror_error_t **error );

int libevtx_chunk_free(
     libevtx_chunk_t **chunk,
     liberror_error_t **error );

int libevtx_chunk_read(
     libevtx_chunk_t *chunk,
     libbfio_handle_t *file_io_handle,
     libevtx_io_handle_t *io_handle,
     libfdata_vector_t *pages_vector,
     libfcache_cache_t *pages_cache,
     liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

