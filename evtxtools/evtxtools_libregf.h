/*
 * The libregf header wrapper
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

#if !defined( _EVTXTOOLS_LIBREGF_H )
#define _EVTXTOOLS_LIBREGF_H

#include <common.h>

/* Define HAVE_LOCAL_LIBREGF for local use of libregf
 */
#if defined( HAVE_LOCAL_LIBREGF )

#include <libregf_codepage.h>
#include <libregf_definitions.h>
#include <libregf_file.h>
#include <libregf_key.h>
#include <libregf_value.h>
#include <libregf_types.h>

#else

/* If libtool DLL support is enabled set LIBREGF_DLL_IMPORT
 * before including libregf.h
 */
#if defined( _WIN32 ) && defined( DLL_IMPORT )
#define LIBREGF_DLL_IMPORT
#endif

#include <libregf.h>

#endif

#endif

