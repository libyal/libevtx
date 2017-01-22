/*
 * Resource file
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

#include "evtxtools_libbfio.h"
#include "evtxtools_libcerror.h"
#include "evtxtools_libexe.h"
#include "evtxtools_libfcache.h"
#include "evtxtools_libwrc.h"
#include "message_string.h"
#include "resource_file.h"

/* Creates a resource file
 * Make sure the value resource_file is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int resource_file_initialize(
     resource_file_t **resource_file,
     uint32_t preferred_language_identifier,
     libcerror_error_t **error )
{
	static char *function = "resource_file_initialize";

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
	if( *resource_file != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid resource file value already set.",
		 function );

		return( -1 );
	}
	*resource_file = memory_allocate_structure(
	                  resource_file_t );

	if( *resource_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create resource file.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *resource_file,
	     0,
	     sizeof( resource_file_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear resource file.",
		 function );

		memory_free(
		 *resource_file );

		*resource_file = NULL;

		return( -1 );
	}
	if( libexe_file_initialize(
	     &( ( *resource_file )->exe_file ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create EXE file.",
		 function );

		goto on_error;
	}
	if( libwrc_stream_initialize(
	     &( ( *resource_file )->resource_stream ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create resource stream.",
		 function );

		goto on_error;
	}
	if( libfcache_cache_initialize(
	     &( ( *resource_file )->message_string_cache ),
	     16,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create message string cache.",
		 function );

		goto on_error;
	}
	( *resource_file )->preferred_language_identifier = preferred_language_identifier;

	return( 1 );

on_error:
	if( *resource_file != NULL )
	{
		if( ( *resource_file )->resource_stream != NULL )
		{
			libwrc_stream_free(
			 &( ( *resource_file )->resource_stream ),
			 NULL );
		}
		if( ( *resource_file )->exe_file != NULL )
		{
			libexe_file_free(
			 &( ( *resource_file )->exe_file ),
			 NULL );
		}
		memory_free(
		 *resource_file );

		*resource_file = NULL;
	}
	return( -1 );
}

/* Frees a resource file
 * Returns 1 if successful or -1 on error
 */
