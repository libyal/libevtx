/* 
 * Export handle
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

#include "evtxinput.h"
#include "evtxtools_libcdirectory.h"
#include "evtxtools_libcerror.h"
#include "evtxtools_libcnotify.h"
#include "evtxtools_libclocale.h"
#include "evtxtools_libcpath.h"
#include "evtxtools_libcstring.h"
#include "evtxtools_libevtx.h"
#include "evtxtools_libfcache.h"
#include "evtxtools_libfdatetime.h"
#include "evtxtools_libregf.h"
#include "export_handle.h"
#include "log_handle.h"
#include "message_file.h"

#define EXPORT_HANDLE_NOTIFY_STREAM	stdout

const char *export_handle_get_event_log_type(
             int event_log_type )
{
	switch( event_log_type )
	{
		case EVTXTOOLS_EVENT_LOG_TYPE_APPLICATION:
			return( "Application" );

		case EVTXTOOLS_EVENT_LOG_TYPE_INTERNET_EXPLORER:
			return( "Internet Explorer" );

		case EVTXTOOLS_EVENT_LOG_TYPE_SECURITY:
			return( "Security" );

		case EVTXTOOLS_EVENT_LOG_TYPE_SYSTEM:
			return( "System" );

		case EVTXTOOLS_EVENT_LOG_TYPE_WINDOWS_POWERSHELL:
			return( "Windows PowerShell" );

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

		case LIBEVTX_EVENT_LEVEL_INFORMATION:
			return( "Information" );

		case LIBEVTX_EVENT_LEVEL_VERBOSE:
			return( "Verbose" );

		default:
			break;
	}
	return( "(Unknown)" );
}

/* Initializes the export handle
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
	if( libfcache_cache_initialize(
	     &( ( *export_handle )->message_file_cache ),
	     16,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create message file cache.",
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
	( *export_handle )->export_mode                   = EXPORT_MODE_ITEMS;
	( *export_handle )->export_format                 = EXPORT_FORMAT_TEXT;
	( *export_handle )->event_log_type                = EVTXTOOLS_EVENT_LOG_TYPE_UNKNOWN;
	( *export_handle )->preferred_language_identifier = 0x00000409UL;
	( *export_handle )->ascii_codepage                = LIBEVTX_CODEPAGE_WINDOWS_1252;
	( *export_handle )->notify_stream                 = EXPORT_HANDLE_NOTIFY_STREAM;

	return( 1 );

on_error:
	if( *export_handle != NULL )
	{
		if( ( *export_handle )->message_file_cache != NULL )
		{
			libfcache_cache_free(
			 &( ( *export_handle )->message_file_cache ),
			 NULL );
		}
		memory_free(
		 *export_handle );

		*export_handle = NULL;
	}
	return( -1 );
}

/* Frees the export handle and its elements
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
		if( ( *export_handle )->system_registry_file != NULL )
		{
			if( libregf_file_free(
			     &( ( *export_handle )->system_registry_file ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free system registry file.",
				 function );

				result = -1;
			}
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
		if( libfcache_cache_free(
		     &( ( *export_handle )->message_file_cache ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free message file cache.",
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

	if( export_handle->system_registry_file != NULL )
	{
		if( libregf_file_signal_abort(
		     export_handle->system_registry_file,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to signal system registry file to abort.",
			 function );

			return( -1 );
		}
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
     const libcstring_system_character_t *string,
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
	string_length = libcstring_system_string_length(
	                 string );

	if( string_length == 3 )
	{
		if( libcstring_system_string_compare(
		     string,
		     _LIBCSTRING_SYSTEM_STRING( "all" ),
		     3 ) == 0 )
		{
			export_handle->export_mode = EXPORT_MODE_ALL;

			result = 1;
		}
	}
	else if( string_length == 5 )
	{
		if( libcstring_system_string_compare(
		     string,
		     _LIBCSTRING_SYSTEM_STRING( "items" ),
		     5 ) == 0 )
		{
			export_handle->export_mode = EXPORT_MODE_ITEMS;

			result = 1;
		}
	}
	else if( string_length == 9 )
	{
		if( libcstring_system_string_compare(
		     string,
		     _LIBCSTRING_SYSTEM_STRING( "recovered" ),
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
     const libcstring_system_character_t *string,
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
	string_length = libcstring_system_string_length(
	                 string );

	if( string_length == 3 )
	{
		if( libcstring_system_string_compare(
		     string,
		     _LIBCSTRING_SYSTEM_STRING( "xml" ),
		     3 ) == 0 )
		{
			export_handle->export_format = EXPORT_FORMAT_XML;

			result = 1;
		}
	}
	else if( string_length == 4 )
	{
		if( libcstring_system_string_compare(
		     string,
		     _LIBCSTRING_SYSTEM_STRING( "text" ),
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
     const libcstring_system_character_t *string,
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

	string_length = libcstring_system_string_length(
	                 string );

#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
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
	return( result );
}

/* Sets the event log type
 * Returns 1 if successful or -1 on error
 */
