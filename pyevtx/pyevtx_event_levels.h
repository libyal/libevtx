/*
 * Python object definition of the libevtx event levels
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

#if !defined( _PYEVTX_EVENT_LEVELS_H )
#define _PYEVTX_EVENT_LEVELS_H

#include <common.h>
#include <types.h>

#include "pyevtx_libevtx.h"
#include "pyevtx_python.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct pyevtx_event_levels pyevtx_event_levels_t;

struct pyevtx_event_levels
{
	/* Python object initialization
	 */
	PyObject_HEAD
};

extern PyTypeObject pyevtx_event_levels_type_object;

int pyevtx_event_levels_init_type(
     PyTypeObject *type_object );

PyObject *pyevtx_event_levels_new(
           void );

int pyevtx_event_levels_init(
     pyevtx_event_levels_t *definitions_object );

void pyevtx_event_levels_free(
      pyevtx_event_levels_t *definitions_object );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _PYEVTX_EVENT_LEVELS_H ) */

