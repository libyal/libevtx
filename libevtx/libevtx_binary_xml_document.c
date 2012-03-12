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

#include <liberror.h>
#include <libnotify.h>

#include "libevtx_binary_xml_document.h"
#include "libevtx_binary_xml_token.h"
#include "libevtx_definitions.h"
#include "libevtx_libfvalue.h"
#include "libevtx_libuna.h"

/* Initialize binary XML document
 * Make sure the value binary XML document is pointing to is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libevtx_binary_xml_document_initialize(
     libevtx_binary_xml_document_t **binary_xml_document,
     liberror_error_t **error )
{
	static char *function = "libevtx_binary_xml_document_initialize";

	if( binary_xml_document == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid binary XML document.",
		 function );

		return( -1 );
	}
	if( *binary_xml_document != NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid binary XML document value already set.",
		 function );

		return( -1 );
	}
	*binary_xml_document = memory_allocate_structure(
	                 libevtx_binary_xml_document_t );

	if( *binary_xml_document == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create binary XML document.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     ( *binary_xml_document ),
	     0,
	     sizeof( libevtx_binary_xml_document_t ) ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_SET_FAILED,
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
     liberror_error_t **error )
{
	static char *function = "libevtx_binary_xml_document_free";
	int result            = 1;

	if( binary_xml_document == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid binary XML document.",
		 function );

		return( -1 );
	}
	if( *binary_xml_document != NULL )
	{
		memory_free(
		 *binary_xml_document );

		*binary_xml_document = NULL;
	}
	return( result );
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
     liberror_error_t **error )
{
	libevtx_binary_xml_token_t *binary_xml_token = NULL;
	static char *function                        = "libevtx_binary_xml_document_read";

	if( binary_xml_document == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid binary XML document.",
		 function );

		return( -1 );
	}
	if( io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( chunk_data == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk data.",
		 function );

		return( -1 );
	}
	if( chunk_data_size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid binary XML document data size value exceeds maximum.",
		 function );

		goto on_error;
	}
	if( chunk_data_offset >= chunk_data_size )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid chunk data offset value out of bounds.",
		 function );

		goto on_error;
	}
	if( libevtx_binary_xml_token_initialize(
	     &binary_xml_token,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
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
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_READ_FAILED,
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
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_RUNTIME,
					 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
					 "%s: invalid binary XML document data size value too small.",
					 function );

					goto on_error;
				}
#if defined( HAVE_DEBUG_OUTPUT )
				if( libnotify_verbose != 0 )
				{
					libnotify_printf(
					 "%s: end of file data:\n",
					 function );
					libnotify_print_data(
					 &( chunk_data[ chunk_data_offset ] ),
					 1,
					 0 );
				}
#endif
#if defined( HAVE_DEBUG_OUTPUT )
				if( libnotify_verbose != 0 )
				{
					libnotify_printf(
					 "%s: type\t\t\t\t\t: 0x%02" PRIx8 "\n",
					 function,
					 chunk_data[ chunk_data_offset ] );

					libnotify_printf(
					 "\n" );
				}
#endif
				binary_xml_token->size = 1;

				break;

			case LIBEVTX_BINARY_XML_TOKEN_TEMPLATE_INSTANCE:
				if( libevtx_binary_xml_document_read_template_instance(
				     binary_xml_document,
				     binary_xml_token,
				     io_handle,
				     chunk_data,
				     chunk_data_size,
				     chunk_data_offset,
				     error ) != 1 )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_IO,
					 LIBERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read document template instance.",
					 function );

					goto on_error;
				}
				break;

			case LIBEVTX_BINARY_XML_TOKEN_FRAGMENT_HEADER:
				if( libevtx_binary_xml_document_read_fragment_header(
				     binary_xml_document,
				     binary_xml_token,
				     chunk_data,
				     chunk_data_size,
				     chunk_data_offset,
				     error ) != 1 )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_IO,
					 LIBERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read fragment header.",
					 function );

					goto on_error;
				}
				break;

			default:
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
				 "%s: invalid binary XML token - unsupported type: 0x%02" PRIx8 ".",
				 function,
				 binary_xml_token->type );

				goto on_error;
		}
		chunk_data_offset += binary_xml_token->size;

		if( binary_xml_token->type == LIBEVTX_BINARY_XML_TOKEN_END_OF_FILE )
		{
			break;
		}
	}
	if( libevtx_binary_xml_token_free(
	     &binary_xml_token,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
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
     liberror_error_t **error )
{
	libevtx_binary_xml_token_t *binary_xml_sub_token = NULL;
	libfvalue_value_t *name_value                    = NULL;
	static char *function                            = "libevtx_binary_xml_document_read_attribute";
	size_t trailing_data_size                        = 0;
	uint32_t attribute_name_offset                   = 0;
	uint32_t attribute_name_size                     = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint32_t value_32bit                             = 0;
	uint16_t value_16bit                             = 0;
#endif

	if( binary_xml_document == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid binary XML document.",
		 function );

		return( -1 );
	}
	if( binary_xml_token == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid binary XML token.",
		 function );

		return( -1 );
	}
	if( ( binary_xml_token->type & 0xbf ) != LIBEVTX_BINARY_XML_TOKEN_ATTRIBUTE )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid binary XML token - unsupported type: 0x%02" PRIx8 ".",
		 function,
		 binary_xml_token->type );

		return( -1 );
	}
	if( chunk_data == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk data.",
		 function );

		return( -1 );
	}
	if( chunk_data_size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid binary XML document data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( chunk_data_offset >= chunk_data_size )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid chunk data offset value out of bounds.",
		 function );

		return( -1 );
	}
	if( ( chunk_data_offset + 5 ) >= chunk_data_size )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid binary XML document data size value too small.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: attribute data:\n",
		 function );
		libnotify_print_data(
		 &( chunk_data[ chunk_data_offset ] ),
		 5,
		 0 );
	}
#endif
	byte_stream_copy_to_uint32_little_endian(
	 &( chunk_data[ chunk_data_offset + 1 ] ),
	 attribute_name_offset );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: type\t\t\t: 0x%02" PRIx8 "\n",
		 function,
		 &( chunk_data[ chunk_data_offset ] ) );

		libnotify_printf(
		 "%s: name offset\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 attribute_name_offset );

		libnotify_printf(
		 "\n" );
	}
#endif
	binary_xml_token->size = 5;
	chunk_data_offset     += 5;

	if( attribute_name_offset > chunk_data_offset )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid attribute data offset value out of bounds.",
		 function );

		goto on_error;
	}
	if( chunk_data_offset < attribute_name_offset )
	{
		trailing_data_size = attribute_name_offset - chunk_data_offset;

#if defined( HAVE_DEBUG_OUTPUT )
		if( libnotify_verbose != 0 )
		{
			libnotify_printf(
			 "%s: trailing data:\n",
			 function );
			libnotify_print_data(
			 &( chunk_data[ chunk_data_offset ] ),
			 trailing_data_size,
			 0 );
		}
#endif
		binary_xml_token->size += trailing_data_size;
		chunk_data_offset      += trailing_data_size;
	}
	if( ( attribute_name_offset + 8 ) > chunk_data_size )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid binary XML document data size value too small.",
		 function );

		goto on_error;
	}
	byte_stream_copy_to_uint16_little_endian(
	 &( chunk_data[ attribute_name_offset + 6 ] ),
	 attribute_name_size );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		byte_stream_copy_to_uint32_little_endian(
		 &( chunk_data[ attribute_name_offset ] ),
		 value_32bit );
		libnotify_printf(
		 "%s: name unknown1\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint16_little_endian(
		 &( chunk_data[ attribute_name_offset + 4 ] ),
		 value_16bit );
		libnotify_printf(
		 "%s: name hash\t\t\t: 0x%04" PRIx16 "\n",
		 function,
		 value_16bit );

		libnotify_printf(
		 "%s: name number of characters\t: %" PRIu16 "\n",
		 function,
		 attribute_name_size );
	}
#endif
	attribute_name_size += 1;
	attribute_name_size *= 2;

	if( ( attribute_name_offset + 8 + attribute_name_size ) > chunk_data_size )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid attribute name size value out of bounds.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: name data:\n",
		 function );
		libnotify_print_data(
		 &( chunk_data[ attribute_name_offset + 8 ] ),
		 attribute_name_size,
		 0 );
	}
#endif
	if( libfvalue_value_initialize(
	     &name_value,
	     LIBFVALUE_VALUE_TYPE_STRING_UTF16,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create name value.",
		 function );

		goto on_error;
	}
	if( libfvalue_value_set_data(
	     name_value,
	     &( chunk_data[ attribute_name_offset + 8 ] ),
	     attribute_name_size,
	     LIBFVALUE_ENDIAN_LITTLE,
	     LIBFVALUE_VALUE_DATA_FLAG_NON_MANAGED,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set name value data.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: name\t\t\t: ",
		 function );

		if( libfvalue_debug_print_value(
		     name_value,
		     0,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print name value.",
			 function );

			goto on_error;
		}
		libnotify_printf(
		 "\n" );

		libnotify_printf(
		 "\n" );
	}
#endif
	if( libfvalue_value_free(
	     &name_value,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free name value.",
		 function );

		goto on_error;
	}
	if( chunk_data_offset == attribute_name_offset )
	{
		binary_xml_token->size += 8 + attribute_name_size;
		chunk_data_offset      += 8 + attribute_name_size;
	}
	if( libevtx_binary_xml_token_initialize(
	     &binary_xml_sub_token,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
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
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read binary XML sub token.",
		 function );

		goto on_error;
	}
	switch( binary_xml_sub_token->type & 0xbf )
	{
		case LIBEVTX_BINARY_XML_TOKEN_VALUE:
/* TODO make sure this is a string value */
			if( libevtx_binary_xml_document_read_value(
			     binary_xml_document,
			     binary_xml_sub_token,
			     chunk_data,
			     chunk_data_size,
			     chunk_data_offset,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_IO,
				 LIBERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read value.",
				 function );

				goto on_error;
			}
			break;

		case LIBEVTX_BINARY_XML_TOKEN_OPTIONAL_SUBSTITUTION:
			if( libevtx_binary_xml_document_read_optional_substitution(
			     binary_xml_document,
			     binary_xml_sub_token,
			     chunk_data,
			     chunk_data_size,
			     chunk_data_offset,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_IO,
				 LIBERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read optional substitution.",
				 function );

				goto on_error;
			}
			break;

		default:
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: invalid binary XML sub token - unsupported type: 0x%02" PRIx8 ".",
			 function,
			 binary_xml_sub_token->type );

			goto on_error;
	}
	binary_xml_token->size += binary_xml_sub_token->size;
	chunk_data_offset      += binary_xml_sub_token->size;

	if( libevtx_binary_xml_token_free(
	     &binary_xml_sub_token,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
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
	if( name_value != NULL )
	{
		libfvalue_value_free(
		 &name_value,
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
     liberror_error_t **error )
{
	libevtx_binary_xml_token_t *binary_xml_sub_token = NULL;
	libfvalue_value_t *name_value                    = NULL;
	const uint8_t *binary_xml_document_data          = NULL;
	static char *function                            = "libevtx_binary_xml_document_read_element";
	size_t binary_xml_document_data_size             = 0;
	size_t trailing_data_size                        = 0;
	uint32_t attribute_list_size                     = 0;
	uint32_t element_name_offset                     = 0;
	uint32_t element_name_size                       = 0;
	uint32_t element_size                            = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint32_t value_32bit                             = 0;
	uint16_t value_16bit                             = 0;
#endif

	if( binary_xml_document == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid binary XML document.",
		 function );

		return( -1 );
	}
	if( binary_xml_token == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid binary XML token.",
		 function );

		return( -1 );
	}
	if( ( binary_xml_token->type & 0xbf ) != LIBEVTX_BINARY_XML_TOKEN_OPEN_START_ELEMENT_TAG )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid binary XML token - unsupported type: 0x%02" PRIx8 ".",
		 function,
		 binary_xml_token->type );

		return( -1 );
	}
	if( chunk_data == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk data.",
		 function );

		return( -1 );
	}
	if( chunk_data_size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid binary XML document data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( chunk_data_offset >= chunk_data_size )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid chunk data offset value out of bounds.",
		 function );

		return( -1 );
	}
	binary_xml_document_data      = &( chunk_data[ chunk_data_offset ] );
	binary_xml_document_data_size = chunk_data_size - chunk_data_offset;

	if( binary_xml_document_data_size < 11 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid binary XML document data size value too small.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: element data:\n",
		 function );
		libnotify_print_data(
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
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: type\t\t\t\t: 0x%02" PRIx8 "\n",
		 function,
		 binary_xml_document_data[ 0 ] );

		byte_stream_copy_to_uint16_little_endian(
		 &( binary_xml_document_data[ 1 ] ),
		 value_16bit );
		libnotify_printf(
		 "%s: dependency identifier\t\t: %" PRIi16 " (0x%04" PRIx16 ")\n",
		 function,
		 (int16_t) value_16bit,
		 value_16bit );

		libnotify_printf(
		 "%s: size\t\t\t\t: %" PRIu32 "\n",
		 function,
		 element_size );

		libnotify_printf(
		 "%s: name offset\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 element_name_offset );

		libnotify_printf(
		 "\n" );
	}
