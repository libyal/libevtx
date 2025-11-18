/*
 * The libwrc header wrapper
 *
 * Copyright (C) 2011-2025, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _EVTXTOOLS_LIBWRC_H )
#define _EVTXTOOLS_LIBWRC_H

#include <common.h>

/* Define HAVE_LOCAL_LIBWRC for local use of libwrc
 */
#if defined( HAVE_LOCAL_LIBWRC )

#include <libwrc_definitions.h>
#include <libwrc_manifest_resource.h>
#include <libwrc_message_table_resource.h>
#include <libwrc_mui_resource.h>
#include <libwrc_resource.h>
#include <libwrc_resource_item.h>
#include <libwrc_stream.h>
#include <libwrc_string_table_resource.h>
#include <libwrc_version_information_resource.h>
#include <libwrc_types.h>

#else

/* If libtool DLL support is enabled set LIBWRC_DLL_IMPORT
 * before including libwrc.h
 */
#if defined( _WIN32 ) && defined( DLL_IMPORT )
#define LIBWRC_DLL_IMPORT
#endif

#include <libwrc.h>

#endif /* defined( HAVE_LOCAL_LIBWRC ) */

#endif /* !defined( _EVTXTOOLS_LIBWRC_H ) */

