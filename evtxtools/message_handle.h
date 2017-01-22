/*
 * Message handle
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

#if !defined( _MESSAGE_HANDLE_H )
#define _MESSAGE_HANDLE_H

#include <common.h>
#include <types.h>

#include "evtxtools_libcerror.h"
#include "evtxtools_libfcache.h"
#include "evtxtools_libregf.h"
#include "message_string.h"
#include "path_handle.h"
#include "registry_file.h"
#include "resource_file.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct message_handle message_handle_t;

struct message_handle
{
	/* The SOFTWARE registry filename
	 */
	system_character_t *software_registry_filename;

	/* The SOFTWARE registry filename size
	 */
	size_t software_registry_filename_size;

	/* The SYSTEM registry filename
	 */
	system_character_t *system_registry_filename;

	/* The SYSTEM registry filename size
	 */
	size_t system_registry_filename_size;

	/* The name of the directory containing the SOFTWARE and SYSTEM registry filename
	 */
	system_character_t *registry_directory_name;

	/* The size of the name of the directory containing the SOFTWARE and SYSTEM registry filename
	 */
	size_t registry_directory_name_size;

	/* The SOFTWARE registry file
	 */
	registry_file_t *software_registry_file;

	/* The %SystemRoot% path
	 */
	system_character_t *system_root_path;

	/* The %SystemRoot% path size
	 */
	size_t system_root_path_size;

	/* The %WinDir% path
	 */
	system_character_t *windows_directory_path;

	/* The %WinDir% path size
	 */
	size_t windows_directory_path_size;

	/* The SYSTEM registry file
	 */
	registry_file_t *system_registry_file;

	/* The path handle
	 */
	path_handle_t *path_handle;

	/* The current control set
	 */
	uint32_t current_control_set;

	/* The SOFTWARE WinEvt publishers key
	 */
	libregf_key_t *winevt_publishers_key;

	/* The SYSTEM (control set 1) eventlog services key
	 */
	libregf_key_t *control_set_1_eventlog_services_key;

	/* The SYSTEM (control set 2) eventlog services key
	 */
	libregf_key_t *control_set_2_eventlog_services_key;

	/* The resource files path
	 */
	const system_character_t *resource_files_path;

	/* The resource file cache
	 */
	libfcache_cache_t *resource_file_cache;

	/* The next available resource file cache index
	 */
	int next_resource_file_cache_index;

	/* The MUI resource file cache
	 */
	libfcache_cache_t *mui_resource_file_cache;

	/* The next available MUI resource file cache index
	 */
	int next_mui_resource_file_cache_index;

	/* The ascii codepage
	 */
	int ascii_codepage;

	/* The preferred language identifier
	 */
	uint32_t preferred_language_identifier;
};

int message_handle_initialize(
     message_handle_t **message_handle,
     libcerror_error_t **error );

int message_handle_free(
     message_handle_t **message_handle,
     libcerror_error_t **error );

int message_handle_signal_abort(
     message_handle_t *message_handle,
     libcerror_error_t **error );

int message_handle_set_ascii_codepage(
     message_handle_t *message_handle,
     int ascii_codepage,
     libcerror_error_t **error );

int message_handle_set_preferred_language_identifier(
     message_handle_t *message_handle,
     uint32_t preferred_language_identifier,
     libcerror_error_t **error );

int message_handle_set_event_log_type_from_filename(
     message_handle_t *message_handle,
     const system_character_t *filename,
     libcerror_error_t **error );

int message_handle_set_software_registry_filename(
     message_handle_t *message_handle,
     const system_character_t *filename,
     libcerror_error_t **error );

int message_handle_set_system_registry_filename(
     message_handle_t *message_handle,
     const system_character_t *filename,
     libcerror_error_t **error );

int message_handle_set_registry_directory_name(
     message_handle_t *message_handle,
     const system_character_t *name,
     libcerror_error_t **error );

int message_handle_set_resource_files_path(
     message_handle_t *message_handle,
     const system_character_t *path,
     libcerror_error_t **error );

int message_handle_open_software_registry_file(
     message_handle_t *message_handle,
     libcerror_error_t **error );

int message_handle_open_system_registry_file(
     message_handle_t *message_handle,
     const char *eventlog_key_name,
     libcerror_error_t **error );

int message_handle_open_input(
     message_handle_t *message_handle,
     const char *eventlog_key_name,
     libcerror_error_t **error );

int message_handle_close_input(
     message_handle_t *message_handle,
     libcerror_error_t **error );

int message_handle_get_value_by_event_source(
     message_handle_t *message_handle,
     const system_character_t *event_source,
     size_t event_source_length,
     const system_character_t *value_name,
     size_t value_name_length,
     system_character_t **value_string,
     size_t *value_string_size,
     libcerror_error_t **error );

int message_handle_get_value_by_provider_identifier(
     message_handle_t *message_handle,
     const system_character_t *provider_identifier,
     size_t provider_identifier_length,
     const system_character_t *value_name,
     size_t value_name_length,
     system_character_t **value_string,
     size_t *value_string_size,
     libcerror_error_t **error );

int message_handle_get_resource_file_path(
     message_handle_t *message_handle,
     const system_character_t *resource_filename,
     size_t resource_filename_length,
     const system_character_t *language_string,
     size_t language_string_length,
     system_character_t **resource_file_path,
     size_t *resource_file_path_size,
     libcerror_error_t **error );

int message_handle_get_resource_file(
     message_handle_t *message_handle,
     const system_character_t *resource_filename,
     size_t resource_filename_length,
     const system_character_t *resource_file_path,
     resource_file_t **resource_file,
     libcerror_error_t **error );

int message_handle_get_resource_file_from_cache(
     message_handle_t *message_handle,
     const system_character_t *resource_filename,
     size_t resource_filename_length,
     resource_file_t **resource_file,
     libcerror_error_t **error );

int message_handle_get_mui_resource_file_path(
     message_handle_t *message_handle,
     const system_character_t *resource_filename,
     size_t resource_filename_length,
     system_character_t **resource_file_path,
     size_t *resource_file_path_size,
     libcerror_error_t **error );

int message_handle_get_mui_resource_file(
     message_handle_t *message_handle,
     const system_character_t *resource_filename,
     size_t resource_filename_length,
     const system_character_t *resource_file_path,
     resource_file_t **resource_file,
     libcerror_error_t **error );

int message_handle_get_mui_resource_file_from_cache(
     message_handle_t *message_handle,
     const system_character_t *resource_filename,
     size_t resource_filename_length,
     resource_file_t **resource_file,
     libcerror_error_t **error );

int message_handle_get_message_string_from_resource_file(
     message_handle_t *message_handle,
     const system_character_t *resource_filename,
     size_t resource_filename_length,
     uint32_t message_identifier,
     message_string_t **message_string,
     libcerror_error_t **error );

int message_handle_get_message_string(
     message_handle_t *message_handle,
     const system_character_t *resource_filename,
     size_t resource_filename_length,
     uint32_t message_identifier,
     message_string_t **message_string,
     libcerror_error_t **error );

int message_handle_get_resource_file_by_provider_identifier(
     message_handle_t *message_handle,
     const system_character_t *resource_filename,
     size_t resource_filename_length,
     const uint8_t *provider_identifier,
     size_t provider_identifier_size,
     resource_file_t **resource_file,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _MESSAGE_HANDLE_H ) */