int export_handle_set_event_log_type(
     export_handle_t *export_handle,
     const libcstring_system_character_t *string,
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
     const libcstring_system_character_t *filename,
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

/* Opens the system registry file
 * Returns 1 if successful or -1 on error
 */
int export_handle_open_system_registry_file(
     export_handle_t *export_handle,
     const libcstring_system_character_t *filename,
     libcerror_error_t **error )
{
	libregf_key_t *base_key    = NULL;
	libregf_key_t *root_key    = NULL;
	libregf_key_t *sub_key     = NULL;
	libregf_value_t *value     = NULL;
	const char *sub_key_path   = NULL;
	const char *value_name     = NULL;
	static char *function      = "export_handle_open_system_registry_file";
	size_t sub_key_path_length = 0;
	size_t value_name_length   = 0;
	int number_of_sub_keys     = 0;
	int result                 = 0;

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
	if( libregf_file_initialize(
	     &( export_handle->system_registry_file ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize system registry file.",
		 function );

		goto on_error;
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	if( libregf_file_open_wide(
	     export_handle->system_registry_file,
	     filename,
	     LIBREGF_OPEN_READ,
	     error ) != 1 )
#else
	if( libregf_file_open(
	     export_handle->system_registry_file,
	     filename,
	     LIBREGF_OPEN_READ,
	     error ) != 1 )
#endif
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open system registry file.",
		 function );

		goto on_error;
	}
	if( libregf_file_get_root_key(
	     export_handle->system_registry_file,
	     &root_key,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve root key.",
		 function );

		goto on_error;
	}
	if( libregf_key_get_number_of_sub_keys(
	     root_key,
	     &number_of_sub_keys,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of sub keys.",
		 function );

		goto on_error;
	}
	if( number_of_sub_keys == 1 )
	{
		if( libregf_key_get_sub_key(
		     root_key,
		     0,
		     &base_key,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve base key.",
			 function );

			goto on_error;
		}
	}
	else if( number_of_sub_keys > 1 )
	{
		base_key = root_key;
	}
	/* Get the current control set from:
	 * SYSTEM\Select\Current
	 */
	sub_key_path = "Select";

	sub_key_path_length = libcstring_narrow_string_length(
	                       sub_key_path );

	result = libregf_key_get_sub_key_by_utf8_path(
		  base_key,
		  (uint8_t *) sub_key_path,
		  sub_key_path_length,
		  &sub_key,
		  error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve sub key: %s.",
		 function,
		 sub_key_path );

		goto on_error;
	}
	else if( result != 0 )
	{
		value_name = "Current";

		value_name_length = libcstring_narrow_string_length(
		                     value_name );

		result = libregf_key_get_value_by_utf8_name(
			  sub_key,
			  (uint8_t *) value_name,
			  value_name_length,
			  &value,
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value: %s.",
			 function,
			 value_name );

			goto on_error;
		}
		else if( result != 0 )
		{
			if( libregf_value_get_value_32bit(
			     value,
			     &( export_handle->current_control_set ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve 32-bit value: %s.",
				 function,
				 value_name );

				goto on_error;
			}
			if( libregf_value_free(
			     &value,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free value.",
				 function );

				goto on_error;
			}
		}
		if( ( export_handle->current_control_set != 1 )
		 && ( export_handle->current_control_set != 2 ) )
		{
			fprintf(
			 export_handle->notify_stream,
			 "Unsupported current control set defaulting to 1.\n" );

			export_handle->current_control_set = 1;
		}
	}
	if( libregf_key_free(
	     &sub_key,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free sub key.",
		 function );

		goto on_error;
	}
	/* Get the control set 1 event log key:
	 * SYSTEM\ControlSet001\Services\Eventlog
	 */
	sub_key_path = "ControlSet001\\Services\\Eventlog";

	sub_key_path_length = libcstring_narrow_string_length(
	                       sub_key_path );

	result = libregf_key_get_sub_key_by_utf8_path(
		  base_key,
		  (uint8_t *) sub_key_path,
		  sub_key_path_length,
		  &sub_key,
		  error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve sub key: %s.",
		 function,
		 sub_key_path );

		goto on_error;
	}
	else if( result != 0 )
	{
		sub_key_path = export_handle_get_event_log_type(
		                export_handle->event_log_type );

		sub_key_path_length = libcstring_system_string_length(
				   sub_key_path );

		result = libregf_key_get_sub_key_by_utf8_name(
			  sub_key,
			  (uint8_t *) sub_key_path,
			  sub_key_path_length,
			  &( export_handle->control_set1_key ),
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve sub key: %s.",
			 function,
			 sub_key_path );

			goto on_error;
		}
	}
	if( libregf_key_free(
	     &sub_key,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free sub key.",
		 function );

		goto on_error;
	}
	/* Get the control set 2 event log key:
	 * SYSTEM\ControlSet002\Services\Eventlog
	 */
	sub_key_path = "ControlSet002\\Services\\Eventlog";

	sub_key_path_length = libcstring_system_string_length(
	                       sub_key_path );

	result = libregf_key_get_sub_key_by_utf8_path(
		  base_key,
		  (uint8_t *) sub_key_path,
		  sub_key_path_length,
		  &sub_key,
		  error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve sub key: %s.",
		 function,
		 sub_key_path );

		goto on_error;
	}
	else if( result != 0 )
	{
		sub_key_path = export_handle_get_event_log_type(
		                export_handle->event_log_type );

		sub_key_path_length = libcstring_system_string_length(
				       sub_key_path );

		result = libregf_key_get_sub_key_by_utf8_name(
			  sub_key,
			  (uint8_t *) sub_key_path,
			  sub_key_path_length,
			  &( export_handle->control_set2_key ),
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve sub key: %s.",
			 function,
			 sub_key_path );

			goto on_error;
		}
	}
	if( libregf_key_free(
	     &sub_key,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free sub key.",
		 function );

		goto on_error;
	}
	if( base_key != root_key )
	{
		if( libregf_key_free(
		     &base_key,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free base key.",
			 function );

			goto on_error;
		}
	}
	if( libregf_key_free(
	     &root_key,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free root key.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( value != NULL )
	{
		libregf_value_free(
		 &value,
		 NULL );
	}
	if( sub_key != NULL )
	{
		libregf_key_free(
		 &sub_key,
		 NULL );
	}
	if( ( base_key != NULL )
	 && ( base_key != root_key ) )
	{
		libregf_key_free(
		 &base_key,
		 NULL );
	}
	if( root_key != NULL )
	{
		libregf_key_free(
		 &root_key,
		 NULL );
	}
	if( export_handle->control_set1_key != NULL )
	{
		libregf_key_free(
		 &( export_handle->control_set1_key ),
		 NULL );
	}
	if( export_handle->control_set2_key != NULL )
	{
		libregf_key_free(
		 &( export_handle->control_set2_key ),
		 NULL );
	}
	if( export_handle->system_registry_file != NULL )
	{
		libregf_file_free(
		 &( export_handle->system_registry_file ),
		 NULL );
	}
	return( -1 );
}

/* Opens the input
 * Returns 1 if successful or -1 on error
 */
int export_handle_open_input(
     export_handle_t *export_handle,
     const libcstring_system_character_t *filename,
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
	if( export_handle->system_registry_filename != NULL )
	{
		if( export_handle_open_system_registry_file(
		     export_handle,
		     export_handle->system_registry_filename,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_OPEN_FAILED,
			 "%s: unable to open system registry file.",
			 function );

			return( -1 );
		}
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
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
		if( export_handle->control_set1_key != NULL )
		{
			if( libregf_key_free(
			     &( export_handle->control_set1_key ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free control set 1 key.",
				 function );

				result = -1;
			}
		}
		if( export_handle->control_set2_key != NULL )
		{
			if( libregf_key_free(
			     &( export_handle->control_set2_key ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free control set 2 key.",
				 function );

				result = -1;
			}
		}
		if( export_handle->system_registry_file != NULL )
		{
			if( libregf_file_close(
			     export_handle->system_registry_file,
			     error ) != 0 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_CLOSE_FAILED,
				 "%s: unable to close system registry file.",
				 function );

				result = -1;
			}
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

/* Retrieves the message filename for a specific event source
 * The message filename is retrieved from the SYSTEM Windows Registry File if available
 * Returns 1 if successful, 0 if such event source or -1 error
 */
int export_handle_get_message_filename(
     export_handle_t *export_handle,
     const libcstring_system_character_t *event_source,
     size_t event_source_length,
     libcstring_system_character_t **message_filename,
     size_t *message_filename_size,
     libcerror_error_t **error )
{
	libregf_key_t *key       = NULL;
	libregf_value_t *value   = NULL;
	const char *value_name   = NULL;
	static char *function    = "export_handle_get_message_filename";
	size_t value_name_length = 0;
	int result               = 0;

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
	if( message_filename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid message filename.",
		 function );

		return( -1 );
	}
	if( *message_filename != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid message filename value already set.",
		 function );

		return( -1 );
	}
	if( message_filename_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid message filename size.",
		 function );

		return( -1 );
	}
	if( export_handle->control_set1_key != NULL )
	{
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
		result = libregf_key_get_sub_key_by_utf16_name(
			  export_handle->control_set1_key,
			  (uint16_t *) event_source,
			  event_source_length,
			  &key,
			  error );
#else
		result = libregf_key_get_sub_key_by_utf8_name(
			  export_handle->control_set1_key,
			  (uint8_t *) event_source,
			  event_source_length,
			  &key,
			  error );
#endif
		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve sub key: %" PRIs_LIBCSTRING_SYSTEM ".",
			 function,
			 event_source );

			goto on_error;
		}
	}
	if( result == 0 )
	{
		if( export_handle->control_set2_key != NULL )
		{
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
			result = libregf_key_get_sub_key_by_utf16_name(
				  export_handle->control_set2_key,
				  (uint16_t *) event_source,
				  event_source_length,
				  &key,
				  error );
#else
			result = libregf_key_get_sub_key_by_utf8_name(
				  export_handle->control_set2_key,
				  (uint8_t *) event_source,
				  event_source_length,
				  &key,
				  error );
#endif
			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve sub key: %" PRIs_LIBCSTRING_SYSTEM ".",
				 function,
				 event_source );

				goto on_error;
			}
		}
	}
	if( result != 0 )
	{
		value_name = "EventMessageFile";

		value_name_length = libcstring_narrow_string_length(
		                     value_name );

		result = libregf_key_get_value_by_utf8_name(
			  key,
			  (uint8_t *) value_name,
			  value_name_length,
			  &value,
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value: %s.",
			 function,
			 value_name );

			goto on_error;
		}
		else if( result != 0 )
		{
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
			result = libregf_value_get_value_utf16_string_size(
			          value,
			          message_filename_size,
			          error );
#else
			result = libregf_value_get_value_utf8_string_size(
			          value,
			          message_filename_size,
			          error );
#endif
			if( result != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve value string size.",
				 function );

				goto on_error;
			}
			*message_filename = libcstring_system_string_allocate(
					     *message_filename_size );

			if( message_filename == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
				 "%s: unable to create value string.",
				 function );

				goto on_error;
			}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
			result = libregf_value_get_value_utf16_string(
				  value,
				  (uint16_t *) *message_filename,
				  *message_filename_size,
				  error );
