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

#include "libevtx_array_type.h"
#include "libevtx_libcerror.h"
#include "libevtx_libcnotify.h"
#include "libevtx_libfvalue.h"
#include "libevtx_xml_tag.h"

/* Initialize XML tag
 * Make sure the value XML tag is pointing to is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libevtx_xml_tag_initialize(
     libevtx_xml_tag_t **xml_tag,
     libcerror_error_t **error )
{
	static char *function = "libevtx_xml_tag_initialize";

	if( xml_tag == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid XML tag.",
		 function );

		return( -1 );
	}
	if( *xml_tag != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid XML tag value already set.",
		 function );

		return( -1 );
	}
	*xml_tag = memory_allocate_structure(
	            libevtx_xml_tag_t );

	if( *xml_tag == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create XML tag.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *xml_tag,
	     0,
	     sizeof( libevtx_xml_tag_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear XML tag.",
		 function );

		memory_free(
		 *xml_tag );

		*xml_tag = NULL;

		return( -1 );
	}
	if( libevtx_array_initialize(
	     &( ( *xml_tag )->attributes_array ),
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create attributes array.",
		 function );

		goto on_error;
	}
	if( libevtx_array_initialize(
	     &( ( *xml_tag )->elements_array ),
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create elements array.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *xml_tag != NULL )
	{
		if( ( *xml_tag )->attributes_array != NULL )
		{
			libevtx_array_free(
			 &( ( *xml_tag )->attributes_array ),
			 NULL,
			 NULL );
		}
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
     libcerror_error_t **error )
{
	static char *function = "libevtx_xml_tag_free";
	int result            = 1;

	if( xml_tag == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid XML tag.",
		 function );

		return( -1 );
	}
	if( *xml_tag != NULL )
	{
		if( libevtx_array_free(
		     &( ( *xml_tag )->elements_array ),
		     (int (*)(intptr_t **, libcerror_error_t **)) &libevtx_xml_tag_free,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free elements array.",
			 function );

			result = -1;
		}
		if( libevtx_array_free(
		     &( ( *xml_tag )->attributes_array ),
		     (int (*)(intptr_t **, libcerror_error_t **)) &libevtx_xml_tag_free,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free attributes array.",
			 function );

			result = -1;
		}
		if( ( *xml_tag )->value != NULL )
		{
			if( libfvalue_value_free(
			     &( ( *xml_tag )->value ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free value.",
				 function );

				result = -1;
			}
		}
		if( ( *xml_tag )->name != NULL )
		{
			if( libfvalue_value_free(
			     &( ( *xml_tag )->name ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free name.",
				 function );

				result = -1;
			}
		}
		memory_free(
		 *xml_tag );

		*xml_tag = NULL;
	}
	return( result );
}

/* Appends an attribute
 * Returns 1 if successful or -1 on error
 */