#endif
	binary_xml_token->size = 11;
	chunk_data_offset     += 11;

	/* The first 7 bytes are not included in the element size
	 */
	element_size -= 4;

	if( element_name_offset > chunk_data_offset )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid element data offset value out of bounds.",
		 function );

		goto on_error;
	}
	if( chunk_data_offset < element_name_offset )
	{
		trailing_data_size = element_name_offset - chunk_data_offset;

#if defined( HAVE_DEBUG_OUTPUT )
		if( libnotify_verbose != 0 )
		{
			libnotify_printf(
			 "%s: trailing data:\n",
			 function );
			libnotify_print_data(
			 &( chunk_data[ chunk_data_offset ] ),
			 trailing_data_size,
			 0 );
		}
#endif
		binary_xml_token->size += trailing_data_size;
		chunk_data_offset      += trailing_data_size;
		element_size           -= trailing_data_size;
	}
	if( ( element_name_offset + 8 ) > chunk_data_size )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid binary XML document data size value too small.",
		 function );

		goto on_error;
	}
	byte_stream_copy_to_uint16_little_endian(
	 &( chunk_data[ element_name_offset + 6 ] ),
	 element_name_size );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		byte_stream_copy_to_uint32_little_endian(
		 &( chunk_data[ element_name_offset ] ),
		 value_32bit );
		libnotify_printf(
		 "%s: name unknown1\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint16_little_endian(
		 &( chunk_data[ element_name_offset + 4 ] ),
		 value_16bit );
		libnotify_printf(
		 "%s: name hash\t\t\t: 0x%04" PRIx16 "\n",
		 function,
		 value_16bit );

		libnotify_printf(
		 "%s: name number of characters\t: %" PRIu16 "\n",
		 function,
		 element_name_size );
	}
