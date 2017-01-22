/*
 * File functions
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
#include <narrow_string.h>
#include <types.h>
#include <wide_string.h>

#include "libevtx_chunks_table.h"
#include "libevtx_codepage.h"
#include "libevtx_chunk.h"
#include "libevtx_debug.h"
#include "libevtx_definitions.h"
#include "libevtx_i18n.h"
#include "libevtx_io_handle.h"
#include "libevtx_file.h"
#include "libevtx_libbfio.h"
#include "libevtx_libcerror.h"
#include "libevtx_libcnotify.h"
#include "libevtx_libfcache.h"
#include "libevtx_libfdata.h"
#include "libevtx_record.h"
#include "libevtx_record_values.h"

/* Creates a file
 * Make sure the value file is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libevtx_file_initialize(
     libevtx_file_t **file,
     libcerror_error_t **error )
{
	libevtx_internal_file_t *internal_file = NULL;
	static char *function                  = "libevtx_file_initialize";

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	if( *file != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file value already set.",
		 function );

		return( -1 );
	}
	internal_file = memory_allocate_structure(
	                 libevtx_internal_file_t );

	if( internal_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create file.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     internal_file,
	     0,
	     sizeof( libevtx_internal_file_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear file.",
		 function );

		memory_free(
		 internal_file );

		return( -1 );
	}
	if( libevtx_io_handle_initialize(
	     &( internal_file->io_handle ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create IO handle.",
		 function );

		goto on_error;
	}
	if( libevtx_i18n_initialize(
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initalize internationalization (i18n).",
		 function );

		goto on_error;
	}
	*file = (libevtx_file_t *) internal_file;

	return( 1 );

on_error:
	if( internal_file != NULL )
	{
		if( internal_file->io_handle != NULL )
		{
			libevtx_io_handle_free(
			 &( internal_file->io_handle ),
			 NULL );
		}
		memory_free(
		 internal_file );
	}
	return( -1 );
}

/* Frees a file
 * Returns 1 if successful or -1 on error
 */
int libevtx_file_free(
     libevtx_file_t **file,
     libcerror_error_t **error )
{
	libevtx_internal_file_t *internal_file = NULL;
	static char *function                  = "libevtx_file_free";
	int result                             = 1;

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	if( *file != NULL )
	{
		internal_file = (libevtx_internal_file_t *) *file;

		if( internal_file->file_io_handle != NULL )
		{
			if( libevtx_file_close(
			     *file,
			     error ) != 0 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_CLOSE_FAILED,
				 "%s: unable to close file.",
				 function );

				result = -1;
			}
		}
		*file = NULL;

		if( libevtx_io_handle_free(
		     &( internal_file->io_handle ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free IO handle.",
			 function );

			result = -1;
		}
		memory_free(
		 internal_file );
	}
	return( result );
}

/* Signals a file to abort its current activity
 * Returns 1 if successful or -1 on error
 */
int libevtx_file_signal_abort(
     libevtx_file_t *file,
     libcerror_error_t **error )
{
	libevtx_internal_file_t *internal_file = NULL;
	static char *function                  = "libevtx_file_signal_abort";

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	internal_file = (libevtx_internal_file_t *) file;

	if( internal_file->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file - missing IO handle.",
		 function );

		return( -1 );
	}
	internal_file->io_handle->abort = 1;

	return( 1 );
}

/* Opens a file
 * Returns 1 if successful or -1 on error
 */
