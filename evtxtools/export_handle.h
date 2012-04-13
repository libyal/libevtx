/* 
 * Export handle
 *
 * Copyright (c) 2011-2012, Joachim Metz <jbmetz@users.sourceforge.net>
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
#include "log_handle.h"

#if defined( __cplusplus )
extern "C" {
#endif

enum EXPORT_MODES
{
	EXPORT_MODE_ALL				= (int) 'a',
	EXPORT_MODE_ITEMS			= (int) 'i',
	EXPORT_MODE_RECOVERED			= (int) 'r'
};

typedef struct export_handle export_handle_t;

struct export_handle
{
	/* The export mode
	 */
	uint8_t export_mode;

	/* The libevt input file
	 */
	libevtx_file_t *input_file;

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

int export_handle_set_ascii_codepage(
     export_handle_t *export_handle,
     const libcstring_system_character_t *string,
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
int export_handle_export_record(
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

