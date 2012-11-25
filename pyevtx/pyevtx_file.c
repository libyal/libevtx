/*
 * Python object definition of the libevtx file
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
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( HAVE_WINAPI )
#include <stdlib.h>
#endif

#include "pyevtx.h"
#include "pyevtx_codepage.h"
#include "pyevtx_file.h"
#include "pyevtx_file_object_io_handle.h"
#include "pyevtx_libbfio.h"
#include "pyevtx_libcerror.h"
#include "pyevtx_libclocale.h"
#include "pyevtx_libcstring.h"
#include "pyevtx_libevtx.h"
#include "pyevtx_python.h"
#include "pyevtx_record.h"
#include "pyevtx_records.h"

#if !defined( LIBEVTX_HAVE_BFIO )
LIBEVTX_EXTERN \
int libevtx_file_open_file_io_handle(
     libevtx_file_t *file,
     libbfio_handle_t *file_io_handle,
     int access_flags,
     libevtx_error_t **error );
#endif

PyMethodDef pyevtx_file_object_methods[] = {

	{ "signal_abort",
	  (PyCFunction) pyevtx_file_signal_abort,
	  METH_NOARGS,
	  "signal_abort() -> None\n"
	  "\n"
	  "Signals the file to abort the current activity" },

	/* Functions to access the file */

	{ "open",
	  (PyCFunction) pyevtx_file_open,
	  METH_VARARGS | METH_KEYWORDS,
	  "open(filename, access_flags) -> None\n"
	  "\n"
	  "Opens a file" },

	{ "open_file_object",
	  (PyCFunction) pyevtx_file_open_file_object,
	  METH_VARARGS | METH_KEYWORDS,
	  "open(file_object, access_flags) -> None\n"
	  "\n"
	  "Opens a file using a file-like object" },

	{ "close",
	  (PyCFunction) pyevtx_file_close,
	  METH_NOARGS,
	  "close() -> None\n"
	  "\n"
	  "Closes a file" },

	{ "get_ascii_codepage",
	  (PyCFunction) pyevtx_file_get_ascii_codepage,
	  METH_NOARGS,
	  "get_ascii_codepage() -> String\n"
	  "\n"
	  "Returns the codepage used for ASCII strings in the file" },

	{ "set_ascii_codepage",
	  (PyCFunction) pyevtx_file_set_ascii_codepage,
	  METH_VARARGS | METH_KEYWORDS,
	  "set_ascii_codepage(codepage) -> None\n"
	  "\n"
	  "Set the codepage used for ASCII strings in the file\n"
	  "Expects the codepage to be a String containing a Python codec definition" },

	/* Functions to access the records */

	{ "get_number_of_records",
	  (PyCFunction) pyevtx_file_get_number_of_records,
	  METH_NOARGS,
	  "get_number_of_records() -> Integer\n"
	  "\n"
	  "Retrieves the number of records" },

	{ "get_record",
	  (PyCFunction) pyevtx_file_get_record,
	  METH_VARARGS | METH_KEYWORDS,
	  "get_record(index) -> Object or None\n"
	  "\n"
	  "Retrieves a specific record" },

	{ "get_number_of_recovered_records",
	  (PyCFunction) pyevtx_file_get_number_of_recovered_records,
	  METH_NOARGS,
	  "get_number_of_recovered_records() -> Integer\n"
	  "\n"
	  "Retrieves the number of recovered records" },

	{ "get_recovered_record",
	  (PyCFunction) pyevtx_file_get_recovered_record,
	  METH_VARARGS | METH_KEYWORDS,
	  "get_recovered_record(index) -> Object or None\n"
	  "\n"
	  "Retrieves a specific recovered record" },

	/* Sentinel */
	{ NULL, NULL, 0, NULL }
};

