/*
 * Support functions
 *
 * Copyright (c) 2011, Joachim Metz <jbmetz@users.sourceforge.net>
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

#include <libcstring.h>
#include <liberror.h>

#include "libevtx_codepage.h"
#include "libevtx_definitions.h"
#include "libevtx_io_handle.h"
#include "libevtx_libbfio.h"
#include "libevtx_support.h"

#if !defined( HAVE_LOCAL_LIBEVTX )

/* Returns the library version
 */
const char *libevtx_get_version(
             void )
{
	return( (const char *) LIBEVTX_VERSION_STRING );
}

/* Returns the access flags for reading
 */
int libevtx_get_access_flags_read(
     void )
{
	return( (uint8_t) LIBEVTX_ACCESS_FLAG_READ );
}

/* Retrieves the narrow system string codepage
 * A value of 0 represents no codepage, UTF-8 encoding is used instead
 * Returns 1 if successful or -1 on error
 */
int libevtx_get_codepage(
     int *codepage,
     liberror_error_t **error )
{
	static char *function = "libevtx_get_codepage";

	if( codepage == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid codepage.",
		 function );

		return( -1 );
	}
	*codepage = libcstring_narrow_system_string_codepage;

	return( 1 );
}

/* Sets the narrow system string codepage
 * A value of 0 represents no codepage, UTF-8 encoding is used instead
 * Returns 1 if successful or -1 on error
 */
int libevtx_set_codepage(
     int codepage,
     liberror_error_t **error )
{
	static char *function = "libevtx_set_codepage";

	if( ( codepage != LIBEVTX_CODEPAGE_ASCII )
	 && ( codepage != LIBEVTX_CODEPAGE_ISO_8859_1 )
	 && ( codepage != LIBEVTX_CODEPAGE_ISO_8859_2 )
	 && ( codepage != LIBEVTX_CODEPAGE_ISO_8859_3 )
	 && ( codepage != LIBEVTX_CODEPAGE_ISO_8859_4 )
	 && ( codepage != LIBEVTX_CODEPAGE_ISO_8859_5 )
	 && ( codepage != LIBEVTX_CODEPAGE_ISO_8859_6 )
	 && ( codepage != LIBEVTX_CODEPAGE_ISO_8859_7 )
	 && ( codepage != LIBEVTX_CODEPAGE_ISO_8859_8 )
	 && ( codepage != LIBEVTX_CODEPAGE_ISO_8859_9 )
	 && ( codepage != LIBEVTX_CODEPAGE_ISO_8859_10 )
	 && ( codepage != LIBEVTX_CODEPAGE_ISO_8859_11 )
	 && ( codepage != LIBEVTX_CODEPAGE_ISO_8859_13 )
	 && ( codepage != LIBEVTX_CODEPAGE_ISO_8859_14 )
	 && ( codepage != LIBEVTX_CODEPAGE_ISO_8859_15 )
	 && ( codepage != LIBEVTX_CODEPAGE_ISO_8859_16 )
	 && ( codepage != LIBEVTX_CODEPAGE_KOI8_R )
	 && ( codepage != LIBEVTX_CODEPAGE_KOI8_U )
	 && ( codepage != LIBEVTX_CODEPAGE_WINDOWS_874 )
	 && ( codepage != LIBEVTX_CODEPAGE_WINDOWS_1250 )
	 && ( codepage != LIBEVTX_CODEPAGE_WINDOWS_1251 )
	 && ( codepage != LIBEVTX_CODEPAGE_WINDOWS_1252 )
	 && ( codepage != LIBEVTX_CODEPAGE_WINDOWS_1253 )
	 && ( codepage != LIBEVTX_CODEPAGE_WINDOWS_1254 )
	 && ( codepage != LIBEVTX_CODEPAGE_WINDOWS_1256 )
	 && ( codepage != LIBEVTX_CODEPAGE_WINDOWS_1257 )
	 && ( codepage != LIBEVTX_CODEPAGE_WINDOWS_1258 )
	 && ( codepage != 0 ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported codepage.",
		 function );

		return( -1 );
	}
	libcstring_narrow_system_string_codepage = codepage;

	return( 1 );
}

#endif /* !defined( HAVE_LOCAL_LIBEVTX ) */

/* Determines if a file is an EDB file (check for the EDB file signature)
 * Returns 1 if true, 0 if not or -1 on error
 */
