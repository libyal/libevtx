/*
 * Message handle
 *
 * Copyright (c) 2011-2012, Joachim Metz <joachim.metz@gmail.com>
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

#include "evtxtools_libcdirectory.h"
#include "evtxtools_libcerror.h"
#include "evtxtools_libcpath.h"
#include "evtxtools_libcsplit.h"
#include "evtxtools_libcstring.h"
#include "evtxtools_libevtx.h"
#include "evtxtools_libfcache.h"
#include "evtxtools_libregf.h"
#include "message_file.h"
#include "message_handle.h"
#include "path_handle.h"
#include "registry_file.h"

/* Initializes the message handle
 * Returns 1 if successful or -1 on error
 */
int message_handle_initialize(
     message_handle_t **message_handle,
     libcerror_error_t **error )
{
	static char *function = "message_handle_initialize";

	if( message_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid message handle.",
		 function );

		return( -1 );
	}
	if( *message_handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid message handle value already set.",
		 function );

		return( -1 );
	}
	*message_handle = memory_allocate_structure(
	                   message_handle_t );

	if( *message_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create message handle.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *message_handle,
	     0,
	     sizeof( message_handle_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear message handle.",
		 function );

		memory_free(
		 *message_handle );

		*message_handle = NULL;

		return( -1 );
	}
	if( path_handle_initialize(
	     &( ( *message_handle )->path_handle ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create path handle.",
		 function );

		goto on_error;
	}
	if( libfcache_cache_initialize(
	     &( ( *message_handle )->message_file_cache ),
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
	if( libfcache_cache_initialize(
	     &( ( *message_handle )->mui_message_file_cache ),
	     16,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create MUI message file cache.",
		 function );

		goto on_error;
	}
	( *message_handle )->ascii_codepage                = LIBREGF_CODEPAGE_WINDOWS_1252;
	( *message_handle )->preferred_language_identifier = 0x00000409UL;

	return( 1 );

on_error:
	if( *message_handle != NULL )
	{
		if( ( *message_handle )->message_file_cache != NULL )
		{
			libfcache_cache_free(
			 &( ( *message_handle )->message_file_cache ),
			 NULL );
		}
		if( ( *message_handle )->path_handle != NULL )
		{
			path_handle_free(
			 &( ( *message_handle )->path_handle ),
			 NULL );
		}
		memory_free(
		 *message_handle );

		*message_handle = NULL;
	}
	return( -1 );
}

/* Frees the message handle and its elements
 * Returns 1 if successful or -1 on error
 */
int message_handle_free(
     message_handle_t **message_handle,
     libcerror_error_t **error )
{
	static char *function = "message_handle_free";
	int result            = 1;

	if( message_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid message handle.",
		 function );

		return( -1 );
	}
	if( *message_handle != NULL )
	{
		if( ( *message_handle )->software_registry_filename != NULL )
		{
			memory_free(
			 ( *message_handle )->software_registry_filename );
		}
		if( ( *message_handle )->system_registry_filename != NULL )
		{
			memory_free(
			 ( *message_handle )->system_registry_filename );
		}
		if( ( *message_handle )->registry_directory_name != NULL )
		{
			memory_free(
			 ( *message_handle )->registry_directory_name );
		}
		if( ( *message_handle )->software_registry_file != NULL )
		{
			if( registry_file_free(
			     &( ( *message_handle )->software_registry_file ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free software registry file.",
				 function );

				result = -1;
			}
		}
		if( ( *message_handle )->system_root_path != NULL )
		{
			memory_free(
			 ( *message_handle )->system_root_path );
		}
		if( ( *message_handle )->windows_directory_path != NULL )
		{
			memory_free(
			 ( *message_handle )->windows_directory_path );
		}
		if( ( *message_handle )->system_registry_file != NULL )
		{
			if( registry_file_free(
			     &( ( *message_handle )->system_registry_file ),
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
		if( path_handle_free(
		     &( ( *message_handle )->path_handle ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free path handle.",
			 function );

			result = -1;
		}
		if( libfcache_cache_free(
		     &( ( *message_handle )->message_file_cache ),
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
		if( libfcache_cache_free(
		     &( ( *message_handle )->mui_message_file_cache ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free MUI message file cache.",
			 function );

			result = -1;
		}
		if( ( *message_handle )->winevt_publishers_key != NULL )
		{
			if( libregf_key_free(
			     &( ( *message_handle )->winevt_publishers_key ),
			     NULL ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free winevt publishers key.",
				 function );

				result = -1;
			}
		}
		if( ( *message_handle )->control_set_1_eventlog_services_key != NULL )
		{
			if( libregf_key_free(
			     &( ( *message_handle )->control_set_1_eventlog_services_key ),
			     NULL ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free control set 1 eventlog services key.",
				 function );

				result = -1;
			}
		}
		if( ( *message_handle )->control_set_2_eventlog_services_key != NULL )
		{
			if( libregf_key_free(
			     &( ( *message_handle )->control_set_2_eventlog_services_key ),
			     NULL ) != 1 )
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
		memory_free(
		 *message_handle );

		*message_handle = NULL;
	}
	return( result );
}

/* Signals the message handle to abort
 * Returns 1 if successful or -1 on error
 */
int message_handle_signal_abort(
     message_handle_t *message_handle,
     libcerror_error_t **error )
{
	static char *function = "message_handle_signal_abort";

	if( message_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid message handle.",
		 function );

		return( -1 );
	}
	if( message_handle->software_registry_file != NULL )
	{
		if( registry_file_signal_abort(
		     message_handle->software_registry_file,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to signal software registry file to abort.",
			 function );

			return( -1 );
		}
	}
	if( message_handle->system_registry_file != NULL )
	{
		if( registry_file_signal_abort(
		     message_handle->system_registry_file,
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
	return( 1 );
}

/* Sets the ascii codepage
 * Returns 1 if successful or -1 on error
 */
int message_handle_set_ascii_codepage(
     message_handle_t *message_handle,
     int ascii_codepage,
     libcerror_error_t **error )
{
	static char *function = "message_handle_set_ascii_codepage";

	if( message_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid message handle.",
		 function );

		return( -1 );
	}
	message_handle->ascii_codepage = ascii_codepage;

	return( 1 );
}

/* Sets the preferred language identifier
 * Returns 1 if successful or -1 on error
 */
int message_handle_set_preferred_language_identifier(
     message_handle_t *message_handle,
     uint32_t preferred_language_identifier,
     libcerror_error_t **error )
{
	static char *function = "message_handle_set_preferred_language_identifier";

	if( message_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid message handle.",
		 function );

		return( -1 );
	}
	message_handle->preferred_language_identifier = preferred_language_identifier;

	return( 1 );
}

/* Sets the name of the software registry file
 * Returns 1 if successful or -1 error
 */
int message_handle_set_software_registry_filename(
     message_handle_t *message_handle,
     const libcstring_system_character_t *filename,
     libcerror_error_t **error )
{
	static char *function  = "message_handle_set_software_registry_filename";
	size_t filename_length = 0;

	if( message_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid message handle.",
		 function );

		return( -1 );
	}
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
	filename_length = libcstring_system_string_length(
	                   filename );

	if( filename_length > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid filename length value out of bounds.",
		 function );

		goto on_error;
	}
	if( message_handle->software_registry_filename != NULL )
	{
		memory_free(
		 message_handle->software_registry_filename );

		message_handle->software_registry_filename = NULL;
	}
	message_handle->software_registry_filename_size = filename_length + 1;

	message_handle->software_registry_filename = libcstring_system_string_allocate(
	                                              message_handle->software_registry_filename_size );

	if( message_handle->software_registry_filename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create software registry filename.",
		 function );

		goto on_error;
	}
	if( libcstring_system_string_copy(
	     message_handle->software_registry_filename,
	     filename,
	     filename_length ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy software registry filename.",
		 function );

		goto on_error;
	}
	( message_handle->software_registry_filename )[ filename_length ] = 0;

	return( 1 );

on_error:
	if( message_handle->software_registry_filename != NULL )
	{
		memory_free(
		 message_handle->software_registry_filename );

		message_handle->software_registry_filename = NULL;
	}
	message_handle->software_registry_filename_size = 0;

	return( -1 );
}

/* Sets the name of the system registry file
 * Returns 1 if successful or -1 error
 */
int message_handle_set_system_registry_filename(
     message_handle_t *message_handle,
     const libcstring_system_character_t *filename,
     libcerror_error_t **error )
{
	static char *function  = "message_handle_set_system_registry_filename";
	size_t filename_length = 0;

	if( message_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid message handle.",
		 function );

		return( -1 );
	}
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
	filename_length = libcstring_system_string_length(
	                   filename );

	if( filename_length > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid filename length value out of bounds.",
		 function );

		goto on_error;
	}
	if( message_handle->system_registry_filename != NULL )
	{
		memory_free(
		 message_handle->system_registry_filename );

		message_handle->system_registry_filename = NULL;
	}
	message_handle->system_registry_filename_size = filename_length + 1;

	message_handle->system_registry_filename = libcstring_system_string_allocate(
	                                            message_handle->system_registry_filename_size );

	if( message_handle->system_registry_filename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create system registry filename.",
		 function );

		goto on_error;
	}
	if( libcstring_system_string_copy(
	     message_handle->system_registry_filename,
	     filename,
	     filename_length ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy system registry filename.",
		 function );

		goto on_error;
	}
	( message_handle->system_registry_filename )[ filename_length ] = 0;

	return( 1 );

on_error:
	if( message_handle->system_registry_filename != NULL )
	{
		memory_free(
		 message_handle->system_registry_filename );

		message_handle->system_registry_filename = NULL;
	}
	message_handle->system_registry_filename_size = 0;

	return( -1 );
}

/* Sets the name of the directory containing the software and system registry file
 * Returns 1 if successful or -1 error
 */
int message_handle_set_registry_directory_name(
     message_handle_t *message_handle,
     const libcstring_system_character_t *name,
     libcerror_error_t **error )
{
	static char *function = "message_handle_set_registry_directory_name";
	size_t name_length    = 0;

	if( message_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid message handle.",
		 function );

		return( -1 );
	}
	if( name == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid name.",
		 function );

		return( -1 );
	}
	name_length = libcstring_system_string_length(
	               name );

	if( name_length > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid name length value out of bounds.",
		 function );

		goto on_error;
	}
	if( message_handle->registry_directory_name != NULL )
	{
		memory_free(
		 message_handle->registry_directory_name );

		message_handle->registry_directory_name = NULL;
	}
	message_handle->registry_directory_name_size = name_length + 1;

	message_handle->registry_directory_name = libcstring_system_string_allocate(
	                                           message_handle->registry_directory_name_size );

	if( message_handle->registry_directory_name == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create registry directory name.",
		 function );

		goto on_error;
	}
	if( libcstring_system_string_copy(
	     message_handle->registry_directory_name,
	     name,
	     name_length ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy registry directory name.",
		 function );

		goto on_error;
	}
	( message_handle->registry_directory_name )[ name_length ] = 0;

	return( 1 );

on_error:
	if( message_handle->registry_directory_name != NULL )
	{
		memory_free(
		 message_handle->registry_directory_name );

		message_handle->registry_directory_name = NULL;
	}
	message_handle->registry_directory_name_size = 0;

	return( -1 );
}

