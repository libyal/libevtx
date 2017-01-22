/*
 * Registry file
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

#if !defined( _REGISTRY_FILE_H )
#define _REGISTRY_FILE_H

#include <common.h>
#include <types.h>

#include "evtxtools_libcerror.h"
#include "evtxtools_libregf.h"

#if defined( __cplusplus )
extern "C" {
#endif

/* TODO add more types */
enum REGISTRY_FILE_TYPES
{
	REGISTRY_FILE_TYPE_NTUSER_DAT,
	REGISTRY_FILE_TYPE_SAM,
	REGISTRY_FILE_TYPE_SECURITY,
	REGISTRY_FILE_TYPE_SOFTWARE,
	REGISTRY_FILE_TYPE_SYSTEM
};

typedef struct registry_file registry_file_t;

struct registry_file
{
	/* The libregf file
	 */
	libregf_file_t *regf_file;

	/* The registry file type
	 */
	uint8_t type;

	/* The libregf root key
	 */
	libregf_key_t *root_key;

	/* The libregf base key
	 */
	libregf_key_t *base_key;

	/* The current control set
	 */
	uint32_t current_control_set;

	/* The current control set key
	 */
	libregf_key_t *current_control_set_key;

	/* The control set 1 key
	 */
	libregf_key_t *control_set1_key;

	/* The control set 2 key
	 */
	libregf_key_t *control_set2_key;

	/* Value to indicate if the message file is open
	 */
	int is_open;
};

int registry_file_initialize(
     registry_file_t **registry_file,
     libcerror_error_t **error );

int registry_file_free(
     registry_file_t **registry_file,
     libcerror_error_t **error );

int registry_file_signal_abort(
     registry_file_t *registry_file,
     libcerror_error_t **error );

int registry_file_set_ascii_codepage(
     registry_file_t *registry_file,
     int ascii_codepage,
     libcerror_error_t **error );

int registry_file_open(
     registry_file_t *registry_file,
     const system_character_t *filename,
     libcerror_error_t **error );

int registry_file_close(
     registry_file_t *registry_file,
     libcerror_error_t **error );

int registry_file_get_key_by_path(
     registry_file_t *registry_file,
     const system_character_t *key_path,
     size_t key_path_length,
     libregf_key_t **key,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _REGISTRY_FILE_H ) */