int resource_file_free(
     resource_file_t **resource_file,
     libcerror_error_t **error )
{
	static char *function = "resource_file_free";
	int result            = 1;

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
	if( *resource_file != NULL )
	{
		if( ( *resource_file )->is_open != 0 )
		{
			if( resource_file_close(
			     *resource_file,
			     error ) != 0 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_CLOSE_FAILED,
				 "%s: unable to close resource file.",
				 function );

				result = -1;
			}
		}
		if( libfcache_cache_free(
		     &( ( *resource_file )->message_string_cache ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free message string cache.",
			 function );

			result = -1;
		}
		if( libwrc_stream_free(
		     &( ( *resource_file )->resource_stream ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free resource stream.",
			 function );

			result = -1;
		}
		if( libexe_file_free(
		     &( ( *resource_file )->exe_file ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free EXE file.",
			 function );

			result = -1;
		}
		if( ( *resource_file )->name != NULL )
		{
			memory_free(
			 ( *resource_file )->name );
		}
		memory_free(
		 *resource_file );

		*resource_file = NULL;
	}
	return( result );
}

/* Opens the resource file
 * Returns 1 if successful or -1 on error
 */
int resource_file_open(
     resource_file_t *resource_file,
     const system_character_t *filename,
     libcerror_error_t **error )
{
	static char *function    = "resource_file_open";
	uint32_t virtual_address = 0;

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
	if( resource_file->is_open != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid resource file already open.",
		 function );

		return( -1 );
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	if( libexe_file_open_wide(
	     resource_file->exe_file,
	     filename,
	     LIBEXE_OPEN_READ,
	     error ) != 1 )
#else
	if( libexe_file_open(
	     resource_file->exe_file,
	     filename,
	     LIBEXE_OPEN_READ,
	     error ) != 1 )
#endif
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open EXE file.",
		 function );

		goto on_error;
	}
	if( libexe_file_get_section_by_name(
	     resource_file->exe_file,
	     ".rsrc",
	     5,
	     &( resource_file->resource_section ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve resource section.",
		 function );

		goto on_error;
	}
	if( libexe_section_get_virtual_address(
	     resource_file->resource_section,
	     &virtual_address,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve resource section virtual adress.",
		 function );

		goto on_error;
	}
	if( libexe_section_get_data_file_io_handle(
	     resource_file->resource_section,
	     &( resource_file->resource_section_file_io_handle ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve resource section file IO handle.",
		 function );

		goto on_error;
	}
	if( libwrc_stream_set_virtual_address(
	     resource_file->resource_stream,
	     virtual_address,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set resource stream virtual adress.",
		 function );

		goto on_error;
	}
	if( libwrc_stream_open_file_io_handle(
	     resource_file->resource_stream,
	     resource_file->resource_section_file_io_handle,
	     LIBWRC_OPEN_READ,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open resource stream.",
		 function );

		goto on_error;
	}
	resource_file->is_open = 1;

	return( 1 );

on_error:
	if( resource_file->resource_section_file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &( resource_file->resource_section_file_io_handle ),
		 NULL );
	}
	if( resource_file->resource_section != NULL )
	{
		libexe_section_free(
		 &( resource_file->resource_section ),
		 NULL );
	}
	libexe_file_close(
	 resource_file->exe_file,
	 NULL );

	return( -1 );
}

/* Closes the resource file
 * Returns the 0 if succesful or -1 on error
 */
int resource_file_close(
     resource_file_t *resource_file,
     libcerror_error_t **error )
{
	static char *function = "resource_file_close";
	int result            = 0;

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
	if( resource_file->is_open != 0 )
	{
		if( libfcache_cache_empty(
		     resource_file->message_string_cache,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to empty message string cache.",
			 function );

			result = -1;
		}
		if( resource_file->message_table_resource != NULL )
		{
			if( libwrc_resource_free(
			     &( resource_file->message_table_resource ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free message table resource.",
				 function );

				result = -1;
			}
		}
		if( resource_file->mui_resource != NULL )
		{
			if( libwrc_resource_free(
			     &( resource_file->mui_resource ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free MUI resource.",
				 function );

				result = -1;
			}
		}
		if( resource_file->wevt_template_resource != NULL )
		{
			if( libwrc_resource_free(
			     &( resource_file->wevt_template_resource ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free WEVT_TEMPLATE resource.",
				 function );

				result = -1;
			}
		}
		if( libwrc_stream_close(
		     resource_file->resource_stream,
		     error ) != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_CLOSE_FAILED,
			 "%s: unable to close resource stream.",
			 function );

			result = -1;
		}
		if( resource_file->resource_section_file_io_handle != NULL )
		{
			if( libbfio_handle_free(
			     &( resource_file->resource_section_file_io_handle ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free resource section file IO handle.",
				 function );

				result = -1;
			}
		}
		if( resource_file->resource_section != NULL )
		{
			if( libexe_section_free(
			     &( resource_file->resource_section ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free resource section.",
				 function );

				result = -1;
			}
		}
		if( libexe_file_close(
		     resource_file->exe_file,
		     error ) != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_CLOSE_FAILED,
			 "%s: unable to close EXE file.",
			 function );

			result = -1;
		}
		resource_file->is_open = 0;
	}
	return( result );
}

/* Retrieves an available language identifier from the resource
 * This function will look for the preferred languague identifier otherwise default to the first
 * Returns 1 if successful or -1 on error
 */
int resource_file_get_resource_available_languague_identifier(
     resource_file_t *resource_file,
     libwrc_resource_t *resource,
     uint32_t *language_identifier,
     libcerror_error_t **error )
{
	static char *function                 = "resource_file_get_resource_available_languague_identifier";
	uint32_t resource_language_identifier = 0;
	int language_index                    = 0;
	int number_of_languages               = 0;

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
	if( language_identifier == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid language identifier.",
		 function );

		return( -1 );
	}
	if( libwrc_resource_get_number_of_languages(
	     resource,
	     &number_of_languages,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of languages.",
		 function );

		return( -1 );
	}
	if( libwrc_resource_get_language_identifier(
	     resource,
	     language_index,
	     language_identifier,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve language identifier: %d.",
		 function,
		 language_index );

		return( -1 );
	}
	for( language_index = 1;
	     language_index < number_of_languages;
	     language_index++ )
	{
		if( libwrc_resource_get_language_identifier(
		     resource,
		     language_index,
		     &resource_language_identifier,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve language identifier: %d.",
			 function,
			 language_index );

			return( -1 );
		}
		if( ( resource_file->preferred_language_identifier & 0x000003ffUL ) == ( resource_language_identifier & 0x000003ffUL ) )
		{
			*language_identifier = resource_language_identifier;

			break;
		}
	}
	return( 1 );
}

/* Sets the name
 * Returns 1 if successful or -1 error
 */
int resource_file_set_name(
     resource_file_t *resource_file,
     const system_character_t *name,
     size_t name_length,
     libcerror_error_t **error )
{
	static char *function = "resource_file_set_name";

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
	if( resource_file->is_open != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid resource file already open.",
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
	if( name_length > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid name length value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( resource_file->name != NULL )
	{
		memory_free(
		 resource_file->name );

		resource_file->name = NULL;
	}
	resource_file->name_size = name_length + 1;

	resource_file->name = system_string_allocate(
	                       resource_file->name_size );

	if( resource_file->name == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create name.",
		 function );

		goto on_error;
	}
	if( system_string_copy(
	     resource_file->name,
	     name,
	     name_length ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy name.",
		 function );

		goto on_error;
	}
	( resource_file->name )[ name_length ] = 0;

	return( 1 );

on_error:
	if( resource_file->name != NULL )
	{
		memory_free(
		 resource_file->name );

		resource_file->name = NULL;
	}
	resource_file->name_size = 0;

	return( -1 );
}

/* Retrieves a message string from the cache
 * Returns 1 if successful, 0 if not available or -1 error
 */
int resource_file_get_message_string_from_cache(
     resource_file_t *resource_file,
     uint32_t message_string_identifier,
     message_string_t **message_string,
     libcerror_error_t **error )
{
	libfcache_cache_value_t *cache_value = NULL;
	static char *function                = "resource_file_get_message_string_from_cache";
	int cache_index                      = 0;
	int result                           = 0;

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
	for( cache_index = 0;
	     cache_index < 16;
	     cache_index++ )
	{
		if( libfcache_cache_get_value_by_index(
		     resource_file->message_string_cache,
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
			     (intptr_t **) message_string,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve message string from cache value: %d.",
				 function,
				 cache_index );

				return( -1 );
			}
		}
		if( ( *message_string != NULL )
		 && ( message_string_identifier == ( *message_string )->identifier ) )
		{
			result = 1;

			break;
		}
	}
	if( result == 0 )
	{
		*message_string = NULL;
	}
	return( result );
}

/* Retrieves a specific message string
 * Returns 1 if successful, 0 if no such message string or -1 error
 */
int resource_file_get_message_string(
     resource_file_t *resource_file,
     uint32_t message_string_identifier,
     message_string_t **message_string,
     libcerror_error_t **error )
{
	static char *function        = "resource_file_get_message_string";
	uint32_t language_identifier = 0;
	time_t timestamp             = 0;
	int result                   = 0;

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
	if( resource_file->message_table_resource == NULL )
	{
		result = libwrc_stream_get_resource_by_type(
		          resource_file->resource_stream,
		          LIBWRC_RESOURCE_TYPE_MESSAGE_TABLE,
		          &( resource_file->message_table_resource ),
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve message table resource.",
			 function );

			return( -1 );
		}
		else if( result == 0 )
		{
			return( 0 );
		}
	}
	result = resource_file_get_message_string_from_cache(
	          resource_file,
	          message_string_identifier,
	          message_string,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve message string from cache.",
		 function );

		return( -1 );
	}
	else if( result == 0 )
	{
		if( resource_file_get_resource_available_languague_identifier(
		     resource_file,
		     resource_file->message_table_resource,
		     &language_identifier,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve an available language identifier.",
			 function );

			return( -1 );
		}
		*message_string = NULL;

		if( message_string_initialize(
		     message_string,
		     message_string_identifier,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create message string.",
			 function );

			return( -1 );
		}
		result = message_string_get_from_message_table_resource(
		          *message_string,
		          resource_file->message_table_resource,
		          language_identifier,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve message string: 0x%08" PRIx32 ".",
			 function,
			 message_string_identifier );

			message_string_free(
			 message_string,
			 NULL );

			return( -1 );
		}
		else if( result != 0 )
		{
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

				message_string_free(
				 message_string,
				 NULL );

				return( -1 );
			}
			if( libfcache_cache_set_value_by_index(
			     resource_file->message_string_cache,
			     resource_file->next_message_string_cache_index,
			     0,
			     resource_file->next_message_string_cache_index,
			     timestamp,
			     (intptr_t *) *message_string,
			     (int (*)(intptr_t **, libcerror_error_t **)) &message_string_free,
			     LIBFCACHE_CACHE_VALUE_FLAG_MANAGED,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set message string in cache entry: %d.",
				 function,
				 resource_file->next_message_string_cache_index );

				message_string_free(
				 message_string,
				 NULL );

				return( -1 );
			}
			resource_file->next_message_string_cache_index++;

			if( resource_file->next_message_string_cache_index == 16 )
			{
				resource_file->next_message_string_cache_index = 0;
			}
			message_string = NULL;
		}
		if( message_string != NULL )
		{
			if( message_string_free(
			     message_string,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free message string.",
				 function );

				return( -1 );
			}
		}
	}
	return( result );
}

