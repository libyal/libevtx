/*
 * Input/Output (IO) handle functions
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
#include <byte_stream.h>
#include <memory.h>
#include <types.h>

#include <liberror.h>
#include <libnotify.h>

#include "libevtx_codepage.h"
#include "libevtx_debug.h"
#include "libevtx_definitions.h"
#include "libevtx_io_handle.h"
#include "libevtx_libbfio.h"
#include "libevtx_libfcache.h"
#include "libevtx_libfdata.h"
#include "libevtx_unused.h"

#include "evtx_file_header.h"

const uint8_t *evtx_file_signature = (uint8_t *) "ElfFile";

/* Initialize an IO handle
 * Make sure the value io_handle is pointing to is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libevtx_io_handle_initialize(
     libevtx_io_handle_t **io_handle,
     liberror_error_t **error )
{
	static char *function = "libevtx_io_handle_initialize";

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
	if( *io_handle == NULL )
	{
		*io_handle = memory_allocate_structure(
		              libevtx_io_handle_t );

		if( *io_handle == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create IO handle.",
			 function );

			goto on_error;
		}
		if( memory_set(
		     *io_handle,
		     0,
		     sizeof( libevtx_io_handle_t ) ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear IO handle.",
			 function );

			goto on_error;
		}
		( *io_handle )->chunk_size     = 0x00010000UL;
		( *io_handle )->ascii_codepage = LIBEVTX_CODEPAGE_WINDOWS_1252;
	}
	return( 1 );

on_error:
	if( *io_handle != NULL )
	{
		memory_free(
		 *io_handle );

		*io_handle = NULL;
	}
	return( -1 );
}

/* Frees an IO handle
 * Returns 1 if successful or -1 on error
 */
int libevtx_io_handle_free(
     libevtx_io_handle_t **io_handle,
     liberror_error_t **error )
{
	static char *function = "libevtx_io_handle_free";
	int result            = 1;

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
	if( *io_handle != NULL )
	{
		memory_free(
		 *io_handle );

		*io_handle = NULL;
	}
	return( result );
}

/* Reads the file (or database) header
 * Returns 1 if successful or -1 on error
 */
int libevtx_io_handle_read_file_header(
     libevtx_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     liberror_error_t **error )
{
	uint8_t *file_header_data = NULL;
	static char *function     = "libevtx_io_handle_read_file_header";
	size_t read_size          = 4096;
	ssize_t read_count        = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint64_t value_64bit      = 0;
	uint32_t value_32bit      = 0;
	uint16_t value_16bit      = 0;
#endif

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
#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: reading file header at offset: %" PRIi64 " (0x%08" PRIx64 ")\n",
		 function,
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
		 "%s: unable to seek file header offset: %" PRIi64 ".",
		 function,
		 file_offset );

		return( -1 );
	}
	file_header_data = (uint8_t *) memory_allocate(
	                                sizeof( uint8_t ) * read_size );

	if( file_header_data == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create file header data.",
		 function );

		return( -1 );
	}
	read_count = libbfio_handle_read(
	              file_io_handle,
	              file_header_data,
	              read_size,
	              error );

	if( read_count != (ssize_t) read_size )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read file header.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: file header data:\n",
		 function );
		libnotify_print_data(
		 file_header_data,
		 sizeof( evtx_file_header_t ),
		 0 );
	}
#endif
	if( memory_compare(
	     ( (evtx_file_header_t *) file_header_data )->signature,
	     evtx_file_signature,
	     8 ) != 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported file signature.",
		 function );

		goto on_error;
	}

