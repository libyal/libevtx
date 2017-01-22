/*
 * Message handle
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

#if defined( TIME_WITH_SYS_TIME )
#include <sys/time.h>
#include <time.h>
#elif defined( HAVE_SYS_TIME_H )
#include <sys/time.h>
#else
#include <time.h>
#endif

#include "evtxtools_libcdirectory.h"
#include "evtxtools_libcerror.h"
#include "evtxtools_libcpath.h"
#include "evtxtools_libcsplit.h"
#include "evtxtools_libevtx.h"
#include "evtxtools_libfcache.h"
#include "evtxtools_libregf.h"
#include "evtxtools_libwrc.h"
#include "evtxtools_system_split_string.h"
#include "evtxtools_wide_string.h"
#include "message_handle.h"
#include "message_string.h"
#include "path_handle.h"
#include "registry_file.h"
#include "resource_file.h"

/* Creates a message handle
 * Make sure the value message_handle is referencing, is set to NULL
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
	     &( ( *message_handle )->resource_file_cache ),
	     16,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create resource file cache.",
		 function );

		goto on_error;
	}
	if( libfcache_cache_initialize(
	     &( ( *message_handle )->mui_resource_file_cache ),
	     16,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create MUI resource file cache.",
		 function );

		goto on_error;
	}
	( *message_handle )->ascii_codepage                = LIBREGF_CODEPAGE_WINDOWS_1252;
	( *message_handle )->preferred_language_identifier = 0x00000409UL;

	return( 1 );

on_error:
	if( *message_handle != NULL )
	{
		if( ( *message_handle )->resource_file_cache != NULL )
		{
			libfcache_cache_free(
			 &( ( *message_handle )->resource_file_cache ),
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

/* Frees a message handle
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
		     &( ( *message_handle )->resource_file_cache ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free resource file cache.",
			 function );

			result = -1;
		}
		if( libfcache_cache_free(
		     &( ( *message_handle )->mui_resource_file_cache ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free MUI resource file cache.",
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
     const system_character_t *filename,
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
	filename_length = system_string_length(
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

	message_handle->software_registry_filename = system_string_allocate(
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
	if( system_string_copy(
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
     const system_character_t *filename,
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
	filename_length = system_string_length(
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

	message_handle->system_registry_filename = system_string_allocate(
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
	if( system_string_copy(
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
     const system_character_t *name,
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
	name_length = system_string_length(
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

	message_handle->registry_directory_name = system_string_allocate(
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
	if( system_string_copy(
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

/* Sets the resource files (search) path
 * Returns 1 if successful or -1 error
 */
int message_handle_set_resource_files_path(
     message_handle_t *message_handle,
     const system_character_t *path,
     libcerror_error_t **error )
{
	static char *function = "message_handle_set_resource_files_path";

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
	message_handle->resource_files_path = path;

	return( 1 );
}

