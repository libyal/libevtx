/*
 * Integer functions
 *
 * Copyright (c) 2011-2013, Joachim Metz <joachim.metz@gmail.com>
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

#include "pyevtx.h"
#include "pyevtx_integer.h"
#include "pyevtx_python.h"

/* Creates a new signed integer object from a 64-bit value
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyevtx_integer_signed_new_from_64bit(
           int64_t value_64bit )
{
	PyObject *integer_object = NULL;
	static char *function    = "pyevtx_integer_signed_new_from_64bit";

#if defined( HAVE_LONG_LONG )
	if( ( value_64bit < (int64_t) LLONG_MIN )
	 || ( value_64bit > (int64_t) LLONG_MAX ) )
	{
		PyErr_Format(
		 PyExc_OverflowError,
		 "%s: 64-bit value out of bounds.",
		 function );

		return( NULL );
	}
	integer_object = PyLong_FromLongLong(
	                  (long long) value_64bit );
#else
	if( current_offset > (off64_t) LONG_MAX )
	{
		PyErr_Format(
		 PyExc_OverflowError,
		 "%s: 64-bit value out of bounds.",
		 function );

		return( NULL );
	}
	integer_object = PyLong_FromLong(
	                  (long) value_64bit );
#endif
	return( integer_object );
}

/* Creates a new unsigned integer object from a 64-bit value
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyevtx_integer_unsigned_new_from_64bit(
           uint64_t value_64bit )
{
	PyObject *integer_object = NULL;
	static char *function    = "pyevtx_integer_unsigned_new_from_64bit";

#if defined( HAVE_LONG_LONG )
	if( value_64bit > (uint64_t) ULLONG_MAX )
	{
		PyErr_Format(
		 PyExc_OverflowError,
		 "%s: 64-bit value exceeds maximum.",
		 function );

		return( NULL );
	}
	integer_object = PyLong_FromUnsignedLongLong(
	                  (long long) value_64bit );
#else
	if( value_64bit > (uint64_t) ULONG_MAX )
	{
		PyErr_Format(
		 PyExc_OverflowError,
		 "%s: 64-bit value exceeds maximum.",
		 function );

		return( NULL );
	}
	integer_object = PyLong_FromUnsingedLong(
	                  (long) value_64bit );
#endif
	return( integer_object );
}

