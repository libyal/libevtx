/*
 * File functions
 *
 * Copyright (c) 2011, Joachim Metz <jbmetz@users.sourceforge.net>
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

#if !defined( _LIBEVTX_INTERNAL_FILE_H )
#define _LIBEVTX_INTERNAL_FILE_H

#include <common.h>
#include <types.h>

#include <liberror.h>

#include "libevtx_extern.h"
#include "libevtx_io_handle.h"
#include "libevtx_libbfio.h"
#include "libevtx_libfdata.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libevtx_internal_file libevtx_internal_file_t;

struct libevtx_internal_file
{
	/* The IO handle
	 */
	libevtx_io_handle_t *io_handle;

	/* The file IO handle
	 */
	libbfio_handle_t *file_io_handle;

	/* Value to indicate if the file IO handle was created inside the library
	 */
	uint8_t file_io_handle_created_in_library;

	/* The chunks vector
	 */
	libfdata_vector_t *chunks_vector;

	/* The chunks cache
	 */
	libfdata_cache_t *chunks_cache;

	/* Value to indicate if abort was signalled
	 */
	int abort;
};

LIBEVTX_EXTERN \
int libevtx_file_initialize(
     libevtx_file_t **file,
     liberror_error_t **error );

LIBEVTX_EXTERN \
int libevtx_file_free(
     libevtx_file_t **file,
     liberror_error_t **error );

LIBEVTX_EXTERN \
int libevtx_file_signal_abort(
     libevtx_file_t *file,
     liberror_error_t **error );

LIBEVTX_EXTERN \
int libevtx_file_open(
     libevtx_file_t *file,
     const char *filename,
     int access_flags,
     liberror_error_t **error );

#if defined( HAVE_WIDE_CHARACTER_TYPE )
LIBEVTX_EXTERN \
int libevtx_file_open_wide(
     libevtx_file_t *file,
     const wchar_t *filename,
     int access_flags,
     liberror_error_t **error );
#endif

LIBEVTX_EXTERN \
int libevtx_file_open_file_io_handle(
     libevtx_file_t *file,
     libbfio_handle_t *file_io_handle,
     int access_flags,
     liberror_error_t **error );

LIBEVTX_EXTERN \
int libevtx_file_close(
     libevtx_file_t *file,
     liberror_error_t **error );

int libevtx_file_open_read(
     libevtx_internal_file_t *internal_file,
     liberror_error_t **error );

LIBEVTX_EXTERN \
int libevtx_file_get_format_version(
     libevtx_file_t *file,
     uint16_t *major_version,
     uint16_t *minor_version,
     liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