#else
			result = libregf_value_get_value_utf8_string(
				  value,
				  (uint8_t *) *message_filename,
				  *message_filename_size,
				  error );
#endif
			if( result != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve value string.",
				 function );

				goto on_error;
			}
			if( libregf_value_free(
			     &value,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free value.",
				 function );

				goto on_error;
			}
		}
		if( libregf_key_free(
		     &key,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free key.",
			 function );

			goto on_error;
		}
	}
	return( result );

on_error:
	if( value != NULL )
	{
		libregf_value_free(
		 &value,
		 NULL );
	}
	if( key != NULL )
	{
		libregf_key_free(
		 &key,
		 NULL );
	}
	if( *message_filename != NULL )
	{
		memory_free(
		 *message_filename );

		*message_filename = NULL;
	}
	*message_filename_size = 0;

	return( -1 );
}

/* Retrieves the path of the message file based on the message filename
 * Returns 1 if successful or -1 error
 */
int export_handle_get_message_file_path(
     export_handle_t *export_handle,
     const libcstring_system_character_t *message_filename,
     size_t message_filename_length,
     libcstring_system_character_t **message_file_path,
     size_t *message_file_path_size,
     libcerror_error_t **error )
{
	libcdirectory_directory_t *directory                           = NULL;
	libcdirectory_directory_entry_t *directory_entry               = NULL;
	libcstring_system_character_t *directory_entry_name            = NULL;
	libcstring_system_character_t *message_filename_string_segment = NULL;
	static char *function                                          = "export_handle_get_message_file_path";
	size_t directory_entry_name_length                             = 0;
	size_t message_file_path_index                                 = 0;
	size_t message_files_path_length                               = 0;
	size_t message_filename_directory_name_index                   = 0;
	size_t message_filename_string_segment_size                    = 0;
	uint8_t directory_entry_type                                   = 0;
	int message_filename_number_of_segments                        = 0;
	int message_filename_segment_index                             = 0;
	int result                                                     = 0;

#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	libcpath_wide_split_string_t *message_filename_split_string    = NULL;
#else
	libcpath_narrow_split_string_t *message_filename_split_string  = NULL;
#endif

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
	if( message_filename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid message filename.",
		 function );

		return( -1 );
	}
	if( message_filename_length == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_ZERO_OR_LESS,
		 "%s: invalid message filenmae length is zero.",
		 function );

		return( -1 );
	}
	if( message_filename_length > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid message filename length value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( message_file_path == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid message file path.",
		 function );

		return( -1 );
	}
	if( *message_file_path != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid message file path value already set.",
		 function );

		return( -1 );
	}
	if( message_file_path_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid message file path size.",
		 function );

		return( -1 );
	}