/* Opens the software registry file
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int message_handle_open_software_registry_file(
     message_handle_t *message_handle,
     libcerror_error_t **error )
{
	system_character_t *key_path          = NULL;
	system_character_t *software_filename = NULL;
	libregf_key_t *sub_key                = NULL;
	libregf_value_t *value                = NULL;
	const char *sub_key_path              = NULL;
	const char *value_name                = NULL;
	static char *function                 = "message_handle_open_software_registry_file";
	size_t key_path_length                = 0;
	size_t value_name_length              = 0;
	int result                            = 0;

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
		software_filename = system_string_allocate(
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
		if( system_string_copy(
		     software_filename,
		     _SYSTEM_STRING( "SOFTWARE" ),
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
			 "%s: unable to determine if directory has entry: %" PRIs_SYSTEM ".",
			 function,
			 software_filename );

			goto on_error;
		}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
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
		key_path = _SYSTEM_STRING( "Microsoft\\Windows NT\\CurrentVersion" );

		key_path_length = system_string_length(
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
			 "%s: unable to retrieve sub key: %" PRIs_SYSTEM ".",
			 function,
			 sub_key_path );

			goto on_error;
		}
		else if( result != 0 )
		{
			value_name = "SystemRoot";

			value_name_length = narrow_string_length(
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
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
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
					 || ( ( sizeof( system_character_t ) * message_handle->system_root_path_size ) > (size_t) SSIZE_MAX ) )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
						 "%s: invalid system root path string size value exceeds maximum.",
						 function );

						goto on_error;
					}
					message_handle->system_root_path = system_string_allocate(
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
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
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
		 || ( ( message_handle->system_root_path )[ 1 ] != (system_character_t) ':' )
		 || ( ( message_handle->system_root_path )[ 2 ] != (system_character_t) '\\' ) )
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

		message_handle->system_root_path = system_string_allocate(
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
		if( system_string_copy(
		     message_handle->system_root_path,
		     _SYSTEM_STRING( "C:\\Windows" ),
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

		message_handle->windows_directory_path = system_string_allocate(
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
		if( system_string_copy(
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
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int message_handle_open_system_registry_file(
     message_handle_t *message_handle,
     const char *eventlog_key_name,
     libcerror_error_t **error )
{
	libregf_key_t *sub_key              = NULL;
	system_character_t *key_path        = NULL;
	system_character_t *system_filename = NULL;
	static char *function               = "message_handle_open_system_registry_file";
	size_t eventlog_key_name_length     = 0;
	size_t key_path_length              = 0;
	int result                          = 0;

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
	eventlog_key_name_length = narrow_string_length(
	                            eventlog_key_name );

	if( ( message_handle->system_registry_filename == NULL )
	 && ( message_handle->registry_directory_name != NULL ) )
	{
		system_filename = system_string_allocate(
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
		if( system_string_copy(
		     system_filename,
		     _SYSTEM_STRING( "SYSTEM" ),
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
			 "%s: unable to determine if directory has entry: %" PRIs_SYSTEM ".",
			 function,
			 system_filename );

			goto on_error;
		}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
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
	key_path = _SYSTEM_STRING( "Microsoft\\Windows\\CurrentVersion\\WINEVT\\Publishers" );

	key_path_length = system_string_length(
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
		 "%s: unable to retrieve sub key: %" PRIs_SYSTEM ".",
		 function,
		 key_path );

		goto on_error;
	}
	/* Get the control set 1 eventlog services key:
	 * SYSTEM\ControlSet001\Services\Eventlog
	 */
	key_path = _SYSTEM_STRING( "ControlSet001\\Services\\Eventlog" );

	key_path_length = system_string_length(
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
		 "%s: unable to retrieve sub key: %" PRIs_SYSTEM ".",
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
	key_path = _SYSTEM_STRING( "ControlSet002\\Services\\Eventlog" );

	key_path_length = system_string_length(
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
		 "%s: unable to retrieve sub key: %" PRIs_SYSTEM ".",
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
	if( libfcache_cache_empty(
	     message_handle->resource_file_cache,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to empty resource file cache.",
		 function );

		result = -1;
	}
	if( libfcache_cache_empty(
	     message_handle->mui_resource_file_cache,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to empty MUI resource file cache.",
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
     const system_character_t *event_source,
     size_t event_source_length,
     const system_character_t *value_name,
     size_t value_name_length,
     system_character_t **value_string,
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
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
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
			 "%s: unable to retrieve sub key: %" PRIs_SYSTEM ".",
			 function,
			 event_source );

			goto on_error;
		}
	}
	if( result == 0 )
	{
		if( message_handle->control_set_2_eventlog_services_key != NULL )
		{
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
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
				 "%s: unable to retrieve sub key: %" PRIs_SYSTEM ".",
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
			 "%s: unable to retrieve value: %" PRIs_SYSTEM ".",
			 function,
			 value_name );

			goto on_error;
		}
		else if( result != 0 )
		{
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
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
			*value_string = system_string_allocate(
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
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
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
     const system_character_t *provider_identifier,
     size_t provider_identifier_length,
     const system_character_t *value_name,
     size_t value_name_length,
     system_character_t **value_string,
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
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
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
			 "%s: unable to retrieve sub key: %" PRIs_SYSTEM ".",
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
			 "%s: unable to retrieve value: %" PRIs_SYSTEM ".",
			 function,
			 value_name );

			goto on_error;
		}
		else if( result != 0 )
		{
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
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
			*value_string = system_string_allocate(
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
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
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

/* Retrieves the path of the resource file based on the resource filename
 * Returns 1 if successful, 0 if not available or -1 error
 */
int message_handle_get_resource_file_path(
     message_handle_t *message_handle,
     const system_character_t *resource_filename,
     size_t resource_filename_length,
     const system_character_t *language_string,
     size_t language_string_length,
     system_character_t **resource_file_path,
     size_t *resource_file_path_size,
     libcerror_error_t **error )
{
	system_character_t *mui_string                        = NULL;
	system_character_t *resource_filename_string_segment  = NULL;
	system_split_string_t *resource_filename_split_string = NULL;
	static char *function                                 = "message_handle_get_resource_file_path";
	size_t mui_string_size                                = 0;
	size_t resource_file_path_index                       = 0;
	size_t resource_filename_directory_name_index         = 0;
	size_t resource_filename_string_segment_size          = 0;
	size_t resource_files_path_length                     = 0;
	uint8_t directory_entry_type                          = 0;
	int resource_filename_number_of_segments              = 0;
	int resource_filename_segment_index                   = 0;
	int result                                            = 0;

#if defined( WINAPI )
	const system_character_t *volume_letter               = NULL;
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
	if( resource_filename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid resource filename.",
		 function );

		return( -1 );
	}
	if( resource_filename_length == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_ZERO_OR_LESS,
		 "%s: invalid resource filename length is zero.",
		 function );

		return( -1 );
	}
	if( resource_filename_length > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid resource filename length value exceeds maximum.",
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
	if( resource_file_path == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid resource file path.",
		 function );

		return( -1 );
	}
	if( *resource_file_path != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid resource file path value already set.",
		 function );

		return( -1 );
	}
	if( resource_file_path_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid resource file path size.",
		 function );

		return( -1 );
	}
	if( resource_filename_length > 2 )
	{
		/* Check if the resource filename starts with a volume letter
		 */
		if( ( resource_filename[ 1 ] == (system_character_t) ':' )
		 && ( ( ( resource_filename[ 0 ] >= (system_character_t) 'A' )
		   &&   ( resource_filename[ 0 ] <= (system_character_t) 'Z' ) )
		  || ( ( resource_filename[ 0 ] >= (system_character_t) 'a' )
		   &&  ( resource_filename[ 0 ] <= (system_character_t) 'z' ) ) ) )
		{
			resource_filename_directory_name_index = 2;

			if( ( resource_filename_length >= 3 )
			 && ( resource_filename[ 2 ] == (system_character_t) '\\' ) )
			{
				resource_filename_directory_name_index += 1;
			}
#if defined( WINAPI )
			volume_letter = resource_filename;
#endif
		}
	}
	if( system_string_split(
	     &( resource_filename[ resource_filename_directory_name_index ] ),
	     resource_filename_length - resource_filename_directory_name_index + 1,
	     (system_character_t) '\\',
	     &resource_filename_split_string,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to split resource filename.",
		 function );

		goto on_error;
	}
	if( system_split_string_get_number_of_segments(
	     resource_filename_split_string,
	     &resource_filename_number_of_segments,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of resource filename string segments.",
		 function );

		goto on_error;
	}
	*resource_file_path_size = 0;

	for( resource_filename_segment_index = 0;
	     resource_filename_segment_index < resource_filename_number_of_segments;
	     resource_filename_segment_index++ )
	{
		if( system_split_string_get_segment_by_index(
		     resource_filename_split_string,
		     resource_filename_segment_index,
		     &resource_filename_string_segment,
		     &resource_filename_string_segment_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve resource filename string segment: %d.",
			 function,
			 resource_filename_segment_index );

			goto on_error;
		}
		if( resource_filename_string_segment == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing resource filename string segment: %d.",
			 function,
			 resource_filename_segment_index );

			goto on_error;
		}
		if( resource_filename_string_segment_size == 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported empty resource filename string segment: %d.",
			 function,
			 resource_filename_segment_index );

			goto on_error;
		}
		else if( ( resource_filename_string_segment_size == 2 )
		      && ( resource_filename_string_segment[ 0 ] == (system_character_t) '.' ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported relative path in resource filename string segment: %d.",
			 function,
			 resource_filename_segment_index );

			goto on_error;
		}
		else if( ( resource_filename_string_segment_size == 3 )
		      && ( resource_filename_string_segment[ 0 ] == (system_character_t) '.' )
		      && ( resource_filename_string_segment[ 1 ] == (system_character_t) '.' ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported relative path in resource filename string segment: %d.",
			 function,
			 resource_filename_segment_index );

			goto on_error;
		}
		else if( ( resource_filename_string_segment[ 0 ] == (system_character_t) '%' )
		      && ( resource_filename_string_segment[ resource_filename_string_segment_size - 2 ] == (system_character_t) '%' ) )
		{
			if( resource_filename_string_segment_size == 9 )
			{
				/* Expand %WinDir% to WINDOWS
				 */
				if( system_string_compare_no_case(
				     resource_filename_string_segment,
				     _SYSTEM_STRING( "%WinDir%" ),
				     8 ) == 0 )
				{
					resource_filename_string_segment_size = 8;
#if defined( WINAPI )
					volume_letter = message_handle->windows_directory_path;
#endif
				}
			}
			else if( resource_filename_string_segment_size == 13 )
			{
				/* Expand %SystemRoot%
				 */
				if( system_string_compare_no_case(
				     resource_filename_string_segment,
				     _SYSTEM_STRING( "%SystemRoot%" ),
				     12 ) == 0 )
				{
					resource_filename_string_segment_size = message_handle->system_root_path_size - 3;
#if defined( WINAPI )
					volume_letter = message_handle->system_root_path;
#endif
				}
			}
		}
		*resource_file_path_size += resource_filename_string_segment_size;
	}
	if( language_string != NULL )
	{
		/* Add: <LANGUAGE>/<FILENAME>.mui
		 */
		*resource_file_path_size += language_string_length + 5;
	}
	if( message_handle->resource_files_path != NULL )
	{
		resource_files_path_length = system_string_length(
		                              message_handle->resource_files_path );
	}
	if( ( message_handle->resource_files_path != NULL )
	 && ( resource_files_path_length > 0 ) )
	{
		*resource_file_path_size += resource_files_path_length;

		if( message_handle->resource_files_path[ resource_files_path_length - 1 ] != (system_character_t) LIBCPATH_SEPARATOR )
		{
			*resource_file_path_size += 1;
		}
	}
