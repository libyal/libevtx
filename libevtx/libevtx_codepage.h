/*
 * Codepage functions
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

#if !defined( _LIBEVTX_INTERNAL_CODEPAGE_H )
#define _LIBEVTX_INTERNAL_CODEPAGE_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

#if !defined( HAVE_LOCAL_LIBEVTX )

#include <libevtx/codepage.h>

/* Define HAVE_LOCAL_LIBEVTX for local use of libevtx
 * The definitions in <libevtx/codepage.h> are copied here
 * for local use of libevtx
 */
#else

/* The codepage definitions
 */
enum LIBEVTX_CODEPAGES
{
	LIBEVTX_CODEPAGE_ASCII		= 20127,

	LIBEVTX_CODEPAGE_ISO_8859_1	= 28591,
	LIBEVTX_CODEPAGE_ISO_8859_2	= 28592,
	LIBEVTX_CODEPAGE_ISO_8859_3	= 28593,
	LIBEVTX_CODEPAGE_ISO_8859_4	= 28594,
	LIBEVTX_CODEPAGE_ISO_8859_5	= 28595,
	LIBEVTX_CODEPAGE_ISO_8859_6	= 28596,
	LIBEVTX_CODEPAGE_ISO_8859_7	= 28597,
	LIBEVTX_CODEPAGE_ISO_8859_8	= 28598,
	LIBEVTX_CODEPAGE_ISO_8859_9	= 28599,
	LIBEVTX_CODEPAGE_ISO_8859_10	= 28600,
	LIBEVTX_CODEPAGE_ISO_8859_11	= 28601,
	LIBEVTX_CODEPAGE_ISO_8859_13	= 28603,
	LIBEVTX_CODEPAGE_ISO_8859_14	= 28604,
	LIBEVTX_CODEPAGE_ISO_8859_15	= 28605,
	LIBEVTX_CODEPAGE_ISO_8859_16	= 28606,

	LIBEVTX_CODEPAGE_KOI8_R		= 20866,
	LIBEVTX_CODEPAGE_KOI8_U		= 21866,

	LIBEVTX_CODEPAGE_WINDOWS_874	= 874,
	LIBEVTX_CODEPAGE_WINDOWS_932	= 932,
	LIBEVTX_CODEPAGE_WINDOWS_936	= 936,
	LIBEVTX_CODEPAGE_WINDOWS_949	= 949,
	LIBEVTX_CODEPAGE_WINDOWS_950	= 950,
	LIBEVTX_CODEPAGE_WINDOWS_1250	= 1250,
	LIBEVTX_CODEPAGE_WINDOWS_1251	= 1251,
	LIBEVTX_CODEPAGE_WINDOWS_1252	= 1252,
	LIBEVTX_CODEPAGE_WINDOWS_1253	= 1253,
	LIBEVTX_CODEPAGE_WINDOWS_1254	= 1254,
	LIBEVTX_CODEPAGE_WINDOWS_1255	= 1255,
	LIBEVTX_CODEPAGE_WINDOWS_1256	= 1256,
	LIBEVTX_CODEPAGE_WINDOWS_1257	= 1257,
	LIBEVTX_CODEPAGE_WINDOWS_1258	= 1258
};

#endif /* !defined( HAVE_LOCAL_LIBEVTX ) */

#if defined( HAVE_DEBUG_OUTPUT )

typedef struct libevtx_codepage libevtx_codepage_t;

struct libevtx_codepage
{
	/* The codepage
	 */
	uint32_t codepage;

	/* The identifier
	 */
	const char *identifier;

	/* The description
	 */
	const char *description;
};
const char *libevtx_codepage_get_identifier(
             uint32_t codepage );

const char *libevtx_codepage_get_description(
             uint32_t codepage );

#endif /* defined( HAVE_DEBUG_OUTPUT ) */

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBEVTX_INTERNAL_CODEPAGE_H ) */