/* TODO make sure ->message_files_path is sane, check for path separator */
	message_files_path_length = libcstring_system_string_length(
	                             export_handle->message_files_path );

	if( message_filename_length > 2 )
	{
		/* Check if the message filename starts with a volume letter
		 */
		if( ( message_filename[ 1 ] == (libcstring_system_character_t) ':' )
		 && ( ( ( message_filename[ 0 ] >= (libcstring_system_character_t) 'A' )
		   &&   ( message_filename[ 0 ] <= (libcstring_system_character_t) 'Z' ) )
		  || ( ( message_filename[ 0 ] >= (libcstring_system_character_t) 'a' )
		   &&  ( message_filename[ 0 ] <= (libcstring_system_character_t) 'z' ) ) ) )
		{
			message_filename_directory_name_index = 2;

			if( ( message_filename_length >= 3 )
			 && ( message_filename[ 2 ] == (libcstring_system_character_t) '\\' ) )
			{
				message_filename_directory_name_index += 1;
			}
		}
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	if( libcpath_wide_string_split(
	     &( message_filename[ message_filename_directory_name_index ] ),
	     message_filename_length - message_filename_directory_name_index + 1,
	     (libcstring_system_character_t) '\\',
	     &message_filename_split_string,
	     error ) != 1 )
#else
	if( libcpath_narrow_string_split(
	     &( message_filename[ message_filename_directory_name_index ] ),
	     message_filename_length - message_filename_directory_name_index + 1,
	     (libcstring_system_character_t) '\\',
	     &message_filename_split_string,
	     error ) != 1 )
#endif
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to split message filename.",
		 function );

		goto on_error;
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	if( libcpath_wide_split_string_get_number_of_segments(
	     message_filename_split_string,
	     &message_filename_number_of_segments,
	     error ) != 1 )
#else
	if( libcpath_narrow_split_string_get_number_of_segments(
	     message_filename_split_string,
	     &message_filename_number_of_segments,
	     error ) != 1 )
#endif
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of message filename string segments.",
		 function );

		goto on_error;
	}
	*message_file_path_size = message_files_path_length + 1;

	for( message_filename_segment_index = 0;
	     message_filename_segment_index < message_filename_number_of_segments;
	     message_filename_segment_index++ )
	{
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
		if( libcpath_wide_split_string_get_segment_by_index(
		     message_filename_split_string,
		     message_filename_segment_index,
		     &message_filename_string_segment,
		     &message_filename_string_segment_size,
		     error ) != 1 )
#else
		if( libcpath_narrow_split_string_get_segment_by_index(
		     message_filename_split_string,
		     message_filename_segment_index,
		     &message_filename_string_segment,
		     &message_filename_string_segment_size,
		     error ) != 1 )
#endif
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve message filename string segment: %d.",
			 function,
			 message_filename_segment_index );

			goto on_error;
		}
		if( message_filename_string_segment == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing message filename string segment: %d.",
			 function,
			 message_filename_segment_index );

			goto on_error;
		}
		if( message_filename_string_segment_size == 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported empty message filename string segment: %d.",
			 function,
			 message_filename_segment_index );

			goto on_error;
		}
		else if( ( message_filename_string_segment_size == 2 )
		      && ( message_filename_string_segment[ 0 ] == (libcstring_system_character_t) '.' ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported relative path in message filename string segment: %d.",
			 function,
			 message_filename_segment_index );

			goto on_error;
		}
		else if( ( message_filename_string_segment_size == 3 )
		      && ( message_filename_string_segment[ 0 ] == (libcstring_system_character_t) '.' )
		      && ( message_filename_string_segment[ 1 ] == (libcstring_system_character_t) '.' ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported relative path in message filename string segment: %d.",
			 function,
			 message_filename_segment_index );

			goto on_error;
		}
		else if( ( message_filename_string_segment[ 0 ] == (libcstring_system_character_t) '%' )
		      && ( message_filename_string_segment[ message_filename_string_segment_size - 2 ] == (libcstring_system_character_t) '%' ) )
		{
			if( ( message_filename_string_segment_size - 1 ) == 12 )
			{
				/* Expand %SystemRoot% to WINDOWS
				 */
				if( libcstring_system_string_compare(
				     message_filename_string_segment,
				     _LIBCSTRING_SYSTEM_STRING( "%SystemRoot%" ),
				     12 ) == 0 )
				{
					message_filename_string_segment_size = 8;
				}
			}
		}
		*message_file_path_size += message_filename_string_segment_size;
	}
	message_file_path_index = 0;

	*message_file_path = libcstring_system_string_allocate(
	                      *message_file_path_size );

	if( *message_file_path == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create message file path.",
		 function );

		goto on_error;
	}