#if defined( WINAPI )
	else if( volume_letter != NULL )
	{
		*resource_file_path_size += 3;
	}
#endif
	else
	{
		*resource_file_path_size += 2;
	}
	*resource_file_path_size += 1;

	*resource_file_path = system_string_allocate(
	                       *resource_file_path_size );

	if( *resource_file_path == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create resource file path.",
		 function );

		goto on_error;
	}
	resource_file_path_index = 0;

	if( ( message_handle->resource_files_path != NULL )
	 && ( resource_files_path_length > 0 ) )
	{
		if( system_string_copy(
		     &( ( *resource_file_path )[ resource_file_path_index ] ),
		     message_handle->resource_files_path,
		     resource_files_path_length ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy resource files path to resource file path.",
			 function );

			goto on_error;
		}
		resource_file_path_index += resource_files_path_length;

		if( message_handle->resource_files_path[ resource_files_path_length - 1 ] != (system_character_t) LIBCPATH_SEPARATOR )
		{
			( *resource_file_path )[ resource_file_path_index++ ] = (system_character_t) LIBCPATH_SEPARATOR;
		}
	}
#if defined( WINAPI )
	else if( volume_letter != NULL )
	{
		( *resource_file_path )[ resource_file_path_index++ ] = volume_letter[ 0 ];
		( *resource_file_path )[ resource_file_path_index++ ] = (system_character_t) ':';
		( *resource_file_path )[ resource_file_path_index++ ] = (system_character_t) LIBCPATH_SEPARATOR;
	}
