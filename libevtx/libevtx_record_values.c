/*
 * Record values functions
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
#include <byte_stream.h>
#include <memory.h>
#include <system_string.h>
#include <types.h>

#include "libevtx_byte_stream.h"
#include "libevtx_io_handle.h"
#include "libevtx_libcerror.h"
#include "libevtx_libcnotify.h"
#include "libevtx_libfdatetime.h"
#include "libevtx_libfvalue.h"
#include "libevtx_libfwevt.h"
#include "libevtx_record_values.h"
#include "libevtx_template_definition.h"

#include "evtx_event_record.h"

const uint8_t evtx_event_record_signature[ 4 ] = { 0x2a, 0x2a, 0x00, 0x00 };

/* Creates record values
 * Make sure the value record_values is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libevtx_record_values_initialize(
     libevtx_record_values_t **record_values,
     libcerror_error_t **error )
{
	static char *function = "libevtx_record_values_initialize";

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
	if( *record_values != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid record values value already set.",
		 function );

		return( -1 );
	}
	*record_values = memory_allocate_structure(
	                  libevtx_record_values_t );

	if( *record_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create record values.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *record_values,
	     0,
	     sizeof( libevtx_record_values_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear record values.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *record_values != NULL )
	{
		memory_free(
		 *record_values );

		*record_values = NULL;
	}
	return( -1 );
}

/* Frees record values
 * Returns 1 if successful or -1 on error
 */
int libevtx_record_values_free(
     libevtx_record_values_t **record_values,
     libcerror_error_t **error )
{
	static char *function = "libevtx_record_values_free";
	int result            = 1;

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
	if( *record_values != NULL )
	{
		if( ( *record_values )->string_identifiers_array != NULL )
		{
			if( libcdata_array_free(
			     &( ( *record_values )->string_identifiers_array ),
			     NULL,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free the string identifiers array.",
				 function );

				result = -1;
			}
		}
		if( ( *record_values )->strings_array != NULL )
		{
			if( libcdata_array_free(
			     &( ( *record_values )->strings_array ),
			     NULL,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free the strings array.",
				 function );

				result = -1;
			}
		}
		if( ( *record_values )->xml_document != NULL )
		{
			if( libfwevt_xml_document_free(
			     &( ( *record_values )->xml_document ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free XML document.",
				 function );

				result = -1;
			}
		}
		memory_free(
		 *record_values );

		*record_values = NULL;
	}
	return( result );
}

/* Clones the record values
 * Returns 1 if successful or -1 on error
 */
int libevtx_record_values_clone(
     libevtx_record_values_t **destination_record_values,
     libevtx_record_values_t *source_record_values,
     libcerror_error_t **error )
{
	static char *function = "libevtx_record_values_free";

	if( destination_record_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination record values.",
		 function );

		return( -1 );
	}
	if( *destination_record_values != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid destination record values value already set.",
		 function );

		return( -1 );
	}
	if( source_record_values == NULL )
	{
		*destination_record_values = NULL;

		return( 1 );
	}
	*destination_record_values = memory_allocate_structure(
	                              libevtx_record_values_t );

	if( *destination_record_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create destination record values.",
		 function );

		goto on_error;
	}
	if( memory_copy(
	     *destination_record_values,
	     source_record_values,
	     sizeof( libevtx_record_values_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy record values.",
		 function );

		goto on_error;
	}
	( *destination_record_values )->xml_document = NULL;

	if( source_record_values->xml_document != NULL )
	{
		if( libfwevt_xml_document_clone(
		     &( ( *destination_record_values )->xml_document ),
		     source_record_values->xml_document,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create destination XML document.",
			 function );

			goto on_error;
		}
	}
	return( 1 );

on_error:
	if( *destination_record_values != NULL )
	{
		memory_free(
		 *destination_record_values );

		*destination_record_values = NULL;
	}
	return( -1 );
}

/* Reads the record values header
 * Returns 1 if successful, 0 if not or -1 on error
 */
int libevtx_record_values_read_header(
     libevtx_record_values_t *record_values,
     libevtx_io_handle_t *io_handle,
     const uint8_t *chunk_data,
     size_t chunk_data_size,
     size_t chunk_data_offset,
     libcerror_error_t **error )
{
	const uint8_t *event_record_data  = NULL;
	static char *function             = "libevtx_record_values_read_header";
	size_t event_record_data_size     = 0;
	uint32_t size_copy                = 0;
	int result                        = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	system_character_t filetime_string[ 32 ];

	libfdatetime_filetime_t *filetime = NULL;
#endif

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
	if( chunk_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk data.",
		 function );

		return( -1 );
	}
	if( chunk_data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid chunk data size value exceeds maximum.",
		 function );

		goto on_error;
	}
	if( chunk_data_offset >= chunk_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid chunk data offset value out of bounds.",
		 function );

		goto on_error;
	}
	event_record_data      = &( chunk_data[ chunk_data_offset ] );
	event_record_data_size = chunk_data_size - chunk_data_offset;

	if( event_record_data_size < ( sizeof( evtx_event_record_header_t ) + 4 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid event record data size value too small.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: event record header data:\n",
		 function );
		libcnotify_print_data(
		 event_record_data,
		 sizeof( evtx_event_record_header_t ),
		 0 );
	}
#endif
	result = libevtx_byte_stream_check_for_zero_byte_fill(
	          event_record_data,
	          sizeof( evtx_event_record_header_t ),
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine of event record header is 0-byte filled.",
		 function );

		goto on_error;
	}
	else if( result != 0 )
	{
		return( 0 );
	}
	if( memory_compare(
	     ( (evtx_event_record_header_t *) event_record_data )->signature,
	     evtx_event_record_signature,
	     4 ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported event record signature.",
		 function );

		goto on_error;
	}
	record_values->chunk_data_offset = chunk_data_offset;

	byte_stream_copy_to_uint32_little_endian(
	 ( (evtx_event_record_header_t *) event_record_data )->size,
	 record_values->data_size );

	byte_stream_copy_to_uint64_little_endian(
	 ( (evtx_event_record_header_t *) event_record_data )->identifier,
	 record_values->identifier );

	byte_stream_copy_to_uint64_little_endian(
	 ( (evtx_event_record_header_t *) event_record_data )->written_time,
	 record_values->written_time );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: signature\t\t\t\t: \\x%02x\\x%02x\\x%02x\\x%02x\n",
		 function,
		 ( (evtx_event_record_header_t *) event_record_data )->signature[ 0 ],
		 ( (evtx_event_record_header_t *) event_record_data )->signature[ 1 ],
		 ( (evtx_event_record_header_t *) event_record_data )->signature[ 2 ] ,
		 ( (evtx_event_record_header_t *) event_record_data )->signature[ 3 ] );

		libcnotify_printf(
		 "%s: size\t\t\t\t\t: %" PRIu32 "\n",
		 function,
		 record_values->data_size );

		libcnotify_printf(
		 "%s: identifier\t\t\t\t: %" PRIu64 "\n",
		 function,
		 record_values->identifier );

		if( libfdatetime_filetime_initialize(
		     &filetime,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create file time.",
			 function );

			goto on_error;
		}
		if( libfdatetime_filetime_copy_from_byte_stream(
		     filetime,
		     ( (evtx_event_record_header_t *) event_record_data )->written_time,
		     8,
		     LIBFDATETIME_ENDIAN_LITTLE,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to copy file time from byte stream.",
			 function );

			goto on_error;
		}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libfdatetime_filetime_copy_to_utf16_string(
			  filetime,
			  (uint16_t *) filetime_string,
			  32,
			  LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME_NANO_SECONDS,
			  error );
#else
		result = libfdatetime_filetime_copy_to_utf8_string(
			  filetime,
			  (uint8_t *) filetime_string,
			  32,
			  LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME_NANO_SECONDS,
			  error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to copy file time to string.",
			 function );

			goto on_error;
		}
		libcnotify_printf(
		 "%s: written time\t\t\t\t: %" PRIs_SYSTEM " UTC\n",
		 function,
		 filetime_string );

		if( libfdatetime_filetime_free(
		     &filetime,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free file time.",
			 function );

			goto on_error;
		}
	}
#endif
	if( ( record_values->data_size < sizeof( evtx_event_record_header_t ) )
	 || ( record_values->data_size > ( event_record_data_size - 4 ) ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid event record data size value out of bounds.",
		 function );

		goto on_error;
	}
	byte_stream_copy_to_uint32_little_endian(
	 &( event_record_data[ record_values->data_size - 4 ] ),
	 size_copy );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: size copy\t\t\t\t: %" PRIu32 "\n",
		 function,
		 size_copy );

		libcnotify_printf(
		 "\n" );
	}
#endif
	if( record_values->data_size != size_copy )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_INPUT,
		 LIBCERROR_INPUT_ERROR_VALUE_MISMATCH,
		 "%s: value mismatch for size and size copy.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
#if defined( HAVE_DEBUG_OUTPUT )
	if( filetime != NULL )
	{
		libfdatetime_filetime_free(
		 &filetime,
		 NULL );
	}
#endif
	return( -1 );
}

/* Reads the record values XML document
 * Returns 1 if successful or -1 on error
 */
