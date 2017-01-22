/*
 * Export handle
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
#include <system_string.h>
#include <types.h>
#include <wide_string.h>

#include "evtxinput.h"
#include "evtxtools_libcerror.h"
#include "evtxtools_libcnotify.h"
#include "evtxtools_libclocale.h"
#include "evtxtools_libevtx.h"
#include "evtxtools_libfdatetime.h"
#include "evtxtools_libfguid.h"
#include "export_handle.h"
#include "log_handle.h"
#include "message_handle.h"
#include "message_string.h"
#include "resource_file.h"

#define EXPORT_HANDLE_NOTIFY_STREAM		stdout

const char *export_handle_get_event_log_key_name(
             int event_log_type )
{
	switch( event_log_type )
	{
		case EVTXTOOLS_EVENT_LOG_TYPE_APPLICATION:
			return( "Application" );

		case EVTXTOOLS_EVENT_LOG_TYPE_DFS_REPLICATION:
			return( "DFS Replication" );

		case EVTXTOOLS_EVENT_LOG_TYPE_HARDWARE_EVENTS:
			return( "Hardware Events" );

		case EVTXTOOLS_EVENT_LOG_TYPE_INTERNET_EXPLORER:
			return( "Internet Explorer" );

		case EVTXTOOLS_EVENT_LOG_TYPE_KEY_MANAGEMENT_SERVICE:
			return( "Key Management Service" );

		case EVTXTOOLS_EVENT_LOG_TYPE_MEDIA_CENTER:
			return( "Media Center" );

		case EVTXTOOLS_EVENT_LOG_TYPE_SECURITY:
			return( "Security" );

		case EVTXTOOLS_EVENT_LOG_TYPE_SYSTEM:
			return( "System" );

		default:
			break;
	}
	return( "(Unknown)" );
}

const char *export_handle_get_event_level(
             uint8_t event_level )
{
	switch( event_level )
	{
		case LIBEVTX_EVENT_LEVEL_CRITICAL:
			return( "Critical" );

		case LIBEVTX_EVENT_LEVEL_ERROR:
			return( "Error" );

		case LIBEVTX_EVENT_LEVEL_WARNING:
			return( "Warning" );

		case 0:
		case LIBEVTX_EVENT_LEVEL_INFORMATION:
			return( "Information" );

		case LIBEVTX_EVENT_LEVEL_VERBOSE:
			return( "Verbose" );

		default:
			break;
	}
	return( "(Unknown)" );
}

/* Creates an export handle
 * Make sure the value export_handle is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int export_handle_initialize(
     export_handle_t **export_handle,
     libcerror_error_t **error )
{
	static char *function = "export_handle_initialize";

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( *export_handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid export handle value already set.",
		 function );

		return( -1 );
	}
	*export_handle = memory_allocate_structure(
	                  export_handle_t );

	if( *export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create export handle.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *export_handle,
	     0,
	     sizeof( export_handle_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear export handle.",
		 function );

		goto on_error;
	}
	if( message_handle_initialize(
	     &( ( *export_handle )->message_handle ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create message handle.",
		 function );

		goto on_error;
	}
	if( libevtx_file_initialize(
	     &( ( *export_handle )->input_file ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize input file.",
		 function );

		goto on_error;
	}
	( *export_handle )->export_mode    = EXPORT_MODE_ITEMS;
	( *export_handle )->export_format  = EXPORT_FORMAT_TEXT;
	( *export_handle )->event_log_type = EVTXTOOLS_EVENT_LOG_TYPE_UNKNOWN;
	( *export_handle )->ascii_codepage = LIBEVTX_CODEPAGE_WINDOWS_1252;
	( *export_handle )->notify_stream  = EXPORT_HANDLE_NOTIFY_STREAM;

	return( 1 );

on_error:
	if( *export_handle != NULL )
	{
		if( ( *export_handle )->message_handle != NULL )
		{
			message_handle_free(
			 &( ( *export_handle )->message_handle ),
			 NULL );
		}
		memory_free(
		 *export_handle );

		*export_handle = NULL;
	}
	return( -1 );
}

/* Frees an export handle
 * Returns 1 if successful or -1 on error
 */
