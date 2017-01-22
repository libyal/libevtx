/*
 * Path handle
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

#include "evtxtools_libcdirectory.h"
#include "evtxtools_libcerror.h"
#include "path_handle.h"

/* Creates a path handle
 * Make sure the value path_handle is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int path_handle_initialize(
     path_handle_t **path_handle,
     libcerror_error_t **error )
{
	static char *function = "path_handle_initialize";

	if( path_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid path handle.",
		 function );

		return( -1 );
	}
	if( *path_handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid path handle value already set.",
		 function );

		return( -1 );
	}
	*path_handle = memory_allocate_structure(
	                path_handle_t );

	if( *path_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create path handle.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *path_handle,
	     0,
	     sizeof( path_handle_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear path handle.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *path_handle != NULL )
	{
		memory_free(
		 *path_handle );

		*path_handle = NULL;
	}
	return( -1 );
}

/* Frees a path handle
 * Returns 1 if successful or -1 on error
 */
int path_handle_free(
     path_handle_t **path_handle,
     libcerror_error_t **error )
{
	static char *function = "path_handle_free";
	int result            = 1;

	if( path_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid path handle.",
		 function );

		return( -1 );
	}
	if( *path_handle != NULL )
	{
		if( ( *path_handle )->system_root_path != NULL )
		{
			memory_free(
			 ( *path_handle )->system_root_path );
		}
		memory_free(
		 *path_handle );

		*path_handle = NULL;
	}
	return( result );
}

/* Retrieves the name of a directory entry by name ignoring case
 * If a corresponding entry is found entry name is update
 * This function is needed to find case insensitive directory entries on a case sensitive system
 * Return 1 if successful, 0 if no corresponding entry was found or -1 on error
 */
int path_handle_get_directory_entry_name_by_name_no_case(
     path_handle_t *path_handle,
     const system_character_t *path,
     size_t path_length,
     system_character_t *entry_name,
     size_t entry_name_size,
     uint8_t entry_type,
     libcerror_error_t **error )
{
	libcdirectory_directory_t *directory             = NULL;
	libcdirectory_directory_entry_t *directory_entry = NULL;
	system_character_t *directory_entry_name         = NULL;
	static char *function                            = "path_handle_get_directory_entry_name_by_name_no_case";
	size_t directory_entry_name_length               = 0;
	int result                                       = 0;

	if( path_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid path handle.",
		 function );

		return( -1 );
	}
	if( path_length > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid path length value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( entry_name == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid entry name.",
		 function );

		return( -1 );
	}
	if( entry_name_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid entry name size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( entry_name == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid entry name.",
		 function );

		return( -1 );
	}
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
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libcdirectory_directory_open_wide(
		  directory,
		  path,
		  error );
#else
	result = libcdirectory_directory_open(
		  directory,
		  path,
		  error );
#endif
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open directory: %" PRIs_SYSTEM ".",
		 function,
		 path );

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
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libcdirectory_directory_has_entry_wide(
		  directory,
		  directory_entry,
		  entry_name,
		  entry_name_size - 1,
		  entry_type,
		  LIBCDIRECTORY_COMPARE_FLAG_NO_CASE,
		  error );
#else
	result = libcdirectory_directory_has_entry(
		  directory,
		  directory_entry,
		  entry_name,
		  entry_name_size - 1,
		  entry_type,
		  LIBCDIRECTORY_COMPARE_FLAG_NO_CASE,
		  error );
#endif
	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_GENERIC,
		 "%s: unable to determine if directory has entry: %" PRIs_SYSTEM ".",
		 function,
		 entry_name );

		goto on_error;
	}
	else if( result != 0 )
	{
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libcdirectory_directory_entry_get_name_wide(
			  directory_entry,
			  (wchar_t **) &directory_entry_name,
			  error );
#else
		result = libcdirectory_directory_entry_get_name(
			  directory_entry,
			  (char **) &directory_entry_name,
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
		directory_entry_name_length = system_string_length(
					       directory_entry_name );

		if( ( directory_entry_name_length + 1 ) != entry_name_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: entry name length value out of bounds.",
			 function );

			goto on_error;
		}
		if( system_string_copy(
		     entry_name,
		     directory_entry_name,
		     directory_entry_name_length ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set entry name.",
			 function );

			goto on_error;
		}
	}
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
		 function );

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
	return( result );

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
	return( -1 );
}