int libevtx_file_open(
     libevtx_file_t *file,
     const char *filename,
     int access_flags,
     libcerror_error_t **error )
{
	libbfio_handle_t *file_io_handle       = NULL;
	libevtx_internal_file_t *internal_file = NULL;
	static char *function                  = "libevtx_file_open";
	size_t filename_length                 = 0;

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	internal_file = (libevtx_internal_file_t *) file;

	if( filename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filename.",
		 function );

		return( -1 );
	}
	if( ( ( access_flags & LIBEVTX_ACCESS_FLAG_READ ) == 0 )
	 && ( ( access_flags & LIBEVTX_ACCESS_FLAG_WRITE ) == 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported access flags.",
		 function );

		return( -1 );
	}
	if( ( access_flags & LIBEVTX_ACCESS_FLAG_WRITE ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: write access currently not supported.",
		 function );

		return( -1 );
	}
	if( libbfio_file_initialize(
	     &file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create file IO handle.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libbfio_handle_set_track_offsets_read(
	     file_io_handle,
	     1,
	     error ) != 1 )
	{
                libcerror_error_set(
                 error,
                 LIBCERROR_ERROR_DOMAIN_RUNTIME,
                 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
                 "%s: unable to set track offsets read in file IO handle.",
                 function );

		goto on_error;
	}
#endif
	filename_length = narrow_string_length(
	                   filename );

	if( libbfio_file_set_name(
	     file_io_handle,
	     filename,
	     filename_length + 1,
	     error ) != 1 )
	{
                libcerror_error_set(
                 error,
                 LIBCERROR_ERROR_DOMAIN_RUNTIME,
                 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
                 "%s: unable to set filename in file IO handle.",
                 function );

		goto on_error;
	}
	if( libevtx_file_open_file_io_handle(
	     file,
	     file_io_handle,
	     access_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open file: %s.",
		 function,
		 filename );

		goto on_error;
	}
	internal_file->file_io_handle_created_in_library = 1;

	return( 1 );

on_error:
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
	return( -1 );
}

#if defined( HAVE_WIDE_CHARACTER_TYPE )

/* Opens a file
 * Returns 1 if successful or -1 on error
 */
int libevtx_file_open_wide(
     libevtx_file_t *file,
     const wchar_t *filename,
     int access_flags,
     libcerror_error_t **error )
{
	libbfio_handle_t *file_io_handle       = NULL;
	libevtx_internal_file_t *internal_file = NULL;
	static char *function                  = "libevtx_file_open_wide";
	size_t filename_length                 = 0;

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	internal_file = (libevtx_internal_file_t *) file;

	if( filename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filename.",
		 function );

		return( -1 );
	}
	if( ( ( access_flags & LIBEVTX_ACCESS_FLAG_READ ) == 0 )
	 && ( ( access_flags & LIBEVTX_ACCESS_FLAG_WRITE ) == 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported access flags.",
		 function );

		return( -1 );
	}
	if( ( access_flags & LIBEVTX_ACCESS_FLAG_WRITE ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: write access currently not supported.",
		 function );

		return( -1 );
	}
	if( libbfio_file_initialize(
	     &file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create file IO handle.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libbfio_handle_set_track_offsets_read(
	     file_io_handle,
	     1,
	     error ) != 1 )
	{
                libcerror_error_set(
                 error,
                 LIBCERROR_ERROR_DOMAIN_RUNTIME,
                 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
                 "%s: unable to set track offsets read in file IO handle.",
                 function );

		goto on_error;
	}
#endif
	filename_length = wide_string_length(
	                   filename );

	if( libbfio_file_set_name_wide(
	     file_io_handle,
	     filename,
	     filename_length + 1,
	     error ) != 1 )
	{
                libcerror_error_set(
                 error,
                 LIBCERROR_ERROR_DOMAIN_RUNTIME,
                 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
                 "%s: unable to set filename in file IO handle.",
                 function );

		goto on_error;
	}
	if( libevtx_file_open_file_io_handle(
	     file,
	     file_io_handle,
	     access_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open file: %ls.",
		 function,
		 filename );

		goto on_error;
	}
	internal_file->file_io_handle_created_in_library = 1;

	return( 1 );

on_error:
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
	return( -1 );
}

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

/* Opens a file using a Basic File IO (bfio) handle
 * Returns 1 if successful or -1 on error
 */
