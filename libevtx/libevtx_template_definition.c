/*
 * Template definition functions
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

#include "libevtx_io_handle.h"
#include "libevtx_libcdata.h"
#include "libevtx_libcerror.h"
#include "libevtx_libcnotify.h"
#include "libevtx_libfwevt.h"
#include "libevtx_template_definition.h"

/* Initializes the template definition and its values
 * Make sure the value template_definition is pointing to is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libevtx_template_definition_initialize(
     libevtx_template_definition_t **template_definition,
     libcerror_error_t **error )
{
	libevtx_internal_template_definition_t *internal_template_definition = NULL;
	static char *function                                                = "libevtx_template_definition_initialize";

	if( template_definition == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid template definition.",
		 function );

		return( -1 );
	}
	if( *template_definition != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid template definition value already set.",
		 function );

		return( -1 );
	}
	internal_template_definition = memory_allocate_structure(
	                                libevtx_internal_template_definition_t );

	if( internal_template_definition == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create template definition.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     internal_template_definition,
	     0,
	     sizeof( libevtx_internal_template_definition_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear template definition.",
		 function );

		goto on_error;
	}
	*template_definition = (libevtx_template_definition_t *) internal_template_definition;

	return( 1 );

on_error:
	if( internal_template_definition != NULL )
	{
		memory_free(
		 internal_template_definition );
	}
	return( -1 );
}

/* Frees a template
 * Returns 1 if successful or -1 on error
 */
int libevtx_template_definition_free(
     libevtx_template_definition_t **template_definition,
     libcerror_error_t **error )
{
	libevtx_internal_template_definition_t *internal_template_definition = NULL;
	static char *function                                                = "libevtx_template_definition_free";
	int result                                                           = 1;

	if( template_definition == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid template.",
		 function );

		return( -1 );
	}
	if( *template_definition != NULL )
	{
		internal_template_definition = (libevtx_internal_template_definition_t *) *template_definition;
		*template_definition         = NULL;

		if( internal_template_definition->binary_xml_data != NULL )
		{
			memory_free(
			 internal_template_definition->binary_xml_data );
		}
		if( internal_template_definition->instance_values_data != NULL )
		{
			memory_free(
			 internal_template_definition->instance_values_data );
		}
		if( internal_template_definition->xml_document != NULL )
		{
			if( libfwevt_xml_document_free(
			     &( internal_template_definition->xml_document ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free XML document.",
				 function );

				result = -1;
			}
		}
		if( libcdata_array_free(
		     &( internal_template_definition->template_values_array ),
		     (int (*)(intptr_t **, libcerror_error_t **)) &libfwevt_xml_template_value_free,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to free template values array.",
			 function );

			result = -1;
		}
		memory_free(
		 internal_template_definition );
	}
	return( result );
}

/* Sets the binary XML data
 * Returns 1 if successful or -1 on error
 */
int libevtx_template_definition_set_binary_xml_data(
     libevtx_template_definition_t *template_definition,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	libevtx_internal_template_definition_t *internal_template_definition = NULL;
	static char *function                                                = "libevtx_template_definition_set_binary_xml_data";

	if( template_definition == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid template definition.",
		 function );

		return( -1 );
	}
	internal_template_definition = (libevtx_internal_template_definition_t *) template_definition;

	if( data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data.",
		 function );

		return( -1 );
	}
	if( data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid data size value exceeds maximum.",
		 function );

		return( -1 );
	}
/* TODO check if already initialized */
	if( internal_template_definition->binary_xml_data != NULL )
	{
		memory_free(
		 internal_template_definition->binary_xml_data );

		internal_template_definition->binary_xml_data      = NULL;
		internal_template_definition->binary_xml_data_size = 0;
	}
	internal_template_definition->binary_xml_data = (uint8_t *) memory_allocate(
	                                                             sizeof( uint8_t ) * data_size );

	if( internal_template_definition->binary_xml_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create binary XML data.",
		 function );

		goto on_error;
	}
	if( memory_copy(
	     internal_template_definition->binary_xml_data,
	     data,
	     data_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy binary XML data.",
		 function );

		goto on_error;
	}
	internal_template_definition->binary_xml_data_size = data_size;

	return( 1 );

