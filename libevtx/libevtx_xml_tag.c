/*
 * XML tag functions
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

#include <liberror.h>

#include "libevtx_array_type.h"
#include "libevtx_libfvalue.h"
#include "libevtx_xml_tag.h"

/* Initialize XML tag
 * Make sure the value XML tag is pointing to is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libevtx_xml_tag_initialize(
     libevtx_xml_tag_t **xml_tag,
     liberror_error_t **error )
{
	static char *function = "libevtx_xml_tag_initialize";

	if( xml_tag == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid binary XML tag.",
		 function );

		return( -1 );
	}
	if( *xml_tag != NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid binary XML tag value already set.",
		 function );

		return( -1 );
	}
	*xml_tag = memory_allocate_structure(
	            libevtx_xml_tag_t );

	if( *xml_tag == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create binary XML tag.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     ( *xml_tag ),
	     0,
	     sizeof( libevtx_xml_tag_t ) ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear binary XML tag.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *xml_tag != NULL )
	{
		memory_free(
		 *xml_tag );

		*xml_tag = NULL;
	}
	return( -1 );
}

/* Frees XML tag
 * Returns 1 if successful or -1 on error
 */
int libevtx_xml_tag_free(
     libevtx_xml_tag_t **xml_tag,
     liberror_error_t **error )
{
	static char *function = "libevtx_xml_tag_free";
	int result            = 1;

	if( xml_tag == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid binary XML tag.",
		 function );

		return( -1 );
	}
	if( *xml_tag != NULL )
	{
		memory_free(
		 *xml_tag );

		*xml_tag = NULL;
	}
	return( result );
}