int libevtx_file_open_file_io_handle(
     libevtx_file_t *file,
     libbfio_handle_t *file_io_handle,
     int access_flags,
     libcerror_error_t **error )
{
	libevtx_internal_file_t *internal_file = NULL;
	static char *function                  = "libevtx_file_open_file_io_handle";
	int bfio_access_flags                  = 0;
	int file_io_handle_is_open             = 0;
	int file_io_handle_opened_in_library   = 0;

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	internal_file = (libevtx_internal_file_t *) file;

	if( internal_file->file_io_handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file - file IO handle already set.",
		 function );

		return( -1 );
	}
	if( file_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file IO handle.",
		 function );

		return( -1 );
	}
	if( ( ( access_flags & LIBEVTX_ACCESS_FLAG_READ ) == 0 )
	 && ( ( access_flags & LIBEVTX_ACCESS_FLAG_WRITE ) == 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported access flags.",
		 function );

		return( -1 );
	}
	if( ( access_flags & LIBEVTX_ACCESS_FLAG_WRITE ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: write access currently not supported.",
		 function );

		return( -1 );
	}
	if( ( access_flags & LIBEVTX_ACCESS_FLAG_READ ) != 0 )
	{
		bfio_access_flags = LIBBFIO_ACCESS_FLAG_READ;
	}
	file_io_handle_is_open = libbfio_handle_is_open(
	                          file_io_handle,
	                          error );

	if( file_io_handle_is_open == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open file.",
		 function );

		goto on_error;
	}
	else if( file_io_handle_is_open == 0 )
	{
		if( libbfio_handle_open(
		     file_io_handle,
		     bfio_access_flags,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_OPEN_FAILED,
			 "%s: unable to open file IO handle.",
			 function );

			goto on_error;
		}
		file_io_handle_opened_in_library = 1;
	}
	if( libevtx_file_open_read(
	     internal_file,
	     file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read from file handle.",
		 function );

		goto on_error;
	}
	internal_file->file_io_handle                   = file_io_handle;
	internal_file->file_io_handle_opened_in_library = file_io_handle_opened_in_library;

	return( 1 );

on_error:
	if( file_io_handle_opened_in_library != 0 )
	{
		libbfio_handle_close(
		 file_io_handle,
		 error );
	}
	return( -1 );
}

/* Closes a file
 * Returns 0 if successful or -1 on error
 */
