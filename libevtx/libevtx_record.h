/*
 * Record functions
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

#if !defined( _LIBEVTX_INTERNAL_RECORD_H )
#define _LIBEVTX_INTERNAL_RECORD_H

#include <common.h>
#include <types.h>

#include "libevtx_extern.h"
#include "libevtx_io_handle.h"
#include "libevtx_libbfio.h"
#include "libevtx_libcerror.h"
#include "libevtx_record_values.h"
#include "libevtx_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libevtx_internal_record libevtx_internal_record_t;

struct libevtx_internal_record
{
	/* The IO handle
	 */
	libevtx_io_handle_t *io_handle;

	/* The file IO handle
	 */
	libbfio_handle_t *file_io_handle;

	/* The (event) record values
	 */
	libevtx_record_values_t *record_values;

	/* The flags
	 */
	uint8_t flags;
};

int libevtx_record_initialize(
     libevtx_record_t **record,
     libevtx_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libevtx_record_values_t *record_values,
     uint8_t flags,
     libcerror_error_t **error );

LIBEVTX_EXTERN \
int libevtx_record_free(
     libevtx_record_t **record,
     libcerror_error_t **error );

LIBEVTX_EXTERN \
int libevtx_record_get_identifier(
     libevtx_record_t *record,
     uint64_t *identifier,
     libcerror_error_t **error );

LIBEVTX_EXTERN \
int libevtx_record_get_written_time(
     libevtx_record_t *record,
     uint64_t *written_time,
     libcerror_error_t **error );

LIBEVTX_EXTERN \
int libevtx_record_get_event_identifier(
     libevtx_record_t *record,
     uint32_t *event_identifier,
     libcerror_error_t **error );

LIBEVTX_EXTERN \
int libevtx_record_get_event_level(
     libevtx_record_t *record,
     uint8_t *event_level,
     libcerror_error_t **error );

LIBEVTX_EXTERN \
int libevtx_record_get_utf8_source_name_size(
     libevtx_record_t *record,
     size_t *utf8_string_size,
     libcerror_error_t **error );

LIBEVTX_EXTERN \
int libevtx_record_get_utf8_source_name(
     libevtx_record_t *record,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

LIBEVTX_EXTERN \
int libevtx_record_get_utf16_source_name_size(
     libevtx_record_t *record,
     size_t *utf16_string_size,
     libcerror_error_t **error );

LIBEVTX_EXTERN \
int libevtx_record_get_utf16_source_name(
     libevtx_record_t *record,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

LIBEVTX_EXTERN \
int libevtx_record_get_utf8_computer_name_size(
     libevtx_record_t *record,
     size_t *utf8_string_size,
     libcerror_error_t **error );

LIBEVTX_EXTERN \
int libevtx_record_get_utf8_computer_name(
     libevtx_record_t *record,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

LIBEVTX_EXTERN \
int libevtx_record_get_utf16_computer_name_size(
     libevtx_record_t *record,
     size_t *utf16_string_size,
     libcerror_error_t **error );

LIBEVTX_EXTERN \
int libevtx_record_get_utf16_computer_name(
     libevtx_record_t *record,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

LIBEVTX_EXTERN \
int libevtx_record_get_utf8_xml_string_size(
     libevtx_record_t *record,
     size_t *utf8_string_size,
     libcerror_error_t **error );

LIBEVTX_EXTERN \
int libevtx_record_get_utf8_xml_string(
     libevtx_record_t *record,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

LIBEVTX_EXTERN \
int libevtx_record_get_utf16_xml_string_size(
     libevtx_record_t *record,
     size_t *utf16_string_size,
     libcerror_error_t **error );

LIBEVTX_EXTERN \
int libevtx_record_get_utf16_xml_string(
     libevtx_record_t *record,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

