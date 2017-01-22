/*
 * Debug functions
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
#include <types.h>

#include "libevtx_debug.h"
#include "libevtx_definitions.h"
#include "libevtx_libbfio.h"
#include "libevtx_libcerror.h"
#include "libevtx_libcnotify.h"

#if defined( HAVE_DEBUG_OUTPUT )

/* Prints the file flags
 */
void libevtx_debug_print_file_flags(
      uint32_t file_flags )
{
	if( ( file_flags & LIBEVTX_FILE_FLAG_IS_DIRTY ) != 0 )
	{
		libcnotify_printf(
		 "Is dirty\n" );
	}
	if( ( file_flags & LIBEVTX_FILE_FLAG_IS_FULL ) != 0 )
	{
		libcnotify_printf(
		 "Is full\n" );
	}
}

/* Prints the value type
 */
void libevtx_debug_print_value_type(
      uint8_t value_type )
{
	switch( value_type & 0x7f )
	{
		case LIBEVTX_VALUE_TYPE_NULL:
			libcnotify_printf(
			 "NULL" );
			break;

		case LIBEVTX_VALUE_TYPE_STRING_UTF16:
			libcnotify_printf(
			 "UTF-16 string" );
			break;

		case LIBEVTX_VALUE_TYPE_STRING_BYTE_STREAM:
			libcnotify_printf(
			 "Byte stream string" );
			break;

		case LIBEVTX_VALUE_TYPE_INTEGER_8BIT:
			libcnotify_printf(
			 "Integer 8-bit signed" );
			break;

		case LIBEVTX_VALUE_TYPE_UNSIGNED_INTEGER_8BIT:
			libcnotify_printf(
			 "Integer 8-bit unsigned" );
			break;

		case LIBEVTX_VALUE_TYPE_INTEGER_16BIT:
			libcnotify_printf(
			 "Integer 16-bit signed" );
			break;

		case LIBEVTX_VALUE_TYPE_UNSIGNED_INTEGER_16BIT:
			libcnotify_printf(
			 "Integer 16-bit unsigned" );
			break;

		case LIBEVTX_VALUE_TYPE_INTEGER_32BIT:
			libcnotify_printf(
			 "Integer 32-bit signed" );
			break;

		case LIBEVTX_VALUE_TYPE_UNSIGNED_INTEGER_32BIT:
			libcnotify_printf(
			 "Integer 32-bit unsigned" );
			break;

		case LIBEVTX_VALUE_TYPE_INTEGER_64BIT:
			libcnotify_printf(
			 "Integer 64-bit signed" );
			break;

		case LIBEVTX_VALUE_TYPE_UNSIGNED_INTEGER_64BIT:
			libcnotify_printf(
			 "Integer 64-bit unsigned" );
			break;

		case LIBEVTX_VALUE_TYPE_FLOATING_POINT_32BIT:
			libcnotify_printf(
			 "Floating point 32-bit (single precision)" );
			break;

		case LIBEVTX_VALUE_TYPE_FLOATING_POINT_64BIT:
			libcnotify_printf(
			 "Floating point 64-bit (double precision)" );
			break;

		case LIBEVTX_VALUE_TYPE_BOOLEAN:
			libcnotify_printf(
			 "Boolean" );
			break;

		case LIBEVTX_VALUE_TYPE_BINARY_DATA:
			libcnotify_printf(
			 "Binary data" );
			break;

		case LIBEVTX_VALUE_TYPE_GUID:
			libcnotify_printf(
			 "GUID" );
			break;

		case LIBEVTX_VALUE_TYPE_SIZE:
			libcnotify_printf(
			 "Size" );
			break;

		case LIBEVTX_VALUE_TYPE_FILETIME:
			libcnotify_printf(
			 "Filetime" );
			break;

		case LIBEVTX_VALUE_TYPE_SYSTEMTIME:
			libcnotify_printf(
			 "Systemtime" );
			break;

		case LIBEVTX_VALUE_TYPE_NT_SECURITY_IDENTIFIER:
			libcnotify_printf(
			 "NT Security Identifier (SID)" );
			break;

		case LIBEVTX_VALUE_TYPE_HEXADECIMAL_INTEGER_32BIT:
			libcnotify_printf(
			 "Hexadecimal integer 32-bit" );
			break;

		case LIBEVTX_VALUE_TYPE_HEXADECIMAL_INTEGER_64BIT:
			libcnotify_printf(
			 "Hexadecimal integer 64-bit" );
			break;

		case LIBEVTX_VALUE_TYPE_BINARY_XML:
			libcnotify_printf(
			 "Binary XML" );
			break;

		default:
			libcnotify_printf(
			 "UNKNOWN" );
			break;
	}
}

/* Prints the read offsets
 * Returns 1 if successful or -1 on error
 */
int libevtx_debug_print_read_offsets(
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error )
{
	static char *function = "libevtx_debug_print_read_offsets";
	off64_t offset        = 0;
	size64_t size         = 0;
	int number_of_offsets = 0;
	int offset_iterator   = 0;

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
	if( libbfio_handle_get_number_of_offsets_read(
	     file_io_handle,
	     &number_of_offsets,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of offsets read.",
		 function );

		return( -1 );
	}
	libcnotify_printf(
	 "Offsets read:\n" );

	for( offset_iterator = 0;
	     offset_iterator < number_of_offsets;
	     offset_iterator++ )
	{
		if( libbfio_handle_get_offset_read(
		     file_io_handle,
		     offset_iterator,
		     &offset,
		     &size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve offset: %d.",
			 function,
			 offset_iterator );

			return( -1 );
		}
		libcnotify_printf(
		 "%08" PRIi64 " ( 0x%08" PRIx64 " ) - %08" PRIi64 " ( 0x%08" PRIx64 " ) size: %" PRIi64 "\n",
		 offset,
		 offset,
		 offset + (off64_t) size,
		 offset + (off64_t) size,
		 size );
	}
	libcnotify_printf(
	 "\n" );

	return( 1 );
}

#endif

