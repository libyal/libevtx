/*
 * Python object definition of the libevtx record
 *
 * Copyright (c) 2009-2012, Joachim Metz <joachim.metz@gmail.com>
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
#include "pyevtx_datetime.h"
#include "pyevtx_libcerror.h"
#include "pyevtx_libcstring.h"
#include "pyevtx_libevtx.h"
#include "pyevtx_python.h"
#include "pyevtx_record.h"

PyMethodDef pyevtx_record_object_methods[] = {

	/* Functions to access the record values */

	{ "get_written_time",
	  (PyCFunction) pyevtx_record_get_written_time,
	  METH_NOARGS,
	  "get_written_time() -> Datetime\n"
	  "\n"
	  "Returns the written date and time" },

	/* Sentinel */
	{ NULL, NULL, 0, NULL }
};

PyGetSetDef pyevtx_record_object_get_set_definitions[] = {

	{ "written_time",
	  (getter) pyevtx_record_get_written_time,
	  (setter) 0,
	  "The written date and time",
	  NULL },

	/* Sentinel */
	{ NULL, NULL, NULL, NULL, NULL }
};

PyTypeObject pyevtx_record_type_object = {
	PyObject_HEAD_INIT( NULL )

	/* ob_size */
	0,
	/* tp_name */
	"pyevtx.record",
	/* tp_basicsize */
	sizeof( pyevtx_record_t ),
	/* tp_recordsize */
	0,
	/* tp_dealloc */
	(destructor) pyevtx_record_free,
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
	Py_TPFLAGS_DEFAULT,
	/* tp_doc */
	"pyevtx record object (wraps libevtx_record_t)",
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
	pyevtx_record_object_methods,
	/* tp_members */
	0,
	/* tp_getset */
	pyevtx_record_object_get_set_definitions,
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
	(initproc) pyevtx_record_init,
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

/* Creates a new record object
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyevtx_record_new(
           PyObject *self )
{
	pyevtx_record_t *pyevtx_record = NULL;
	static char *function    = "pyevtx_record_new";

	pyevtx_record = PyObject_New(
	              struct pyevtx_record,
	              &pyevtx_record_type_object );

	if( pyevtx_record == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to initialize record.",
		 function );

		return( NULL );
	}
	if( pyevtx_record_init(
	     pyevtx_record ) != 0 )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to initialize record.",
		 function );

		goto on_error;
	}
	return( (PyObject *) pyevtx_record );

on_error:
	if( pyevtx_record != NULL )
	{
		Py_DecRef(
		 (PyObject *) pyevtx_record );
	}
	return( NULL );
}

/* Intializes an record object
 * Returns 0 if successful or -1 on error
 */
int pyevtx_record_init(
     pyevtx_record_t *pyevtx_record )
{
	static char *function = "pyevtx_record_init";

	if( pyevtx_record == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	/* Make sure libevtx record is set to NULL
	 */
	pyevtx_record->record = NULL;

	return( 0 );
}

/* Frees an record object
 */
void pyevtx_record_free(
      pyevtx_record_t *pyevtx_record )
{
	char error_string[ PYEVTX_ERROR_STRING_SIZE ];

	libcerror_error_t *error = NULL;
	static char *function    = "pyevtx_record_free";

	if( pyevtx_record == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid record.",
		 function );

		return;
	}
	if( pyevtx_record->ob_type == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid record - missing ob_type.",
		 function );

		return;
	}
	if( pyevtx_record->ob_type->tp_free == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid record - invalid ob_type - missing tp_free.",
		 function );

		return;
	}
	if( pyevtx_record->record == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid record - missing libevtx record.",
		 function );

		return;
	}
	if( libevtx_record_free(
	     &( pyevtx_record->record ),
	     &error ) != 1 )
	{
		if( libcerror_error_backtrace_sprint(
		     error,
		     error_string,
		     PYEVTX_ERROR_STRING_SIZE ) == -1 )
                {
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to free libevtx record.",
			 function );
		}
		else
                {
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to free libevtx record.\n%s",
			 function,
			 error_string );
		}
		libcerror_error_free(
		 &error );
	}
	pyevtx_record->ob_type->tp_free(
	 (PyObject*) pyevtx_record );
}

/* Retrieves the written date and time
 * Returns a Python object holding the offset if successful or NULL on error
 */
PyObject *pyevtx_record_get_written_time(
           pyevtx_record_t *pyevtx_record )
{
	char error_string[ PYEVTX_ERROR_STRING_SIZE ];

	libcerror_error_t *error   = NULL;
	PyObject *date_time_object = NULL;
	static char *function      = "pyevtx_record_get_written_time";
	uint64_t filetime          = 0;
	int result                 = 0;

	if( pyevtx_record == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid record.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libevtx_record_get_written_time(
	          pyevtx_record->record,
	          &filetime,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		if( libcerror_error_backtrace_sprint(
		     error,
		     error_string,
		     PYEVTX_ERROR_STRING_SIZE ) == -1 )
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to retrieve written time.",
			 function );
		}
		else
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to retrieve written time.\n%s",
			 function,
			 error_string );
		}
		libcerror_error_free(
		 &error );

		return( NULL );
	}
	date_time_object = pyevtx_datetime_new_from_filetime(
	                    filetime );

	return( date_time_object );
}