int export_handle_free(
     export_handle_t **export_handle,
     libcerror_error_t **error )
{
	static char *function = "export_handle_free";
	int result            = 1;

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( *export_handle != NULL )
	{
		if( ( *export_handle )->input_is_open != 0 )
		{
			if( export_handle_close_input(
			     *export_handle,
			     error ) != 0 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_CLOSE_FAILED,
				 "%s: unable to close export handle.",
				 function );

				result = -1;
			}
		}
		if( message_handle_free(
		     &( ( *export_handle )->message_handle ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free message handle.",
			 function );

			result = -1;
		}
		if( libevtx_file_free(
		     &( ( *export_handle )->input_file ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free input file.",
			 function );

			result = -1;
		}
		memory_free(
		 *export_handle );

		*export_handle = NULL;
	}
	return( result );
}

/* Signals the export handle to abort
 * Returns 1 if successful or -1 on error
 */
int export_handle_signal_abort(
     export_handle_t *export_handle,
     libcerror_error_t **error )
{
	static char *function = "export_handle_signal_abort";

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	export_handle->abort = 1;

	if( message_handle_signal_abort(
	     export_handle->message_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to signal message handle to abort.",
		 function );

		return( -1 );
	}
	if( export_handle->input_file != NULL )
	{
		if( libevtx_file_signal_abort(
		     export_handle->input_file,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to signal input file to abort.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Sets the export mode
 * Returns 1 if successful, 0 if unsupported values or -1 on error
 */
int export_handle_set_export_mode(
     export_handle_t *export_handle,
     const system_character_t *string,
     libcerror_error_t **error )
{
	static char *function = "export_handle_set_export_mode";
	size_t string_length  = 0;
	int result            = 0;

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid string.",
		 function );

		return( -1 );
	}
	string_length = system_string_length(
	                 string );

	if( string_length == 3 )
	{
		if( system_string_compare(
		     string,
		     _SYSTEM_STRING( "all" ),
		     3 ) == 0 )
		{
			export_handle->export_mode = EXPORT_MODE_ALL;

			result = 1;
		}
	}
	else if( string_length == 5 )
	{
		if( system_string_compare(
		     string,
		     _SYSTEM_STRING( "items" ),
		     5 ) == 0 )
		{
			export_handle->export_mode = EXPORT_MODE_ITEMS;

			result = 1;
		}
	}
	else if( string_length == 9 )
	{
		if( system_string_compare(
		     string,
		     _SYSTEM_STRING( "recovered" ),
		     9 ) == 0 )
		{
			export_handle->export_mode = EXPORT_MODE_RECOVERED;

			result = 1;
		}
	}
	return( result );
}

/* Sets the export format
 * Returns 1 if successful, 0 if unsupported values or -1 on error
 */
int export_handle_set_export_format(
     export_handle_t *export_handle,
     const system_character_t *string,
     libcerror_error_t **error )
{
	static char *function = "export_handle_export_format";
	size_t string_length  = 0;
	int result            = 0;

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	string_length = system_string_length(
	                 string );

	if( string_length == 3 )
	{
		if( system_string_compare(
		     string,
		     _SYSTEM_STRING( "xml" ),
		     3 ) == 0 )
		{
			export_handle->export_format = EXPORT_FORMAT_XML;

			result = 1;
		}
	}
	else if( string_length == 4 )
	{
		if( system_string_compare(
		     string,
		     _SYSTEM_STRING( "text" ),
		     4 ) == 0 )
		{
			export_handle->export_format = EXPORT_FORMAT_TEXT;

			result = 1;
		}
	}
	return( result );
}

/* Sets the ascii codepage
 * Returns 1 if successful or -1 on error
 */
int export_handle_set_ascii_codepage(
     export_handle_t *export_handle,
     const system_character_t *string,
     libcerror_error_t **error )
{
	static char *function  = "export_handle_set_ascii_codepage";
	size_t string_length   = 0;
	uint32_t feature_flags = 0;
	int result             = 0;

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	feature_flags = LIBCLOCALE_CODEPAGE_FEATURE_FLAG_HAVE_KOI8
	              | LIBCLOCALE_CODEPAGE_FEATURE_FLAG_HAVE_WINDOWS;

	string_length = system_string_length(
	                 string );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libclocale_codepage_copy_from_string_wide(
	          &( export_handle->ascii_codepage ),
	          string,
	          string_length,
	          feature_flags,
	          error );
#else
	result = libclocale_codepage_copy_from_string(
	          &( export_handle->ascii_codepage ),
	          string,
	          string_length,
	          feature_flags,
	          error );
#endif
	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine ASCII codepage.",
		 function );

		return( -1 );
	}
	else if( result != 0 )
	{
		if( message_handle_set_ascii_codepage(
		     export_handle->message_handle,
		     export_handle->ascii_codepage,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set ASCII codepage in message handle.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Sets the preferred language identifier
 * Returns 1 if successful or -1 on error
 */
int export_handle_set_preferred_language_identifier(
     export_handle_t *export_handle,
     uint32_t preferred_language_identifier,
     libcerror_error_t **error )
{
	static char *function = "export_handle_set_preferred_language_identifier";

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( message_handle_set_preferred_language_identifier(
	     export_handle->message_handle,
	     preferred_language_identifier,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set preferred language identifier in message handle.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Sets the event log type
 * Returns 1 if successful or -1 on error
 */
int export_handle_set_event_log_type(
     export_handle_t *export_handle,
     const system_character_t *string,
     libcerror_error_t **error )
{
	static char *function = "export_handle_set_event_log_type";
	int result            = 0;

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	result = evtxinput_determine_event_log_type(
	          string,
	          &( export_handle->event_log_type ),
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine event log type.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Sets the event log type from the filename
 * Returns 1 if successful or -1 on error
 */
int export_handle_set_event_log_type_from_filename(
     export_handle_t *export_handle,
     const system_character_t *filename,
     libcerror_error_t **error )
{
	static char *function = "export_handle_set_event_log_type_from_filename";
	int result            = 0;

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	result = evtxinput_determine_event_log_type_from_filename(
	          filename,
	          &( export_handle->event_log_type ),
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine event log type from filename.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Sets the name of the software registry file
 * Returns 1 if successful or -1 error
 */
int export_handle_set_software_registry_filename(
     export_handle_t *export_handle,
     const system_character_t *filename,
     libcerror_error_t **error )
{
	static char *function = "export_handle_set_software_registry_filename";

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( message_handle_set_software_registry_filename(
	     export_handle->message_handle,
	     filename,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set SOFTWARE registry filename in message handle.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Sets the name of the system registry file
 * Returns 1 if successful or -1 error
 */
int export_handle_set_system_registry_filename(
     export_handle_t *export_handle,
     const system_character_t *filename,
     libcerror_error_t **error )
{
	static char *function = "export_handle_set_system_registry_filename";

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( message_handle_set_system_registry_filename(
	     export_handle->message_handle,
	     filename,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set SYSTEM registry filename in message handle.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Sets the name of the directory containing the software and system registry file
 * Returns 1 if successful or -1 error
 */
int export_handle_set_registry_directory_name(
     export_handle_t *export_handle,
     const system_character_t *name,
     libcerror_error_t **error )
{
	static char *function = "export_handle_set_registry_directory_name";

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( message_handle_set_registry_directory_name(
	     export_handle->message_handle,
	     name,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set registry directory name in message handle.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Sets the path of the resource files
 * Returns 1 if successful or -1 error
 */
int export_handle_set_resource_files_path(
     export_handle_t *export_handle,
     const system_character_t *path,
     libcerror_error_t **error )
{
	static char *function = "export_handle_set_resource_files_path";

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( message_handle_set_resource_files_path(
	     export_handle->message_handle,
	     path,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set resource files path in message handle.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Opens the input
 * Returns 1 if successful or -1 on error
 */
int export_handle_open_input(
     export_handle_t *export_handle,
     const system_character_t *filename,
     libcerror_error_t **error )
{
	static char *function = "export_handle_open_input";

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( export_handle->input_is_open != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid export handle input is already open.",
		 function );

		return( -1 );
	}
	if( message_handle_open_input(
	     export_handle->message_handle,
	     export_handle_get_event_log_key_name(
	      export_handle->event_log_type ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open input of message handle.",
		 function );

		return( -1 );
	}
	if( libevtx_file_set_ascii_codepage(
	     export_handle->input_file,
	     export_handle->ascii_codepage,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set ASCII codepage in input file.",
		 function );

		return( -1 );
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	if( libevtx_file_open_wide(
	     export_handle->input_file,
	     filename,
	     LIBEVTX_OPEN_READ,
	     error ) != 1 )
#else
	if( libevtx_file_open(
	     export_handle->input_file,
	     filename,
	     LIBEVTX_OPEN_READ,
	     error ) != 1 )
#endif
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open input file.",
		 function );

		return( -1 );
	}
	export_handle->input_is_open = 1;

	return( 1 );
}

/* Closes the input
 * Returns the 0 if succesful or -1 on error
 */
int export_handle_close_input(
     export_handle_t *export_handle,
     libcerror_error_t **error )
{
	static char *function = "export_handle_close_input";
	int result            = 0;

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( export_handle->input_is_open != 0 )
	{
		if( message_handle_close_input(
		     export_handle->message_handle,
		     error ) != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_CLOSE_FAILED,
			 "%s: unable to close input of message handle.",
			 function );

			result = -1;
		}
		if( libevtx_file_close(
		     export_handle->input_file,
		     error ) != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_CLOSE_FAILED,
			 "%s: unable to close input file.",
			 function );

			result = -1;
		}
		export_handle->input_is_open = 0;
	}
	return( result );
}

/* Copies the GUID string to a byte stream
 * Returns 1 if successful or -1 on error
 */
int export_handle_guid_string_copy_to_byte_stream(
     export_handle_t *export_handle,
     const system_character_t *string,
     size_t string_length,
     uint8_t *byte_stream,
     size_t byte_stream_size,
     libcerror_error_t **error )
{
	libfguid_identifier_t *guid = NULL;
	static char *function       = "export_handle_guid_string_copy_to_byte_stream";
	int result                  = 0;

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( libfguid_identifier_initialize(
	     &guid,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create GUID.",
		 function );

		goto on_error;
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libfguid_identifier_copy_from_utf16_string(
		  guid,
		  (uint16_t *) string,
		  string_length,
		  LIBFGUID_STRING_FORMAT_FLAG_USE_MIXED_CASE | LIBFGUID_STRING_FORMAT_FLAG_USE_SURROUNDING_BRACES,
		  error );
#else
	result = libfguid_identifier_copy_from_utf8_string(
		  guid,
		  (uint8_t *) string,
		  string_length,
		  LIBFGUID_STRING_FORMAT_FLAG_USE_MIXED_CASE | LIBFGUID_STRING_FORMAT_FLAG_USE_SURROUNDING_BRACES,
		  error );
#endif
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to copy GUID from string.",
		 function );

		goto on_error;
	}
	if( libfguid_identifier_copy_to_byte_stream(
	     guid,
	     byte_stream,
	     byte_stream_size,
	     LIBFGUID_ENDIAN_LITTLE,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to copy GUID to byte stream.",
		 function );

		goto on_error;
	}
	if( libfguid_identifier_free(
	     &guid,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free GUID.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( guid != NULL )
	{
		libfguid_identifier_free(
		 &guid,
		 NULL );
	}
	return( -1 );
}

/* Retrieves the template definition from the resource file
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int export_handle_resource_file_get_template_definition(
     export_handle_t *export_handle,
     resource_file_t *resource_file,
     const uint8_t *provider_identifier,
     size_t provider_identifier_size,
     uint32_t event_identifier,
     libevtx_template_definition_t **template_definition,
     libcerror_error_t **error )
{
	libwrc_wevt_event_t *wevt_event                             = NULL;
	libwrc_wevt_provider_t *wevt_provider                       = NULL;
	libwrc_wevt_template_definition_t *wevt_template_definition = NULL;
	uint8_t *template_data                                      = NULL;
	static char *function                                       = "export_handle_resource_file_get_template_definition";
	size_t template_data_size                                   = 0;
	uint32_t template_data_offset                               = 0;
	int result                                                  = 0;

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
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
	result = resource_file_get_template_definition(
		  resource_file,
		  provider_identifier,
		  provider_identifier_size,
		  event_identifier,
		  &wevt_provider,
		  &wevt_event,
		  &wevt_template_definition,
		  error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve WEVT template definition.",
		 function );

		goto on_error;
	}
	else if( result != 0 )
	{
		if( libwrc_wevt_template_definition_get_offset(
		     wevt_template_definition,
		     &template_data_offset,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve template offset.",
			 function );

			goto on_error;
		}
		if( libwrc_wevt_template_definition_get_size(
		     wevt_template_definition,
		     &template_data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve template size.",
			 function );

			goto on_error;
		}
		if( template_data_size > 0 )
		{
			template_data = (uint8_t *) memory_allocate(
			                             sizeof( uint8_t ) * template_data_size );

			if( template_data == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
				 "%s: unable to create template data.",
				 function );

				goto on_error;
			}
			if( libwrc_wevt_template_definition_get_data(
			     wevt_template_definition,
			     template_data,
			     template_data_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve template data.",
				 function );

				goto on_error;
			}
/* TODO cache the EVTX template definitions ? */
			if( libevtx_template_definition_initialize(
			     template_definition,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create template definitions.",
				 function );

				goto on_error;
			}
			if( libevtx_template_definition_set_data(
			     *template_definition,
			     template_data,
			     template_data_size,
			     template_data_offset,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set template data.",
				 function );

				goto on_error;
			}
			memory_free(
			 template_data );

			template_data = NULL;
		}
		if( libwrc_wevt_template_definition_free(
		     &wevt_template_definition,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free template definition.",
			 function );

			goto on_error;
		}
		if( libwrc_wevt_event_free(
		     &wevt_event,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free event.",
			 function );

			goto on_error;
		}
		if( libwrc_wevt_provider_free(
		     &wevt_provider,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free provider.",
			 function );

			goto on_error;
		}
	}
	return( result );

on_error:
	if( template_data != NULL )
	{
		memory_free(
		 template_data );
	}
	if( wevt_template_definition != NULL )
	{
		libwrc_wevt_template_definition_free(
		 &wevt_template_definition,
		 NULL );
	}
	if( wevt_event != NULL )
	{
		libwrc_wevt_event_free(
		 &wevt_event,
		 NULL );
	}
	if( wevt_provider != NULL )
	{
		libwrc_wevt_provider_free(
		 &wevt_provider,
		 NULL );
	}
	if( *template_definition != NULL )
	{
		libevtx_template_definition_free(
		 template_definition,
		 NULL );
	}
	return( -1 );
}

/* Exports the record event message
 * Returns 1 if successful or -1 on error
 */
int export_handle_export_record_event_message(
     export_handle_t *export_handle,
     libevtx_record_t *record,
     const system_character_t *event_provider_identifier,
     size_t event_provider_identifier_length,
     const system_character_t *event_source,
     size_t event_source_length,
     uint32_t event_identifier,
     log_handle_t *log_handle,
     libcerror_error_t **error )
{
	uint8_t provider_identifier[ 16 ];

	libevtx_template_definition_t *template_definition = NULL;
	message_string_t *message_string                   = NULL;
	resource_file_t *resource_file                     = NULL;
	system_character_t *message_filename               = NULL;
	system_character_t *resource_filename              = NULL;
	system_character_t *value_string                   = NULL;
	static char *function                              = "export_handle_export_record_event_message";
	size_t message_filename_size                       = 0;
	size_t resource_filename_size                      = 0;
	size_t value_string_size                           = 0;
	uint32_t event_identifier_qualifiers               = 0;
	uint32_t message_identifier                        = 0;
	int number_of_strings                              = 0;
	int result                                         = 0;
	int value_string_index                             = 0;

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( event_provider_identifier != NULL )
	{
		result = message_handle_get_value_by_provider_identifier(
		          export_handle->message_handle,
		          event_provider_identifier,
		          event_provider_identifier_length,
		          _SYSTEM_STRING( "ResourceFileName" ),
		          16,
		          &resource_filename,
		          &resource_filename_size,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve resource filename by provider identifier.",
			 function );

			goto on_error;
		}
		result = message_handle_get_value_by_provider_identifier(
		          export_handle->message_handle,
		          event_provider_identifier,
		          event_provider_identifier_length,
		          _SYSTEM_STRING( "MessageFileName" ),
		          15,
		          &message_filename,
		          &message_filename_size,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve message filename by provider identifier.",
			 function );

			goto on_error;
		}
	}
	if( ( message_filename == NULL )
	 && ( event_source != NULL ) )
	{
		result = message_handle_get_value_by_event_source(
		          export_handle->message_handle,
		          event_source,
		          event_source_length,
		          _SYSTEM_STRING( "EventMessageFile" ),
		          16,
		          &message_filename,
		          &message_filename_size,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve message filename by event source.",
			 function );

			goto on_error;
		}
	}
	if( resource_filename != NULL )
	{
		fprintf(
		 export_handle->notify_stream,
		 "Resource filename\t\t: %" PRIs_SYSTEM "\n",
		 resource_filename );
	}
	if( resource_filename != NULL )
	{
		if( export_handle_guid_string_copy_to_byte_stream(
		     export_handle,
		     event_provider_identifier,
		     event_provider_identifier_length,
		     provider_identifier,
		     16,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to copy GUID from string.",
			 function );

			goto on_error;
		}
		result = message_handle_get_resource_file_by_provider_identifier(
			  export_handle->message_handle,
			  resource_filename,
			  resource_filename_size - 1,
			  provider_identifier,
			  16,
			  &resource_file,
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve resource file.",
			 function );

			goto on_error;
		}
		else if( result != 0 )
		{
			result = resource_file_get_event_message_identifier(
				  resource_file,
				  provider_identifier,
				  16,
				  event_identifier,
				  &message_identifier,
				  error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve message identifier.",
				 function );

				goto on_error;
			}
			else if( result == 0 )
			{
				message_identifier = 0;
			}
			if( export_handle->use_template_definition != 0 )
			{
				result = export_handle_resource_file_get_template_definition(
					  export_handle,
					  resource_file,
					  provider_identifier,
					  16,
					  event_identifier,
					  &template_definition,
					  error );

				if( result == -1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve tempate definition.",
					 function );

					goto on_error;
				}
			}
		}
		memory_free(
		 resource_filename );

		resource_filename = NULL;
	}
	if( message_filename != NULL )
	{
		fprintf(
		 export_handle->notify_stream,
		 "Message filename\t\t: %" PRIs_SYSTEM "\n",
		 message_filename );

		if( message_identifier == 0 )
		{
			result = libevtx_record_get_event_identifier_qualifiers(
				  record,
				  &event_identifier_qualifiers,
				  error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve event identifier qualifiers.",
				 function );

				goto on_error;
			}
			else if( result != 0 )
			{
#if defined( HAVE_DEBUG_OUTPUT )
				fprintf(
				 export_handle->notify_stream,
				 "Event identifier qualifiers\t: 0x%08" PRIx32 "\n",
				 event_identifier_qualifiers );
#endif
				message_identifier = event_identifier_qualifiers << 16;
			}
			message_identifier |= event_identifier;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		fprintf(
		 export_handle->notify_stream,
		 "Message identifier\t\t: 0x%08" PRIx32 "\n",
		 message_identifier );
#endif
		result = message_handle_get_message_string(
			  export_handle->message_handle,
			  message_filename,
			  message_filename_size - 1,
			  message_identifier,
			  &message_string,
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve message string.",
			 function );

			goto on_error;
		}
		memory_free(
		 message_filename );

		message_filename = NULL;
	}
	if( export_handle->use_template_definition != 0 )
	{
		result = libevtx_record_parse_data_with_template_definition(
			  record,
			  template_definition,
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to parse record data.",
			 function );

#if defined( HAVE_DEBUG_OUTPUT )
			if( ( error != NULL )
			 && ( *error != NULL ) )
			{
				libcnotify_print_error_backtrace(
				 *error );
			}
#endif
			libcerror_error_free(
			 error );
		}
		if( libevtx_template_definition_free(
		     &template_definition,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free template definition.",
			 function );

			goto on_error;
		}
	}
	if( libevtx_record_get_number_of_strings(
	     record,
	     &number_of_strings,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of strings in record.",
		 function );

		goto on_error;
	}
	fprintf(
	 export_handle->notify_stream,
	 "Number of strings\t\t: %d\n",
	 number_of_strings );

	for( value_string_index = 0;
	     value_string_index < number_of_strings;
	     value_string_index++ )
	{
		fprintf(
		 export_handle->notify_stream,
		 "String: %d\t\t\t: ",
		 value_string_index + 1 );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libevtx_record_get_utf16_string_size(
			  record,
			  value_string_index,
			  &value_string_size,
			  error );
#else
		result = libevtx_record_get_utf8_string_size(
			  record,
			  value_string_index,
			  &value_string_size,
			  error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve string: %d size.",
			 function,
			 value_string_index );

			goto on_error;
		}
		if( value_string_size > 0 )
		{
			value_string = system_string_allocate(
					value_string_size );

			if( value_string == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
				 "%s: unable to create value string.",
				 function );

				goto on_error;
			}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
			result = libevtx_record_get_utf16_string(
				  record,
				  value_string_index,
				  (uint16_t *) value_string,
				  value_string_size,
				  error );
#else
			result = libevtx_record_get_utf8_string(
				  record,
				  value_string_index,
				  (uint8_t *) value_string,
				  value_string_size,
				  error );
#endif
			if( result != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve string: %d.",
				 function,
				 value_string_index );

				goto on_error;
			}
			fprintf(
			 export_handle->notify_stream,
			 "%" PRIs_SYSTEM "",
			 value_string );

			memory_free(
			 value_string );

			value_string = NULL;
		}
		fprintf(
		 export_handle->notify_stream,
		 "\n" );
	}
	if( message_string != NULL )
	{
		if( message_string_fprint(
		     message_string,
		     record,
		     export_handle->notify_stream,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print message string.",
			 function );

			goto on_error;
		}
		message_string = NULL;
	}
	return( 1 );

on_error:
	if( value_string != NULL )
	{
		memory_free(
		 value_string );
	}
	if( template_definition != NULL )
	{
		libevtx_template_definition_free(
		 &template_definition,
		 NULL );
	}
	if( message_filename != NULL )
	{
		memory_free(
		 message_filename );
	}
	if( resource_filename != NULL )
	{
		memory_free(
		 resource_filename );
	}
	return( -1 );
}

/* Exports the record
 * Returns 1 if successful or -1 on error
 */
int export_handle_export_record(
     export_handle_t *export_handle,
     libevtx_record_t *record,
     log_handle_t *log_handle,
     libcerror_error_t **error )
{
	static char *function = "export_handle_export_record";

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( export_handle->export_format == EXPORT_FORMAT_TEXT )
	{
		if( export_handle_export_record_text(
		     export_handle,
		     record,
		     log_handle,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to export record in text.",
			 function );

			return( -1 );
		}
	}
	else if( export_handle->export_format == EXPORT_FORMAT_XML )
	{
		if( export_handle_export_record_xml(
		     export_handle,
		     record,
		     log_handle,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to export record in XML.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Exports the record in the text format
 * Returns 1 if successful or -1 on error
 */
int export_handle_export_record_text(
     export_handle_t *export_handle,
     libevtx_record_t *record,
     log_handle_t *log_handle,
     libcerror_error_t **error )
{
	system_character_t filetime_string[ 48 ];

	libfdatetime_filetime_t *filetime       = NULL;
	system_character_t *source_name         = NULL;
	system_character_t *provider_identifier = NULL;
	system_character_t *value_string        = NULL;
	static char *function                   = "export_handle_export_record_text";
	size_t source_name_size                 = 0;
	size_t provider_identifier_size         = 0;
	size_t value_string_size                = 0;
	uint64_t value_64bit                    = 0;
	uint32_t event_identifier               = 0;
	uint8_t event_level                     = 0;
	int result                              = 0;

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
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
	if( libfdatetime_filetime_initialize(
	     &filetime,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create filetime.",
		 function );

		goto on_error;
	}
	if( libevtx_record_get_identifier(
	     record,
	     &value_64bit,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve identifier.",
		 function );

		goto on_error;
	}
	fprintf(
	 export_handle->notify_stream,
	 "Event number\t\t\t: %" PRIu64 "\n",
	 value_64bit );

	if( libevtx_record_get_written_time(
	     record,
	     &value_64bit,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve written time.",
		 function );

		goto on_error;
	}
	if( libfdatetime_filetime_copy_from_64bit(
	     filetime,
	     value_64bit,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to copy filetime from 64-bit.",
		 function );

		goto on_error;
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libfdatetime_filetime_copy_to_utf16_string(
		  filetime,
		  (uint16_t *) filetime_string,
		  48,
		  LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME_NANO_SECONDS,
		  error );
#else
	result = libfdatetime_filetime_copy_to_utf8_string(
		  filetime,
		  (uint8_t *) filetime_string,
		  48,
		  LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME_NANO_SECONDS,
		  error );
#endif
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to copy filetime to string.",
		 function );

		goto on_error;
	}
	fprintf(
	 export_handle->notify_stream,
	 "Written time\t\t\t: %" PRIs_SYSTEM " UTC\n",
	 filetime_string );

	if( libfdatetime_filetime_free(
	     &filetime,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free filetime.",
		 function );

		goto on_error;
	}
	if( libevtx_record_get_event_level(
	     record,
	     &event_level,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve event level.",
		 function );

		goto on_error;
	}
	fprintf(
	 export_handle->notify_stream,
	 "Event level\t\t\t: %s (%d)\n",
	 export_handle_get_event_level(
	  event_level ),
	 event_level );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libevtx_record_get_utf16_user_security_identifier_size(
	          record,
	          &value_string_size,
	          error );
#else
	result = libevtx_record_get_utf8_user_security_identifier_size(
	          record,
	          &value_string_size,
	          error );
#endif
	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve user security identifier size.",
		 function );

		goto on_error;
	}
	if( ( result != 0 )
	 && ( value_string_size > 0 ) )
	{
		value_string = system_string_allocate(
		                value_string_size );

		if( value_string == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create value string.",
			 function );

			goto on_error;
		}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libevtx_record_get_utf16_user_security_identifier(
		          record,
		          (uint16_t *) value_string,
		          value_string_size,
		          error );
#else
		result = libevtx_record_get_utf8_user_security_identifier(
		          record,
		          (uint8_t *) value_string,
		          value_string_size,
		          error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve user security identifier.",
			 function );

			goto on_error;
		}
		fprintf(
		 export_handle->notify_stream,
		 "User security identifier\t: %" PRIs_SYSTEM "\n",
		 value_string );

		memory_free(
		 value_string );

		value_string = NULL;
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libevtx_record_get_utf16_computer_name_size(
	          record,
	          &value_string_size,
	          error );
#else
	result = libevtx_record_get_utf8_computer_name_size(
	          record,
	          &value_string_size,
	          error );
#endif
	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve computer name size.",
		 function );

		goto on_error;
	}
	if( ( result != 0 )
	 && ( value_string_size > 0 ) )
	{
		value_string = system_string_allocate(
		                value_string_size );

		if( value_string == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create value string.",
			 function );

			goto on_error;
		}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libevtx_record_get_utf16_computer_name(
		          record,
		          (uint16_t *) value_string,
		          value_string_size,
		          error );
#else
		result = libevtx_record_get_utf8_computer_name(
		          record,
		          (uint8_t *) value_string,
		          value_string_size,
		          error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve computer name.",
			 function );

			goto on_error;
		}
		fprintf(
		 export_handle->notify_stream,
		 "Computer name\t\t\t: %" PRIs_SYSTEM "\n",
		 value_string );

		memory_free(
		 value_string );

		value_string = NULL;
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libevtx_record_get_utf16_provider_identifier_size(
	          record,
	          &provider_identifier_size,
	          error );
#else
	result = libevtx_record_get_utf8_provider_identifier_size(
	          record,
	          &provider_identifier_size,
	          error );
#endif
	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve provider identifier size.",
		 function );

		goto on_error;
	}
	if( ( result != 0 )
	 && ( provider_identifier_size > 0 ) )
	{
		provider_identifier = system_string_allocate(
		                       provider_identifier_size );

		if( provider_identifier == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create event source.",
			 function );

			goto on_error;
		}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libevtx_record_get_utf16_provider_identifier(
		          record,
		          (uint16_t *) provider_identifier,
		          provider_identifier_size,
		          error );
#else
		result = libevtx_record_get_utf8_provider_identifier(
		          record,
		          (uint8_t *) provider_identifier,
		          provider_identifier_size,
		          error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve provider identifier.",
			 function );

			goto on_error;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		fprintf(
		 export_handle->notify_stream,
		 "Provider identifier\t\t: %" PRIs_SYSTEM "\n",
		 provider_identifier );
#endif
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libevtx_record_get_utf16_source_name_size(
	          record,
	          &source_name_size,
	          error );
#else
	result = libevtx_record_get_utf8_source_name_size(
	          record,
	          &source_name_size,
	          error );
#endif
	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve source name size.",
		 function );

		goto on_error;
	}
	if( ( result != 0 )
	 && ( source_name_size > 0 ) )
	{
		source_name = system_string_allocate(
		               source_name_size );

		if( source_name == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create event source.",
			 function );

			goto on_error;
		}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libevtx_record_get_utf16_source_name(
		          record,
		          (uint16_t *) source_name,
		          source_name_size,
		          error );
#else
		result = libevtx_record_get_utf8_source_name(
		          record,
		          (uint8_t *) source_name,
		          source_name_size,
		          error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve source name.",
			 function );

			goto on_error;
		}
		fprintf(
		 export_handle->notify_stream,
		 "Source name\t\t\t: %" PRIs_SYSTEM "\n",
		 source_name );
	}