#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: signature\t\t\t\t: %c%c%c%c%c%c%c\n",
		 function,
		 ( (evtx_file_header_t *) file_header_data )->signature[ 0 ],
		 ( (evtx_file_header_t *) file_header_data )->signature[ 1 ],
		 ( (evtx_file_header_t *) file_header_data )->signature[ 2 ],
		 ( (evtx_file_header_t *) file_header_data )->signature[ 3 ],
		 ( (evtx_file_header_t *) file_header_data )->signature[ 4 ],
		 ( (evtx_file_header_t *) file_header_data )->signature[ 5 ] ,
		 ( (evtx_file_header_t *) file_header_data )->signature[ 6 ] );

		byte_stream_copy_to_uint64_little_endian(
		 ( (evtx_file_header_t *) file_header_data )->first_chunk_number,
		 value_64bit );
		libnotify_printf(
		 "%s: first chunk number\t\t\t: %" PRIu64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (evtx_file_header_t *) file_header_data )->last_chunk_number,
		 value_64bit );
		libnotify_printf(
		 "%s: last chunk number\t\t\t: %" PRIu64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (evtx_file_header_t *) file_header_data )->next_record_identifier,
		 value_64bit );
		libnotify_printf(
		 "%s: next record identifier\t\t: %" PRIu64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint16_little_endian(
		 ( (evtx_file_header_t *) file_header_data )->header_size,
		 value_16bit );
		libnotify_printf(
		 "%s: header size\t\t\t\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		byte_stream_copy_to_uint16_little_endian(
		 ( (evtx_file_header_t *) file_header_data )->unknown1,
		 value_16bit );
		libnotify_printf(
		 "%s: unknown1\t\t\t\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		byte_stream_copy_to_uint16_little_endian(
		 ( (evtx_file_header_t *) file_header_data )->major_version,
		 value_16bit );
		libnotify_printf(
		 "%s: major version\t\t\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		byte_stream_copy_to_uint16_little_endian(
		 ( (evtx_file_header_t *) file_header_data )->minor_version,
		 value_16bit );
		libnotify_printf(
		 "%s: minor version\t\t\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		byte_stream_copy_to_uint16_little_endian(
		 ( (evtx_file_header_t *) file_header_data )->header_block_size,
		 value_16bit );
		libnotify_printf(
		 "%s: header block size\t\t\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		byte_stream_copy_to_uint16_little_endian(
		 ( (evtx_file_header_t *) file_header_data )->number_of_chunks,
		 value_16bit );
		libnotify_printf(
		 "%s: number of chunks\t\t\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		libnotify_printf(
		 "%s: unknown2:\n",
		 function );
		libnotify_print_data(
		 ( (evtx_file_header_t *) file_header_data )->unknown2,
		 76,
		 0 );

		byte_stream_copy_to_uint32_little_endian(
		 ( (evtx_file_header_t *) file_header_data )->flags,
		 value_32bit );
		libnotify_printf(
		 "%s: flags\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (evtx_file_header_t *) file_header_data )->checksum,
		 value_32bit );
		libnotify_printf(
		 "%s: checksum\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		libnotify_printf(
		 "\n" );
	}
#endif
#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: trailing data:\n",
		 function );
		libnotify_print_data(
		 &( file_header_data[ sizeof( evtx_file_header_t ) ] ),
		 read_size - sizeof( evtx_file_header_t ),
		 LIBNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	memory_free(
	 file_header_data );

	file_header_data = NULL;

	return( 1 );

on_error:
	if( file_header_data != NULL )
	{
		memory_free(
		 file_header_data );
	}
	return( -1 );
}

/* Reads a chunk
 * Callback function for the chunk vector
 * Returns 1 if successful or -1 on error
 */
int libevtx_io_handle_read_chunk(
     intptr_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfdata_vector_t *vector,
     libfcache_cache_t *cache,
     int element_index,
     off64_t element_data_offset,
     size64_t element_data_size LIBEVTX_ATTRIBUTE_UNUSED,
     uint8_t read_flags LIBEVTX_ATTRIBUTE_UNUSED,
     liberror_error_t **error )
{
/* TODO
	libevtx_chunk_t *chunk = NULL;
*/
	static char *function = "libevtx_io_handle_read_chunk";

	LIBEVTX_UNREFERENCED_PARAMETER( element_data_size );
	LIBEVTX_UNREFERENCED_PARAMETER( read_flags );

/*
	if( libevtx_chunk_initialize(
	     &chunk,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create chunk.",
		 function );

		goto on_error;
	}
	if( libevtx_chunk_read(
	     chunk,
	     (libevtx_io_handle_t *) io_handle,
	     file_io_handle,
	     element_data_offset,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read chunk.",
		 function );

		goto on_error;
	}
	if( libfdata_vector_set_element_value_by_index(
	     vector,
	     cache,
	     element_index,
	     (intptr_t *) chunk,
	     &libevtx_chunk_free,
	     LIBFDATA_LIST_ELEMENT_VALUE_FLAG_MANAGED,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set chunk as element value.",
		 function );

		goto on_error;
	}
*/
	return( 1 );

on_error:
/*
	if( chunk != NULL )
	{
		libevtx_chunk_free(
		 (intptr_t *) chunk,
		 NULL );
	}
*/
	return( -1 );
}

