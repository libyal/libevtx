/*
 * Legacy functions
 *
 * Copyright (C) 2011-2020, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#if !defined( _LIBEVTX_LEGACY_H )
#define _LIBEVTX_LEGACY_H

#include <common.h>
#include <types.h>

#include "libevtx_extern.h"
#include "libevtx_libcerror.h"
#include "libevtx_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

#if !defined( HAVE_LOCAL_LIBEVTX )

LIBEVTX_EXTERN \
int libevtx_record_parse_data(
     libevtx_record_t *record,
     libevtx_template_definition_t *template_definition,
     libcerror_error_t **error );

#endif

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBEVTX_LEGACY_H ) */

