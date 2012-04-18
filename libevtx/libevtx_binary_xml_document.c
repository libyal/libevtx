/*
 * Binary XML document functions
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
#include <byte_stream.h>
#include <memory.h>
#include <types.h>

#include "libevtx_binary_xml_document.h"
#include "libevtx_binary_xml_template_value.h"
#include "libevtx_binary_xml_token.h"
#include "libevtx_debug.h"
#include "libevtx_definitions.h"
#include "libevtx_libcerror.h"
#include "libevtx_libcnotify.h"
#include "libevtx_libfvalue.h"
#include "libevtx_libuna.h"
#include "libevtx_xml_tag.h"

/* TODO only used for value format flags definitions */
#include "libevtx_libfdatetime.h"
#include "libevtx_libfguid.h"

/* Initialize binary XML document
 * Make sure the value binary XML document is pointing to is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libevtx_binary_xml_document_initialize(
     libevtx_binary_xml_document_t **binary_xml_document,
     libcerror_error_t **error )
{
	static char *function = "libevtx_binary_xml_document_initialize";

	if( binary_xml_document == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid binary XML document.",
		 function );

		return( -1 );
	}
	if( *binary_xml_document != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid binary XML document value already set.",
		 function );

		return( -1 );
	}
	*binary_xml_document = memory_allocate_structure(
	                        libevtx_binary_xml_document_t );

	if( *binary_xml_document == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create binary XML document.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *binary_xml_document,
	     0,
	     sizeof( libevtx_binary_xml_document_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear binary XML document.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *binary_xml_document != NULL )
	{
		memory_free(
		 *binary_xml_document );

		*binary_xml_document = NULL;
	}
	return( -1 );
}

/* Frees binary XML document
 * Returns 1 if successful or -1 on error
 */
int libevtx_binary_xml_document_free(
     libevtx_binary_xml_document_t **binary_xml_document,
     libcerror_error_t **error )
{
	static char *function = "libevtx_binary_xml_document_free";
	int result            = 1;

	if( binary_xml_document == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid binary XML document.",
		 function );

		return( -1 );
	}
	if( *binary_xml_document != NULL )
	{
		if( ( *binary_xml_document )->root_xml_tag != NULL )
		{
			if( libevtx_xml_tag_free(
			     &( ( *binary_xml_document )->root_xml_tag ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free root XML tag.",
				 function );

				result = -1;
			}
		}
		memory_free(
		 *binary_xml_document );

		*binary_xml_document = NULL;
	}
	return( result );
}

/* Clones the binary XML document
 * Returns 1 if successful or -1 on error
 */
int libevtx_binary_xml_document_clone(
     libevtx_binary_xml_document_t **destination_binary_xml_document,
     libevtx_binary_xml_document_t *source_binary_xml_document,
     libcerror_error_t **error )
{
	static char *function = "libevtx_binary_xml_document_free";

	if( destination_binary_xml_document == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination binary XML document.",
		 function );

		return( -1 );
	}
	if( *destination_binary_xml_document != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid destination binary XML document value already set.",
		 function );

		return( -1 );
	}
	if( source_binary_xml_document == NULL )
	{
		*destination_binary_xml_document = NULL;

		return( 1 );
	}
	/* TODO clone tags */

	return( 1 );

on_error:
	if( *destination_binary_xml_document != NULL )
	{
		libevtx_binary_xml_document_free(
		 destination_binary_xml_document,
		 NULL );
	}
	return( -1 );
}

/* Reads a binary XML document
 * Returns 1 if successful or -1 on error
 */
int libevtx_binary_xml_document_read(
     libevtx_binary_xml_document_t *binary_xml_document,
     libevtx_io_handle_t *io_handle,
     const uint8_t *chunk_data,
     size_t chunk_data_size,
     size_t chunk_data_offset,
     libcerror_error_t **error )
{
	libevtx_binary_xml_token_t *binary_xml_token = NULL;
	static char *function                        = "libevtx_binary_xml_document_read";

	if( binary_xml_document == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid binary XML document.",
		 function );

		return( -1 );
	}
	if( binary_xml_document->root_xml_tag != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid binary XML document - root XML tag already set.",
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
		 "%s: invalid binary XML document data size value exceeds maximum.",
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
	if( libevtx_binary_xml_token_initialize(
	     &binary_xml_token,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create binary XML token.",
		 function );

		goto on_error;
	}
	while( chunk_data_offset < chunk_data_size )
	{
		if( libevtx_binary_xml_token_read(
		     binary_xml_token,
		     io_handle,
		     chunk_data,
		     chunk_data_size,
		     chunk_data_offset,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read binary XML token.",
			 function );

			goto on_error;
		}
/* TODO validate the order */
		switch( binary_xml_token->type & 0xbf )
		{
			case LIBEVTX_BINARY_XML_TOKEN_END_OF_FILE:
				if( ( chunk_data_offset + 1 ) > chunk_data_size )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
					 "%s: invalid binary XML document data size value too small.",
					 function );

					goto on_error;
				}
#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: end of file data:\n",
					 function );
					libcnotify_print_data(
					 &( chunk_data[ chunk_data_offset ] ),
					 1,
					 0 );
				}
#endif
#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: type\t\t\t\t\t: 0x%02" PRIx8 "\n",
					 function,
					 chunk_data[ chunk_data_offset ] );

					libcnotify_printf(
					 "\n" );
				}
#endif
				binary_xml_token->size = 1;

				break;

			case LIBEVTX_BINARY_XML_TOKEN_FRAGMENT_HEADER:
				if( libevtx_binary_xml_document_read_fragment(
				     binary_xml_document,
				     binary_xml_token,
				     io_handle,
				     chunk_data,
				     chunk_data_size,
				     chunk_data_offset,
				     binary_xml_document->root_xml_tag,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read fragment header.",
					 function );

					goto on_error;
				}
				break;

			default:
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
				 "%s: invalid binary XML token - unsupported type: 0x%02" PRIx8 ".",
				 function,
				 binary_xml_token->type );

				goto on_error;
		}
		binary_xml_document->size += binary_xml_token->size;
		chunk_data_offset         += binary_xml_token->size;

		if( binary_xml_token->type == LIBEVTX_BINARY_XML_TOKEN_END_OF_FILE )
		{
			break;
		}
	}
	if( libevtx_binary_xml_token_free(
	     &binary_xml_token,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free binary XML token.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( binary_xml_token != NULL )
	{
		libevtx_binary_xml_token_free(
		 &binary_xml_token,
		 NULL );
	}
	return( -1 );
}

/* Reads an attribute from a binary XML document
 * Returns 1 if successful or -1 on error
 */
int libevtx_binary_xml_document_read_attribute(
     libevtx_binary_xml_document_t *binary_xml_document,
     libevtx_binary_xml_token_t *binary_xml_token,
     libevtx_io_handle_t *io_handle,
     const uint8_t *chunk_data,
     size_t chunk_data_size,
     size_t chunk_data_offset,
     libevtx_array_t *template_values_array,
     libevtx_xml_tag_t *xml_tag,
     libcerror_error_t **error )
{
	libevtx_binary_xml_token_t *binary_xml_sub_token = NULL;
	libevtx_xml_tag_t *attribute_xml_tag             = NULL;
	const uint8_t *binary_xml_document_data          = NULL;
	static char *function                            = "libevtx_binary_xml_document_read_attribute";
	size_t binary_xml_document_data_offset           = 0;
	size_t binary_xml_document_data_size             = 0;
	size_t trailing_data_size                        = 0;
	size_t template_value_offset                     = 0;
	uint32_t attribute_name_offset                   = 0;
	uint32_t attribute_name_size                     = 0;
	int result                                       = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint32_t value_32bit                             = 0;
	uint16_t value_16bit                             = 0;
#endif

	if( binary_xml_document == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid binary XML document.",
		 function );

		return( -1 );
	}
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
	if( ( binary_xml_token->type & 0xbf ) != LIBEVTX_BINARY_XML_TOKEN_ATTRIBUTE )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid binary XML token - unsupported type: 0x%02" PRIx8 ".",
		 function,
		 binary_xml_token->type );

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
		 "%s: invalid binary XML document data size value exceeds maximum.",
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
	binary_xml_document_data      = &( chunk_data[ chunk_data_offset ] );
	binary_xml_document_data_size = chunk_data_size - chunk_data_offset;

	if( binary_xml_document_data_size < 5 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid binary XML document data size value too small.",
		 function );

		goto on_error;
	}
	if( libevtx_binary_xml_token_initialize(
	     &binary_xml_sub_token,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create binary XML sub token.",
		 function );

		goto on_error;
	}
	do
	{
		if( libevtx_xml_tag_initialize(
		     &attribute_xml_tag,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create attribute XML tag.",
			 function );

			goto on_error;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: attribute data:\n",
			 function );
			libcnotify_print_data(
			 binary_xml_document_data,
			 5,
			 0 );
		}
#endif
		byte_stream_copy_to_uint32_little_endian(
		 &( binary_xml_document_data[ 1 ] ),
		 attribute_name_offset );

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: type\t\t\t: 0x%02" PRIx8 "\n",
			 function,
			 binary_xml_document_data[ 0 ] );

			libcnotify_printf(
			 "%s: name offset\t\t\t: 0x%08" PRIx32 "\n",
			 function,
			 attribute_name_offset );

			libcnotify_printf(
			 "\n" );
		}
