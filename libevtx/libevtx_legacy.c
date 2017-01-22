/*
 * Legacy functions
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

#include "libevtx_libcerror.h"
#include "libevtx_record.h"
#include "libevtx_record_values.h"

#if !defined( HAVE_LOCAL_LIBEVTX )

/* Parses the record data
 * Returns 1 if successful, 0 if data could not be parsed or -1 on error
 */
int libevtx_record_parse_data(
     libevtx_record_t *record,
     libevtx_template_definition_t *template_definition,
     libcerror_error_t **error )
{
	libevtx_internal_record_t *internal_record = NULL;
	static char *function                      = "libevtx_record_parse_data";
	int result                                 = 0;

	if( record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	internal_record = (libevtx_internal_record_t *) record;

	result = libevtx_record_values_parse_data(
	          internal_record->record_values,
	          internal_record->io_handle,
	          (libevtx_internal_template_definition_t *) template_definition,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GENERIC,
		 "%s: unable to parse data.",
		 function );

		return( -1 );
	}
	return( result );
}

#endif /* !defined( HAVE_LOCAL_LIBEVTX ) */