int libevtx_file_close(
     libevtx_file_t *file,
     libcerror_error_t **error )
{
	libevtx_internal_file_t *internal_file = NULL;
	static char *function                  = "libevtx_file_close";
	int result                             = 0;

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	internal_file = (libevtx_internal_file_t *) file;

	if( internal_file->file_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file - missing file IO handle.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( internal_file->file_io_handle_created_in_library != 0 )
		{
			if( libevtx_debug_print_read_offsets(
			     internal_file->file_io_handle,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
				 "%s: unable to print the read offsets.",
				 function );

				result = -1;
			}
		}
	}
#endif
	if( internal_file->file_io_handle_opened_in_library != 0 )
	{
		if( libbfio_handle_close(
		     internal_file->file_io_handle,
		     error ) != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_CLOSE_FAILED,
			 "%s: unable to close file IO handle.",
			 function );

			result = -1;
		}
		internal_file->file_io_handle_opened_in_library = 0;
	}
	if( internal_file->file_io_handle_created_in_library != 0 )
	{
		if( libbfio_handle_free(
		     &( internal_file->file_io_handle ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free file IO handle.",
			 function );

			result = -1;
		}
		internal_file->file_io_handle_created_in_library = 0;
	}
	internal_file->file_io_handle = NULL;

	if( libevtx_io_handle_clear(
	     internal_file->io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to clear IO handle.",
		 function );

		result = -1;
	}
	if( libfdata_list_free(
	     &( internal_file->recovered_records_list ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free recovered records list.",
		 function );

		result = -1;
	}
	if( libfdata_list_free(
	     &( internal_file->records_list ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free records list.",
		 function );

		result = -1;
	}
	if( libfcache_cache_free(
	     &( internal_file->records_cache ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free records cache.",
		 function );

		result = -1;
	}
	if( libfdata_vector_free(
	     &( internal_file->chunks_vector ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free chunks vector.",
		 function );

		result = -1;
	}
	if( libfcache_cache_free(
	     &( internal_file->chunks_cache ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free chunks cache.",
		 function );

		result = -1;
	}
	return( result );
}

/* Opens a file for reading
 * Returns 1 if successful or -1 on error
 */
int libevtx_file_open_read(
     libevtx_internal_file_t *internal_file,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error )
{
	libevtx_chunk_t *chunk                 = NULL;
	libevtx_record_values_t *record_values = NULL;
	libevtx_chunks_table_t *chunks_table   = NULL;
	static char *function                  = "libevtx_file_open_read";
	off64_t file_offset                    = 0;
	size64_t file_size                     = 0;
	uint16_t chunk_index                   = 0;
	uint16_t number_of_chunks              = 0;
	uint16_t number_of_records             = 0;
	uint16_t record_index                  = 0;
	int element_index                      = 0;
	int result                             = 0;
	int segment_index                      = 0;

#if defined( HAVE_VERBOSE_OUTPUT )
	uint64_t previous_record_identifier    = 0;
#endif
#if defined( HAVE_DEBUG_OUTPUT )
	uint8_t *trailing_data                 = NULL;
	size_t trailing_data_size              = 0;
	ssize_t read_count                     = 0;
#endif

	if( internal_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	if( internal_file->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file - missing IO handle.",
		 function );

		return( -1 );
	}
	if( internal_file->chunks_vector != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file - chunks vector already set.",
		 function );

		return( -1 );
	}
	if( internal_file->chunks_cache != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file - chunks cache already set.",
		 function );

		return( -1 );
	}
	if( internal_file->records_list != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file - records list already set.",
		 function );

		return( -1 );
	}
	if( internal_file->recovered_records_list != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file - recovered records list already set.",
		 function );

		return( -1 );
	}
	if( internal_file->records_cache != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file - records cache already set.",
		 function );

		return( -1 );
	}
	if( libbfio_handle_get_size(
	     file_io_handle,
	     &file_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file size.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "Reading file header:\n" );
	}
#endif
	if( libevtx_io_handle_read_file_header(
	     internal_file->io_handle,
	     file_io_handle,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read file header.",
		 function );

		goto on_error;
	}
	internal_file->io_handle->chunks_data_size = file_size
	                                           - internal_file->io_handle->chunks_data_offset;

/* TODO clone function ? */
	if( libfdata_vector_initialize(
	     &( internal_file->chunks_vector ),
	     (size64_t) internal_file->io_handle->chunk_size,
	     (intptr_t *) internal_file->io_handle,
	     NULL,
	     NULL,
	     (int (*)(intptr_t *, intptr_t *, libfdata_vector_t *, libfcache_cache_t *, int, int, off64_t, size64_t, uint32_t, uint8_t, libcerror_error_t **)) &libevtx_io_handle_read_chunk,
	     NULL,
	     LIBFDATA_DATA_HANDLE_FLAG_NON_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create chunks vector.",
		 function );

		goto on_error;
	}
	if( libfdata_vector_append_segment(
	     internal_file->chunks_vector,
	     &segment_index,
	     0,
	     internal_file->io_handle->chunks_data_offset,
	     internal_file->io_handle->chunks_data_size,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append segment to chunks vector.",
		 function );

		goto on_error;
	}
	if( libfcache_cache_initialize(
	     &( internal_file->chunks_cache ),
	     LIBEVTX_MAXIMUM_CACHE_ENTRIES_CHUNKS,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create chunks cache.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "Reading chunks:\n" );
	}
#endif
	if( libevtx_chunks_table_initialize(
	     &chunks_table,
	     internal_file->io_handle,
	     internal_file->chunks_vector,
	     internal_file->chunks_cache,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create chunks table.",
		 function );

		goto on_error;
	}
/* TODO clone function ? */
	if( libfdata_list_initialize(
	     &( internal_file->records_list ),
	     (intptr_t *) chunks_table,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libevtx_chunks_table_free,
	     NULL,
	     (int (*)(intptr_t *, intptr_t *, libfdata_list_element_t *, libfcache_cache_t *, int, off64_t, size64_t, uint32_t, uint8_t, libcerror_error_t **)) &libevtx_chunks_table_read_record,
	     NULL,
	     LIBFDATA_DATA_HANDLE_FLAG_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create records list.",
		 function );

		goto on_error;
	}
	/* The chunks_table is managed by the list */

	if( libfdata_list_initialize(
	     &( internal_file->recovered_records_list ),
	     (intptr_t *) chunks_table,
	     NULL,
	     NULL,
	     (int (*)(intptr_t *, intptr_t *, libfdata_list_element_t *, libfcache_cache_t *, int, off64_t, size64_t, uint32_t, uint8_t, libcerror_error_t **)) &libevtx_chunks_table_read_record,
	     NULL,
	     LIBFDATA_DATA_HANDLE_FLAG_NON_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create recovered records list.",
		 function );

		chunks_table = NULL;

		goto on_error;
	}
	chunks_table = NULL;

	if( libfcache_cache_initialize(
	     &( internal_file->records_cache ),
	     LIBEVTX_MAXIMUM_CACHE_ENTRIES_RECORDS,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create records cache.",
		 function );

		goto on_error;
	}
	file_offset = internal_file->io_handle->chunks_data_offset;

	while( ( file_offset + internal_file->io_handle->chunk_size ) <= (off64_t) file_size )
	{
		if( libevtx_chunk_initialize(
		     &chunk,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create chunk: %" PRIu16 ".",
			 function,
			 chunk_index );

			goto on_error;
		}
		result = libevtx_chunk_read(
		          chunk,
		          internal_file->io_handle,
		          file_io_handle,
		          file_offset,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read chunk: %" PRIu16 ".",
			 function,
			 chunk_index );

			goto on_error;
		}
		else if( result == 0 )
		{
			if( chunk_index < internal_file->io_handle->number_of_chunks )
			{
#if defined( HAVE_VERBOSE_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: corruption detected in chunk: %" PRIu16 ".\n",
					 function,
					 chunk_index );
				}
#endif
				internal_file->io_handle->flags |= LIBEVTX_IO_HANDLE_FLAG_IS_CORRUPTED;
			}
		}
		else
		{
			if( ( chunk->flags & LIBEVTX_CHUNK_FLAG_IS_CORRUPTED ) != 0 )
			{
#if defined( HAVE_VERBOSE_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: corruption detected in chunk: %" PRIu16 ".\n",
					 function,
					 chunk_index );
				}