#endif
		binary_xml_document_data_offset = 5;

		if( attribute_name_offset > ( chunk_data_offset + binary_xml_document_data_offset ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid attribute data offset value out of bounds.",
			 function );

			goto on_error;
		}
		if( ( chunk_data_offset + binary_xml_document_data_offset ) < attribute_name_offset )
		{
			trailing_data_size = attribute_name_offset - ( chunk_data_offset - binary_xml_document_data_offset );

#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: trailing data:\n",
				 function );
				libcnotify_print_data(
				 &( binary_xml_document_data[ binary_xml_document_data_offset ] ),
				 trailing_data_size,
				 0 );
			}
#endif
			binary_xml_document_data_offset += trailing_data_size;
		}
		if( ( attribute_name_offset + 8 ) > chunk_data_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid binary XML document data size value too small.",
			 function );

			goto on_error;
		}
		byte_stream_copy_to_uint16_little_endian(
		 &( chunk_data[ attribute_name_offset + 6 ] ),
		 attribute_name_size );

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			byte_stream_copy_to_uint32_little_endian(
			 &( chunk_data[ attribute_name_offset ] ),
			 value_32bit );
			libcnotify_printf(
			 "%s: name unknown1\t\t: 0x%08" PRIx32 "\n",
			 function,
			 value_32bit );

			byte_stream_copy_to_uint16_little_endian(
			 &( chunk_data[ attribute_name_offset + 4 ] ),
			 value_16bit );
			libcnotify_printf(
			 "%s: name hash\t\t\t: 0x%04" PRIx16 "\n",
			 function,
			 value_16bit );

			libcnotify_printf(
			 "%s: name number of characters\t: %" PRIu16 "\n",
			 function,
			 attribute_name_size );
		}
#endif
		attribute_name_size += 1;
		attribute_name_size *= 2;

		if( ( attribute_name_offset + 8 + attribute_name_size ) > chunk_data_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid attribute name size value out of bounds.",
			 function );

			goto on_error;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: name data:\n",
			 function );
			libcnotify_print_data(
			 &( chunk_data[ attribute_name_offset + 8 ] ),
			 attribute_name_size,
			 0 );
		}
#endif
		if( libfvalue_value_initialize(
		     &( attribute_xml_tag->name ),
		     LIBFVALUE_VALUE_TYPE_STRING_UTF16,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create name value.",
			 function );

			goto on_error;
		}
		if( libfvalue_value_set_data(
		     attribute_xml_tag->name,
		     &( chunk_data[ attribute_name_offset + 8 ] ),
		     attribute_name_size,
		     LIBFVALUE_ENDIAN_LITTLE,
		     LIBFVALUE_VALUE_DATA_FLAG_NON_MANAGED,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set name value data.",
			 function );

			goto on_error;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: name\t\t\t: ",
			 function );

			if( libfvalue_debug_print_value(
			     attribute_xml_tag->name,
			     0,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
				 "%s: unable to print name value.",
				 function );

				goto on_error;
			}
			libcnotify_printf(
			 "\n" );

			libcnotify_printf(
			 "\n" );
		}
#endif
		if( ( chunk_data_offset + binary_xml_document_data_offset ) == attribute_name_offset )
		{
			binary_xml_document_data_offset += 8 + attribute_name_size;
		}
		if( libevtx_binary_xml_token_read(
		     binary_xml_sub_token,
		     io_handle,
		     chunk_data,
		     chunk_data_size,
		     chunk_data_offset + binary_xml_document_data_offset,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read binary XML sub token.",
			 function );

			goto on_error;
		}
		result = 1;

		switch( binary_xml_sub_token->type & 0xbf )
		{
			case LIBEVTX_BINARY_XML_TOKEN_VALUE:
				if( template_value_offset != 0 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
					 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
					 "%s: invalid template value offset value out of bounds.",
					 function );

					goto on_error;
				}
				if( libevtx_binary_xml_document_read_value(
				     binary_xml_document,
				     binary_xml_sub_token,
				     chunk_data,
				     chunk_data_size,
				     chunk_data_offset + binary_xml_document_data_offset,
				     attribute_xml_tag,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read value.",
					 function );

					goto on_error;
				}
				break;

			case LIBEVTX_BINARY_XML_TOKEN_NORMAL_SUBSTITUTION:
				result = libevtx_binary_xml_document_read_normal_substitution(
				          binary_xml_document,
				          binary_xml_sub_token,
				          io_handle,
				          chunk_data,
				          chunk_data_size,
				          chunk_data_offset + binary_xml_document_data_offset,
				          template_values_array,
				          &template_value_offset,
				          attribute_xml_tag,
				          error );

				if( result == -1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read normal substitution.",
					 function );

					goto on_error;
				}
				break;

			case LIBEVTX_BINARY_XML_TOKEN_OPTIONAL_SUBSTITUTION:
				result = libevtx_binary_xml_document_read_optional_substitution(
					  binary_xml_document,
					  binary_xml_sub_token,
					  io_handle,
					  chunk_data,
					  chunk_data_size,
					  chunk_data_offset + binary_xml_document_data_offset,
					  template_values_array,
					  &template_value_offset,
					  attribute_xml_tag,
					  error );

				if( result == -1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read optional substitution.",
					 function );

					goto on_error;
				}
				break;

			default:
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
				 "%s: invalid binary XML sub token - unsupported type: 0x%02" PRIx8 ".",
				 function,
				 binary_xml_sub_token->type );

				goto on_error;
		}
		if( result != 0 )
		{
			if( libevtx_xml_tag_append_attribute(
			     xml_tag,
			     attribute_xml_tag,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable to append attribute to XML tag.",
				 function );

				goto on_error;
			}
			attribute_xml_tag = NULL;
		}
		binary_xml_document_data_offset += binary_xml_sub_token->size;
	}
	while( template_value_offset > 0 );

	binary_xml_token->size = binary_xml_document_data_offset;

	if( attribute_xml_tag != NULL )
	{
		if( libevtx_xml_tag_free(
		     &attribute_xml_tag,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free attribute XML tag.",
			 function );

			goto on_error;
		}
	}
	if( libevtx_binary_xml_token_free(
	     &binary_xml_sub_token,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free binary XML sub token.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( attribute_xml_tag != NULL )
	{
		libevtx_xml_tag_free(
		 &attribute_xml_tag,
		 NULL );
	}
	if( binary_xml_sub_token != NULL )
	{
		libevtx_binary_xml_token_free(
		 &binary_xml_sub_token,
		 NULL );
	}
	return( -1 );
}

/* Reads an element from a binary XML document
 * Returns 1 if successful or -1 on error
 */
int libevtx_binary_xml_document_read_element(
     libevtx_binary_xml_document_t *binary_xml_document,
     libevtx_binary_xml_token_t *binary_xml_token,
     libevtx_io_handle_t *io_handle,
     const uint8_t *chunk_data,
     size_t chunk_data_size,
     size_t chunk_data_offset,
     libevtx_array_t *template_values_array,
     libevtx_xml_tag_t *xml_tag,
     libcerror_error_t **error )
{
	libevtx_binary_xml_token_t *binary_xml_sub_token = NULL;
	libevtx_xml_tag_t *element_xml_tag               = NULL;
	const uint8_t *binary_xml_document_data          = NULL;
	static char *function                            = "libevtx_binary_xml_document_read_element";
	size_t binary_xml_document_data_offset           = 0;
	size_t binary_xml_document_data_size             = 0;
	size_t trailing_data_size                        = 0;
	size_t template_value_offset                     = 0;
	uint32_t attribute_list_size                     = 0;
	uint32_t element_name_offset                     = 0;
	uint32_t element_name_size                       = 0;
	uint32_t element_size                            = 0;
	int result                                       = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint32_t value_32bit                             = 0;
	uint16_t value_16bit                             = 0;
#endif

	if( binary_xml_document == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid binary XML document.",
		 function );

		return( -1 );
	}
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
	if( ( binary_xml_token->type & 0xbf ) != LIBEVTX_BINARY_XML_TOKEN_OPEN_START_ELEMENT_TAG )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid binary XML token - unsupported type: 0x%02" PRIx8 ".",
		 function,
		 binary_xml_token->type );

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
		 "%s: invalid binary XML document data size value exceeds maximum.",
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
	binary_xml_document_data      = &( chunk_data[ chunk_data_offset ] );
	binary_xml_document_data_size = chunk_data_size - chunk_data_offset;

	if( binary_xml_document_data_size < 11 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid binary XML document data size value too small.",
		 function );

		return( -1 );
	}
	if( libevtx_binary_xml_token_initialize(
	     &binary_xml_sub_token,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create binary XML sub token.",
		 function );

		goto on_error;
	}
	do
	{
		if( libevtx_xml_tag_initialize(
		     &element_xml_tag,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create element XML tag.",
			 function );

			goto on_error;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: element data:\n",
			 function );
			libcnotify_print_data(
			 binary_xml_document_data,
			 11,
			 0 );
		}