int libevtx_record_values_read_xml_document(
     libevtx_record_values_t *record_values,
     libevtx_io_handle_t *io_handle,
     const uint8_t *chunk_data,
     size_t chunk_data_size,
     libcerror_error_t **error )
{
	static char *function         = "libevtx_record_values_read_xml_document";
	size_t chunk_data_offset      = 0;
	size_t event_record_data_size = 0;
	uint8_t flags                 = 0;

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
	if( record_values->xml_document != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid record values - XML document already set.",
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
	if( chunk_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk data.",
		 function );

		return( -1 );
	}
	if( chunk_data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid chunk data size value exceeds maximum.",
		 function );

		goto on_error;
	}
	if( record_values->chunk_data_offset >= chunk_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid record values - chunk data offset value out of bounds.",
		 function );

		goto on_error;
	}
	chunk_data_offset      = record_values->chunk_data_offset
	                       + sizeof( evtx_event_record_header_t );
	event_record_data_size = record_values->data_size
	                       - ( sizeof( evtx_event_record_header_t ) + 4 );

	if( chunk_data_offset >= chunk_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid chunk data offset value out of bounds.",
		 function );

		goto on_error;
	}
	if( ( chunk_data_offset + event_record_data_size ) > chunk_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid event record data size value out of bounds.",
		 function );

		goto on_error;
	}
	if( libfwevt_xml_document_initialize(
	     &( record_values->xml_document ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create XML document.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: event record data:\n",
		 function );
		libcnotify_print_data(
		 &( chunk_data[ chunk_data_offset ] ),
		 event_record_data_size,
		 0 );
	}
#endif
	flags = LIBFWEVT_XML_DOCUMENT_READ_FLAG_HAS_DATA_OFFSETS
	      | LIBFWEVT_XML_DOCUMENT_READ_FLAG_HAS_DEPENDENCY_IDENTIFIERS;

	if( libfwevt_xml_document_read(
	     record_values->xml_document,
	     chunk_data,
	     chunk_data_size,
	     chunk_data_offset,
	     io_handle->ascii_codepage,
	     flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read binary XML document.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: XML document:\n",
		 function );

		if( libfwevt_xml_document_debug_print(
		     record_values->xml_document,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print XML document.",
			 function );

			goto on_error;
		}
	}
#endif
	return( 1 );

on_error:
	if( record_values->xml_document != NULL )
	{
		libfwevt_xml_document_free(
		 &( record_values->xml_document ),
		 NULL );
	}
	return( -1 );
}

/* Retrieves the event identifier
 * Returns 1 if successful or -1 on error
 */
