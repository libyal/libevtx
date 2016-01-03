/*
 * Python object definition of the strings sequence and iterator
 *
 * Copyright (C) 2011-2016, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _PYEVTX_STRINGS_H )
#define _PYEVTX_STRINGS_H

#include <common.h>
#include <types.h>

#include "pyevtx_libevtx.h"
#include "pyevtx_python.h"
#include "pyevtx_record.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct pyevtx_strings pyevtx_strings_t;

struct pyevtx_strings
{
	/* Python object initialization
	 */
	PyObject_HEAD

	/* The record object
	 */
	pyevtx_record_t *record_object;

	/* The get string by index callback function
	 */
	PyObject* (*get_string_by_index)(
	             pyevtx_record_t *record_object,
	             int string_index );

	/* The (current) string index
	 */
	int string_index;

	/* The number of strings
	 */
	int number_of_strings;
};

extern PyTypeObject pyevtx_strings_type_object;

PyObject *pyevtx_strings_new(
           pyevtx_record_t *record_object,
           PyObject* (*get_string_by_index)(
                        pyevtx_record_t *record_object,
                        int string_index ),
           int number_of_strings );

int pyevtx_strings_init(
     pyevtx_strings_t *pyevtx_strings );

void pyevtx_strings_free(
      pyevtx_strings_t *pyevtx_strings );

Py_ssize_t pyevtx_strings_len(
            pyevtx_strings_t *pyevtx_strings );

PyObject *pyevtx_strings_getitem(
           pyevtx_strings_t *pyevtx_strings,
           Py_ssize_t item_index );

PyObject *pyevtx_strings_iter(
           pyevtx_strings_t *pyevtx_strings );

PyObject *pyevtx_strings_iternext(
           pyevtx_strings_t *pyevtx_strings );

#if defined( __cplusplus )
}
#endif

#endif