#endif
	else
	{
		( *resource_file_path )[ resource_file_path_index++ ] = (system_character_t) '.';
		( *resource_file_path )[ resource_file_path_index++ ] = (system_character_t) LIBCPATH_SEPARATOR;
	}
	for( resource_filename_segment_index = 0;
	     resource_filename_segment_index < resource_filename_number_of_segments;
	     resource_filename_segment_index++ )
	{
		if( ( language_string != NULL )
		 && ( resource_filename_segment_index == ( resource_filename_number_of_segments - 1 ) ) )
		{
			/* Make a copy of the language string so it can be written to
			 */
			mui_string_size = language_string_length + 1;

			mui_string = system_string_allocate(
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
			if( system_string_copy(
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

			( *resource_file_path )[ resource_file_path_index ] = 0;

			result = path_handle_get_directory_entry_name_by_name_no_case(
				  message_handle->path_handle,
				  *resource_file_path,
				  resource_file_path_index + 1,
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
				 "%s: unable to determine if directory has entry: %" PRIs_SYSTEM ".",
				 function,
				 mui_string );

				goto on_error;
			}
			else if( result != 0 )
			{
				if( system_string_copy(
				     &( ( *resource_file_path )[ resource_file_path_index ] ),
				     mui_string,
				     language_string_length ) == NULL )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
					 "%s: unable to set MUI language string in resource file path.",
					 function );

					goto on_error;
				}
				resource_file_path_index += language_string_length;

				( *resource_file_path )[ resource_file_path_index++ ] = (system_character_t) LIBCPATH_SEPARATOR;
			}
			memory_free(
			 mui_string );

			mui_string = NULL;

			if( result == 0 )
			{
				break;
			}
		}
		if( system_split_string_get_segment_by_index(
		     resource_filename_split_string,
		     resource_filename_segment_index,
		     &resource_filename_string_segment,
		     &resource_filename_string_segment_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve resource filename string segment: %d.",
			 function,
			 resource_filename_segment_index );

			goto on_error;
		}
		if( resource_filename_string_segment == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing resource filename string segment: %d.",
			 function,
			 resource_filename_segment_index );

			goto on_error;
		}
		if( ( resource_filename_string_segment[ 0 ] == (system_character_t) '%' )
		 && ( resource_filename_string_segment[ resource_filename_string_segment_size - 2 ] == (system_character_t) '%' ) )
		{
			if( resource_filename_string_segment_size == 9 )
			{
				/* Expand %WinDir%
				 */
				if( system_string_compare_no_case(
				     resource_filename_string_segment,
				     _SYSTEM_STRING( "%WinDir%" ),
				     8 ) == 0 )
				{
					resource_filename_string_segment      = &( ( message_handle->windows_directory_path )[ 3 ] );
					resource_filename_string_segment_size = message_handle->windows_directory_path_size - 3;
				}
			}
			else if( resource_filename_string_segment_size == 13 )
			{
				/* Expand %SystemRoot%
				 */
				if( system_string_compare_no_case(
				     resource_filename_string_segment,
				     _SYSTEM_STRING( "%SystemRoot%" ),
				     12 ) == 0 )
				{
					resource_filename_string_segment      = &( ( message_handle->system_root_path )[ 3 ] );
					resource_filename_string_segment_size = message_handle->system_root_path_size - 3;
				}
			}
		}
		if( resource_filename_segment_index < ( resource_filename_number_of_segments - 1 ) )
		{
			directory_entry_type = LIBCDIRECTORY_ENTRY_TYPE_DIRECTORY;
		}
		else
		{
			if( language_string != NULL )
			{
				/* Add .mui to the filename
			 	 */
				mui_string = system_string_allocate(
					      resource_filename_string_segment_size + 4 );

				if( mui_string == NULL )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_MEMORY,
					 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
					 "%s: unable to create MUI resource filename string.",
					 function );

					goto on_error;
				}
				resource_filename_string_segment_size--;

				if( system_string_copy(
				     mui_string,
				     resource_filename_string_segment,
				     resource_filename_string_segment_size ) == NULL )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
					 "%s: unable to copy MUI resource filename string.",
					 function );

					goto on_error;
				}
				mui_string[ resource_filename_string_segment_size++ ] = (system_character_t) '.';
				mui_string[ resource_filename_string_segment_size++ ] = (system_character_t) 'm';
				mui_string[ resource_filename_string_segment_size++ ] = (system_character_t) 'u';
				mui_string[ resource_filename_string_segment_size++ ] = (system_character_t) 'i';
				mui_string[ resource_filename_string_segment_size++ ] = 0;

				resource_filename_string_segment = mui_string;
			}
			directory_entry_type = LIBCDIRECTORY_ENTRY_TYPE_FILE;
		}
		( *resource_file_path )[ resource_file_path_index ] = 0;

		result = path_handle_get_directory_entry_name_by_name_no_case(
		          message_handle->path_handle,
		          *resource_file_path,
		          resource_file_path_index + 1,
			  resource_filename_string_segment,
			  resource_filename_string_segment_size,
		          directory_entry_type,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_GENERIC,
			 "%s: unable to determine if directory has entry: %" PRIs_SYSTEM ".",
			 function,
			 resource_filename_string_segment );

			goto on_error;
		}
		else if( result != 0 )
		{
			if( system_string_copy(
			     &( ( *resource_file_path )[ resource_file_path_index ] ),
			     resource_filename_string_segment,
			     resource_filename_string_segment_size - 1 ) == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set resource filename string segment: %d in resource file path.",
				 function,
				 resource_filename_segment_index );

				goto on_error;
			}
			resource_file_path_index += resource_filename_string_segment_size - 1;

			( *resource_file_path )[ resource_file_path_index++ ] = (system_character_t) LIBCPATH_SEPARATOR;
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
	( *resource_file_path )[ resource_file_path_index - 1 ] = 0;

	if( system_split_string_free(
	     &resource_filename_split_string,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free resource filename split string.",
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
	if( resource_filename_split_string != NULL )
	{
		system_split_string_free(
		 &resource_filename_split_string,
		 NULL );
	}
	if( *resource_file_path != NULL )
	{
		memory_free(
		 *resource_file_path );

		*resource_file_path = NULL;
	}
	*resource_file_path_size = 0;

	return( -1 );
}

/* Retrieves a specific resource file and adds it to the cache
 * Returns 1 if successful, 0 if resource file was not found or -1 error
 */
int message_handle_get_resource_file(
     message_handle_t *message_handle,
     const system_character_t *resource_filename,
     size_t resource_filename_length,
     const system_character_t *resource_file_path,
     resource_file_t **resource_file,
     libcerror_error_t **error )
{
	static char *function = "message_handle_get_resource_file";
	time_t timestamp      = 0;

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
	if( resource_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid resource file.",
		 function );

		return( -1 );
	}
	if( resource_file_initialize(
	     resource_file,
	     message_handle->preferred_language_identifier,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create resource file.",
		 function );

		goto on_error;
	}
	if( resource_file_set_name(
	     *resource_file,
	     resource_filename,
	     resource_filename_length,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set name in resource file.",
		 function );

		goto on_error;
	}
	if( resource_file_open(
	     *resource_file,
	     resource_file_path,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open resource file: %" PRIs_SYSTEM ".",
		 function,
		 resource_file_path );

		goto on_error;
	}
	if( libfcache_date_time_get_timestamp(
	     &timestamp,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve cache timestamp.",
		 function );

		return( -1 );
	}
	if( libfcache_cache_set_value_by_index(
	     message_handle->resource_file_cache,
	     message_handle->next_resource_file_cache_index,
	     0,
	     message_handle->next_resource_file_cache_index,
	     timestamp,
	     (intptr_t *) *resource_file,
	     (int (*)(intptr_t **, libcerror_error_t **)) &resource_file_free,
	     LIBFCACHE_CACHE_VALUE_FLAG_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set resource file in cache entry: %d.",
		 function,
		 message_handle->next_resource_file_cache_index );

		goto on_error;
	}
	message_handle->next_resource_file_cache_index++;

	if( message_handle->next_resource_file_cache_index == 16 )
	{
		message_handle->next_resource_file_cache_index = 0;
	}
	return( 1 );

on_error:
	if( *resource_file != NULL )
	{
		resource_file_free(
		 resource_file,
		 NULL );
	}
	return( -1 );
}