PyGetSetDef pyevtx_file_object_get_set_definitions[] = {

	{ "ascii_codepage",
	  (getter) pyevtx_file_get_ascii_codepage,
	  (setter) pyevtx_file_set_ascii_codepage,
	  "The codepage used for ASCII strings in the file",
	  NULL },

	{ "number_of_records",
	  (getter) pyevtx_file_get_number_of_records,
	  (setter) 0,
	  "The number of records",
	  NULL },

	{ "records",
	  (getter) pyevtx_file_get_records,
	  (setter) 0,
	  "The records",
	  NULL },

	{ "number_of_recovered_records",
	  (getter) pyevtx_file_get_number_of_recovered_records,
	  (setter) 0,
	  "The number of records",
	  NULL },

	{ "recoverd_records",
	  (getter) pyevtx_file_get_recovered_records,
	  (setter) 0,
	  "The recovered records",
	  NULL },

	/* Sentinel */
	{ NULL, NULL, NULL, NULL, NULL }
};

PyTypeObject pyevtx_file_type_object = {
	PyObject_HEAD_INIT( NULL )

	/* ob_size */
	0,
	/* tp_name */
	"pyevtx.file",
	/* tp_basicsize */
	sizeof( pyevtx_file_t ),
	/* tp_itemsize */
	0,
	/* tp_dealloc */
	(destructor) pyevtx_file_free,
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
	"pyevtx file object (wraps libevtx_file_t)",
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
	pyevtx_file_object_methods,
	/* tp_members */
	0,
	/* tp_getset */
	pyevtx_file_object_get_set_definitions,
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
	(initproc) pyevtx_file_init,
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

/* Creates a new file object
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyevtx_file_new(
           void )
{
	pyevtx_file_t *pyevtx_file = NULL;
	static char *function      = "pyevtx_file_new";

	pyevtx_file = PyObject_New(
	               struct pyevtx_file,
	               &pyevtx_file_type_object );

	if( pyevtx_file == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to initialize file.",
		 function );

		return( NULL );
	}
	if( pyevtx_file_init(
	     pyevtx_file ) != 0 )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to initialize file.",
		 function );

		goto on_error;
	}
	return( (PyObject *) pyevtx_file );

on_error:
	if( pyevtx_file != NULL )
	{
		Py_DecRef(
		 (PyObject *) pyevtx_file );
	}
	return( NULL );
}

/* Creates a new file object and opens it
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyevtx_file_new_open(
           PyObject *self,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *pyevtx_file = NULL;

	pyevtx_file = pyevtx_file_new();

	pyevtx_file_open(
	 (pyevtx_file_t *) pyevtx_file,
	 arguments,
	 keywords );

	return( pyevtx_file );
}

/* Intializes a file object
 * Returns 0 if successful or -1 on error
 */
int pyevtx_file_init(
     pyevtx_file_t *pyevtx_file )
{
	char error_string[ PYEVTX_ERROR_STRING_SIZE ];

	static char *function    = "pyevtx_file_init";
	libcerror_error_t *error = NULL;

	if( pyevtx_file == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	/* Make sure libevtx file is set to NULL
	 */
	pyevtx_file->file = NULL;

	if( libevtx_file_initialize(
	     &( pyevtx_file->file ),
	     &error ) != 1 )
	{
		if( libcerror_error_backtrace_sprint(
		     error,
		     error_string,
		     PYEVTX_ERROR_STRING_SIZE ) == -1 )
                {
			PyErr_Format(
			 PyExc_MemoryError,
			 "%s: unable to initialize file.",
			 function );
		}
		else
		{
			PyErr_Format(
			 PyExc_MemoryError,
			 "%s: unable to initialize file.\n%s",
			 function,
			 error_string );
		}
		libcerror_error_free(
		 &error );

		return( -1 );
	}
	return( 0 );
}

/* Frees a file object
 */
void pyevtx_file_free(
      pyevtx_file_t *pyevtx_file )
{
	char error_string[ PYEVTX_ERROR_STRING_SIZE ];

	libcerror_error_t *error = NULL;
	static char *function    = "pyevtx_file_free";
	int result               = 0;

	if( pyevtx_file == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file.",
		 function );

		return;
	}
	if( pyevtx_file->ob_type == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file - missing ob_type.",
		 function );

		return;
	}
	if( pyevtx_file->ob_type->tp_free == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file - invalid ob_type - missing tp_free.",
		 function );

		return;
	}
	if( pyevtx_file->file == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file - missing libevtx file.",
		 function );

		return;
	}
	Py_BEGIN_ALLOW_THREADS

	result = libevtx_file_free(
	          &( pyevtx_file->file ),
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
			 PyExc_MemoryError,
			 "%s: unable to free libevtx file.",
			 function );
		}
		else
		{
			PyErr_Format(
			 PyExc_MemoryError,
			 "%s: unable to free libevtx file.\n%s",
			 function,
			 error_string );
		}
		libcerror_error_free(
		 &error );
	}
	pyevtx_file->ob_type->tp_free(
	 (PyObject*) pyevtx_file );
}

