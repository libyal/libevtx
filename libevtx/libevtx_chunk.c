/*
 * Database functions
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

#include <common.h>
#include <memory.h>
#include <types.h>

#include <liberror.h>
#include <libnotify.h>

#include "libevtx_definitions.h"
#include "libevtx_chunk.h"
#include "libevtx_io_handle.h"
#include "libevtx_libbfio.h"
#include "libevtx_libfcache.h"
#include "libevtx_libfdata.h"

#include "evtx_chunk.h"

/* Initialize a chunk
 * Make sure the value chunk is pointing to is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libevtx_chunk_initialize(
     libevtx_chunk_t **chunk,
     liberror_error_t **error )
{
	static char *function = "libevtx_chunk_initialize";

	if( chunk == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk.",
		 function );

		return( -1 );
	}
	if( *chunk != NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid chunk value already set.",
		 function );

		return( -1 );
	}
	*chunk = memory_allocate_structure(
	          libevtx_chunk_t );

	if( *chunk == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create chunk.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     ( *chunk ),
	     0,
	     sizeof( libevtx_chunk_t ) ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear chunk.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *chunk != NULL )
	{
		memory_free(
		 *chunk );

		*chunk = NULL;
	}
	return( -1 );
}

/* Frees a chunk
 * Returns 1 if successful or -1 on error
 */
int libevtx_chunk_free(
     libevtx_chunk_t **chunk,
     liberror_error_t **error )
{
	static char *function = "libevtx_chunk_free";

	if( chunk == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk.",
		 function );

		return( -1 );
	}
	if( *chunk != NULL )
	{
		memory_free(
		 *chunk );

		*chunk = NULL;
	}
	return( 1 );
}

/* Reads the chunk
 * Returns 1 if successful or -1 on error
 */
int libevtx_chunk_read(
     libevtx_chunk_t *chunk,
     libbfio_handle_t *file_io_handle,
     libevtx_io_handle_t *io_handle,
     libfdata_vector_t *pages_vector,
     libfcache_cache_t *pages_cache,
     liberror_error_t **error )
{
	static char *function = "libevtx_chunk_read";

	if( chunk == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk.",
		 function );

		return( -1 );
	}
/* TODO */
	return( -1 );
}

