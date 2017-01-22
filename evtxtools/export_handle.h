/*
 * Export handle
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

#if !defined( _EXPORT_HANDLE_H )
#define _EXPORT_HANDLE_H

#include <common.h>
#include <file_stream.h>
#include <types.h>

#include "evtxtools_libcerror.h"
#include "evtxtools_libevtx.h"
#include "log_handle.h"
#include "message_handle.h"
#include "message_string.h"
#include "resource_file.h"

#if defined( __cplusplus )
extern "C" {
#endif

enum EXPORT_MODES
{
	EXPORT_MODE_ALL				= (int) 'a',
	EXPORT_MODE_ITEMS			= (int) 'i',
	EXPORT_MODE_RECOVERED			= (int) 'r'
};

enum EXPORT_FORMATS
{
	EXPORT_FORMAT_TEXT			= (int) 't',
	EXPORT_FORMAT_XML			= (int) 'x'
};

typedef struct export_handle export_handle_t;

struct export_handle
{
	/* The export mode
	 */
	uint8_t export_mode;

	/* The export format
	 */
	uint8_t export_format;

	/* The libevtx input file
	 */
	libevtx_file_t *input_file;

	/* The message handle
	 */
	message_handle_t *message_handle;

	/* The event log type
	 */
	int event_log_type;

	/* Value to indicate to use a template definition to parse event data
	 */
	int use_template_definition;

	/* Value to indicate the input is open
	 */
	int input_is_open;

	/* The ascii codepage
	 */
	int ascii_codepage;

	/* The notification output stream
	 */
	FILE *notify_stream;

	/* Value to indicate if abort was signalled
	 */
	int abort;
};

const char *export_handle_get_event_log_key_name(
             int event_log_type );

const char *export_handle_get_event_level(
             uint8_t event_level );

int export_handle_initialize(
     export_handle_t **export_handle,
     libcerror_error_t **error );

int export_handle_free(
     export_handle_t **export_handle,
     libcerror_error_t **error );

int export_handle_signal_abort(
     export_handle_t *export_handle,
     libcerror_error_t **error );

int export_handle_set_export_mode(
     export_handle_t *export_handle,
     const system_character_t *string,
     libcerror_error_t **error );

int export_handle_set_export_format(
     export_handle_t *export_handle,
     const system_character_t *string,
     libcerror_error_t **error );

int export_handle_set_ascii_codepage(
     export_handle_t *export_handle,
     const system_character_t *string,
     libcerror_error_t **error );

int export_handle_set_preferred_language_identifier(
     export_handle_t *export_handle,
     uint32_t preferred_language_identifier,
     libcerror_error_t **error );

int export_handle_set_event_log_type(
     export_handle_t *export_handle,
     const system_character_t *string,
     libcerror_error_t **error );

int export_handle_set_event_log_type_from_filename(
     export_handle_t *export_handle,
     const system_character_t *filename,
     libcerror_error_t **error );

int export_handle_set_software_registry_filename(
     export_handle_t *export_handle,
     const system_character_t *filename,
     libcerror_error_t **error );

int export_handle_set_system_registry_filename(
     export_handle_t *export_handle,
     const system_character_t *filename,
     libcerror_error_t **error );

int export_handle_set_registry_directory_name(
     export_handle_t *export_handle,
     const system_character_t *name,
     libcerror_error_t **error );

int export_handle_set_resource_files_path(
     export_handle_t *export_handle,
     const system_character_t *path,
     libcerror_error_t **error );

int export_handle_open_input(
     export_handle_t *export_handle,
     const system_character_t *filename,
     libcerror_error_t **error );

int export_handle_close_input(
     export_handle_t *export_handle,
     libcerror_error_t **error );

/* Record specific export functions
 */
int export_handle_guid_string_copy_to_byte_stream(
     export_handle_t *export_handle,
     const system_character_t *string,
     size_t string_length,
     uint8_t *byte_stream,
     size_t byte_stream_size,
     libcerror_error_t **error );

int export_handle_resource_file_get_template_definition(
     export_handle_t *export_handle,
     resource_file_t *resource_file,
     const uint8_t *provider_identifier,
     size_t provider_identifier_size,
     uint32_t event_identifier,
     libevtx_template_definition_t **template_definition,
     libcerror_error_t **error );

int export_handle_message_string_fprint(
     export_handle_t *export_handle,
     message_string_t *message_string,
     libevtx_record_t *record,
     libcerror_error_t **error );

int export_handle_export_record_event_message(
     export_handle_t *export_handle,
     libevtx_record_t *record,
     const system_character_t *event_provider_identifier,
     size_t event_provider_identifier_length,
     const system_character_t *event_source,
     size_t event_source_length,
     uint32_t event_identifier,
     log_handle_t *log_handle,
     libcerror_error_t **error );

int export_handle_export_record(
     export_handle_t *export_handle,
     libevtx_record_t *record,
     log_handle_t *log_handle,
     libcerror_error_t **error );

int export_handle_export_record_text(
     export_handle_t *export_handle,
     libevtx_record_t *record,
     log_handle_t *log_handle,
     libcerror_error_t **error );

int export_handle_export_record_xml(
     export_handle_t *export_handle,
     libevtx_record_t *record,
     log_handle_t *log_handle,
     libcerror_error_t **error );

/* File export functions
 */
int export_handle_export_records(
     export_handle_t *export_handle,
     libevtx_file_t *file,
     log_handle_t *log_handle,
     libcerror_error_t **error );

int export_handle_export_recovered_records(
     export_handle_t *export_handle,
     libevtx_file_t *file,
     log_handle_t *log_handle,
     libcerror_error_t **error );

int export_handle_export_file(
     export_handle_t *export_handle,
     log_handle_t *log_handle,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _EXPORT_HANDLE_H ) */