/* Retrieves the MUI file type
 * Returns 1 if successful, 0 if not available or -1 error
 */
int resource_file_get_mui_file_type(
     resource_file_t *resource_file,
     uint32_t *file_type,
     libcerror_error_t **error )
{
	static char *function        = "resource_file_get_mui_file_type";
	uint32_t language_identifier = 0;
	int result                   = 0;

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
	if( resource_file->mui_resource == NULL )
	{
		result = libwrc_stream_get_resource_by_utf8_name(
		          resource_file->resource_stream,
		          (uint8_t *) "MUI",
		          3,
		          &( resource_file->mui_resource ),
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve MUI resource.",
			 function );

			return( -1 );
		}
		else if( result == 0 )
		{
			return( 0 );
		}
	}
	if( resource_file_get_resource_available_languague_identifier(
	     resource_file,
	     resource_file->mui_resource,
	     &language_identifier,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve an available language identifier.",
		 function );

		return( -1 );
	}
	if( libwrc_mui_get_file_type(
	     resource_file->mui_resource,
	     language_identifier,
	     file_type,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file type.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves a specific provider from the WEVT_TEMPLATE resource
 * Returns 1 if successful, 0 if not available or -1 error
 */
int resource_file_get_provider(
     resource_file_t *resource_file,
     const uint8_t *provider_identifier,
     size_t provider_identifier_size,
     libwrc_wevt_provider_t **provider,
     libcerror_error_t **error )
{
	static char *function        = "resource_file_get_provider";
	uint32_t language_identifier = 0;
	int result                   = 0;

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
	if( resource_file->wevt_template_resource == NULL )
	{
		result = libwrc_stream_get_resource_by_utf8_name(
		          resource_file->resource_stream,
		          (uint8_t *) "WEVT_TEMPLATE",
		          13,
		          &( resource_file->wevt_template_resource ),
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve WEVT_TEMPLATE resource.",
			 function );

			return( -1 );
		}
		else if( result == 0 )
		{
			return( 0 );
		}
	}
	if( resource_file_get_resource_available_languague_identifier(
	     resource_file,
	     resource_file->wevt_template_resource,
	     &language_identifier,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve an available language identifier.",
		 function );

		return( -1 );
	}
	result = libwrc_wevt_template_get_provider_by_identifier(
	          resource_file->wevt_template_resource,
	          language_identifier,
	          provider_identifier,
	          provider_identifier_size,
	          provider,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve provider.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Retrieves a specific event from a a specific provider from the WEVT_TEMPLATE resource
 * Returns 1 if successful, 0 if not available or -1 error
 */
int resource_file_get_event(
     resource_file_t *resource_file,
     const uint8_t *provider_identifier,
     size_t provider_identifier_size,
     uint32_t event_identifier,
     libwrc_wevt_provider_t **provider,
     libwrc_wevt_event_t **event,
     libcerror_error_t **error )
{
	static char *function = "resource_file_get_event_message_identifier";
	int result            = 0;

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
	if( provider == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid provider.",
		 function );

		return( -1 );
	}
	result = resource_file_get_provider(
	          resource_file,
	          provider_identifier,
	          provider_identifier_size,
	          provider,
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
		result = libwrc_wevt_provider_get_event_by_identifier(
			  *provider,
			  event_identifier,
			  event,
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve event.",
			 function );

			goto on_error;
		}
	}
	return( result );

on_error:
	if( *provider != NULL )
	{
		libwrc_wevt_provider_free(
		 provider,
		 NULL );
	}
	return( -1 );
}

/* Retrieves the template definition from a specific event from a a specific provider from the WEVT_TEMPLATE resource
 * Returns 1 if successful, 0 if not available or -1 error
 */
int resource_file_get_template_definition(
     resource_file_t *resource_file,
     const uint8_t *provider_identifier,
     size_t provider_identifier_size,
     uint32_t event_identifier,
     libwrc_wevt_provider_t **provider,
     libwrc_wevt_event_t **event,
     libwrc_wevt_template_definition_t **template_definition,
     libcerror_error_t **error )
{
	static char *function = "resource_file_get_template_definition";
	int result            = 0;

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
	if( provider == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid provider.",
		 function );

		return( -1 );
	}
	if( event == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid event.",
		 function );

		return( -1 );
	}
	result = resource_file_get_event(
	          resource_file,
	          provider_identifier,
	          provider_identifier_size,
	          event_identifier,
	          provider,
	          event,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve event.",
		 function );

		goto on_error;
	}
	else if( result != 0 )
	{
		result = libwrc_wevt_event_get_template_definition(
			  *event,
			  template_definition,
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve template definition.",
			 function );

			goto on_error;
		}
	}
	return( 1 );

