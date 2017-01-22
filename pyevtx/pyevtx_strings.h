/*
 * Python object definition of the sequence and iterator object of strings
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

#if !defined( _PYEVTX_STRINGS_H )
#define _PYEVTX_STRINGS_H

#include <common.h>
#include <types.h>

#include "pyevtx_libevtx.h"
#include "pyevtx_python.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct pyevtx_strings pyevtx_strings_t;

struct pyevtx_strings
{
	/* Python object initialization
	 */
	PyObject_HEAD

	/* The parent object
	 */
	PyObject *parent_object;

	/* The get item by index callback function
	 */
	PyObject* (*get_item_by_index)(
	             PyObject *parent_object,
	             int index );

	/* The current index
	 */
	int current_index;

	/* The number of items
	 */
	int number_of_items;
};

extern PyTypeObject pyevtx_strings_type_object;

PyObject *pyevtx_strings_new(
           PyObject *parent_object,
           PyObject* (*get_item_by_index)(
                        PyObject *parent_object,
                        int index ),
           int number_of_items );

int pyevtx_strings_init(
     pyevtx_strings_t *strings_object );

void pyevtx_strings_free(
      pyevtx_strings_t *strings_object );

Py_ssize_t pyevtx_strings_len(
            pyevtx_strings_t *strings_object );

PyObject *pyevtx_strings_getitem(
           pyevtx_strings_t *strings_object,
           Py_ssize_t item_index );

PyObject *pyevtx_strings_iter(
           pyevtx_strings_t *strings_object );

PyObject *pyevtx_strings_iternext(
           pyevtx_strings_t *strings_object );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _PYEVTX_STRINGS_H ) */