#endif
	element_name_size += 1;
	element_name_size *= 2;

	if( ( element_name_offset + 8 + element_name_size ) > chunk_data_size )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid element name size value out of bounds.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: name data:\n",
		 function );
		libnotify_print_data(
		 &( chunk_data[ element_name_offset + 8 ] ),
		 element_name_size,
		 0 );
	}
#endif
	if( libfvalue_value_initialize(
	     &name_value,
	     LIBFVALUE_VALUE_TYPE_STRING_UTF16,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create name value.",
		 function );

		goto on_error;
	}
	if( libfvalue_value_set_data(
	     name_value,
	     &( chunk_data[ element_name_offset + 8 ] ),
	     element_name_size,
	     LIBFVALUE_ENDIAN_LITTLE,
	     LIBFVALUE_VALUE_DATA_FLAG_NON_MANAGED,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set name value data.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: name\t\t\t\t: ",
		 function );

		if( libfvalue_debug_print_value(
		     name_value,
		     0,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print name value.",
			 function );

			goto on_error;
		}
		libnotify_printf(
		 "\n" );

		libnotify_printf(
		 "\n" );
	}
#endif
	if( libfvalue_value_free(
	     &name_value,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free name value.",
		 function );

		goto on_error;
	}
	if( chunk_data_offset == element_name_offset )
	{
		binary_xml_token->size += 8 + element_name_size;
		chunk_data_offset      += 8 + element_name_size;
		element_size           -= 8 + element_name_size;
	}
	if( libevtx_binary_xml_token_initialize(
	     &binary_xml_sub_token,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create binary XML sub token.",
		 function );

		goto on_error;
	}
	if( ( binary_xml_token->type & LIBEVTX_BINARY_XML_TOKEN_FLAG_HAS_MORE_DATA ) != 0 )
	{
		if( ( chunk_data_offset + 4 ) > chunk_data_size )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid binary XML document data size value too small.",
			 function );

			goto on_error;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libnotify_verbose != 0 )
		{
			libnotify_printf(
			 "%s: attribute list data:\n",
			 function );
			libnotify_print_data(
			 &( chunk_data[ chunk_data_offset ] ),
			 4,
			 0 );
		}