#endif
		byte_stream_copy_to_uint32_little_endian(
		 &( binary_xml_document_data[ 3 ] ),
		 element_size );

		byte_stream_copy_to_uint32_little_endian(
		 &( binary_xml_document_data[ 7 ] ),
		 element_name_offset );

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: type\t\t\t\t: 0x%02" PRIx8 "\n",
			 function,
			 binary_xml_document_data[ 0 ] );

			byte_stream_copy_to_uint16_little_endian(
			 &( binary_xml_document_data[ 1 ] ),
			 value_16bit );
			libcnotify_printf(
			 "%s: dependency identifier\t\t: %" PRIi16 " (0x%04" PRIx16 ")\n",
			 function,
			 (int16_t) value_16bit,
			 value_16bit );

			libcnotify_printf(
			 "%s: size\t\t\t\t: %" PRIu32 "\n",
			 function,
			 element_size );

			libcnotify_printf(
			 "%s: name offset\t\t\t: 0x%08" PRIx32 "\n",
			 function,
			 element_name_offset );

			libcnotify_printf(
			 "\n" );
		}
#endif
		binary_xml_document_data_offset = 11;

		/* The first 7 bytes are not included in the element size
		 */
		element_size -= 4;

		if( element_name_offset > ( chunk_data_offset + binary_xml_document_data_offset ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid element data offset value out of bounds.",
			 function );

			goto on_error;
		}
		if( ( chunk_data_offset + binary_xml_document_data_offset ) < element_name_offset )
		{
			trailing_data_size = element_name_offset - ( chunk_data_offset + binary_xml_document_data_offset );

#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: trailing data:\n",
				 function );
				libcnotify_print_data(
				 &( binary_xml_document_data[ binary_xml_document_data_offset ] ),
				 trailing_data_size,
				 0 );
			}
#endif
			binary_xml_document_data_offset += trailing_data_size;
			element_size                    -= trailing_data_size;
		}
		if( ( element_name_offset + 8 ) > chunk_data_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid binary XML document data size value too small.",
			 function );

			goto on_error;
		}
		byte_stream_copy_to_uint16_little_endian(
		 &( chunk_data[ element_name_offset + 6 ] ),
		 element_name_size );

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			byte_stream_copy_to_uint32_little_endian(
			 &( chunk_data[ element_name_offset ] ),
			 value_32bit );
			libcnotify_printf(
			 "%s: name unknown1\t\t\t: 0x%08" PRIx32 "\n",
			 function,
			 value_32bit );

			byte_stream_copy_to_uint16_little_endian(
			 &( chunk_data[ element_name_offset + 4 ] ),
			 value_16bit );
			libcnotify_printf(
			 "%s: name hash\t\t\t: 0x%04" PRIx16 "\n",
			 function,
			 value_16bit );

			libcnotify_printf(
			 "%s: name number of characters\t: %" PRIu16 "\n",
			 function,
			 element_name_size );
		}
#endif
		element_name_size += 1;
		element_name_size *= 2;

		if( ( element_name_offset + 8 + element_name_size ) > chunk_data_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid element name size value out of bounds.",
			 function );

			goto on_error;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: name data:\n",
			 function );
			libcnotify_print_data(
			 &( chunk_data[ element_name_offset + 8 ] ),
			 element_name_size,
			 0 );
		}
#endif
		if( libfvalue_value_initialize(
		     &( element_xml_tag->name ),
		     LIBFVALUE_VALUE_TYPE_STRING_UTF16,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create name value.",
			 function );

			goto on_error;
		}
		if( libfvalue_value_set_data(
		     element_xml_tag->name,
		     &( chunk_data[ element_name_offset + 8 ] ),
		     element_name_size,
		     LIBFVALUE_ENDIAN_LITTLE,
		     LIBFVALUE_VALUE_DATA_FLAG_NON_MANAGED,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set name value data.",
			 function );

			goto on_error;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: name\t\t\t\t: ",
			 function );

			if( libfvalue_debug_print_value(
			     element_xml_tag->name,
			     0,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
				 "%s: unable to print name value.",
				 function );

				goto on_error;
			}
			libcnotify_printf(
			 "\n" );

			libcnotify_printf(
			 "\n" );
		}
#endif
		if( ( chunk_data_offset + binary_xml_document_data_offset ) == element_name_offset )
		{
			binary_xml_document_data_offset += 8 + element_name_size;
			element_size                    -= 8 + element_name_size;
		}
		if( ( binary_xml_token->type & LIBEVTX_BINARY_XML_TOKEN_FLAG_HAS_MORE_DATA ) != 0 )
		{
			if( ( chunk_data_offset + binary_xml_document_data_offset + 4 ) > chunk_data_size )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: invalid binary XML document data size value too small.",
				 function );

				goto on_error;
			}
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: attribute list data:\n",
				 function );
				libcnotify_print_data(
				 &( binary_xml_document_data[ binary_xml_document_data_offset ] ),
				 4,
				 0 );
			}
#endif
			byte_stream_copy_to_uint32_little_endian(
			 &( binary_xml_document_data[ binary_xml_document_data_offset ] ),
			 attribute_list_size );

#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: attribute list size\t\t: %" PRIu32 "\n",
				 function,
				 attribute_list_size );

				libcnotify_printf(
				 "\n" );
			}
#endif
			binary_xml_document_data_offset += 4;
			element_size                    -= 4;

			while( attribute_list_size > 0 )
			{
				if( libevtx_binary_xml_token_read(
				     binary_xml_sub_token,
				     io_handle,
				     chunk_data,
				     chunk_data_size,
				     chunk_data_offset + binary_xml_document_data_offset,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read binary XML sub token.",
					 function );

					goto on_error;
				}
				if( libevtx_binary_xml_document_read_attribute(
				     binary_xml_document,
				     binary_xml_sub_token,
				     io_handle,
				     chunk_data,
				     chunk_data_size,
				     chunk_data_offset + binary_xml_document_data_offset,
				     template_values_array,
				     element_xml_tag,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read attribute.",
					 function );

					goto on_error;
				}
				binary_xml_document_data_offset += binary_xml_sub_token->size;
				element_size                    -= binary_xml_sub_token->size;

				if( attribute_list_size < binary_xml_sub_token->size )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
					 "%s: invalid attribute list size value too small.",
					 function );

					goto on_error;
				}
				attribute_list_size -= binary_xml_sub_token->size;
			}
		}
		if( libevtx_binary_xml_token_read(
		     binary_xml_sub_token,
		     io_handle,
		     chunk_data,
		     chunk_data_size,
		     chunk_data_offset + binary_xml_document_data_offset,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read binary XML sub token.",
			 function );

			goto on_error;
		}
		if( ( binary_xml_sub_token->type != LIBEVTX_BINARY_XML_TOKEN_CLOSE_START_ELEMENT_TAG )
		 && ( binary_xml_sub_token->type != LIBEVTX_BINARY_XML_TOKEN_CLOSE_EMPTY_ELEMENT_TAG ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: invalid binary XML token - unsupported type: 0x%02" PRIx8 ".",
			 function,
			 binary_xml_token->type );

			return( -1 );
		}
		if( ( chunk_data_offset + binary_xml_document_data_offset + 1 ) > chunk_data_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid binary XML document data size value too small.",
			 function );

			goto on_error;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: close element tag data:\n",
			 function );
			libcnotify_print_data(
			 &( binary_xml_document_data[ binary_xml_document_data_offset ] ),
			 1,
			 0 );
		}
#endif
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: type\t\t\t\t: 0x%02" PRIx8 "\n",
			 function,
			 binary_xml_document_data[ binary_xml_document_data_offset ] );

			libcnotify_printf(
			 "\n" );
		}
