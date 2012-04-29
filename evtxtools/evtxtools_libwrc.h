/*
 * The libwrc header wrapper
 *
 * Copyright (c) 2011-2012, Joachim Metz <jbmetz@users.sourceforge.net>
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

#if !defined( _EVTXTOOLS_LIBWRC_H )
#define _EVTXTOOLS_LIBWRC_H

#include <common.h>

/* Define HAVE_LOCAL_LIBWRC for local use of libwrc
 */
#if defined( HAVE_LOCAL_LIBWRC )

#include <libwrc_definitions.h>
#include <libwrc_manifest.h>
#include <libwrc_message_table.h>
#include <libwrc_resource.h>
#include <libwrc_stream.h>
#include <libwrc_string.h>
#include <libwrc_version.h>
#include <libwrc_types.h>

#elif defined( HAVE_LIBWRC_H )

/* If libtool DLL support is enabled set LIBWRC_DLL_IMPORT
 * before including libwrc.h
 */
#if defined( _WIN32 ) && defined( DLL_IMPORT )
#define LIBWRC_DLL_IMPORT
#endif

#include <libwrc.h>

#else
#error Missing libwrc.h
#endif

#endif

