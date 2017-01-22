/*
 * Message string
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

#if !defined( _MESSAGE_STRING_H )
#define _MESSAGE_STRING_H

#include <common.h>
#include <file_stream.h>
#include <types.h>

#include "evtxtools_libcerror.h"
#include "evtxtools_libevtx.h"
#include "evtxtools_libwrc.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct message_string message_string_t;

struct message_string
{
	/* The identifier
	 */
	uint32_t identifier;

	/* The string
	 */
	system_character_t *string;

	/* The string size
	 */
	size_t string_size;
};

int message_string_initialize(
     message_string_t **message_string,
     uint32_t message_string_identifier,
     libcerror_error_t **error );

int message_string_free(
     message_string_t **message_string,
     libcerror_error_t **error );

int message_string_get_from_message_table_resource(
     message_string_t *message_string,
     libwrc_resource_t *message_table_resource,
     uint32_t language_identifier,
     libcerror_error_t **error );

int message_string_fprint(
     message_string_t *message_string,
     libevtx_record_t *record,
     FILE *stream,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _MESSAGE_STRING_H ) */