#endif
		byte_stream_copy_to_uint32_little_endian(
		 &( chunk_data[ chunk_data_offset ] ),
		 attribute_list_size );

#if defined( HAVE_DEBUG_OUTPUT )
		if( libnotify_verbose != 0 )
		{
			libnotify_printf(
			 "%s: attribute list size\t\t: %" PRIu32 "\n",
			 function,
			 attribute_list_size );

			libnotify_printf(
			 "\n" );
		}
#endif
		binary_xml_token->size += 4;
		chunk_data_offset      += 4;
		element_size           -= 4;

		while( attribute_list_size > 0 )
		{
			if( libevtx_binary_xml_token_read(
			     binary_xml_sub_token,
			     io_handle,
			     chunk_data,
			     chunk_data_size,
			     chunk_data_offset,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_IO,
				 LIBERROR_IO_ERROR_READ_FAILED,
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
			     chunk_data_offset,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_IO,
				 LIBERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read attribute.",
				 function );

				goto on_error;
			}
			binary_xml_token->size += binary_xml_sub_token->size;
			chunk_data_offset      += binary_xml_sub_token->size;
			element_size           -= binary_xml_sub_token->size;

			if( attribute_list_size < binary_xml_sub_token->size )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
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
	     chunk_data_offset,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read binary XML sub token.",
		 function );

		goto on_error;
	}
	if( ( binary_xml_sub_token->type != LIBEVTX_BINARY_XML_TOKEN_CLOSE_START_ELEMENT_TAG )
	 && ( binary_xml_sub_token->type != LIBEVTX_BINARY_XML_TOKEN_CLOSE_EMPTY_ELEMENT_TAG ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid binary XML token - unsupported type: 0x%02" PRIx8 ".",
		 function,
		 binary_xml_token->type );

		return( -1 );
	}
	if( ( chunk_data_offset + 1 ) > chunk_data_size )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid binary XML document data size value too small.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: close element tag data:\n",
		 function );
		libnotify_print_data(
		 &( chunk_data[ chunk_data_offset ] ),
		 1,
		 0 );
	}
#endif
#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: type\t\t\t\t: 0x%02" PRIx8 "\n",
		 function,
		 chunk_data[ chunk_data_offset ] );

		libnotify_printf(
		 "\n" );
	}