/* Retrieves a specific resource file from the cache
 * Returns 1 if successful, 0 if not available or -1 error
 */
int message_handle_get_resource_file_from_cache(
     message_handle_t *message_handle,
     const system_character_t *resource_filename,
     size_t resource_filename_length,
     resource_file_t **resource_file,
     libcerror_error_t **error )
{
	libfcache_cache_value_t *cache_value = NULL;
	static char *function                = "message_handle_get_resource_file_from_cache";
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
	if( resource_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid resource file.",
		 function );

		return( -1 );
	}
	for( cache_index = 0;
	     cache_index < 16;
	     cache_index++ )
	{
		if( libfcache_cache_get_value_by_index(
		     message_handle->resource_file_cache,
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
			     (intptr_t **) resource_file,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve resource file from cache value: %d.",
				 function,
				 cache_index );

				return( -1 );
			}
		}
		if( *resource_file != NULL )
		{
			if( ( resource_filename_length + 1 ) != ( *resource_file )->name_size )
			{
				*resource_file = NULL;
			}
			else if( system_string_compare(
				  resource_filename,
				  ( *resource_file )->name,
				  resource_filename_length ) != 0 )
			{
				*resource_file = NULL;
			}
		}
		if( *resource_file != NULL )
		{
			result = 1;

			break;
		}
	}
	if( result == 0 )
	{
		*resource_file = NULL;
	}
	return( result );
}