/* TODO category ? */

	if( libevtx_record_get_event_identifier(
	     record,
	     &event_identifier,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve event identifier.",
		 function );

		goto on_error;
	}
	fprintf(
	 export_handle->notify_stream,
	 "Event identifier\t\t: 0x%08" PRIx32 " (%" PRIu32 ")\n",
	 event_identifier,
	 event_identifier );

	if( export_handle_export_record_event_message(
	     export_handle,
	     record,
	     provider_identifier,
	     provider_identifier_size - 1,
	     source_name,
	     source_name_size - 1,
	     event_identifier,
	     log_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GENERIC,
		 "%s: unable to export event message.",
		 function );

		goto on_error;
	}
	fprintf(
	 export_handle->notify_stream,
	 "\n" );

	if( provider_identifier != NULL )
	{
		memory_free(
		 provider_identifier );

		provider_identifier = NULL;
	}
	if( source_name != NULL )
	{
		memory_free(
		 source_name );

		source_name = NULL;
	}
	return( 1 );

on_error:
	if( provider_identifier != NULL )
	{
		memory_free(
		 provider_identifier );
	}
	if( source_name != NULL )
	{
		memory_free(
		 source_name );
	}
	if( value_string != NULL )
	{
		memory_free(
		 value_string );
	}
	if( filetime != NULL )
	{
		libfdatetime_filetime_free(
		 &filetime,
		 NULL );
	}
	return( -1 );
}

