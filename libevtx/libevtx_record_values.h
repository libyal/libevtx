/*
 * Record values functions
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

#if !defined( _LIBEVTX_RECORD_VALUES_H )
#define _LIBEVTX_RECORD_VALUES_H

#include <common.h>
#include <types.h>

#include "libevtx_io_handle.h"
#include "libevtx_libcdata.h"
#include "libevtx_libcerror.h"
#include "libevtx_libfvalue.h"
#include "libevtx_libfwevt.h"
#include "libevtx_template_definition.h"
#include "libevtx_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

extern const uint8_t evtx_event_record_signature[ 4 ];

typedef struct libevtx_record_values libevtx_record_values_t;

struct libevtx_record_values
{
	/* The offset
	 */
	off64_t offset;

	/* The data size
	 */
	uint32_t data_size;

	/* The chunk data offset
	 */
	size_t chunk_data_offset;

	/* The identifier
	 */
	uint64_t identifier;

	/* The written time
	 */
	uint64_t written_time;

	/* The XML document
	 */
	libfwevt_xml_document_t *xml_document;

	/* Reference to the provider XML tag
	 */
	libfwevt_xml_tag_t *provider_xml_tag;

	/* Reference to the provider identifier value
	 */
	libfvalue_value_t *provider_identifier_value;

	/* Reference to the provider name value
	 */
	libfvalue_value_t *provider_name_value;

	/* Reference to the event identifier XML tag
	 */
	libfwevt_xml_tag_t *event_identifier_xml_tag;

	/* Reference to the level value
	 */
	libfvalue_value_t *level_value;

	/* Reference to the task value
	 */
	libfvalue_value_t *task_value;

	/* Reference to the oppcode value
	 */
	libfvalue_value_t *oppcode_value;

	/* Reference to the keywords value
	 */
	libfvalue_value_t *keywords_value;

	/* Reference to the channel value
	 */
	libfvalue_value_t *channel_value;

	/* Reference to the computer value
	 */
	libfvalue_value_t *computer_value;

	/* Reference to the user security identifier (SID) value
	 */
	libfvalue_value_t *user_security_identifier_value;

	/* The string identifiers array
	 */
	libcdata_array_t *string_identifiers_array;

	/* The strings array
	 */
	libcdata_array_t *strings_array;

	/* Reference to the BinaryData value
	 */
	libfvalue_value_t *binary_data_value;

	/* Value to indicate the data was parsed
	 */
	uint8_t data_parsed;
};

int libevtx_record_values_initialize(
     libevtx_record_values_t **record_values,
     libcerror_error_t **error );

int libevtx_record_values_free(
     libevtx_record_values_t **record_values,
     libcerror_error_t **error );

int libevtx_record_values_clone(
     libevtx_record_values_t **destination_record_values,
     libevtx_record_values_t *source_record_values,
     libcerror_error_t **error );

int libevtx_record_values_read_header(
     libevtx_record_values_t *record_values,
     libevtx_io_handle_t *io_handle,
     const uint8_t *chunk_data,
     size_t chunk_data_size,
     size_t chunk_data_offset,
     libcerror_error_t **error );

int libevtx_record_values_read_xml_document(
     libevtx_record_values_t *record_values,
     libevtx_io_handle_t *io_handle,
     const uint8_t *chunk_data,
     size_t chunk_data_size,
     libcerror_error_t **error );

int libevtx_record_values_get_event_identifier(
     libevtx_record_values_t *record_values,
     uint32_t *event_identifier,
     libcerror_error_t **error );

int libevtx_record_values_get_event_identifier_qualifiers(
     libevtx_record_values_t *record_values,
     uint32_t *event_identifier_qualifiers,
     libcerror_error_t **error );

int libevtx_record_values_get_event_level(
     libevtx_record_values_t *record_values,
     uint8_t *event_level,
     libcerror_error_t **error );

int libevtx_record_values_get_utf8_provider_identifier_size(
     libevtx_record_values_t *record_values,
     size_t *utf8_string_size,
     libcerror_error_t **error );