#endif
				if( chunk_index < internal_file->io_handle->number_of_chunks )
				{
					internal_file->io_handle->flags |= LIBEVTX_IO_HANDLE_FLAG_IS_CORRUPTED;
				}
			}
			if( ( chunk_index < internal_file->io_handle->number_of_chunks )
			 || ( ( chunk->flags & LIBEVTX_CHUNK_FLAG_IS_CORRUPTED ) == 0 ) )
			{
				number_of_chunks++;
			}
			if( libevtx_chunk_get_number_of_records(
			     chunk,
			     &number_of_records,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve chunk: %" PRIu16 " number of records.",
				 function,
				 chunk_index );

				goto on_error;
			}
			for( record_index = 0;
			     record_index < number_of_records;
			     record_index++ )
			{
				if( libevtx_chunk_get_record(
				     chunk,
				     record_index,
				     &record_values,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve chunk: %" PRIu16 " record: %" PRIu16 ".",
					 function,
					 chunk_index,
					 record_index );

					goto on_error;
				}
				if( record_values == NULL )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
					 "%s: missing chunk: %" PRIu16 " record: %" PRIu16 ".",
					 function,
					 chunk_index,
					 record_index );

					goto on_error;
				}
				if( record_values->identifier < internal_file->io_handle->first_record_identifier )
				{
					internal_file->io_handle->first_record_identifier = record_values->identifier;
				}
				if( record_values->identifier > internal_file->io_handle->last_record_identifier )
				{
					internal_file->io_handle->last_record_identifier = record_values->identifier;
				}
#if defined( HAVE_VERBOSE_OUTPUT )
				if( ( chunk_index == 0 )
				 && ( record_index == 0 ) )
				{
					previous_record_identifier = record_values->identifier;
				}
				else
				{
					previous_record_identifier++;

					if( record_values->identifier != previous_record_identifier )
					{
						if( libcnotify_verbose != 0 )
						{
							libcnotify_printf(
							 "%s: detected gap in record identifier ( %" PRIu64 " != %" PRIu64 " ).\n",
							 function,
							 previous_record_identifier,
							 record_values->identifier );
						}
						previous_record_identifier = record_values->identifier;
					}
				}
#endif
				/* The chunk index is stored in the element data size
				 */
				if( ( chunk_index < internal_file->io_handle->number_of_chunks )
				 || ( ( internal_file->io_handle->file_flags & LIBEVTX_FILE_FLAG_IS_DIRTY ) != 0 ) )
				{
					if( libfdata_list_append_element(
					     internal_file->records_list,
					     &element_index,
					     0,
					     file_offset + record_values->chunk_data_offset,
					     (size64_t) chunk_index,
					     0,
					     error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
						 "%s: unable to append element to records list.",
						 function );

						goto on_error;
					}
				}
				else
				{
					/* If the file is not dirty, records found in chunks outside the indicated
					 * range are considered recovered
					 */
					if( libfdata_list_append_element(
					     internal_file->recovered_records_list,
					     &element_index,
					     0,
					     file_offset + record_values->chunk_data_offset,
					     (size64_t) chunk_index,
					     0,
					     error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
						 "%s: unable to append element to recovered records list.",
						 function );

						goto on_error;
					}
				}
