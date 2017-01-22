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

#include <common.h>
#include <memory.h>
#include <types.h>

#include "libevtx_byte_stream.h"
#include "libevtx_checksum.h"
#include "libevtx_chunk.h"
#include "libevtx_definitions.h"
#include "libevtx_io_handle.h"
#include "libevtx_libbfio.h"
#include "libevtx_libcdata.h"
#include "libevtx_libcerror.h"
#include "libevtx_libcnotify.h"
#include "libevtx_record_values.h"

#include "evtx_chunk.h"
#include "evtx_event_record.h"

const uint8_t *evtx_chunk_signature = (uint8_t *) "ElfChnk";

/* Creates a chunk
 * Make sure the value chunk is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libevtx_chunk_initialize(
     libevtx_chunk_t **chunk,
     libcerror_error_t **error )
{
	static char *function = "libevtx_chunk_initialize";

	if( chunk == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk.",
		 function );

		return( -1 );
	}
	if( *chunk != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid chunk value already set.",
		 function );

		return( -1 );
	}
	*chunk = memory_allocate_structure(
	          libevtx_chunk_t );

	if( *chunk == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create chunk.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *chunk,
	     0,
	     sizeof( libevtx_chunk_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear chunk.",
		 function );

		memory_free(
		 *chunk );

		*chunk = NULL;

		return( -1 );
	}
	if( libcdata_array_initialize(
	     &( ( *chunk )->records_array ),
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create chunk records array.",
		 function );

		goto on_error;
	}
	if( libcdata_array_initialize(
	     &( ( *chunk )->recovered_records_array ),
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create chunk recovered records array.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *chunk != NULL )
	{
		if( ( *chunk )->records_array != NULL )
		{
			libcdata_array_free(
			 &( ( *chunk )->records_array ),
			 NULL,
			 NULL );
		}
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
     libcerror_error_t **error )
{
	static char *function = "libevtx_chunk_free";
	int result            = 1;

	if( chunk == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk.",
		 function );

		return( -1 );
	}
	if( *chunk != NULL )
	{
		if( libcdata_array_free(
		     &( ( *chunk )->recovered_records_array ),
		     (int (*)(intptr_t **, libcerror_error_t **)) &libevtx_record_values_free,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free the chunk recovered records array.",
			 function );

			result = -1;
		}
		if( libcdata_array_free(
		     &( ( *chunk )->records_array ),
		     (int (*)(intptr_t **, libcerror_error_t **)) &libevtx_record_values_free,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free the chunk records array.",
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
 * Returns 1 if successful, 0 if the chunk is 0-byte filled or -1 on error
 */
int libevtx_chunk_read(
     libevtx_chunk_t *chunk,
     libevtx_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     libcerror_error_t **error )
{
	libevtx_record_values_t *record_values      = NULL;
	uint8_t *chunk_data                         = NULL;
	static char *function                       = "libevtx_chunk_read";
	size_t chunk_data_offset                    = 0;
	size_t chunk_data_size                      = 0;
	size_t xml_data_offset                      = 0;
	size_t xml_data_size                        = 0;
	ssize_t read_count                          = 0;
	uint64_t calculated_number_of_event_records = 0;
	uint64_t first_event_record_identifier      = 0;
	uint64_t first_event_record_number          = 0;
	uint64_t last_event_record_identifier       = 0;
	uint64_t last_event_record_number           = 0;
	uint64_t number_of_event_records            = 0;
	uint32_t calculated_checksum                = 0;
	uint32_t event_records_checksum             = 0;
	uint32_t free_space_offset                  = 0;
	uint32_t header_size                        = 0;
	uint32_t last_event_record_offset           = 0;
	uint32_t stored_checksum                    = 0;
	int entry_index                             = 0;
	int result                                  = 0;

#if defined( HAVE_DEBUG_OUTPUT ) || defined( HAVE_VERBOSE_OUTPUT )
	uint64_t calculated_chunk_number            = 0;
#endif
#if defined( HAVE_DEBUG_OUTPUT )
	ssize_t free_space_size                     = 0;
	uint32_t value_32bit                        = 0;
#endif

	if( chunk == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk.",
		 function );

		return( -1 );
	}
	if( chunk->data != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid chunk data already set.",
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
#if defined( HAVE_DEBUG_OUTPUT ) || defined( HAVE_VERBOSE_OUTPUT )
	calculated_chunk_number = (uint64_t) ( ( file_offset - io_handle->chunk_size ) / io_handle->chunk_size );
#endif
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
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
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek chunk offset: %" PRIi64 ".",
		 function,
		 file_offset );

		goto on_error;
	}
	chunk->file_offset = file_offset;

	chunk->data = (uint8_t *) memory_allocate(
	                           (size_t) io_handle->chunk_size );

	if( chunk->data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create chunk data.",
		 function );

		goto on_error;
	}
	chunk->data_size = (size_t) io_handle->chunk_size;

	read_count = libbfio_handle_read_buffer(
	              file_io_handle,
	              chunk->data,
	              chunk->data_size,
	              error );

	if( read_count != (ssize_t) chunk->data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read chunk data.",
		 function );

		goto on_error;
	}
	chunk_data      = chunk->data;
	chunk_data_size = chunk->data_size;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: chunk header data:\n",
		 function );
		libcnotify_print_data(
		 chunk_data,
		 sizeof( evtx_chunk_header_t ),
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	result = libevtx_byte_stream_check_for_zero_byte_fill(
	          chunk_data,
	          chunk_data_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine of chunk is 0-byte filled.",
		 function );

		goto on_error;
	}
	else if( result != 0 )
	{
		return( 0 );
	}
	if( memory_compare(
	     ( (evtx_chunk_header_t *) chunk_data )->signature,
	     evtx_chunk_signature,
	     8 ) != 0 )
	{
#if defined( HAVE_VERBOSE_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: unsupported chunk signature.\n",
			 function );
		}