/* TODO make sure ->message_files_path is sane */
	if( libcstring_system_string_copy(
	     &( ( *message_file_path )[ message_file_path_index ] ),
	     export_handle->message_files_path,
	     message_files_path_length ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy message files path to message file path.",
		 function );

		goto on_error;
	}
	message_file_path_index += message_files_path_length;

	for( message_filename_segment_index = 0;
	     message_filename_segment_index < message_filename_number_of_segments;
	     message_filename_segment_index++ )
	{
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
		if( libcpath_wide_split_string_get_segment_by_index(
		     message_filename_split_string,
		     message_filename_segment_index,
		     &message_filename_string_segment,
		     &message_filename_string_segment_size,
		     error ) != 1 )
#else
		if( libcpath_narrow_split_string_get_segment_by_index(
		     message_filename_split_string,
		     message_filename_segment_index,
		     &message_filename_string_segment,
		     &message_filename_string_segment_size,
		     error ) != 1 )
#endif
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve message filename string segment: %d.",
			 function,
			 message_filename_segment_index );

			goto on_error;
		}
		if( message_filename_string_segment == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing message filename string segment: %d.",
			 function,
			 message_filename_segment_index );

			goto on_error;
		}
		if( ( message_filename_string_segment[ 0 ] == (libcstring_system_character_t) '%' )
		 && ( message_filename_string_segment[ message_filename_string_segment_size - 2 ] == (libcstring_system_character_t) '%' ) )
		{
			if( ( message_filename_string_segment_size - 1 ) == 12 )
			{
				/* Expand %SystemRoot% to WINDOWS
				 */
				if( libcstring_system_string_compare(
				     message_filename_string_segment,
				     _LIBCSTRING_SYSTEM_STRING( "%SystemRoot%" ),
				     12 ) == 0 )
				{
					message_filename_string_segment      = _LIBCSTRING_SYSTEM_STRING( "Windows" );
					message_filename_string_segment_size = 8;
				}
			}
		}
		/* Terminate the path string we currently have
		 */
		( *message_file_path )[ message_file_path_index ] = 0;

		if( libcdirectory_directory_initialize(
		     &directory,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create directory.",
			 function );

			goto on_error;
		}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
		result = libcdirectory_directory_open_wide(
		          directory,
		          *message_file_path,
		          error );
#else
		result = libcdirectory_directory_open(
		          directory,
		          *message_file_path,
		          error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_OPEN_FAILED,
			 "%s: unable to open directory: %" PRIs_LIBCSTRING_SYSTEM ".",
			 function,
			 message_file_path );

			goto on_error;
		}
		if( libcdirectory_directory_entry_initialize(
		     &directory_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create directory entry.",
			 function );

			goto on_error;
		}
		if( message_filename_segment_index < ( message_filename_number_of_segments - 1 ) )
		{
			directory_entry_type = LIBCDIRECTORY_ENTRY_TYPE_DIRECTORY;
		}
		else
		{
			directory_entry_type = LIBCDIRECTORY_ENTRY_TYPE_FILE;
		}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
		result = libcdirectory_directory_has_entry_wide(
			  directory,
			  directory_entry,
			  message_filename_string_segment,
			  message_filename_string_segment_size - 1,
			  directory_entry_type,
			  LIBCDIRECTORY_COMPARE_FLAG_NO_CASE,
			  error );
#else
		result = libcdirectory_directory_has_entry(
			  directory,
			  directory_entry,
			  message_filename_string_segment,
			  message_filename_string_segment_size - 1,
			  directory_entry_type,
			  LIBCDIRECTORY_COMPARE_FLAG_NO_CASE,
			  error );
#endif
		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_GENERIC,
			 "%s: unable to determine if directory has entry: %" PRIs_LIBCSTRING_SYSTEM ".",
			 function,
			 message_filename_string_segment );

			goto on_error;
		}
		else if( result != 0 )
		{
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
			result = libcdirectory_directory_entry_get_name_wide(
			          directory_entry,
			          &directory_entry_name,
			          error );
#else
			result = libcdirectory_directory_entry_get_name(
			          directory_entry,
			          &directory_entry_name,
			          error );
#endif
			if( result != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve directory entry name.",
				 function );

				goto on_error;
			}
			directory_entry_name_length = libcstring_system_string_length(
			                               directory_entry_name );

			if( directory_entry_name_length != ( message_filename_string_segment_size - 1 ) )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: directory entry name length value out of bounds.",
				 function );

				goto on_error;
			}
			if( libcstring_system_string_copy(
			     &( ( *message_file_path )[ message_file_path_index ] ),
			     directory_entry_name,
			     directory_entry_name_length ) == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set message filename string segment: %d in message file path.",
				 function,
				 message_filename_segment_index );

				goto on_error;
			}
		}
		message_file_path_index += message_filename_string_segment_size - 1;

		( *message_file_path )[ message_file_path_index ] = (libcstring_system_character_t) LIBCPATH_SEPARATOR;

		message_file_path_index += 1;

		if( libcdirectory_directory_entry_free(
		     &directory_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free directory entry.",
			 function );

			goto on_error;
		}
		if( libcdirectory_directory_close(
		     directory,
		     error ) != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_CLOSE_FAILED,
			 "%s: unable to close directory.",
			 function,
			 message_file_path );

			goto on_error;
		}
		if( libcdirectory_directory_free(
		     &directory,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free directory.",
			 function );

			goto on_error;
		}
	}
	( *message_file_path )[ message_file_path_index - 1 ] = 0;