on_error:
	if( *event != NULL )
	{
		libwrc_wevt_event_free(
		 event,
		 NULL );
	}
	if( *provider != NULL )
	{
		libwrc_wevt_provider_free(
		 provider,
		 NULL );
	}
	return( -1 );
}

/* Retrieves the message identifier from a specific event from a a specific provider from the WEVT_TEMPLATE resource
 * Returns 1 if successful, 0 if not available or -1 error
 */
int resource_file_get_event_message_identifier(
     resource_file_t *resource_file,
     const uint8_t *provider_identifier,
     size_t provider_identifier_size,
     uint32_t event_identifier,
     uint32_t *message_identifier,
     libcerror_error_t **error )
{
	libwrc_wevt_event_t *event       = NULL;
	libwrc_wevt_provider_t *provider = NULL;
	static char *function            = "resource_file_get_event_message_identifier";
	int result                       = 0;

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
	result = resource_file_get_event(
	          resource_file,
	          provider_identifier,
	          provider_identifier_size,
	          event_identifier,
	          &provider,
	          &event,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve event.",
		 function );

		goto on_error;
	}
	else if( result != 0 )
	{
		if( libwrc_wevt_event_get_message_identifier(
		     event,
		     message_identifier,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve message identifier.",
			 function );

			goto on_error;
		}
		if( libwrc_wevt_event_free(
		     &event,
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
	}
	return( result );

on_error:
	if( event != NULL )
	{
		libwrc_wevt_event_free(
		 &event,
		 NULL );
	}
	if( provider != NULL )
	{
		libwrc_wevt_provider_free(
		 &provider,
		 NULL );
	}
	return( -1 );
}