/* TODO cache record values ? */
			}
			if( libevtx_chunk_get_number_of_recovered_records(
			     chunk,
			     &number_of_records,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve chunk: %" PRIu16 " number of recovered records.",
				 function,
				 chunk_index );

				goto on_error;
			}
			for( record_index = 0;
			     record_index < number_of_records;
			     record_index++ )
			{
				if( libevtx_chunk_get_recovered_record(
				     chunk,
				     record_index,
				     &record_values,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve chunk: %" PRIu16 " recovered record: %" PRIu16 ".",
					 function,
					 chunk_index,
					 record_index );

					goto on_error;
				}
				if( record_values == NULL )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
					 "%s: missing chunk: %" PRIu16 " recovered record: %" PRIu16 ".",
					 function,
					 chunk_index,
					 record_index );

					goto on_error;
				}
/* TODO check for and remove duplicate identifiers ? */
				/* The chunk index is stored in the element data size
				 */
				if( libfdata_list_append_element(
				     internal_file->recovered_records_list,
				     &element_index,
				     0,
				     file_offset + record_values->chunk_data_offset,
				     (size64_t) chunk_index,
				     0,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
					 "%s: unable to append element to recovered records list.",
					 function );

					goto on_error;
				}
			}
		}
		file_offset += chunk->data_size;

		if( libevtx_chunk_free(
		     &chunk,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free chunk: %" PRIu16 ".",
			 function,
			 chunk_index );

			goto on_error;
		}
		chunk_index++;
	}
	internal_file->io_handle->chunks_data_size = file_offset
	                                           - internal_file->io_handle->chunks_data_offset;

	if( number_of_chunks != internal_file->io_handle->number_of_chunks )
	{
#if defined( HAVE_VERBOSE_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: mismatch in number of chunks ( %" PRIu16 " != %" PRIu16 " ).\n",
			 function,
			 internal_file->io_handle->number_of_chunks,
			 chunk_index );
		}
#endif
		internal_file->io_handle->flags |= LIBEVTX_IO_HANDLE_FLAG_IS_CORRUPTED;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( file_offset < (off64_t) file_size )
		{
			trailing_data_size = (size_t) ( file_size - file_offset );

			trailing_data = (uint8_t *) memory_allocate(
			                             sizeof( uint8_t ) * trailing_data_size );

			if( trailing_data == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
				 "%s: unable to create trailing data.",
				 function );

				goto on_error;
			}
			read_count = libbfio_handle_read_buffer(
				      file_io_handle,
				      trailing_data,
				      trailing_data_size,
				      error );

			if( read_count != (ssize_t) trailing_data_size )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read trailing data.",
				 function );

				memory_free(
				 trailing_data );

				goto on_error;
			}
			file_offset += read_count;

			libcnotify_printf(
			 "%s: trailing data:\n",
			 function );
			libcnotify_print_data(
			 trailing_data,
			 trailing_data_size,
			 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );

			memory_free(
			 trailing_data );

			trailing_data = NULL;
		}
	}
#endif
	return( 1 );

on_error:
#if defined( HAVE_DEBUG_OUTPUT )
	if( trailing_data != NULL )
	{
		memory_free(
		 trailing_data );
	}
#endif
	if( chunk != NULL )
	{
		libevtx_chunk_free(
		 &chunk,
		 NULL );
	}
	if( internal_file->records_cache != NULL )
	{
		libfcache_cache_free(
		 &( internal_file->records_cache ),
		 NULL );
	}
	if( internal_file->records_list != NULL )
	{
		libfdata_list_free(
		 &( internal_file->records_list ),
		 NULL );
	}
	if( chunks_table != NULL )
	{
		libevtx_chunks_table_free(
		 &chunks_table,
		 NULL );
	}
	if( internal_file->chunks_cache != NULL )
	{
		libfcache_cache_free(
		 &( internal_file->chunks_cache ),
		 NULL );
	}
	if( internal_file->chunks_vector != NULL )
	{
		libfdata_vector_free(
		 &( internal_file->chunks_vector ),
		 NULL );
	}
	return( -1 );
}

/* Determine if the file corrupted
 * Returns 1 if corrupted, 0 if not or -1 on error
 */