#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	if( libcpath_wide_split_string_free(
	     &message_filename_split_string,
	     error ) != 1 )
#else
	if( libcpath_narrow_split_string_free(
	     &message_filename_split_string,
	     error ) != 1 )
#endif
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free message filename split string.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( directory_entry != NULL )
	{
		libcdirectory_directory_entry_free(
		 &directory_entry,
		 NULL );
	}
	if( directory != NULL )
	{
		libcdirectory_directory_free(
		 &directory,
		 NULL );
	}
	if( message_filename_split_string != NULL )
	{
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
		libcpath_wide_split_string_free(
		 &message_filename_split_string,
		 NULL );
#else
		libcpath_narrow_split_string_free(
		 &message_filename_split_string,
		 NULL );
#endif
	}
	if( *message_file_path != NULL )
	{
		memory_free(
		 *message_file_path );

		*message_file_path = NULL;
	}
	*message_file_path_size = 0;

	return( -1 );
}

/* Retrieves the message string from a specific message file
 * Returns 1 if successful, 0 if no such message or -1 error
 */
int export_handle_get_message_string(
     export_handle_t *export_handle,
     const libcstring_system_character_t *message_filename,
     size_t message_filename_length,
     uint32_t message_identifier,
     libcstring_system_character_t **message_string,
     size_t *message_string_size,
     libcerror_error_t **error )
{
	libcstring_system_character_t *message_file_path = NULL;
	libfcache_cache_value_t *cache_value             = NULL;
	message_file_t *message_file                     = NULL;
	static char *function                            = "export_handle_get_message_string";
	size_t message_file_path_size                    = 0;
	int cache_index                                  = 0;
	int result                                       = 0;

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
	if( message_filename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid message filename.",
		 function );

		return( -1 );
	}
	if( ( message_filename_length == 0 )
	 || ( message_filename_length > (size_t) SSIZE_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid message filename length value out of bounds.",
		 function );

		return( -1 );
	}
	if( message_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid message string.",
		 function );

		return( -1 );
	}
	if( message_string_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid message string size.",
		 function );

		return( -1 );
	}
	for( cache_index = 0;
	     cache_index < 16;
	     cache_index++ )
	{
		if( libfcache_cache_get_value_by_index(
		     export_handle->message_file_cache,
		     cache_index,
		     &cache_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve cache value: %d.",
			 function,
			 cache_index );

			return( -1 );
		}
		if( cache_value != NULL )
		{
			if( libfcache_cache_value_get_value(
			     cache_value,
			     (intptr_t **) &message_file,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve message file from cache value: %d.",
				 function,
				 cache_index );

				return( -1 );
			}
		}
		if( message_file != NULL )
		{
			if( message_filename_length != ( message_file->name_size - 1 ) )
			{
				message_file = NULL;
			}
			else if( libcstring_system_string_compare(
				  message_filename,
				  message_file->name,
				  message_filename_length ) != 0 )
			{
				message_file = NULL;
			}
		}
		if( message_file != NULL )
		{
			break;
		}
	}
	if( message_file == NULL )
	{
		if( export_handle_get_message_file_path(
		     export_handle,
		     message_filename,
		     message_filename_length,
		     &message_file_path,
		     &message_file_path_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve message file path.",
			 function );

			goto on_error;
		}
		if( message_file_initialize(
		     &message_file,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create message file.",
			 function );

			goto on_error;
		}
		if( message_file_set_name(
		     message_file,
		     message_filename,
		     message_filename_length,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set name in message file.",
			 function );

			message_file_free(
			 &message_file,
			 NULL );

			goto on_error;
		}
		if( message_file_open(
		     message_file,
		     message_file_path,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_OPEN_FAILED,
			 "%s: unable to open message file: %" PRIs_LIBCSTRING_SYSTEM ".",
			 function,
			 message_file_path );

			message_file_free(
			 &message_file,
			 NULL );

			goto on_error;
		}
		if( libfcache_cache_set_value_by_index(
		     export_handle->message_file_cache,
		     export_handle->next_message_file_cache_index,
		     export_handle->next_message_file_cache_index,
		     libfcache_date_time_get_timestamp(),
		     (intptr_t *) message_file,
		     (int (*)(intptr_t **, libcerror_error_t **)) &message_file_free,
		     LIBFDATA_LIST_ELEMENT_VALUE_FLAG_MANAGED,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set message file in cache entry: %d.",
			 function,
			 export_handle->next_message_file_cache_index );

			message_file_free(
			 &message_file,
			 NULL );

			goto on_error;
		}
		export_handle->next_message_file_cache_index++;

		if( export_handle->next_message_file_cache_index == 16 )
		{
			export_handle->next_message_file_cache_index = 0;
		}
		memory_free(
		 message_file_path );

		message_file_path = NULL;
	}
	result = message_file_get_string(
		  message_file,
		  export_handle->preferred_language_identifier,
		  message_identifier,
		  message_string,
		  message_string_size,
		  error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve message string: 0x%" PRIx32 ".",
		 function );

		goto on_error;
	}
	return( result );

on_error:
	if( message_file_path != NULL )
	{
		memory_free(
		 message_file_path );
	}
	if( message_string != NULL )
	{
		memory_free(
		 *message_string );

		*message_string = NULL;
	}
	*message_string_size = 0;

	return( -1 );
}

/* Prints the message string
 * Returns 1 if successful or -1 on error
 */
