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

#include <common.h>
#include <memory.h>
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( HAVE_WINAPI )
#include <stdlib.h>
#endif

#include "pyevtx.h"
#include "pyevtx_file.h"
#include "pyevtx_file_iterator.h"
#include "pyevtx_libcerror.h"
#include "pyevtx_libevtx.h"
#include "pyevtx_python.h"
#include "pyevtx_record.h"

PyTypeObject pyevtx_file_iterator_type_object = {
	PyObject_HEAD_INIT( NULL )

	/* ob_size */
	0,
	/* tp_name */
	"pyevtx._file_iterator",
	/* tp_basicsize */
	sizeof( pyevtx_file_iterator_t ),
	/* tp_itemsize */
	0,
	/* tp_dealloc */
	(destructor) pyevtx_file_iterator_free,
	/* tp_print */
	0,
	/* tp_getattr */
	0,
	/* tp_setattr */
	0,
	/* tp_compare */
	0,
	/* tp_repr */
	0,
	/* tp_as_number */
	0,
	/* tp_as_sequence */
	0,
	/* tp_as_mapping */
	0,
	/* tp_hash */
	0,
	/* tp_call */
	0,
	/* tp_str */
	0,
	/* tp_getattro */
	0,
	/* tp_setattro */
	0,
	/* tp_as_buffer */
	0,
        /* tp_flags */
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_HAVE_ITER,
	/* tp_doc */
	"internal pyevtx file iteration object",
	/* tp_traverse */
	0,
	/* tp_clear */
	0,
	/* tp_richcompare */
	0,
	/* tp_weaklistoffset */
	0,
	/* tp_iter */
	0,
	/* tp_iternext */
	0,
	/* tp_methods */
	0,
	/* tp_members */
	0,
	/* tp_getset */
	0,
	/* tp_base */
	0,
	/* tp_dict */
	0,
	/* tp_descr_get */
	0,
	/* tp_descr_set */
	0,
	/* tp_dictoffset */
	0,
	/* tp_init */
	(initproc) pyevtx_file_iterator_init,
	/* tp_alloc */
	0,
	/* tp_new */
	0,
	/* tp_free */
	0,
	/* tp_is_gc */
	0,
	/* tp_bases */
	NULL,
	/* tp_mro */
	NULL,
	/* tp_cache */
	NULL,
	/* tp_subclasses */
	NULL,
	/* tp_weaklist */
	NULL,
	/* tp_del */
	0
};

/* Creates a new file iterator object
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyevtx_file_iterator_new(
           pyevtx_file_t *file_object,
           int mode,
           int number_of_records )
{
	pyevtx_file_iterator_t *pyevtx_file_iterator = NULL;
	static char *function                        = "pyevtx_file_iterator_new";

	if( file_object == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file object.",
		 function );

		return( NULL );
	}
	/* Make sure the file iterator values are initialized
	 */
	pyevtx_file_iterator = PyObject_New(
	                        struct pyevtx_file_iterator,
	                        &pyevtx_file_iterator_type_object );

	if( pyevtx_file_iterator == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to initialize file iterator.",
		 function );

		return( NULL );
	}
	if( pyevtx_file_iterator_init(
	     pyevtx_file_iterator ) != 0 )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to initialize file iterator.",
		 function );

		goto on_error;
	}
	pyevtx_file_iterator->file_object = file_object;

	Py_IncRef(
	 (PyObject *) pyevtx_file_iterator->file_object );

	return( (PyObject *) pyevtx_file_iterator );

on_error:
	if( pyevtx_file_iterator != NULL )
	{
		Py_DecRef(
		 (PyObject *) pyevtx_file_iterator );
	}
	return( NULL );
}

/* Intializes a file iterator object
 * Returns 0 if successful or -1 on error
 */
int pyevtx_file_iterator_init(
     pyevtx_file_iterator_t *pyevtx_file_iterator )
{
	static char *function    = "pyevtx_file_iterator_init";
	libcerror_error_t *error = NULL;

	if( pyevtx_file_iterator == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file iterator.",
		 function );

		return( -1 );
	}
	/* Make sure the file iterator values are initialized
	 */
	pyevtx_file_iterator->file_object       = NULL;
	pyevtx_file_iterator->mode              = 0;
	pyevtx_file_iterator->record_index      = 0;
	pyevtx_file_iterator->number_of_records = 0;

	return( 0 );
}

/* Frees a file iterator object
 */
void pyevtx_file_iterator_free(
      pyevtx_file_iterator_t *pyevtx_file_iterator )
{
	libcerror_error_t *error = NULL;
	static char *function    = "pyevtx_file_iterator_free";
	int result               = 0;

	if( pyevtx_file_iterator == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file iterator.",
		 function );

		return;
	}
	if( pyevtx_file_iterator->ob_type == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file iterator - missing ob_type.",
		 function );

		return;
	}
	if( pyevtx_file_iterator->ob_type->tp_free == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file iterator - invalid ob_type - missing tp_free.",
		 function );

		return;
	}
	if( pyevtx_file_iterator->file_object != NULL )
	{
		Py_DecRef(
		 (PyObject *) pyevtx_file_iterator->file_object );
	}
	pyevtx_file_iterator->ob_type->tp_free(
	 (PyObject*) pyevtx_file_iterator );
}

/* The file iterator iter() function
 */
PyObject *pyevtx_file_iterator_iter(
           pyevtx_file_iterator_t *pyevtx_file_iterator )
{
	static char *function = "pyevtx_file_iterator_iter";

	if( pyevtx_file_iterator == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file iterator.",
		 function );

		return( NULL );
	}
	Py_IncRef(
	 (PyObject *) pyevtx_file_iterator );

	return( (PyObject *) pyevtx_file_iterator );
}

/* The file iterator iternext() function
 */
PyObject *pyevtx_file_iterator_iternext(
           pyevtx_file_iterator_t *pyevtx_file_iterator )
{
	PyObject *record_object = NULL;
	static char *function   = "pyevtx_file_iterator_iternext";

	if( pyevtx_file_iterator == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file iterator.",
		 function );

		return( NULL );
	}
	if( ( pyevtx_file_iterator->mode != PYEVTX_FILE_ITERATOR_MODE_ITEMS )
	 && ( pyevtx_file_iterator->mode != PYEVTX_FILE_ITERATOR_MODE_RECOVERED ) )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file iterator - unsupported mode.",
		 function );

		return( NULL );
	}
	if( pyevtx_file_iterator->record_index < 0 )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file iterator - invalid record index.",
		 function );

		return( NULL );
	}
	if( pyevtx_file_iterator->number_of_records < 0 )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file iterator - invalid number of records.",
		 function );

		return( NULL );
	}
	if( pyevtx_file_iterator->record_index >= pyevtx_file_iterator->number_of_records )
	{
		PyErr_SetNone(
		 PyExc_StopIteration );

		return( NULL );
	}
	if( pyevtx_file_iterator->mode == PYEVTX_FILE_ITERATOR_MODE_ITEMS )
	{
		record_object = pyevtx_file_get_record_by_index(
		                 pyevtx_file_iterator->file_object,
		                 pyevtx_file_iterator->record_index );
	}
	if( pyevtx_file_iterator->mode == PYEVTX_FILE_ITERATOR_MODE_RECOVERED )
	{
		record_object = pyevtx_file_get_recovered_record_by_index(
		                 pyevtx_file_iterator->file_object,
		                 pyevtx_file_iterator->record_index );
	}
	if( record_object != NULL )
	{
		pyevtx_file_iterator->record_index++;
	}
	return( record_object );
}