int libevtx_record_values_get_event_identifier(
     libevtx_record_values_t *record_values,
     uint32_t *event_identifier,
     libcerror_error_t **error )
{
	libfvalue_value_t *event_identifier_value = NULL;
	libfwevt_xml_tag_t *root_xml_tag          = NULL;
	libfwevt_xml_tag_t *system_xml_tag        = NULL;
	static char *function                    = "libevtx_record_values_get_event_identifier";

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
	if( record_values->xml_document == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid record values - missing XML document.",
		 function );

		return( -1 );
	}
	if( record_values->event_identifier_xml_tag == NULL )
	{
		if( libfwevt_xml_document_get_root_xml_tag(
		     record_values->xml_document,
		     &root_xml_tag,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve root XML element.",
			 function );

			return( -1 );
		}
		if( libfwevt_xml_tag_get_element_by_utf8_name(
		     root_xml_tag,
		     (uint8_t *) "System",
		     6,
		     &system_xml_tag,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve System XML element.",
			 function );

			return( -1 );
		}
		if( libfwevt_xml_tag_get_element_by_utf8_name(
		     system_xml_tag,
		     (uint8_t *) "EventID",
		     7,
		     &( record_values->event_identifier_xml_tag ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve EventID XML element.",
			 function );

			return( -1 );
		}
	}
	if( libfwevt_xml_tag_get_value(
	     record_values->event_identifier_xml_tag,
	     &event_identifier_value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve EventID XML element value.",
		 function );

		return( -1 );
	}
	if( libfvalue_value_copy_to_32bit(
	     event_identifier_value,
	     0,
	     event_identifier,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy value to event identifier.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the event identifier qualifiers
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libevtx_record_values_get_event_identifier_qualifiers(
     libevtx_record_values_t *record_values,
     uint32_t *event_identifier_qualifiers,
     libcerror_error_t **error )
{
	libfvalue_value_t *qualifiers_value    = NULL;
	libfwevt_xml_tag_t *qualifiers_xml_tag = NULL;
	libfwevt_xml_tag_t *root_xml_tag       = NULL;
	libfwevt_xml_tag_t *system_xml_tag     = NULL;
	static char *function                  = "libevtx_record_values_get_event_identifier_qualifiers";
	int result                             = 0;

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
	if( record_values->xml_document == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid record values - missing XML document.",
		 function );

		return( -1 );
	}
	if( record_values->event_identifier_xml_tag == NULL )
	{
		if( libfwevt_xml_document_get_root_xml_tag(
		     record_values->xml_document,
		     &root_xml_tag,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve root XML element.",
			 function );

			return( -1 );
		}
		if( libfwevt_xml_tag_get_element_by_utf8_name(
		     root_xml_tag,
		     (uint8_t *) "System",
		     6,
		     &system_xml_tag,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve System XML element.",
			 function );

			return( -1 );
		}
		if( libfwevt_xml_tag_get_element_by_utf8_name(
		     system_xml_tag,
		     (uint8_t *) "EventID",
		     7,
		     &( record_values->event_identifier_xml_tag ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve EventID XML element.",
			 function );

			return( -1 );
		}
	}
	result = libfwevt_xml_tag_get_attribute_by_utf8_name(
	          record_values->event_identifier_xml_tag,
	          (uint8_t *) "Qualifiers",
	          10,
	          &qualifiers_xml_tag,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve Qualifiers XML attribute.",
		 function );

		return( -1 );
	}
	else if( result != 0 )
	{
		if( libfwevt_xml_tag_get_value(
		     qualifiers_xml_tag,
		     &qualifiers_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve Qualifiers XML element value.",
			 function );

			return( -1 );
		}
		if( libfvalue_value_copy_to_32bit(
		     qualifiers_value,
		     0,
		     event_identifier_qualifiers,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy value to qualifiers.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves the event level
 * Returns 1 if successful or -1 on error
 */
int libevtx_record_values_get_event_level(
     libevtx_record_values_t *record_values,
     uint8_t *event_level,
     libcerror_error_t **error )
{
	libfwevt_xml_tag_t *level_xml_tag  = NULL;
	libfwevt_xml_tag_t *root_xml_tag   = NULL;
	libfwevt_xml_tag_t *system_xml_tag = NULL;
	static char *function              = "libevtx_record_values_get_event_level";

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
	if( record_values->xml_document == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid record values - missing XML document.",
		 function );

		return( -1 );
	}
	if( record_values->level_value == NULL )
	{
		if( libfwevt_xml_document_get_root_xml_tag(
		     record_values->xml_document,
		     &root_xml_tag,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve root XML element.",
			 function );

			return( -1 );
		}
		if( libfwevt_xml_tag_get_element_by_utf8_name(
		     root_xml_tag,
		     (uint8_t *) "System",
		     6,
		     &system_xml_tag,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve System XML element.",
			 function );

			return( -1 );
		}
		if( libfwevt_xml_tag_get_element_by_utf8_name(
		     system_xml_tag,
		     (uint8_t *) "Level",
		     5,
		     &level_xml_tag,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve Level XML element.",
			 function );

			return( -1 );
		}
		if( libfwevt_xml_tag_get_value(
		     level_xml_tag,
		     &( record_values->level_value ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve Level XML element value.",
			 function );

			return( -1 );
		}
	}
	if( libfvalue_value_copy_to_8bit(
	     record_values->level_value,
	     0,
	     event_level,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy value to event level.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of the UTF-8 encoded provider identifier
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libevtx_record_values_get_utf8_provider_identifier_size(
     libevtx_record_values_t *record_values,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	libfwevt_xml_tag_t *provider_identifier_xml_tag = NULL;
	libfwevt_xml_tag_t *root_xml_tag                = NULL;
	libfwevt_xml_tag_t *system_xml_tag              = NULL;
	static char *function                           = "libevtx_record_values_get_utf8_provider_identifier_size";
	int result                                      = 0;

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
	if( record_values->xml_document == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid record values - missing XML document.",
		 function );

		return( -1 );
	}
	if( record_values->provider_xml_tag == NULL )
	{
		if( libfwevt_xml_document_get_root_xml_tag(
		     record_values->xml_document,
		     &root_xml_tag,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve root XML element.",
			 function );

			return( -1 );
		}
		result = libfwevt_xml_tag_get_element_by_utf8_name(
		          root_xml_tag,
		          (uint8_t *) "System",
		          6,
		          &system_xml_tag,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve System XML element.",
			 function );

			return( -1 );
		}
		else if( result == 0 )
		{
			return( 0 );
		}
		result = libfwevt_xml_tag_get_element_by_utf8_name(
		          system_xml_tag,
		          (uint8_t *) "Provider",
		          8,
		          &( record_values->provider_xml_tag ),
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve Provider XML element.",
			 function );

			return( -1 );
		}
		else if( result == 0 )
		{
			return( 0 );
		}
	}
	if( record_values->provider_name_value == NULL )
	{
		result = libfwevt_xml_tag_get_attribute_by_utf8_name(
		          record_values->provider_xml_tag,
		          (uint8_t *) "Guid",
		          4,
		          &provider_identifier_xml_tag,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve Guid XML attribute.",
			 function );

			return( -1 );
		}
		if( result == 0 )
		{
			return( 0 );
		}
		if( libfwevt_xml_tag_get_value(
		     provider_identifier_xml_tag,
		     &( record_values->provider_identifier_value ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve provider GUID XML element value.",
			 function );

			return( -1 );
		}
	}
	if( libfvalue_value_get_utf8_string_size(
	     record_values->provider_identifier_value,
	     0,
	     utf8_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 string size of provider identifier.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-8 encoded provider identifier
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libevtx_record_values_get_utf8_provider_identifier(
     libevtx_record_values_t *record_values,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	libfwevt_xml_tag_t *provider_identifier_xml_tag = NULL;
	libfwevt_xml_tag_t *root_xml_tag                = NULL;
	libfwevt_xml_tag_t *system_xml_tag              = NULL;
	static char *function                           = "libevtx_record_values_get_utf8_provider_identifier";
	int result                                      = 0;

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
	if( record_values->xml_document == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid record values - missing XML document.",
		 function );

		return( -1 );
	}
	if( record_values->provider_xml_tag == NULL )
	{
		if( libfwevt_xml_document_get_root_xml_tag(
		     record_values->xml_document,
		     &root_xml_tag,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve root XML element.",
			 function );

			return( -1 );
		}
		result = libfwevt_xml_tag_get_element_by_utf8_name(
		          root_xml_tag,
		          (uint8_t *) "System",
		          6,
		          &system_xml_tag,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve System XML element.",
			 function );

			return( -1 );
		}
		else if( result == 0 )
		{
			return( 0 );
		}
		result = libfwevt_xml_tag_get_element_by_utf8_name(
		          system_xml_tag,
		          (uint8_t *) "Provider",
		          8,
		          &( record_values->provider_xml_tag ),
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve Provider XML element.",
			 function );

			return( -1 );
		}
		else if( result == 0 )
		{
			return( 0 );
		}
	}
	if( record_values->provider_name_value == NULL )
	{
		result = libfwevt_xml_tag_get_attribute_by_utf8_name(
		          record_values->provider_xml_tag,
		          (uint8_t *) "Guid",
		          4,
		          &provider_identifier_xml_tag,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve Guid XML attribute.",
			 function );

			return( -1 );
		}
		if( result == 0 )
		{
			return( 0 );
		}
		if( libfwevt_xml_tag_get_value(
		     provider_identifier_xml_tag,
		     &( record_values->provider_identifier_value ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve provider GUID XML element value.",
			 function );

			return( -1 );
		}
	}
	if( libfvalue_value_copy_to_utf8_string(
	     record_values->provider_identifier_value,
	     0,
	     utf8_string,
	     utf8_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy provider identifier to UTF-8 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of the UTF-16 encoded provider identifier
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libevtx_record_values_get_utf16_provider_identifier_size(
     libevtx_record_values_t *record_values,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	libfwevt_xml_tag_t *provider_identifier_xml_tag = NULL;
	libfwevt_xml_tag_t *root_xml_tag                = NULL;
	libfwevt_xml_tag_t *system_xml_tag              = NULL;
	static char *function                           = "libevtx_record_values_get_utf16_provider_identifier_size";
	int result                                      = 0;

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
	if( record_values->xml_document == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid record values - missing XML document.",
		 function );

		return( -1 );
	}
	if( record_values->provider_xml_tag == NULL )
	{
		if( libfwevt_xml_document_get_root_xml_tag(
		     record_values->xml_document,
		     &root_xml_tag,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve root XML element.",
			 function );

			return( -1 );
		}
		result = libfwevt_xml_tag_get_element_by_utf8_name(
		          root_xml_tag,
		          (uint8_t *) "System",
		          6,
		          &system_xml_tag,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve System XML element.",
			 function );

			return( -1 );
		}
		else if( result == 0 )
		{
			return( 0 );
		}
		result = libfwevt_xml_tag_get_element_by_utf8_name(
		          system_xml_tag,
		          (uint8_t *) "Provider",
		          8,
		          &( record_values->provider_xml_tag ),
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve Provider XML element.",
			 function );

			return( -1 );
		}
		else if( result == 0 )
		{
			return( 0 );
		}
	}
	if( record_values->provider_name_value == NULL )
	{
		result = libfwevt_xml_tag_get_attribute_by_utf8_name(
		          record_values->provider_xml_tag,
		          (uint8_t *) "Guid",
		          4,
		          &provider_identifier_xml_tag,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve Guid XML attribute.",
			 function );

			return( -1 );
		}
		if( result == 0 )
		{
			return( 0 );
		}
		if( libfwevt_xml_tag_get_value(
		     provider_identifier_xml_tag,
		     &( record_values->provider_identifier_value ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve provider GUID XML element value.",
			 function );

			return( -1 );
		}
	}
	if( libfvalue_value_get_utf16_string_size(
	     record_values->provider_identifier_value,
	     0,
	     utf16_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-16 string size of provider identifier.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-16 encoded provider identifier
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libevtx_record_values_get_utf16_provider_identifier(
     libevtx_record_values_t *record_values,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	libfwevt_xml_tag_t *provider_identifier_xml_tag = NULL;
	libfwevt_xml_tag_t *root_xml_tag                = NULL;
	libfwevt_xml_tag_t *system_xml_tag              = NULL;
	static char *function                           = "libevtx_record_values_get_utf16_provider_identifier";
	int result                                      = 0;

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
	if( record_values->xml_document == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid record values - missing XML document.",
		 function );

		return( -1 );
	}
	if( record_values->provider_xml_tag == NULL )
	{
		if( libfwevt_xml_document_get_root_xml_tag(
		     record_values->xml_document,
		     &root_xml_tag,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve root XML element.",
			 function );

			return( -1 );
		}
		result = libfwevt_xml_tag_get_element_by_utf8_name(
		          root_xml_tag,
		          (uint8_t *) "System",
		          6,
		          &system_xml_tag,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve System XML element.",
			 function );

			return( -1 );
		}
		else if( result == 0 )
		{
			return( 0 );
		}
		result = libfwevt_xml_tag_get_element_by_utf8_name(
		          system_xml_tag,
		          (uint8_t *) "Provider",
		          8,
		          &( record_values->provider_xml_tag ),
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve Provider XML element.",
			 function );

			return( -1 );
		}
		else if( result == 0 )
		{
			return( 0 );
		}
	}
	if( record_values->provider_name_value == NULL )
	{
		result = libfwevt_xml_tag_get_attribute_by_utf8_name(
		          record_values->provider_xml_tag,
		          (uint8_t *) "Guid",
		          4,
		          &provider_identifier_xml_tag,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve Guid XML attribute.",
			 function );

			return( -1 );
		}
		if( result == 0 )
		{
			return( 0 );
		}
		if( libfwevt_xml_tag_get_value(
		     provider_identifier_xml_tag,
		     &( record_values->provider_identifier_value ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve provider GUID XML element value.",
			 function );

			return( -1 );
		}
	}
	if( libfvalue_value_copy_to_utf16_string(
	     record_values->provider_identifier_value,
	     0,
	     utf16_string,
	     utf16_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy provider identifier to UTF-16 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of the UTF-8 encoded source name
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libevtx_record_values_get_utf8_source_name_size(
     libevtx_record_values_t *record_values,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	libfwevt_xml_tag_t *provider_name_xml_tag = NULL;
	libfwevt_xml_tag_t *root_xml_tag          = NULL;
	libfwevt_xml_tag_t *system_xml_tag        = NULL;
	static char *function                     = "libevtx_record_values_get_utf8_source_name_size";
	int result                                = 0;

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
	if( record_values->xml_document == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid record values - missing XML document.",
		 function );

		return( -1 );
	}
	if( record_values->provider_xml_tag == NULL )
	{
		if( libfwevt_xml_document_get_root_xml_tag(
		     record_values->xml_document,
		     &root_xml_tag,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve root XML element.",
			 function );

			return( -1 );
		}
		result = libfwevt_xml_tag_get_element_by_utf8_name(
		          root_xml_tag,
		          (uint8_t *) "System",
		          6,
		          &system_xml_tag,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve System XML element.",
			 function );

			return( -1 );
		}
		else if( result == 0 )
		{
			return( 0 );
		}
		result = libfwevt_xml_tag_get_element_by_utf8_name(
		          system_xml_tag,
		          (uint8_t *) "Provider",
		          8,
		          &( record_values->provider_xml_tag ),
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve Provider XML element.",
			 function );

			return( -1 );
		}
		else if( result == 0 )
		{
			return( 0 );
		}
	}
	if( record_values->provider_name_value == NULL )
	{
		result = libfwevt_xml_tag_get_attribute_by_utf8_name(
		          record_values->provider_xml_tag,
		          (uint8_t *) "EventSourceName",
		          15,
		          &provider_name_xml_tag,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve EventSourceName XML attribute.",
			 function );

			return( -1 );
		}
		else if( result == 0 )
		{
			result = libfwevt_xml_tag_get_attribute_by_utf8_name(
				  record_values->provider_xml_tag,
				  (uint8_t *) "Name",
				  4,
				  &provider_name_xml_tag,
				  error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve Name XML attribute.",
				 function );

				return( -1 );
			}
		}
		if( result == 0 )
		{
			return( 0 );
		}
		if( libfwevt_xml_tag_get_value(
		     provider_name_xml_tag,
		     &( record_values->provider_name_value ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve provider name XML element value.",
			 function );

			return( -1 );
		}
	}
	if( libfvalue_value_get_utf8_string_size(
	     record_values->provider_name_value,
	     0,
	     utf8_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 string size of provider name.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-8 encoded source name
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libevtx_record_values_get_utf8_source_name(
     libevtx_record_values_t *record_values,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	libfwevt_xml_tag_t *provider_name_xml_tag = NULL;
	libfwevt_xml_tag_t *root_xml_tag          = NULL;
	libfwevt_xml_tag_t *system_xml_tag        = NULL;
	static char *function                     = "libevtx_record_values_get_utf8_source_name";
	int result                                = 0;

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
	if( record_values->xml_document == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid record values - missing XML document.",
		 function );

		return( -1 );
	}
	if( record_values->provider_xml_tag == NULL )
	{
		if( libfwevt_xml_document_get_root_xml_tag(
		     record_values->xml_document,
		     &root_xml_tag,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve root XML element.",
			 function );

			return( -1 );
		}
		result = libfwevt_xml_tag_get_element_by_utf8_name(
		          root_xml_tag,
		          (uint8_t *) "System",
		          6,
		          &system_xml_tag,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve System XML element.",
			 function );

			return( -1 );
		}
		else if( result == 0 )
		{
			return( 0 );
		}
		result = libfwevt_xml_tag_get_element_by_utf8_name(
		          system_xml_tag,
		          (uint8_t *) "Provider",
		          8,
		          &( record_values->provider_xml_tag ),
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve Provider XML element.",
			 function );

			return( -1 );
		}
		else if( result == 0 )
		{
			return( 0 );
		}
	}
	if( record_values->provider_name_value == NULL )
	{
		result = libfwevt_xml_tag_get_attribute_by_utf8_name(
		          record_values->provider_xml_tag,
		          (uint8_t *) "EventSourceName",
		          15,
		          &provider_name_xml_tag,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve EventSourceName XML attribute.",
			 function );

			return( -1 );
		}
		else if( result == 0 )
		{
			result = libfwevt_xml_tag_get_attribute_by_utf8_name(
				  record_values->provider_xml_tag,
				  (uint8_t *) "Name",
				  4,
				  &provider_name_xml_tag,
				  error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve Name XML attribute.",
				 function );

				return( -1 );
			}
		}
		if( result == 0 )
		{
			return( 0 );
		}
		if( libfwevt_xml_tag_get_value(
		     provider_name_xml_tag,
		     &( record_values->provider_name_value ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve provider name XML element value.",
			 function );

			return( -1 );
		}
	}
	if( libfvalue_value_copy_to_utf8_string(
	     record_values->provider_name_value,
	     0,
	     utf8_string,
	     utf8_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy provider name to UTF-8 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of the UTF-16 encoded source name
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libevtx_record_values_get_utf16_source_name_size(
     libevtx_record_values_t *record_values,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	libfwevt_xml_tag_t *provider_name_xml_tag = NULL;
	libfwevt_xml_tag_t *root_xml_tag          = NULL;
	libfwevt_xml_tag_t *system_xml_tag        = NULL;
	static char *function                     = "libevtx_record_values_get_utf16_source_name_size";
	int result                                = 0;

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
	if( record_values->xml_document == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid record values - missing XML document.",
		 function );

		return( -1 );
	}
	if( record_values->provider_xml_tag == NULL )
	{
		if( libfwevt_xml_document_get_root_xml_tag(
		     record_values->xml_document,
		     &root_xml_tag,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve root XML element.",
			 function );

			return( -1 );
		}
		result = libfwevt_xml_tag_get_element_by_utf8_name(
		          root_xml_tag,
		          (uint8_t *) "System",
		          6,
		          &system_xml_tag,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve System XML element.",
			 function );

			return( -1 );
		}
		else if( result == 0 )
		{
			return( 0 );
		}
		result = libfwevt_xml_tag_get_element_by_utf8_name(
		          system_xml_tag,
		          (uint8_t *) "Provider",
		          8,
		          &( record_values->provider_xml_tag ),
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve Provider XML element.",
			 function );

			return( -1 );
		}
		else if( result == 0 )
		{
			return( 0 );
		}
	}
	if( record_values->provider_name_value == NULL )
	{
		result = libfwevt_xml_tag_get_attribute_by_utf8_name(
		          record_values->provider_xml_tag,
		          (uint8_t *) "EventSourceName",
		          15,
		          &provider_name_xml_tag,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve EventSourceName XML attribute.",
			 function );

			return( -1 );
		}
		else if( result == 0 )
		{
			result = libfwevt_xml_tag_get_attribute_by_utf8_name(
				  record_values->provider_xml_tag,
				  (uint8_t *) "Name",
				  4,
				  &provider_name_xml_tag,
				  error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve Name XML attribute.",
				 function );

				return( -1 );
			}
		}
		if( result == 0 )
		{
			return( 0 );
		}
		if( libfwevt_xml_tag_get_value(
		     provider_name_xml_tag,
		     &( record_values->provider_name_value ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve provider name XML element value.",
			 function );

			return( -1 );
		}
	}
	if( libfvalue_value_get_utf16_string_size(
	     record_values->provider_name_value,
	     0,
	     utf16_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-16 string size of provider name.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-16 encoded source name
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libevtx_record_values_get_utf16_source_name(
     libevtx_record_values_t *record_values,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	libfwevt_xml_tag_t *provider_name_xml_tag = NULL;
	libfwevt_xml_tag_t *root_xml_tag          = NULL;
	libfwevt_xml_tag_t *system_xml_tag        = NULL;
	static char *function                     = "libevtx_record_values_get_utf16_source_name";
	int result                                = 0;

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
	if( record_values->xml_document == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid record values - missing XML document.",
		 function );

		return( -1 );
	}
	if( record_values->provider_xml_tag == NULL )
	{
		if( libfwevt_xml_document_get_root_xml_tag(
		     record_values->xml_document,
		     &root_xml_tag,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve root XML element.",
			 function );

			return( -1 );
		}
		result = libfwevt_xml_tag_get_element_by_utf8_name(
		          root_xml_tag,
		          (uint8_t *) "System",
		          6,
		          &system_xml_tag,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve System XML element.",
			 function );

			return( -1 );
		}
		else if( result == 0 )
		{
			return( 0 );
		}
		result = libfwevt_xml_tag_get_element_by_utf8_name(
		          system_xml_tag,
		          (uint8_t *) "Provider",
		          8,
		          &( record_values->provider_xml_tag ),
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve Provider XML element.",
			 function );

			return( -1 );
		}
		else if( result == 0 )
		{
			return( 0 );
		}
	}
	if( record_values->provider_name_value == NULL )
	{
		result = libfwevt_xml_tag_get_attribute_by_utf8_name(
		          record_values->provider_xml_tag,
		          (uint8_t *) "EventSourceName",
		          15,
		          &provider_name_xml_tag,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve EventSourceName XML attribute.",
			 function );

			return( -1 );
		}
		else if( result == 0 )
		{
			result = libfwevt_xml_tag_get_attribute_by_utf8_name(
				  record_values->provider_xml_tag,
				  (uint8_t *) "Name",
				  4,
				  &provider_name_xml_tag,
				  error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve Name XML attribute.",
				 function );

				return( -1 );
			}
		}
		if( result == 0 )
		{
			return( 0 );
		}
		if( libfwevt_xml_tag_get_value(
		     provider_name_xml_tag,
		     &( record_values->provider_name_value ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve provider name XML element value.",
			 function );

			return( -1 );
		}
	}
	if( libfvalue_value_copy_to_utf16_string(
	     record_values->provider_name_value,
	     0,
	     utf16_string,
	     utf16_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy provider name to UTF-16 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of the UTF-8 encoded computer name
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libevtx_record_values_get_utf8_computer_name_size(
     libevtx_record_values_t *record_values,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	libfwevt_xml_tag_t *computer_xml_tag = NULL;
	libfwevt_xml_tag_t *root_xml_tag     = NULL;
	libfwevt_xml_tag_t *system_xml_tag   = NULL;
	static char *function                = "libevtx_record_values_get_utf8_computer_name_size";
	int result                           = 0;

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
	if( record_values->xml_document == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid record values - missing XML document.",
		 function );

		return( -1 );
	}
	if( record_values->computer_value == NULL )
	{
		if( libfwevt_xml_document_get_root_xml_tag(
		     record_values->xml_document,
		     &root_xml_tag,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve root XML element.",
			 function );

			return( -1 );
		}
		result = libfwevt_xml_tag_get_element_by_utf8_name(
		          root_xml_tag,
		          (uint8_t *) "System",
		          6,
		          &system_xml_tag,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve System XML element.",
			 function );

			return( -1 );
		}
		else if( result == 0 )
		{
			return( 0 );
		}
		result = libfwevt_xml_tag_get_element_by_utf8_name(
		          system_xml_tag,
		          (uint8_t *) "Computer",
		          8,
		          &computer_xml_tag,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve Computer XML element.",
			 function );

			return( -1 );
		}
		else if( result == 0 )
		{
			return( 0 );
		}
		if( libfwevt_xml_tag_get_value(
		     computer_xml_tag,
		     &( record_values->computer_value ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve computer XML element value.",
			 function );

			return( -1 );
		}
	}
	if( libfvalue_value_get_utf8_string_size(
	     record_values->computer_value,
	     0,
	     utf8_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 string size of computer name.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-8 encoded computer name
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libevtx_record_values_get_utf8_computer_name(
     libevtx_record_values_t *record_values,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	libfwevt_xml_tag_t *computer_xml_tag = NULL;
	libfwevt_xml_tag_t *root_xml_tag     = NULL;
	libfwevt_xml_tag_t *system_xml_tag   = NULL;
	static char *function                = "libevtx_record_values_get_utf8_computer_name";
	int result                           = 0;

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
	if( record_values->xml_document == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid record values - missing XML document.",
		 function );

		return( -1 );
	}
	if( record_values->computer_value == NULL )
	{
		if( libfwevt_xml_document_get_root_xml_tag(
		     record_values->xml_document,
		     &root_xml_tag,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve root XML element.",
			 function );

			return( -1 );
		}
		result = libfwevt_xml_tag_get_element_by_utf8_name(
		          root_xml_tag,
		          (uint8_t *) "System",
		          6,
		          &system_xml_tag,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve System XML element.",
			 function );

			return( -1 );
		}
		else if( result == 0 )
		{
			return( 0 );
		}
		result = libfwevt_xml_tag_get_element_by_utf8_name(
		          system_xml_tag,
		          (uint8_t *) "Computer",
		          8,
		          &computer_xml_tag,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve Computer XML element.",
			 function );

			return( -1 );
		}
		else if( result == 0 )
		{
			return( 0 );
		}
		if( libfwevt_xml_tag_get_value(
		     computer_xml_tag,
		     &( record_values->computer_value ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve computer XML element value.",
			 function );

			return( -1 );
		}
	}
	if( libfvalue_value_copy_to_utf8_string(
	     record_values->computer_value,
	     0,
	     utf8_string,
	     utf8_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy computer name to UTF-8 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of the UTF-16 encoded computer name
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libevtx_record_values_get_utf16_computer_name_size(
     libevtx_record_values_t *record_values,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	libfwevt_xml_tag_t *computer_xml_tag = NULL;
	libfwevt_xml_tag_t *root_xml_tag     = NULL;
	libfwevt_xml_tag_t *system_xml_tag   = NULL;
	static char *function                = "libevtx_record_values_get_utf16_computer_name_size";
	int result                           = 0;

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
	if( record_values->xml_document == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid record values - missing XML document.",
		 function );

		return( -1 );
	}
	if( record_values->computer_value == NULL )
	{
		if( libfwevt_xml_document_get_root_xml_tag(
		     record_values->xml_document,
		     &root_xml_tag,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve root XML element.",
			 function );

			return( -1 );
		}
		result = libfwevt_xml_tag_get_element_by_utf8_name(
		          root_xml_tag,
		          (uint8_t *) "System",
		          6,
		          &system_xml_tag,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve System XML element.",
			 function );

			return( -1 );
		}
		else if( result == 0 )
		{
			return( 0 );
		}
		result = libfwevt_xml_tag_get_element_by_utf8_name(
		          system_xml_tag,
		          (uint8_t *) "Computer",
		          8,
		          &computer_xml_tag,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve Computer XML element.",
			 function );

			return( -1 );
		}
		else if( result == 0 )
		{
			return( 0 );
		}
		if( libfwevt_xml_tag_get_value(
		     computer_xml_tag,
		     &( record_values->computer_value ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve computer XML element value.",
			 function );

			return( -1 );
		}
	}
	if( libfvalue_value_get_utf16_string_size(
	     record_values->computer_value,
	     0,
	     utf16_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-16 string size of computer name.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-16 encoded computer name
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libevtx_record_values_get_utf16_computer_name(
     libevtx_record_values_t *record_values,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	libfwevt_xml_tag_t *computer_xml_tag = NULL;
	libfwevt_xml_tag_t *root_xml_tag     = NULL;
	libfwevt_xml_tag_t *system_xml_tag   = NULL;
	static char *function                = "libevtx_record_values_get_utf16_computer_name";
	int result                           = 0;

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
	if( record_values->xml_document == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid record values - missing XML document.",
		 function );

		return( -1 );
	}
	if( record_values->computer_value == NULL )
	{
		if( libfwevt_xml_document_get_root_xml_tag(
		     record_values->xml_document,
		     &root_xml_tag,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve root XML element.",
			 function );

			return( -1 );
		}
		result = libfwevt_xml_tag_get_element_by_utf8_name(
		          root_xml_tag,
		          (uint8_t *) "System",
		          6,
		          &system_xml_tag,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve System XML element.",
			 function );

			return( -1 );
		}
		else if( result == 0 )
		{
			return( 0 );
		}
		result = libfwevt_xml_tag_get_element_by_utf8_name(
		          system_xml_tag,
		          (uint8_t *) "Computer",
		          8,
		          &computer_xml_tag,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve Computer XML element.",
			 function );

			return( -1 );
		}
		else if( result == 0 )
		{
			return( 0 );
		}
		if( libfwevt_xml_tag_get_value(
		     computer_xml_tag,
		     &( record_values->computer_value ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve computer XML element value.",
			 function );

			return( -1 );
		}
	}
	if( libfvalue_value_copy_to_utf16_string(
	     record_values->computer_value,
	     0,
	     utf16_string,
	     utf16_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy computer name to UTF-16 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of the UTF-8 encoded user security identifier
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libevtx_record_values_get_utf8_user_security_identifier_size(
     libevtx_record_values_t *record_values,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	libfwevt_xml_tag_t *root_xml_tag     = NULL;
	libfwevt_xml_tag_t *security_xml_tag = NULL;
	libfwevt_xml_tag_t *system_xml_tag   = NULL;
	libfwevt_xml_tag_t *user_id_xml_tag  = NULL;
	static char *function                = "libevtx_record_values_get_utf8_user_security_identifier_size";
	int result                           = 0;

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
	if( record_values->xml_document == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid record values - missing XML document.",
		 function );

		return( -1 );
	}
	if( record_values->user_security_identifier_value == NULL )
	{
		if( libfwevt_xml_document_get_root_xml_tag(
		     record_values->xml_document,
		     &root_xml_tag,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve root XML element.",
			 function );

			return( -1 );
		}
		result = libfwevt_xml_tag_get_element_by_utf8_name(
		          root_xml_tag,
		          (uint8_t *) "System",
		          6,
		          &system_xml_tag,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve System XML element.",
			 function );

			return( -1 );
		}
		else if( result == 0 )
		{
			return( 0 );
		}
		result = libfwevt_xml_tag_get_element_by_utf8_name(
		          system_xml_tag,
		          (uint8_t *) "Security",
		          8,
		          &security_xml_tag,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve Security XML element.",
			 function );

			return( -1 );
		}
		else if( result == 0 )
		{
			return( 0 );
		}
		result = libfwevt_xml_tag_get_attribute_by_utf8_name(
		          security_xml_tag,
		          (uint8_t *) "UserID",
		          6,
		          &user_id_xml_tag,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve UserID XML attribute.",
			 function );

			return( -1 );
		}
		else if( result == 0 )
		{
			return( 0 );
		}
		if( libfwevt_xml_tag_get_value(
		     user_id_xml_tag,
		     &( record_values->user_security_identifier_value ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve user identifier XML element value.",
			 function );

			return( -1 );
		}
	}
	if( libfvalue_value_get_utf8_string_size(
	     record_values->user_security_identifier_value,
	     0,
	     utf8_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 string size of user security identifier.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-8 encoded user security identifier
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libevtx_record_values_get_utf8_user_security_identifier(
     libevtx_record_values_t *record_values,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	libfwevt_xml_tag_t *root_xml_tag     = NULL;
	libfwevt_xml_tag_t *security_xml_tag = NULL;
	libfwevt_xml_tag_t *system_xml_tag   = NULL;
	libfwevt_xml_tag_t *user_id_xml_tag  = NULL;
	static char *function                = "libevtx_record_values_get_utf8_user_security_identifier";
	int result                           = 0;

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
	if( record_values->xml_document == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid record values - missing XML document.",
		 function );

		return( -1 );
	}
	if( record_values->user_security_identifier_value == NULL )
	{
		if( libfwevt_xml_document_get_root_xml_tag(
		     record_values->xml_document,
		     &root_xml_tag,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve root XML element.",
			 function );

			return( -1 );
		}
		result = libfwevt_xml_tag_get_element_by_utf8_name(
		          root_xml_tag,
		          (uint8_t *) "System",
		          6,
		          &system_xml_tag,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve System XML element.",
			 function );

			return( -1 );
		}
		else if( result == 0 )
		{
			return( 0 );
		}
		result = libfwevt_xml_tag_get_element_by_utf8_name(
		          system_xml_tag,
		          (uint8_t *) "Security",
		          8,
		          &security_xml_tag,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve Security XML element.",
			 function );

			return( -1 );
		}
		else if( result == 0 )
		{
			return( 0 );
		}
		result = libfwevt_xml_tag_get_attribute_by_utf8_name(
		          security_xml_tag,
		          (uint8_t *) "UserID",
		          6,
		          &user_id_xml_tag,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve UserID XML attribute.",
			 function );

			return( -1 );
		}
		else if( result == 0 )
		{
			return( 0 );
		}
		if( libfwevt_xml_tag_get_value(
		     user_id_xml_tag,
		     &( record_values->user_security_identifier_value ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve user identifier XML element value.",
			 function );

			return( -1 );
		}
	}
	if( libfvalue_value_copy_to_utf8_string(
	     record_values->user_security_identifier_value,
	     0,
	     utf8_string,
	     utf8_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy user security identifier to UTF-8 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of the UTF-16 encoded user security identifier
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libevtx_record_values_get_utf16_user_security_identifier_size(
     libevtx_record_values_t *record_values,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	libfwevt_xml_tag_t *root_xml_tag     = NULL;
	libfwevt_xml_tag_t *security_xml_tag = NULL;
	libfwevt_xml_tag_t *system_xml_tag   = NULL;
	libfwevt_xml_tag_t *user_id_xml_tag  = NULL;
	static char *function                = "libevtx_record_values_get_utf16_user_security_identifier_size";
	int result                           = 0;

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
	if( record_values->xml_document == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid record values - missing XML document.",
		 function );

		return( -1 );
	}
	if( record_values->user_security_identifier_value == NULL )
	{
		if( libfwevt_xml_document_get_root_xml_tag(
		     record_values->xml_document,
		     &root_xml_tag,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve root XML element.",
			 function );

			return( -1 );
		}
		result = libfwevt_xml_tag_get_element_by_utf8_name(
		          root_xml_tag,
		          (uint8_t *) "System",
		          6,
		          &system_xml_tag,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve System XML element.",
			 function );

			return( -1 );
		}
		else if( result == 0 )
		{
			return( 0 );
		}
		result = libfwevt_xml_tag_get_element_by_utf8_name(
		          system_xml_tag,
		          (uint8_t *) "Security",
		          8,
		          &security_xml_tag,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve Security XML element.",
			 function );

			return( -1 );
		}
		else if( result == 0 )
		{
			return( 0 );
		}
		result = libfwevt_xml_tag_get_attribute_by_utf8_name(
		          security_xml_tag,
		          (uint8_t *) "UserID",
		          6,
		          &user_id_xml_tag,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve UserID XML attribute.",
			 function );

			return( -1 );
		}
		else if( result == 0 )
		{
			return( 0 );
		}
		if( libfwevt_xml_tag_get_value(
		     user_id_xml_tag,
		     &( record_values->user_security_identifier_value ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve user identifier XML element value.",
			 function );

			return( -1 );
		}
	}
	if( libfvalue_value_get_utf16_string_size(
	     record_values->user_security_identifier_value,
	     0,
	     utf16_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-16 string size of user security identifier.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-16 encoded user security identifier
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libevtx_record_values_get_utf16_user_security_identifier(
     libevtx_record_values_t *record_values,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	libfwevt_xml_tag_t *root_xml_tag     = NULL;
	libfwevt_xml_tag_t *security_xml_tag = NULL;
	libfwevt_xml_tag_t *system_xml_tag   = NULL;
	libfwevt_xml_tag_t *user_id_xml_tag  = NULL;
	static char *function                = "libevtx_record_values_get_utf16_user_security_identifier";
	int result                           = 0;

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
	if( record_values->xml_document == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid record values - missing XML document.",
		 function );

		return( -1 );
	}
	if( record_values->user_security_identifier_value == NULL )
	{
		if( libfwevt_xml_document_get_root_xml_tag(
		     record_values->xml_document,
		     &root_xml_tag,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve root XML element.",
			 function );

			return( -1 );
		}
		result = libfwevt_xml_tag_get_element_by_utf8_name(
		          root_xml_tag,
		          (uint8_t *) "System",
		          6,
		          &system_xml_tag,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve System XML element.",
			 function );

			return( -1 );
		}
		else if( result == 0 )
		{
			return( 0 );
		}
		result = libfwevt_xml_tag_get_element_by_utf8_name(
		          system_xml_tag,
		          (uint8_t *) "Security",
		          8,
		          &security_xml_tag,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve Security XML element.",
			 function );

			return( -1 );
		}
		else if( result == 0 )
		{
			return( 0 );
		}
		result = libfwevt_xml_tag_get_attribute_by_utf8_name(
		          security_xml_tag,
		          (uint8_t *) "UserID",
		          6,
		          &user_id_xml_tag,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve UserID XML attribute.",
			 function );

			return( -1 );
		}
		else if( result == 0 )
		{
			return( 0 );
		}
		if( libfwevt_xml_tag_get_value(
		     user_id_xml_tag,
		     &( record_values->user_security_identifier_value ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve user identifier XML element value.",
			 function );

			return( -1 );
		}
	}
	if( libfvalue_value_copy_to_utf16_string(
	     record_values->user_security_identifier_value,
	     0,
	     utf16_string,
	     utf16_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy user security identifier to UTF-16 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Parses a data XML tag for the record values using the template
 * Returns 1 if successful, 0 if data could not be parsed or -1 on error
 */
int libevtx_record_values_parse_data_xml_tag_by_template(
     libevtx_record_values_t *record_values,
     libfwevt_xml_tag_t *data_xml_tag,
     libfwevt_xml_tag_t *template_xml_tag,
     libcerror_error_t **error )
{
	libfwevt_xml_tag_t *sub_data_xml_tag     = NULL;
	libfwevt_xml_tag_t *sub_template_xml_tag = NULL;
	uint8_t *data_name                       = NULL;
	uint8_t *template_name                   = NULL;
	static char *function                    = "libevtx_record_values_parse_data_xml_tag_by_template";
	size_t data_name_size                    = 0;
	size_t template_name_size                = 0;
	uint8_t template_xml_tag_flags           = 0;
	int attribute_index                      = 0;
	int entry_index                          = 0;
	int sub_element_index                    = 0;
	int number_of_data_attributes            = 0;
	int number_of_data_elements              = 0;
	int number_of_template_attributes        = 0;
	int number_of_template_elements          = 0;
	int result                               = 0;

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
	if( libfwevt_xml_tag_get_number_of_attributes(
	     data_xml_tag,
	     &number_of_data_attributes,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of data attributes.",
		 function );

		goto on_error;
	}
	if( template_xml_tag != NULL )
	{
		if( libfwevt_xml_tag_get_number_of_attributes(
		     template_xml_tag,
		     &number_of_template_attributes,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of template attributes.",
			 function );

			goto on_error;
		}
		if( number_of_data_attributes != number_of_template_attributes )
		{
			return( 0 );
		}
	}
	if( libfwevt_xml_tag_get_number_of_elements(
	     data_xml_tag,
	     &number_of_data_elements,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of data elements.",
		 function );

		goto on_error;
	}
	if( template_xml_tag != NULL )
	{
		if( libfwevt_xml_tag_get_number_of_elements(
		     template_xml_tag,
		     &number_of_template_elements,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of template elements.",
			 function );

			goto on_error;
		}
		if( number_of_data_elements != number_of_template_elements )
		{
			return( 0 );
		}
	}
	if( template_xml_tag != NULL )
	{
		if( libfwevt_xml_tag_get_utf8_name_size(
		     data_xml_tag,
		     &data_name_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve data element name size.",
			 function );

			goto on_error;
		}
		if( libfwevt_xml_tag_get_utf8_name_size(
		     template_xml_tag,
		     &template_name_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve template element name size.",
			 function );

			goto on_error;
		}
		if( data_name_size != template_name_size )
		{
			return( 0 );
		}
		data_name = (uint8_t *) memory_allocate(
		                         sizeof( uint8_t ) * data_name_size );

		if( data_name == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create data name.",
			 function );

			goto on_error;
		}
		template_name = (uint8_t *) memory_allocate(
		                             sizeof( uint8_t ) * template_name_size );

		if( template_name == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create template name.",
			 function );

			goto on_error;
		}
		if( libfwevt_xml_tag_get_utf8_name(
		     data_xml_tag,
		     data_name,
		     data_name_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve data element name.",
			 function );

			goto on_error;
		}
		if( libfwevt_xml_tag_get_utf8_name(
		     template_xml_tag,
		     template_name,
		     template_name_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve template element name.",
			 function );

			goto on_error;
		}
		result = memory_compare(
			  data_name,
			  template_name,
			  sizeof( uint8_t ) * template_name_size );

		memory_free(
		 template_name );

		template_name = NULL;

		memory_free(
		 data_name );

		data_name = NULL;

		if( result != 0 )
		{
			return( 0 );
		}
	}
	for( attribute_index = 0;
	     attribute_index < number_of_data_attributes;
	     attribute_index++ )
	{
		if( libfwevt_xml_tag_get_attribute_by_index(
		     data_xml_tag,
		     attribute_index,
		     &sub_data_xml_tag,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve data attribute: %d.",
			 function,
			 attribute_index );

			goto on_error;
		}
		if( template_xml_tag != NULL )
		{
			if( libfwevt_xml_tag_get_attribute_by_index(
			     template_xml_tag,
			     attribute_index,
			     &sub_template_xml_tag,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve template attribute: %d.",
				 function,
				 attribute_index );

				goto on_error;
			}
		}
		result = libevtx_record_values_parse_data_xml_tag_by_template(
		          record_values,
		          sub_data_xml_tag,
		          sub_template_xml_tag,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to parse event data attribute: %d XML tag.",
			 function,
			 attribute_index );

			goto on_error;
		}
	}
	if( number_of_data_elements == 0 )
	{
		if( template_xml_tag != NULL )
		{
			if( libfwevt_xml_tag_get_flags(
			     template_xml_tag,
			     &template_xml_tag_flags,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve template XML tag flags.",
				 function );

				goto on_error;
			}
		}
		else
		{
			if( libfwevt_xml_tag_get_flags(
			     data_xml_tag,
			     &template_xml_tag_flags,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve element XML tag flags.",
				 function );

				goto on_error;
			}
		}
		if( template_xml_tag_flags == LIBFWEVT_XML_TAG_FLAG_IS_TEMPLATE_DEFINITION )
		{
			if( libcdata_array_append_entry(
			     record_values->string_identifiers_array,
			     &entry_index,
			     (intptr_t *) template_xml_tag,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable to append template XML tag to string identifiers array.",
				 function );

				goto on_error;
			}
			if( libcdata_array_append_entry(
			     record_values->strings_array,
			     &entry_index,
			     (intptr_t *) data_xml_tag,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable to append data XML tag to strings array.",
				 function );

				goto on_error;
			}
		}
	}
	else for( sub_element_index = 0;
	          sub_element_index < number_of_data_elements;
	          sub_element_index++ )
	{
		if( libfwevt_xml_tag_get_element_by_index(
		     data_xml_tag,
		     sub_element_index,
		     &sub_data_xml_tag,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve data sub element: %d.",
			 function,
			 sub_element_index );

			goto on_error;
		}
		if( template_xml_tag != NULL )
		{
			if( libfwevt_xml_tag_get_element_by_index(
			     template_xml_tag,
			     sub_element_index,
			     &sub_template_xml_tag,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve template sub element: %d.",
				 function,
				 sub_element_index );

				goto on_error;
			}
		}
		result = libevtx_record_values_parse_data_xml_tag_by_template(
		          record_values,
		          sub_data_xml_tag,
		          sub_template_xml_tag,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to parse event data sub element: %d XML tag.",
			 function,
			 sub_element_index );

			goto on_error;
		}
	}
	return( 1 );

on_error:
	if( template_name != NULL )
	{
		memory_free(
		 template_name );
	}
	if( data_name != NULL )
	{
		memory_free(
		 data_name );
	}
	return( -1 );
}

/* Parses the record values data
 * Returns 1 if successful, 0 if data could not be parsed or -1 on error
 */
int libevtx_record_values_parse_data(
     libevtx_record_values_t *record_values,
     libevtx_io_handle_t *io_handle,
     libevtx_internal_template_definition_t *internal_template_definition,
     libcerror_error_t **error )
{
	libfwevt_xml_tag_t *element_xml_tag       = NULL;
	libfwevt_xml_tag_t *event_data_xml_tag    = NULL;
	libfwevt_xml_tag_t *root_xml_tag          = NULL;
	libfwevt_xml_tag_t *template_root_xml_tag = NULL;
	libfwevt_xml_tag_t *user_data_xml_tag     = NULL;
	static char *function                     = "libevtx_record_values_parse_data";
	int number_of_elements                    = 0;
	int result                                = 0;

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
	if( record_values->data_parsed != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid record values - data already parsed.",
		 function );

		return( -1 );
	}
	if( record_values->string_identifiers_array != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid record values - string identifiers array value already set.",
		 function );

		return( -1 );
	}
	if( record_values->strings_array != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid record values - strings array value already set.",
		 function );

		return( -1 );
	}
	if( libcdata_array_initialize(
	     &( record_values->string_identifiers_array ),
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create string identifiers array.",
		 function );

		goto on_error;
	}
	if( libcdata_array_initialize(
	     &( record_values->strings_array ),
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create strings array.",
		 function );

		goto on_error;
	}
	if( internal_template_definition != NULL )
	{
		if( internal_template_definition->xml_document == NULL )
		{
			if( libevtx_template_definition_read(
			     internal_template_definition,
			     io_handle,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read template definition.",
				 function );

				goto on_error;
			}
		}
		if( libfwevt_xml_document_get_root_xml_tag(
		     internal_template_definition->xml_document,
		     &template_root_xml_tag,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve template root XML element.",
			 function );

			goto on_error;
		}
	}
	if( libfwevt_xml_document_get_root_xml_tag(
	     record_values->xml_document,
	     &root_xml_tag,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve root XML element.",
		 function );

		goto on_error;
	}
	result = libfwevt_xml_tag_get_element_by_utf8_name(
		  root_xml_tag,
		  (uint8_t *) "EventData",
		  9,
		  &event_data_xml_tag,
		  error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve EventData XML element.",
		 function );

		goto on_error;
	}
	else if( result == 0 )
	{
		result = libfwevt_xml_tag_get_element_by_utf8_name(
			  root_xml_tag,
			  (uint8_t *) "ProcessingErrorData",
			  19,
			  &event_data_xml_tag,
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve ProcessingErrorData XML element.",
			 function );

			goto on_error;
		}
	}
	if( result != 0 )
	{
		/* The EventData templates start with the EventData or ProcessingErrorData
		 */
		result = libevtx_record_values_parse_data_xml_tag_by_template(
			  record_values,
			  event_data_xml_tag,
			  template_root_xml_tag,
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to parse event data root element.",
			 function );

			goto on_error;
		}
	}
	else
	{
		result = libfwevt_xml_tag_get_element_by_utf8_name(
			  root_xml_tag,
			  (uint8_t *) "UserData",
			  8,
			  &user_data_xml_tag,
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve UserData XML element.",
			 function );

			goto on_error;
		}
		else if( result != 0 )
		{
			result = 0;

			/* The UserData templates start with the EventXML tag
			 */
			if( libfwevt_xml_tag_get_number_of_elements(
			     user_data_xml_tag,
			     &number_of_elements,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve number of user data elements.",
				 function );

				goto on_error;
			}
			if( number_of_elements == 1 )
			{
				if( libfwevt_xml_tag_get_element_by_index(
				     user_data_xml_tag,
				     0,
				     &element_xml_tag,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve user data element: 0.",
					 function );

					goto on_error;
				}
				result = libevtx_record_values_parse_data_xml_tag_by_template(
					  record_values,
					  element_xml_tag,
					  template_root_xml_tag,
					  error );

				if( result == -1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to parse user data element: 0.",
					 function );

					goto on_error;
				}
			}
		}
	}
	record_values->data_parsed = 1;

	return( result );