#endif
	binary_xml_token->size += 1;
	chunk_data_offset      += 1;
	element_size           -= 1;

	if( binary_xml_sub_token->type == LIBEVTX_BINARY_XML_TOKEN_CLOSE_START_ELEMENT_TAG )
	{
		while( element_size > 0 )
		{
			if( libevtx_binary_xml_token_read(
			     binary_xml_sub_token,
			     io_handle,
			     chunk_data,
			     chunk_data_size,
			     chunk_data_offset,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_IO,
				 LIBERROR_IO_ERROR_READ_FAILED,
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
					     chunk_data_offset,
					     error ) != 1 )
					{
						liberror_error_set(
						 error,
						 LIBERROR_ERROR_DOMAIN_IO,
						 LIBERROR_IO_ERROR_READ_FAILED,
						 "%s: unable to read element.",
						 function );

						goto on_error;
					}
					break;

				case LIBEVTX_BINARY_XML_TOKEN_CLOSE_EMPTY_ELEMENT_TAG:
				case LIBEVTX_BINARY_XML_TOKEN_END_ELEMENT_TAG:
					if( ( chunk_data_offset + 1 ) > chunk_data_size )
					{
						liberror_error_set(
						 error,
						 LIBERROR_ERROR_DOMAIN_RUNTIME,
						 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
						 "%s: invalid binary XML document data size value too small.",
						 function );

						goto on_error;
					}
#if defined( HAVE_DEBUG_OUTPUT )
					if( libnotify_verbose != 0 )
					{
						libnotify_printf(
						 "%s: end element tag data:\n",
						 function );
						libnotify_print_data(
						 &( chunk_data[ chunk_data_offset ] ),
						 1,
						 0 );
					}
#endif
#if defined( HAVE_DEBUG_OUTPUT )
					if( libnotify_verbose != 0 )
					{
						libnotify_printf(
						 "%s: type\t\t\t\t: 0x%02" PRIx8 "\n",
						 function,
						 chunk_data[ chunk_data_offset ] );

						libnotify_printf(
						 "\n" );
					}
#endif
					binary_xml_sub_token->size = 1;

					break;

				case LIBEVTX_BINARY_XML_TOKEN_VALUE:
					if( libevtx_binary_xml_document_read_value(
					     binary_xml_document,
					     binary_xml_sub_token,
					     chunk_data,
					     chunk_data_size,
					     chunk_data_offset,
					     error ) != 1 )
					{
						liberror_error_set(
						 error,
						 LIBERROR_ERROR_DOMAIN_IO,
						 LIBERROR_IO_ERROR_READ_FAILED,
						 "%s: unable to read value.",
						 function );

						goto on_error;
					}
					break;

				case LIBEVTX_BINARY_XML_TOKEN_OPTIONAL_SUBSTITUTION:
					if( libevtx_binary_xml_document_read_optional_substitution(
					     binary_xml_document,
					     binary_xml_sub_token,
					     chunk_data,
					     chunk_data_size,
					     chunk_data_offset,
					     error ) != 1 )
					{
						liberror_error_set(
						 error,
						 LIBERROR_ERROR_DOMAIN_IO,
						 LIBERROR_IO_ERROR_READ_FAILED,
						 "%s: unable to read optional substitution.",
						 function );

						goto on_error;
					}
					break;

				default:
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_RUNTIME,
					 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
					 "%s: invalid binary XML sub token - unsupported type: 0x%02" PRIx8 ".",
					 function,
					 binary_xml_sub_token->type );

					goto on_error;
			}
			binary_xml_token->size += binary_xml_sub_token->size;
			chunk_data_offset      += binary_xml_sub_token->size;

			if( element_size < binary_xml_sub_token->size )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
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
	if( element_size > 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid element size value out of bounds.",
		 function );

		goto on_error;
	}
	if( libevtx_binary_xml_token_free(
	     &binary_xml_sub_token,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
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
	if( name_value != NULL )
	{
		libfvalue_value_free(
		 &name_value,
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
     liberror_error_t **error )
{
	const uint8_t *binary_xml_document_data = NULL;
	static char *function                   = "libevtx_binary_xml_document_read_fragment_header";
	size_t binary_xml_document_data_size    = 0;

	if( binary_xml_document == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid binary XML document.",
		 function );

		return( -1 );
	}
	if( binary_xml_token == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid binary XML token.",
		 function );

		return( -1 );
	}
	if( binary_xml_token->type != LIBEVTX_BINARY_XML_TOKEN_FRAGMENT_HEADER )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid binary XML token - unsupported type: 0x%02" PRIx8 ".",
		 function,
		 binary_xml_token->type );

		return( -1 );
	}
	if( chunk_data == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk data.",
		 function );

		return( -1 );
	}
	if( chunk_data_size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid binary XML document data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( chunk_data_offset >= chunk_data_size )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid chunk data offset value out of bounds.",
		 function );

		return( -1 );
	}
	binary_xml_document_data      = &( chunk_data[ chunk_data_offset ] );
	binary_xml_document_data_size = chunk_data_size - chunk_data_offset;

	if( binary_xml_document_data_size < 4 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid binary XML document data size value too small.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: fragment header data:\n",
		 function );
		libnotify_print_data(
		 binary_xml_document_data,
		 4,
		 0 );
	}