/* Retrieves a specific MUI resource file and adds it to the cache
 * Returns 1 if successful, 0 if resource file was not found or -1 error
 */
int message_handle_get_mui_resource_file(
     message_handle_t *message_handle,
     const system_character_t *resource_filename,
     size_t resource_filename_length,
     const system_character_t *resource_file_path,
     resource_file_t **resource_file,
     libcerror_error_t **error )
{
	static char *function = "message_handle_get_mui_resource_file";
	time_t timestamp      = 0;

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
	if( resource_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid resource file.",
		 function );

		return( -1 );
	}
	if( resource_file_initialize(
	     resource_file,
	     message_handle->preferred_language_identifier,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create resource file.",
		 function );

		goto on_error;
	}
	if( resource_file_set_name(
	     *resource_file,
	     resource_filename,
	     resource_filename_length,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set name in resource file.",
		 function );

		goto on_error;
	}
	if( resource_file_open(
	     *resource_file,
	     resource_file_path,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open resource file: %" PRIs_SYSTEM ".",
		 function,
		 resource_file_path );

		goto on_error;
	}
	if( libfcache_date_time_get_timestamp(
	     &timestamp,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve cache timestamp.",
		 function );

		return( -1 );
	}
	if( libfcache_cache_set_value_by_index(
	     message_handle->mui_resource_file_cache,
	     message_handle->next_mui_resource_file_cache_index,
	     0,
	     message_handle->next_mui_resource_file_cache_index,
	     timestamp,
	     (intptr_t *) *resource_file,
	     (int (*)(intptr_t **, libcerror_error_t **)) &resource_file_free,
	     LIBFCACHE_CACHE_VALUE_FLAG_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set resource file in cache entry: %d.",
		 function,
		 message_handle->next_mui_resource_file_cache_index );

		goto on_error;
	}
	message_handle->next_mui_resource_file_cache_index++;

	if( message_handle->next_mui_resource_file_cache_index == 16 )
	{
		message_handle->next_mui_resource_file_cache_index = 0;
	}
	return( 1 );

on_error:
	if( resource_file != NULL )
	{
		resource_file_free(
		 resource_file,
		 NULL );
	}
	return( -1 );
}

/* Retrieves a specific MUI resource file from the cache
 * Returns 1 if successful, 0 if resource file was not found or -1 error
 */
int message_handle_get_mui_resource_file_from_cache(
     message_handle_t *message_handle,
     const system_character_t *resource_filename,
     size_t resource_filename_length,
     resource_file_t **resource_file,
     libcerror_error_t **error )
{
	libfcache_cache_value_t *cache_value = NULL;
	static char *function                = "message_handle_get_mui_resource_file_from_cache";
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
	if( resource_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid resource file.",
		 function );

		return( -1 );
	}
	for( cache_index = 0;
	     cache_index < 16;
	     cache_index++ )
	{
		if( libfcache_cache_get_value_by_index(
		     message_handle->mui_resource_file_cache,
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
			     (intptr_t **) resource_file,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve resource file from cache value: %d.",
				 function,
				 cache_index );

				return( -1 );
			}
		}
		if( *resource_file != NULL )
		{
			if( ( resource_filename_length + 1 ) != ( *resource_file )->name_size )
			{
				*resource_file = NULL;
			}
			else if( system_string_compare(
				  resource_filename,
				  ( *resource_file )->name,
				  resource_filename_length ) != 0 )
			{
				*resource_file = NULL;
			}
		}
		if( *resource_file != NULL )
		{
			result = 1;

			break;
		}
	}
	return( result );
}

/* Retrieves the message string from a specific resource file
 * Returns 1 if successful, 0 if not available or -1 error
 */