on_error:
	if( internal_template_definition->binary_xml_data != NULL )
	{
		memory_free(
		 internal_template_definition->binary_xml_data );

		internal_template_definition->binary_xml_data = NULL;
	}
	internal_template_definition->binary_xml_data_size = 0;

	return( -1 );
}

/* Sets the instance values data
 * Returns 1 if successful or -1 on error
 */
int libevtx_template_definition_set_instance_values_data(
     libevtx_template_definition_t *template_definition,
     int number_of_values,
     size_t base_offset,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	libevtx_internal_template_definition_t *internal_template_definition = NULL;
	static char *function                                                = "libevtx_template_definition_set_instance_values_data";

	if( template_definition == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid template definition.",
		 function );

		return( -1 );
	}
	internal_template_definition = (libevtx_internal_template_definition_t *) template_definition;

	if( data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data.",
		 function );

		return( -1 );
	}
	if( data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid data size value exceeds maximum.",
		 function );

		return( -1 );
	}
/* TODO check if already initialized */
	if( internal_template_definition->instance_values_data != NULL )
	{
		memory_free(
		 internal_template_definition->instance_values_data );

		internal_template_definition->number_of_instance_values        = 0;
		internal_template_definition->instance_values_data_base_offset = 0;
		internal_template_definition->instance_values_data             = NULL;
		internal_template_definition->instance_values_data_size        = 0;
	}
	internal_template_definition->instance_values_data = (uint8_t *) memory_allocate(
	                                                                  sizeof( uint8_t ) * data_size );

	if( internal_template_definition->instance_values_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create instance values data.",
		 function );

		goto on_error;
	}
	if( memory_copy(
	     internal_template_definition->instance_values_data,
	     data,
	     data_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy instance values data.",
		 function );

		goto on_error;
	}
	internal_template_definition->number_of_instance_values        = number_of_values;
	internal_template_definition->instance_values_data_base_offset = base_offset;
	internal_template_definition->instance_values_data_size        = data_size;

	return( 1 );

on_error:
	if( internal_template_definition->instance_values_data != NULL )
	{
		memory_free(
		 internal_template_definition->instance_values_data );

		internal_template_definition->instance_values_data = NULL;
	}
	internal_template_definition->instance_values_data_size = 0;

	return( -1 );
}

/* Reads the template
 * Returns 1 if successful or -1 on error
 */
int libevtx_template_definition_read(
     libevtx_internal_template_definition_t *internal_template_definition,
     libevtx_io_handle_t *io_handle,
     libcerror_error_t **error )
{
	static char *function = "libevtx_template_definition_read";

	if( internal_template_definition == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid template definition.",
		 function );

		return( -1 );
	}
	if( internal_template_definition->binary_xml_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid template definition - missing binary XML data.",
		 function );

		return( -1 );
	}
	if( internal_template_definition->xml_document != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid template definition - XML document already set.",
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
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: template instance values data:\n",
		 function );
		libcnotify_print_data(
		 internal_template_definition->instance_values_data,
		 internal_template_definition->instance_values_data_size,
		 0 );
	}
#endif
	if( internal_template_definition->instance_values_data_size > 0 )
	{
		if( libevtx_template_definition_read_instance_values(
		     internal_template_definition,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read template instance values.",
			 function );

			goto on_error;
		}
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: template binary XML document data:\n",
		 function );
		libcnotify_print_data(
		 internal_template_definition->binary_xml_data,
		 internal_template_definition->binary_xml_data_size,
		 0 );
	}