int export_handle_message_string_fprint(
     export_handle_t *export_handle,
     const libcstring_system_character_t *message_string,
     size_t message_string_length,
     libevtx_record_t *record,
     libcerror_error_t **error )
{
	libcstring_system_character_t *value_string = NULL;
	static char *function                       = "export_handle_message_string_fprint";
	size_t conversion_specifier_length          = 0;
	size_t message_string_index                 = 0;
	size_t value_string_size                    = 0;
	int number_of_strings                       = 0;
	int result                                  = 0;
	int value_string_index                      = 0;

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
	if( message_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid message string.",
		 function );

		return( -1 );
	}
	if( message_string_length > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid message string length value exceeds maximum.",
		 function );

		return( -1 );
	}
#ifdef TODO
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
#endif /* TODO */
#if defined( HAVE_DEBUG_OUTPUT )
	fprintf(
	 export_handle->notify_stream,
	 "Message format string\t: %" PRIs_LIBCSTRING_SYSTEM "\n",
	 message_string );

	fprintf(
	 export_handle->notify_stream,
	 "Number of strings\t: %d\n",
	 number_of_strings );
#endif

	fprintf(
	 export_handle->notify_stream,
	 "Message string\t\t: " );

	message_string_index = 0;

#ifdef TODO
	while( message_string_index < message_string_length )
	{
		if( ( message_string[ message_string_index ] == (libcstring_system_character_t) '%' )
		 && ( ( message_string_index + 1 ) < message_string_length ) )
		{
/* TODO add support for more conversion specifiers */
			/* Ignore %0 = end of string, %n = new line, %r = cariage return */
			if( ( message_string[ message_string_index + 1 ] == (libcstring_system_character_t) '0' )
			 || ( message_string[ message_string_index + 1 ] == (libcstring_system_character_t) 'n' )
			 || ( message_string[ message_string_index + 1 ] == (libcstring_system_character_t) 'r' ) )
			{
				message_string_index += 2;

				continue;
			}
			if( ( message_string[ message_string_index + 1 ] < (libcstring_system_character_t) '1' )
			 || ( message_string[ message_string_index + 1 ] > (libcstring_system_character_t) '9' ) )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
				 "%s: unsupported conversion specifier: %%"
				 "%" PRIc_LIBCSTRING_SYSTEM ".",
				 function,
				 message_string[ message_string_index + 1 ] );

				goto on_error;
			}
			conversion_specifier_length = 2;

		 	if( ( ( message_string_index + 4 ) < message_string_length )
			 && ( message_string[ message_string_index + 2 ] == (libcstring_system_character_t) '!' ) )
			{
				if( ( message_string[ message_string_index + 3 ] != (libcstring_system_character_t) 's' )
				 || ( message_string[ message_string_index + 4 ] != (libcstring_system_character_t) '!' ) )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
					 "%s: unsupported conversion specifier: %%"
					 "%" PRIc_LIBCSTRING_SYSTEM "%" PRIc_LIBCSTRING_SYSTEM
					 "%" PRIc_LIBCSTRING_SYSTEM "%" PRIc_LIBCSTRING_SYSTEM ".",
					 function,
					 message_string[ message_string_index + 1 ],
					 message_string[ message_string_index + 2 ],
					 message_string[ message_string_index + 3 ],
					 message_string[ message_string_index + 4 ] );

					goto on_error;
				}
				conversion_specifier_length = 5;
			}
			value_string_index = (int) message_string[ message_string_index + 1 ]
			                   - (int) '0'
			                   - 1;

#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
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
				value_string = libcstring_system_string_allocate(
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
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
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
				 "%" PRIs_LIBCSTRING_SYSTEM "",
				 value_string );

				memory_free(
				 value_string );

				value_string = NULL;
			}
			message_string_index += conversion_specifier_length;
		}
		else
		{
			if( message_string[ message_string_index ] != 0 )
			{
				fprintf(
				 export_handle->notify_stream,
				 "%" PRIc_LIBCSTRING_SYSTEM "",
				 message_string[ message_string_index ] );
			}
			message_string_index += 1;
		}
	}
#endif /* TODO */
	fprintf(
	 export_handle->notify_stream,
	 "\n" );

	return( 1 );

on_error:
	if( value_string != NULL )
	{
		memory_free(
		 value_string );
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
	libcstring_system_character_t filetime_string[ 32 ];

	libcstring_system_character_t *event_source                    = NULL;
	libcstring_system_character_t *message_filename                = NULL;
	libcstring_system_character_t *message_filename_string_segment = NULL;
	libcstring_system_character_t *message_string                  = NULL;
	libcstring_system_character_t *value_string                    = NULL;
	libfdatetime_filetime_t *filetime                              = NULL;
	static char *function                                          = "export_handle_export_record_text";
	size_t event_source_size                                       = 0;
	size_t message_filename_size                                   = 0;
	size_t message_filename_string_segment_size                    = 0;
	size_t message_string_size                                     = 0;
	size_t value_string_size                                       = 0;
	uint64_t value_64bit                                           = 0;
	uint32_t event_identifier                                      = 0;
	uint8_t event_level                                            = 0;
	int message_filename_number_of_segments                        = 0;
	int message_filename_segment_index                             = 0;
	int result                                                     = 0;

#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	libcpath_wide_split_string_t *message_filename_split_string    = NULL;
#else
	libcpath_narrow_split_string_t *message_filename_split_string  = NULL;
#endif

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
	 "Event number\t\t: %" PRIu64 "\n",
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
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
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
		 "%s: unable to copy filetime to string.",
		 function );

		goto on_error;
	}
	fprintf(
	 export_handle->notify_stream,
	 "Written time\t\t: %" PRIs_LIBCSTRING_SYSTEM " UTC\n",
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
	 "Event identifier\t: 0x%08" PRIx32 "\n",
	 event_identifier );

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
	 "Event level\t\t: %s\n",
	 export_handle_get_event_level(
	  event_level ) );