#endif
		binary_xml_document_data_offset += 1;
		element_size                    -= 1;

		if( binary_xml_sub_token->type == LIBEVTX_BINARY_XML_TOKEN_CLOSE_START_ELEMENT_TAG )
		{
			result = 1;

			while( element_size > 0 )
			{
				if( libevtx_binary_xml_token_read(
				     binary_xml_sub_token,
				     io_handle,
				     chunk_data,
				     chunk_data_size,
				     chunk_data_offset + binary_xml_document_data_offset,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read binary XML sub token.",
					 function );

					goto on_error;
				}
				switch( binary_xml_sub_token->type & 0xbf )
				{
					case LIBEVTX_BINARY_XML_TOKEN_OPEN_START_ELEMENT_TAG:
						if( libevtx_binary_xml_document_read_element(
						     binary_xml_document,
						     binary_xml_sub_token,
						     io_handle,
						     chunk_data,
						     chunk_data_size,
						     chunk_data_offset + binary_xml_document_data_offset,
						     template_values_array,
						     element_xml_tag,
						     error ) != 1 )
						{
							libcerror_error_set(
							 error,
							 LIBCERROR_ERROR_DOMAIN_IO,
							 LIBCERROR_IO_ERROR_READ_FAILED,
							 "%s: unable to read element.",
							 function );

							goto on_error;
						}
						break;

					case LIBEVTX_BINARY_XML_TOKEN_CLOSE_EMPTY_ELEMENT_TAG:
					case LIBEVTX_BINARY_XML_TOKEN_END_ELEMENT_TAG:
						if( ( chunk_data_offset + binary_xml_document_data_offset + 1 ) > chunk_data_size )
						{
							libcerror_error_set(
							 error,
							 LIBCERROR_ERROR_DOMAIN_RUNTIME,
							 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
							 "%s: invalid binary XML document data size value too small.",
							 function );

							goto on_error;
						}
#if defined( HAVE_DEBUG_OUTPUT )
						if( libcnotify_verbose != 0 )
						{
							libcnotify_printf(
							 "%s: end element tag data:\n",
							 function );
							libcnotify_print_data(
							 &( binary_xml_document_data[ binary_xml_document_data_offset ] ),
							 1,
							 0 );
						}
#endif
#if defined( HAVE_DEBUG_OUTPUT )
						if( libcnotify_verbose != 0 )
						{
							libcnotify_printf(
							 "%s: type\t\t\t\t: 0x%02" PRIx8 "\n",
							 function,
							 binary_xml_document_data[ binary_xml_document_data_offset ] );

							libcnotify_printf(
							 "\n" );
						}
#endif
						binary_xml_sub_token->size = 1;

						break;

					case LIBEVTX_BINARY_XML_TOKEN_VALUE:
						if( template_value_offset != 0 )
						{
							libcerror_error_set(
							 error,
							 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
							 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
							 "%s: invalid template value offset value out of bounds.",
							 function );

							goto on_error;
						}
						if( libevtx_binary_xml_document_read_value(
						     binary_xml_document,
						     binary_xml_sub_token,
						     chunk_data,
						     chunk_data_size,
						     chunk_data_offset + binary_xml_document_data_offset,
						     element_xml_tag,
						     error ) != 1 )
						{
							libcerror_error_set(
							 error,
							 LIBCERROR_ERROR_DOMAIN_IO,
							 LIBCERROR_IO_ERROR_READ_FAILED,
							 "%s: unable to read value.",
							 function );

							goto on_error;
						}
						break;

					case LIBEVTX_BINARY_XML_TOKEN_NORMAL_SUBSTITUTION:
						result = libevtx_binary_xml_document_read_normal_substitution(
							  binary_xml_document,
							  binary_xml_sub_token,
							  io_handle,
							  chunk_data,
							  chunk_data_size,
							  chunk_data_offset + binary_xml_document_data_offset,
							  template_values_array,
							  &template_value_offset,
							  element_xml_tag,
							  error );

						if( result == -1 )
						{
							libcerror_error_set(
							 error,
							 LIBCERROR_ERROR_DOMAIN_IO,
							 LIBCERROR_IO_ERROR_READ_FAILED,
							 "%s: unable to read normal substitution.",
							 function );

							goto on_error;
						}
						break;

					case LIBEVTX_BINARY_XML_TOKEN_OPTIONAL_SUBSTITUTION:
						result = libevtx_binary_xml_document_read_optional_substitution(
							  binary_xml_document,
							  binary_xml_sub_token,
							  io_handle,
							  chunk_data,
							  chunk_data_size,
							  chunk_data_offset + binary_xml_document_data_offset,
							  template_values_array,
							  &template_value_offset,
							  element_xml_tag,
							  error );

						if( result == -1 )
						{
							libcerror_error_set(
							 error,
							 LIBCERROR_ERROR_DOMAIN_IO,
							 LIBCERROR_IO_ERROR_READ_FAILED,
							 "%s: unable to read optional substitution.",
							 function );

							goto on_error;
						}
						break;

					default:
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
						 "%s: invalid binary XML sub token - unsupported type: 0x%02" PRIx8 ".",
						 function,
						 binary_xml_sub_token->type );

						goto on_error;
				}
				binary_xml_document_data_offset += binary_xml_sub_token->size;

				if( element_size < binary_xml_sub_token->size )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
					 "%s: invalid element size value too small.",
					 function );

					goto on_error;
				}
				element_size -= binary_xml_sub_token->size;

				if( ( binary_xml_sub_token->type == LIBEVTX_BINARY_XML_TOKEN_CLOSE_EMPTY_ELEMENT_TAG )
				 || ( binary_xml_sub_token->type == LIBEVTX_BINARY_XML_TOKEN_END_ELEMENT_TAG ) )
				{
					break;
				}
			}
		}
		else if( binary_xml_sub_token->type == LIBEVTX_BINARY_XML_TOKEN_CLOSE_EMPTY_ELEMENT_TAG )
		{
			result = 1;
		}
		if( element_size > 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid element size value out of bounds.",
			 function );

			goto on_error;
		}
		if( result != 0 )
		{
			if( xml_tag != NULL )
			{
				if( libevtx_xml_tag_append_element(
				     xml_tag,
				     element_xml_tag,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
					 "%s: unable to append element to XML tag.",
					 function );

					goto on_error;
				}
				element_xml_tag = NULL;
			}
			else if(  binary_xml_document->root_xml_tag == NULL )
			{
				binary_xml_document->root_xml_tag = element_xml_tag;

				element_xml_tag = NULL;
			}
		}
	}
	while( template_value_offset > 0 );

	binary_xml_token->size = binary_xml_document_data_offset;

	if( element_xml_tag != NULL )
	{
		if( libevtx_xml_tag_free(
		     &element_xml_tag,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free element XML tag.",
			 function );

			goto on_error;
		}
	}
	if( libevtx_binary_xml_token_free(
	     &binary_xml_sub_token,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free binary XML sub token.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( ( element_xml_tag != NULL )
	 && ( element_xml_tag != binary_xml_document->root_xml_tag ) )
	{
		libevtx_xml_tag_free(
		 &element_xml_tag,
		 NULL );
	}
	if( binary_xml_sub_token != NULL )
	{
		libevtx_binary_xml_token_free(
		 &binary_xml_sub_token,
		 NULL );
	}
	return( -1 );
}

/* Reads a fragment from a binary XML document
 * Returns 1 if successful or -1 on error
 */
int libevtx_binary_xml_document_read_fragment(
     libevtx_binary_xml_document_t *binary_xml_document,
     libevtx_binary_xml_token_t *binary_xml_token,
     libevtx_io_handle_t *io_handle,
     const uint8_t *chunk_data,
     size_t chunk_data_size,
     size_t chunk_data_offset,
     libevtx_xml_tag_t *xml_tag,
     libcerror_error_t **error )
{
	libevtx_binary_xml_token_t *binary_xml_sub_token = NULL;
	static char *function                            = "libevtx_binary_xml_document_read_fragment";

	if( binary_xml_document == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid binary XML document.",
		 function );

		return( -1 );
	}
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
	if( libevtx_binary_xml_document_read_fragment_header(
	     binary_xml_document,
	     binary_xml_token,
	     chunk_data,
	     chunk_data_size,
	     chunk_data_offset,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read fragment header.",
		 function );

		goto on_error;
	}
	chunk_data_offset += binary_xml_token->size;

	if( libevtx_binary_xml_token_initialize(
	     &binary_xml_sub_token,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create binary XML sub token.",
		 function );

		goto on_error;
	}
	if( libevtx_binary_xml_token_read(
	     binary_xml_sub_token,
	     io_handle,
	     chunk_data,
	     chunk_data_size,
	     chunk_data_offset,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read binary XML sub token.",
		 function );

		goto on_error;
	}
	switch( binary_xml_sub_token->type & 0xbf )
	{
/* TODO what about template values array */
		case LIBEVTX_BINARY_XML_TOKEN_OPEN_START_ELEMENT_TAG:
			if( libevtx_binary_xml_document_read_element(
			     binary_xml_document,
			     binary_xml_sub_token,
			     io_handle,
			     chunk_data,
			     chunk_data_size,
			     chunk_data_offset,
			     NULL,
			     xml_tag,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read element.",
				 function );

				goto on_error;
			}
			break;

		case LIBEVTX_BINARY_XML_TOKEN_TEMPLATE_INSTANCE:
			if( libevtx_binary_xml_document_read_template_instance(
			     binary_xml_document,
			     binary_xml_sub_token,
			     io_handle,
			     chunk_data,
			     chunk_data_size,
			     chunk_data_offset,
			     xml_tag,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read document template instance.",
				 function );

				goto on_error;
			}
			break;

		default:
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: invalid binary XML token - unsupported type: 0x%02" PRIx8 ".",
			 function,
			 binary_xml_sub_token->type );

			goto on_error;
	}
	binary_xml_token->size += binary_xml_sub_token->size;

	if( libevtx_binary_xml_token_free(
	     &binary_xml_sub_token,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free binary XML sub token.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( binary_xml_sub_token != NULL )
	{
		libevtx_binary_xml_token_free(
		 &binary_xml_sub_token,
		 NULL );
	}
	return( -1 );
}

/* Reads a fragment header from a binary XML document
 * Returns 1 if successful or -1 on error
 */
int libevtx_binary_xml_document_read_fragment_header(
     libevtx_binary_xml_document_t *binary_xml_document,
     libevtx_binary_xml_token_t *binary_xml_token,
     const uint8_t *chunk_data,
     size_t chunk_data_size,
     size_t chunk_data_offset,
     libcerror_error_t **error )
{
	const uint8_t *binary_xml_document_data = NULL;
	static char *function                   = "libevtx_binary_xml_document_read_fragment_header";
	size_t binary_xml_document_data_size    = 0;

	if( binary_xml_document == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid binary XML document.",
		 function );

		return( -1 );
	}
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
	if( binary_xml_token->type != LIBEVTX_BINARY_XML_TOKEN_FRAGMENT_HEADER )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid binary XML token - unsupported type: 0x%02" PRIx8 ".",
		 function,
		 binary_xml_token->type );

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
		 "%s: invalid binary XML document data size value exceeds maximum.",
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
	binary_xml_document_data      = &( chunk_data[ chunk_data_offset ] );
	binary_xml_document_data_size = chunk_data_size - chunk_data_offset;

	if( binary_xml_document_data_size < 4 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid binary XML document data size value too small.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: fragment header data:\n",
		 function );
		libcnotify_print_data(
		 binary_xml_document_data,
		 4,
		 0 );
	}