on_error:
	if( record_values->strings_array != NULL )
	{
		libcdata_array_free(
		 &( record_values->strings_array ),
		 NULL,
		 NULL );
	}
	if( record_values->string_identifiers_array != NULL )
	{
		libcdata_array_free(
		 &( record_values->string_identifiers_array ),
		 NULL,
		 NULL );
	}
	return( -1 );
}

/* Retrieves the number of strings
 * Returns 1 if successful or -1 on error
 */
int libevtx_record_values_get_number_of_strings(
     libevtx_record_values_t *record_values,
     libevtx_io_handle_t *io_handle,
     int *number_of_strings,
     libcerror_error_t **error )
{
	static char *function = "libevtx_record_values_get_number_of_strings";

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
	if( record_values->xml_document == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid record values - missing XML document.",
		 function );

		return( -1 );
	}
	if( record_values->data_parsed == 0 )
	{
		if( libevtx_record_values_parse_data(
		     record_values,
		     io_handle,
		     NULL,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to parse data.",
			 function );

			return( -1 );
		}
	}
	if( libcdata_array_get_number_of_entries(
	     record_values->strings_array,
	     number_of_strings,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
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
int libevtx_record_values_get_utf8_string_size(
     libevtx_record_values_t *record_values,
     libevtx_io_handle_t *io_handle,
     int string_index,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	libfwevt_xml_tag_t *string_xml_tag = NULL;
	static char *function              = "libevtx_record_values_get_utf8_string_size";

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
	if( record_values->xml_document == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid record values - missing XML document.",
		 function );

		return( -1 );
	}
	if( record_values->data_parsed == 0 )
	{
		if( libevtx_record_values_parse_data(
		     record_values,
		     io_handle,
		     NULL,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to parse data.",
			 function );

			return( -1 );
		}
	}
	if( libcdata_array_get_entry_by_index(
	     record_values->strings_array,
	     string_index,
	     (intptr_t **) &string_xml_tag,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve string: %d.",
		 function,
		 string_index );

		return( -1 );
	}
	if( libfwevt_xml_tag_get_utf8_value_size(
	     string_xml_tag,
	     utf8_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve string: %d value size.",
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
int libevtx_record_values_get_utf8_string(
     libevtx_record_values_t *record_values,
     libevtx_io_handle_t *io_handle,
     int string_index,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	libfwevt_xml_tag_t *string_xml_tag = NULL;
	static char *function              = "libevtx_record_values_get_utf8_string";

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
	if( record_values->xml_document == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid record values - missing XML document.",
		 function );

		return( -1 );
	}
	if( record_values->data_parsed == 0 )
	{
		if( libevtx_record_values_parse_data(
		     record_values,
		     io_handle,
		     NULL,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to parse data.",
			 function );

			return( -1 );
		}
	}
	if( libcdata_array_get_entry_by_index(
	     record_values->strings_array,
	     string_index,
	     (intptr_t **) &string_xml_tag,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve string: %d.",
		 function,
		 string_index );

		return( -1 );
	}
	if( libfwevt_xml_tag_get_utf8_value(
	     string_xml_tag,
	     utf8_string,
	     utf8_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve string: %d value.",
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
int libevtx_record_values_get_utf16_string_size(
     libevtx_record_values_t *record_values,
     libevtx_io_handle_t *io_handle,
     int string_index,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	libfwevt_xml_tag_t *string_xml_tag = NULL;
	static char *function              = "libevtx_record_values_get_utf16_string_size";

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
	if( record_values->xml_document == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid record values - missing XML document.",
		 function );

		return( -1 );
	}
	if( record_values->data_parsed == 0 )
	{
		if( libevtx_record_values_parse_data(
		     record_values,
		     io_handle,
		     NULL,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to parse data.",
			 function );

			return( -1 );
		}
	}
	if( libcdata_array_get_entry_by_index(
	     record_values->strings_array,
	     string_index,
	     (intptr_t **) &string_xml_tag,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve string: %d.",
		 function,
		 string_index );

		return( -1 );
	}
	if( libfwevt_xml_tag_get_utf16_value_size(
	     string_xml_tag,
	     utf16_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve string: %d value size.",
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
int libevtx_record_values_get_utf16_string(
     libevtx_record_values_t *record_values,
     libevtx_io_handle_t *io_handle,
     int string_index,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	libfwevt_xml_tag_t *string_xml_tag = NULL;
	static char *function              = "libevtx_record_values_get_utf16_string";

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
	if( record_values->xml_document == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid record values - missing XML document.",
		 function );

		return( -1 );
	}
	if( record_values->data_parsed == 0 )
	{
		if( libevtx_record_values_parse_data(
		     record_values,
		     io_handle,
		     NULL,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to parse data.",
			 function );

			return( -1 );
		}
	}
	if( libcdata_array_get_entry_by_index(
	     record_values->strings_array,
	     string_index,
	     (intptr_t **) &string_xml_tag,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve string: %d.",
		 function,
		 string_index );

		return( -1 );
	}
	if( libfwevt_xml_tag_get_utf16_value(
	     string_xml_tag,
	     utf16_string,
	     utf16_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve string: %d value.",
		 function,
		 string_index );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of the data
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libevtx_record_values_get_data_size(
     libevtx_record_values_t *record_values,
     libevtx_io_handle_t *io_handle,
     size_t *data_size,
     libcerror_error_t **error )
{
	libfwevt_xml_tag_t *binary_data_tag    = NULL;
	libfwevt_xml_tag_t *event_data_xml_tag = NULL;
	libfwevt_xml_tag_t *root_xml_tag       = NULL;
	static char *function                  = "libevtx_record_values_get_data_size";
	int result                             = 0;

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
	if( record_values->xml_document == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid record values - missing XML document.",
		 function );

		return( -1 );
	}
	if( record_values->data_parsed == 0 )
	{
		if( libevtx_record_values_parse_data(
		     record_values,
		     io_handle,
		     NULL,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to parse data.",
			 function );

			return( -1 );
		}
	}
	if( record_values->binary_data_value == NULL )
	{
		if( libfwevt_xml_document_get_root_xml_tag(
		     record_values->xml_document,
		     &root_xml_tag,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve root XML element.",
			 function );

			return( -1 );
		}
		result = libfwevt_xml_tag_get_element_by_utf8_name(
			  root_xml_tag,
			  (uint8_t *) "EventData",
			  9,
			  &event_data_xml_tag,
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve EventData XML element.",
			 function );

			return( -1 );
		}
		else if( result == 0 )
		{
			return( 0 );
		}
		result = libfwevt_xml_tag_get_element_by_utf8_name(
			  event_data_xml_tag,
			  (uint8_t *) "BinaryData",
			  10,
			  &binary_data_tag,
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve BinaryData XML element.",
			 function );

			return( -1 );
		}
		else if( result == 0 )
		{
			return( 0 );
		}
		if( libfwevt_xml_tag_get_value(
		     binary_data_tag,
		     &( record_values->binary_data_value ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve BinaryData XML element value.",
			 function );

			return( -1 );
		}
	}
	if( libfvalue_value_get_data_size(
	     record_values->binary_data_value,
	     data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve size of binary data.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the data
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libevtx_record_values_get_data(
     libevtx_record_values_t *record_values,
     libevtx_io_handle_t *io_handle,
     uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	libfwevt_xml_tag_t *binary_data_tag    = NULL;
	libfwevt_xml_tag_t *event_data_xml_tag = NULL;
	libfwevt_xml_tag_t *root_xml_tag       = NULL;
	static char *function                  = "libevtx_record_values_get_data";
	int result                             = 0;

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
	if( record_values->xml_document == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid record values - missing XML document.",
		 function );

		return( -1 );
	}
	if( record_values->data_parsed == 0 )
	{
		if( libevtx_record_values_parse_data(
		     record_values,
		     io_handle,
		     NULL,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to parse data.",
			 function );

			return( -1 );
		}
	}
	if( record_values->binary_data_value == NULL )
	{
		if( libfwevt_xml_document_get_root_xml_tag(
		     record_values->xml_document,
		     &root_xml_tag,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve root XML element.",
			 function );

			return( -1 );
		}
		result = libfwevt_xml_tag_get_element_by_utf8_name(
			  root_xml_tag,
			  (uint8_t *) "EventData",
			  9,
			  &event_data_xml_tag,
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve EventData XML element.",
			 function );

			return( -1 );
		}
		else if( result == 0 )
		{
			return( 0 );
		}
		result = libfwevt_xml_tag_get_element_by_utf8_name(
			  event_data_xml_tag,
			  (uint8_t *) "BinaryData",
			  10,
			  &binary_data_tag,
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve BinaryData XML element.",
			 function );

			return( -1 );
		}
		else if( result == 0 )
		{
			return( 0 );
		}
		if( libfwevt_xml_tag_get_value(
		     binary_data_tag,
		     &( record_values->binary_data_value ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve BinaryData XML element value.",
			 function );

			return( -1 );
		}
	}
	if( libfvalue_value_copy_data(
	     record_values->binary_data_value,
	     data,
	     data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy binary data.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of the UTF-8 encoded XML string
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libevtx_record_values_get_utf8_xml_string_size(
     libevtx_record_values_t *record_values,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libevtx_record_values_get_utf8_xml_string_size";

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
	if( libfwevt_xml_document_get_utf8_xml_string_size(
	     record_values->xml_document,
	     utf8_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 string size of XML document.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-8 encoded XML string
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libevtx_record_values_get_utf8_xml_string(
     libevtx_record_values_t *record_values,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libevtx_record_values_get_utf8_xml_string";

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
	if( libfwevt_xml_document_get_utf8_xml_string(
	     record_values->xml_document,
	     utf8_string,
	     utf8_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 string of XML document.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of the UTF-16 encoded XML string
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libevtx_record_values_get_utf16_xml_string_size(
     libevtx_record_values_t *record_values,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libevtx_record_values_get_utf16_xml_string_size";

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
	if( libfwevt_xml_document_get_utf16_xml_string_size(
	     record_values->xml_document,
	     utf16_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-16 string size of XML document.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-16 encoded XML string
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libevtx_record_values_get_utf16_xml_string(
     libevtx_record_values_t *record_values,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libevtx_record_values_get_utf16_xml_string";

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
	if( libfwevt_xml_document_get_utf16_xml_string(
	     record_values->xml_document,
	     utf16_string,
	     utf16_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-16 string of XML document.",
		 function );

		return( -1 );
	}
	return( 1 );
}

