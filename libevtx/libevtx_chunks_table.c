/*
 * Chunks table functions
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

#include "libevtx_chunk.h"
#include "libevtx_chunks_table.h"
#include "libevtx_io_handle.h"
#include "libevtx_libbfio.h"
#include "libevtx_libcerror.h"
#include "libevtx_libfcache.h"
#include "libevtx_libfdata.h"
#include "libevtx_record_values.h"
#include "libevtx_unused.h"

/* Initialize chunks table
 * Make sure the value chunks table is pointing to is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libevtx_chunks_table_initialize(
     libevtx_chunks_table_t **chunks_table,
     libevtx_io_handle_t *io_handle,
     libfdata_vector_t *chunks_vector,
     libfcache_cache_t *chunks_cache,
     libcerror_error_t **error )
{
	static char *function = "libevtx_chunks_table_initialize";

	if( chunks_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunks table.",
		 function );

		return( -1 );
	}
	if( *chunks_table != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid chunks table value already set.",
		 function );

		return( -1 );
	}
	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	*chunks_table = memory_allocate_structure(
	            libevtx_chunks_table_t );

	if( *chunks_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create chunks table.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     ( *chunks_table ),
	     0,
	     sizeof( libevtx_chunks_table_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear chunks table.",
		 function );


		goto on_error;
	}
	( *chunks_table )->io_handle     = io_handle;
	( *chunks_table )->chunks_vector = chunks_vector;
	( *chunks_table )->chunks_cache  = chunks_cache;

	return( 1 );

on_error:
	if( *chunks_table != NULL )
	{
		memory_free(
		 *chunks_table );

		*chunks_table = NULL;
	}
	return( -1 );
}

/* Frees chunks table
 * Returns 1 if successful or -1 on error
 */
int libevtx_chunks_table_free(
     libevtx_chunks_table_t **chunks_table,
     libcerror_error_t **error )
{
	static char *function = "libevtx_chunks_table_free";
	int result            = 1;

	if( chunks_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunks table.",
		 function );

		return( -1 );
	}
	if( *chunks_table != NULL )
	{
		memory_free(
		 *chunks_table );

		*chunks_table = NULL;
	}
	return( result );
}

/* Reads a chunk
 * Callback function for the chunk vector
 * Returns 1 if successful or -1 on error
 */
int libevtx_chunks_table_read_record(
     intptr_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfdata_list_element_t *list_element,
     libfcache_cache_t *cache,
     off64_t element_data_offset,
     size64_t element_data_size,
     uint32_t element_data_flags LIBEVTX_ATTRIBUTE_UNUSED,
     uint8_t read_flags LIBEVTX_ATTRIBUTE_UNUSED,
     libcerror_error_t **error )
{
	libevtx_chunk_t *chunk                 = NULL;
	libevtx_chunks_table_t *chunks_table   = NULL;
	libevtx_record_values_t *record_values = NULL;
	static char *function                  = "libevtx_io_handle_read_chunk";

	LIBEVTX_UNREFERENCED_PARAMETER( element_data_size );
	LIBEVTX_UNREFERENCED_PARAMETER( read_flags );

	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	chunks_table = (libevtx_chunks_table_t *) io_handle;

	if( libfdata_vector_get_element_value_at_offset(
	     chunks_table->chunks_vector,
	     file_io_handle,
	     chunks_table->chunks_cache,
	     element_data_offset,
	     (intptr_t **) &chunk,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve chunk.",
		 function );

		goto on_error;
	}
	if( chunk == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing chunk.",
		 function );

		goto on_error;
	}
/* TODO
	if( libfdata_vector_set_element_value_by_index(
	     vector,
	     cache,
	     element_index,
	     (intptr_t *) chunk,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libevtx_chunk_free,
	     LIBFDATA_LIST_ELEMENT_VALUE_FLAG_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set chunk as element value.",
		 function );

		goto on_error;
	}
*/
	return( 1 );

on_error:
	if( record_values != NULL )
	{
		libevtx_record_values_free(
		 &record_values,
		 NULL );
	}
	return( -1 );
}