/* Exports the record in the XML format
 * Returns 1 if successful or -1 on error
 */
int export_handle_export_record_xml(
     export_handle_t *export_handle,
     libevtx_record_t *record,
     log_handle_t *log_handle,
     libcerror_error_t **error )
{
	system_character_t *event_xml = NULL;
	static char *function         = "export_handle_export_record_xml";
	size_t event_xml_size         = 0;

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
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
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	if( libevtx_record_get_utf16_xml_string_size(
	     record,
	     &event_xml_size,
	     error ) != 1 )
#else
	if( libevtx_record_get_utf8_xml_string_size(
	     record,
	     &event_xml_size,
	     error ) != 1 )
#endif
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve event XML size.",
		 function );

		goto on_error;
	}
	if( event_xml_size > 0 )
	{
		event_xml = system_string_allocate(
		             event_xml_size );

		if( event_xml == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create event XML.",
			 function );

			goto on_error;
		}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		if( libevtx_record_get_utf16_xml_string(
		     record,
		     (uint16_t *) event_xml,
		     event_xml_size,
		     error ) != 1 )
#else
		if( libevtx_record_get_utf8_xml_string(
		     record,
		     (uint8_t *) event_xml,
		     event_xml_size,
		     error ) != 1 )
#endif
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve event XML.",
			 function );

			goto on_error;
		}
		/* Note that the event XML ends with a new line
		 */
		fprintf(
		 export_handle->notify_stream,
		 "%" PRIs_SYSTEM "",
		 event_xml );

		memory_free(
		 event_xml );

		event_xml = NULL;
	}
	fprintf(
	 export_handle->notify_stream,
	 "\n" );

	return( 1 );

