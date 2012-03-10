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

#include <common.h>
#include <memory.h>
#include <types.h>

#include <liberror.h>
#include <libnotify.h>

#include "libevtx_array_type.h"
#include "libevtx_checksum.h"
#include "libevtx_chunk.h"
#include "libevtx_definitions.h"
#include "libevtx_event_values.h"
#include "libevtx_io_handle.h"
#include "libevtx_libbfio.h"

#include "evtx_chunk.h"

const uint8_t *evtx_chunk_signature = (uint8_t *) "ElfChnk";

/* Creates a chunk value
 * Returns 1 if successful or -1 on error
 */
int libevtx_chunk_value_initialize(
     libevtx_chunk_value_t **chunk_value,
     liberror_error_t **error )
{
	static char *function = "libevtx_chunk_value_initialize";

	if( chunk_value == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk value.",
		 function );

		return( -1 );
	}
	if( *chunk_value != NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid chunk value value already set.",
		 function );

		return( -1 );
	}
	*chunk_value = memory_allocate_structure(
	                libevtx_chunk_value_t );

	if( *chunk_value == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create chunk value.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *chunk_value,
	     0,
	     sizeof( libevtx_chunk_value_t ) ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear chunk value.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *chunk_value != NULL )
	{
		memory_free(
		 *chunk_value );

		*chunk_value = NULL;
	}
	return( -1 );
}

/* Frees the chunk value
 * Returns 1 if successful or -1 on error
 */
int libevtx_chunk_value_free(
     libevtx_chunk_value_t **chunk_value,
     liberror_error_t **error )
{
	static char *function = "libevtx_chunk_value_free";

	if( chunk_value == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk value.",
		 function );

		return( -1 );
	}
	if( *chunk_value != NULL )
	{
		/* The data is referenced and freed elsewhere
		 */
		memory_free(
		 *chunk_value );

		*chunk_value = NULL;
	}
	return( 1 );
}

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
	if( libevtx_array_initialize(
	     &( ( *chunk )->values_array ),
	     0,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create chunk values array.",
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
	int result            = 1;

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
		if( libevtx_array_free(
		     &( ( *chunk )->values_array ),
		     (int (*)(intptr_t **, liberror_error_t **)) &libevtx_chunk_value_free,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free the chunk values array.",
			 function );

			result = -1;
		}
		if( ( *chunk )->data != NULL )
		{
			memory_free(
			 ( *chunk )->data );
		}
		memory_free(
		 *chunk );

		*chunk = NULL;
	}
	return( result );
}

/* Reads the chunk
 * Returns 1 if successful or -1 on error
 */
int libevtx_chunk_read(
     libevtx_chunk_t *chunk,
     libevtx_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     liberror_error_t **error )
{
	uint8_t *chunk_values_data         = NULL;
	static char *function              = "libevtx_chunk_read";
	size_t chunk_values_data_offset    = 0;
	size_t chunk_values_data_size      = 0;
	ssize_t read_count                 = 0;
	uint64_t calculated_chunk_number   = 0;
	uint32_t calculated_checksum       = 0;
	uint32_t event_records_checksum    = 0;
	uint32_t first_event_record_number = 0;
	uint32_t free_space_offset         = 0;
	uint32_t last_event_record_number  = 0;
	uint32_t last_event_record_offset  = 0;
	uint32_t number_of_event_records   = 0;
	uint32_t stored_checksum           = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint64_t value_64bit               = 0;
	uint32_t value_32bit               = 0;
#endif

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
	if( chunk->data != NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid chunk data already set.",
		 function );

		return( -1 );
	}
	if( io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	calculated_chunk_number = (uint64_t) ( ( file_offset - io_handle->chunk_size ) / io_handle->chunk_size );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: reading chunk: %" PRIu64 " at offset: %" PRIi64 " (0x%08" PRIx64 ")\n",
		 function,
		 calculated_chunk_number,
		 file_offset,
		 file_offset );
	}
#endif
	if( libbfio_handle_seek_offset(
	     file_io_handle,
	     file_offset,
	     SEEK_SET,
	     error ) == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek chunk offset: %" PRIi64 ".",
		 function,
		 file_offset );

		goto on_error;
	}
	chunk->data = (uint8_t *) memory_allocate(
	                           (size_t) io_handle->chunk_size );

	if( chunk->data == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create chunk data.",
		 function );

		goto on_error;
	}
	chunk->data_size = (size_t) io_handle->chunk_size;

	read_count = libbfio_handle_read(
	              file_io_handle,
	              chunk->data,
	              chunk->data_size,
	              error );

	if( read_count != (ssize_t) chunk->data_size )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read chunk data.",
		 function );

		goto on_error;
	}
	chunk_values_data      = chunk->data;
	chunk_values_data_size = chunk->data_size;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: chunk header data:\n",
		 function );
		libnotify_print_data(
		 chunk_values_data,
		 sizeof( evtx_chunk_header_t ),
		 0 );
	}