#endif
		chunk->flags |= LIBEVTX_CHUNK_FLAG_IS_CORRUPTED;
	}
	else
	{
		byte_stream_copy_to_uint64_little_endian(
		 ( (evtx_chunk_header_t *) chunk_data )->first_event_record_number,
		 first_event_record_number );

		byte_stream_copy_to_uint64_little_endian(
		 ( (evtx_chunk_header_t *) chunk_data )->last_event_record_number,
		 last_event_record_number );

		byte_stream_copy_to_uint64_little_endian(
		 ( (evtx_chunk_header_t *) chunk_data )->first_event_record_identifier,
		 first_event_record_identifier );

		byte_stream_copy_to_uint64_little_endian(
		 ( (evtx_chunk_header_t *) chunk_data )->last_event_record_identifier,
		 last_event_record_identifier );

		byte_stream_copy_to_uint32_little_endian(
		 ( (evtx_chunk_header_t *) chunk_data )->header_size,
		 header_size );

		byte_stream_copy_to_uint32_little_endian(
		 ( (evtx_chunk_header_t *) chunk_data )->last_event_record_offset,
		 last_event_record_offset );

		byte_stream_copy_to_uint32_little_endian(
		 ( (evtx_chunk_header_t *) chunk_data )->free_space_offset,
		 free_space_offset );

		byte_stream_copy_to_uint32_little_endian(
		 ( (evtx_chunk_header_t *) chunk_data )->event_records_checksum,
		 event_records_checksum );

		byte_stream_copy_to_uint32_little_endian(
		 ( (evtx_chunk_header_t *) chunk_data )->checksum,
		 stored_checksum );

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: signature\t\t\t\t\t\t: %c%c%c%c%c%c%c\\x%02x\n",
			 function,
			 ( (evtx_chunk_header_t *) chunk_data )->signature[ 0 ],
			 ( (evtx_chunk_header_t *) chunk_data )->signature[ 1 ],
			 ( (evtx_chunk_header_t *) chunk_data )->signature[ 2 ],
			 ( (evtx_chunk_header_t *) chunk_data )->signature[ 3 ],
			 ( (evtx_chunk_header_t *) chunk_data )->signature[ 4 ],
			 ( (evtx_chunk_header_t *) chunk_data )->signature[ 5 ] ,
			 ( (evtx_chunk_header_t *) chunk_data )->signature[ 6 ],
			 ( (evtx_chunk_header_t *) chunk_data )->signature[ 7 ] );

			libcnotify_printf(
			 "%s: first event record number\t\t\t\t: %" PRIu64 "\n",
			 function,
			 first_event_record_number );

			libcnotify_printf(
			 "%s: last event record number\t\t\t\t: %" PRIu64 "\n",
			 function,
			 last_event_record_number );

			libcnotify_printf(
			 "%s: first event record identifier\t\t\t: %" PRIu64 "\n",
			 function,
			 first_event_record_identifier );

			libcnotify_printf(
			 "%s: last event record identifier\t\t\t: %" PRIu64 "\n",
			 function,
			 last_event_record_identifier );

			libcnotify_printf(
			 "%s: header size\t\t\t\t\t\t: %" PRIu32 "\n",
			 function,
			 header_size );

			libcnotify_printf(
			 "%s: last event record offset\t\t\t\t: 0x%08" PRIx32 "\n",
			 function,
			 last_event_record_offset );

			libcnotify_printf(
			 "%s: free space offset\t\t\t\t\t: 0x%08" PRIx32 "\n",
			 function,
			 free_space_offset );

			libcnotify_printf(
			 "%s: event records checksum\t\t\t\t: 0x%08" PRIx32 "\n",
			 function,
			 event_records_checksum );

			libcnotify_printf(
			 "%s: unknown1:\n",
			 function );
			libcnotify_print_data(
			 ( (evtx_chunk_header_t *) chunk_data )->unknown1,
			 64,
			 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );

			byte_stream_copy_to_uint32_little_endian(
			 ( (evtx_chunk_header_t *) chunk_data )->unknown2,
			 value_32bit );
			libcnotify_printf(
			 "%s: unknown2\t\t\t\t\t\t: 0x%08" PRIx32 "\n",
			 function,
			 value_32bit );

			libcnotify_printf(
			 "%s: checksum\t\t\t\t\t\t: 0x%08" PRIx32 "\n",
			 function,
			 stored_checksum );

			libcnotify_printf(
			 "\n" );
		}