int message_handle_get_message_string_from_resource_file(
     message_handle_t *message_handle,
     const system_character_t *resource_filename,
     size_t resource_filename_length,
     uint32_t message_identifier,
     message_string_t **message_string,
     libcerror_error_t **error )
{
	system_character_t *resource_file_path     = NULL;
	system_character_t *mui_resource_file_path = NULL;
	resource_file_t *resource_file             = NULL;
	static char *function                      = "message_handle_get_message_string_from_resource_file";
	size_t resource_file_path_size             = 0;
	size_t mui_resource_file_path_size         = 0;
	uint32_t mui_file_type                     = 0;
	int result                                 = 0;

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
	if( resource_filename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid resource filename.",
		 function );

		return( -1 );
	}
	if( ( resource_filename_length == 0 )
	 || ( resource_filename_length > (size_t) SSIZE_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid resource filename length value out of bounds.",
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
	result = message_handle_get_resource_file_from_cache(
		  message_handle,
		  resource_filename,
		  resource_filename_length,
		  &resource_file,
		  error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve resource file from cache.",
		 function );

		goto on_error;
	}
	if( result == 0 )
	{
		result = message_handle_get_resource_file_path(
		          message_handle,
		          resource_filename,
		          resource_filename_length,
		          NULL,
		          0,
		          &resource_file_path,
		          &resource_file_path_size,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve resource file path.",
			 function );

			goto on_error;
		}
		else if( result != 0 )
		{
			if( message_handle_get_resource_file(
			     message_handle,
			     resource_filename,
			     resource_filename_length,
			     resource_file_path,
			     &resource_file,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve resource file: %" PRIs_SYSTEM ".",
				 function,
				 resource_file_path );

				goto on_error;
			}
		}
	}
	if( resource_file != NULL )
	{
		result = resource_file_get_message_string(
			  resource_file,
			  message_identifier,
			  message_string,
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve message string: 0x%08" PRIx32 ".",
			 function,
			 message_identifier );

			goto on_error;
		}
		else if( result == 0 )
		{
			result = resource_file_get_mui_file_type(
				  resource_file,
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
				resource_file = NULL;

				result = message_handle_get_mui_resource_file_from_cache(
					  message_handle,
					  resource_filename,
					  resource_filename_length,
					  &resource_file,
					  error );

				if( result == -1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve MUI resource file from cache.",
					 function );

					goto on_error;
				}
				else if( result == 0 )
				{
/* TODO add support to determine language string */
					/* The MUI resource file path is: %PATH%/%LANGUAGE%/%FILENAME%.mui
					 */
					result = message_handle_get_resource_file_path(
						  message_handle,
					          resource_filename,
					          resource_filename_length,
					          _SYSTEM_STRING( "en-US" ),
					          5,
						  &mui_resource_file_path,
						  &mui_resource_file_path_size,
						  error );

					if( result == -1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
						 "%s: unable to retrieve MUI resource file path.",
						 function );

						goto on_error;
					}
					else if( result != 0 )
					{
						if( message_handle_get_mui_resource_file(
						     message_handle,
						     resource_filename,
						     resource_filename_length,
						     mui_resource_file_path,
						     &resource_file,
						     error ) != 1 )
						{
							libcerror_error_set(
							 error,
							 LIBCERROR_ERROR_DOMAIN_RUNTIME,
							 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
							 "%s: unable to retrieve MUI resource file: %" PRIs_SYSTEM ".",
							 function,
							 mui_resource_file_path );

							goto on_error;
						}
						memory_free(
						 mui_resource_file_path );

						mui_resource_file_path = NULL;
					}
				}
				if( resource_file != NULL )
				{
					result = resource_file_get_message_string(
						  resource_file,
						  message_identifier,
						  message_string,
						  error );

					if( result == -1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
						 "%s: unable to retrieve message string: 0x%08" PRIx32 ".",
						 function,
						 message_identifier );

						goto on_error;
					}
				}
			}
		}
	}
	if( resource_file_path != NULL )
	{
		memory_free(
		 resource_file_path );

		resource_file_path = NULL;
	}
	return( result );

on_error:
	if( mui_resource_file_path != NULL )
	{
		memory_free(
		 mui_resource_file_path );
	}
	if( resource_file_path != NULL )
	{
		memory_free(
		 resource_file_path );
	}
	if( *message_string != NULL )
	{
		message_string_free(
		 message_string,
		 NULL );
	}
	return( -1 );
}

/* Retrieves the message string from one or more specified resource files
 * Returns 1 if successful, 0 if not available or -1 error
 */
int message_handle_get_message_string(
     message_handle_t *message_handle,
     const system_character_t *resource_filename,
     size_t resource_filename_length,
     uint32_t message_identifier,
     message_string_t **message_string,
     libcerror_error_t **error )
{
	system_character_t *resource_filename_string_segment  = NULL;
	system_split_string_t *resource_filename_split_string = NULL;
	static char *function                                 = "message_handle_get_message_string";
	size_t resource_filename_string_segment_size          = 0;
	int resource_filename_number_of_segments              = 0;
	int resource_filename_segment_index                   = 0;
	int result                                            = 0;

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
	/* The resource filename can contain multiple file names separated by ;
	 */
	if( system_string_split(
	     resource_filename,
	     resource_filename_length + 1,
	     (system_character_t) ';',
	     &resource_filename_split_string,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to split resource filename.",
		 function );

		goto on_error;
	}
	if( system_split_string_get_number_of_segments(
	     resource_filename_split_string,
	     &resource_filename_number_of_segments,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of resource filename string segments.",
		 function );

		goto on_error;
	}
	for( resource_filename_segment_index = 0;
	     resource_filename_segment_index < resource_filename_number_of_segments;
	     resource_filename_segment_index++ )
	{
		if( system_split_string_get_segment_by_index(
		     resource_filename_split_string,
		     resource_filename_segment_index,
		     &resource_filename_string_segment,
		     &resource_filename_string_segment_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve resource filename string segment: %d.",
			 function,
			 resource_filename_segment_index );

			goto on_error;
		}
		if( resource_filename_string_segment == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing resource filename string segment: %d.",
			 function,
			 resource_filename_segment_index );

			goto on_error;
		}
		result = message_handle_get_message_string_from_resource_file(
			  message_handle,
			  resource_filename_string_segment,
			  resource_filename_string_segment_size - 1,
			  message_identifier,
			  message_string,
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve message string: 0x%08" PRIx32 " from: %" PRIs_SYSTEM ".",
			 function,
			 message_identifier,
			 resource_filename_string_segment );

			goto on_error;
		}
		else if( result != 0 )
		{
			break;
		}
	}
	if( system_split_string_free(
	     &resource_filename_split_string,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free resource filename split string.",
		 function );

		goto on_error;
	}
	return( result );

