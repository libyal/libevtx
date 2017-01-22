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

/* Creates a chunks table
 * Make sure the value chunks_table is referencing, is set to NULL
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
	     *chunks_table,
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

/* Frees a chunks table
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
     int data_range_file_index LIBEVTX_ATTRIBUTE_UNUSED,
     off64_t data_range_offset,
     size64_t data_range_size,
     uint32_t data_range_flags LIBEVTX_ATTRIBUTE_UNUSED,
     uint8_t read_flags LIBEVTX_ATTRIBUTE_UNUSED,
     libcerror_error_t **error )
{
	libevtx_chunk_t *chunk                       = NULL;
	libevtx_chunks_table_t *chunks_table         = NULL;
	libevtx_record_values_t *chunk_record_values = NULL;
	libevtx_record_values_t *record_values       = NULL;
	static char *function                        = "libevtx_io_handle_read_chunk";
	size_t calculated_chunk_data_offset          = 0;
	size_t chunk_data_offset                     = 0;
	uint16_t number_of_records                   = 0;
	uint16_t record_index                        = 0;

	LIBEVTX_UNREFERENCED_PARAMETER( data_range_file_index );
	LIBEVTX_UNREFERENCED_PARAMETER( data_range_flags );
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

	/* The chunk index is stored in the data range size
	*/
	if( data_range_size > (uint64_t) UINT16_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid data range size value out of bounds.",
		 function );

		goto on_error;
	}
	if( libfdata_vector_get_element_value_by_index(
	     chunks_table->chunks_vector,
	     (intptr_t *) file_io_handle,
	     chunks_table->chunks_cache,
	     (uint16_t) data_range_size,
	     (intptr_t **) &chunk,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve chunk: %" PRIu64 ".",
		 function,
		 data_range_size );

		goto on_error;
	}
	if( chunk == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing chunk: %" PRIu64 ".",
		 function,
		 data_range_size );

		goto on_error;
	}
	if( ( data_range_offset < chunk->file_offset )
	 || ( data_range_offset >= (off64_t) ( chunk->file_offset + chunk->data_size ) ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid chunk file offset value out of bounds.",
		 function );

		goto on_error;
	}
	calculated_chunk_data_offset = (size_t) ( data_range_offset - chunk->file_offset );

	if( libevtx_chunk_get_number_of_records(
	     chunk,
	     &number_of_records,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of records from chunk.",
		 function );

		goto on_error;
	}
/* TODO optimize determining the corresponding record */
	for( record_index = 0;
	     record_index < number_of_records;
	     record_index++ )
	{
		if( libevtx_chunk_get_record(
		     chunk,
		     record_index,
		     &chunk_record_values,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve record: %" PRIu16 " from chunk.",
			 function,
			 record_index );

			goto on_error;
		}
		if( chunk_record_values == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing record: %" PRIu16 ".",
			 function,
			 record_index );

			goto on_error;
		}
		chunk_data_offset = chunk_record_values->chunk_data_offset;

		if( calculated_chunk_data_offset == chunk_data_offset )
		{
			break;
		}
	}
/* TODO allow to control look up in normal vs recovered */
	if( calculated_chunk_data_offset != chunk_data_offset )
	{
		if( libevtx_chunk_get_number_of_recovered_records(
		     chunk,
		     &number_of_records,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of recovered records from chunk.",
			 function );

			goto on_error;
		}
		for( record_index = 0;
		     record_index < number_of_records;
		     record_index++ )
		{
			if( libevtx_chunk_get_recovered_record(
			     chunk,
			     record_index,
			     &chunk_record_values,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve recovered record: %" PRIu16 " from chunk.",
				 function,
				 record_index );

				goto on_error;
			}
			if( chunk_record_values == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: missing recovered record: %" PRIu16 ".",
				 function,
				 record_index );

				goto on_error;
			}
			chunk_data_offset = chunk_record_values->chunk_data_offset;

			if( calculated_chunk_data_offset == chunk_data_offset )
			{
				break;
			}
		}
	}
	if( calculated_chunk_data_offset != chunk_data_offset )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: no record found at offset: %" PRIi64 ".",
		 function,
		 data_range_offset );

		goto on_error;
	}
	/* The record values are managed by the chunk and freed after usage
	 * A copy is created to make sure that the records values that are passed
	 * to the records list can be managed by the list
	 */
	if( libevtx_record_values_clone(
	     &record_values,
	     chunk_record_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create record values.",
		 function );

		goto on_error;
	}
	if( libevtx_record_values_read_xml_document(
	     record_values,
	     chunks_table->io_handle,
	     chunk->data,
	     chunk->data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read record values XML document.",
		 function );

		goto on_error;
	}
	if( libfdata_list_element_set_element_value(
	     list_element,
	     (intptr_t *) file_io_handle,
	     cache,
	     (intptr_t *) record_values,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libevtx_record_values_free,
	     LIBFDATA_LIST_ELEMENT_VALUE_FLAG_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set record values as element value.",
		 function );

		goto on_error;
	}
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