#endif
		if( header_size != 128 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported header size: %" PRIu32 ".",
			 function,
			 header_size );

			goto on_error;
		}
		if( libevtx_checksum_calculate_little_endian_crc32(
		     &calculated_checksum,
		     chunk_data,
		     120,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to calculate CRC-32 checksum.",
			 function );

			goto on_error;
		}
		if( libevtx_checksum_calculate_little_endian_crc32(
		     &calculated_checksum,
		     &( chunk_data[ 128 ] ),
		     384,
		     calculated_checksum,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to calculate CRC-32 checksum.",
			 function );

			goto on_error;
		}
		if( stored_checksum != calculated_checksum )
		{
#if defined( HAVE_VERBOSE_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: mismatch in chunk: %" PRIu64 " header CRC-32 checksum ( 0x%08" PRIx32 " != 0x%08" PRIx32 " ).\n",
				 function,
				 calculated_chunk_number,
				 stored_checksum,
				 calculated_checksum );
			}
#endif
			chunk->flags |= LIBEVTX_CHUNK_FLAG_IS_CORRUPTED;
		}
		chunk_data_offset = sizeof( evtx_chunk_header_t );

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: chunk table data:\n",
			 function );
			libcnotify_print_data(
			 &( chunk_data[ chunk_data_offset ] ),
			 384,
			 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
		}
#endif
		chunk_data_offset += 384;
/* TODO can free_space_offset be 0 ? */

		if( ( free_space_offset < chunk_data_offset )
		 || ( free_space_offset > chunk_data_size ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid free space offset value out of bounds.",
			 function );

			goto on_error;
		}
		if( libevtx_checksum_calculate_little_endian_crc32(
		     &calculated_checksum,
		     &( chunk_data[ 512 ] ),
		     free_space_offset - chunk_data_offset,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to calculate CRC-32 checksum.",
			 function );

			goto on_error;
		}
		if( event_records_checksum != calculated_checksum )
		{
#if defined( HAVE_VERBOSE_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: mismatch in chunk: %" PRIu64 " event records CRC-32 checksum ( 0x%08" PRIx32 " != 0x%08" PRIx32 " ).\n",
				 function,
				 calculated_chunk_number,
				 event_records_checksum,
				 calculated_checksum );
			}
#endif
			chunk->flags |= LIBEVTX_CHUNK_FLAG_IS_CORRUPTED;
		}
		while( chunk_data_offset <= last_event_record_offset )
		{
			if( libevtx_record_values_initialize(
			     &record_values,
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
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: reading record at offset: %" PRIi64 " (0x%08" PRIx64 ")\n",
				 function,
				 file_offset + chunk_data_offset,
				 file_offset + chunk_data_offset );
			}
#endif
			result = libevtx_record_values_read_header(
				  record_values,
				  io_handle,
				  chunk_data,
				  chunk_data_size,
				  chunk_data_offset,
				  error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read record values header at offset: %" PRIi64 ".",
				 function,
				 file_offset + chunk_data_offset );

#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					if( ( error != NULL )
					 && ( *error != NULL ) )
					{
						libcnotify_print_error_backtrace(
						 *error );
					}
				}