#endif
	if( memory_compare(
	     ( (evtx_chunk_header_t *) chunk_values_data )->signature,
	     evtx_chunk_signature,
	     8 ) != 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported chunk signature.",
		 function );

		goto on_error;
	}
	byte_stream_copy_to_uint64_little_endian(
	 ( (evtx_chunk_header_t *) chunk_values_data )->first_event_record_number,
	 first_event_record_number );

	byte_stream_copy_to_uint64_little_endian(
	 ( (evtx_chunk_header_t *) chunk_values_data )->last_event_record_number,
	 last_event_record_number );

	byte_stream_copy_to_uint32_little_endian(
	 ( (evtx_chunk_header_t *) chunk_values_data )->last_event_record_offset,
	 last_event_record_offset );

	byte_stream_copy_to_uint32_little_endian(
	 ( (evtx_chunk_header_t *) chunk_values_data )->free_space_offset,
	 free_space_offset );

	byte_stream_copy_to_uint32_little_endian(
	 ( (evtx_chunk_header_t *) chunk_values_data )->event_records_checksum,
	 event_records_checksum );

	byte_stream_copy_to_uint32_little_endian(
	 ( (evtx_chunk_header_t *) chunk_values_data )->checksum,
	 stored_checksum );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: signature\t\t\t\t\t\t: %c%c%c%c%c%c%c\\x%02x\n",
		 function,
		 ( (evtx_chunk_header_t *) chunk_values_data )->signature[ 0 ],
		 ( (evtx_chunk_header_t *) chunk_values_data )->signature[ 1 ],
		 ( (evtx_chunk_header_t *) chunk_values_data )->signature[ 2 ],
		 ( (evtx_chunk_header_t *) chunk_values_data )->signature[ 3 ],
		 ( (evtx_chunk_header_t *) chunk_values_data )->signature[ 4 ],
		 ( (evtx_chunk_header_t *) chunk_values_data )->signature[ 5 ] ,
		 ( (evtx_chunk_header_t *) chunk_values_data )->signature[ 6 ],
		 ( (evtx_chunk_header_t *) chunk_values_data )->signature[ 7 ] );

		libnotify_printf(
		 "%s: first event record number\t\t\t\t: %" PRIu64 "\n",
		 function,
		 first_event_record_number );

		libnotify_printf(
		 "%s: last event record number\t\t\t\t: %" PRIu64 "\n",
		 function,
		 last_event_record_number );

		byte_stream_copy_to_uint64_little_endian(
		 ( (evtx_chunk_header_t *) chunk_values_data )->first_event_record_identifier,
		 value_64bit );
		libnotify_printf(
		 "%s: first event record identifier\t\t\t: %" PRIu64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (evtx_chunk_header_t *) chunk_values_data )->last_event_record_identifier,
		 value_64bit );
		libnotify_printf(
		 "%s: last event record identifier\t\t\t: %" PRIu64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (evtx_chunk_header_t *) chunk_values_data )->header_size,
		 value_32bit );
		libnotify_printf(
		 "%s: header size\t\t\t\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		libnotify_printf(
		 "%s: last event record offset\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 last_event_record_offset );

		libnotify_printf(
		 "%s: free space offset\t\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 free_space_offset );

		libnotify_printf(
		 "%s: event records checksum\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 event_records_checksum );

		libnotify_printf(
		 "%s: unknown1:\n",
		 function );
		libnotify_print_data(
		 ( (evtx_chunk_header_t *) chunk_values_data )->unknown1,
		 64,
		 0 );

		byte_stream_copy_to_uint32_little_endian(
		 ( (evtx_chunk_header_t *) chunk_values_data )->unknown2,
		 value_32bit );
		libnotify_printf(
		 "%s: unknown2\t\t\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		libnotify_printf(
		 "%s: checksum\t\t\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 stored_checksum );

		libnotify_printf(
		 "\n" );
	}
#endif
	if( libevtx_checksum_calculate_little_endian_crc32(
	     &calculated_checksum,
	     chunk_values_data,
	     120,
	     0,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to calculate CRC-32 checksum.",
		 function );

		goto on_error;
	}
	if( libevtx_checksum_calculate_little_endian_crc32(
	     &calculated_checksum,
	     &( chunk_values_data[ 128 ] ),
	     384,
	     calculated_checksum,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to calculate CRC-32 checksum.",
		 function );

		goto on_error;
	}
	if( stored_checksum != calculated_checksum )
	{
#if defined( HAVE_VERBOSE_OUTPUT )
		if( libnotify_verbose != 0 )
		{
			libnotify_printf(
			 "%s: mismatch in chunk: %" PRIu64 " header CRC-32 checksum ( 0x%08" PRIx32 " != 0x%08" PRIx32 " ).\n",
			 function,
			 calculated_chunk_number,
			 stored_checksum,
			 calculated_checksum );
		}
#endif
		io_handle->flags |= LIBEVTX_FILE_FLAG_CORRUPTED;
	}
	chunk_values_data_offset = sizeof( evtx_chunk_header_t );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: chunk table data:\n",
		 function );
		libnotify_print_data(
		 &( chunk_values_data[ chunk_values_data_offset ] ),
		 384,
		 0 );
	}
#endif
	chunk_values_data_offset += 384;
/* TODO can free_space_offset be 0 ? */

	if( ( free_space_offset < chunk_values_data_offset )
	 || ( free_space_offset > chunk_values_data_size ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid free space offset value out of bounds.",
		 function );

		goto on_error;
	}
	if( libevtx_checksum_calculate_little_endian_crc32(
	     &calculated_checksum,
	     &( chunk_values_data[ 512 ] ),
	     free_space_offset - chunk_values_data_offset,
	     0,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to calculate CRC-32 checksum.",
		 function );

		goto on_error;
	}
	if( event_records_checksum != calculated_checksum )
	{
#if defined( HAVE_VERBOSE_OUTPUT )
		if( libnotify_verbose != 0 )
		{
			libnotify_printf(
			 "%s: mismatch in chunk: %" PRIu64 " events records CRC-32 checksum ( 0x%08" PRIx32 " != 0x%08" PRIx32 " ).\n",
			 function,
			 calculated_chunk_number,
			 event_records_checksum,
			 calculated_checksum );
		}
#endif
		io_handle->flags |= LIBEVTX_FILE_FLAG_CORRUPTED;
	}
/* TODO */
	libevtx_event_values_t *event_values = NULL;

	while( chunk_values_data_offset < chunk_values_data_size )
	{
		if( libevtx_event_values_initialize(
		     &event_values,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create event values.",
			 function );

			goto on_error;
		}
		if( libevtx_event_values_read(
		     event_values,
		     io_handle,
		     chunk_values_data,
		     chunk_values_data_size,
		     chunk_values_data_offset,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read event values.",
			 function );

			goto on_error;
		}
		chunk_values_data_offset += event_values->size;

		if( libevtx_event_values_free(
		     &event_values,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free event values.",
			 function );

			goto on_error;
		}
		number_of_event_records++;
	}

/* TODO validate number of event records */
	return( 1 );

on_error:
	if( chunk->data != NULL )
	{
		memory_free(
		 chunk->data );

		chunk->data = NULL;
	}
	return( -1 );
}