/* Sets the message files (search) path
 * Returns 1 if successful or -1 error
 */
int message_handle_set_message_files_path(
     message_handle_t *message_handle,
     const libcstring_system_character_t *path,
     libcerror_error_t **error )
{
	static char *function = "message_handle_set_message_files_path";

	if( message_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid message handle.",
		 function );

		return( -1 );
	}
	if( path == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid path.",
		 function );

		return( -1 );
	}
	message_handle->message_files_path = path;

	return( 1 );
}

/* Opens the software registry file
 * Returns 1 if successful, 0 if no file was specified or -1 on error
 */
int message_handle_open_software_registry_file(
     message_handle_t *message_handle,
     libcerror_error_t **error )
{
	libcstring_system_character_t *key_path          = NULL;
	libcstring_system_character_t *software_filename = NULL;
	libregf_key_t *sub_key                           = NULL;
	libregf_value_t *value                           = NULL;
	const char *sub_key_path                         = NULL;
	const char *value_name                           = NULL;
	static char *function                            = "message_handle_open_software_registry_file";
	size_t key_path_length                           = 0;
	size_t value_name_length                         = 0;
	int result                                       = 0;

	if( message_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid message handle.",
		 function );

		return( -1 );
	}
	if( ( message_handle->software_registry_filename == NULL )
	 && ( message_handle->registry_directory_name != NULL ) )
	{
		software_filename = libcstring_system_string_allocate(
		                     9 );

		if( software_filename == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create software filename.",
			 function );

			goto on_error;
		}
		if( libcstring_system_string_copy(
		     software_filename,
		     _LIBCSTRING_SYSTEM_STRING( "SOFTWARE" ),
		     8 ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to set software filename.",
			 function );

			goto on_error;
		}
		software_filename[ 8 ] = 0;

		result = path_handle_get_directory_entry_name_by_name_no_case(
		          message_handle->path_handle,
			  message_handle->registry_directory_name,
			  message_handle->registry_directory_name_size - 1,
			  software_filename,
			  9,
		          LIBCDIRECTORY_ENTRY_TYPE_FILE,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_GENERIC,
			 "%s: unable to determine if directory has entry: %" PRIs_LIBCSTRING_SYSTEM ".",
			 function,
			 software_filename );

			goto on_error;
		}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
		result = libcpath_path_join_wide(
			  &( message_handle->software_registry_filename ),
			  &( message_handle->software_registry_filename_size ),
			  message_handle->registry_directory_name,
			  message_handle->registry_directory_name_size - 1,
			  software_filename,
			  8,
			  error );
