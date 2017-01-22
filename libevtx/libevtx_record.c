/*
 * Record functions
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

#include "libevtx_definitions.h"
#include "libevtx_io_handle.h"
#include "libevtx_libbfio.h"
#include "libevtx_libcerror.h"
#include "libevtx_record.h"
#include "libevtx_record_values.h"

/* Creates a record
 * Make sure the value record is referencing, is set to NULL
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
	static char *function                      = "libevtx_record_initialize";

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
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
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
	static char *function                      = "libevtx_record_free";

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

/* Retrieves the offset
 * Returns 1 if successful or -1 on error
 */
int libevtx_record_get_offset(
     libevtx_record_t *record,
     off64_t *offset,
     libcerror_error_t **error )
{
	libevtx_internal_record_t *internal_record = NULL;
	static char *function                      = "libevtx_record_get_offset";

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
		 "%s: invalid record - missing record values.",
		 function );

		return( -1 );
	}
	if( offset == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid offset.",
		 function );

		return( -1 );
	}
	*offset = internal_record->record_values->offset;

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
	static char *function                      = "libevtx_record_get_identifier";

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
		 "%s: invalid record - missing record values.",
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

/* Retrieves the 64-bit FILETIME value containing the written time
 * Returns 1 if successful or -1 on error
 */
int libevtx_record_get_written_time(
     libevtx_record_t *record,
     uint64_t *filetime,
     libcerror_error_t **error )
{
	libevtx_internal_record_t *internal_record = NULL;
	static char *function                      = "libevtx_record_get_written_time";

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
		 "%s: invalid record - missing record values.",
		 function );

		return( -1 );
	}
	if( filetime == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filetime.",
		 function );

		return( -1 );
	}
	*filetime = internal_record->record_values->written_time;

	return( 1 );
}

/* Retrieves the event identifier
 * Returns 1 if successful or -1 on error
 */