#endif
	if( libfwevt_xml_document_initialize(
	     &( internal_template_definition->xml_document ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create XML document.",
		 function );

		goto on_error;
	}
	if( libfwevt_xml_document_read(
	     internal_template_definition->xml_document,
	     internal_template_definition->binary_xml_data,
	     internal_template_definition->binary_xml_data_size,
	     0,
	     io_handle->ascii_codepage,
	     0,
	     internal_template_definition->template_values_array,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read binary XML document.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: XML document:\n",
		 function );

		if( libfwevt_xml_document_debug_print(
		     xml_document,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print XML document.",
			 function );

			goto on_error;
		}
		libcnotify_printf(
		 "\n" );
	}
#endif
	return( 1 );

on_error:
	if( internal_template_definition->xml_document != NULL )
	{
		libfwevt_xml_document_free(
		 &( internal_template_definition->xml_document ),
		 NULL );
	}
	if( internal_template_definition->template_values_array != NULL )
	{
		libcdata_array_free(
		 &( internal_template_definition->template_values_array ),
		 (int (*)(intptr_t **, libcerror_error_t **)) &libfwevt_xml_template_value_free,
		 NULL );
	}
	return( -1 );
}

/* Reads the template instance values
 * Returns 1 if successful or -1 on error
 */
int libevtx_template_definition_read_instance_values(
     libevtx_internal_template_definition_t *internal_template_definition,
     libcerror_error_t **error )
{
	libfwevt_xml_template_value_t *template_value = NULL;
	uint8_t *instance_values_data                 = NULL;
	static char *function                         = "libevtx_template_definition_read_instance_values";
	size_t instance_values_data_offset            = 0;
	size_t instance_values_data_size              = 0;
	uint32_t first_template_value_data_offset     = 0;
	uint32_t template_value_data_offset           = 0;
	uint16_t template_value_data_size             = 0;
	uint8_t template_value_type                   = 0;
	int entry_index                               = 0;
	int template_value_index                      = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint32_t value_32bit                          = 0;
	uint16_t value_16bit                          = 0;
#endif

	if( internal_template_definition == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid template definition.",
		 function );

		return( -1 );
	}
	if( internal_template_definition->instance_values_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid template definition - missing instance values data.",
		 function );

		return( -1 );
	}
	if( internal_template_definition->template_values_array != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid template definition - template values array already set.",
		 function );

		return( -1 );
	}
	instance_values_data      = internal_template_definition->instance_values_data;
	instance_values_data_size = internal_template_definition->instance_values_data_size;

	if( libcdata_array_initialize(
	     &( internal_template_definition->template_values_array ),
	     0,
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
/* TODO validate number of values ? */
	do
	{
		if( ( instance_values_data_offset + 20 ) >= instance_values_data_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid binary data too small.",
			 function );

			goto on_error;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: value: %02d header data:\n",
			 function,
			 template_value_index );
			libcnotify_print_data(
			 &( instance_values_data[ instance_values_data_offset ] ),
			 20,
			 0 );
		}
#endif
		template_value_type = instance_values_data[ instance_values_data_offset + 4 ];

		byte_stream_copy_to_uint32_little_endian(
		 &( instance_values_data[ instance_values_data_offset + 16 ] ),
		 template_value_data_offset );

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			byte_stream_copy_to_uint32_little_endian(
			 &( instance_values_data[ instance_values_data_offset ] ),
			 value_32bit );
			libcnotify_printf(
			 "%s: value: %02d unknown1\t: 0x%08" PRIx32 "\n",
			 function,
			 template_value_index,
			 value_32bit );

			libcnotify_printf(
			 "%s: value: %02d type\t\t: 0x%02" PRIx8 " (",
			 function,
			 template_value_index,
			 template_value_type );
			libfwevt_debug_print_value_type(
			 template_value_type );
			libcnotify_printf(
			 ")\n" );

			libcnotify_printf(
			 "%s: value: %02d unknown2\t: 0x%02" PRIx8 "\n",
			 function,
			 template_value_index,
			 instance_values_data[ instance_values_data_offset + 5 ] );

			byte_stream_copy_to_uint16_little_endian(
			 &( instance_values_data[ instance_values_data_offset + 6 ] ),
			 value_16bit );
			libcnotify_printf(
			 "%s: value: %02d unknown3\t: 0x%04" PRIx16 "\n",
			 function,
			 template_value_index,
			 value_16bit );

			byte_stream_copy_to_uint32_little_endian(
			 &( instance_values_data[ instance_values_data_offset + 8 ] ),
			 value_32bit );
			libcnotify_printf(
			 "%s: value: %02d unknown4\t: 0x%08" PRIx32 "\n",
			 function,
			 template_value_index,
			 value_32bit );

			byte_stream_copy_to_uint32_little_endian(
			 &( instance_values_data[ instance_values_data_offset + 12 ] ),
			 value_32bit );
			libcnotify_printf(
			 "%s: value: %02d unknown5\t: 0x%08" PRIx32 "\n",
			 function,
			 template_value_index,
			 value_32bit );

			libcnotify_printf(
			 "%s: value: %02d offset\t\t: 0x%08" PRIx32 "\n",
			 function,
			 template_value_index,
			 template_value_data_offset );

			libcnotify_printf(
			 "\n" );
		}
