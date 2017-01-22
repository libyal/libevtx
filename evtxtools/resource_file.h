/*
 * Resource file
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

#if !defined( _RESOURCE_FILE_H )
#define _RESOURCE_FILE_H

#include <common.h>
#include <types.h>

#include "evtxtools_libbfio.h"
#include "evtxtools_libcerror.h"
#include "evtxtools_libexe.h"
#include "evtxtools_libfcache.h"
#include "evtxtools_libwrc.h"
#include "message_string.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct resource_file resource_file_t;

struct resource_file
{
	/* The name
	 */
	system_character_t *name;

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

	/* The preferred language identifier
	 */
	uint32_t preferred_language_identifier;

	/* The libwrc message table resource
	 */
	libwrc_resource_t *message_table_resource;

	/* The libwrc MUI resource
	 */
	libwrc_resource_t *mui_resource;

	/* The libwrc WEVT_TEMPLATE resource
	 */
	libwrc_resource_t *wevt_template_resource;

	/* The message string cache
	 */
	libfcache_cache_t *message_string_cache;

	/* The next available message string cache index
	 */
	int next_message_string_cache_index;

	/* Value to indicate if the message file is open
	 */
	int is_open;
};

int resource_file_initialize(
     resource_file_t **resource_file,
     uint32_t preferred_language_identifier,
     libcerror_error_t **error );

int resource_file_free(
     resource_file_t **resource_file,
     libcerror_error_t **error );

int resource_file_open(
     resource_file_t *resource_file,
     const system_character_t *filename,
     libcerror_error_t **error );

int resource_file_close(
     resource_file_t *resource_file,
     libcerror_error_t **error );

int resource_file_get_resource_available_languague_identifier(
     resource_file_t *resource_file,
     libwrc_resource_t *resource,
     uint32_t *language_identifier,
     libcerror_error_t **error );

int resource_file_set_name(
     resource_file_t *resource_file,
     const system_character_t *name,
     size_t name_length,
     libcerror_error_t **error );

int resource_file_get_message_string_from_cache(
     resource_file_t *resource_file,
     uint32_t message_string_identifier,
     message_string_t **message_string,
     libcerror_error_t **error );

int resource_file_get_message_string(
     resource_file_t *resource_file,
     uint32_t message_string_identifier,
     message_string_t **message_string,
     libcerror_error_t **error );

int resource_file_get_mui_file_type(
     resource_file_t *resource_file,
     uint32_t *file_type,
     libcerror_error_t **error );

int resource_file_get_provider(
     resource_file_t *resource_file,
     const uint8_t *provider_identifier,
     size_t provider_identifier_size,
     libwrc_wevt_provider_t **provider,
     libcerror_error_t **error );

int resource_file_get_event(
     resource_file_t *resource_file,
     const uint8_t *provider_identifier,
     size_t provider_identifier_size,
     uint32_t event_identifier,
     libwrc_wevt_provider_t **provider,
     libwrc_wevt_event_t **event,
     libcerror_error_t **error );

int resource_file_get_template_definition(
     resource_file_t *resource_file,
     const uint8_t *provider_identifier,
     size_t provider_identifier_size,
     uint32_t event_identifier,
     libwrc_wevt_provider_t **provider,
     libwrc_wevt_event_t **event,
     libwrc_wevt_template_definition_t **template_definition,
     libcerror_error_t **error );

int resource_file_get_event_message_identifier(
     resource_file_t *resource_file,
     const uint8_t *provider_identifier,
     size_t provider_identifier_size,
     uint32_t event_identifier,
     uint32_t *message_identifier,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _RESOURCE_FILE_H ) */

