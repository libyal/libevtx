/*
 * Python object definition of the sequence and iterator object of records
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

#if !defined( _PYEVTX_RECORDS_H )
#define _PYEVTX_RECORDS_H

#include <common.h>
#include <types.h>

#include "pyevtx_libevtx.h"
#include "pyevtx_python.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct pyevtx_records pyevtx_records_t;

struct pyevtx_records
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

extern PyTypeObject pyevtx_records_type_object;

PyObject *pyevtx_records_new(
           PyObject *parent_object,
           PyObject* (*get_item_by_index)(
                        PyObject *parent_object,
                        int index ),
           int number_of_items );

int pyevtx_records_init(
     pyevtx_records_t *records_object );

void pyevtx_records_free(
      pyevtx_records_t *records_object );

Py_ssize_t pyevtx_records_len(
            pyevtx_records_t *records_object );

PyObject *pyevtx_records_getitem(
           pyevtx_records_t *records_object,
           Py_ssize_t item_index );

PyObject *pyevtx_records_iter(
           pyevtx_records_t *records_object );

PyObject *pyevtx_records_iternext(
           pyevtx_records_t *records_object );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _PYEVTX_RECORDS_H ) */