int libevtx_record_get_event_identifier(
     libevtx_record_t *record,
     uint32_t *event_identifier,
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

	if( libevtx_record_values_get_event_identifier(
	     internal_record->record_values,
	     event_identifier,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve event identifier from record values.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the event identifier qualifiers
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libevtx_record_get_event_identifier_qualifiers(
     libevtx_record_t *record,
     uint32_t *event_identifier_qualifiers,
     libcerror_error_t **error )
{
	libevtx_internal_record_t *internal_record = NULL;
	static char *function                      = "libevtx_record_get_event_identifier_qualifiers";
	int result                                 = 0;

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

	result = libevtx_record_values_get_event_identifier_qualifiers(
	          internal_record->record_values,
	          event_identifier_qualifiers,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve event identifier qualifiers from record values.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Retrieves the event level
 * Returns 1 if successful or -1 on error
 */
int libevtx_record_get_event_level(
     libevtx_record_t *record,
     uint8_t *event_level,
     libcerror_error_t **error )
{
	libevtx_internal_record_t *internal_record = NULL;
	static char *function                      = "libevtx_record_get_event_level";

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

	if( libevtx_record_values_get_event_level(
	     internal_record->record_values,
	     event_level,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve event level from record values.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of the UTF-8 encoded provider identifier
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libevtx_record_get_utf8_provider_identifier_size(
     libevtx_record_t *record,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	libevtx_internal_record_t *internal_record = NULL;
	static char *function                      = "libevtx_record_get_utf8_provider_identifier_size";
	int result                                 = 0;

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

	result = libevtx_record_values_get_utf8_provider_identifier_size(
	          internal_record->record_values,
	          utf8_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 string size of provider identifier.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Retrieves the UTF-8 encoded provider identifier
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libevtx_record_get_utf8_provider_identifier(
     libevtx_record_t *record,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	libevtx_internal_record_t *internal_record = NULL;
	static char *function                      = "libevtx_record_get_utf8_provider_identifier";
	int result                                 = 0;

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

	result = libevtx_record_values_get_utf8_provider_identifier(
	          internal_record->record_values,
	          utf8_string,
	          utf8_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy provider identifier to UTF-8 string.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Retrieves the size of the UTF-16 encoded provider identifier
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libevtx_record_get_utf16_provider_identifier_size(
     libevtx_record_t *record,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	libevtx_internal_record_t *internal_record = NULL;
	static char *function                      = "libevtx_record_get_utf16_provider_identifier_size";
	int result                                 = 0;

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

	result = libevtx_record_values_get_utf16_provider_identifier_size(
	          internal_record->record_values,
	          utf16_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-16 string size of provider identifier.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Retrieves the UTF-16 encoded provider identifier
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libevtx_record_get_utf16_provider_identifier(
     libevtx_record_t *record,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	libevtx_internal_record_t *internal_record = NULL;
	static char *function                      = "libevtx_record_get_utf16_provider_identifier";
	int result                                 = 0;

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

	result = libevtx_record_values_get_utf16_provider_identifier(
	          internal_record->record_values,
	          utf16_string,
	          utf16_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy provider identifier to UTF-16 string.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Retrieves the size of the UTF-8 encoded source name
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libevtx_record_get_utf8_source_name_size(
     libevtx_record_t *record,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	libevtx_internal_record_t *internal_record = NULL;
	static char *function                      = "libevtx_record_get_utf8_source_name_size";
	int result                                 = 0;

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

	result = libevtx_record_values_get_utf8_source_name_size(
	          internal_record->record_values,
	          utf8_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 string size of source name.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Retrieves the UTF-8 encoded source name
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libevtx_record_get_utf8_source_name(
     libevtx_record_t *record,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	libevtx_internal_record_t *internal_record = NULL;
	static char *function                      = "libevtx_record_get_utf8_source_name";
	int result                                 = 0;

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

	result = libevtx_record_values_get_utf8_source_name(
	          internal_record->record_values,
	          utf8_string,
	          utf8_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy source name to UTF-8 string.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Retrieves the size of the UTF-16 encoded source name
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libevtx_record_get_utf16_source_name_size(
     libevtx_record_t *record,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	libevtx_internal_record_t *internal_record = NULL;
	static char *function                      = "libevtx_record_get_utf16_source_name_size";
	int result                                 = 0;

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

	result = libevtx_record_values_get_utf16_source_name_size(
	          internal_record->record_values,
	          utf16_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-16 string size of source name.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Retrieves the UTF-16 encoded source name
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libevtx_record_get_utf16_source_name(
     libevtx_record_t *record,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	libevtx_internal_record_t *internal_record = NULL;
	static char *function                      = "libevtx_record_get_utf16_source_name";
	int result                                 = 0;

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

	result = libevtx_record_values_get_utf16_source_name(
	          internal_record->record_values,
	          utf16_string,
	          utf16_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy source name to UTF-16 string.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Retrieves the size of the UTF-8 encoded computer name
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libevtx_record_get_utf8_computer_name_size(
     libevtx_record_t *record,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	libevtx_internal_record_t *internal_record = NULL;
	static char *function                      = "libevtx_record_get_utf8_computer_name_size";
	int result                                 = 0;

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

	result = libevtx_record_values_get_utf8_computer_name_size(
	          internal_record->record_values,
	          utf8_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 string size of computer name.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Retrieves the UTF-8 encoded computer name
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libevtx_record_get_utf8_computer_name(
     libevtx_record_t *record,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	libevtx_internal_record_t *internal_record = NULL;
	static char *function                      = "libevtx_record_get_utf8_computer_name";
	int result                                 = 0;

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

	result = libevtx_record_values_get_utf8_computer_name(
	          internal_record->record_values,
	          utf8_string,
	          utf8_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy computer name to UTF-8 string.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Retrieves the size of the UTF-16 encoded computer name
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libevtx_record_get_utf16_computer_name_size(
     libevtx_record_t *record,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	libevtx_internal_record_t *internal_record = NULL;
	static char *function                      = "libevtx_record_get_utf16_computer_name_size";
	int result                                 = 0;

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

	result = libevtx_record_values_get_utf16_computer_name_size(
	          internal_record->record_values,
	          utf16_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-16 string size of computer name.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Retrieves the UTF-16 encoded computer name
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libevtx_record_get_utf16_computer_name(
     libevtx_record_t *record,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	libevtx_internal_record_t *internal_record = NULL;
	static char *function                      = "libevtx_record_get_utf16_computer_name";
	int result                                 = 0;

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

	result = libevtx_record_values_get_utf16_computer_name(
	          internal_record->record_values,
	          utf16_string,
	          utf16_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy computer name to UTF-16 string.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Retrieves the size of the UTF-8 encoded user security identifier
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libevtx_record_get_utf8_user_security_identifier_size(
     libevtx_record_t *record,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	libevtx_internal_record_t *internal_record = NULL;
	static char *function                      = "libevtx_record_get_utf8_user_security_identifier_size";
	int result                                 = 0;

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

	result = libevtx_record_values_get_utf8_user_security_identifier_size(
	          internal_record->record_values,
	          utf8_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 string size of user security identifier.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Retrieves the UTF-8 encoded user security identifier
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libevtx_record_get_utf8_user_security_identifier(
     libevtx_record_t *record,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	libevtx_internal_record_t *internal_record = NULL;
	static char *function                      = "libevtx_record_get_utf8_user_security_identifier";
	int result                                 = 0;

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

	result = libevtx_record_values_get_utf8_user_security_identifier(
	          internal_record->record_values,
	          utf8_string,
	          utf8_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy user security identifier to UTF-8 string.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Retrieves the size of the UTF-16 encoded user security identifier
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libevtx_record_get_utf16_user_security_identifier_size(
     libevtx_record_t *record,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	libevtx_internal_record_t *internal_record = NULL;
	static char *function                      = "libevtx_record_get_utf16_user_security_identifier_size";
	int result                                 = 0;

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

	result = libevtx_record_values_get_utf16_user_security_identifier_size(
	          internal_record->record_values,
	          utf16_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-16 string size of user security identifier.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Retrieves the UTF-16 encoded user security identifier
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libevtx_record_get_utf16_user_security_identifier(
     libevtx_record_t *record,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	libevtx_internal_record_t *internal_record = NULL;
	static char *function                      = "libevtx_record_get_utf16_user_security_identifier";
	int result                                 = 0;

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

	result = libevtx_record_values_get_utf16_user_security_identifier(
	          internal_record->record_values,
	          utf16_string,
	          utf16_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy user security identifier to UTF-16 string.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Parses the record data with a template definition
 * This function needs to be called before accessing the strings otherwise
 * the record data will be parsed without a template definition by default
 * Returns 1 if successful, 0 if data could not be parsed or -1 on error
 */
int libevtx_record_parse_data_with_template_definition(
     libevtx_record_t *record,
     libevtx_template_definition_t *template_definition,
     libcerror_error_t **error )
{
	libevtx_internal_record_t *internal_record = NULL;
	static char *function                      = "libevtx_record_parse_data_with_template_definition";
	int result                                 = 0;

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

	if( template_definition == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid template definition.",
		 function );

		return( -1 );
	}
	result = libevtx_record_values_parse_data(
	          internal_record->record_values,
	          internal_record->io_handle,
	          (libevtx_internal_template_definition_t *) template_definition,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GENERIC,
		 "%s: unable to parse data.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Retrieves the number of strings
 * Returns 1 if successful or -1 on error
 */
int libevtx_record_get_number_of_strings(
     libevtx_record_t *record,
     int *number_of_strings,
     libcerror_error_t **error )
{
	libevtx_internal_record_t *internal_record = NULL;
	static char *function                      = "libevtx_record_get_number_of_strings";

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

	if( libevtx_record_values_get_number_of_strings(
	     internal_record->record_values,
	     internal_record->io_handle,
	     number_of_strings,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to retrieve number of strings.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of a specific UTF-8 encoded string
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libevtx_record_get_utf8_string_size(
     libevtx_record_t *record,
     int string_index,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	libevtx_internal_record_t *internal_record = NULL;
	static char *function                      = "libevtx_record_get_utf8_string_size";

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

	if( libevtx_record_values_get_utf8_string_size(
	     internal_record->record_values,
	     internal_record->io_handle,
	     string_index,
	     utf8_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to retrieve size of UTF-8 string: %d.",
		 function,
		 string_index );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves a specific UTF-8 encoded string
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libevtx_record_get_utf8_string(
     libevtx_record_t *record,
     int string_index,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	libevtx_internal_record_t *internal_record = NULL;
	static char *function                      = "libevtx_record_get_utf8_string";

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

	if( libevtx_record_values_get_utf8_string(
	     internal_record->record_values,
	     internal_record->io_handle,
	     string_index,
	     utf8_string,
	     utf8_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to retrieve UTF-8 string: %d.",
		 function,
		 string_index );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of a specific UTF-16 encoded string
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libevtx_record_get_utf16_string_size(
     libevtx_record_t *record,
     int string_index,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	libevtx_internal_record_t *internal_record = NULL;
	static char *function                      = "libevtx_record_get_utf16_string_size";

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

	if( libevtx_record_values_get_utf16_string_size(
	     internal_record->record_values,
	     internal_record->io_handle,
	     string_index,
	     utf16_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to retrieve size of UTF-16 string: %d.",
		 function,
		 string_index );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves a specific UTF-16 encoded string
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libevtx_record_get_utf16_string(
     libevtx_record_t *record,
     int string_index,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	libevtx_internal_record_t *internal_record = NULL;
	static char *function                      = "libevtx_record_get_utf16_string";

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

	if( libevtx_record_values_get_utf16_string(
	     internal_record->record_values,
	     internal_record->io_handle,
	     string_index,
	     utf16_string,
	     utf16_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to retrieve UTF-16 string: %d.",
		 function,
		 string_index );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of the data
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libevtx_record_get_data_size(
     libevtx_record_t *record,
     size_t *data_size,
     libcerror_error_t **error )
{
	libevtx_internal_record_t *internal_record = NULL;
	static char *function                      = "libevtx_record_get_data_size";
	int result                                 = 0;

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

	result = libevtx_record_values_get_data_size(
	          internal_record->record_values,
	          internal_record->io_handle,
	          data_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to retrieve data size.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Retrieves the data
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libevtx_record_get_data(
     libevtx_record_t *record,
     uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	libevtx_internal_record_t *internal_record = NULL;
	static char *function                      = "libevtx_record_get_data";
	int result                                 = 0;

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

	result = libevtx_record_values_get_data(
	          internal_record->record_values,
	          internal_record->io_handle,
	          data,
	          data_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to retrieve data.",
		 function );

		return( -1 );
	}
	return( result );
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
		 "%s: unable to retrieve UTF-8 string size of event XML.",
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
		 "%s: unable to copy event XML to UTF-8 string.",
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
		 "%s: unable to retrieve UTF-16 string size of event XML.",
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
		 "%s: unable to copy event XML to UTF-16 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