/* Signals the file to abort the current activity
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyevtx_file_signal_abort(
           pyevtx_file_t *pyevtx_file )
{
	char error_string[ PYEVTX_ERROR_STRING_SIZE ];

	libcerror_error_t *error = NULL;
	static char *function    = "pyevtx_file_signal_abort";
	int result               = 0;

	if( pyevtx_file == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libevtx_file_signal_abort(
	          pyevtx_file->file,
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
			 "%s: unable to signal abort.",
			 function );
		}
		else
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to signal abort.\n%s",
			 function,
			 error_string );
		}
		libcerror_error_free(
		 &error );

		return( NULL );
	}
	Py_IncRef(
	 Py_None );

	return( Py_None );
}

/* Opens a file
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyevtx_file_open(
           pyevtx_file_t *pyevtx_file,
           PyObject *arguments,
           PyObject *keywords )
{
	char error_string[ PYEVTX_ERROR_STRING_SIZE ];

	libcerror_error_t *error    = NULL;
	char *filename              = NULL;
	static char *keyword_list[] = { "filename", "access_flags", NULL };
	static char *function       = "pyevtx_file_open";
	int access_flags            = 0;
	int result                  = 0;

	if( pyevtx_file == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file.",
		 function );

		return( NULL );
	}
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "s|i",
	     keyword_list,
	     &filename,
	     &access_flags ) == 0 )
        {
                return( NULL );
        }
	/* Default to read-only if no access flags were provided
	 */
	if( access_flags == 0 )
	{
		access_flags = libevtx_get_access_flags_read();
	}
	Py_BEGIN_ALLOW_THREADS

	result = libevtx_file_open(
	          pyevtx_file->file,
                  filename,
                  access_flags,
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
			 "%s: unable to open file.",
			 function );
		}
		else
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to open file.\n%s",
			 function,
			 error_string );
		}
		libcerror_error_free(
		 &error );

		return( NULL );
	}
	Py_IncRef(
	 Py_None );

	return( Py_None );
}

/* Opens a file using a file-like object
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyevtx_file_open_file_object(
           pyevtx_file_t *pyevtx_file,
           PyObject *arguments,
           PyObject *keywords )
{
	char error_string[ PYEVTX_ERROR_STRING_SIZE ];

	PyObject *file_object            = NULL;
	libbfio_handle_t *file_io_handle = NULL;
	libcerror_error_t *error         = NULL;
	static char *keyword_list[]      = { "file_object", "access_flags", NULL };
	static char *function            = "pyevtx_file_open_file_object";
	int access_flags                 = 0;
	int result                       = 0;

	if( pyevtx_file == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file.",
		 function );

		return( NULL );
	}
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "O|i",
	     keyword_list,
	     &file_object,
	     &access_flags ) == 0 )
        {
                return( NULL );
        }
	/* Default to read-only if no access flags were provided
	 */
	if( access_flags == 0 )
	{
		access_flags = libevtx_get_access_flags_read();
	}
	if( pyevtx_file_object_initialize(
	     &file_io_handle,
	     file_object,
	     &error ) != 1 )
	{
		if( libcerror_error_backtrace_sprint(
		     error,
		     error_string,
		     PYEVTX_ERROR_STRING_SIZE ) == -1 )
                {
			PyErr_Format(
			 PyExc_MemoryError,
			 "%s: unable to initialize file IO handle.",
			 function );
		}
		else
		{
			PyErr_Format(
			 PyExc_MemoryError,
			 "%s: unable to initialize file IO handle.\n%s",
			 function,
			 error_string );
		}
		libcerror_error_free(
		 &error );

		goto on_error;
	}
	Py_BEGIN_ALLOW_THREADS

	result = libevtx_file_open_file_io_handle(
	          pyevtx_file->file,
                  file_io_handle,
                  access_flags,
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
			 "%s: unable to open file.",
			 function );
		}
		else
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to open file.\n%s",
			 function,
			 error_string );
		}
		libcerror_error_free(
		 &error );

		goto on_error;
	}
	Py_IncRef(
	 Py_None );

	return( Py_None );