#endif
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: type\t\t\t: 0x%02" PRIx8 "\n",
		 function,
		 binary_xml_document_data[ 0 ] );

		libcnotify_printf(
		 "%s: major version\t\t: %" PRIu8 "\n",
		 function,
		 binary_xml_document_data[ 1 ] );

		libcnotify_printf(
		 "%s: minor version\t\t: %" PRIu8 "\n",
		 function,
		 binary_xml_document_data[ 2 ] );

		libcnotify_printf(
		 "%s: flags\t\t\t: 0x%02" PRIx8 "\n",
		 function,
		 binary_xml_document_data[ 3 ] );

		libcnotify_printf(
		 "\n" );
	}
#endif
/* TODO check values */
	binary_xml_token->size = 4;
	
	return( 1 );
}

/* Reads a normal substitution from a binary XML document
 * Returns 1 if successful or -1 on error
 */
int libevtx_binary_xml_document_read_normal_substitution(
     libevtx_binary_xml_document_t *binary_xml_document,
     libevtx_binary_xml_token_t *binary_xml_token,
     libevtx_io_handle_t *io_handle,
     const uint8_t *chunk_data,
     size_t chunk_data_size,
     size_t chunk_data_offset,
     libevtx_array_t *template_values_array,
     size_t *template_value_offset,
     libevtx_xml_tag_t *xml_tag,
     libcerror_error_t **error )
{
	const uint8_t *binary_xml_document_data = NULL;
	static char *function                   = "libevtx_binary_xml_document_read_normal_substitution";
	size_t binary_xml_document_data_size    = 0;
	uint16_t template_value_index           = 0;
	uint8_t template_value_type             = 0;
	int result                              = 0;

	if( binary_xml_document == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid binary XML document.",
		 function );

		return( -1 );
	}
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
	if( binary_xml_token->type != LIBEVTX_BINARY_XML_TOKEN_NORMAL_SUBSTITUTION )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid binary XML token - unsupported type: 0x%02" PRIx8 ".",
		 function,
		 binary_xml_token->type );

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
		 "%s: invalid binary XML document data size value exceeds maximum.",
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
	binary_xml_document_data      = &( chunk_data[ chunk_data_offset ] );
	binary_xml_document_data_size = chunk_data_size - chunk_data_offset;

	if( binary_xml_document_data_size < 4 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid binary XML document data size value too small.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: normal substitution data:\n",
		 function );
		libcnotify_print_data(
		 binary_xml_document_data,
		 4,
		 0 );
	}
#endif
	byte_stream_copy_to_uint16_little_endian(
	 &( binary_xml_document_data[ 1 ] ),
	 template_value_index );

	template_value_type = binary_xml_document_data[ 3 ];

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: type\t\t: 0x%02" PRIx8 "\n",
		 function,
		 binary_xml_document_data[ 0 ] );

		libcnotify_printf(
		 "%s: identifier\t: %" PRIu16 "\n",
		 function,
		 template_value_index );

		libcnotify_printf(
		 "%s: value type\t: 0x%02" PRIx8 " (",
		 function,
		 template_value_type );
		libevtx_debug_print_value_type(
		 template_value_type );
		libcnotify_printf(
		 ")\n" );

		libcnotify_printf(
		 "\n" );
	}
#endif
	binary_xml_token->size = 4;
	chunk_data_offset     += 4;

	result = libevtx_binary_xml_document_substitute_template_value(
	          binary_xml_document,
	          io_handle,
	          chunk_data,
	          chunk_data_size,
	          template_values_array,
	          template_value_index,
	          template_value_type,
	          template_value_offset,
	          xml_tag,
	          error );

	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to substitute template value.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Reads an optional substitution from a binary XML document
 * Returns 1 if successful, 0 if no substitution or -1 on error
 */
int libevtx_binary_xml_document_read_optional_substitution(
     libevtx_binary_xml_document_t *binary_xml_document,
     libevtx_binary_xml_token_t *binary_xml_token,
     libevtx_io_handle_t *io_handle,
     const uint8_t *chunk_data,
     size_t chunk_data_size,
     size_t chunk_data_offset,
     libevtx_array_t *template_values_array,
     size_t *template_value_offset,
     libevtx_xml_tag_t *xml_tag,
     libcerror_error_t **error )
{
	const uint8_t *binary_xml_document_data = NULL;
	static char *function                   = "libevtx_binary_xml_document_read_optional_substitution";
	size_t binary_xml_document_data_size    = 0;
	uint16_t template_value_index           = 0;
	uint8_t template_value_type             = 0;
	int result                              = 0;

	if( binary_xml_document == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid binary XML document.",
		 function );

		return( -1 );
	}
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
	if( binary_xml_token->type != LIBEVTX_BINARY_XML_TOKEN_OPTIONAL_SUBSTITUTION )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid binary XML token - unsupported type: 0x%02" PRIx8 ".",
		 function,
		 binary_xml_token->type );

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
		 "%s: invalid binary XML document data size value exceeds maximum.",
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
	binary_xml_document_data      = &( chunk_data[ chunk_data_offset ] );
	binary_xml_document_data_size = chunk_data_size - chunk_data_offset;

	if( binary_xml_document_data_size < 4 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid binary XML document data size value too small.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: optional substitution data:\n",
		 function );
		libcnotify_print_data(
		 binary_xml_document_data,
		 4,
		 0 );
	}
#endif
	byte_stream_copy_to_uint16_little_endian(
	 &( binary_xml_document_data[ 1 ] ),
	 template_value_index );

	template_value_type = binary_xml_document_data[ 3 ];

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: type\t\t: 0x%02" PRIx8 "\n",
		 function,
		 binary_xml_document_data[ 0 ] );

		libcnotify_printf(
		 "%s: identifier\t: %" PRIu16 "\n",
		 function,
		 template_value_index );

		libcnotify_printf(
		 "%s: value type\t: 0x%02" PRIx8 " (",
		 function,
		 template_value_type );
		libevtx_debug_print_value_type(
		 template_value_type );
		libcnotify_printf(
		 ")\n" );

		libcnotify_printf(
		 "\n" );
	}
#endif
	binary_xml_token->size = 4;
	chunk_data_offset     += 4;

	result = libevtx_binary_xml_document_substitute_template_value(
	          binary_xml_document,
	          io_handle,
	          chunk_data,
	          chunk_data_size,
	          template_values_array,
	          template_value_index,
	          template_value_type,
	          template_value_offset,
	          xml_tag,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to substitute template value.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Reads a template instance from a binary XML document
 * Returns 1 if successful or -1 on error
 */
int libevtx_binary_xml_document_read_template_instance(
     libevtx_binary_xml_document_t *binary_xml_document,
     libevtx_binary_xml_token_t *binary_xml_token,
     libevtx_io_handle_t *io_handle,
     const uint8_t *chunk_data,
     size_t chunk_data_size,
     size_t chunk_data_offset,
     libevtx_xml_tag_t *xml_tag,
     libcerror_error_t **error )
{
	libevtx_array_t *template_values_array           = NULL;
	libevtx_binary_xml_token_t *binary_xml_sub_token = NULL;
	const uint8_t *binary_xml_document_data          = NULL;
	static char *function                            = "libevtx_binary_xml_document_read_template_instance";
	size_t binary_xml_document_data_size             = 0;
	size_t template_values_data_offset               = 0;
	size_t template_data_offset                      = 0;
	size_t template_data_size                        = 0;
	size_t trailing_data_size                        = 0;
	uint32_t template_definition_data_offset         = 0;
	uint32_t template_definition_data_size           = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint32_t value_32bit                             = 0;
#endif

	if( binary_xml_document == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid binary XML document.",
		 function );

		return( -1 );
	}
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
	if( binary_xml_token->type != LIBEVTX_BINARY_XML_TOKEN_TEMPLATE_INSTANCE )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid binary XML token - unsupported type: 0x%02" PRIx8 ".",
		 function,
		 binary_xml_token->type );

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
		 "%s: invalid binary XML document data size value exceeds maximum.",
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
	binary_xml_document_data      = &( chunk_data[ chunk_data_offset ] );
	binary_xml_document_data_size = chunk_data_size - chunk_data_offset;

	if( ( chunk_data_offset + 10 ) >= chunk_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid binary XML document data size value too small.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: template instance header data:\n",
		 function );
		libcnotify_print_data(
		 &( chunk_data[ chunk_data_offset ] ),
		 10,
		 0 );
	}
#endif
	byte_stream_copy_to_uint32_little_endian(
	 &( chunk_data[ chunk_data_offset + 6 ] ),
	 template_definition_data_offset );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: type\t\t: 0x%02" PRIx8 "\n",
		 function,
		 chunk_data[ chunk_data_offset ] );

		libcnotify_printf(
		 "%s: unknown1\t\t: %" PRIu8 "\n",
		 function,
		 chunk_data[ chunk_data_offset + 1 ] );

		byte_stream_copy_to_uint32_little_endian(
		 &( chunk_data[ chunk_data_offset + 2 ] ),
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown2\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "%s: data offset\t\t: 0x%08" PRIx32 "\n",
		 function,
		 template_definition_data_offset );

		libcnotify_printf(
		 "\n" );
	}
