/*
 * Byte stream functions
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

#include "libevtx_byte_stream.h"
#include "libevtx_libcerror.h"
#include "libevtx_types.h"

/* Checks if a byte stream is filled with 0-byte values
 * Returns 1 if true, 0 if not or -1 on error
 */
int libevtx_byte_stream_check_for_zero_byte_fill(
     const uint8_t *byte_stream,
     size_t byte_stream_size,
     libcerror_error_t **error )
{
	libevtx_aligned_t *aligned_byte_stream_index = NULL;
	uint8_t *byte_stream_index                   = NULL;
	static char *function                        = "libevtx_byte_stream_check_for_zero_byte_fill";

	if( byte_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid byte stream.",
		 function );

		return( -1 );
	}
	if( byte_stream_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid byte stream size value exceeds maximum.",
		 function );

		return( -1 );
	}
	byte_stream_index = (uint8_t *) byte_stream;

	/* Only optimize for byte stream larger than the alignment
	 */
	if( byte_stream_size > ( 2 * sizeof( libevtx_aligned_t ) ) )
	{
		/* Align the byte stream index
		 */
		while( ( (intptr_t) byte_stream_index % sizeof( libevtx_aligned_t ) ) != 0 )
		{
			if( *byte_stream_index != 0 )
			{
				return( 0 );
			}
			byte_stream_index += 1;
			byte_stream_size  -= 1;
		}
		aligned_byte_stream_index = (libevtx_aligned_t *) byte_stream_index;

		while( byte_stream_size > sizeof( libevtx_aligned_t ) )
		{
			if( *aligned_byte_stream_index != 0 )
			{
				return( 0 );
			}
			aligned_byte_stream_index += 1;
			byte_stream_size          -= sizeof( libevtx_aligned_t );
		}
		byte_stream_index = (uint8_t *) aligned_byte_stream_index;
	}
	while( byte_stream_size != 0 )
	{
		if( *byte_stream_index != 0 )
		{
			return( 0 );
		}
		byte_stream_index += 1;
		byte_stream_size  -= 1;
	}
	return( 1 );
}

