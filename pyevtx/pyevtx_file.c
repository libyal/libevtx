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

#include <common.h>
#include <narrow_string.h>
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( HAVE_WINAPI )
#include <stdlib.h>
#endif

#include "pyevtx_codepage.h"
#include "pyevtx_error.h"
#include "pyevtx_file.h"
#include "pyevtx_file_object_io_handle.h"
#include "pyevtx_libbfio.h"
#include "pyevtx_libcerror.h"
#include "pyevtx_libclocale.h"
#include "pyevtx_libevtx.h"
#include "pyevtx_python.h"
#include "pyevtx_record.h"
#include "pyevtx_records.h"
#include "pyevtx_unused.h"

#if !defined( LIBEVTX_HAVE_BFIO )

LIBEVTX_EXTERN \
int libevtx_file_open_file_io_handle(
     libevtx_file_t *file,
     libbfio_handle_t *file_io_handle,
     int access_flags,
     libevtx_error_t **error );

#endif /* !defined( LIBEVTX_HAVE_BFIO ) */

PyMethodDef pyevtx_file_object_methods[] = {

	{ "signal_abort",
	  (PyCFunction) pyevtx_file_signal_abort,
	  METH_NOARGS,
	  "signal_abort() -> None\n"
	  "\n"
	  "Signals the file to abort the current activity." },

	{ "open",
	  (PyCFunction) pyevtx_file_open,
	  METH_VARARGS | METH_KEYWORDS,
	  "open(filename, mode='r') -> None\n"
	  "\n"
	  "Opens a file." },

	{ "open_file_object",
	  (PyCFunction) pyevtx_file_open_file_object,
	  METH_VARARGS | METH_KEYWORDS,
	  "open_file_object(file_object, mode='r') -> None\n"
	  "\n"
	  "Opens a file using a file-like object." },

	{ "close",
	  (PyCFunction) pyevtx_file_close,
	  METH_NOARGS,
	  "close() -> None\n"
	  "\n"
	  "Closes a file." },

	{ "is_corrupted",
	  (PyCFunction) pyevtx_file_is_corrupted,
	  METH_NOARGS,
	  "is_corrupted() -> Boolean\n"
	  "\n"
	  "Determines if the file is corrupted." },

	{ "get_ascii_codepage",
	  (PyCFunction) pyevtx_file_get_ascii_codepage,
	  METH_NOARGS,
	  "get_ascii_codepage() -> String\n"
	  "\n"
	  "Retrieves the codepage for ASCII strings used in the file." },

	{ "set_ascii_codepage",
	  (PyCFunction) pyevtx_file_set_ascii_codepage,
	  METH_VARARGS | METH_KEYWORDS,
	  "set_ascii_codepage(codepage) -> None\n"
	  "\n"
	  "Sets the codepage for ASCII strings used in the file.\n"
	  "Expects the codepage to be a string containing a Python codec definition." },

	{ "get_format_version",
	  (PyCFunction) pyevtx_file_get_format_version,
	  METH_NOARGS,
	  "get_format_version() -> Unicode string or None\n"
	  "\n"
	  "Retrieves the format version." },

	{ "get_number_of_records",
	  (PyCFunction) pyevtx_file_get_number_of_records,
	  METH_NOARGS,
	  "get_number_of_records() -> Integer or None\n"
	  "\n"
	  "Retrieves the number of records." },

	{ "get_record",
	  (PyCFunction) pyevtx_file_get_record,
	  METH_VARARGS | METH_KEYWORDS,
	  "get_record(record_index) -> Object or None\n"
	  "\n"
	  "Retrieves the record specified by the index." },

	{ "get_number_of_recovered_records",
	  (PyCFunction) pyevtx_file_get_number_of_recovered_records,
	  METH_NOARGS,
	  "get_number_of_recovered_records() -> Integer or None\n"
	  "\n"
	  "Retrieves the number of recovered records." },

	{ "get_recovered_record",
	  (PyCFunction) pyevtx_file_get_recovered_record,
	  METH_VARARGS | METH_KEYWORDS,
	  "get_recovered_record(record_index) -> Object or None\n"
	  "\n"
	  "Retrieves the recovered record specified by the index." },

	/* Sentinel */
	{ NULL, NULL, 0, NULL }
};