#endif
		instance_values_data_offset += 20;

		if( template_value_data_offset < internal_template_definition->instance_values_data_base_offset )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid template value data offset value out of bounds.",
			 function );

			goto on_error;
		}
		template_value_data_offset -= internal_template_definition->instance_values_data_base_offset;

		if( template_value_data_offset >= instance_values_data_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid template value data offset value out of bounds.",
			 function );

			goto on_error;
		}
		if( ( template_value_data_offset + 4 ) >= instance_values_data_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid binary data too small.",
			 function );

			goto on_error;
		}
		if( first_template_value_data_offset == 0 )
		{
			first_template_value_data_offset = template_value_data_offset;
		}
		byte_stream_copy_to_uint32_little_endian(
		 &( instance_values_data[ template_value_data_offset ] ),
		 template_value_data_size );

		if( ( template_value_data_offset + template_value_data_size ) >= instance_values_data_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid binary data too small.",
			 function );

			goto on_error;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: value: %02d value data:\n",
			 function,
			 template_value_index );
			libcnotify_print_data(
			 &( instance_values_data[ template_value_data_offset ] ),
			 template_value_data_size,
			 0 );
		}
#endif
		if( template_value_data_size > 0 )
		{
			if( template_value_data_size < 4 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: invalid template value data size value out of bounds.",
				 function );

				goto on_error;
			}
			template_value_data_size -= 4;
		}
		template_value_data_offset += 4;

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: value: %02d size\t\t: %" PRIu32 "\n",
			 function,
			 template_value_index,
			 template_value_data_size );

			libcnotify_printf(
			 "%s: value: %02d data:\n",
			 function,
			 template_value_index );
			libcnotify_print_data(
			 &( instance_values_data[ template_value_data_offset ] ),
			 template_value_data_size,
			 0 );

			libcnotify_printf(
			 "\n" );
		}
#endif
		if( libfwevt_xml_template_value_initialize(
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
/* TODO set value to override value type */
		if( libfwevt_xml_template_value_set_type(
		     template_value,
		     0x01,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set template value type.",
			 function );

			goto on_error;
		}
		if( libfwevt_xml_template_value_set_offset(
		     template_value,
		     template_value_data_offset,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set template value data offset.",
			 function );

			goto on_error;
		}
		if( libfwevt_xml_template_value_set_size(
		     template_value,
		     template_value_data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set template value data size.",
			 function );

			goto on_error;
		}
		if( libcdata_array_append_entry(
		     internal_template_definition->template_values_array,
		     &entry_index,
		     (intptr_t *) template_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to append template value: %d to array.",
			 function,
			 template_value_index );

			goto on_error;
		}
		template_value = NULL;

		template_value_index++;
	}
	while( instance_values_data_offset < first_template_value_data_offset );

	return( 1 );

on_error:
	if( template_value != NULL )
	{
		libfwevt_xml_template_value_free(
		 &template_value,
		 NULL );
	}
	if( internal_template_definition->template_values_array != NULL )
	{
		libcdata_array_free(
		 &( internal_template_definition->template_values_array ),
		 (int (*)(intptr_t **, libcerror_error_t **)) &libfwevt_xml_template_value_free,
		 NULL );
	}
	return( -1 );
}