int libevtx_file_is_corrupted(
     libevtx_file_t *file,
     libcerror_error_t **error )
{
	libevtx_internal_file_t *internal_file = NULL;
	static char *function                  = "libevtx_file_is_corrupted";

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	internal_file = (libevtx_internal_file_t *) file;

	if( internal_file->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file - missing IO handle.",
		 function );

		return( -1 );
	}
	if( ( internal_file->io_handle->flags & LIBEVTX_IO_HANDLE_FLAG_IS_CORRUPTED ) != 0 )
	{
		return( 1 );
	}
	return( 0 );
}

/* Retrieves the file ASCII codepage
 * Returns 1 if successful or -1 on error
 */
int libevtx_file_get_ascii_codepage(
     libevtx_file_t *file,
     int *ascii_codepage,
     libcerror_error_t **error )
{
	libevtx_internal_file_t *internal_file = NULL;
	static char *function                  = "libevtx_file_get_ascii_codepage";

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	internal_file = (libevtx_internal_file_t *) file;

	if( internal_file->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file - missing IO handle.",
		 function );

		return( -1 );
	}
	if( ascii_codepage == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid ASCII codepage.",
		 function );

		return( -1 );
	}
	*ascii_codepage = internal_file->io_handle->ascii_codepage;

	return( 1 );
}

/* Sets the file ASCII codepage
 * Returns 1 if successful or -1 on error
 */
int libevtx_file_set_ascii_codepage(
     libevtx_file_t *file,
     int ascii_codepage,
     libcerror_error_t **error )
{
	libevtx_internal_file_t *internal_file = NULL;
	static char *function                  = "libevtx_file_set_ascii_codepage";

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	internal_file = (libevtx_internal_file_t *) file;

	if( internal_file->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file - missing IO handle.",
		 function );

		return( -1 );
	}
	if( ( ascii_codepage != LIBEVTX_CODEPAGE_ASCII )
	 && ( ascii_codepage != LIBEVTX_CODEPAGE_WINDOWS_874 )
	 && ( ascii_codepage != LIBEVTX_CODEPAGE_WINDOWS_932 )
	 && ( ascii_codepage != LIBEVTX_CODEPAGE_WINDOWS_936 )
	 && ( ascii_codepage != LIBEVTX_CODEPAGE_WINDOWS_949 )
	 && ( ascii_codepage != LIBEVTX_CODEPAGE_WINDOWS_950 )
	 && ( ascii_codepage != LIBEVTX_CODEPAGE_WINDOWS_1250 )
	 && ( ascii_codepage != LIBEVTX_CODEPAGE_WINDOWS_1251 )
	 && ( ascii_codepage != LIBEVTX_CODEPAGE_WINDOWS_1252 )
	 && ( ascii_codepage != LIBEVTX_CODEPAGE_WINDOWS_1253 )
	 && ( ascii_codepage != LIBEVTX_CODEPAGE_WINDOWS_1254 )
	 && ( ascii_codepage != LIBEVTX_CODEPAGE_WINDOWS_1255 )
	 && ( ascii_codepage != LIBEVTX_CODEPAGE_WINDOWS_1256 )
	 && ( ascii_codepage != LIBEVTX_CODEPAGE_WINDOWS_1257 )
	 && ( ascii_codepage != LIBEVTX_CODEPAGE_WINDOWS_1258 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported ASCII codepage.",
		 function );

		return( -1 );
	}
	internal_file->io_handle->ascii_codepage = ascii_codepage;

	return( 1 );
}

/* Retrieves the format version
 * Returns 1 if successful or -1 on error
 */
int libevtx_file_get_format_version(
     libevtx_file_t *file,
     uint16_t *major_version,
     uint16_t *minor_version,
     libcerror_error_t **error )
{
	libevtx_internal_file_t *internal_file = NULL;
	static char *function                  = "libevtx_file_get_format_version";

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	internal_file = (libevtx_internal_file_t *) file;

	if( internal_file->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file - missing IO handle.",
		 function );

		return( -1 );
	}
	if( major_version == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid major version.",
		 function );

		return( -1 );
	}
	if( minor_version == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid minor version.",
		 function );

		return( -1 );
	}
	*major_version = internal_file->io_handle->major_version;
	*minor_version = internal_file->io_handle->minor_version;

	return( 1 );
}

/* Retrieves the file version
 * Returns 1 if successful or -1 on error
 */