#endif
#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: type\t\t\t: 0x%02" PRIx8 "\n",
		 function,
		 binary_xml_document_data[ 0 ] );

		libnotify_printf(
		 "%s: major version\t\t: %" PRIu8 "\n",
		 function,
		 binary_xml_document_data[ 1 ] );

		libnotify_printf(
		 "%s: minor version\t\t: %" PRIu8 "\n",
		 function,
		 binary_xml_document_data[ 2 ] );

		libnotify_printf(
		 "%s: flags\t\t\t: 0x%02" PRIx8 "\n",
		 function,
		 binary_xml_document_data[ 3 ] );

		libnotify_printf(
		 "\n" );
	}
#endif
/* TODO check values */
	binary_xml_token->size = 4;
	
	return( 1 );
}

/* Reads an optional substitution from a binary XML document
 * Returns 1 if successful or -1 on error
 */
int libevtx_binary_xml_document_read_optional_substitution(
     libevtx_binary_xml_document_t *binary_xml_document,
     libevtx_binary_xml_token_t *binary_xml_token,
     const uint8_t *chunk_data,
     size_t chunk_data_size,
     size_t chunk_data_offset,
     liberror_error_t **error )
{
	const uint8_t *binary_xml_document_data = NULL;
	static char *function                   = "libevtx_binary_xml_document_read_optional_substitution";
	size_t binary_xml_document_data_size    = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint16_t value_16bit                    = 0;
#endif

	if( binary_xml_document == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid binary XML document.",
		 function );

		return( -1 );
	}
	if( binary_xml_token == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid binary XML token.",
		 function );

		return( -1 );
	}
	if( binary_xml_token->type != LIBEVTX_BINARY_XML_TOKEN_OPTIONAL_SUBSTITUTION )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid binary XML token - unsupported type: 0x%02" PRIx8 ".",
		 function,
		 binary_xml_token->type );

		return( -1 );
	}
	if( chunk_data == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk data.",
		 function );

		return( -1 );
	}
	if( chunk_data_size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid binary XML document data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( chunk_data_offset >= chunk_data_size )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid chunk data offset value out of bounds.",
		 function );

		return( -1 );
	}
	binary_xml_document_data      = &( chunk_data[ chunk_data_offset ] );
	binary_xml_document_data_size = chunk_data_size - chunk_data_offset;

	if( binary_xml_document_data_size < 4 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid binary XML document data size value too small.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: optional substitution data:\n",
		 function );
		libnotify_print_data(
		 binary_xml_document_data,
		 4,
		 0 );
	}
#endif
#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: type\t\t: 0x%02" PRIx8 "\n",
		 function,
		 binary_xml_document_data[ 0 ] );

		byte_stream_copy_to_uint16_little_endian(
		 &( binary_xml_document_data[ 1 ] ),
		 value_16bit );
		libnotify_printf(
		 "%s: identifier\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		libnotify_printf(
		 "%s: value type\t: %" PRIu8 "\n",
		 function,
		 binary_xml_document_data[ 3 ] );

		libnotify_printf(
		 "\n" );
	}
#endif
	binary_xml_token->size = 4;
	chunk_data_offset     += 4;
	
	return( 1 );
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
     liberror_error_t **error )
{
	libevtx_binary_xml_token_t *binary_xml_sub_token = NULL;
	const uint8_t *binary_xml_document_data          = NULL;
	static char *function                            = "libevtx_binary_xml_document_read_template_instance";
	size_t binary_xml_document_data_size             = 0;
	size_t template_value_definitions_data_size      = 0;
	size_t template_values_data_size                 = 0;
	size_t template_data_offset                      = 0;
	size_t template_data_size                        = 0;
	size_t trailing_data_size                        = 0;
	uint32_t number_of_template_values               = 0;
	uint32_t template_definition_data_offset         = 0;
	uint32_t template_definition_data_size           = 0;
	uint32_t template_value_index                    = 0;
	uint16_t template_value_data_size                = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint32_t value_32bit                             = 0;
#endif

	if( binary_xml_document == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid binary XML document.",
		 function );

		return( -1 );
	}
	if( binary_xml_token == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid binary XML token.",
		 function );

		return( -1 );
	}
	if( binary_xml_token->type != LIBEVTX_BINARY_XML_TOKEN_TEMPLATE_INSTANCE )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid binary XML token - unsupported type: 0x%02" PRIx8 ".",
		 function,
		 binary_xml_token->type );

		return( -1 );
	}
	if( chunk_data == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk data.",
		 function );

		return( -1 );
	}
	if( chunk_data_size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid binary XML document data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( chunk_data_offset >= chunk_data_size )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid chunk data offset value out of bounds.",
		 function );

		return( -1 );
	}
	binary_xml_document_data      = &( chunk_data[ chunk_data_offset ] );
	binary_xml_document_data_size = chunk_data_size - chunk_data_offset;

	if( ( chunk_data_offset + 10 ) >= chunk_data_size )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid binary XML document data size value too small.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: template instance header data:\n",
		 function );
		libnotify_print_data(
		 &( chunk_data[ chunk_data_offset ] ),
		 10,
		 0 );
	}