/* Retrieves the number of chunk values
 * Returns 1 if successful or -1 on error
 */
int libevtx_chunk_get_number_of_values(
     libevtx_chunk_t *chunk,
     uint16_t *number_of_values,
     liberror_error_t **error )
{
	static char *function      = "libevtx_chunk_get_number_of_values";
	int chunk_number_of_values = 0;

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
	if( number_of_values == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid number of values.",
		 function );

		return( -1 );
	}
	if( libevtx_array_get_number_of_entries(
	     chunk->values_array,
	     &chunk_number_of_values,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of values.",
		 function );

		return( -1 );
	}
	if( chunk_number_of_values > (int) UINT16_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid number of chunk values value exceeds maximum.",
		 function );

		return( -1 );
	}
	*number_of_values = (uint16_t) chunk_number_of_values;

	return( 1 );
}

/* Retrieves the chunk value at the index
 * Returns 1 if successful or -1 on error
 */
int libevtx_chunk_get_value(
     libevtx_chunk_t *chunk,
     uint16_t value_index,
     libevtx_chunk_value_t **chunk_value,
     liberror_error_t **error )
{
	static char *function = "libevtx_chunk_get_value";

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
	if( libevtx_array_get_entry_by_index(
	     chunk->values_array,
	     (int) value_index,
	     (intptr_t **) chunk_value,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve chunk value: %" PRIu16 ".",
		 function,
		 value_index );

		return( -1 );
	}
	return( 1 );
}