int libevtx_file_get_version(
     libevtx_file_t *file,
     uint16_t *major_version,
     uint16_t *minor_version,
     libcerror_error_t **error )
{
	libevtx_internal_file_t *internal_file = NULL;
	static char *function                  = "libevtx_file_get_version";

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	internal_file = (libevtx_internal_file_t *) file;

	if( internal_file->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file - missing IO handle.",
		 function );

		return( -1 );
	}
	if( major_version == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid major version.",
		 function );

		return( -1 );
	}
	if( minor_version == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid minor version.",
		 function );

		return( -1 );
	}
	*major_version = internal_file->io_handle->major_version;
	*minor_version = internal_file->io_handle->minor_version;

	return( 1 );
}

/* Retrieves the flags
 * Returns 1 if successful or -1 on error
 */
int libevtx_file_get_flags(
     libevtx_file_t *file,
     uint32_t *flags,
     libcerror_error_t **error )
{
	libevtx_internal_file_t *internal_file = NULL;
	static char *function                  = "libevtx_file_get_flags";

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	internal_file = (libevtx_internal_file_t *) file;

	if( internal_file->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file - missing IO handle.",
		 function );

		return( -1 );
	}
	if( flags == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid flags.",
		 function );

		return( -1 );
	}
	*flags = internal_file->io_handle->file_flags;

	return( 1 );
}

/* Retrieves the number of records
 * Returns 1 if successful or -1 on error
 */
int libevtx_file_get_number_of_records(
     libevtx_file_t *file,
     int *number_of_records,
     libcerror_error_t **error )
{
	libevtx_internal_file_t *internal_file = NULL;
	static char *function                  = "libevtx_file_get_number_of_records";

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	internal_file = (libevtx_internal_file_t *) file;

	if( libfdata_list_get_number_of_elements(
	     internal_file->records_list,
	     number_of_records,
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
	return( 1 );
}

/* Retrieves a specific record
 * Returns 1 if successful or -1 on error
 */
int libevtx_file_get_record(
     libevtx_file_t *file,
     int record_index,
     libevtx_record_t **record,
     libcerror_error_t **error )
{
	libevtx_internal_file_t *internal_file = NULL;
	libevtx_record_values_t *record_values = NULL;
	static char *function                  = "libevtx_file_get_record";

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	internal_file = (libevtx_internal_file_t *) file;

	if( record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	if( libfdata_list_get_element_value_by_index(
	     internal_file->records_list,
	     (intptr_t *) internal_file->file_io_handle,
	     internal_file->records_cache,
	     record_index,
	     (intptr_t **) &record_values,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve record values: %d.",
		 function,
		 record_index );

		return( -1 );
	}
	if( libevtx_record_initialize(
	     record,
	     internal_file->io_handle,
	     internal_file->file_io_handle,
	     record_values,
	     LIBEVTX_RECORD_FLAGS_DEFAULT,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create record.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the number of recovered records
 * Returns 1 if successful or -1 on error
 */
int libevtx_file_get_number_of_recovered_records(
     libevtx_file_t *file,
     int *number_of_records,
     libcerror_error_t **error )
{
	libevtx_internal_file_t *internal_file = NULL;
	static char *function                  = "libevtx_file_get_number_of_recovered_records";

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	internal_file = (libevtx_internal_file_t *) file;

	if( libfdata_list_get_number_of_elements(
	     internal_file->recovered_records_list,
	     number_of_records,
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
	return( 1 );
}

/* Retrieves a specific recovered record
 * Returns 1 if successful or -1 on error
 */
int libevtx_file_get_recovered_record(
     libevtx_file_t *file,
     int record_index,
     libevtx_record_t **record,
     libcerror_error_t **error )
{
	libevtx_internal_file_t *internal_file = NULL;
	libevtx_record_values_t *record_values = NULL;
	static char *function                  = "libevtx_file_get_recovered_record";

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	internal_file = (libevtx_internal_file_t *) file;

	if( record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	if( libfdata_list_get_element_value_by_index(
	     internal_file->recovered_records_list,
	     (intptr_t *) internal_file->file_io_handle,
	     internal_file->records_cache,
	     record_index,
	     (intptr_t **) &record_values,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve record values: %d.",
		 function,
		 record_index );

		return( -1 );
	}
	if( libevtx_record_initialize(
	     record,
	     internal_file->io_handle,
	     internal_file->file_io_handle,
	     record_values,
	     LIBEVTX_RECORD_FLAGS_DEFAULT,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create record.",
		 function );

		return( -1 );
	}
	return( 1 );
}

