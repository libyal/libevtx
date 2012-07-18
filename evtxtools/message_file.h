/* 
 * Message file
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

#if !defined( _MESSAGE_FILE_H )
#define _MESSAGE_FILE_H

#include <common.h>
#include <types.h>

#include "evtxtools_libcerror.h"
#include "evtxtools_libcstring.h"
#include "evtxtools_libexe.h"
#include "evtxtools_libwrc.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct message_file message_file_t;

struct message_file
{
	/* The name
	 */
	libcstring_system_character_t *name;

	/* The name size
	 */
	size_t name_size;

	/* The libexe file
	 */
	libexe_file_t *exe_file;

	/* The libexe resource (.rsrc) section
	 */
	libexe_section_t *resource_section;

	/* The resource (.rsrc) section file IO handle
	 */
	libbfio_handle_t *resource_section_file_io_handle;

	/* The libwrc resource stream
	 */
	libwrc_stream_t *resource_stream;

	/* The libwrc message table resource
	 */
	libwrc_resource_t *message_table_resource;

	/* Value to indicate if the message file is open
	 */
	int is_open;
};

int message_file_initialize(
     message_file_t **message_file,
     libcerror_error_t **error );

int message_file_free(
     message_file_t **message_file,
     libcerror_error_t **error );

int message_file_open(
     message_file_t *message_file,
     const libcstring_system_character_t *filename,
     libcerror_error_t **error );

int message_file_close(
     message_file_t *message_file,
     libcerror_error_t **error );

int message_file_get_string(
     message_file_t *message_file,
     uint32_t preferred_language_identifier,
     uint32_t message_identifier,
     libcstring_system_character_t **message_string,
     size_t *message_string_size,
     libcerror_error_t **error );

int message_file_set_name(
     message_file_t *message_file,
     const libcstring_system_character_t *name,
     size_t name_length,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