#else
		result = libcpath_path_join(
			  &( message_handle->software_registry_filename ),
			  &( message_handle->software_registry_filename_size ),
			  message_handle->registry_directory_name,
			  message_handle->registry_directory_name_size - 1,
			  software_filename,
			  8,
			  error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable create software registry filename.",
			 function );

			return( -1 );
		}
		memory_free(
		 software_filename );

		software_filename = NULL;
	}
	if( message_handle->software_registry_filename != NULL )
	{
		if( registry_file_initialize(
		     &( message_handle->software_registry_file ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to initialize software registry file.",
			 function );

			return( -1 );
		}
		if( registry_file_set_ascii_codepage(
		     message_handle->software_registry_file,
		     message_handle->ascii_codepage,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set ASCII codepage in software registry file.",
			 function );

			return( -1 );
		}
		if( registry_file_open(
		     message_handle->software_registry_file,
		     message_handle->software_registry_filename,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_OPEN_FAILED,
			 "%s: unable to open software registry file.",
			 function );

			return( -1 );
		}
		/* Get the value of %SystemRoot% from:
		 * SOFTWARE\Microsoft\Windows NT\CurrentVersion\SystemRoot
		 */
		key_path = _LIBCSTRING_SYSTEM_STRING( "Microsoft\\Windows NT\\CurrentVersion" );

		key_path_length = libcstring_system_string_length(
		                   key_path );

		result = registry_file_get_key_by_path(
			  message_handle->software_registry_file,
			  key_path,
			  key_path_length,
			  &sub_key,
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve sub key: %" PRIs_LIBCSTRING_SYSTEM ".",
			 function,
			 sub_key_path );

			goto on_error;
		}
		else if( result != 0 )
		{
			value_name = "SystemRoot";

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
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
				result = libregf_value_get_value_utf16_string_size(
					  value,
					  &( message_handle->system_root_path_size ),
					  error );
#else
				result = libregf_value_get_value_utf8_string_size(
					  value,
					  &( message_handle->system_root_path_size ),
					  error );
#endif
				if( result == -1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve value: %s string size.",
					 function,
					 value_name );

					goto on_error;
				}
				if( ( result != 0 )
				 && ( message_handle->system_root_path_size > 0 ) )
				{
					if( ( message_handle->system_root_path_size > (size_t) SSIZE_MAX )
					 || ( ( sizeof( libcstring_system_character_t ) * message_handle->system_root_path_size ) > (size_t) SSIZE_MAX ) )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
						 "%s: invalid system root path string size value exceeds maximum.",
						 function );

						goto on_error;
					}
					message_handle->system_root_path = libcstring_system_string_allocate(
						                            message_handle->system_root_path_size );

					if( message_handle->system_root_path == NULL )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_MEMORY,
						 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
						 "%s: unable to create system root path string.",
						 function );

						goto on_error;
					}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
					result = libregf_value_get_value_utf16_string(
						  value,
						  (uint16_t *) message_handle->system_root_path,
						  message_handle->system_root_path_size,
						  error );
#else
					result = libregf_value_get_value_utf8_string(
						  value,
						  (uint8_t *) message_handle->system_root_path,
						  message_handle->system_root_path_size,
						  error );
#endif
					if( result != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
						 "%s: unable to retrieve value: %s string.",
						 function,
						 value_name );

						memory_free(
						 message_handle->system_root_path );

						message_handle->system_root_path      = NULL;
						message_handle->system_root_path_size = 0;

						goto on_error;
					}
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
	}
	/* Check if %SystemRoot% contains a sane value
	 */
/* TODO what if system root constists of multiple paths */
	if( message_handle->system_root_path != NULL )
	{
		if( ( message_handle->system_root_path_size < 4 )
		 || ( ( message_handle->system_root_path )[ 1 ] != (libcstring_system_character_t) ':' )
		 || ( ( message_handle->system_root_path )[ 2 ] != (libcstring_system_character_t) '\\' ) )
		{
			memory_free(
			 message_handle->system_root_path );

			message_handle->system_root_path      = NULL;
			message_handle->system_root_path_size = 0;
		}
	}
	/* If no usable %SystemRoot% was found use the default: C:\Windows
	 */
	if( message_handle->system_root_path == NULL )
	{
		message_handle->system_root_path_size = 11;

		message_handle->system_root_path = libcstring_system_string_allocate(
		                                    message_handle->system_root_path_size );

		if( message_handle->system_root_path == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create system root path string.",
			 function );

			goto on_error;
		}
		if( libcstring_system_string_copy(
		     message_handle->system_root_path,
		     _LIBCSTRING_SYSTEM_STRING( "C:\\Windows" ),
		     10 ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy default value to system root path string.",
			 function );

			memory_free(
			 message_handle->system_root_path );

			message_handle->system_root_path      = NULL;
			message_handle->system_root_path_size = 0;

			goto on_error;
		}
		( message_handle->system_root_path )[ 10 ] = 0;
	}
/* TODO for now %WinDir% is a copy of %SystemRoot% */
	if( message_handle->windows_directory_path == NULL )
	{
		message_handle->windows_directory_path_size = message_handle->system_root_path_size;

		message_handle->windows_directory_path = libcstring_system_string_allocate(
		                                           message_handle->windows_directory_path_size );

		if( message_handle->windows_directory_path == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create windows directory path string.",
			 function );

			goto on_error;
		}
		if( libcstring_system_string_copy(
		     message_handle->windows_directory_path,
		     message_handle->system_root_path,
		     message_handle->windows_directory_path_size - 1 ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy default value to windows directory path string.",
			 function );

			memory_free(
			 message_handle->windows_directory_path );

			message_handle->windows_directory_path      = NULL;
			message_handle->windows_directory_path_size = 0;

			goto on_error;
		}
		( message_handle->windows_directory_path )[ message_handle->windows_directory_path_size - 1 ] = 0;
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
	if( software_filename != NULL )
	{
		memory_free(
		 software_filename );
	}
	return( -1 );
}

/* Opens the system registry file
 * Returns 1 if successful, 0 if no file was specified or -1 on error
 */
int message_handle_open_system_registry_file(
     message_handle_t *message_handle,
     const char *eventlog_key_name,
     libcerror_error_t **error )
{
	libcstring_system_character_t *key_path        = NULL;
	libcstring_system_character_t *system_filename = NULL;
	libregf_key_t *sub_key                         = NULL;
	static char *function                          = "message_handle_open_system_registry_file";
	size_t eventlog_key_name_length                = 0;
	size_t key_path_length                         = 0;
	int result                                     = 0;

	if( message_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid message handle.",
		 function );

		return( -1 );
	}
	if( eventlog_key_name == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid eventlog key name.",
		 function );

		return( -1 );
	}
	eventlog_key_name_length = libcstring_narrow_string_length(
	                            eventlog_key_name );

	if( ( message_handle->system_registry_filename == NULL )
	 && ( message_handle->registry_directory_name != NULL ) )
	{
		system_filename = libcstring_system_string_allocate(
		                   7 );

		if( system_filename == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create system filename.",
			 function );

			goto on_error;
		}
		if( libcstring_system_string_copy(
		     system_filename,
		     _LIBCSTRING_SYSTEM_STRING( "SYSTEM" ),
		     6 ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to set system filename.",
			 function );

			goto on_error;
		}
		system_filename[ 6 ] = 0;

		result = path_handle_get_directory_entry_name_by_name_no_case(
		          message_handle->path_handle,
			  message_handle->registry_directory_name,
			  message_handle->registry_directory_name_size - 1,
			  system_filename,
			  7,
		          LIBCDIRECTORY_ENTRY_TYPE_FILE,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_GENERIC,
			 "%s: unable to determine if directory has entry: %" PRIs_LIBCSTRING_SYSTEM ".",
			 function,
			 system_filename );

			goto on_error;
		}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
		result = libcpath_path_join_wide(
			  &( message_handle->system_registry_filename ),
			  &( message_handle->system_registry_filename_size ),
			  message_handle->registry_directory_name,
			  message_handle->registry_directory_name_size - 1,
			  system_filename,
			  6,
			  error );