#endif
	binary_xml_token->size = 10;
	chunk_data_offset     += 10;

	if( template_definition_data_offset > chunk_data_offset )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid template definition data offset value out of bounds.",
		 function );

		goto on_error;
	}
	if( chunk_data_offset < template_definition_data_offset )
	{
		trailing_data_size = template_definition_data_offset - chunk_data_offset;

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: trailing data:\n",
			 function );
			libcnotify_print_data(
			 &( chunk_data[ chunk_data_offset ] ),
			 trailing_data_size,
			 0 );
		}
#endif
		binary_xml_token->size += trailing_data_size;
		chunk_data_offset      += trailing_data_size;
	}
	template_data_offset = template_definition_data_offset;

	if( ( template_data_offset + 24 ) >= chunk_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid binary XML document data size value too small.",
		 function );

		goto on_error;
	}
	byte_stream_copy_to_uint32_little_endian(
	 &( chunk_data[ template_data_offset + 20 ] ),
	 template_definition_data_size );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		byte_stream_copy_to_uint32_little_endian(
		 &( chunk_data[ template_data_offset ] ),
		 value_32bit );
		libcnotify_printf(
		 "%s: offset next\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "%s: identifier:\n",
		 function );
		libcnotify_print_data(
		 &( chunk_data[ template_data_offset + 4 ] ),
		 16,
		 0 );

		libcnotify_printf(
		 "%s: definition size\t: %" PRIu32 "\n",
		 function,
		 template_definition_data_size );

		libcnotify_printf(
		 "\n" );
	}
#endif
	if( template_data_offset == chunk_data_offset )
	{
		template_values_data_offset = 24 + template_definition_data_size;
	}
	else
	{
		template_values_data_offset = 0;
	}
	template_data_offset += 24;

	if( template_values_data_offset >= binary_xml_document_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid template values data offset value out of bounds.",
		 function );

		goto on_error;
	}
	if( libevtx_binary_xml_document_read_template_instance_values(
	     binary_xml_document,
	     binary_xml_token,
	     chunk_data,
	     chunk_data_size,
	     chunk_data_offset + template_values_data_offset,
	     &template_values_array,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read document template instance values.",
		 function );

		goto on_error;
	}
	if( libevtx_binary_xml_token_initialize(
	     &binary_xml_sub_token,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create binary XML sub token.",
		 function );

		goto on_error;
	}
	if( libevtx_binary_xml_token_read(
	     binary_xml_sub_token,
	     io_handle,
	     chunk_data,
	     chunk_data_size,
	     template_data_offset,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read binary XML sub token.",
		 function );

		goto on_error;
	}
	if( libevtx_binary_xml_document_read_fragment_header(
	     binary_xml_document,
	     binary_xml_sub_token,
	     chunk_data,
	     chunk_data_size,
	     template_data_offset,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read fragment header.",
		 function );

		goto on_error;
	}
	template_data_offset += binary_xml_sub_token->size;

	if( libevtx_binary_xml_token_read(
	     binary_xml_sub_token,
	     io_handle,
	     chunk_data,
	     chunk_data_size,
	     template_data_offset,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read binary XML sub token.",
		 function );

		goto on_error;
	}
	if( libevtx_binary_xml_document_read_element(
	     binary_xml_document,
	     binary_xml_sub_token,
	     io_handle,
	     chunk_data,
	     chunk_data_size,
	     template_data_offset,
	     template_values_array,
	     xml_tag,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read element.",
		 function );

		goto on_error;
	}
	template_data_offset += binary_xml_sub_token->size;

	if( libevtx_binary_xml_token_read(
	     binary_xml_sub_token,
	     io_handle,
	     chunk_data,
	     chunk_data_size,
	     template_data_offset,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read binary XML sub token.",
		 function );

		goto on_error;
	}
	if( binary_xml_sub_token->type != LIBEVTX_BINARY_XML_TOKEN_END_OF_FILE )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid binary XML token - unsupported type: 0x%02" PRIx8 ".",
		 function,
		 binary_xml_token->type );

		return( -1 );
	}
	if( ( chunk_data_offset + 1 ) > chunk_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid binary XML document data size value too small.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: end of file data:\n",
		 function );
		libcnotify_print_data(
		 &( chunk_data[ chunk_data_offset ] ),
		 1,
		 0 );
	}
#endif
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: type\t\t: 0x%02" PRIx8 "\n",
		 function,
		 chunk_data[ chunk_data_offset ] );

		libcnotify_printf(
		 "\n" );
	}
#endif
	template_data_offset += 1;

	if( libevtx_binary_xml_token_free(
	     &binary_xml_sub_token,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free binary XML sub token.",
		 function );

		goto on_error;
	}
	if( template_definition_data_offset == chunk_data_offset )
	{
		template_data_size = template_data_offset
		                   - template_definition_data_offset;

		binary_xml_token->size += template_data_size;
		chunk_data_offset      += template_data_size;

		/* The template data size does not include the first 33 bytes
		 * of the template definition
		 * In this case the template data size contains 24 of the 33 bytes
		 */
		if( template_definition_data_size < ( template_data_size - 24 ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid template definition data size value too small.",
			 function );

			goto on_error;
		}
		template_definition_data_size -= template_data_size - 24;
	}
/* TODO check if template_definition_data_size is 0 */

	if( libevtx_array_free(
	     &template_values_array,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libevtx_binary_xml_template_value_free,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free template values array.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( template_values_array != NULL )
	{
		libevtx_array_free(
		 &template_values_array,
		 (int (*)(intptr_t **, libcerror_error_t **)) &libevtx_binary_xml_template_value_free,
		 NULL );
	}
	if( binary_xml_sub_token != NULL )
	{
		libevtx_binary_xml_token_free(
		 &binary_xml_sub_token,
		 NULL );
	}
	return( -1 );
}

/* Reads the template instance values from a binary XML document
 * Returns 1 if successful or -1 on error
 */
int libevtx_binary_xml_document_read_template_instance_values(
     libevtx_binary_xml_document_t *binary_xml_document,
     libevtx_binary_xml_token_t *binary_xml_token,
     const uint8_t *chunk_data,
     size_t chunk_data_size,
     size_t chunk_data_offset,
     libevtx_array_t **template_values_array,
     libcerror_error_t **error )
{
	libevtx_binary_xml_template_value_t *template_value = NULL;
	static char *function                               = "libevtx_binary_xml_document_read_template_instance_values";
	size_t template_value_definitions_data_size         = 0;
	size_t template_values_data_size                    = 0;
	uint32_t number_of_template_values                  = 0;
	uint32_t template_value_index                       = 0;

	if( binary_xml_document == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid binary XML document.",
		 function );

		return( -1 );
	}
	if( ( chunk_data_offset + 4 ) >= chunk_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid chunk data offset value out of bounds.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: template instance data:\n",
		 function );
		libcnotify_print_data(
		 &( chunk_data[ chunk_data_offset ] ),
		 4,
		 0 );
	}
#endif
	byte_stream_copy_to_uint32_little_endian(
	 &( chunk_data[ chunk_data_offset ] ),
	 number_of_template_values );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: number of values\t: %" PRIu32 "\n",
		 function,
		 number_of_template_values );

		libcnotify_printf(
		 "\n" );
	}
#endif
	binary_xml_token->size += 4;
	chunk_data_offset      += 4;

	template_value_definitions_data_size = number_of_template_values * 4;

	if( ( chunk_data_offset + template_value_definitions_data_size ) >= chunk_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid chunk data offset value out of bounds.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: template instance value descriptor data:\n",
		 function );
		libcnotify_print_data(
		 &( chunk_data[ chunk_data_offset ] ),
		 template_value_definitions_data_size,
		 0 );
	}
#endif
	if( libevtx_array_initialize(
	     template_values_array,
	     number_of_template_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create template values array.",
		 function );

		goto on_error;
	}
	for( template_value_index = 0;
	     template_value_index < number_of_template_values;
	     template_value_index++ )
	{
		if( libevtx_binary_xml_template_value_initialize(
		     &template_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create template value.",
			 function );

			goto on_error;
		}
		byte_stream_copy_to_uint16_little_endian(
		 &( chunk_data[ chunk_data_offset ] ),
		 template_value->data_size );

		template_value->type = chunk_data[ chunk_data_offset + 2 ];

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: value: %02" PRIu32 " size\t: %" PRIu16 "\n",
			 function,
			 template_value_index,
			 template_value->data_size );

			libcnotify_printf(
			 "%s: value: %02" PRIu32 " type\t: 0x%02" PRIx8 " (",
			 function,
			 template_value_index,
			 template_value->type );
			libevtx_debug_print_value_type(
			 template_value->type );
			libcnotify_printf(
			 ")\n" );

			libcnotify_printf(
			 "%s: value: %02" PRIu32 " unknown1\t: 0x%02" PRIx8 "\n",
			 function,
			 template_value_index,
			 chunk_data[ chunk_data_offset + 3 ] );

			libcnotify_printf(
			 "\n" );
		}
#endif
		binary_xml_token->size += 4;
		chunk_data_offset      += 4;

		template_values_data_size += template_value->data_size;

		if( libevtx_array_set_entry_by_index(
		     *template_values_array,
		     (int) template_value_index,
		     (intptr_t *) template_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set template value: %" PRIu32 " in array.",
			 function,
			 template_value_index );

			libevtx_binary_xml_template_value_free(
			 &template_value,
			 NULL );

			goto on_error;
		}
		template_value = NULL;
	}
	if( ( chunk_data_offset + template_values_data_size ) >= chunk_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid chunk data offset value out of bounds.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: values data:\n",
		 function );
		libcnotify_print_data(
		 &( chunk_data[ chunk_data_offset ] ),
		 template_values_data_size,
		 0 );
	}