int libevtx_xml_tag_append_attribute(
     libevtx_xml_tag_t *xml_tag,
     libevtx_xml_tag_t *attribute_xml_tag,
     libcerror_error_t **error )
{
	static char *function = "libevtx_xml_tag_free";
	int entry_index       = 0;

	if( xml_tag == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid XML tag.",
		 function );

		return( -1 );
	}
	if( libevtx_array_append_entry(
	     xml_tag->attributes_array,
	     &entry_index,
	     (intptr_t *) attribute_xml_tag,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append attribute XML tag to array.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Appends an element
 * Returns 1 if successful or -1 on error
 */
int libevtx_xml_tag_append_element(
     libevtx_xml_tag_t *xml_tag,
     libevtx_xml_tag_t *element_xml_tag,
     libcerror_error_t **error )
{
	static char *function = "libevtx_xml_tag_free";
	int entry_index       = 0;

	if( xml_tag == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid XML tag.",
		 function );

		return( -1 );
	}
	if( libevtx_array_append_entry(
	     xml_tag->elements_array,
	     &entry_index,
	     (intptr_t *) element_xml_tag,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append element XML tag to array.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of UTF-8 formatted string of the XML tag 
 * Returns 1 if successful or -1 on error
 */
int libevtx_xml_tag_get_utf8_xml_string_size(
     libevtx_xml_tag_t *xml_tag,
     int xml_tag_level,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	libevtx_xml_tag_t *attribute_xml_tag = NULL;
	libevtx_xml_tag_t *element_xml_tag   = NULL;
	static char *function                = "libevtx_xml_tag_get_utf8_xml_string_size";
	size_t name_size                     = 0;
	size_t string_size                   = 0;
	int attribute_index                  = 0;
	int element_index                    = 0;
	int number_of_attributes             = 0;
	int number_of_elements               = 0;

	if( xml_tag == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid XML tag.",
		 function );

		return( -1 );
	}
	if( utf8_string_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-8 string size.",
		 function );

		return( -1 );
	}
	if( libevtx_array_get_number_of_entries(
	     xml_tag->attributes_array,
	     &number_of_attributes,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of attributes.",
		 function );

		return( -1 );
	}
	if( libevtx_array_get_number_of_entries(
	     xml_tag->elements_array,
	     &number_of_elements,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of elements.",
		 function );

		return( -1 );
	}
	if( libfvalue_value_get_utf8_string_size(
	     xml_tag->name,
	     0,
	     &name_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 string size of name.",
		 function );

		return( -1 );
	}
	/* The size of:
	 *   2 x ' ' character per indentation level
	 *   1 x '<' character
	 *   element name
	 *   1 x '>' character
	 *   1 x '\n' character
	 */
	*utf8_string_size = ( xml_tag_level * 2 ) + name_size + 3;

	if( number_of_elements == 0 )
	{
		/* The size of:
		 *   1 x '/' character
		 */
		*utf8_string_size += 1;
	}
	else
	{
		/* The size of:
		 *   2 x ' ' character per indentation level
		 *   1 x '<' character
		 *   1 x '/' character
		 *   element name
		 *   1 x '>' character
		 *   1 x '\n' character
		 */
		*utf8_string_size += ( xml_tag_level * 2 ) + name_size + 4;
	}
	if( number_of_attributes > 0 )
	{
		for( attribute_index = 0;
		     attribute_index < number_of_attributes;
		     attribute_index++ )
		{
			if( libevtx_array_get_entry_by_index(
			     xml_tag->attributes_array,
			     attribute_index,
			     (intptr_t **) &attribute_xml_tag,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve attribute: %d.",
				 function,
				 attribute_index );

				return( -1 );
			}
			if( attribute_xml_tag == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: missing attribute: %d.",
				 function,
				 attribute_index );

				return( -1 );
			}
			if( libfvalue_value_get_utf8_string_size(
			     attribute_xml_tag->name,
			     0,
			     &string_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve UTF-8 string size of attribute: %d name.",
				 function,
				 attribute_index );

				return( -1 );
			}
			/* The size of:
			 *   1 x ' ' character
			 *   attribute name
			 *   1 x '=' character
			 *   2 x '"' character
			 */
			*utf8_string_size += string_size + 4;

			if( libfvalue_value_get_utf8_string_size(
			     attribute_xml_tag->value,
			     0,
			     &string_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve UTF-8 string size of attribute: %d value.",
				 function,
				 attribute_index );

				return( -1 );
			}
		}
	}
	if( xml_tag->value != NULL )
	{
		if( libfvalue_value_get_utf8_string_size(
		     xml_tag->value,
		     0,
		     &string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve UTF-8 string size of value.",
			 function );

			return( -1 );
		}
		/* The size of:
		 *   value formatted as a string
		 */
		*utf8_string_size += string_size;
	}
	else if( number_of_elements > 0 )
	{
		for( element_index = 0;
		     element_index < number_of_elements;
		     element_index++ )
		{
			if( libevtx_array_get_entry_by_index(
			     xml_tag->elements_array,
			     element_index,
			     (intptr_t **) &element_xml_tag,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve sub element: %d.",
				 function,
				 element_index );

				return( -1 );
			}
			if( libevtx_xml_tag_get_utf8_xml_string_size(
			     element_xml_tag,
			     xml_tag_level + 1,
			     &string_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve UTF-8 string size of sub element: %d.",
				 function,
				 element_index );

				return( -1 );
			}
			/* The size of:
			 *   sub element formatted as a string
			 */
			*utf8_string_size += string_size;
		}
	}
	return( 1 );
}

/* Retrieves the UTF-8 formatted string of the XML tag 
 * Returns 1 if successful or -1 on error
 */
int libevtx_xml_tag_get_utf8_xml_string(
     libevtx_xml_tag_t *xml_tag,
     int xml_tag_level,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	libevtx_xml_tag_t *attribute_xml_tag = NULL;
	libevtx_xml_tag_t *element_xml_tag   = NULL;
	static char *function                = "libevtx_xml_tag_get_utf8_xml_string_size";
	size_t name_size                     = 0;
	size_t string_index                  = 0;
	size_t string_size                   = 0;
	int attribute_index                  = 0;
	int element_index                    = 0;
	int indentation_iterator             = 0;
	int number_of_attributes             = 0;
	int number_of_elements               = 0;

	if( xml_tag == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid XML tag.",
		 function );

		return( -1 );
	}
	if( utf8_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-8 string.",
		 function );

		return( -1 );
	}
	if( utf8_string_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UTF-8 string size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( libevtx_array_get_number_of_entries(
	     xml_tag->attributes_array,
	     &number_of_attributes,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of attributes.",
		 function );

		return( -1 );
	}
	if( libevtx_array_get_number_of_entries(
	     xml_tag->elements_array,
	     &number_of_elements,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of elements.",
		 function );

		return( -1 );
	}
	if( libfvalue_value_get_utf8_string_size(
	     xml_tag->name,
	     0,
	     &name_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 string size of name.",
		 function );

		return( -1 );
	}
	if( ( string_index + ( xml_tag_level * 2 ) + 1 ) > utf8_string_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: UTF-8 string size too small.",
		 function );

		return( -1 );
	}
	for( indentation_iterator = 0;
	     indentation_iterator < xml_tag_level;
	     indentation_iterator++ )
	{
		utf8_string[ string_index++ ] = (uint8_t) ' ';
		utf8_string[ string_index++ ] = (uint8_t) ' ';
	}
	utf8_string[ string_index++ ] = (uint8_t) '<';

	if( libfvalue_value_copy_to_utf8_string(
	     xml_tag->name,
	     0,
	     &( utf8_string[ string_index ] ),
	     utf8_string_size - string_index,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy name to UTF-8 string.",
		 function );

		return( -1 );
	}
	string_index += name_size;

	if( number_of_attributes > 0 )
	{
		for( attribute_index = 0;
		     attribute_index < number_of_attributes;
		     attribute_index++ )
		{
			if( libevtx_array_get_entry_by_index(
			     xml_tag->attributes_array,
			     attribute_index,
			     (intptr_t **) &attribute_xml_tag,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve attribute: %d.",
				 function,
				 attribute_index );

				return( -1 );
			}
			if( attribute_xml_tag == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: missing attribute: %d.",
				 function,
				 attribute_index );

				return( -1 );
			}
			if( ( string_index + 1 ) > utf8_string_size )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
				 "%s: UTF-8 string size too small.",
				 function );

				return( -1 );
			}
			utf8_string[ string_index++ ] = (uint8_t) ' ';

			if( libfvalue_value_get_utf8_string_size(
			     attribute_xml_tag->name,
			     0,
			     &string_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve UTF-8 string size of attribute: %d name.",
				 function,
				 attribute_index );

				return( -1 );
			}
			if( libfvalue_value_copy_to_utf8_string(
			     attribute_xml_tag->name,
			     0,
			     &( utf8_string[ string_index ] ),
			     utf8_string_size - string_index,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy attribute: %d name to UTF-8 string.",
				 function,
				 attribute_index );

				return( -1 );
			}
			string_index += name_size;

			if( ( string_index + 2 ) > utf8_string_size )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
				 "%s: UTF-8 string size too small.",
				 function );

				return( -1 );
			}
			utf8_string[ string_index++ ] = (uint8_t) '=';
			utf8_string[ string_index++ ] = (uint8_t) '"';

			if( libfvalue_value_get_utf8_string_size(
			     attribute_xml_tag->value,
			     0,
			     &string_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve UTF-8 string size of attribute: %d value.",
				 function,
				 attribute_index );

				return( -1 );
			}
			if( libfvalue_value_copy_to_utf8_string(
			     attribute_xml_tag->value,
			     0,
			     &( utf8_string[ string_index ] ),
			     utf8_string_size - string_index,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy attribute: %d value to UTF-8 string.",
				 function,
				 attribute_index );

				return( -1 );
			}
			string_index += name_size;

			if( ( string_index + 1 ) > utf8_string_size )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
				 "%s: UTF-8 string size too small.",
				 function );

				return( -1 );
			}
			utf8_string[ string_index++ ] = (uint8_t) '"';
		}
	}
	if( ( xml_tag->value != NULL )
	 || ( number_of_elements > 0 ) )
	{
		if( ( string_index + 2 ) > utf8_string_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
			 "%s: UTF-8 string size too small.",
			 function );

			return( -1 );
		}
		utf8_string[ string_index++ ] = (uint8_t) '>';
		utf8_string[ string_index++ ] = (uint8_t) '\n';
	}
	if( xml_tag->value != NULL )
	{
		if( libfvalue_value_get_utf8_string_size(
		     xml_tag->value,
		     0,
		     &string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve UTF-8 string size of value.",
			 function );

			return( -1 );
		}
		if( libfvalue_value_copy_to_utf8_string(
		     xml_tag->value,
		     0,
		     &( utf8_string[ string_index ] ),
		     utf8_string_size - string_index,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy value to UTF-8 string.",
			 function );

			return( -1 );
		}
		string_index += name_size;
	}
	else if( number_of_elements > 0 )
	{
		for( element_index = 0;
		     element_index < number_of_elements;
		     element_index++ )
		{
			if( libevtx_array_get_entry_by_index(
			     xml_tag->elements_array,
			     element_index,
			     (intptr_t **) &element_xml_tag,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve sub element: %d.",
				 function,
				 element_index );

				return( -1 );
			}
			if( libevtx_xml_tag_get_utf8_xml_string_size(
			     element_xml_tag,
			     xml_tag_level + 1,
			     &string_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve UTF-8 string size of sub element: %d.",
				 function,
				 element_index );

				return( -1 );
			}
			if( libevtx_xml_tag_get_utf8_xml_string(
			     element_xml_tag,
			     xml_tag_level + 1,
			     &( utf8_string[ string_index ] ),
			     utf8_string_size - string_index,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to retrieve copy sub element: %d to UTF-8 string.",
				 function,
				 element_index );

				return( -1 );
			}
			string_index += string_size;
		}
		if( ( string_index + ( xml_tag_level * 2 ) ) > utf8_string_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
			 "%s: UTF-8 string size too small.",
			 function );

			return( -1 );
		}
		for( indentation_iterator = 0;
		     indentation_iterator < xml_tag_level;
		     indentation_iterator++ )
		{
			utf8_string[ string_index++ ] = (uint8_t) ' ';
			utf8_string[ string_index++ ] = (uint8_t) ' ';
		}
	}
	if( ( xml_tag->value != NULL )
	 || ( number_of_elements > 0 ) )
	{
		if( ( string_index + 2 ) > utf8_string_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
			 "%s: UTF-8 string size too small.",
			 function );

			return( -1 );
		}
		utf8_string[ string_index++ ] = (uint8_t) '<';
		utf8_string[ string_index++ ] = (uint8_t) '/';

		if( libfvalue_value_copy_to_utf8_string(
		     xml_tag->name,
		     0,
		     &( utf8_string[ string_index ] ),
		     utf8_string_size - string_index,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy name to UTF-8 string.",
			 function );

			return( -1 );
		}
		string_index += name_size;
	}
	else
	{
		if( ( string_index + 1 ) > utf8_string_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
			 "%s: UTF-8 string size too small.",
			 function );

			return( -1 );
		}
		utf8_string[ string_index++ ] = (uint8_t) '/';
	}
	if( ( string_index + 2 ) > utf8_string_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: UTF-8 string size too small.",
		 function );

		return( -1 );
	}
	utf8_string[ string_index++ ] = (uint8_t) '>';
	utf8_string[ string_index++ ] = (uint8_t) '\n';

	return( 1 );
}