on_error:
	if( event_xml != NULL )
	{
		memory_free(
		 event_xml );
	}
	return( -1 );
}

/* Exports the records
 * Returns the 1 if succesful, 0 if no records are available or -1 on error
 */
int export_handle_export_records(
     export_handle_t *export_handle,
     libevtx_file_t *file,
     log_handle_t *log_handle,
     libcerror_error_t **error )
{
	libevtx_record_t *record = NULL;
	static char *function   = "export_handle_export_records";
	int number_of_records   = 0;
	int record_index        = 0;

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
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
	if( libevtx_file_get_number_of_records(
	     file,
	     &number_of_records,
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
	if( number_of_records == 0 )
	{
		return( 0 );
	}
	for( record_index = 0;
	     record_index < number_of_records;
	     record_index++ )
	{
		if( export_handle->abort != 0 )
		{
			return( -1 );
		}
		if( libevtx_file_get_record(
		     file,
		     record_index,
		     &record,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve record: %d.",
			 function,
			 record_index );

			return( -1 );
		}
		if( export_handle_export_record(
		     export_handle,
		     record,
		     log_handle,
		     error ) != 1 )
		{
			fprintf(
			 export_handle->notify_stream,
			 "Unable to export record: %d.\n\n",
			 record_index );

			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to export record: %d.",
			 function,
			 record_index );

#if defined( HAVE_DEBUG_OUTPUT )
			if( ( error != NULL )
			 && ( *error != NULL ) )
			{
				libcnotify_print_error_backtrace(
				 *error );
			}
#endif
			libcerror_error_free(
			 error );
		}
		if( libevtx_record_free(
		     &record,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free record: %d.",
			 function,
			 record_index );

			return( -1 );
		}
	}
	return( 1 );
}

/* Exports the recovered records
 * Returns the 1 if succesful, 0 if no records are available or -1 on error
 */
int export_handle_export_recovered_records(
     export_handle_t *export_handle,
     libevtx_file_t *file,
     log_handle_t *log_handle,
     libcerror_error_t **error )
{
	libevtx_record_t *record = NULL;
	static char *function   = "export_handle_export_recovered_records";
	int number_of_records   = 0;
	int record_index        = 0;

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
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
	if( libevtx_file_get_number_of_recovered_records(
	     file,
	     &number_of_records,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of recovered records.",
		 function );

		return( -1 );
	}
	if( number_of_records == 0 )
	{
		return( 0 );
	}
	for( record_index = 0;
	     record_index < number_of_records;
	     record_index++ )
	{
		if( export_handle->abort != 0 )
		{
			return( -1 );
		}
		if( libevtx_file_get_recovered_record(
		     file,
		     record_index,
		     &record,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve recovered record: %d.",
			 function,
			 record_index );

#if defined( HAVE_DEBUG_OUTPUT )
			if( ( error != NULL )
			 && ( *error != NULL ) )
			{
				libcnotify_print_error_backtrace(
				 *error );
			}
#endif
			libcerror_error_free(
			 error );
		}
		else if( export_handle_export_record(
		          export_handle,
		          record,
		          log_handle,
		          error ) != 1 )
		{
			fprintf(
			 export_handle->notify_stream,
			 "Unable to export recovered record: %d.\n\n",
			 record_index );

			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to export recovered record: %d.",
			 function,
			 record_index );

#if defined( HAVE_DEBUG_OUTPUT )
			if( ( error != NULL )
			 && ( *error != NULL ) )
			{
				libcnotify_print_error_backtrace(
				 *error );
			}
#endif
			libcerror_error_free(
			 error );
		}
		if( libevtx_record_free(
		     &record,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free record: %d.",
			 function,
			 record_index );

			return( -1 );
		}
	}
	return( 1 );
}

/* Exports the records from the file
 * Returns the 1 if succesful, 0 if no records are available or -1 on error
 */
int export_handle_export_file(
     export_handle_t *export_handle,
     log_handle_t *log_handle,
     libcerror_error_t **error )
{
	static char *function        = "export_handle_export_file";
	int result_recovered_records = 0;
	int result_records           = 0;

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( export_handle->export_mode != EXPORT_MODE_RECOVERED )
	{
		result_records = export_handle_export_records(
				  export_handle,
				  export_handle->input_file,
				  log_handle,
				  error );

		if( result_records == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to export records.",
			 function );

			return( -1 );
		}
	}
	if( export_handle->export_mode != EXPORT_MODE_ITEMS )
	{
		result_recovered_records = export_handle_export_recovered_records(
					    export_handle,
					    export_handle->input_file,
					    log_handle,
					    error );

		if( result_recovered_records == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to export recovered records.",
			 function );

			return( -1 );
		}
	}
	if( ( result_records != 0 )
	 || ( result_recovered_records != 0 ) )
	{
		return( 1 );
	}
	return( 0 );
}