#else
		result = libcpath_path_join(
			  &( message_handle->system_registry_filename ),
			  &( message_handle->system_registry_filename_size ),
			  message_handle->registry_directory_name,
			  message_handle->registry_directory_name_size - 1,
			  system_filename,
			  6,
			  error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable create system registry filename.",
			 function );

			return( -1 );
		}
		memory_free(
		 system_filename );

		system_filename = NULL;
	}
	if( message_handle->system_registry_filename == NULL )
	{
		return( 0 );
	}
	if( registry_file_initialize(
	     &( message_handle->system_registry_file ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize system registry file.",
		 function );

		return( -1 );
	}
	if( registry_file_set_ascii_codepage(
	     message_handle->system_registry_file,
	     message_handle->ascii_codepage,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set ASCII codepage in system registry file.",
		 function );

		return( -1 );
	}
	if( registry_file_open(
	     message_handle->system_registry_file,
	     message_handle->system_registry_filename,
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
	/* Get the winevt providers key
	 * SOFTWARE\Microsoft\Windows\CurrentVersion\WINEVT\Publishers
	 */
	key_path = _LIBCSTRING_SYSTEM_STRING( "Microsoft\\Windows\\CurrentVersion\\WINEVT\\Publishers" );

	key_path_length = libcstring_system_string_length(
	                   key_path );

	result = registry_file_get_key_by_path(
		  message_handle->software_registry_file,
		  key_path,
		  key_path_length,
	          &( message_handle->winevt_publishers_key ),
		  error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve sub key: %" PRIs_LIBCSTRING_SYSTEM ".",
		 function,
		 key_path );

		goto on_error;
	}
	/* Get the control set 1 eventlog services key:
	 * SYSTEM\ControlSet001\Services\Eventlog
	 */
	key_path = _LIBCSTRING_SYSTEM_STRING( "ControlSet001\\Services\\Eventlog" );

	key_path_length = libcstring_system_string_length(
	                   key_path );

	result = registry_file_get_key_by_path(
		  message_handle->system_registry_file,
		  key_path,
		  key_path_length,
		  &sub_key,
		  error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve sub key: %" PRIs_LIBCSTRING_SYSTEM ".",
		 function,
		 key_path );

		goto on_error;
	}
	else if( result != 0 )
	{
		result = libregf_key_get_sub_key_by_utf8_name(
			  sub_key,
			  (uint8_t *) eventlog_key_name,
			  eventlog_key_name_length,
			  &( message_handle->control_set_1_eventlog_services_key ),
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve sub key: %s.",
			 function,
			 eventlog_key_name );

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
	/* Get the control set 2 eventlog services key:
	 * SYSTEM\ControlSet002\Services\Eventlog
	 */
	key_path = _LIBCSTRING_SYSTEM_STRING( "ControlSet002\\Services\\Eventlog" );

	key_path_length = libcstring_system_string_length(
	                   key_path );

	result = registry_file_get_key_by_path(
		  message_handle->system_registry_file,
		  key_path,
		  key_path_length,
		  &sub_key,
		  error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve sub key: %" PRIs_LIBCSTRING_SYSTEM ".",
		 function,
		 key_path );

		goto on_error;
	}
	else if( result != 0 )
	{
		result = libregf_key_get_sub_key_by_utf8_name(
			  sub_key,
			  (uint8_t *) eventlog_key_name,
			  eventlog_key_name_length,
			  &( message_handle->control_set_2_eventlog_services_key ),
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve sub key: %s.",
			 function,
			 eventlog_key_name );

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
	return( 1 );

on_error:
	if( sub_key != NULL )
	{
		libregf_key_free(
		 &sub_key,
		 NULL );
	}
	if( system_filename != NULL )
	{
		memory_free(
		 system_filename );
	}
	return( -1 );
}

/* Opens the input
 * Returns 1 if successful or -1 on error
 */
int message_handle_open_input(
     message_handle_t *message_handle,
     const char *eventlog_key_name,
     libcerror_error_t **error )
{
	static char *function = "message_handle_open_input";
	int result            = 0;

	if( message_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid message handle.",
		 function );

		return( -1 );
	}
	result = message_handle_open_software_registry_file(
	          message_handle,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open software registry file.",
		 function );

		return( -1 );
	}
	result = message_handle_open_system_registry_file(
	          message_handle,
	          eventlog_key_name,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open system registry file.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Closes the input
 * Returns the 0 if succesful or -1 on error
 */
int message_handle_close_input(
     message_handle_t *message_handle,
     libcerror_error_t **error )
{
	static char *function = "message_handle_close_input";
	int result            = 0;

	if( message_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid message handle.",
		 function );

		return( -1 );
	}
	if( message_handle->software_registry_file != NULL )
	{
		if( registry_file_close(
		     message_handle->software_registry_file,
		     error ) != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_CLOSE_FAILED,
			 "%s: unable to close software registry file.",
			 function );

			result = -1;
		}
	}
	if( message_handle->system_registry_file != NULL )
	{
		if( registry_file_close(
		     message_handle->system_registry_file,
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
	if( libfcache_cache_clear(
	     message_handle->message_file_cache,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to clear message file cache.",
		 function );

		result = -1;
	}
	if( libfcache_cache_clear(
	     message_handle->mui_message_file_cache,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to clear MUI message file cache.",
		 function );

		result = -1;
	}
	return( result );
}

/* Retrieves a value for a specific event source
 * The value is retrieved from the event source key in the SYSTEM Windows Registry File if available
 * Returns 1 if successful, 0 if such event source or -1 error
 */
int message_handle_get_value_by_event_source(
     message_handle_t *message_handle,
     const libcstring_system_character_t *event_source,
     size_t event_source_length,
     const libcstring_system_character_t *value_name,
     size_t value_name_length,
     libcstring_system_character_t **value_string,
     size_t *value_string_size,
     libcerror_error_t **error )
{
	libregf_key_t *key     = NULL;
	libregf_value_t *value = NULL;
	static char *function  = "message_handle_get_value_by_event_source";
	int result             = 0;

	if( message_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid message handle.",
		 function );

		return( -1 );
	}
	if( value_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value string.",
		 function );

		return( -1 );
	}
	if( *value_string != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid value string value already set.",
		 function );

		return( -1 );
	}
	if( value_string_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value string size.",
		 function );

		return( -1 );
	}
	if( message_handle->control_set_1_eventlog_services_key != NULL )
	{
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
		result = libregf_key_get_sub_key_by_utf16_name(
			  message_handle->control_set_1_eventlog_services_key,
			  (uint16_t *) event_source,
			  event_source_length,
			  &key,
			  error );
#else
		result = libregf_key_get_sub_key_by_utf8_name(
			  message_handle->control_set_1_eventlog_services_key,
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
		if( message_handle->control_set_2_eventlog_services_key != NULL )
		{
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
			result = libregf_key_get_sub_key_by_utf16_name(
				  message_handle->control_set_2_eventlog_services_key,
				  (uint16_t *) event_source,
				  event_source_length,
				  &key,
				  error );
#else
			result = libregf_key_get_sub_key_by_utf8_name(
				  message_handle->control_set_2_eventlog_services_key,
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
			 "%s: unable to retrieve value: %" PRIs_LIBCSTRING_SYSTEM ".",
			 function,
			 value_name );

			goto on_error;
		}
		else if( result != 0 )
		{
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
			result = libregf_value_get_value_utf16_string_size(
			          value,
			          value_string_size,
			          error );
#else
			result = libregf_value_get_value_utf8_string_size(
			          value,
			          value_string_size,
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
			if( *value_string_size == 0 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: missing value string.",
				 function );

				goto on_error;
			}
			*value_string = libcstring_system_string_allocate(
					 *value_string_size );

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
			result = libregf_value_get_value_utf16_string(
				  value,
				  (uint16_t *) *value_string,
				  *value_string_size,
				  error );
#else
			result = libregf_value_get_value_utf8_string(
				  value,
				  (uint8_t *) *value_string,
				  *value_string_size,
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
	if( *value_string != NULL )
	{
		memory_free(
		 *value_string );

		*value_string = NULL;
	}
	*value_string_size = 0;

	return( -1 );
}

/* Retrieves a value for a specific provider identifier
 * The value is retrieved from the WINEVT provider key in the SOFTWARE Windows Registry File if available
 * Returns 1 if successful, 0 if such event source or -1 error
 */
int message_handle_get_value_by_provider_identifier(
     message_handle_t *message_handle,
     const libcstring_system_character_t *provider_identifier,
     size_t provider_identifier_length,
     const libcstring_system_character_t *value_name,
     size_t value_name_length,
     libcstring_system_character_t **value_string,
     size_t *value_string_size,
     libcerror_error_t **error )
{
	libregf_key_t *key     = NULL;
	libregf_value_t *value = NULL;
	static char *function  = "message_handle_get_value_by_provider_identifier";
	int result             = 0;

	if( message_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid message handle.",
		 function );

		return( -1 );
	}
	if( value_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value string.",
		 function );

		return( -1 );
	}
	if( *value_string != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid value string value already set.",
		 function );

		return( -1 );
	}
	if( value_string_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value string size.",
		 function );

		return( -1 );
	}
	if( message_handle->winevt_publishers_key != NULL )
	{
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
		result = libregf_key_get_sub_key_by_utf16_name(
			  message_handle->winevt_publishers_key,
			  (uint16_t *) provider_identifier,
			  provider_identifier_length,
			  &key,
			  error );
#else
		result = libregf_key_get_sub_key_by_utf8_name(
			  message_handle->winevt_publishers_key,
			  (uint8_t *) provider_identifier,
			  provider_identifier_length,
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
			 provider_identifier );

			goto on_error;
		}
	}
	if( result != 0 )
	{
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
			 "%s: unable to retrieve value: %" PRIs_LIBCSTRING_SYSTEM ".",
			 function,
			 value_name );

			goto on_error;
		}
		else if( result != 0 )
		{
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
			result = libregf_value_get_value_utf16_string_size(
			          value,
			          value_string_size,
			          error );
#else
			result = libregf_value_get_value_utf8_string_size(
			          value,
			          value_string_size,
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
			if( *value_string_size == 0 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: missing value string.",
				 function );

				goto on_error;
			}
			*value_string = libcstring_system_string_allocate(
			                 *value_string_size );

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
			result = libregf_value_get_value_utf16_string(
				  value,
				  (uint16_t *) *value_string,
				  *value_string_size,
				  error );
#else
			result = libregf_value_get_value_utf8_string(
				  value,
				  (uint8_t *) *value_string,
				  *value_string_size,
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
	if( *value_string != NULL )
	{
		memory_free(
		 *value_string );

		*value_string = NULL;
	}
	*value_string_size = 0;

	return( -1 );
}

/* Retrieves the path of the message file based on the message filename
 * Returns 1 if successful, 0 if no path can be found or -1 error
 */
int message_handle_get_message_file_path(
     message_handle_t *message_handle,
     const libcstring_system_character_t *message_filename,
     size_t message_filename_length,
     const libcstring_system_character_t *language_string,
     size_t language_string_length,
     libcstring_system_character_t **message_file_path,
     size_t *message_file_path_size,
     libcerror_error_t **error )
{
	libcstring_system_character_t *message_filename_string_segment = NULL;
	libcstring_system_character_t *mui_string                      = NULL;
	static char *function                                          = "message_handle_get_message_file_path";
	size_t message_file_path_index                                 = 0;
	size_t message_files_path_length                               = 0;
	size_t message_filename_directory_name_index                   = 0;
	size_t message_filename_string_segment_size                    = 0;
	size_t mui_string_size                                         = 0;
	uint8_t directory_entry_type                                   = 0;
	int message_filename_number_of_segments                        = 0;
	int message_filename_segment_index                             = 0;
	int result                                                     = 0;

#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	libcsplit_wide_split_string_t *message_filename_split_string   = NULL;
#else
	libcsplit_narrow_split_string_t *message_filename_split_string = NULL;
#endif
#if defined( WINAPI )
	const libcstring_system_character_t *volume_letter             = NULL;
#endif

	if( message_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid message handle.",
		 function );

		return( -1 );
	}
	if( message_handle->system_root_path == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid message handle - missing system root path.",
		 function );

		goto on_error;
	}
	if( ( message_handle->system_root_path_size < 4 )
	 || ( message_handle->system_root_path_size > (size_t) SSIZE_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid message handle - system root path size value out of bounds.",
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
		 "%s: invalid message filename length is zero.",
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
	if( language_string != NULL )
	{
		if( language_string_length > (size_t) SSIZE_MAX )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
			 "%s: invalid language string length value exceeds maximum.",
			 function );

			return( -1 );
		}
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
#if defined( WINAPI )
			volume_letter = message_filename;
#endif
		}
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	if( libcsplit_wide_string_split(
	     &( message_filename[ message_filename_directory_name_index ] ),
	     message_filename_length - message_filename_directory_name_index + 1,
	     (libcstring_system_character_t) '\\',
	     &message_filename_split_string,
	     error ) != 1 )
