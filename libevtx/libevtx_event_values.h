/*
 * Event values functions
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

#if !defined( _LIBEVTX_EVENT_VALUES_H )
#define _LIBEVTX_EVENT_VALUES_H

#include <common.h>
#include <types.h>

#include "libevtx_binary_xml_document.h"
#include "libevtx_io_handle.h"
#include "libevtx_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libevtx_event_values libevtx_event_values_t;

struct libevtx_event_values
{
	/* The size
	 */
	uint32_t size;

	/* The identifier
	 */
	uint64_t identifier;

	/* The creation time
	 */
	uint64_t creation_time;

	/* The XML document
	 */
	libevtx_binary_xml_document_t *xml_document;
};

int libevtx_event_values_initialize(
     libevtx_event_values_t **event_values,
     libcerror_error_t **error );

int libevtx_event_values_free(
     libevtx_event_values_t **event_values,
     libcerror_error_t **error );

int libevtx_event_values_read(
     libevtx_event_values_t *event_values,
     libevtx_io_handle_t *io_handle,
     const uint8_t *chunk_data,
     size_t chunk_data_size,
     size_t chunk_data_offset,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