int libevtx_record_values_get_utf8_provider_identifier(
     libevtx_record_values_t *record_values,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

int libevtx_record_values_get_utf16_provider_identifier_size(
     libevtx_record_values_t *record_values,
     size_t *utf16_string_size,
     libcerror_error_t **error );

int libevtx_record_values_get_utf16_provider_identifier(
     libevtx_record_values_t *record_values,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

int libevtx_record_values_get_utf8_source_name_size(
     libevtx_record_values_t *record_values,
     size_t *utf8_string_size,
     libcerror_error_t **error );

int libevtx_record_values_get_utf8_source_name(
     libevtx_record_values_t *record_values,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

int libevtx_record_values_get_utf16_source_name_size(
     libevtx_record_values_t *record_values,
     size_t *utf16_string_size,
     libcerror_error_t **error );

int libevtx_record_values_get_utf16_source_name(
     libevtx_record_values_t *record_values,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

int libevtx_record_values_get_utf8_computer_name_size(
     libevtx_record_values_t *record_values,
     size_t *utf8_string_size,
     libcerror_error_t **error );

int libevtx_record_values_get_utf8_computer_name(
     libevtx_record_values_t *record_values,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

int libevtx_record_values_get_utf16_computer_name_size(
     libevtx_record_values_t *record_values,
     size_t *utf16_string_size,
     libcerror_error_t **error );

int libevtx_record_values_get_utf16_computer_name(
     libevtx_record_values_t *record_values,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

int libevtx_record_values_get_utf8_user_security_identifier_size(
     libevtx_record_values_t *record_values,
     size_t *utf8_string_size,
     libcerror_error_t **error );

int libevtx_record_values_get_utf8_user_security_identifier(
     libevtx_record_values_t *record_values,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

int libevtx_record_values_get_utf16_user_security_identifier_size(
     libevtx_record_values_t *record_values,
     size_t *utf16_string_size,
     libcerror_error_t **error );

int libevtx_record_values_get_utf16_user_security_identifier(
     libevtx_record_values_t *record_values,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

int libevtx_record_values_parse_data(
     libevtx_record_values_t *record_values,
     libevtx_io_handle_t *io_handle,
     libevtx_internal_template_definition_t *internal_template_definition,
     libcerror_error_t **error );

int libevtx_record_values_get_number_of_strings(
     libevtx_record_values_t *record_values,
     libevtx_io_handle_t *io_handle,
     int *number_of_strings,
     libcerror_error_t **error );

int libevtx_record_values_get_utf8_string_size(
     libevtx_record_values_t *record_values,
     libevtx_io_handle_t *io_handle,
     int string_index,
     size_t *utf8_string_size,
     libcerror_error_t **error );

int libevtx_record_values_get_utf8_string(
     libevtx_record_values_t *record_values,
     libevtx_io_handle_t *io_handle,
     int string_index,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

int libevtx_record_values_get_utf16_string_size(
     libevtx_record_values_t *record_values,
     libevtx_io_handle_t *io_handle,
     int string_index,
     size_t *utf16_string_size,
     libcerror_error_t **error );

int libevtx_record_values_get_utf16_string(
     libevtx_record_values_t *record_values,
     libevtx_io_handle_t *io_handle,
     int string_index,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

int libevtx_record_values_get_data_size(
     libevtx_record_values_t *record_values,
     libevtx_io_handle_t *io_handle,
     size_t *data_size,
     libcerror_error_t **error );

int libevtx_record_values_get_data(
     libevtx_record_values_t *record_values,
     libevtx_io_handle_t *io_handle,
     uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int libevtx_record_values_get_utf8_xml_string_size(
     libevtx_record_values_t *record_values,
     size_t *utf8_string_size,
     libcerror_error_t **error );

int libevtx_record_values_get_utf8_xml_string(
     libevtx_record_values_t *record_values,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

int libevtx_record_values_get_utf16_xml_string_size(
     libevtx_record_values_t *record_values,
     size_t *utf16_string_size,
     libcerror_error_t **error );

int libevtx_record_values_get_utf16_xml_string(
     libevtx_record_values_t *record_values,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBEVTX_RECORD_VALUES_H ) */

