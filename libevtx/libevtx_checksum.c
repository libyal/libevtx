/*
 * Checksum functions
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

#include "libevtx_libcerror.h"

/* Table of CRC-32 values of 8-bit values
 */
uint32_t libevtx_checksum_crc32_table[ 256 ];

/* Value to indicate the CRC-32 table been computed
 */
int libevtx_checksum_crc32_table_computed = 0;

/* Initializes the internal CRC-32 table
 * The table speeds up the CRC-32 calculation
 */
void libevtx_checksum_initialize_crc32_table(
      void )
{
	uint32_t crc32             = 0;
	uint32_t crc32_table_index = 0;
	uint8_t bit_iterator       = 0;

	for( crc32_table_index = 0;
	     crc32_table_index < 256;
	     crc32_table_index++ )
	{
		crc32 = (uint32_t) crc32_table_index;

		for( bit_iterator = 0;
		     bit_iterator < 8;
		     bit_iterator++ )
		{
			if( crc32 & 1 )
			{
				crc32 = (uint32_t) 0xedb88320UL ^ ( crc32 >> 1 );
			}
			else
			{
				crc32 = crc32 >> 1;
			}
		}
		libevtx_checksum_crc32_table[ crc32_table_index ] = crc32;
	}
	libevtx_checksum_crc32_table_computed = 1;
}

/* Calculates the CRC-32 of a buffer
 * Based on RFC 1952
 * Returns 1 if successful or -1 on error
 */
int libevtx_checksum_calculate_little_endian_crc32(
     uint32_t *crc32,
     uint8_t *buffer,
     size_t size,
     uint32_t initial_value,
     libcerror_error_t **error )
{
	static char *function      = "libevtx_checksum_calculate_little_endian_crc32";
	size_t buffer_offset       = 0;
	uint32_t crc32_table_index = 0;

	if( crc32 == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid CRC-32.",
		 function );

		return( -1 );
	}
	if( buffer == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid buffer.",
		 function );

		return( -1 );
	}
	if( size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid size value exceeds maximum.",
		 function );

		return( -1 );
	}
	*crc32 = initial_value ^ (uint32_t) 0xffffffffUL;

        if( libevtx_checksum_crc32_table_computed == 0 )
	{
		libevtx_checksum_initialize_crc32_table();
	}
        for( buffer_offset = 0;
	     buffer_offset < size;
	     buffer_offset++ )
	{
		crc32_table_index = ( *crc32 ^ buffer[ buffer_offset ] ) & 0x000000ffUL;

		*crc32 = libevtx_checksum_crc32_table[ crc32_table_index ] ^ ( *crc32 >> 8 );
        }
        *crc32 ^= 0xffffffffUL;

	return( 1 );
}

/* Calculates the weak CRC-32 of a buffer
 * Based on RFC 1952, without initial and final XOR operation
 * Returns 1 if successful or -1 on error
 */
int libevtx_checksum_calculate_little_endian_weak_crc32(
     uint32_t *crc32,
     uint8_t *buffer,
     size_t size,
     uint32_t initial_value,
     libcerror_error_t **error )
{
	static char *function      = "libevtx_checksum_calculate_little_endian_weak_crc32";
	size_t buffer_offset       = 0;
	uint32_t crc32_table_index = 0;

	if( crc32 == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid CRC-32.",
		 function );

		return( -1 );
	}
	if( buffer == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid buffer.",
		 function );

		return( -1 );
	}
	if( size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid size value exceeds maximum.",
		 function );

		return( -1 );
	}
	*crc32 = initial_value;

        if( libevtx_checksum_crc32_table_computed == 0 )
	{
		libevtx_checksum_initialize_crc32_table();
	}
        for( buffer_offset = 0;
	     buffer_offset < size;
	     buffer_offset++ )
	{
		crc32_table_index = ( *crc32 ^ buffer[ buffer_offset ] ) & 0x000000ffUL;

		*crc32 = libevtx_checksum_crc32_table[ crc32_table_index ] ^ ( *crc32 >> 8 );
        }
        return( 1 );
}

