/*
 * Binary XML template value functions
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
#include <memory.h>
#include <types.h>

#include "libevtx_binary_xml_template_value.h"
#include "libevtx_libcerror.h"

/* Initialize binary XML template value
 * Make sure the value binary XML template value is pointing to is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libevtx_binary_xml_template_value_initialize(
     libevtx_binary_xml_template_value_t **binary_xml_template_value,
     libcerror_error_t **error )
{
	static char *function = "libevtx_binary_xml_template_value_initialize";

	if( binary_xml_template_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid binary XML template value.",
		 function );

		return( -1 );
	}
	if( *binary_xml_template_value != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid binary XML template value value already set.",
		 function );

		return( -1 );
	}
	*binary_xml_template_value = memory_allocate_structure(
	                              libevtx_binary_xml_template_value_t );

	if( *binary_xml_template_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create binary XML template value.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     ( *binary_xml_template_value ),
	     0,
	     sizeof( libevtx_binary_xml_template_value_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear binary XML template value.",
		 function );


		goto on_error;
	}
	return( 1 );

on_error:
	if( *binary_xml_template_value != NULL )
	{
		memory_free(
		 *binary_xml_template_value );

		*binary_xml_template_value = NULL;
	}
	return( -1 );
}

/* Frees binary XML template value
 * Returns 1 if successful or -1 on error
 */
int libevtx_binary_xml_template_value_free(
     libevtx_binary_xml_template_value_t **binary_xml_template_value,
     libcerror_error_t **error )
{
	static char *function = "libevtx_binary_xml_template_value_free";

	if( binary_xml_template_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid binary XML template value.",
		 function );

		return( -1 );
	}
	if( *binary_xml_template_value != NULL )
	{
		memory_free(
		 *binary_xml_template_value );

		*binary_xml_template_value = NULL;
	}
	return( 1 );
}