#else
	if( libcsplit_narrow_string_split(
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
	if( libcsplit_wide_split_string_get_number_of_segments(
	     message_filename_split_string,
	     &message_filename_number_of_segments,
	     error ) != 1 )
#else
	if( libcsplit_narrow_split_string_get_number_of_segments(
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
	*message_file_path_size = 0;

	for( message_filename_segment_index = 0;
	     message_filename_segment_index < message_filename_number_of_segments;
	     message_filename_segment_index++ )
	{
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
		if( libcsplit_wide_split_string_get_segment_by_index(
		     message_filename_split_string,
		     message_filename_segment_index,
		     &message_filename_string_segment,
		     &message_filename_string_segment_size,
		     error ) != 1 )
#else
		if( libcsplit_narrow_split_string_get_segment_by_index(
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
			if( message_filename_string_segment_size == 9 )
			{
				/* Expand %WinDir% to WINDOWS
				 */
				if( libcstring_system_string_compare_no_case(
				     message_filename_string_segment,
				     _LIBCSTRING_SYSTEM_STRING( "%WinDir%" ),
				     8 ) == 0 )
				{
					message_filename_string_segment_size = 8;
#if defined( WINAPI )
					volume_letter = message_handle->windows_directory_path;
#endif
				}
			}
			else if( message_filename_string_segment_size == 13 )
			{
				/* Expand %SystemRoot%
				 */
				if( libcstring_system_string_compare_no_case(
				     message_filename_string_segment,
				     _LIBCSTRING_SYSTEM_STRING( "%SystemRoot%" ),
				     12 ) == 0 )
				{
					message_filename_string_segment_size = message_handle->system_root_path_size - 3;
#if defined( WINAPI )
					volume_letter = message_handle->system_root_path;
#endif
				}
			}
		}
		*message_file_path_size += message_filename_string_segment_size;
	}
	if( language_string != NULL )
	{
		/* Add: <LANGUAGE>/<FILENAME>.mui
		 */
		*message_file_path_size += language_string_length + 5;
	}
	if( message_handle->message_files_path != NULL )
	{
		message_files_path_length = libcstring_system_string_length(
		                             message_handle->message_files_path );
	}
	if( ( message_handle->message_files_path != NULL )
	 && ( message_files_path_length > 0 ) )
	{
		*message_file_path_size += message_files_path_length;

		if( message_handle->message_files_path[ message_files_path_length - 1 ] != (libcstring_system_character_t) LIBCPATH_SEPARATOR )
		{
			*message_file_path_size += 1;
		}
	}
#if defined( WINAPI )
	else if( volume_letter != NULL )
	{
		*message_file_path_size += 3;
	}
#endif
	else
	{
		*message_file_path_size += 2;
	}
	*message_file_path_size += 1;

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
	message_file_path_index = 0;

	if( ( message_handle->message_files_path != NULL )
	 && ( message_files_path_length > 0 ) )
	{
		if( libcstring_system_string_copy(
		     &( ( *message_file_path )[ message_file_path_index ] ),
		     message_handle->message_files_path,
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

		if( message_handle->message_files_path[ message_files_path_length - 1 ] != (libcstring_system_character_t) LIBCPATH_SEPARATOR )
		{
			( *message_file_path )[ message_file_path_index++ ] = (libcstring_system_character_t) LIBCPATH_SEPARATOR;
		}
	}
#if defined( WINAPI )
	else if( volume_letter != NULL )
	{
		( *message_file_path )[ message_file_path_index++ ] = volume_letter[ 0 ];
		( *message_file_path )[ message_file_path_index++ ] = (libcstring_system_character_t) ':';
		( *message_file_path )[ message_file_path_index++ ] = (libcstring_system_character_t) LIBCPATH_SEPARATOR;
	}
#endif
	else
	{
		( *message_file_path )[ message_file_path_index++ ] = (libcstring_system_character_t) '.';
		( *message_file_path )[ message_file_path_index++ ] = (libcstring_system_character_t) LIBCPATH_SEPARATOR;
	}
	for( message_filename_segment_index = 0;
	     message_filename_segment_index < message_filename_number_of_segments;
	     message_filename_segment_index++ )
	{
		if( ( language_string != NULL )
		 && ( message_filename_segment_index == ( message_filename_number_of_segments - 1 ) ) )
		{
			/* Make a copy of the language string so it can be written to
			 */
			mui_string_size = language_string_length + 1;

			mui_string = libcstring_system_string_allocate(
			              mui_string_size );

			if( mui_string == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
				 "%s: unable to create MUI language string.",
				 function );

				goto on_error;
			}
			if( libcstring_system_string_copy(
			     mui_string,
			     language_string,
			     language_string_length ) == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy MUI language string.",
				 function );

				goto on_error;
			}
			mui_string[ language_string_length ] = 0;

			( *message_file_path )[ message_file_path_index ] = 0;

			result = path_handle_get_directory_entry_name_by_name_no_case(
				  message_handle->path_handle,
				  *message_file_path,
				  message_file_path_index + 1,
				  mui_string,
				  mui_string_size,
				  LIBCDIRECTORY_ENTRY_TYPE_DIRECTORY,
				  error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_GENERIC,
				 "%s: unable to determine if directory has entry: %" PRIs_LIBCSTRING_SYSTEM ".",
				 function,
				 mui_string );

				goto on_error;
			}
			else if( result != 0 )
			{
				if( libcstring_system_string_copy(
				     &( ( *message_file_path )[ message_file_path_index ] ),
				     mui_string,
				     language_string_length ) == NULL )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
					 "%s: unable to set MUI language string in message file path.",
					 function );

					goto on_error;
				}
				message_file_path_index += language_string_length;

				( *message_file_path )[ message_file_path_index++ ] = (libcstring_system_character_t) LIBCPATH_SEPARATOR;
			}
			memory_free(
			 mui_string );

			mui_string = NULL;

			if( result == 0 )
			{
				break;
			}
		}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
		if( libcsplit_wide_split_string_get_segment_by_index(
		     message_filename_split_string,
		     message_filename_segment_index,
		     &message_filename_string_segment,
		     &message_filename_string_segment_size,
		     error ) != 1 )
#else
		if( libcsplit_narrow_split_string_get_segment_by_index(
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
			if( message_filename_string_segment_size == 9 )
			{
				/* Expand %WinDir%
				 */
				if( libcstring_system_string_compare_no_case(
				     message_filename_string_segment,
				     _LIBCSTRING_SYSTEM_STRING( "%WinDir%" ),
				     8 ) == 0 )
				{
					message_filename_string_segment      = &( ( message_handle->windows_directory_path )[ 3 ] );
					message_filename_string_segment_size = message_handle->windows_directory_path_size - 3;
				}
			}
			else if( message_filename_string_segment_size == 13 )
			{
				/* Expand %SystemRoot%
				 */
				if( libcstring_system_string_compare_no_case(
				     message_filename_string_segment,
				     _LIBCSTRING_SYSTEM_STRING( "%SystemRoot%" ),
				     12 ) == 0 )
				{
					message_filename_string_segment      = &( ( message_handle->system_root_path )[ 3 ] );
					message_filename_string_segment_size = message_handle->system_root_path_size - 3;
				}
			}
		}
		if( message_filename_segment_index < ( message_filename_number_of_segments - 1 ) )
		{
			directory_entry_type = LIBCDIRECTORY_ENTRY_TYPE_DIRECTORY;
		}
		else
		{
			if( language_string != NULL )
			{
				/* Add .mui to the filename
			 	 */
				mui_string = libcstring_system_string_allocate(
					      message_filename_string_segment_size + 4 );

				if( mui_string == NULL )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_MEMORY,
					 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
					 "%s: unable to create MUI message filename string.",
					 function );

					goto on_error;
				}
				message_filename_string_segment_size--;

				if( libcstring_system_string_copy(
				     mui_string,
				     message_filename_string_segment,
				     message_filename_string_segment_size ) == NULL )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
					 "%s: unable to copy MUI message filename string.",
					 function );

					goto on_error;
				}
				mui_string[ message_filename_string_segment_size++ ] = (libcstring_system_character_t) '.';
				mui_string[ message_filename_string_segment_size++ ] = (libcstring_system_character_t) 'm';
				mui_string[ message_filename_string_segment_size++ ] = (libcstring_system_character_t) 'u';
				mui_string[ message_filename_string_segment_size++ ] = (libcstring_system_character_t) 'i';
				mui_string[ message_filename_string_segment_size++ ] = 0;

				message_filename_string_segment = mui_string;
			}
			directory_entry_type = LIBCDIRECTORY_ENTRY_TYPE_FILE;
		}
		( *message_file_path )[ message_file_path_index ] = 0;

		result = path_handle_get_directory_entry_name_by_name_no_case(
		          message_handle->path_handle,
		          *message_file_path,
		          message_file_path_index + 1,
			  message_filename_string_segment,
			  message_filename_string_segment_size,
		          directory_entry_type,
		          error );

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
			if( libcstring_system_string_copy(
			     &( ( *message_file_path )[ message_file_path_index ] ),
			     message_filename_string_segment,
			     message_filename_string_segment_size - 1 ) == NULL )
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
			message_file_path_index += message_filename_string_segment_size - 1;

			( *message_file_path )[ message_file_path_index++ ] = (libcstring_system_character_t) LIBCPATH_SEPARATOR;
		}
		if( mui_string != NULL )
		{
			memory_free(
			 mui_string );

			mui_string = NULL;
		}
		if( result == 0 )
		{
			break;
		}
	}
	( *message_file_path )[ message_file_path_index - 1 ] = 0;

