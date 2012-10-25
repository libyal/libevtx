/*
 * Template functions
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

#include "libevtx_libcerror.h"
#include "libevtx_template.h"

/* Initializes the template and its values
 * Returns 1 if successful or -1 on error
 */
int libevtx_template_initialize(
     libevtx_template_t **template,
     libcerror_error_t **error )
{
	libevtx_internal_template_t *internal_template = NULL;
	static char *function                          = "libevtx_template_initialize";

	if( template == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid template.",
		 function );

		return( -1 );
	}
	if( *template != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid template value already set.",
		 function );

		return( -1 );
	}
	internal_template = memory_allocate_structure(
	                     libevtx_internal_template_t );

	if( internal_template == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create internal template.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     internal_template,
	     0,
	     sizeof( libevtx_internal_template_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear internal template.",
		 function );

		memory_free(
		 internal_template );

		return( -1 );
	}
	*template = (libevtx_template_t *) internal_template;

	return( 1 );

on_error:
	if( internal_template != NULL )
	{
		memory_free(
		 internal_template );
	}
	return( -1 );
}

/* Frees a template
 * Returns 1 if successful or -1 on error
 */
int libevtx_template_free(
     libevtx_template_t **template,
     libcerror_error_t **error )
{
	libevtx_internal_template_t *internal_template = NULL;
	static char *function                          = "libevtx_template_free";

	if( template == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid template.",
		 function );

		return( -1 );
	}
	if( *template != NULL )
	{
		internal_template = (libevtx_internal_template_t *) *template;
		*template         = NULL;

		memory_free(
		 internal_template );
	}
	return( 1 );
}

