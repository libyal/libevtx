/*
 * Message string
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
#include <file_stream.h>
#include <memory.h>
#include <narrow_string.h>
#include <system_string.h>
#include <types.h>
#include <wide_string.h>

#include "evtxtools_libcerror.h"
#include "evtxtools_libevtx.h"
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
     libwrc_resource_t *message_table_resource,
     uint32_t language_identifier,
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
	result = libwrc_message_table_get_index_by_identifier(
		  message_table_resource,
		  language_identifier,
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
		result = libwrc_message_table_get_utf16_string_size(
			  message_table_resource,
			  language_identifier,
			  message_index,
			  &( message_string->string_size ),
			  error );
#else
		result = libwrc_message_table_get_utf8_string_size(
			  message_table_resource,
			  language_identifier,
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
		result = libwrc_message_table_get_utf16_string(
			  message_table_resource,
			  language_identifier,
			  message_index,
			  (uint16_t *) message_string->string,
			  message_string->string_size,
			  error );
#else
		result = libwrc_message_table_get_utf8_string(
			  message_table_resource,
			  language_identifier,
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

/* Prints the message string to a FILE stream
 * Returns 1 if successful or -1 on error
 */
int message_string_fprint(
     message_string_t *message_string,
     libevtx_record_t *record,
     FILE *stream,
     libcerror_error_t **error )
{
	system_character_t *value_string   = NULL;
	static char *function              = "message_string_fprint";
	size_t conversion_specifier_length = 0;
	size_t message_string_length       = 0;
	size_t message_string_index        = 0;
	size_t value_string_size           = 0;
	system_character_t last_character  = 0;
	int number_of_strings              = 0;
	int result                         = 0;
	int value_string_index             = 0;

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
#if defined( HAVE_DEBUG_OUTPUT )
	fprintf(
	 stream,
	 "Message format string\t\t: %" PRIs_SYSTEM "\n",
	 message_string->string );
#endif
	fprintf(
	 stream,
	 "Message string\t\t\t: " );

	message_string_length = message_string->string_size - 1;
	message_string_index  = 0;

	while( message_string_index < message_string_length )
	{
		if( ( ( message_string->string )[ message_string_index ] == (system_character_t) '%' )
		 && ( ( message_string_index + 1 ) < message_string_length ) )
		{
/* TODO add support for more conversion specifiers */
			/* Ignore %0 = end of string, %r = cariage return */
			if( ( ( message_string->string )[ message_string_index + 1 ] == (system_character_t) '0' )
			 || ( ( message_string->string )[ message_string_index + 1 ] == (system_character_t) 'r' ) )
			{
				message_string_index += 2;

				continue;
			}
			/* Replace:
			 *  %<space> = <space>
			 *  %! = !
			 *  %% = %
			 *  %. = .
			 */
			if( ( ( message_string->string )[ message_string_index + 1 ] == (system_character_t) ' ' )
			 || ( ( message_string->string )[ message_string_index + 1 ] == (system_character_t) '!' )
			 || ( ( message_string->string )[ message_string_index + 1 ] == (system_character_t) '%' )
			 || ( ( message_string->string )[ message_string_index + 1 ] == (system_character_t) '.' ) )
			{
				last_character = ( message_string->string )[ message_string_index + 1 ];

				fprintf(
				 stream,
				 "%c",
				 last_character );

				message_string_index += 2;

				continue;
			}
			/* Replace %b = <space> */
			if( ( message_string->string )[ message_string_index + 1 ] == (system_character_t) 'b' )
			{
				last_character = (system_character_t) ' ';

				fprintf(
				 stream,
				 "%c",
				 last_character );

				message_string_index += 2;

				continue;
			}
			/* Replace %n = <new line> */
			if( ( message_string->string )[ message_string_index + 1 ] == (system_character_t) 'n' )
			{
				if( last_character != (system_character_t) '\n' )
				{
					last_character = (system_character_t) '\n';

					fprintf(
					 stream,
					 "%c",
					 last_character );
				}
				message_string_index += 2;

				continue;
			}
			/* Replace %t = <tab> */
			if( ( message_string->string )[ message_string_index + 1 ] == (system_character_t) 't' )
			{
				last_character = (system_character_t) '\t';

				fprintf(
				 stream,
				 "%c",
				 last_character );

				message_string_index += 2;

				continue;
			}
			if( ( ( message_string->string )[ message_string_index + 1 ] < (system_character_t) '1' )
			 || ( ( message_string->string )[ message_string_index + 1 ] > (system_character_t) '9' ) )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
				 "%s: unsupported conversion specifier: %" PRIs_SYSTEM ".",
				 function,
				 &( ( message_string->string )[ message_string_index ] ) );

				goto on_error;
			}
			value_string_index = (int) ( message_string->string )[ message_string_index + 1 ] - (int) '0';

			conversion_specifier_length = 2;

		 	if( ( ( message_string_index + 3 ) < message_string_length )
			 && ( ( message_string->string )[ message_string_index + 2 ] >= (system_character_t) '0' )
			 && ( ( message_string->string )[ message_string_index + 2 ] <= (system_character_t) '9' ) )
			{
				value_string_index *= 10;
				value_string_index += (int) ( message_string->string )[ message_string_index + 2 ] - (int) '0';

				conversion_specifier_length += 1;
			}
			value_string_index -= 1;

		 	if( ( ( message_string_index + conversion_specifier_length + 3 ) < message_string_length )
			 && ( ( message_string->string )[ message_string_index + conversion_specifier_length ] == (system_character_t) '!' ) )
			{
				if( ( ( message_string->string )[ message_string_index + conversion_specifier_length + 1 ] != (system_character_t) 's' )
				 || ( ( message_string->string )[ message_string_index + conversion_specifier_length + 2 ] != (system_character_t) '!' ) )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
					 "%s: unsupported conversion specifier: %" PRIs_SYSTEM ".",
					 function,
					 &( ( message_string->string )[ message_string_index ] ) );

					goto on_error;
				}
				conversion_specifier_length += 3;
			}
/* TODO remove index check after user data support */
			if( value_string_index < number_of_strings )
			{
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
					 stream,
					 "%" PRIs_SYSTEM "",
					 value_string );

					memory_free(
					 value_string );

					value_string = NULL;
				}
				message_string_index += conversion_specifier_length;
			}
			else
			{
				do
				{
					fprintf(
					 stream,
					 "%" PRIc_SYSTEM "",
					 ( message_string->string )[ message_string_index++ ] );

					conversion_specifier_length--;
				}
				while( conversion_specifier_length > 0 );

				last_character = ( message_string->string )[ message_string_index ];
			}
		}
		else
		{
			if( ( message_string->string )[ message_string_index ] != 0 )
			{
				if( ( message_string->string )[ message_string_index ] == (system_character_t) '\r' )
				{
					/* Ignore \r characters */
				}
				else if( ( ( message_string->string )[ message_string_index ] == (system_character_t) '\n' )
				      && ( last_character == (system_character_t) '\n' ) )
				{
					/* Ignore multiple \n characters */
				}
				else
				{
					fprintf(
					 stream,
					 "%" PRIc_SYSTEM "",
					 ( message_string->string )[ message_string_index ] );

					last_character = ( message_string->string )[ message_string_index ];
				}
			}
			message_string_index += 1;
		}
	}
	fprintf(
	 stream,
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

