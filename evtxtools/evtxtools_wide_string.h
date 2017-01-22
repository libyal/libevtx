/*
 * Wide character string functions
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

#if !defined( _EVTXTOOLS_WIDE_STRING_H )
#define _EVTXTOOLS_WIDE_STRING_H

#include <common.h>
#include <wide_string.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

#if !defined( wide_string_compare_no_case )

#define wide_string_compare_no_case( string1, string2, size ) \
	evtxtools_wide_string_compare_no_case( string1, string2, size )

#endif /* !defined( wide_string_compare_no_case ) */

#if defined( TOWLOWER ) && !defined( HAVE_WCSNCASECMP ) && !defined( HAVE_WCSCASECMP ) && !defined( WINAPI )

int evtxtools_wide_string_compare_no_case(
     const wchar_t *string1,
     const wchar_t *string2,
     size_t comparision_length );

#endif /* defined( TOWLOWER ) && !defined( HAVE_WCSNCASECMP ) && !defined( HAVE_WCSCASECMP ) && !defined( WINAPI ) */

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _EVTXTOOLS_WIDE_STRING_H ) */