#endif
				libcerror_error_free(
				 error );
			}
			if( result != 1 )
			{
				break;
			}
			chunk_data_offset += record_values->data_size;

			if( libcdata_array_append_entry(
			     chunk->records_array,
			     &entry_index,
			     (intptr_t *) record_values,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable to append record values to records array.",
				 function );

				goto on_error;
			}
			record_values = NULL;

			number_of_event_records++;
		}
		if( first_event_record_number > last_event_record_number )
		{
#if defined( HAVE_VERBOSE_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: invalid chunk: %" PRIu64 " first event record number: %" PRIu64 " exceeds last event record number: %" PRIu64 ".\n",
				 function,
				 calculated_chunk_number,
				 first_event_record_number,
				 last_event_record_number );
			}
#endif
			chunk->flags |= LIBEVTX_CHUNK_FLAG_IS_CORRUPTED;
		}
		else if( result == 1 )
		{
			calculated_number_of_event_records = last_event_record_number - first_event_record_number + 1;

#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: calculated number of records\t\t\t: %" PRIu64 "\n",
				 function,
				 calculated_number_of_event_records );
			}
#endif
			if( number_of_event_records != calculated_number_of_event_records )
			{
#if defined( HAVE_VERBOSE_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: mismatch in chunk: %" PRIu64 " number of event records ( %" PRIu64 " != %" PRIu64 " ).\n",
					 function,
					 calculated_chunk_number,
					 number_of_event_records,
					 calculated_number_of_event_records );
				}
#endif
				chunk->flags |= LIBEVTX_CHUNK_FLAG_IS_CORRUPTED;
			}
		}
		if( first_event_record_identifier > last_event_record_identifier )
		{
#if defined( HAVE_VERBOSE_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: in chunk: %" PRIu64 " first event record identifier: %" PRIu64 " exceeds last event record identifier: %" PRIu64 ".\n",
				 function,
				 calculated_chunk_number,
				 first_event_record_identifier,
				 last_event_record_identifier );
			}
#endif
			/* TODO mark this as corruption ? */
		}
	}
	if( chunk_data_offset < chunk_data_size )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		free_space_size = chunk_data_size - chunk_data_offset;

		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: free space data:\n",
			 function );
			libcnotify_print_data(
			 &( chunk_data[ chunk_data_offset ] ),
			 free_space_size,
			 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
		}
#endif
		while( chunk_data_offset < chunk_data_size )
		{
/* TODO optimize scan ? */
			if( memory_compare(
			     &( chunk_data[ chunk_data_offset ] ),
			     evtx_event_record_signature,
			     4 ) == 0 )
			{
				if( record_values == NULL )
				{
					if( libevtx_record_values_initialize(
					     &record_values,
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
				}
#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: reading recovered record at offset: %" PRIi64 " (0x%08" PRIx64 ")\n",
					 function,
					 file_offset + chunk_data_offset,
					 file_offset + chunk_data_offset );
				}
#endif
				if( libevtx_record_values_read_header(
				     record_values,
				     io_handle,
				     chunk_data,
				     chunk_data_size,
				     chunk_data_offset,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read record values header at offset: %" PRIi64 ".",
					 function,
					 file_offset + chunk_data_offset );

#if defined( HAVE_DEBUG_OUTPUT )
					if( libcnotify_verbose != 0 )
					{
						if( ( error != NULL )
						 && ( *error != NULL ) )
						{
							libcnotify_print_error_backtrace(
							 *error );
						}
					}
#endif
					libcerror_error_free(
					 error );
				}
				else
				{
					xml_data_offset = chunk_data_offset + sizeof( evtx_event_record_header_t );
					xml_data_size   = 0;

					if( record_values->data_size > ( sizeof( evtx_event_record_header_t ) + 4 ) )
					{
						xml_data_size = record_values->data_size - ( sizeof( evtx_event_record_header_t ) + 4 );
					}
					result = 0;

					if( xml_data_size > 0 )
					{
						if( ( xml_data_size >= 5 )
						 && ( chunk_data[ xml_data_offset ] == 0x0a ) )
						{
							result = 1;
						}
						else if( ( xml_data_size >= 4 )
						      && ( chunk_data[ xml_data_offset ] == 0x0f )
						      && ( chunk_data[ xml_data_offset + 1 ] == 0x01 )
						      && ( chunk_data[ xml_data_offset + 2 ] == 0x01 )
						      && ( chunk_data[ xml_data_offset + 3 ] == 0x00 ) )
						{
							result = 1;
						}
/* TODO what about 0x00 allow it ? */
					}
					if( result != 0 )
					{
						chunk_data_offset += record_values->data_size - 4;

						if( libcdata_array_append_entry(
						     chunk->recovered_records_array,
						     &entry_index,
						     (intptr_t *) record_values,
						     error ) != 1 )
						{
							libcerror_error_set(
							 error,
							 LIBCERROR_ERROR_DOMAIN_RUNTIME,
							 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
							 "%s: unable to append record values to recovered records array.",
							 function );

							goto on_error;
						}
						record_values = NULL;
					}
				}
			}
			chunk_data_offset += 4;
		}
		if( record_values != NULL )
		{
			if( libevtx_record_values_free(
			     &record_values,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free record values.",
				 function );

				goto on_error;
			}
		}
	}
	return( 1 );

