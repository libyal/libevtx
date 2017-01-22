/*
 * The libfwevt header wrapper
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

#if !defined( _LIBEVTX_LIBFWEVT_H )
#define _LIBEVTX_LIBFWEVT_H

#include <common.h>

/* Define HAVE_LOCAL_LIBFWEVT for local use of libfwevt
 */
#if defined( HAVE_LOCAL_LIBFWEVT )

#include <libfwevt_channel.h>
#include <libfwevt_definitions.h>
#include <libfwevt_event.h>
#include <libfwevt_keyword.h>
#include <libfwevt_level.h>
#include <libfwevt_manifest.h>
#include <libfwevt_map.h>
#include <libfwevt_opcode.h>
#include <libfwevt_provider.h>
#include <libfwevt_task.h>
#include <libfwevt_template.h>
#include <libfwevt_types.h>
#include <libfwevt_xml_document.h>
#include <libfwevt_xml_tag.h>
#include <libfwevt_xml_template_value.h>

#else

/* If libtool DLL support is enabled set LIBFWEVT_DLL_IMPORT
 * before including libfwevt.h
 */
#if defined( _WIN32 ) && defined( DLL_IMPORT )
#define LIBFWEVT_DLL_IMPORT
#endif

#include <libfwevt.h>

#endif

#endif