#endif
	for( template_value_index = 0;
	     template_value_index < number_of_template_values;
	     template_value_index++ )
	{
		if( libevtx_array_get_entry_by_index(
		     *template_values_array,
		     (int) template_value_index,
		     (intptr_t **) &template_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve template value: %" PRIu32 " from array.",
			 function,
			 template_value_index );

			goto on_error;
		}
		template_value->data              = &( chunk_data[ chunk_data_offset ] );
		template_value->chunk_data_offset = chunk_data_offset;

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: value: %02" PRIu32 " data:\n",
			 function,
			 template_value_index );
			libcnotify_print_data(
			 template_value->data,
			 template_value->data_size,
			 0 );
		}
#endif
		chunk_data_offset += template_value->data_size;
	}
	binary_xml_token->size += template_values_data_size;

	return( 1 );

on_error:
	if( template_values_array != NULL )
	{
		libevtx_array_free(
		 template_values_array,
		 (int (*)(intptr_t **, libcerror_error_t **)) &libevtx_binary_xml_template_value_free,
		 NULL );
	}
	return( -1 );
}

/* Reads a value from a binary XML document
 * Returns 1 if successful or -1 on error
 */
int libevtx_binary_xml_document_read_value(
     libevtx_binary_xml_document_t *binary_xml_document,
     libevtx_binary_xml_token_t *binary_xml_token,
     const uint8_t *chunk_data,
     size_t chunk_data_size,
     size_t chunk_data_offset,
     libevtx_xml_tag_t *xml_tag,
     libcerror_error_t **error )
{
	const uint8_t *binary_xml_document_data = NULL;
	static char *function                   = "libevtx_binary_xml_document_read_value";
	size_t binary_xml_document_data_size    = 0;
	size_t value_data_size                  = 0;
	uint8_t value_type                      = 0;

	if( binary_xml_document == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid binary XML document.",
		 function );

		return( -1 );
	}
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
	if( binary_xml_token->type != LIBEVTX_BINARY_XML_TOKEN_VALUE )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid binary XML token - unsupported type: 0x%02" PRIx8 ".",
		 function,
		 binary_xml_token->type );

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
		 "%s: invalid binary XML document data size value exceeds maximum.",
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
	if( xml_tag == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid XML tag.",
		 function );

		return( -1 );
	}
	binary_xml_document_data      = &( chunk_data[ chunk_data_offset ] );
	binary_xml_document_data_size = chunk_data_size - chunk_data_offset;

	if( binary_xml_document_data_size < 4 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid binary XML document data size value too small.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: value data:\n",
		 function );
		libcnotify_print_data(
		 binary_xml_document_data,
		 4,
		 0 );
	}
#endif
	value_type = binary_xml_document_data[ 1 ];

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: type\t\t\t\t: 0x%02" PRIx8 "\n",
		 function,
		 binary_xml_document_data[ 0 ] );

		libcnotify_printf(
		 "%s: value type\t\t\t: 0x%02" PRIx8 " (",
		 function,
		 value_type );
		libevtx_debug_print_value_type(
		 value_type );
		libcnotify_printf(
		 ")\n" );
	}
#endif
	binary_xml_token->size = 4;
	chunk_data_offset     += 4;

	switch( value_type )
	{
		case LIBEVTX_VALUE_TYPE_STRING_UTF16:
			byte_stream_copy_to_uint16_little_endian(
			 &( binary_xml_document_data[ 2 ] ),
			 value_data_size );

#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: number of characters\t\t: %" PRIzd "\n",
				 function,
				 value_data_size );
			}
#endif
			value_data_size *= 2;

			value_type = LIBFVALUE_VALUE_TYPE_STRING_UTF16;

			break;

		default:
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported value type: 0x%02" PRIx8 ".",
			 function,
			 value_type );

			goto on_error;
	}
	if( ( chunk_data_offset + value_data_size ) > chunk_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid value data size value out of bounds.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: value data:\n",
		 function );
		libcnotify_print_data(
		 &( chunk_data[ chunk_data_offset ] ),
		 value_data_size,
		 0 );
	}
#endif
	if( libfvalue_value_initialize(
	     &( xml_tag->value ),
	     value_type,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create value.",
		 function );

		goto on_error;
	}
	if( libfvalue_value_set_data(
	     xml_tag->value,
	     &( chunk_data[ chunk_data_offset ] ),
	     value_data_size,
	     LIBFVALUE_ENDIAN_LITTLE,
	     LIBFVALUE_VALUE_DATA_FLAG_NON_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set value data.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: value\t\t\t\t: ",
		 function );

		if( libfvalue_debug_print_value(
		     xml_tag->value,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print value.",
			 function );

			goto on_error;
		}
		libcnotify_printf(
		 "\n" );

		libcnotify_printf(
		 "\n" );
	}
#endif
	binary_xml_token->size += value_data_size;
	chunk_data_offset      += value_data_size;
	
	return( 1 );

on_error:
	if( xml_tag->value != NULL )
	{
		libfvalue_value_free(
		 &( xml_tag->value ),
		 NULL );
	}
	return( -1 );
}

/* Substitutes a substitution placeholder with a template value
 * Returns 1 if successful, 0 if no substitution or -1 on error
 */
int libevtx_binary_xml_document_substitute_template_value(
     libevtx_binary_xml_document_t *binary_xml_document,
     libevtx_io_handle_t *io_handle,
     const uint8_t *chunk_data,
     size_t chunk_data_size,
     libevtx_array_t *template_values_array,
     uint16_t template_value_index,
     uint8_t template_value_type,
     size_t *template_value_offset,
     libevtx_xml_tag_t *xml_tag,
     libcerror_error_t **error )
{
	libevtx_binary_xml_template_value_t *template_value = NULL;
	libevtx_binary_xml_token_t *binary_xml_sub_token    = NULL;
	static char *function                               = "libevtx_binary_xml_document_substitute_template_value";
	size_t value_data_size                              = 0;
	uint32_t value_format_flags                         = 0;
	uint8_t value_type                                  = 0;
	int value_format                                    = 0;

	if( binary_xml_document == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid binary XML document.",
		 function );

		return( -1 );
	}
	if( template_value_offset == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid template value offset.",
		 function );

		return( -1 );
	}
	if( xml_tag == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid XML tag.",
		 function );

		return( -1 );
	}
	if( libevtx_array_get_entry_by_index(
	     template_values_array,
	     (int) template_value_index,
	     (intptr_t **) &template_value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve template value: %" PRIu16 " from array.",
		 function,
		 template_value_index );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: value: %02" PRIu32 " size\t: %" PRIu16 "\n",
		 function,
		 template_value_index,
		 template_value->data_size );

		libcnotify_printf(
		 "%s: value: %02" PRIu32 " type\t: 0x%02" PRIx8 " (",
		 function,
		 template_value_index,
		 template_value->type );
		libevtx_debug_print_value_type(
		 template_value->type );
		libcnotify_printf(
		 ")\n" );

		libcnotify_printf(
		 "%s: value: %02" PRIu32 " data:\n",
		 function,
		 template_value_index );
		libcnotify_print_data(
		 template_value->data,
		 template_value->data_size,
		 0 );

		libcnotify_printf(
		 "\n" );
	}
#endif
	/* No substitution
	 */
	if( template_value->type == LIBEVTX_VALUE_TYPE_NULL )
	{
		return( 0 );
	}
#if defined( HAVE_VERBOSE_OUTPUT )
	if( template_value_type != template_value->type )
	{
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: mismatch in value type ( 0x%02" PRIx8 " != 0x%02" PRIx8 " ).\n",
			 function,
			 template_value_type,
			 template_value->type );
		}
	}