on_error:
	if( record_values != NULL )
	{
		libevtx_record_values_free(
		 &record_values,
		 NULL );
	}
	if( chunk->data != NULL )
	{
		memory_free(
		 chunk->data );

		chunk->data = NULL;
	}
	return( -1 );
}

/* Retrieves the number of records
 * Returns 1 if successful or -1 on error
 */
int libevtx_chunk_get_number_of_records(
     libevtx_chunk_t *chunk,
     uint16_t *number_of_records,
     libcerror_error_t **error )
{
	static char *function       = "libevtx_chunk_get_number_of_records";
	int chunk_number_of_records = 0;

	if( chunk == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk.",
		 function );

		return( -1 );
	}
	if( number_of_records == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid number of records.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_number_of_entries(
	     chunk->records_array,
	     &chunk_number_of_records,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of records.",
		 function );

		return( -1 );
	}
	if( chunk_number_of_records > (int) UINT16_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid number of chunk records value exceeds maximum.",
		 function );

		return( -1 );
	}
	*number_of_records = (uint16_t) chunk_number_of_records;

	return( 1 );
}

/* Retrieves the record at the index
 * Returns 1 if successful or -1 on error
 */
int libevtx_chunk_get_record(
     libevtx_chunk_t *chunk,
     uint16_t record_index,
     libevtx_record_values_t **record_values,
     libcerror_error_t **error )
{
	static char *function = "libevtx_chunk_get_record";

	if( chunk == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_entry_by_index(
	     chunk->records_array,
	     (int) record_index,
	     (intptr_t **) record_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve record: %" PRIu16 ".",
		 function,
		 record_index );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the number of recovered records
 * Returns 1 if successful or -1 on error
 */
int libevtx_chunk_get_number_of_recovered_records(
     libevtx_chunk_t *chunk,
     uint16_t *number_of_records,
     libcerror_error_t **error )
{
	static char *function       = "libevtx_chunk_get_number_of_recovered_records";
	int chunk_number_of_records = 0;

	if( chunk == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk.",
		 function );

		return( -1 );
	}
	if( number_of_records == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid number of records.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_number_of_entries(
	     chunk->recovered_records_array,
	     &chunk_number_of_records,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of records.",
		 function );

		return( -1 );
	}
	if( chunk_number_of_records > (int) UINT16_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid number of chunk records value exceeds maximum.",
		 function );

		return( -1 );
	}
	*number_of_records = (uint16_t) chunk_number_of_records;

	return( 1 );
}

/* Retrieves the recovered record at the index
 * Returns 1 if successful or -1 on error
 */
int libevtx_chunk_get_recovered_record(
     libevtx_chunk_t *chunk,
     uint16_t record_index,
     libevtx_record_values_t **record_values,
     libcerror_error_t **error )
{
	static char *function = "libevtx_chunk_get_recovered_record";

	if( chunk == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_entry_by_index(
	     chunk->recovered_records_array,
	     (int) record_index,
	     (intptr_t **) record_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve record: %" PRIu16 ".",
		 function,
		 record_index );

		return( -1 );
	}
	return( 1 );
}