on_error:
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
	return( NULL );
}

/* Closes a file
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyevtx_file_close(
           pyevtx_file_t *pyevtx_file )
{
	char error_string[ PYEVTX_ERROR_STRING_SIZE ];

	libcerror_error_t *error = NULL;
	static char *function    = "pyevtx_file_close";
	int result               = 0;

	if( pyevtx_file == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libevtx_file_close(
	          pyevtx_file->file,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 0 )
	{
		if( libcerror_error_backtrace_sprint(
		     error,
		     error_string,
		     PYEVTX_ERROR_STRING_SIZE ) == -1 )
                {
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to close file.",
			 function );
		}
		else
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to close file.\n%s",
			 function,
			 error_string );
		}
		libcerror_error_free(
		 &error );

		return( NULL );
	}
	Py_IncRef(
	 Py_None );

	return( Py_None );
}

/* Retrieves the codepage used for ASCII strings in the file
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyevtx_file_get_ascii_codepage(
           pyevtx_file_t *pyevtx_file )
{
	char error_string[ PYEVTX_ERROR_STRING_SIZE ];

	libcerror_error_t *error    = NULL;
	PyObject *string_object     = NULL;
	const char *codepage_string = NULL;
	static char *function       = "pyevtx_file_get_ascii_codepage";
	int ascii_codepage          = 0;

	if( pyevtx_file == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file.",
		 function );

		return( NULL );
	}
	if( libevtx_file_get_ascii_codepage(
	     pyevtx_file->file,
	     &ascii_codepage,
	     &error ) != 1 )
	{
		if( libcerror_error_backtrace_sprint(
		     error,
		     error_string,
		     PYEVTX_ERROR_STRING_SIZE ) == -1 )
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to retrieve ASCII codepage.",
			 function );
		}
		else
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to retrieve ASCII codepage.\n%s",
			 function,
			 error_string );
		}
		libcerror_error_free(
		 &error );

		return( NULL );
	}
	codepage_string = pyevtx_codepage_to_string(
	                   ascii_codepage );

	if( codepage_string == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: unsupported ASCII codepage: %d.",
		 function,
		 ascii_codepage );

		return( NULL );
	}
	string_object = PyString_FromString(
	                 codepage_string );

	if( string_object == NULL )
	{
		PyErr_Format(
		 PyExc_IOError,
		 "%s: unable to convert codepage string into string object.",
		 function );

		return( NULL );
	}
	return( string_object );
}

/* Sets the codepage used for ASCII strings in the file
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyevtx_file_set_ascii_codepage(
           pyevtx_file_t *pyevtx_file,
           PyObject *arguments,
           PyObject *keywords )
{
	char error_string[ PYEVTX_ERROR_STRING_SIZE ];

	libcerror_error_t *error      = NULL;
	char *codepage_string         = NULL;
	static char *keyword_list[]   = { "codepage", NULL };
	static char *function         = "pyevtx_file_set_ascii_codepage";
	size_t codepage_string_length = 0;
	uint32_t feature_flags        = 0;
	int ascii_codepage            = 0;

	if( pyevtx_file == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file.",
		 function );

		return( NULL );
	}
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "s",
	     keyword_list,
	     &codepage_string ) == 0 )
        {
                return( NULL );
        }
	if( codepage_string == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid codepage string.",
		 function );

		return( NULL );
	}
	codepage_string_length = libcstring_narrow_string_length(
	                          codepage_string );

	feature_flags = LIBCLOCALE_CODEPAGE_FEATURE_FLAG_HAVE_KOI8
	              | LIBCLOCALE_CODEPAGE_FEATURE_FLAG_HAVE_WINDOWS;

	if( libclocale_codepage_copy_from_string(
	     &ascii_codepage,
	     codepage_string,
	     codepage_string_length,
	     feature_flags,
	     &error ) != 1 )
	{
		if( libcerror_error_backtrace_sprint(
		     error,
		     error_string,
		     PYEVTX_ERROR_STRING_SIZE ) == -1 )
		{
			PyErr_Format(
			 PyExc_RuntimeError,
			 "%s: unable to determine ASCII codepage.",
			 function );
		}
		else
		{
			PyErr_Format(
			 PyExc_RuntimeError,
			 "%s: unable to determine ASCII codepage.\n%s",
			 function,
			 error_string );
		}
		libcerror_error_free(
		 &error );

		return( NULL );
	}
	if( libevtx_file_set_ascii_codepage(
	     pyevtx_file->file,
	     ascii_codepage,
	     &error ) != 1 )
	{
		if( libcerror_error_backtrace_sprint(
		     error,
		     error_string,
		     PYEVTX_ERROR_STRING_SIZE ) == -1 )
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to set ASCII codepage.",
			 function );
		}
		else
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to set ASCII codepage.\n%s",
			 function,
			 error_string );
		}
		libcerror_error_free(
		 &error );

		return( NULL );
	}
	Py_IncRef(
	 Py_None );

	return( Py_None );
}

/* Retrieves the number of records
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyevtx_file_get_number_of_records(
           pyevtx_file_t *pyevtx_file )
{
	char error_string[ PYEVTX_ERROR_STRING_SIZE ];

	libcerror_error_t *error = NULL;
	static char *function    = "pyevtx_file_get_number_of_records";
	int number_of_records    = 0;
	int result               = 0;

	if( pyevtx_file == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid file.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libevtx_file_get_number_of_records(
	          pyevtx_file->file,
	          &number_of_records,
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
			 "%s: unable to retrieve number of records.",
			 function );
		}
		else
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to retrieve number of records.\n%s",
			 function,
			 error_string );
		}
		libcerror_error_free(
		 &error );

		return( NULL );
	}
	return( PyInt_FromLong(
	         (long) number_of_records ) );
}

/* Retrieves a specific record by index
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyevtx_file_get_record_by_index(
           pyevtx_file_t *pyevtx_file,
           int record_index )
{
	char error_string[ PYEVTX_ERROR_STRING_SIZE ];

	libcerror_error_t *error = NULL;
	libevtx_record_t *record = NULL;
	PyObject *record_object  = NULL;
	static char *function    = "pyevtx_file_get_record_by_index";
	int result               = 0;

	if( pyevtx_file == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid file.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libevtx_file_get_record(
	          pyevtx_file->file,
	          record_index,
	          &record,
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
			 "%s: unable to retrieve record: %d.",
			 function,
			 record_index );
		}
		else
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to retrieve record: %d.\n%s",
			 function,
			 record_index,
			 error_string );
		}
		libcerror_error_free(
		 &error );

		goto on_error;
	}
	record_object = pyevtx_record_new(
	                 record,
	                 pyevtx_file );

	if( record_object == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create record object.",
		 function );

		goto on_error;
	}
	return( record_object );

on_error:
	if( record != NULL )
	{
		libevtx_record_free(
		 &record,
		 NULL );
	}
	return( NULL );
}

/* Retrieves a specific record
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyevtx_file_get_record(
           pyevtx_file_t *pyevtx_file,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *record_object     = NULL;
	static char *keyword_list[] = { "record_index", NULL };
	static char *function       = "pyevtx_file_get_record";
	int record_index            = 0;

	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "i",
	     keyword_list,
	     &record_index ) == 0 )
        {
		return( NULL );
        }
	record_object = pyevtx_file_get_record_by_index(
	                 pyevtx_file,
	                 record_index );

	return( record_object );
}

/* Retrieves a records sequence and iterator object for the records
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyevtx_file_get_records(
           pyevtx_file_t *pyevtx_file )
{
	char error_string[ PYEVTX_ERROR_STRING_SIZE ];

	libcerror_error_t *error = NULL;
	PyObject *records_object = NULL;
	static char *function    = "pyevtx_file_get_records";
	int number_of_records    = 0;
	int result               = 0;

	if( pyevtx_file == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid file.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libevtx_file_get_number_of_records(
	          pyevtx_file->file,
	          &number_of_records,
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
			 "%s: unable to retrieve number of records.",
			 function );
		}
		else
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to retrieve number of records.\n%s",
			 function,
			 error_string );
		}
		libcerror_error_free(
		 &error );

		return( NULL );
	}
	records_object = pyevtx_records_new(
	                  pyevtx_file,
	                  &pyevtx_file_get_record_by_index,
	                  number_of_records );

	if( records_object == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create records object.",
		 function );

		return( NULL );
	}
	return( records_object );
}

/* Retrieves the number of recovered records
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyevtx_file_get_number_of_recovered_records(
           pyevtx_file_t *pyevtx_file )
{
	char error_string[ PYEVTX_ERROR_STRING_SIZE ];

	libcerror_error_t *error = NULL;
	static char *function    = "pyevtx_file_get_number_of_recovered_records";
	int number_of_records    = 0;
	int result               = 0;

	if( pyevtx_file == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid file.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libevtx_file_get_number_of_recovered_records(
	          pyevtx_file->file,
	          &number_of_records,
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
			 "%s: unable to retrieve number of recovered records.",
			 function );
		}
		else
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to retrieve number of recovered records.\n%s",
			 function,
			 error_string );
		}
		libcerror_error_free(
		 &error );

		return( NULL );
	}
	return( PyInt_FromLong(
	         (long) number_of_records ) );
}

/* Retrieves a specific recovered record by index
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyevtx_file_get_recovered_record_by_index(
           pyevtx_file_t *pyevtx_file,
           int record_index )
{
	char error_string[ PYEVTX_ERROR_STRING_SIZE ];

	libcerror_error_t *error = NULL;
	libevtx_record_t *record = NULL;
	PyObject *record_object  = NULL;
	static char *function    = "pyevtx_file_get_recovered_record_by_index";
	int result               = 0;

	if( pyevtx_file == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid file.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libevtx_file_get_recovered_record(
	          pyevtx_file->file,
	          record_index,
	          &record,
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
			 "%s: unable to retrieve recovered record: %d.",
			 function,
			 record_index );
		}
		else
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to retrieve recovered record: %d.\n%s",
			 function,
			 record_index,
			 error_string );
		}
		libcerror_error_free(
		 &error );

		goto on_error;
	}
	record_object = pyevtx_record_new(
	                 record,
	                 pyevtx_file );

	if( record_object == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create recovered record object.",
		 function );

		goto on_error;
	}
	return( record_object );

on_error:
	if( record != NULL )
	{
		libevtx_record_free(
		 &record,
		 NULL );
	}
	return( NULL );
}

/* Retrieves a specific recovered record
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyevtx_file_get_recovered_record(
           pyevtx_file_t *pyevtx_file,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *record_object     = NULL;
	static char *keyword_list[] = { "record_index", NULL };
	static char *function       = "pyevtx_file_get_recovered_record";
	int record_index            = 0;

	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "i",
	     keyword_list,
	     &record_index ) == 0 )
        {
		return( NULL );
        }
	record_object = pyevtx_file_get_recovered_record_by_index(
	                 pyevtx_file,
	                 record_index );

	return( record_object );
}

/* Retrieves a records sequence and iterator object for the recoverd records
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyevtx_file_get_recovered_records(
           pyevtx_file_t *pyevtx_file )
{
	char error_string[ PYEVTX_ERROR_STRING_SIZE ];

	libcerror_error_t *error = NULL;
	PyObject *records_object = NULL;
	static char *function    = "pyevtx_file_get_recovered_records";
	int number_of_records    = 0;
	int result               = 0;

	if( pyevtx_file == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid file.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libevtx_file_get_number_of_recovered_records(
	          pyevtx_file->file,
	          &number_of_records,
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
			 "%s: unable to retrieve number of recovered records.",
			 function );
		}
		else
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to retrieve number of recovered records.\n%s",
			 function,
			 error_string );
		}
		libcerror_error_free(
		 &error );

		return( NULL );
	}
	records_object = pyevtx_records_new(
	                  pyevtx_file,
	                  &pyevtx_file_get_recovered_record_by_index,
	                  number_of_records );

	if( records_object == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create records object.",
		 function );

		return( NULL );
	}
	return( records_object );
}