#endif
	byte_stream_copy_to_uint32_little_endian(
	 &( chunk_data[ chunk_data_offset + 6 ] ),
	 template_definition_data_offset );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: type\t\t: 0x%02" PRIx8 "\n",
		 function,
		 chunk_data[ chunk_data_offset ] );

		libnotify_printf(
		 "%s: unknown1\t\t: %" PRIu8 "\n",
		 function,
		 chunk_data[ chunk_data_offset + 1 ] );

		byte_stream_copy_to_uint32_little_endian(
		 &( chunk_data[ chunk_data_offset + 2 ] ),
		 value_32bit );
		libnotify_printf(
		 "%s: unknown2\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		libnotify_printf(
		 "%s: data offset\t\t: 0x%08" PRIx32 "\n",
		 function,
		 template_definition_data_offset );

		libnotify_printf(
		 "\n" );
	}
#endif
	binary_xml_token->size = 10;
	chunk_data_offset     += 10;

	if( template_definition_data_offset > chunk_data_offset )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid template definition data offset value out of bounds.",
		 function );

		goto on_error;
	}
	if( chunk_data_offset < template_definition_data_offset )
	{
		trailing_data_size = template_definition_data_offset - chunk_data_offset;

#if defined( HAVE_DEBUG_OUTPUT )
		if( libnotify_verbose != 0 )
		{
			libnotify_printf(
			 "%s: trailing data:\n",
			 function );
			libnotify_print_data(
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
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid binary XML document data size value too small.",
		 function );

		goto on_error;
	}
	byte_stream_copy_to_uint32_little_endian(
	 &( chunk_data[ template_data_offset + 20 ] ),
	 template_definition_data_size );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		byte_stream_copy_to_uint32_little_endian(
		 &( chunk_data[ template_data_offset ] ),
		 value_32bit );
		libnotify_printf(
		 "%s: offset next\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		libnotify_printf(
		 "%s: identifier:\n",
		 function );
		libnotify_print_data(
		 &( chunk_data[ template_data_offset + 4 ] ),
		 16,
		 0 );

		libnotify_printf(
		 "%s: definition size\t: %" PRIu32 "\n",
		 function,
		 template_definition_data_size );

		libnotify_printf(
		 "\n" );
	}
#endif
	template_data_offset += 24;

	if( libevtx_binary_xml_token_initialize(
	     &binary_xml_sub_token,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
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
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_READ_FAILED,
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
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_READ_FAILED,
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
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_READ_FAILED,
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
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_READ_FAILED,
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
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read binary XML sub token.",
		 function );

		goto on_error;
	}
	if( binary_xml_sub_token->type != LIBEVTX_BINARY_XML_TOKEN_END_OF_FILE )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid binary XML token - unsupported type: 0x%02" PRIx8 ".",
		 function,
		 binary_xml_token->type );

		return( -1 );
	}
	if( ( chunk_data_offset + 1 ) > chunk_data_size )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid binary XML document data size value too small.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: end of file data:\n",
		 function );
		libnotify_print_data(
		 &( chunk_data[ chunk_data_offset ] ),
		 1,
		 0 );
	}
#endif
#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: type\t\t: 0x%02" PRIx8 "\n",
		 function,
		 chunk_data[ chunk_data_offset ] );

		libnotify_printf(
		 "\n" );
	}
#endif
	template_data_offset += 1;

	if( libevtx_binary_xml_token_free(
	     &binary_xml_sub_token,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
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
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid template definition data size value too small.",
			 function );

			goto on_error;
		}
		template_definition_data_size -= template_data_size - 24;
	}
/* TODO check if template_definition_data_size is 0 */

	if( ( chunk_data_offset + 4 ) >= chunk_data_size )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid chunk data offset value out of bounds.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: template instance data:\n",
		 function );
		libnotify_print_data(
		 &( chunk_data[ chunk_data_offset ] ),
		 4,
		 0 );
	}
#endif
	byte_stream_copy_to_uint32_little_endian(
	 &( chunk_data[ chunk_data_offset ] ),
	 number_of_template_values );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: number of values\t: %" PRIu32 "\n",
		 function,
		 number_of_template_values );

		libnotify_printf(
		 "\n" );
	}
#endif
	binary_xml_token->size += 4;
	chunk_data_offset      += 4;

	template_value_definitions_data_size = number_of_template_values * 4;

	if( ( chunk_data_offset + template_value_definitions_data_size ) >= chunk_data_size )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid chunk data offset value out of bounds.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: template instance value definitions data:\n",
		 function );
		libnotify_print_data(
		 &( chunk_data[ chunk_data_offset ] ),
		 template_value_definitions_data_size,
		 0 );
	}
