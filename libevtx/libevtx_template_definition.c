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

		if( internal_template_definition->data != NULL )
		{
			memory_free(
			 internal_template_definition->data );
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

/* Sets the data
 * Returns 1 if successful or -1 on error
 */
int libevtx_template_definition_set_data(
     libevtx_template_definition_t *template_definition,
     const uint8_t *data,
     size_t data_size,
     size_t data_offset,
     libcerror_error_t **error )
{
	libevtx_internal_template_definition_t *internal_template_definition = NULL;
	static char *function                                                = "libevtx_template_definition_set_data";

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
	if( internal_template_definition->data != NULL )
	{
		memory_free(
		 internal_template_definition->data );

		internal_template_definition->data      = NULL;
		internal_template_definition->data_size = 0;
	}
	internal_template_definition->data = (uint8_t *) memory_allocate(
	                                                  sizeof( uint8_t ) * data_size );

	if( internal_template_definition->data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create data.",
		 function );

		goto on_error;
	}
	if( memory_copy(
	     internal_template_definition->data,
	     data,
	     data_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy data.",
		 function );

		goto on_error;
	}
	internal_template_definition->data_size = data_size;

	return( 1 );

on_error:
	if( internal_template_definition->data != NULL )
	{
		memory_free(
		 internal_template_definition->data );

		internal_template_definition->data = NULL;
	}
	internal_template_definition->data_size = 0;

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
	if( internal_template_definition->data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid template definition - missing data.",
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
		 "%s: template data:\n",
		 function );
		libcnotify_print_data(
		 internal_template_definition->data,
		 internal_template_definition->data_size,
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
	     internal_template_definition->data,
	     internal_template_definition->data_size,
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

