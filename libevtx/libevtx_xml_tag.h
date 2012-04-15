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

#include "libevtx_array_type.h"
#include "libevtx_libcerror.h"
#include "libevtx_libfvalue.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libevtx_xml_tag libevtx_xml_tag_t;

struct libevtx_xml_tag
{
	/* The name (value)
	 */
	libfvalue_value_t *name;

	/* The value (value)
	 */
	libfvalue_value_t *value;

	/* The attributes array
	 */
	libevtx_array_t *attributes_array;

	/* The elements array
	 */
	libevtx_array_t *elements_array;
};

int libevtx_xml_tag_initialize(
     libevtx_xml_tag_t **xml_tag,
     libcerror_error_t **error );

int libevtx_xml_tag_free(
     libevtx_xml_tag_t **xml_tag,
     libcerror_error_t **error );

int libevtx_xml_tag_append_attribute(
     libevtx_xml_tag_t *xml_tag,
     libevtx_xml_tag_t *attribute_xml_tag,
     libcerror_error_t **error );

int libevtx_xml_tag_append_element(
     libevtx_xml_tag_t *xml_tag,
     libevtx_xml_tag_t *element_xml_tag,
     libcerror_error_t **error );

int libevtx_xml_tag_get_utf8_xml_string_size(
     libevtx_xml_tag_t *xml_tag,
     int xml_tag_level,
     size_t *utf8_string_size,
     libcerror_error_t **error );

int libevtx_xml_tag_get_utf8_xml_string_with_index(
     libevtx_xml_tag_t *xml_tag,
     int xml_tag_level,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     size_t *utf8_string_index,
     libcerror_error_t **error );

int libevtx_xml_tag_get_utf16_xml_string_size(
     libevtx_xml_tag_t *xml_tag,
     int xml_tag_level,
     size_t *utf16_string_size,
     libcerror_error_t **error );

int libevtx_xml_tag_get_utf16_xml_string_with_index(
     libevtx_xml_tag_t *xml_tag,
     int xml_tag_level,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     size_t *utf16_string_index,
     libcerror_error_t **error );

#if defined( HAVE_DEBUG_OUTPUT )
int libevtx_xml_tag_debug_print(
     libevtx_xml_tag_t *xml_tag,
     int xml_tag_level,
     libcerror_error_t **error );
#endif

#if defined( __cplusplus )
}
#endif

#endif

