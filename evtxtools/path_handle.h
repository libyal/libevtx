/*
 * Path handle
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

#if !defined( _PATH_HANDLE_H )
#define _PATH_HANDLE_H

#include <common.h>
#include <types.h>

#include "evtxtools_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct path_handle path_handle_t;

struct path_handle
{
	/* The %SystemRoot% path
	 */
	system_character_t *system_root_path;

	/* The %SystemRoot% path size
	 */
	size_t system_root_path_size;
};

int path_handle_initialize(
     path_handle_t **path_handle,
     libcerror_error_t **error );

int path_handle_free(
     path_handle_t **path_handle,
     libcerror_error_t **error );

int path_handle_get_directory_entry_name_by_name_no_case(
     path_handle_t *path_handle,
     const system_character_t *path,
     size_t path_length,
     system_character_t *entry_name,
     size_t entry_name_size,
     uint8_t entry_type,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _PATH_HANDLE_H ) */

