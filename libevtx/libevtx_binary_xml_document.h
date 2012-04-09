/*
 * Binary XML document functions
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

#if !defined( _LIBEVTX_BINARY_XML_DOCUMENT_H )
#define _LIBEVTX_BINARY_XML_DOCUMENT_H

#include <common.h>
#include <types.h>

#include "libevtx_array_type.h"
#include "libevtx_binary_xml_token.h"
#include "libevtx_io_handle.h"
#include "libevtx_libcerror.h"
#include "libevtx_xml_tag.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libevtx_binary_xml_document libevtx_binary_xml_document_t;

struct libevtx_binary_xml_document
{
	/* The root XML tag
	 */
	libevtx_xml_tag_t *root_xml_tag;

	/* The size
	 */
	size_t size;
};

int libevtx_binary_xml_document_initialize(
     libevtx_binary_xml_document_t **binary_xml_document,
     libcerror_error_t **error );

int libevtx_binary_xml_document_free(
     libevtx_binary_xml_document_t **binary_xml_document,
     libcerror_error_t **error );

int libevtx_binary_xml_document_read(
     libevtx_binary_xml_document_t *binary_xml_document,
     libevtx_io_handle_t *io_handle,
     const uint8_t *chunk_data,
     size_t chunk_data_size,
     size_t chunk_data_offset,
     libcerror_error_t **error );

int libevtx_binary_xml_document_read_attribute(
     libevtx_binary_xml_document_t *binary_xml_document,
     libevtx_binary_xml_token_t *binary_xml_token,
     libevtx_io_handle_t *io_handle,
     const uint8_t *chunk_data,
     size_t chunk_data_size,
     size_t chunk_data_offset,
     libevtx_array_t *template_values_array,
     libevtx_xml_tag_t *xml_tag,
     libcerror_error_t **error );

int libevtx_binary_xml_document_read_element(
     libevtx_binary_xml_document_t *binary_xml_document,
     libevtx_binary_xml_token_t *binary_xml_token,
     libevtx_io_handle_t *io_handle,
     const uint8_t *chunk_data,
     size_t chunk_data_size,
     size_t chunk_data_offset,
     libevtx_array_t *template_values_array,
     libevtx_xml_tag_t *xml_tag,
     libcerror_error_t **error );

int libevtx_binary_xml_document_read_fragment(
     libevtx_binary_xml_document_t *binary_xml_document,
     libevtx_binary_xml_token_t *binary_xml_token,
     libevtx_io_handle_t *io_handle,
     const uint8_t *chunk_data,
     size_t chunk_data_size,
     size_t chunk_data_offset,
     libevtx_xml_tag_t *xml_tag,
     libcerror_error_t **error );

int libevtx_binary_xml_document_read_fragment_header(
     libevtx_binary_xml_document_t *binary_xml_document,
     libevtx_binary_xml_token_t *binary_xml_token,
     const uint8_t *chunk_data,
     size_t chunk_data_size,
     size_t chunk_data_offset,
     libcerror_error_t **error );

int libevtx_binary_xml_document_read_normal_substitution(
     libevtx_binary_xml_document_t *binary_xml_document,
     libevtx_binary_xml_token_t *binary_xml_token,
     libevtx_io_handle_t *io_handle,
     const uint8_t *chunk_data,
     size_t chunk_data_size,
     size_t chunk_data_offset,
     libevtx_array_t *template_values_array,
     libevtx_xml_tag_t *xml_tag,
     libcerror_error_t **error );

int libevtx_binary_xml_document_read_optional_substitution(
     libevtx_binary_xml_document_t *binary_xml_document,
     libevtx_binary_xml_token_t *binary_xml_token,
     libevtx_io_handle_t *io_handle,
     const uint8_t *chunk_data,
     size_t chunk_data_size,
     size_t chunk_data_offset,
     libevtx_array_t *template_values_array,
     libevtx_xml_tag_t *xml_tag,
     libcerror_error_t **error );

int libevtx_binary_xml_document_read_template_instance(
     libevtx_binary_xml_document_t *binary_xml_document,
     libevtx_binary_xml_token_t *binary_xml_token,
     libevtx_io_handle_t *io_handle,
     const uint8_t *chunk_data,
     size_t chunk_data_size,
     size_t chunk_data_offset,
     libevtx_xml_tag_t *xml_tag,
     libcerror_error_t **error );

int libevtx_binary_xml_document_read_template_instance_values(
     libevtx_binary_xml_document_t *binary_xml_document,
     libevtx_binary_xml_token_t *binary_xml_token,
     const uint8_t *chunk_data,
     size_t chunk_data_size,
     size_t chunk_data_offset,
     libevtx_array_t **template_values_array,
     libcerror_error_t **error );

int libevtx_binary_xml_document_read_value(
     libevtx_binary_xml_document_t *binary_xml_document,
     libevtx_binary_xml_token_t *binary_xml_token,
     const uint8_t *chunk_data,
     size_t chunk_data_size,
     size_t chunk_data_offset,
     libevtx_xml_tag_t *xml_tag,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

