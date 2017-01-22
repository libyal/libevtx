/*
 * Split system string functions
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

#if !defined( _EVTXTOOLS_SYSTEM_SPLIT_STRING_H )
#define _EVTXTOOLS_SYSTEM_SPLIT_STRING_H

#include <common.h>
#include <types.h>

#include "evtxtools_libcsplit.h"

#if defined( __cplusplus )
extern "C" {
#endif

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
#define system_split_string_t \
	libcsplit_wide_split_string_t

#define system_split_string_free( split_string, error ) \
	libcsplit_wide_split_string_free( split_string, error )

#define system_split_string_get_number_of_segments( split_string, number_of_segments, error ) \
	libcsplit_wide_split_string_get_number_of_segments( split_string, number_of_segments, error )

#define system_split_string_get_segment_by_index( split_string, segment_index, string_segment, string_segment_size, error ) \
	libcsplit_wide_split_string_get_segment_by_index( split_string, segment_index, string_segment, string_segment_size, error )

#define system_string_split( string, string_size, delimiter, split_string, error ) \
	libcsplit_wide_string_split( string, string_size, (wchar_t) delimiter, split_string, error )

#else
#define system_split_string_t \
	libcsplit_narrow_split_string_t

#define system_split_string_free( split_string, error ) \
	libcsplit_narrow_split_string_free( split_string, error )

#define system_split_string_get_number_of_segments( split_string, number_of_segments, error ) \
	libcsplit_narrow_split_string_get_number_of_segments( split_string, number_of_segments, error )

#define system_split_string_get_segment_by_index( split_string, segment_index, string_segment, string_segment_size, error ) \
	libcsplit_narrow_split_string_get_segment_by_index( split_string, segment_index, string_segment, string_segment_size, error )

#define system_string_split( string, string_size, delimiter, split_string, error ) \
	libcsplit_narrow_string_split( string, string_size, (char) delimiter, split_string, error )

#endif /* defined( HAVE_WIDE_SYSTEM_CHARACTER ) */

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _EVTXTOOLS_SYSTEM_SPLIT_STRING_H ) */

