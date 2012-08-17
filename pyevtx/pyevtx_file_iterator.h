/*
 * Python object definition of the file iterator
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

#if !defined( _PYEVTX_FILE_ITERATOR_H )
#define _PYEVTX_FILE_ITERATOR_H

#include <common.h>
#include <types.h>

#include "pyevtx_file.h"
#include "pyevtx_libevtx.h"
#include "pyevtx_python.h"

#if defined( __cplusplus )
extern "C" {
#endif

enum PYEVTX_FILE_ITERATOR_MODES
{
	PYEVTX_FILE_ITERATOR_MODE_ITEMS		= 0,
	PYEVTX_FILE_ITERATOR_MODE_RECOVERED	= 1
};

typedef struct pyevtx_file_iterator pyevtx_file_iterator_t;

struct pyevtx_file_iterator
{
	/* Python object initialization
	 */
	PyObject_HEAD

	/* The pyevtx file object
	 */
	pyevtx_file_t *file_object;

	/* The mode
	 */
	int mode;

	/* The (current) record index
	 */
	int record_index;

	/* The number of records
	 */
	int number_of_records;
};

extern PyTypeObject pyevtx_file_iterator_type_object;

PyObject *pyevtx_file_iterator_new(
           pyevtx_file_t *file_object,
           int mode,
           int number_of_records );

int pyevtx_file_iterator_init(
     pyevtx_file_iterator_t *pyevtx_file_iterator );

void pyevtx_file_iterator_free(
      pyevtx_file_iterator_t *pyevtx_file_iterator );

#if defined( __cplusplus )
}
#endif

#endif