/* Retrieves the size of UTF-16 formatted string of the XML tag 
 * Returns 1 if successful or -1 on error
 */
int libevtx_xml_tag_get_utf16_xml_string_size(
     libevtx_xml_tag_t *xml_tag,
     int xml_tag_level,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
/* TODO */
	return( 1 );
}

/* Retrieves the UTF-16 formatted string of the XML tag 
 * Returns 1 if successful or -1 on error
 */
int libevtx_xml_tag_get_utf16_xml_string(
     libevtx_xml_tag_t *xml_tag,
     int xml_tag_level,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
/* TODO */
	return( 1 );
}

#if defined( HAVE_DEBUG_OUTPUT )

/* Debug prints the XML tag
 * Returns 1 if successful or -1 on error
 */
int libevtx_xml_tag_debug_print(
     libevtx_xml_tag_t *xml_tag,
     int xml_tag_level,
     libcerror_error_t **error )
{
	libevtx_xml_tag_t *attribute_xml_tag = NULL;
	libevtx_xml_tag_t *element_xml_tag   = NULL;
	static char *function                = "libevtx_xml_tag_debug_print";
	int attribute_index                  = 0;
	int element_index                    = 0;
	int indentation_iterator             = 0;
	int number_of_attributes             = 0;
	int number_of_elements               = 0;

	if( xml_tag == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid XML tag.",
		 function );

		return( -1 );
	}
	if( libevtx_array_get_number_of_entries(
	     xml_tag->attributes_array,
	     &number_of_attributes,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of attributes.",
		 function );

		return( -1 );
	}
	if( libevtx_array_get_number_of_entries(
	     xml_tag->elements_array,
	     &number_of_elements,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of elements.",
		 function );

		return( -1 );
	}
	for( indentation_iterator = 0;
	     indentation_iterator < xml_tag_level;
	     indentation_iterator++ )
	{
		libcnotify_printf(
		 "  " );
	}
	libcnotify_printf(
	 "<" );

	if( libfvalue_debug_print_value(
	     xml_tag->name,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
		 "%s: unable to print name.",
		 function );

		return( -1 );
	}
	if( number_of_attributes > 0 )
	{
		libcnotify_printf(
		 " " );

		for( attribute_index = 0;
		     attribute_index < number_of_attributes;
		     attribute_index++ )
		{
			if( libevtx_array_get_entry_by_index(
			     xml_tag->attributes_array,
			     attribute_index,
			     (intptr_t **) &attribute_xml_tag,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve attribute: %d.",
				 function,
				 attribute_index );

				return( -1 );
			}
			if( attribute_xml_tag == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: missing attribute: %d.",
				 function,
				 attribute_index );

				return( -1 );
			}
			if( attribute_index != 0 )
			{
				for( indentation_iterator = 0;
				     indentation_iterator < ( xml_tag_level + 1 );
				     indentation_iterator++ )
				{
					libcnotify_printf(
					 "  " );
				}
			}
			if( libfvalue_debug_print_value(
			     attribute_xml_tag->name,
			     0,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
				 "%s: unable to print attribute name.",
				 function );

				return( -1 );
			}
			libcnotify_printf(
			 "=\"" );

/* TODO as long as substitutions are not handled this is necessary */
			if( attribute_xml_tag->value != NULL )
			{
				if( libfvalue_debug_print_value(
				     attribute_xml_tag->value,
				     0,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
					 "%s: unable to print attribute value.",
					 function );

					return( -1 );
				}
			}
			libcnotify_printf(
			 "\"" );

			if( ( ( attribute_index + 1 ) != number_of_attributes )
			 && ( ( attribute_index > 0 )
			  ||  ( number_of_attributes > 1 ) ) )
			{
				libcnotify_printf(
				 "\n" );
			}
		}
	}
	if( ( xml_tag->value == NULL )
	 && ( number_of_elements == 0 ) )
	{
		libcnotify_printf(
		 "/" );
	}
	libcnotify_printf(
	 ">" );

	if( xml_tag->value != NULL )
	{
		if( libfvalue_debug_print_value(
		     xml_tag->value,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print value.",
			 function );

			return( -1 );
		}
	}
	else
	{
		libcnotify_printf(
		 "\n" );

		for( element_index = 0;
		     element_index < number_of_elements;
		     element_index++ )
		{
			if( libevtx_array_get_entry_by_index(
			     xml_tag->elements_array,
			     element_index,
			     (intptr_t **) &element_xml_tag,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve element: %d.",
				 function,
				 element_index );

				return( -1 );
			}
			if( libevtx_xml_tag_debug_print(
			     element_xml_tag,
			     xml_tag_level + 1,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
				 "%s: unable to print element: %d.",
				 function,
				 element_index );

				return( -1 );
			}
		}
	}
	if( number_of_elements > 0 )
	{
		for( indentation_iterator = 0;
		     indentation_iterator < xml_tag_level;
		     indentation_iterator++ )
		{
			libcnotify_printf(
			 "  " );
		}
	}
	if( ( xml_tag->value != NULL )
	 || ( number_of_elements > 0 ) )
	{
		libcnotify_printf(
		 "<" );

		if( libfvalue_debug_print_value(
		     xml_tag->name,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print name.",
			 function );

			return( -1 );
		}
		libcnotify_printf(
		 "/>\n" );
	}
	if( xml_tag_level == 0 )
	{
		libcnotify_printf(
		 "\n" );
	}
	return( 1 );
}

#endif

