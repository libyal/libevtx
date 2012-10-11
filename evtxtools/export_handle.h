/* 
 * Export handle
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

#if !defined( _EXPORT_HANDLE_H )
#define _EXPORT_HANDLE_H

#include <common.h>
#include <file_stream.h>
#include <types.h>

#include "evtxtools_libcerror.h"
#include "evtxtools_libcstring.h"
#include "evtxtools_libevtx.h"
#include "evtxtools_libfcache.h"
#include "evtxtools_libregf.h"
#include "log_handle.h"
#include "path_handle.h"
#include "registry_file.h"

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

	/* The SOFTWARE registry file
	 */
	registry_file_t *software_registry_file;

	/* The %SystemRoot% path
	 */
	libcstring_system_character_t *system_root_path;

	/* The %SystemRoot% path size
	 */
	size_t system_root_path_size;

	/* The path handle
	 */
	path_handle_t *path_handle;

	/* The SYSTEM registry file
	 */
	registry_file_t *system_registry_file;

	/* The current control set
	 */
	uint32_t current_control_set;

	/* The control set 1 key
	 */
	libregf_key_t *control_set1_key;

	/* The control set 2 key
	 */
	libregf_key_t *control_set2_key;

	/* The preferred language identifier
	 */
	uint32_t preferred_language_identifier;

	/* The message file cache
	 */
	libfcache_cache_t *message_file_cache;

	/* The next available message file cache index
	 */
	int next_message_file_cache_index;

	/* The event log type
	 */
	int event_log_type;

	/* The messages files path
	 */
	const libcstring_system_character_t *message_files_path;

	/* The SOFTWARE registry filename
	 */
	libcstring_system_character_t *software_registry_filename;

	/* The SOFTWARE registry filename size
	 */
	size_t software_registry_filename_size;

	/* The SYSTEM registry filename
	 */
	libcstring_system_character_t *system_registry_filename;

	/* The SYSTEM registry filename size
	 */
	size_t system_registry_filename_size;

	/* The name of the directory containing the SOFTWARE and SYSTEM registry filename
	 */
	libcstring_system_character_t *registry_directory_name;

	/* The size of the name of the directory containing the SOFTWARE and SYSTEM registry filename
	 */
	size_t registry_directory_name_size;

	/* Value to indicate the input is open
	 */
	int input_is_open;

	/* The ascii codepage
	 */
	int ascii_codepage;

	/* The nofication output stream
	 */
	FILE *notify_stream;

	/* Value to indicate if abort was signalled
	 */
	int abort;
};

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
     const libcstring_system_character_t *string,
     libcerror_error_t **error );

int export_handle_set_export_format(
     export_handle_t *export_handle,
     const libcstring_system_character_t *string,
     libcerror_error_t **error );

int export_handle_set_ascii_codepage(
     export_handle_t *export_handle,
     const libcstring_system_character_t *string,
     libcerror_error_t **error );

int export_handle_set_event_log_type(
     export_handle_t *export_handle,
     const libcstring_system_character_t *string,
     libcerror_error_t **error );

int export_handle_set_event_log_type_from_filename(
     export_handle_t *export_handle,
     const libcstring_system_character_t *filename,
     libcerror_error_t **error );

int export_handle_set_software_registry_filename(
     export_handle_t *export_handle,
     const libcstring_system_character_t *filename,
     libcerror_error_t **error );

int export_handle_set_system_registry_filename(
     export_handle_t *export_handle,
     const libcstring_system_character_t *filename,
     libcerror_error_t **error );

int export_handle_set_registry_directory_name(
     export_handle_t *export_handle,
     const libcstring_system_character_t *filename,
     libcerror_error_t **error );

int export_handle_open_software_registry_file(
     export_handle_t *export_handle,
     libcerror_error_t **error );

int export_handle_open_system_registry_file(
     export_handle_t *export_handle,
     libcerror_error_t **error );

int export_handle_open_input(
     export_handle_t *export_handle,
     const libcstring_system_character_t *filename,
     libcerror_error_t **error );

int export_handle_close_input(
     export_handle_t *export_handle,
     libcerror_error_t **error );

/* Record specific export functions
 */
int export_handle_get_message_filename(
     export_handle_t *export_handle,
     const libcstring_system_character_t *event_source,
     size_t event_source_length,
     libcstring_system_character_t **message_filename,
     size_t *message_filename_size,
     libcerror_error_t **error );

int export_handle_get_message_file_path(
     export_handle_t *export_handle,
     const libcstring_system_character_t *message_filename,
     size_t message_filename_length,
     libcstring_system_character_t **message_file_path,
     size_t *message_file_path_size,
     libcerror_error_t **error );

int export_handle_get_message_string(
     export_handle_t *export_handle,
     const libcstring_system_character_t *message_filename,
     size_t message_filename_length,
     uint32_t message_identifier,
     libcstring_system_character_t **message_string,
     size_t *message_string_size,
     libcerror_error_t **error );

int export_handle_message_string_fprint(
     export_handle_t *export_handle,
     const libcstring_system_character_t *message_string,
     size_t message_string_length,
     libevtx_record_t *record,
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

#endif