int libevtx_check_file_signature(
     const char *filename,
     liberror_error_t **error )
{
	libbfio_handle_t *file_io_handle = NULL;
	static char *function            = "libevtx_check_file_signature";
	size_t filename_length           = 0;
	int result                       = 0;

	if( filename == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filename.",
		 function );

		return( -1 );
	}
	filename_length = libcstring_narrow_string_length(
	                   filename );

	if( filename_length == 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filename.",
		 function );

		return( -1 );
	}
	if( libbfio_file_initialize(
	     &file_io_handle,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create file IO handle.",
		 function );

		return( -1 );
	}
	if( libbfio_file_set_name(
	     file_io_handle,
	     filename,
	     filename_length,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set filename in file IO handle.",
		 function );

		libbfio_handle_free(
		 &file_io_handle,
		 NULL );

		return( -1 );
	}
	result = libevtx_check_file_signature_file_io_handle(
	          file_io_handle,
	          error );

	if( result == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to check file signature using a file handle.",
		 function );

		libbfio_handle_free(
		 &file_io_handle,
		 NULL );

		return( -1 );
	}
	if( libbfio_handle_free(
	     &file_io_handle,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to free file IO handle.",
		 function );

		return( -1 );
	}
	return( result );
}

#if defined( HAVE_WIDE_CHARACTER_TYPE )

/* Determines if a file is an EDB file (check for the EDB file signature)
 * Returns 1 if true, 0 if not or -1 on error
 */
int libevtx_check_file_signature_wide(
     const wchar_t *filename,
     liberror_error_t **error )
{
	libbfio_handle_t *file_io_handle = NULL;
	static char *function            = "libevtx_check_file_signature_wide";
	size_t filename_length           = 0;
	int result                       = 0;

	if( filename == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filename.",
		 function );

		return( -1 );
	}
	filename_length = libcstring_wide_string_length(
	                   filename );

	if( filename_length == 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filename.",
		 function );

		return( -1 );
	}
	if( libbfio_file_initialize(
	     &file_io_handle,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create file IO handle.",
		 function );

		return( -1 );
	}
	if( libbfio_file_set_name_wide(
	     file_io_handle,
	     filename,
	     filename_length,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set filename in file IO handle.",
		 function );

		libbfio_handle_free(
		 &file_io_handle,
		 NULL );

		return( -1 );
	}
	result = libevtx_check_file_signature_file_io_handle(
	          file_io_handle,
	          error );

	if( result == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to check file signature using a file handle.",
		 function );

		libbfio_handle_free(
		 &file_io_handle,
		 NULL );

		return( -1 );
	}
	if( libbfio_handle_free(
	     &file_io_handle,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to free file IO handle.",
		 function );

		return( -1 );
	}
	return( result );
}

#endif

/* Determines if a file is an EDB file (check for the EDB file signature) using a Basic File IO (bfio) handle
 * Returns 1 if true, 0 if not or -1 on error
 */
int libevtx_check_file_signature_file_io_handle(
     libbfio_handle_t *file_io_handle,
     liberror_error_t **error )
{
	uint8_t signature[ 8 ];

	static char *function      = "libevtx_check_file_signature_file_io_handle";
	ssize_t read_count         = 0;
	int file_io_handle_is_open = 0;

	if( file_io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file IO handle.",
		 function );

		return( -1 );
	}
	file_io_handle_is_open = libbfio_handle_is_open(
	                          file_io_handle,
	                          error );

	if( file_io_handle_is_open == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open file.",
		 function );

		return( -1 );
	}
	else if( file_io_handle_is_open == 0 )
	{
		if( libbfio_handle_open(
		     file_io_handle,
		     LIBBFIO_OPEN_READ,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_OPEN_FAILED,
			 "%s: unable to open file.",
			 function );

			return( -1 );
		}
	}
	if( libbfio_handle_seek_offset(
	     file_io_handle,
	     0,
	     SEEK_SET,
	     error ) == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek file header offset: 0.",
		 function );

		if( file_io_handle_is_open == 0 )
		{
			libbfio_handle_close(
			 file_io_handle,
			 error );
		}
		return( -1 );
	}
	read_count = libbfio_handle_read(
	              file_io_handle,
	              signature,
	              8,
	              error );

	if( read_count != 8 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read signature.",
		 function );

		if( file_io_handle_is_open == 0 )
		{
			libbfio_handle_close(
			 file_io_handle,
			 error );
		}
		return( -1 );
	}
	if( file_io_handle_is_open == 0 )
	{
		if( libbfio_handle_close(
		     file_io_handle,
		     error ) != 0 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_CLOSE_FAILED,
			 "%s: unable to close file.",
			 function );

			return( -1 );
		}
	}
	if( memory_compare(
	     evtx_file_signature,
	     signature,
	     8 ) == 0 )
	{
		return( 1 );
	}
	return( 0 );
}