#endif
	for( template_value_index = 0;
	     template_value_index < number_of_template_values;
	     template_value_index++ )
	{
		byte_stream_copy_to_uint16_little_endian(
		 &( chunk_data[ chunk_data_offset ] ),
		 template_value_data_size );

#if defined( HAVE_DEBUG_OUTPUT )
		if( libnotify_verbose != 0 )
		{
			libnotify_printf(
			 "%s: value: %02" PRIu32 " size\t: %" PRIu16 "\n",
			 function,
			 template_value_index,
			 template_value_data_size );

			libnotify_printf(
			 "%s: value: %02" PRIu32 " type\t: %" PRIu8 "\n",
			 function,
			 template_value_index,
			 chunk_data[ chunk_data_offset + 2 ] );

			libnotify_printf(
			 "%s: value: %02" PRIu32 " unknown1\t: 0x%02" PRIx8 "\n",
			 function,
			 template_value_index,
			 chunk_data[ chunk_data_offset + 3 ] );

			libnotify_printf(
			 "\n" );
		}
#endif
		binary_xml_token->size += 4;
		chunk_data_offset      += 4;

		template_values_data_size += template_value_data_size;
	}
	if( ( chunk_data_offset + template_values_data_size ) >= chunk_data_size )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid chunk data offset value out of bounds.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: template instance value definitions data:\n",
		 function );
		libnotify_print_data(
		 &( chunk_data[ chunk_data_offset ] ),
		 template_values_data_size,
		 0 );
	}
#endif
	binary_xml_token->size += template_values_data_size;
	chunk_data_offset      += template_values_data_size;

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

/* Reads a value from a binary XML document
 * Returns 1 if successful or -1 on error
 */
int libevtx_binary_xml_document_read_value(
     libevtx_binary_xml_document_t *binary_xml_document,
     libevtx_binary_xml_token_t *binary_xml_token,
     const uint8_t *chunk_data,
     size_t chunk_data_size,
     size_t chunk_data_offset,
     liberror_error_t **error )
{
	libfvalue_value_t *value                = NULL;
	const uint8_t *binary_xml_document_data = NULL;
	static char *function                   = "libevtx_binary_xml_document_read_value";
	size_t binary_xml_document_data_size    = 0;
	size_t value_data_size                  = 0;
	uint8_t value_type                      = 0;

	if( binary_xml_document == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid binary XML document.",
		 function );

		return( -1 );
	}
	if( binary_xml_token == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid binary XML token.",
		 function );

		return( -1 );
	}
	if( binary_xml_token->type != LIBEVTX_BINARY_XML_TOKEN_VALUE )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid binary XML token - unsupported type: 0x%02" PRIx8 ".",
		 function,
		 binary_xml_token->type );

		return( -1 );
	}
	if( chunk_data == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk data.",
		 function );

		return( -1 );
	}
	if( chunk_data_size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid binary XML document data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( chunk_data_offset >= chunk_data_size )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid chunk data offset value out of bounds.",
		 function );

		return( -1 );
	}
	binary_xml_document_data      = &( chunk_data[ chunk_data_offset ] );
	binary_xml_document_data_size = chunk_data_size - chunk_data_offset;

	if( binary_xml_document_data_size < 4 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid binary XML document data size value too small.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: value data:\n",
		 function );
		libnotify_print_data(
		 binary_xml_document_data,
		 4,
		 0 );
	}
#endif
#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: type\t\t\t\t: 0x%02" PRIx8 "\n",
		 function,
		 binary_xml_document_data[ 0 ] );

		libnotify_printf(
		 "%s: value type\t\t\t: %" PRIu8 "\n",
		 function,
		 binary_xml_document_data[ 1 ] );
	}
#endif
	binary_xml_token->size = 4;
	chunk_data_offset     += 4;

	switch( binary_xml_document_data[ 1 ] )
	{
		case 0x01:
			byte_stream_copy_to_uint16_little_endian(
			 &( binary_xml_document_data[ 2 ] ),
			 value_data_size );

#if defined( HAVE_DEBUG_OUTPUT )
			if( libnotify_verbose != 0 )
			{
				libnotify_printf(
				 "%s: number of characters\t\t: %" PRIzd "\n",
				 function,
				 value_data_size );
			}
#endif
			value_data_size *= 2;

			value_type = LIBFVALUE_VALUE_TYPE_STRING_UTF16;

			break;
	}
	if( ( chunk_data_offset + value_data_size ) > chunk_data_size )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid value data size value out of bounds.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: value data:\n",
		 function );
		libnotify_print_data(
		 &( chunk_data[ chunk_data_offset ] ),
		 value_data_size,
		 0 );
	}
#endif
	if( libfvalue_value_initialize(
	     &value,
	     value_type,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create value.",
		 function );

		goto on_error;
	}
	if( libfvalue_value_set_data(
	     value,
	     &( chunk_data[ chunk_data_offset ] ),
	     value_data_size,
	     LIBFVALUE_ENDIAN_LITTLE,
	     LIBFVALUE_VALUE_DATA_FLAG_NON_MANAGED,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set value data.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: value\t\t\t\t: ",
		 function );

		if( libfvalue_debug_print_value(
		     value,
		     0,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print value.",
			 function );

			goto on_error;
		}
		libnotify_printf(
		 "\n" );

		libnotify_printf(
		 "\n" );
	}
#endif
	if( libfvalue_value_free(
	     &value,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free value.",
		 function );

		goto on_error;
	}
	binary_xml_token->size += value_data_size;
	chunk_data_offset      += value_data_size;
	
	return( 1 );

on_error:
	if( value != NULL )
	{
		libfvalue_value_free(
		 &value,
		 NULL );
	}
	return( -1 );
}