#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	if( libcsplit_wide_split_string_free(
	     &message_filename_split_string,
	     error ) != 1 )
#else
	if( libcsplit_narrow_split_string_free(
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
	return( result );

on_error:
	if( mui_string != NULL )
	{
		memory_free(
		 mui_string );
	}
	if( message_filename_split_string != NULL )
	{
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
		libcsplit_wide_split_string_free(
		 &message_filename_split_string,
		 NULL );
#else
		libcsplit_narrow_split_string_free(
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

/* Retrieves a specific message file and adds it to the cache
 * Returns 1 if successful, 0 if message file was not found or -1 error
 */
int message_handle_get_message_file(
     message_handle_t *message_handle,
     const libcstring_system_character_t *message_filename,
     size_t message_filename_length,
     const libcstring_system_character_t *message_file_path,
     message_file_t **message_file,
     libcerror_error_t **error )
{
	static char *function = "message_handle_get_message_file";

	if( message_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid message handle.",
		 function );

		return( -1 );
	}
	if( message_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid message file.",
		 function );

		return( -1 );
	}
	if( message_file_initialize(
	     message_file,
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
	     *message_file,
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

		goto on_error;
	}
	if( message_file_open(
	     *message_file,
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

		goto on_error;
	}
	if( libfcache_cache_set_value_by_index(
	     message_handle->message_file_cache,
	     message_handle->next_message_file_cache_index,
	     message_handle->next_message_file_cache_index,
	     libfcache_date_time_get_timestamp(),
	     (intptr_t *) *message_file,
	     (int (*)(intptr_t **, libcerror_error_t **)) &message_file_free,
	     LIBFCACHE_CACHE_VALUE_FLAG_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set message file in cache entry: %d.",
		 function,
		 message_handle->next_message_file_cache_index );

		goto on_error;
	}
	message_handle->next_message_file_cache_index++;

	if( message_handle->next_message_file_cache_index == 16 )
	{
		message_handle->next_message_file_cache_index = 0;
	}
	return( 1 );

on_error:
	if( *message_file != NULL )
	{
		message_file_free(
		 message_file,
		 NULL );
	}
	return( -1 );
}

/* Retrieves a specific message file from the cache
 * Returns 1 if successful, 0 if message file was not found or -1 error
 */
int message_handle_get_message_file_from_cache(
     message_handle_t *message_handle,
     const libcstring_system_character_t *message_filename,
     size_t message_filename_length,
     message_file_t **message_file,
     libcerror_error_t **error )
{
	libfcache_cache_value_t *cache_value = NULL;
	static char *function                = "message_handle_get_message_file_from_cache";
	int cache_index                      = 0;
	int result                           = 0;

	if( message_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid message handle.",
		 function );

		return( -1 );
	}
	if( message_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid message file.",
		 function );

		return( -1 );
	}
	for( cache_index = 0;
	     cache_index < 16;
	     cache_index++ )
	{
		if( libfcache_cache_get_value_by_index(
		     message_handle->message_file_cache,
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
			     (intptr_t **) message_file,
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
		if( *message_file != NULL )
		{
			if( ( message_filename_length + 1 ) != ( *message_file )->name_size )
			{
				*message_file = NULL;
			}
			else if( libcstring_system_string_compare(
				  message_filename,
				  ( *message_file )->name,
				  message_filename_length ) != 0 )
			{
				*message_file = NULL;
			}
		}
		if( *message_file != NULL )
		{
			result = 1;

			break;
		}
	}
	return( result );
}

/* Retrieves a specific MUI message file and adds it to the cache
 * Returns 1 if successful, 0 if message file was not found or -1 error
 */
int message_handle_get_mui_message_file(
     message_handle_t *message_handle,
     const libcstring_system_character_t *message_filename,
     size_t message_filename_length,
     const libcstring_system_character_t *message_file_path,
     message_file_t **message_file,
     libcerror_error_t **error )
{
	static char *function = "message_handle_get_mui_message_file";

	if( message_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid message handle.",
		 function );

		return( -1 );
	}
	if( message_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid message file.",
		 function );

		return( -1 );
	}
	if( message_file_initialize(
	     message_file,
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
	     *message_file,
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

		goto on_error;
	}
	if( message_file_open(
	     *message_file,
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

		goto on_error;
	}
	if( libfcache_cache_set_value_by_index(
	     message_handle->mui_message_file_cache,
	     message_handle->next_mui_message_file_cache_index,
	     message_handle->next_mui_message_file_cache_index,
	     libfcache_date_time_get_timestamp(),
	     (intptr_t *) *message_file,
	     (int (*)(intptr_t **, libcerror_error_t **)) &message_file_free,
	     LIBFCACHE_CACHE_VALUE_FLAG_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set message file in cache entry: %d.",
		 function,
		 message_handle->next_mui_message_file_cache_index );

		goto on_error;
	}
	message_handle->next_mui_message_file_cache_index++;

	if( message_handle->next_mui_message_file_cache_index == 16 )
	{
		message_handle->next_mui_message_file_cache_index = 0;
	}
	return( 1 );

on_error:
	if( message_file != NULL )
	{
		message_file_free(
		 message_file,
		 NULL );
	}
	return( -1 );
}

/* Retrieves a specific MUI message file from the cache
 * Returns 1 if successful, 0 if message file was not found or -1 error
 */
int message_handle_get_mui_message_file_from_cache(
     message_handle_t *message_handle,
     const libcstring_system_character_t *message_filename,
     size_t message_filename_length,
     message_file_t **message_file,
     libcerror_error_t **error )
{
	libfcache_cache_value_t *cache_value = NULL;
	static char *function                = "message_handle_get_mui_message_file_from_cache";
	int cache_index                      = 0;
	int result                           = 0;

	if( message_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid message handle.",
		 function );

		return( -1 );
	}
	if( message_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid message file.",
		 function );

		return( -1 );
	}
	for( cache_index = 0;
	     cache_index < 16;
	     cache_index++ )
	{
		if( libfcache_cache_get_value_by_index(
		     message_handle->mui_message_file_cache,
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
			     (intptr_t **) message_file,
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
		if( *message_file != NULL )
		{
			if( ( message_filename_length + 1 ) != ( *message_file )->name_size )
			{
				*message_file = NULL;
			}
			else if( libcstring_system_string_compare(
				  message_filename,
				  ( *message_file )->name,
				  message_filename_length ) != 0 )
			{
				*message_file = NULL;
			}
		}
		if( *message_file != NULL )
		{
			result = 1;

			break;
		}
	}
	return( result );
}

/* Retrieves the message string from a specific message file
 * Returns 1 if successful, 0 if no such message or -1 error
 */
int message_handle_get_message_string_from_message_file(
     message_handle_t *message_handle,
     const libcstring_system_character_t *message_filename,
     size_t message_filename_length,
     uint32_t message_identifier,
     libcstring_system_character_t **message_string,
     size_t *message_string_size,
     libcerror_error_t **error )
{
	libcstring_system_character_t *message_file_path     = NULL;
	libcstring_system_character_t *mui_message_file_path = NULL;
	message_file_t *message_file                         = NULL;
	static char *function                                = "message_handle_get_message_string_from_message_file";
	size_t message_file_path_size                        = 0;
	size_t mui_message_file_path_size                    = 0;
	uint32_t mui_file_type                               = 0;
	int result                                           = 0;

	if( message_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid message handle.",
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
	result = message_handle_get_message_file_from_cache(
		  message_handle,
		  message_filename,
		  message_filename_length,
		  &message_file,
		  error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve message file from cache.",
		 function );

		goto on_error;
	}
	if( result == 0 )
	{
		result = message_handle_get_message_file_path(
		          message_handle,
		          message_filename,
		          message_filename_length,
		          NULL,
		          0,
		          &message_file_path,
		          &message_file_path_size,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve message file path.",
			 function );

			goto on_error;
		}
		else if( result != 0 )
		{
			if( message_handle_get_message_file(
			     message_handle,
			     message_filename,
			     message_filename_length,
			     message_file_path,
			     &message_file,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve message file: %" PRIs_LIBCSTRING_SYSTEM ".",
				 function,
				 message_file_path );

				goto on_error;
			}
		}
	}
	if( message_file != NULL )
	{
		result = message_file_get_string(
			  message_file,
			  message_handle->preferred_language_identifier,
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
			 "%s: unable to retrieve message string: 0x%08" PRIx32 ".",
			 function );

			goto on_error;
		}
		else if( result == 0 )
		{
			result = message_file_get_mui_file_type(
				  message_file,
				  message_handle->preferred_language_identifier,
				  &mui_file_type,
				  error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve MUI file type.",
				 function );

				goto on_error;
			}
			else if( result != 0 )
			{
				if( mui_file_type != 0x00000011UL )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
					 "%s: unsupported MUI file type: 0x%08" PRIx32 ".",
					 function,
					 mui_file_type );
				}
				message_file = NULL;

				result = message_handle_get_mui_message_file_from_cache(
					  message_handle,
					  message_filename,
					  message_filename_length,
					  &message_file,
					  error );

				if( result == -1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve MUI message file from cache.",
					 function );

					goto on_error;
				}
				else if( result == 0 )
				{
/* TODO add support to determine language string */
					/* The MUI message file path is: %PATH%/%LANGUAGE%/%FILENAME%.mui
					 */
					result = message_handle_get_message_file_path(
						  message_handle,
					          message_filename,
					          message_filename_length,
					          _LIBCSTRING_SYSTEM_STRING( "en-US" ),
					          5,
						  &mui_message_file_path,
						  &mui_message_file_path_size,
						  error );

					if( result == -1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
						 "%s: unable to retrieve MUI message file path.",
						 function );

						goto on_error;
					}
					else if( result != 0 )
					{
						if( message_handle_get_mui_message_file(
						     message_handle,
						     message_filename,
						     message_filename_length,
						     mui_message_file_path,
						     &message_file,
						     error ) != 1 )
						{
							libcerror_error_set(
							 error,
							 LIBCERROR_ERROR_DOMAIN_RUNTIME,
							 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
							 "%s: unable to retrieve MUI message file: %" PRIs_LIBCSTRING_SYSTEM ".",
							 function,
							 mui_message_file_path );

							goto on_error;
						}
						memory_free(
						 mui_message_file_path );

						mui_message_file_path = NULL;
					}
				}
				if( message_file != NULL )
				{
					result = message_file_get_string(
						  message_file,
						  message_handle->preferred_language_identifier,
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
						 "%s: unable to retrieve message string: 0x%08" PRIx32 ".",
						 function );

						goto on_error;
					}
				}
			}
		}
	}
	if( message_file_path != NULL )
	{
		memory_free(
		 message_file_path );

		message_file_path = NULL;
	}
	return( result );

