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

#include <common.h>
#include <types.h>

#if defined( HAVE_WCTYPE_H )
#include <wctype.h>
#endif

#include "evtxtools_wide_string.h"

#if defined( TOWLOWER ) && !defined( HAVE_WCSNCASECMP ) && !defined( HAVE_WCSCASECMP ) && !defined( WINAPI )

/* Replacement for missing: wcsncasecmp
 * Compares no more than a specified number of wide characters of string1 and string2,
 * ignoring case, returning less than, equal to or greater than zero if string1 is
 * less than, equal to or greater than string.
 */
int evtxtools_wide_string_compare_no_case(
     const wchar_t *string1,
     const wchar_t *string2,
     size_t comparision_length )
{
	wint_t character1 = 0;
	wint_t character2 = 0;

	if( string1 == string2 )
	{
		return( 0 );
	}
	while( comparision_length > 0 )
	{
		character1 = towlower( *string1 );
		character2 = towlower( *string2 );

		if( ( character1 == 0 )
		 || ( character1 != character2 ) )
		{
			return( character1 - character2 );
		}
		string1++;
		string2++;

		comparision_length--;
	}
	return( 0 );
}

#endif /* defined( TOWLOWER ) && !defined( HAVE_WCSNCASECMP ) && !defined( HAVE_WCSCASECMP ) && !defined( WINAPI ) */

