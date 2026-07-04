/*
 * Message string
 *
 * Copyright (C) 2011-2026, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <common.h>
#include <memory.h>
#include <narrow_string.h>
#include <system_string.h>
#include <types.h>
#include <wide_string.h>

#include "evtxtools_libcerror.h"
#include "evtxtools_libwrc.h"
#include "message_string.h"

/* Creates a message string
 * Make sure the value message_string is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int message_string_initialize(
     message_string_t **message_string,
     uint32_t message_string_identifier,
     libcerror_error_t **error )
{
	static char *function = "message_string_initialize";

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
	if( *message_string != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid message string value already set.",
		 function );

		return( -1 );
	}
	*message_string = memory_allocate_structure(
	                   message_string_t );

	if( *message_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create message string.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *message_string,
	     0,
	     sizeof( message_string_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear message string.",
		 function );

		memory_free(
		 *message_string );

		*message_string = NULL;

		return( -1 );
	}
	( *message_string )->identifier = message_string_identifier;

	return( 1 );

on_error:
	if( *message_string != NULL )
	{
		memory_free(
		 *message_string );

		*message_string = NULL;
	}
	return( -1 );
}

/* Frees a message string
 * Returns 1 if successful or -1 on error
 */
int message_string_free(
     message_string_t **message_string,
     libcerror_error_t **error )
{
	static char *function = "message_string_free";
	int result            = 1;

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
	if( *message_string != NULL )
	{
		if( ( *message_string )->string != NULL )
		{
			memory_free(
			 ( *message_string )->string );
		}
		memory_free(
		 *message_string );

		*message_string = NULL;
	}
	return( result );
}

/* Retrieve the message string from the message table resource
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int message_string_get_from_message_table_resource(
     message_string_t *message_string,
     libwrc_message_table_resource_t *message_table_resource,
     libcerror_error_t **error )
{
	static char *function = "message_string_get_from_message_table_resource";
	int message_index     = 0;
	int result            = 0;

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
	if( message_string->string != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid message string - string value already set.",
		 function );

		return( -1 );
	}
	result = libwrc_message_table_resource_get_index_by_identifier(
		  message_table_resource,
		  message_string->identifier,
		  &message_index,
		  error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve message index for identifier: 0x%08" PRIx32 ".",
		 function,
		 message_string->identifier );

		goto on_error;
	}
	else if( result != 0 )
	{
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libwrc_message_table_resource_get_utf16_string_size(
			  message_table_resource,
			  message_index,
			  &( message_string->string_size ),
			  error );
#else
		result = libwrc_message_table_resource_get_utf8_string_size(
			  message_table_resource,
			  message_index,
			  &( message_string->string_size ),
			  error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve message: %d size.",
			 function,
			 message_index );

			goto on_error;
		}
		message_string->string = system_string_allocate(
		                          message_string->string_size );

		if( message_string->string == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create message string.",
			 function );

			goto on_error;
		}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libwrc_message_table_resource_get_utf16_string(
			  message_table_resource,
			  message_index,
			  (uint16_t *) message_string->string,
			  message_string->string_size,
			  error );
#else
		result = libwrc_message_table_resource_get_utf8_string(
			  message_table_resource,
			  message_index,
			  (uint8_t *) message_string->string,
			  message_string->string_size,
			  error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve message string: %d.",
			 function,
			 message_index );

			goto on_error;
		}
	}
	return( result );

on_error:
	if( message_string->string != NULL )
	{
		memory_free(
		 message_string->string );

		message_string->string = NULL;
	}
	message_string->string_size = 0;

	return( -1 );
}

