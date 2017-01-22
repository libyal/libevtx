/*
 * Python object wrapper of libevtx_file_t
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

#if !defined( _PYEVTX_FILE_H )
#define _PYEVTX_FILE_H

#include <common.h>
#include <types.h>

#include "pyevtx_libbfio.h"
#include "pyevtx_libevtx.h"
#include "pyevtx_python.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct pyevtx_file pyevtx_file_t;

struct pyevtx_file
{
	/* Python object initialization
	 */
	PyObject_HEAD

	/* The libevtx file
	 */
	libevtx_file_t *file;

	/* The libbfio file IO handle
	 */
	libbfio_handle_t *file_io_handle;
};

extern PyMethodDef pyevtx_file_object_methods[];
extern PyTypeObject pyevtx_file_type_object;

PyObject *pyevtx_file_new(
           void );

PyObject *pyevtx_file_new_open(
           PyObject *self,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyevtx_file_new_open_file_object(
           PyObject *self,
           PyObject *arguments,
           PyObject *keywords );

int pyevtx_file_init(
     pyevtx_file_t *pyevtx_file );

void pyevtx_file_free(
      pyevtx_file_t *pyevtx_file );

PyObject *pyevtx_file_signal_abort(
           pyevtx_file_t *pyevtx_file,
           PyObject *arguments );

PyObject *pyevtx_file_open(
           pyevtx_file_t *pyevtx_file,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyevtx_file_open_file_object(
           pyevtx_file_t *pyevtx_file,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyevtx_file_close(
           pyevtx_file_t *pyevtx_file,
           PyObject *arguments );

PyObject *pyevtx_file_is_corrupted(
           pyevtx_file_t *pyevtx_file,
           PyObject *arguments );

PyObject *pyevtx_file_get_ascii_codepage(
           pyevtx_file_t *pyevtx_file,
           PyObject *arguments );

int pyevtx_file_set_ascii_codepage_from_string(
     pyevtx_file_t *pyevtx_file,
     const char *codepage_string );

PyObject *pyevtx_file_set_ascii_codepage(
           pyevtx_file_t *pyevtx_file,
           PyObject *arguments,
           PyObject *keywords );

int pyevtx_file_set_ascii_codepage_setter(
     pyevtx_file_t *pyevtx_file,
     PyObject *string_object,
     void *closure );

PyObject *pyevtx_file_get_format_version(
           pyevtx_file_t *pyevtx_file,
           PyObject *arguments );

PyObject *pyevtx_file_get_number_of_records(
           pyevtx_file_t *pyevtx_file,
           PyObject *arguments );

PyObject *pyevtx_file_get_record_by_index(
           PyObject *pyevtx_file,
           int record_index );

PyObject *pyevtx_file_get_record(
           pyevtx_file_t *pyevtx_file,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyevtx_file_get_records(
           pyevtx_file_t *pyevtx_file,
           PyObject *arguments );

PyObject *pyevtx_file_get_number_of_recovered_records(
           pyevtx_file_t *pyevtx_file,
           PyObject *arguments );

PyObject *pyevtx_file_get_recovered_record_by_index(
           PyObject *pyevtx_file,
           int record_index );

PyObject *pyevtx_file_get_recovered_record(
           pyevtx_file_t *pyevtx_file,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyevtx_file_get_recovered_records(
           pyevtx_file_t *pyevtx_file,
           PyObject *arguments );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _PYEVTX_FILE_H ) */

