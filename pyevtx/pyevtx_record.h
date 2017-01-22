/*
 * Python object wrapper of libevtx_record_t
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

#if !defined( _PYEVTX_RECORD_H )
#define _PYEVTX_RECORD_H

#include <common.h>
#include <types.h>

#include "pyevtx_libevtx.h"
#include "pyevtx_python.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct pyevtx_record pyevtx_record_t;

struct pyevtx_record
{
	/* Python object initialization
	 */
	PyObject_HEAD

	/* The libevtx record
	 */
	libevtx_record_t *record;

	/* The parent object
	 */
	PyObject *parent_object;
};

extern PyMethodDef pyevtx_record_object_methods[];
extern PyTypeObject pyevtx_record_type_object;

PyObject *pyevtx_record_new(
           PyTypeObject *type_object,
           libevtx_record_t *record,
           PyObject *parent_object );

int pyevtx_record_init(
     pyevtx_record_t *pyevtx_record );

void pyevtx_record_free(
      pyevtx_record_t *pyevtx_record );

PyObject *pyevtx_record_get_offset(
           pyevtx_record_t *pyevtx_record,
           PyObject *arguments );

PyObject *pyevtx_record_get_identifier(
           pyevtx_record_t *pyevtx_record,
           PyObject *arguments );

PyObject *pyevtx_record_get_written_time(
           pyevtx_record_t *pyevtx_record,
           PyObject *arguments );

PyObject *pyevtx_record_get_written_time_as_integer(
           pyevtx_record_t *pyevtx_record,
           PyObject *arguments );

PyObject *pyevtx_record_get_event_identifier(
           pyevtx_record_t *pyevtx_record,
           PyObject *arguments );

PyObject *pyevtx_record_get_event_identifier_qualifiers(
           pyevtx_record_t *pyevtx_record,
           PyObject *arguments );

PyObject *pyevtx_record_get_event_level(
           pyevtx_record_t *pyevtx_record,
           PyObject *arguments );

PyObject *pyevtx_record_get_provider_identifier(
           pyevtx_record_t *pyevtx_record,
           PyObject *arguments );

PyObject *pyevtx_record_get_source_name(
           pyevtx_record_t *pyevtx_record,
           PyObject *arguments );

PyObject *pyevtx_record_get_computer_name(
           pyevtx_record_t *pyevtx_record,
           PyObject *arguments );

PyObject *pyevtx_record_get_user_security_identifier(
           pyevtx_record_t *pyevtx_record,
           PyObject *arguments );

PyObject *pyevtx_record_get_number_of_strings(
           pyevtx_record_t *pyevtx_record,
           PyObject *arguments );

PyObject *pyevtx_record_get_string_by_index(
           PyObject *pyevtx_record,
           int string_index );

PyObject *pyevtx_record_get_string(
           pyevtx_record_t *pyevtx_record,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyevtx_record_get_strings(
           pyevtx_record_t *pyevtx_record,
           PyObject *arguments );

PyObject *pyevtx_record_get_data(
           pyevtx_record_t *pyevtx_record,
           PyObject *arguments );

PyObject *pyevtx_record_get_xml_string(
           pyevtx_record_t *pyevtx_record,
           PyObject *arguments );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _PYEVTX_RECORD_H ) */

