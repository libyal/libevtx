/*
 * File functions
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

#if !defined( _LIBEVTX_INTERNAL_FILE_H )
#define _LIBEVTX_INTERNAL_FILE_H

#include <common.h>
#include <types.h>

#include "libevtx_extern.h"
#include "libevtx_io_handle.h"
#include "libevtx_libbfio.h"
#include "libevtx_libcerror.h"
#include "libevtx_libfcache.h"
#include "libevtx_libfdata.h"

#if defined( _MSC_VER ) || defined( __BORLANDC__ ) || defined( __MINGW32_VERSION ) || defined( __MINGW64_VERSION_MAJOR )

/* This inclusion is needed otherwise some linkers
 * mess up exporting the legacy functions
 */
#include "libevtx_legacy.h"
#endif

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

	/* Value to indicate if the file IO handle was opened inside the library
	 */
	uint8_t file_io_handle_opened_in_library;

	/* The chunks vector
	 */
	libfdata_vector_t *chunks_vector;

	/* The chunks cache
	 */
	libfcache_cache_t *chunks_cache;

	/* The records list
	 */
	libfdata_list_t *records_list;

	/* The recovered records list
	 */
	libfdata_list_t *recovered_records_list;

	/* The records cache
	 */
	libfcache_cache_t *records_cache;
};

LIBEVTX_EXTERN \
int libevtx_file_initialize(
     libevtx_file_t **file,
     libcerror_error_t **error );

LIBEVTX_EXTERN \
int libevtx_file_free(
     libevtx_file_t **file,
     libcerror_error_t **error );

LIBEVTX_EXTERN \
int libevtx_file_signal_abort(
     libevtx_file_t *file,
     libcerror_error_t **error );

LIBEVTX_EXTERN \
int libevtx_file_open(
     libevtx_file_t *file,
     const char *filename,
     int access_flags,
     libcerror_error_t **error );

#if defined( HAVE_WIDE_CHARACTER_TYPE )

LIBEVTX_EXTERN \
int libevtx_file_open_wide(
     libevtx_file_t *file,
     const wchar_t *filename,
     int access_flags,
     libcerror_error_t **error );

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

LIBEVTX_EXTERN \
int libevtx_file_open_file_io_handle(
     libevtx_file_t *file,
     libbfio_handle_t *file_io_handle,
     int access_flags,
     libcerror_error_t **error );

LIBEVTX_EXTERN \
int libevtx_file_close(
     libevtx_file_t *file,
     libcerror_error_t **error );

int libevtx_file_open_read(
     libevtx_internal_file_t *internal_file,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error );

LIBEVTX_EXTERN \
int libevtx_file_is_corrupted(
     libevtx_file_t *file,
     libcerror_error_t **error );

LIBEVTX_EXTERN \
int libevtx_file_get_ascii_codepage(
     libevtx_file_t *file,
     int *ascii_codepage,
     libcerror_error_t **error );

LIBEVTX_EXTERN \
int libevtx_file_set_ascii_codepage(
     libevtx_file_t *file,
     int ascii_codepage,
     libcerror_error_t **error );

LIBEVTX_EXTERN \
int libevtx_file_get_format_version(
     libevtx_file_t *file,
     uint16_t *major_version,
     uint16_t *minor_version,
     libcerror_error_t **error );

LIBEVTX_EXTERN \
int libevtx_file_get_version(
     libevtx_file_t *file,
     uint16_t *major_version,
     uint16_t *minor_version,
     libcerror_error_t **error );

LIBEVTX_EXTERN \
int libevtx_file_get_flags(
     libevtx_file_t *file,
     uint32_t *flags,
     libcerror_error_t **error );

LIBEVTX_EXTERN \
int libevtx_file_get_number_of_records(
     libevtx_file_t *file,
     int *number_of_records,
     libcerror_error_t **error );

LIBEVTX_EXTERN \
int libevtx_file_get_record(
     libevtx_file_t *file,
     int record_index,
     libevtx_record_t **record,
     libcerror_error_t **error );

LIBEVTX_EXTERN \
int libevtx_file_get_number_of_recovered_records(
     libevtx_file_t *file,
     int *number_of_records,
     libcerror_error_t **error );

LIBEVTX_EXTERN \
int libevtx_file_get_recovered_record(
     libevtx_file_t *file,
     int record_index,
     libevtx_record_t **record,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBEVTX_INTERNAL_FILE_H ) */

