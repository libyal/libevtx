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

#if !defined( _LIBEVTX_XML_TAG_H )
#define _LIBEVTX_XML_TAG_H

#include <common.h>
#include <types.h>

#include <liberror.h>

#include "libevtx_array_type.h"
#include "libevtx_libfvalue.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libevtx_xml_tag libevtx_xml_tag_t;

struct libevtx_xml_tag
{
	/* The name
	 */
	libfvalue_value_t *name;

	/* The attributes array
	 */
	libevtx_array_t *attributes_array;

	/* The value
	 */
	libfvalue_value_t *value;
};

int libevtx_xml_tag_initialize(
     libevtx_xml_tag_t **xml_tag,
     liberror_error_t **error );

int libevtx_xml_tag_free(
     libevtx_xml_tag_t **xml_tag,
     liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