PyGetSetDef pyevtx_file_object_get_set_definitions[] = {

	{ "ascii_codepage",
	  (getter) pyevtx_file_get_ascii_codepage,
	  (setter) pyevtx_file_set_ascii_codepage_setter,
	  "The codepage used for ASCII strings in the file.",
	  NULL },

	{ "format_version",
	  (getter) pyevtx_file_get_format_version,
	  (setter) 0,
	  "The format version.",
	  NULL },

	{ "number_of_records",
	  (getter) pyevtx_file_get_number_of_records,
	  (setter) 0,
	  "The number of records.",
	  NULL },

	{ "records",
	  (getter) pyevtx_file_get_records,
	  (setter) 0,
	  "The records.",
	  NULL },

	{ "number_of_recovered_records",
	  (getter) pyevtx_file_get_number_of_recovered_records,
	  (setter) 0,
	  "The number of recovered records.",
	  NULL },

	{ "recovered_records",
	  (getter) pyevtx_file_get_recovered_records,
	  (setter) 0,
	  "The recovered records.",
	  NULL },

	/* Sentinel */
	{ NULL, NULL, NULL, NULL, NULL }
};

PyTypeObject pyevtx_file_type_object = {
	PyVarObject_HEAD_INIT( NULL, 0 )

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

		goto on_error;
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
           PyObject *self PYEVTX_ATTRIBUTE_UNUSED,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *pyevtx_file = NULL;

	PYEVTX_UNREFERENCED_PARAMETER( self )

	pyevtx_file = pyevtx_file_new();

	pyevtx_file_open(
	 (pyevtx_file_t *) pyevtx_file,
	 arguments,
	 keywords );

	return( pyevtx_file );
}