#endif
	if( template_value->type == LIBEVTX_VALUE_TYPE_BINARY_XML )
	{
		if( libevtx_binary_xml_token_initialize(
		     &binary_xml_sub_token,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create binary XML sub token.",
			 function );

			goto on_error;
		}
		if( libevtx_binary_xml_token_read(
		     binary_xml_sub_token,
		     io_handle,
		     chunk_data,
		     chunk_data_size,
		     template_value->chunk_data_offset,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read binary XML sub token.",
			 function );

			goto on_error;
		}
		switch( binary_xml_sub_token->type & 0xbf )
		{
			case LIBEVTX_BINARY_XML_TOKEN_FRAGMENT_HEADER:
				if( libevtx_binary_xml_document_read_fragment(
				     binary_xml_document,
				     binary_xml_sub_token,
				     io_handle,
				     chunk_data,
				     chunk_data_size,
				     template_value->chunk_data_offset,
				     xml_tag,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read fragment header.",
					 function );

					goto on_error;
				}
				break;

			case LIBEVTX_BINARY_XML_TOKEN_TEMPLATE_INSTANCE:
				if( libevtx_binary_xml_document_read_template_instance(
				     binary_xml_document,
				     binary_xml_sub_token,
				     io_handle,
				     chunk_data,
				     chunk_data_size,
				     template_value->chunk_data_offset,
				     xml_tag,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read document template instance.",
					 function );

					goto on_error;
				}
				break;

			default:
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
				 "%s: invalid binary XML token - unsupported type: 0x%02" PRIx8 ".",
				 function,
				 binary_xml_sub_token->type );

				goto on_error;
		}
		if( libevtx_binary_xml_token_free(
		     &binary_xml_sub_token,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free binary XML sub token.",
			 function );

			goto on_error;
		}
	}
	else
	{
		switch( template_value->type )
		{
			case LIBEVTX_VALUE_TYPE_STRING_UTF16:
			case LIBEVTX_VALUE_TYPE_ARRAY_OF_STRING_UTF16:
				value_type = LIBFVALUE_VALUE_TYPE_STRING_UTF16;
				break;

			case LIBEVTX_VALUE_TYPE_STRING_BYTE_STREAM:
			case LIBEVTX_VALUE_TYPE_ARRAY_OF_STRING_BYTE_STREAM:
				value_type = LIBFVALUE_VALUE_TYPE_STRING_BYTE_STREAM;
				break;

			case LIBEVTX_VALUE_TYPE_INTEGER_8BIT:
				value_format = LIBFVALUE_VALUE_FORMAT_DECIMAL;
				value_type   = LIBFVALUE_VALUE_TYPE_INTEGER_8BIT;
				break;

			case LIBEVTX_VALUE_TYPE_UNSIGNED_INTEGER_8BIT:
				value_format = LIBFVALUE_VALUE_FORMAT_DECIMAL;
				value_type   = LIBFVALUE_VALUE_TYPE_UNSIGNED_INTEGER_8BIT;
				break;

			case LIBEVTX_VALUE_TYPE_INTEGER_16BIT:
				value_format = LIBFVALUE_VALUE_FORMAT_DECIMAL;
				value_type   = LIBFVALUE_VALUE_TYPE_INTEGER_16BIT;
				break;

			case LIBEVTX_VALUE_TYPE_UNSIGNED_INTEGER_16BIT:
				value_format = LIBFVALUE_VALUE_FORMAT_DECIMAL;
				value_type   = LIBFVALUE_VALUE_TYPE_UNSIGNED_INTEGER_16BIT;
				break;

			case LIBEVTX_VALUE_TYPE_INTEGER_32BIT:
				value_format = LIBFVALUE_VALUE_FORMAT_DECIMAL;
				value_type   = LIBFVALUE_VALUE_TYPE_INTEGER_32BIT;
				break;

			case LIBEVTX_VALUE_TYPE_UNSIGNED_INTEGER_32BIT:
				value_format = LIBFVALUE_VALUE_FORMAT_DECIMAL;
				value_type   = LIBFVALUE_VALUE_TYPE_UNSIGNED_INTEGER_32BIT;
				break;

			case LIBEVTX_VALUE_TYPE_HEXADECIMAL_INTEGER_32BIT:
				value_format = LIBFVALUE_VALUE_FORMAT_HEXADECIMAL;
				value_type   = LIBFVALUE_VALUE_TYPE_UNSIGNED_INTEGER_32BIT;
				break;

			case LIBEVTX_VALUE_TYPE_INTEGER_64BIT:
				value_format = LIBFVALUE_VALUE_FORMAT_DECIMAL;
				value_type   = LIBFVALUE_VALUE_TYPE_INTEGER_64BIT;
				break;

			case LIBEVTX_VALUE_TYPE_UNSIGNED_INTEGER_64BIT:
				value_format = LIBFVALUE_VALUE_FORMAT_DECIMAL;
				value_type   = LIBFVALUE_VALUE_TYPE_UNSIGNED_INTEGER_64BIT;
				break;

			case LIBEVTX_VALUE_TYPE_HEXADECIMAL_INTEGER_64BIT:
				value_format = LIBFVALUE_VALUE_FORMAT_HEXADECIMAL;
				value_type   = LIBFVALUE_VALUE_TYPE_UNSIGNED_INTEGER_64BIT;
				break;

			case LIBEVTX_VALUE_TYPE_BOOLEAN:
				value_type = LIBFVALUE_VALUE_TYPE_BOOLEAN;
				break;

			case LIBEVTX_VALUE_TYPE_BINARY_DATA:
/* TODO improve: change libuna flags to fvalue flags ? */
				value_format       = LIBFVALUE_VALUE_FORMAT_BASE16;
				value_format_flags = LIBUNA_BASE16_VARIANT_CASE_UPPER | LIBUNA_BASE16_VARIANT_CHARACTER_LIMIT_NONE;
				value_type         = LIBFVALUE_VALUE_TYPE_BINARY_DATA;
				break;

			case LIBEVTX_VALUE_TYPE_GUID:
/* TODO improve: change libfguid flags to fvalue flags ? */
				value_format       = LIBFVALUE_VALUE_FORMAT_GUID;
				value_format_flags = LIBFGUID_STRING_FORMAT_USE_UPPER_CASE | LIBFGUID_STRING_FORMAT_USE_SURROUNDING_BRACES;
				value_type         = LIBFVALUE_VALUE_TYPE_GUID;
				break;

			case LIBEVTX_VALUE_TYPE_SIZE:
				if( template_value->data_size == 4 )
				{
					value_type = LIBFVALUE_VALUE_TYPE_UNSIGNED_INTEGER_32BIT;
				}
				else if( template_value->data_size == 8 )
				{
					value_type = LIBFVALUE_VALUE_TYPE_UNSIGNED_INTEGER_64BIT;
				}
				else
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
					 "%s: unsupported value data size: %" PRIu16 ".",
					 function,
					 template_value->data_size );

					goto on_error;
				}
				break;

			case LIBEVTX_VALUE_TYPE_FILETIME:
/* TODO improve: change libfdatetime flags to fvalue flags ? */
				value_format       = LIBFVALUE_VALUE_FORMAT_DATE_TIME_ISO8601;
				value_format_flags = LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME_NANO_SECONDS;
				value_type         = LIBFVALUE_VALUE_TYPE_FILETIME;
				break;

/* TODO improve */
			case LIBEVTX_VALUE_TYPE_NT_SECURITY_IDENTIFIER:
			case LIBEVTX_VALUE_TYPE_ARRAY_OF_NT_SECURITY_IDENTIFIER:
				value_format       = LIBFVALUE_VALUE_FORMAT_BASE16;
				value_format_flags = LIBUNA_BASE16_VARIANT_CASE_UPPER | LIBUNA_BASE16_VARIANT_CHARACTER_LIMIT_NONE;
				value_type         = LIBFVALUE_VALUE_TYPE_BINARY_DATA;
				break;

			default:
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
				 "%s: unsupported value type: 0x%02" PRIx8 ".",
				 function,
				 value_type );

				goto on_error;
		}
		if( libfvalue_value_initialize(
		     &( xml_tag->value ),
		     value_type,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create value.",
			 function );

			goto on_error;
		}
		if( value_format != 0 )
		{
			if( libfvalue_value_set_format(
			     xml_tag->value,
			     value_format,
			     value_format_flags,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set value format.",
				 function );

				goto on_error;
			}
		}
		if( value_type == LIBFVALUE_VALUE_TYPE_STRING_BYTE_STREAM )
		{
			if( libfvalue_value_set_codepage(
			     xml_tag->value,
			     io_handle->ascii_codepage,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set value data.",
				 function );

				goto on_error;
			}
		}
		if( ( template_value->type & LIBEVTX_VALUE_TYPE_ARRAY ) != 0 )
		{
			if( ( value_type == LIBFVALUE_VALUE_TYPE_STRING_BYTE_STREAM )
			 || ( value_type == LIBFVALUE_VALUE_TYPE_STRING_UTF16 ) )
			{
				if( template_value->data_size > 0 )
				{
					if( *template_value_offset >= template_value->data_size )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
						 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
						 "%s: invalid template value offset value out of bounds.",
						 function );

						return( -1 );
					}
					if( libfvalue_value_set_data_string(
					     xml_tag->value,
					     &( template_value->data[ *template_value_offset ] ),
					     template_value->data_size - *template_value_offset,
					     LIBFVALUE_ENDIAN_LITTLE,
					     LIBFVALUE_VALUE_DATA_FLAG_NON_MANAGED,
					     error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
						 "%s: unable to set value data.",
						 function );

						goto on_error;
					}
					if( libfvalue_value_get_data_size(
					     xml_tag->value,
					     &value_data_size,
					     error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
						 "%s: unable to retrieve value data size.",
						 function );

						goto on_error;
					}
					*template_value_offset += value_data_size;
				}
			}
			else
			{
/* TODO */
			}
			if( *template_value_offset == template_value->data_size )
			{
				*template_value_offset = 0;
			}
		}
		else
		{
			if( libfvalue_value_set_data(
			     xml_tag->value,
			     template_value->data,
			     template_value->data_size,
			     LIBFVALUE_ENDIAN_LITTLE,
			     LIBFVALUE_VALUE_DATA_FLAG_NON_MANAGED,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set value data.",
				 function );

				goto on_error;
			}
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: value\t\t: ",
			 function );

			if( libfvalue_debug_print_value(
			     xml_tag->value,
			     0,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
				 "%s: unable to print value.",
				 function );

				goto on_error;
			}
			libcnotify_printf(
			 "\n" );

			libcnotify_printf(
			 "\n" );
		}
#endif
	}
	return( 1 );

on_error:
	if( xml_tag->value != NULL )
	{
		libfvalue_value_free(
		 &( xml_tag->value ),
		 NULL );
	}
	if( binary_xml_sub_token != NULL )
	{
		libevtx_binary_xml_token_free(
		 &binary_xml_sub_token,
		 NULL );
	}
	return( -1 );
}

