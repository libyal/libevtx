/*
 * Record functions
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

#include "libevtx_definitions.h"
#include "libevtx_io_handle.h"
#include "libevtx_libbfio.h"
#include "libevtx_libcerror.h"
#include "libevtx_record.h"
#include "libevtx_record_values.h"

/* Initializes the record and its values
 * Returns 1 if successful or -1 on error
 */
int libevtx_record_initialize(
     libevtx_record_t **record,
     libevtx_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libevtx_record_values_t *record_values,
     uint8_t flags,
     libcerror_error_t **error )
{
	libevtx_internal_record_t *internal_record = NULL;
	static char *function                     = "libevtx_record_initialize";

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
	if( *record != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid record value already set.",
		 function );

		return( -1 );
	}
	if( record_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record values.",
		 function );

		return( -1 );
	}
	if( ( flags & ~( LIBEVTX_RECORD_FLAG_MANAGED_FILE_IO_HANDLE ) ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported flags: 0x%02" PRIx8 ".",
		 function,
		 flags );

		return( -1 );
	}
	internal_record = memory_allocate_structure(
	                 libevtx_internal_record_t );

	if( internal_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create internal record.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     internal_record,
	     0,
	     sizeof( libevtx_internal_record_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear internal record.",
		 function );

		memory_free(
		 internal_record );

		return( -1 );
	}
	if( ( flags & LIBEVTX_RECORD_FLAG_MANAGED_FILE_IO_HANDLE ) == 0 )
	{
		internal_record->file_io_handle = file_io_handle;
	}
	else
	{
		if( libbfio_handle_clone(
		     &( internal_record->file_io_handle ),
		     file_io_handle,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy file IO handle.",
			 function );

			goto on_error;
		}
		if( libbfio_handle_set_open_on_demand(
		     internal_record->file_io_handle,
		     1,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to set open on demand in file IO handle.",
			 function );

			goto on_error;
		}
	}
	internal_record->io_handle     = io_handle;
	internal_record->record_values = record_values;
	internal_record->flags         = flags;

	*record = (libevtx_record_t *) internal_record;

	return( 1 );

on_error:
	if( internal_record != NULL )
	{
		if( ( flags & LIBEVTX_RECORD_FLAG_MANAGED_FILE_IO_HANDLE ) != 0 )
		{
			if( internal_record->file_io_handle != NULL )
			{
				libbfio_handle_free(
				 &( internal_record->file_io_handle ),
				 NULL );
			}
		}
		memory_free(
		 internal_record );
	}
	return( -1 );
}

/* Frees a record
 * Returns 1 if successful or -1 on error
 */
int libevtx_record_free(
     libevtx_record_t **record,
     libcerror_error_t **error )
{
	libevtx_internal_record_t *internal_record = NULL;
	static char *function                     = "libevtx_record_free";

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
	if( *record != NULL )
	{
		internal_record = (libevtx_internal_record_t *) *record;
		*record         = NULL;

		/* The io_handle and record_values references are freed elsewhere
		 */
		if( ( internal_record->flags & LIBEVTX_RECORD_FLAG_MANAGED_FILE_IO_HANDLE ) != 0 )
		{
			if( internal_record->file_io_handle != NULL )
			{
				if( libbfio_handle_close(
				     internal_record->file_io_handle,
				     error ) != 0 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_CLOSE_FAILED,
					 "%s: unable to close file IO handle.",
					 function );

					return( -1 );
				}
				if( libbfio_handle_free(
				     &( internal_record->file_io_handle ),
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
					 "%s: unable to free file IO handle.",
					 function );

					return( -1 );
				}
			}
		}
		memory_free(
		 internal_record );
	}
	return( 1 );
}

/* Retrieves the identifier (record number)
 * Returns 1 if successful or -1 on error
 */
int libevtx_record_get_identifier(
     libevtx_record_t *record,
     uint64_t *identifier,
     libcerror_error_t **error )
{
	libevtx_internal_record_t *internal_record = NULL;
	static char *function                     = "libevtx_record_get_identifier";

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
	internal_record = (libevtx_internal_record_t *) record;

	if( internal_record->record_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal record - missing record values.",
		 function );

		return( -1 );
	}
	if( identifier == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid identifier.",
		 function );

		return( -1 );
	}
	*identifier = internal_record->record_values->identifier;

	return( 1 );
}

/* Retrieves the last written time
 * The timestamp is a 64-bit FILETIME date and time value
 * Returns 1 if successful or -1 on error
 */
int libevtx_record_get_last_written_time(
     libevtx_record_t *record,
     uint64_t *last_written_time,
     libcerror_error_t **error )
{
	libevtx_internal_record_t *internal_record = NULL;
	static char *function                     = "libevtx_record_get_last_written_time";

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
	internal_record = (libevtx_internal_record_t *) record;

	if( internal_record->record_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal record - missing record values.",
		 function );

		return( -1 );
	}
	if( last_written_time == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid last written time.",
		 function );

		return( -1 );
	}
	*last_written_time = internal_record->record_values->last_written_time;

	return( 1 );
}

/* Retrieves the event identifier
 * Returns 1 if successful or -1 on error
 */
int libevtx_record_get_event_identifier(
     libevtx_record_t *record,
     uint64_t *event_identifier,
     libcerror_error_t **error )
{
	libevtx_internal_record_t *internal_record = NULL;
	static char *function                      = "libevtx_record_get_event_identifier";

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
	internal_record = (libevtx_internal_record_t *) record;

	if( internal_record->record_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal record - missing record values.",
		 function );

		return( -1 );
	}
	if( event_identifier == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid event identifier.",
		 function );

		return( -1 );
	}
	*event_identifier = internal_record->record_values->event_identifier;

	return( 1 );
}

/* Retrieves the size of the UTF-8 encoded XML string
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libevtx_record_get_utf8_xml_string_size(
     libevtx_record_t *record,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	libevtx_internal_record_t *internal_record = NULL;
	static char *function                      = "libevtx_record_get_utf8_xml_string_size";

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
	internal_record = (libevtx_internal_record_t *) record;

	if( libevtx_record_values_get_utf8_xml_string_size(
	     internal_record->record_values,
	     utf8_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 string size of record values XML.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-8 encoded XML string
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libevtx_record_get_utf8_xml_string(
     libevtx_record_t *record,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	libevtx_internal_record_t *internal_record = NULL;
	static char *function                      = "libevtx_record_get_utf8_xml_string";

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
	internal_record = (libevtx_internal_record_t *) record;

	if( libevtx_record_values_get_utf8_xml_string(
	     internal_record->record_values,
	     utf8_string,
	     utf8_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy record values XML to UTF-8 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of the UTF-16 encoded XML string
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libevtx_record_get_utf16_xml_string_size(
     libevtx_record_t *record,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	libevtx_internal_record_t *internal_record = NULL;
	static char *function                      = "libevtx_record_get_utf16_xml_string_size";

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
	internal_record = (libevtx_internal_record_t *) record;

	if( libevtx_record_values_get_utf16_xml_string_size(
	     internal_record->record_values,
	     utf16_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-16 string size of record values XML.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-16 encoded XML string
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libevtx_record_get_utf16_xml_string(
     libevtx_record_t *record,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	libevtx_internal_record_t *internal_record = NULL;
	static char *function                      = "libevtx_record_get_utf16_xml_string";

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
	internal_record = (libevtx_internal_record_t *) record;

	if( libevtx_record_values_get_utf16_xml_string(
	     internal_record->record_values,
	     utf16_string,
	     utf16_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy record values XML to UTF-16 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