/* Creates a new file object and opens it using a file-like object
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyevtx_file_new_open_file_object(
           PyObject *self PYEVTX_ATTRIBUTE_UNUSED,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *pyevtx_file = NULL;

	PYEVTX_UNREFERENCED_PARAMETER( self )

	pyevtx_file = pyevtx_file_new();

	pyevtx_file_open_file_object(
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
	libcerror_error_t *error = NULL;
	static char *function    = "pyevtx_file_init";

	if( pyevtx_file == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	pyevtx_file->file           = NULL;
	pyevtx_file->file_io_handle = NULL;

	if( libevtx_file_initialize(
	     &( pyevtx_file->file ),
	     &error ) != 1 )
	{
		pyevtx_error_raise(
		 error,
		 PyExc_MemoryError,
		 "%s: unable to initialize file.",
		 function );

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
	struct _typeobject *ob_type = NULL;
	libcerror_error_t *error    = NULL;
	static char *function       = "pyevtx_file_free";
	int result                  = 0;

	if( pyevtx_file == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file.",
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
	ob_type = Py_TYPE(
	           pyevtx_file );

	if( ob_type == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: missing ob_type.",
		 function );

		return;
	}
	if( ob_type->tp_free == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid ob_type - missing tp_free.",
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
		pyevtx_error_raise(
		 error,
		 PyExc_MemoryError,
		 "%s: unable to free libevtx file.",
		 function );

		libcerror_error_free(
		 &error );
	}
	ob_type->tp_free(
	 (PyObject*) pyevtx_file );
}

/* Signals the file to abort the current activity
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyevtx_file_signal_abort(
           pyevtx_file_t *pyevtx_file,
           PyObject *arguments PYEVTX_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	static char *function    = "pyevtx_file_signal_abort";
	int result               = 0;

	PYEVTX_UNREFERENCED_PARAMETER( arguments )

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
		pyevtx_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to signal abort.",
		 function );

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
	PyObject *string_object      = NULL;
	libcerror_error_t *error     = NULL;
	const char *filename_narrow  = NULL;
	static char *function        = "pyevtx_file_open";
	static char *keyword_list[]  = { "filename", "mode", NULL };
	char *mode                   = NULL;
	int result                   = 0;

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	const wchar_t *filename_wide = NULL;
#else
	PyObject *utf8_string_object = NULL;
#endif

	if( pyevtx_file == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file.",
		 function );

		return( NULL );
	}
	/* Note that PyArg_ParseTupleAndKeywords with "s" will force Unicode strings to be converted to narrow character string.
	 * On Windows the narrow character strings contains an extended ASCII string with a codepage. Hence we get a conversion
	 * exception. This will also fail if the default encoding is not set correctly. We cannot use "u" here either since that
	 * does not allow us to pass non Unicode string objects and Python (at least 2.7) does not seems to automatically upcast them.
	 */
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "O|s",
	     keyword_list,
	     &string_object,
	     &mode ) == 0 )
	{
		return( NULL );
	}
	if( ( mode != NULL )
	 && ( mode[ 0 ] != 'r' ) )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: unsupported mode: %s.",
		 function,
		 mode );

		return( NULL );
	}
	PyErr_Clear();

	result = PyObject_IsInstance(
	          string_object,
	          (PyObject *) &PyUnicode_Type );

	if( result == -1 )
	{
		pyevtx_error_fetch_and_raise(
		 PyExc_RuntimeError,
		 "%s: unable to determine if string object is of type unicode.",
		 function );

		return( NULL );
	}
	else if( result != 0 )
	{
		PyErr_Clear();

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		filename_wide = (wchar_t *) PyUnicode_AsUnicode(
		                             string_object );
		Py_BEGIN_ALLOW_THREADS

		result = libevtx_file_open_wide(
		          pyevtx_file->file,
		          filename_wide,
		          LIBEVTX_OPEN_READ,
		          &error );

		Py_END_ALLOW_THREADS
#else
		utf8_string_object = PyUnicode_AsUTF8String(
		                      string_object );

		if( utf8_string_object == NULL )
		{
			pyevtx_error_fetch_and_raise(
			 PyExc_RuntimeError,
			 "%s: unable to convert unicode string to UTF-8.",
			 function );

			return( NULL );
		}
#if PY_MAJOR_VERSION >= 3
		filename_narrow = PyBytes_AsString(
		                   utf8_string_object );
#else
		filename_narrow = PyString_AsString(
		                   utf8_string_object );
#endif
		Py_BEGIN_ALLOW_THREADS

		result = libevtx_file_open(
		          pyevtx_file->file,
		          filename_narrow,
		          LIBEVTX_OPEN_READ,
		          &error );

		Py_END_ALLOW_THREADS

		Py_DecRef(
		 utf8_string_object );
#endif
		if( result != 1 )
		{
			pyevtx_error_raise(
			 error,
			 PyExc_IOError,
			 "%s: unable to open file.",
			 function );

			libcerror_error_free(
			 &error );

			return( NULL );
		}
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	PyErr_Clear();

#if PY_MAJOR_VERSION >= 3
	result = PyObject_IsInstance(
	          string_object,
	          (PyObject *) &PyBytes_Type );
#else
	result = PyObject_IsInstance(
	          string_object,
	          (PyObject *) &PyString_Type );
#endif
	if( result == -1 )
	{
		pyevtx_error_fetch_and_raise(
		 PyExc_RuntimeError,
		 "%s: unable to determine if string object is of type string.",
		 function );

		return( NULL );
	}
	else if( result != 0 )
	{
		PyErr_Clear();

#if PY_MAJOR_VERSION >= 3
		filename_narrow = PyBytes_AsString(
		                   string_object );
#else
		filename_narrow = PyString_AsString(
		                   string_object );
#endif
		Py_BEGIN_ALLOW_THREADS

		result = libevtx_file_open(
		          pyevtx_file->file,
		          filename_narrow,
		          LIBEVTX_OPEN_READ,
		          &error );

		Py_END_ALLOW_THREADS

		if( result != 1 )
		{
			pyevtx_error_raise(
			 error,
			 PyExc_IOError,
			 "%s: unable to open file.",
			 function );

			libcerror_error_free(
			 &error );

			return( NULL );
		}
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	PyErr_Format(
	 PyExc_TypeError,
	 "%s: unsupported string object type.",
	 function );

	return( NULL );
}

/* Opens a file using a file-like object
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyevtx_file_open_file_object(
           pyevtx_file_t *pyevtx_file,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *file_object       = NULL;
	libcerror_error_t *error    = NULL;
	static char *function       = "pyevtx_file_open_file_object";
	static char *keyword_list[] = { "file_object", "mode", NULL };
	char *mode                  = NULL;
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
	     "O|s",
	     keyword_list,
	     &file_object,
	     &mode ) == 0 )
	{
		return( NULL );
	}
	if( ( mode != NULL )
	 && ( mode[ 0 ] != 'r' ) )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: unsupported mode: %s.",
		 function,
		 mode );

		return( NULL );
	}
	if( pyevtx_file->file_io_handle != NULL )
	{
		pyevtx_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: invalid file - file IO handle already set.",
		 function );

		goto on_error;
	}
	if( pyevtx_file_object_initialize(
	     &( pyevtx_file->file_io_handle ),
	     file_object,
	     &error ) != 1 )
	{
		pyevtx_error_raise(
		 error,
		 PyExc_MemoryError,
		 "%s: unable to initialize file IO handle.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	Py_BEGIN_ALLOW_THREADS

	result = libevtx_file_open_file_io_handle(
	          pyevtx_file->file,
	          pyevtx_file->file_io_handle,
	          LIBEVTX_OPEN_READ,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyevtx_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to open file.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	Py_IncRef(
	 Py_None );

	return( Py_None );

on_error:
	if( pyevtx_file->file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &( pyevtx_file->file_io_handle ),
		 NULL );
	}
	return( NULL );
}

/* Closes a file
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyevtx_file_close(
           pyevtx_file_t *pyevtx_file,
           PyObject *arguments PYEVTX_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	static char *function    = "pyevtx_file_close";
	int result               = 0;

	PYEVTX_UNREFERENCED_PARAMETER( arguments )

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
		pyevtx_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to close file.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	if( pyevtx_file->file_io_handle != NULL )
	{
		Py_BEGIN_ALLOW_THREADS

		result = libbfio_handle_free(
		          &( pyevtx_file->file_io_handle ),
		          &error );

		Py_END_ALLOW_THREADS

		if( result != 1 )
		{
			pyevtx_error_raise(
			 error,
			 PyExc_IOError,
			 "%s: unable to free libbfio file IO handle.",
			 function );

			libcerror_error_free(
			 &error );

			return( NULL );
		}
	}
	Py_IncRef(
	 Py_None );

	return( Py_None );
}

/* Determines if the file is corrupted
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyevtx_file_is_corrupted(
           pyevtx_file_t *pyevtx_file,
           PyObject *arguments PYEVTX_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	static char *function    = "pyevtx_file_is_corrupted";
	int result               = 0;

	PYEVTX_UNREFERENCED_PARAMETER( arguments )

	if( pyevtx_file == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libevtx_file_is_corrupted(
	          pyevtx_file->file,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pyevtx_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to determine if file is corrupted.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	if( result != 0 )
	{
		Py_IncRef(
		 (PyObject *) Py_True );

		return( Py_True );
	}
	Py_IncRef(
	 (PyObject *) Py_False );

	return( Py_False );
}

/* Retrieves the codepage used for ASCII strings in the file
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyevtx_file_get_ascii_codepage(
           pyevtx_file_t *pyevtx_file,
           PyObject *arguments PYEVTX_ATTRIBUTE_UNUSED )
{
	PyObject *string_object     = NULL;
	libcerror_error_t *error    = NULL;
	const char *codepage_string = NULL;
	static char *function       = "pyevtx_file_get_ascii_codepage";
	int ascii_codepage          = 0;
	int result                  = 0;

	PYEVTX_UNREFERENCED_PARAMETER( arguments )

	if( pyevtx_file == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libevtx_file_get_ascii_codepage(
	          pyevtx_file->file,
	          &ascii_codepage,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyevtx_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve ASCII codepage.",
		 function );

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
#if PY_MAJOR_VERSION >= 3
	string_object = PyBytes_FromString(
	                 codepage_string );
#else
	string_object = PyString_FromString(
	                 codepage_string );
#endif
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
 * Returns 1 if successful or -1 on error
 */
int pyevtx_file_set_ascii_codepage_from_string(
     pyevtx_file_t *pyevtx_file,
     const char *codepage_string )
{
	libcerror_error_t *error      = NULL;
	static char *function         = "pyevtx_file_set_ascii_codepage_from_string";
	size_t codepage_string_length = 0;
	uint32_t feature_flags        = 0;
	int ascii_codepage            = 0;
	int result                    = 0;

	if( pyevtx_file == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	if( codepage_string == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid codepage string.",
		 function );

		return( -1 );
	}
	codepage_string_length = narrow_string_length(
	                          codepage_string );

	feature_flags = LIBCLOCALE_CODEPAGE_FEATURE_FLAG_HAVE_WINDOWS;

	if( libclocale_codepage_copy_from_string(
	     &ascii_codepage,
	     codepage_string,
	     codepage_string_length,
	     feature_flags,
	     &error ) != 1 )
	{
		pyevtx_error_raise(
		 error,
		 PyExc_RuntimeError,
		 "%s: unable to determine ASCII codepage.",
		 function );

		libcerror_error_free(
		 &error );

		return( -1 );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libevtx_file_set_ascii_codepage(
	          pyevtx_file->file,
	          ascii_codepage,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyevtx_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to set ASCII codepage.",
		 function );

		libcerror_error_free(
		 &error );

		return( -1 );
	}
	return( 1 );
}

/* Sets the codepage used for ASCII strings in the file
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyevtx_file_set_ascii_codepage(
           pyevtx_file_t *pyevtx_file,
           PyObject *arguments,
           PyObject *keywords )
{
	char *codepage_string       = NULL;
	static char *keyword_list[] = { "codepage", NULL };
	int result                  = 0;

	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "s",
	     keyword_list,
	     &codepage_string ) == 0 )
	{
		return( NULL );
	}
	result = pyevtx_file_set_ascii_codepage_from_string(
	          pyevtx_file,
	          codepage_string );

	if( result != 1 )
	{
		return( NULL );
	}
	Py_IncRef(
	 Py_None );

	return( Py_None );
}

/* Sets the codepage used for ASCII strings in the file
 * Returns a Python object if successful or NULL on error
 */
int pyevtx_file_set_ascii_codepage_setter(
     pyevtx_file_t *pyevtx_file,
     PyObject *string_object,
     void *closure PYEVTX_ATTRIBUTE_UNUSED )
{
	PyObject *utf8_string_object = NULL;
	char *codepage_string        = NULL;
	static char *function        = "pyevtx_file_set_ascii_codepage_setter";
	int result                   = 0;

	PYEVTX_UNREFERENCED_PARAMETER( closure )

	PyErr_Clear();

	result = PyObject_IsInstance(
	          string_object,
	          (PyObject *) &PyUnicode_Type );

	if( result == -1 )
	{
		pyevtx_error_fetch_and_raise(
		 PyExc_RuntimeError,
		 "%s: unable to determine if string object is of type unicode.",
		 function );

		return( -1 );
	}
	else if( result != 0 )
	{
		/* The codepage string should only contain ASCII characters.
		 */
		utf8_string_object = PyUnicode_AsUTF8String(
		                      string_object );

		if( utf8_string_object == NULL )
		{
			pyevtx_error_fetch_and_raise(
			 PyExc_RuntimeError,
			 "%s: unable to convert unicode string to UTF-8.",
			 function );

			return( -1 );
		}
#if PY_MAJOR_VERSION >= 3
		codepage_string = PyBytes_AsString(
		                   utf8_string_object );
#else
		codepage_string = PyString_AsString(
		                   utf8_string_object );
#endif
		if( codepage_string == NULL )
		{
			return( -1 );
		}
		result = pyevtx_file_set_ascii_codepage_from_string(
		          pyevtx_file,
		          codepage_string );

		if( result != 1 )
		{
			return( -1 );
		}
		return( 0 );
	}
	PyErr_Clear();

#if PY_MAJOR_VERSION >= 3
	result = PyObject_IsInstance(
	          string_object,
	          (PyObject *) &PyBytes_Type );
#else
	result = PyObject_IsInstance(
	          string_object,
	          (PyObject *) &PyString_Type );
#endif
	if( result == -1 )
	{
		pyevtx_error_fetch_and_raise(
		 PyExc_RuntimeError,
		 "%s: unable to determine if string object is of type string.",
		 function );

		return( -1 );
	}
	else if( result != 0 )
	{
#if PY_MAJOR_VERSION >= 3
		codepage_string = PyBytes_AsString(
		                   string_object );
#else
		codepage_string = PyString_AsString(
		                   string_object );
#endif
		if( codepage_string == NULL )
		{
			return( -1 );
		}
		result = pyevtx_file_set_ascii_codepage_from_string(
		          pyevtx_file,
		          codepage_string );

		if( result != 1 )
		{
			return( -1 );
		}
		return( 0 );
	}
	PyErr_Format(
	 PyExc_TypeError,
	 "%s: unsupported string object type.",
	 function );

	return( -1 );
}

/* Retrieves the format version
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyevtx_file_get_format_version(
           pyevtx_file_t *pyevtx_file,
           PyObject *arguments PYEVTX_ATTRIBUTE_UNUSED )
{
	char utf8_string[ 4 ];

	PyObject *string_object  = NULL;
	libcerror_error_t *error = NULL;
	const char *errors       = NULL;
	static char *function    = "pyevtx_file_get_format_version";
	uint16_t major_version   = 0;
	uint16_t minor_version   = 0;
	int result               = 0;

	PYEVTX_UNREFERENCED_PARAMETER( arguments )

	if( pyevtx_file == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libevtx_file_get_format_version(
	          pyevtx_file->file,
	          &major_version,
	          &minor_version,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyevtx_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve format version.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	if( major_version > 9 )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: major version out of bounds.",
		 function );

		return( NULL );
	}
	if( minor_version > 9 )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: minor version out of bounds.",
		 function );

		return( NULL );
	}
	utf8_string[ 0 ] = '0' + (char) major_version;
	utf8_string[ 1 ] = '.';
	utf8_string[ 2 ] = '0' + (char) minor_version;
	utf8_string[ 3 ] = 0;

	/* Pass the string length to PyUnicode_DecodeUTF8 otherwise it makes
	 * the end of string character is part of the string
	 */
	string_object = PyUnicode_DecodeUTF8(
	                 utf8_string,
	                 (Py_ssize_t) 3,
	                 errors );

	if( string_object == NULL )
	{
		PyErr_Format(
		 PyExc_IOError,
		 "%s: unable to convert UTF-8 string into Unicode object.",
		 function );

		return( NULL );
	}
	return( string_object );
}

