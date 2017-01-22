/*
 * Template definition functions
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
#include <types.h>

#include "libevtx_io_handle.h"
#include "libevtx_libcdata.h"
#include "libevtx_libcerror.h"
#include "libevtx_libcnotify.h"
#include "libevtx_libfwevt.h"
#include "libevtx_template_definition.h"

/* Creates a template definition
 * Make sure the value template_definition is referencing, is set to NULL
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
	if( libfwevt_template_initialize(
	     &( internal_template_definition->wevt_template ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create WEVT template.",
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

/* Frees a template definition
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

		if( libfwevt_template_free(
		     &( internal_template_definition->wevt_template ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free WEVT template.",
			 function );

			result = -1;
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
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to free XML document.",
				 function );

				result = -1;
			}
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
     uint32_t data_offset,
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

	if( libfwevt_template_set_offset(
	     internal_template_definition->wevt_template,
	     data_offset,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set template offset.",
		 function );

		return( -1 );
	}
	if( libfwevt_template_set_data(
	     internal_template_definition->wevt_template,
	     data,
	     data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set template data.",
		 function );

		return( -1 );
	}
	return( 1 );
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
	if( libfwevt_template_set_ascii_codepage(
	     internal_template_definition->wevt_template,
	     io_handle->ascii_codepage,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set ASCII codepage in template.",
		 function );

		goto on_error;
	}
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
	if( libfwevt_template_read_xml_document(
	     internal_template_definition->wevt_template,
	     internal_template_definition->xml_document,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read XML document from template.",
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
		     internal_template_definition->xml_document,
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
	return( -1 );
}