#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	result = libevtx_record_get_utf16_source_name_size(
	          record,
	          &event_source_size,
	          error );
#else
	result = libevtx_record_get_utf8_source_name_size(
	          record,
	          &event_source_size,
	          error );
#endif
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve source name size.",
		 function );

		goto on_error;
	}
	if( event_source_size > 0 )
	{
		event_source = libcstring_system_string_allocate(
		                event_source_size );

		if( event_source == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create event source.",
			 function );

			goto on_error;
		}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
		result = libevtx_record_get_utf16_source_name(
		          record,
		          (uint16_t *) event_source,
		          event_source_size,
		          error );
#else
		result = libevtx_record_get_utf8_source_name(
		          record,
		          (uint8_t *) event_source,
		          event_source_size,
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
		 "Source name\t\t: %" PRIs_LIBCSTRING_SYSTEM "\n",
		 event_source );
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
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
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve computer name size.",
		 function );

		goto on_error;
	}
	if( value_string_size > 0 )
	{
		value_string = libcstring_system_string_allocate(
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
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
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
		 "Computer name\t\t: %" PRIs_LIBCSTRING_SYSTEM "\n",
		 value_string );

		memory_free(
		 value_string );

		value_string = NULL;
	}
	if( event_source != NULL )
	{
		result = export_handle_get_message_filename(
		          export_handle,
		          event_source,
		          event_source_size - 1,
		          &message_filename,
		          &message_filename_size,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve message filename.",
			 function );

			goto on_error;
		}
		else if( result != 0 )
		{
			fprintf(
			 export_handle->notify_stream,
			 "Message filename\t: %" PRIs_LIBCSTRING_SYSTEM "\n",
			 message_filename );

			/* The message filename can contain multiple file names
			 * separated by ;
			 */
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
			if( libcpath_wide_string_split(
			     message_filename,
			     message_filename_size,
			     (libcstring_system_character_t) ';',
			     &message_filename_split_string,
			     error ) != 1 )
#else
			if( libcpath_narrow_string_split(
			     message_filename,
			     message_filename_size,
			     (libcstring_system_character_t) ';',
			     &message_filename_split_string,
			     error ) != 1 )
#endif
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to split message filename.",
				 function );

				goto on_error;
			}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
			if( libcpath_wide_split_string_get_number_of_segments(
			     message_filename_split_string,
			     &message_filename_number_of_segments,
			     error ) != 1 )
#else
			if( libcpath_narrow_split_string_get_number_of_segments(
			     message_filename_split_string,
			     &message_filename_number_of_segments,
			     error ) != 1 )
#endif
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve number of message filename string segments.",
				 function );

				goto on_error;
			}
			for( message_filename_segment_index = 0;
			     message_filename_segment_index < message_filename_number_of_segments;
			     message_filename_segment_index++ )
			{
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
				if( libcpath_wide_split_string_get_segment_by_index(
				     message_filename_split_string,
				     message_filename_segment_index,
				     &message_filename_string_segment,
				     &message_filename_string_segment_size,
				     error ) != 1 )
#else
				if( libcpath_narrow_split_string_get_segment_by_index(
				     message_filename_split_string,
				     message_filename_segment_index,
				     &message_filename_string_segment,
				     &message_filename_string_segment_size,
				     error ) != 1 )
#endif
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve message filename string segment: %d.",
					 function,
					 message_filename_segment_index );

					goto on_error;
				}
				if( message_filename_string_segment == NULL )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
					 "%s: missing message filename string segment: %d.",
					 function,
					 message_filename_segment_index );

					goto on_error;
				}
				result = export_handle_get_message_string(
					  export_handle,
					  message_filename_string_segment,
					  message_filename_string_segment_size - 1,
					  event_identifier,
					  &message_string,
					  &message_string_size,
					  error );

				if( result == -1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve message string: 0x%08" PRIx32 ".",
					 function,
					 event_identifier );

					goto on_error;
				}
				else if( result != 0 )
				{
					if( export_handle_message_string_fprint(
					     export_handle,
					     message_string,
					     message_string_size - 1,
					     record,
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
					memory_free(
					 message_string );

					message_string = NULL;

					break;
				}
			}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
			if( libcpath_wide_split_string_free(
			     &message_filename_split_string,
			     error ) != 1 )
#else
			if( libcpath_narrow_split_string_free(
			     &message_filename_split_string,
			     error ) != 1 )
#endif
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free message filename split string.",
				 function );

				goto on_error;
			}
			memory_free(
			 message_filename );

			message_filename = NULL;
		}
		memory_free(
		 event_source );

		event_source = NULL;
	}
	fprintf(
	 export_handle->notify_stream,
	 "\n" );

	return( 1 );

on_error:
	if( message_string != NULL )
	{
		memory_free(
		 message_string );
	}
	if( message_filename_split_string != NULL )
	{
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
		libcpath_wide_split_string_free(
		 &message_filename_split_string,
		 NULL );
#else
		libcpath_narrow_split_string_free(
		 &message_filename_split_string,
		 NULL );
#endif
	}
	if( message_filename != NULL )
	{
		memory_free(
		 message_filename );
	}
	if( event_source != NULL )
	{
		memory_free(
		 event_source );
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
	libcstring_system_character_t *event_xml = NULL;
	static char *function                    = "export_handle_export_record_xml";
	size_t event_xml_size                    = 0;
	int result                               = 0;

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
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
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
		event_xml = libcstring_system_string_allocate(
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
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
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
		 "%" PRIs_LIBCSTRING_SYSTEM "",
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

			return( -1 );
		}
		if( export_handle_export_record(
		     export_handle,
		     record,
		     log_handle,
		     error ) != 1 )
		{
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