/* Retrieves the number of records
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyevtx_file_get_number_of_records(
           pyevtx_file_t *pyevtx_file,
           PyObject *arguments PYEVTX_ATTRIBUTE_UNUSED )
{
	PyObject *integer_object = NULL;
	libcerror_error_t *error = NULL;
	static char *function    = "pyevtx_file_get_number_of_records";
	int number_of_records    = 0;
	int result               = 0;

	PYEVTX_UNREFERENCED_PARAMETER( arguments )

	if( pyevtx_file == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
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
		pyevtx_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve number of records.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
#if PY_MAJOR_VERSION >= 3
	integer_object = PyLong_FromLong(
	                  (long) number_of_records );
#else
	integer_object = PyInt_FromLong(
	                  (long) number_of_records );
#endif
	return( integer_object );
}

/* Retrieves the record type object
 * Returns a Python type object if successful or NULL on error
 */
PyTypeObject *pyevtx_file_get_record_type_object(
               libevtx_record_t *record PYEVTX_ATTRIBUTE_UNUSED )
{
	PYEVTX_UNREFERENCED_PARAMETER( record )

	return( &pyevtx_record_type_object );
}

/* Retrieves a specific record by index
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyevtx_file_get_record_by_index(
           PyObject *pyevtx_file,
           int record_index )
{
	PyObject *record_object   = NULL;
	PyTypeObject *type_object = NULL;
	libcerror_error_t *error  = NULL;
	libevtx_record_t *record  = NULL;
	static char *function     = "pyevtx_file_get_record_by_index";
	int result                = 0;

	if( pyevtx_file == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libevtx_file_get_record(
	          ( (pyevtx_file_t *) pyevtx_file )->file,
	          record_index,
	          &record,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyevtx_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve record: %d.",
		 function,
		 record_index );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	type_object = pyevtx_file_get_record_type_object(
	               record );

	if( type_object == NULL )
	{
		PyErr_Format(
		 PyExc_IOError,
		 "%s: unable to retrieve record type object.",
		 function );

		goto on_error;
	}
	record_object = pyevtx_record_new(
	                 type_object,
	                 record,
	                 (PyObject *) pyevtx_file );

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
	                 (PyObject *) pyevtx_file,
	                 record_index );

	return( record_object );
}

/* Retrieves a sequence and iterator object for the records
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyevtx_file_get_records(
           pyevtx_file_t *pyevtx_file,
           PyObject *arguments PYEVTX_ATTRIBUTE_UNUSED )
{
	PyObject *sequence_object = NULL;
	libcerror_error_t *error  = NULL;
	static char *function     = "pyevtx_file_get_records";
	int number_of_records     = 0;
	int result                = 0;

	PYEVTX_UNREFERENCED_PARAMETER( arguments )

	if( pyevtx_file == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
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
		pyevtx_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve number of records.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	sequence_object = pyevtx_records_new(
	                   (PyObject *) pyevtx_file,
	                   &pyevtx_file_get_record_by_index,
	                   number_of_records );

	if( sequence_object == NULL )
	{
		pyevtx_error_raise(
		 error,
		 PyExc_MemoryError,
		 "%s: unable to create sequence object.",
		 function );

		return( NULL );
	}
	return( sequence_object );
}

/* Retrieves the number of recovered records
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyevtx_file_get_number_of_recovered_records(
           pyevtx_file_t *pyevtx_file,
           PyObject *arguments PYEVTX_ATTRIBUTE_UNUSED )
{
	PyObject *integer_object = NULL;
	libcerror_error_t *error = NULL;
	static char *function    = "pyevtx_file_get_number_of_recovered_records";
	int number_of_records    = 0;
	int result               = 0;

	PYEVTX_UNREFERENCED_PARAMETER( arguments )

	if( pyevtx_file == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
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
		pyevtx_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve number of recovered records.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
#if PY_MAJOR_VERSION >= 3
	integer_object = PyLong_FromLong(
	                  (long) number_of_records );
#else
	integer_object = PyInt_FromLong(
	                  (long) number_of_records );
#endif
	return( integer_object );
}

/* Retrieves a specific recovered record by index
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyevtx_file_get_recovered_record_by_index(
           PyObject *pyevtx_file,
           int record_index )
{
	PyObject *record_object   = NULL;
	PyTypeObject *type_object = NULL;
	libcerror_error_t *error  = NULL;
	libevtx_record_t *record  = NULL;
	static char *function     = "pyevtx_file_get_recovered_record_by_index";
	int result                = 0;

	if( pyevtx_file == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libevtx_file_get_recovered_record(
	          ( (pyevtx_file_t *) pyevtx_file )->file,
	          record_index,
	          &record,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyevtx_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve recovered record: %d.",
		 function,
		 record_index );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	type_object = pyevtx_file_get_record_type_object(
	               record );

	if( type_object == NULL )
	{
		PyErr_Format(
		 PyExc_IOError,
		 "%s: unable to retrieve record type object.",
		 function );

		goto on_error;
	}
	record_object = pyevtx_record_new(
	                 type_object,
	                 record,
	                 (PyObject *) pyevtx_file );

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
	                 (PyObject *) pyevtx_file,
	                 record_index );

	return( record_object );
}

/* Retrieves a sequence and iterator object for the recovered records
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyevtx_file_get_recovered_records(
           pyevtx_file_t *pyevtx_file,
           PyObject *arguments PYEVTX_ATTRIBUTE_UNUSED )
{
	PyObject *sequence_object = NULL;
	libcerror_error_t *error  = NULL;
	static char *function     = "pyevtx_file_get_recovered_records";
	int number_of_records     = 0;
	int result                = 0;

	PYEVTX_UNREFERENCED_PARAMETER( arguments )

	if( pyevtx_file == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
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
		pyevtx_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve number of recovered records.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	sequence_object = pyevtx_records_new(
	                   (PyObject *) pyevtx_file,
	                   &pyevtx_file_get_recovered_record_by_index,
	                   number_of_records );

	if( sequence_object == NULL )
	{
		pyevtx_error_raise(
		 error,
		 PyExc_MemoryError,
		 "%s: unable to create sequence object.",
		 function );

		return( NULL );
	}
	return( sequence_object );
}