on_error:
	if( resource_filename_split_string != NULL )
	{
		system_split_string_free(
		 &resource_filename_split_string,
		 NULL );
	}
	if( *message_string != NULL )
	{
		message_string_free(
		 message_string,
		 NULL );
	}
	return( -1 );
}

/* Retrieves a specific resource file by provider identifier
 * Returns 1 if successful, 0 if not available or -1 error
 */
int message_handle_get_resource_file_by_provider_identifier(
     message_handle_t *message_handle,
     const system_character_t *resource_filename,
     size_t resource_filename_length,
     const uint8_t *provider_identifier,
     size_t provider_identifier_size,
     resource_file_t **resource_file,
     libcerror_error_t **error )
{
	libwrc_wevt_provider_t *provider                      = NULL;
	system_character_t *resource_file_path                = NULL;
	system_character_t *resource_filename_string_segment  = NULL;
	system_split_string_t *resource_filename_split_string = NULL;
	static char *function                                 = "message_handle_get_resource_file_by_provider_identifier";
	size_t resource_file_path_size                        = 0;
	size_t resource_filename_string_segment_size          = 0;
	int resource_filename_number_of_segments              = 0;
	int resource_filename_segment_index                   = 0;
	int result                                            = 0;

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
	if( resource_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid resource file.",
		 function );

		return( -1 );
	}
	/* The resource filename can contain multiple file names separated by ;
	 */
	if( system_string_split(
	     resource_filename,
	     resource_filename_length + 1,
	     (system_character_t) ';',
	     &resource_filename_split_string,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to split resource filename.",
		 function );

		goto on_error;
	}
	if( system_split_string_get_number_of_segments(
	     resource_filename_split_string,
	     &resource_filename_number_of_segments,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of resource filename string segments.",
		 function );

		goto on_error;
	}
	for( resource_filename_segment_index = 0;
	     resource_filename_segment_index < resource_filename_number_of_segments;
	     resource_filename_segment_index++ )
	{
		if( system_split_string_get_segment_by_index(
		     resource_filename_split_string,
		     resource_filename_segment_index,
		     &resource_filename_string_segment,
		     &resource_filename_string_segment_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve resource filename string segment: %d.",
			 function,
			 resource_filename_segment_index );

			goto on_error;
		}
		result = message_handle_get_resource_file_from_cache(
			  message_handle,
			  resource_filename_string_segment,
			  resource_filename_string_segment_size - 1,
			  resource_file,
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve resource file: %d from cache.",
			 function,
			 resource_filename_segment_index );

			goto on_error;
		}
		if( result == 0 )
		{
			result = message_handle_get_resource_file_path(
				  message_handle,
				  resource_filename_string_segment,
				  resource_filename_string_segment_size - 1,
				  NULL,
				  0,
				  &resource_file_path,
				  &resource_file_path_size,
				  error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve resource file path.",
				 function );

				goto on_error;
			}
			else if( result != 0 )
			{
				if( message_handle_get_resource_file(
				     message_handle,
				     resource_filename,
				     resource_filename_length,
				     resource_file_path,
				     resource_file,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve resource file: %" PRIs_SYSTEM ".",
					 function,
					 resource_file_path );

					goto on_error;
				}
				memory_free(
				 resource_file_path );

				resource_file_path = NULL;
			}
		}
		if( resource_file != NULL )
		{
			result = resource_file_get_provider(
			          *resource_file,
				  provider_identifier,
				  provider_identifier_size,
				  &provider,
				  error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve provider.",
				 function );

				goto on_error;
			}
			else if( result != 0 )
			{
				if( libwrc_wevt_provider_free(
				     &provider,
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
				break;
			}
		}
	}
	if( system_split_string_free(
	     &resource_filename_split_string,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free resource filename split string.",
		 function );

		goto on_error;
	}
	return( result );

on_error:
	if( resource_file_path != NULL )
	{
		memory_free(
		 resource_file_path );
	}
	if( resource_filename_split_string != NULL )
	{
		system_split_string_free(
		 &resource_filename_split_string,
		 NULL );
	}
	return( -1 );
}

