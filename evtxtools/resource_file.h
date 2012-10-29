/*
 * Resource file
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

#if !defined( _RESOURCE_FILE_H )
#define _RESOURCE_FILE_H

#include <common.h>
#include <types.h>

#include "evtxtools_libcerror.h"
#include "evtxtools_libcstring.h"
#include "evtxtools_libexe.h"
#include "evtxtools_libwrc.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct resource_file resource_file_t;

struct resource_file
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

	/* Value to indicate if the resource file is open
	 */
	int is_open;
};

int resource_file_initialize(
     resource_file_t **resource_file,
     libcerror_error_t **error );

int resource_file_free(
     resource_file_t **resource_file,
     libcerror_error_t **error );

int resource_file_open(
     resource_file_t *resource_file,
     const libcstring_system_character_t *filename,
     libcerror_error_t **error );

int resource_file_close(
     resource_file_t *resource_file,
     libcerror_error_t **error );

int resource_file_set_name(
     resource_file_t *resource_file,
     const libcstring_system_character_t *name,
     size_t name_length,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

