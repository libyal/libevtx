/*
 * Binary XML token functions
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
#include <byte_stream.h>
#include <memory.h>
#include <types.h>

#include "libevtx_binary_xml_token.h"
#include "libevtx_definitions.h"
#include "libevtx_libcerror.h"
#include "libevtx_libcnotify.h"

/* Initialize binary XML token
 * Make sure the value binary XML token is pointing to is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libevtx_binary_xml_token_initialize(
     libevtx_binary_xml_token_t **binary_xml_token,
     libcerror_error_t **error )
{
	static char *function = "libevtx_binary_xml_token_initialize";

	if( binary_xml_token == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid binary XML token.",
		 function );

		return( -1 );
	}
	if( *binary_xml_token != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid binary XML token value already set.",
		 function );

		return( -1 );
	}
	*binary_xml_token = memory_allocate_structure(
	                     libevtx_binary_xml_token_t );

	if( *binary_xml_token == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create binary XML token.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *binary_xml_token,
	     0,
	     sizeof( libevtx_binary_xml_token_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear binary XML token.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *binary_xml_token != NULL )
	{
		memory_free(
		 *binary_xml_token );

		*binary_xml_token = NULL;
	}
	return( -1 );
}

/* Frees binary XML token
 * Returns 1 if successful or -1 on error
 */
int libevtx_binary_xml_token_free(
     libevtx_binary_xml_token_t **binary_xml_token,
     libcerror_error_t **error )
{
	static char *function = "libevtx_binary_xml_token_free";
	int result            = 1;

	if( binary_xml_token == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid binary XML token.",
		 function );

		return( -1 );
	}
	if( *binary_xml_token != NULL )
	{
		memory_free(
		 *binary_xml_token );

		*binary_xml_token = NULL;
	}
	return( result );
}

/* Reads the binary XML token
 * Returns 1 if successful or -1 on error
 */
int libevtx_binary_xml_token_read(
     libevtx_binary_xml_token_t *binary_xml_token,
     libevtx_io_handle_t *io_handle,
     const uint8_t *chunk_data,
     size_t chunk_data_size,
     size_t chunk_data_offset,
     libcerror_error_t **error )
{
	const uint8_t *binary_xml_token_data = NULL;
	static char *function                = "libevtx_binary_xml_token_read";
	size_t binary_xml_token_data_size    = 0;

	if( binary_xml_token == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid binary XML token.",
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
		 "%s: invalid binary XML token data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( chunk_data_offset >= chunk_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid chunk data offset value out of bounds.",
		 function );

		return( -1 );
	}
	binary_xml_token_data      = &( chunk_data[ chunk_data_offset ] );
	binary_xml_token_data_size = chunk_data_size - chunk_data_offset;

	if( binary_xml_token_data_size < 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid binary XML token data size value too small.",
		 function );

		return( -1 );
	}
	switch( binary_xml_token_data[ 0 ] )
	{
		case LIBEVTX_BINARY_XML_TOKEN_END_OF_FILE:
		case LIBEVTX_BINARY_XML_TOKEN_CLOSE_START_ELEMENT_TAG:
		case LIBEVTX_BINARY_XML_TOKEN_CLOSE_EMPTY_ELEMENT_TAG:
		case LIBEVTX_BINARY_XML_TOKEN_END_ELEMENT_TAG:
		case LIBEVTX_BINARY_XML_TOKEN_OPEN_START_ELEMENT_TAG:
		case LIBEVTX_BINARY_XML_TOKEN_OPEN_START_ELEMENT_TAG | LIBEVTX_BINARY_XML_TOKEN_FLAG_HAS_MORE_DATA:
		case LIBEVTX_BINARY_XML_TOKEN_VALUE:
		case LIBEVTX_BINARY_XML_TOKEN_VALUE | LIBEVTX_BINARY_XML_TOKEN_FLAG_HAS_MORE_DATA:
		case LIBEVTX_BINARY_XML_TOKEN_ATTRIBUTE:
		case LIBEVTX_BINARY_XML_TOKEN_ATTRIBUTE | LIBEVTX_BINARY_XML_TOKEN_FLAG_HAS_MORE_DATA:
		case LIBEVTX_BINARY_XML_TOKEN_CDATA_SECTION:
		case LIBEVTX_BINARY_XML_TOKEN_CDATA_SECTION | LIBEVTX_BINARY_XML_TOKEN_FLAG_HAS_MORE_DATA:
		case LIBEVTX_BINARY_XML_TOKEN_CHARACTER_REFERENCE:
		case LIBEVTX_BINARY_XML_TOKEN_CHARACTER_REFERENCE | LIBEVTX_BINARY_XML_TOKEN_FLAG_HAS_MORE_DATA:
		case LIBEVTX_BINARY_XML_TOKEN_ENTITY_REFERENCE:
		case LIBEVTX_BINARY_XML_TOKEN_ENTITY_REFERENCE | LIBEVTX_BINARY_XML_TOKEN_FLAG_HAS_MORE_DATA:
		case LIBEVTX_BINARY_XML_TOKEN_PI_TARGET:
		case LIBEVTX_BINARY_XML_TOKEN_PI_DATA:
		case LIBEVTX_BINARY_XML_TOKEN_TEMPLATE_INSTANCE:
		case LIBEVTX_BINARY_XML_TOKEN_NORMAL_SUBSTITUTION:
		case LIBEVTX_BINARY_XML_TOKEN_OPTIONAL_SUBSTITUTION:
		case LIBEVTX_BINARY_XML_TOKEN_FRAGMENT_HEADER:
			break;

		default:
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported binary XML token type: 0x%02" PRIx8 ".",
			 function,
			 binary_xml_token_data[ 0 ] );

			return( -1 );
	}
	binary_xml_token->type = binary_xml_token_data[ 0 ];

	return( 1 );
}