on_error:
	if( mui_message_file_path != NULL )
	{
		memory_free(
		 mui_message_file_path );
	}
	if( *message_string != NULL )
	{
		memory_free(
		 *message_string );

		*message_string = NULL;
	}
	*message_string_size = 0;

	if( message_file_path != NULL )
	{
		memory_free(
		 message_file_path );
	}
	return( -1 );
}

/* Retrieves the message string from one or specified message files
 * Returns 1 if successful, 0 if no such message or -1 error
 */
int message_handle_get_message_string(
     message_handle_t *message_handle,
     const libcstring_system_character_t *message_filename,
     size_t message_filename_length,
     uint32_t message_identifier,
     libcstring_system_character_t **message_string,
     size_t *message_string_size,
     libcerror_error_t **error )
{
	libcstring_system_character_t *message_filename_string_segment = NULL;
	static char *function                                          = "message_handle_get_message_string";
	size_t message_filename_string_segment_size                    = 0;
	int message_filename_number_of_segments                        = 0;
	int message_filename_segment_index                             = 0;
	int result                                                     = 0;

#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	libcsplit_wide_split_string_t *message_filename_split_string   = NULL;
#else
	libcsplit_narrow_split_string_t *message_filename_split_string = NULL;
#endif

	if( message_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid message handle.",
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
	/* The message filename can contain multiple file names
	 * separated by ;
	 */
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	if( libcsplit_wide_string_split(
	     message_filename,
	     message_filename_length + 1,
	     (libcstring_system_character_t) ';',
	     &message_filename_split_string,
	     error ) != 1 )
#else
	if( libcsplit_narrow_string_split(
	     message_filename,
	     message_filename_length + 1,
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
	if( libcsplit_wide_split_string_get_number_of_segments(
	     message_filename_split_string,
	     &message_filename_number_of_segments,
	     error ) != 1 )
#else
	if( libcsplit_narrow_split_string_get_number_of_segments(
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
		if( libcsplit_wide_split_string_get_segment_by_index(
		     message_filename_split_string,
		     message_filename_segment_index,
		     &message_filename_string_segment,
		     &message_filename_string_segment_size,
		     error ) != 1 )
#else
		if( libcsplit_narrow_split_string_get_segment_by_index(
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
		result = message_handle_get_message_string_from_message_file(
			  message_handle,
			  message_filename_string_segment,
			  message_filename_string_segment_size - 1,
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
			 "%s: unable to retrieve message string: 0x%08" PRIx32 " from: %" PRIs_LIBCSTRING_SYSTEM ".",
			 function,
			 message_identifier,
			 message_filename_string_segment );

			goto on_error;
		}
		else if( result != 0 )
		{
			break;
		}
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	if( libcsplit_wide_split_string_free(
	     &message_filename_split_string,
	     error ) != 1 )
#else
	if( libcsplit_narrow_split_string_free(
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
	return( result );

on_error:
	if( *message_string != NULL )
	{
		memory_free(
		 *message_string );

		*message_string = NULL;
	}
	*message_string_size = 0;

	if( message_filename_split_string != NULL )
	{
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
		libcsplit_wide_split_string_free(
		 &message_filename_split_string,
		 NULL );
#else
		libcsplit_narrow_split_string_free(
		 &message_filename_split_string,
		 NULL );
#endif
	}
	return( -1 );
}

