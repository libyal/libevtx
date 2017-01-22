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

#include <common.h>
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( HAVE_WINAPI )
#include <stdlib.h>
#endif

#include "pyevtx_libcerror.h"
#include "pyevtx_libevtx.h"
#include "pyevtx_python.h"
#include "pyevtx_strings.h"

PySequenceMethods pyevtx_strings_sequence_methods = {
	/* sq_length */
	(lenfunc) pyevtx_strings_len,
	/* sq_concat */
	0,
	/* sq_repeat */
	0,
	/* sq_item */
	(ssizeargfunc) pyevtx_strings_getitem,
	/* sq_slice */
	0,
	/* sq_ass_item */
	0,
	/* sq_ass_slice */
	0,
	/* sq_contains */
	0,
	/* sq_inplace_concat */
	0,
	/* sq_inplace_repeat */
	0
};

PyTypeObject pyevtx_strings_type_object = {
	PyVarObject_HEAD_INIT( NULL, 0 )

	/* tp_name */
	"pyevtx._strings",
	/* tp_basicsize */
	sizeof( pyevtx_strings_t ),
	/* tp_itemsize */
	0,
	/* tp_dealloc */
	(destructor) pyevtx_strings_free,
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
	&pyevtx_strings_sequence_methods,
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
	"pyevtx internal sequence and iterator object of strings",
	/* tp_traverse */
	0,
	/* tp_clear */
	0,
	/* tp_richcompare */
	0,
	/* tp_weaklistoffset */
	0,
	/* tp_iter */
	(getiterfunc) pyevtx_strings_iter,
	/* tp_iternext */
	(iternextfunc) pyevtx_strings_iternext,
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
	(initproc) pyevtx_strings_init,
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

/* Creates a new strings object
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyevtx_strings_new(
           PyObject *parent_object,
           PyObject* (*get_item_by_index)(
                        PyObject *parent_object,
                        int index ),
           int number_of_items )
{
	pyevtx_strings_t *strings_object = NULL;
	static char *function            = "pyevtx_strings_new";

	if( parent_object == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid parent object.",
		 function );

		return( NULL );
	}
	if( get_item_by_index == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid get item by index function.",
		 function );

		return( NULL );
	}
	/* Make sure the strings values are initialized
	 */
	strings_object = PyObject_New(
	                  struct pyevtx_strings,
	                  &pyevtx_strings_type_object );

	if( strings_object == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create strings object.",
		 function );

		goto on_error;
	}
	if( pyevtx_strings_init(
	     strings_object ) != 0 )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to initialize strings object.",
		 function );

		goto on_error;
	}
	strings_object->parent_object     = parent_object;
	strings_object->get_item_by_index = get_item_by_index;
	strings_object->number_of_items   = number_of_items;

	Py_IncRef(
	 (PyObject *) strings_object->parent_object );

	return( (PyObject *) strings_object );

on_error:
	if( strings_object != NULL )
	{
		Py_DecRef(
		 (PyObject *) strings_object );
	}
	return( NULL );
}

/* Intializes a strings object
 * Returns 0 if successful or -1 on error
 */
int pyevtx_strings_init(
     pyevtx_strings_t *strings_object )
{
	static char *function = "pyevtx_strings_init";

	if( strings_object == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid strings object.",
		 function );

		return( -1 );
	}
	/* Make sure the strings values are initialized
	 */
	strings_object->parent_object     = NULL;
	strings_object->get_item_by_index = NULL;
	strings_object->current_index     = 0;
	strings_object->number_of_items   = 0;

	return( 0 );
}

/* Frees a strings object
 */
void pyevtx_strings_free(
      pyevtx_strings_t *strings_object )
{
	struct _typeobject *ob_type = NULL;
	static char *function       = "pyevtx_strings_free";

	if( strings_object == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid strings object.",
		 function );

		return;
	}
	ob_type = Py_TYPE(
	           strings_object );

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
	if( strings_object->parent_object != NULL )
	{
		Py_DecRef(
		 (PyObject *) strings_object->parent_object );
	}
	ob_type->tp_free(
	 (PyObject*) strings_object );
}

/* The strings len() function
 */
Py_ssize_t pyevtx_strings_len(
            pyevtx_strings_t *strings_object )
{
	static char *function = "pyevtx_strings_len";

	if( strings_object == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid strings object.",
		 function );

		return( -1 );
	}
	return( (Py_ssize_t) strings_object->number_of_items );
}

/* The strings getitem() function
 */
PyObject *pyevtx_strings_getitem(
           pyevtx_strings_t *strings_object,
           Py_ssize_t item_index )
{
	PyObject *string_object = NULL;
	static char *function   = "pyevtx_strings_getitem";

	if( strings_object == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid strings object.",
		 function );

		return( NULL );
	}
	if( strings_object->get_item_by_index == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid strings object - missing get item by index function.",
		 function );

		return( NULL );
	}
	if( strings_object->number_of_items < 0 )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid strings object - invalid number of items.",
		 function );

		return( NULL );
	}
	if( ( item_index < 0 )
	 || ( item_index >= (Py_ssize_t) strings_object->number_of_items ) )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid invalid item index value out of bounds.",
		 function );

		return( NULL );
	}
	string_object = strings_object->get_item_by_index(
	                 strings_object->parent_object,
	                 (int) item_index );

	return( string_object );
}

/* The strings iter() function
 */
PyObject *pyevtx_strings_iter(
           pyevtx_strings_t *strings_object )
{
	static char *function = "pyevtx_strings_iter";

	if( strings_object == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid strings object.",
		 function );

		return( NULL );
	}
	Py_IncRef(
	 (PyObject *) strings_object );

	return( (PyObject *) strings_object );
}

/* The strings iternext() function
 */
PyObject *pyevtx_strings_iternext(
           pyevtx_strings_t *strings_object )
{
	PyObject *string_object = NULL;
	static char *function   = "pyevtx_strings_iternext";

	if( strings_object == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid strings object.",
		 function );

		return( NULL );
	}
	if( strings_object->get_item_by_index == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid strings object - missing get item by index function.",
		 function );

		return( NULL );
	}
	if( strings_object->current_index < 0 )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid strings object - invalid current index.",
		 function );

		return( NULL );
	}
	if( strings_object->number_of_items < 0 )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid strings object - invalid number of items.",
		 function );

		return( NULL );
	}
	if( strings_object->current_index >= strings_object->number_of_items )
	{
		PyErr_SetNone(
		 PyExc_StopIteration );

		return( NULL );
	}
	string_object = strings_object->get_item_by_index(
	                 strings_object->parent_object,
	                 strings_object->current_index );

	if( string_object != NULL )
	{
		strings_object->current_index++;
	}
	return( string_object );
}

