/*
 * OSS-Fuzz target for libevtx record type
 *
 * Copyright (C) 2011-2026, Joachim Metz <joachim.metz@gmail.com>
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

#include <stddef.h>
#include <stdint.h>

#include <stdlib.h>

/* Note that some of the OSS-Fuzz engines use C++
 */
extern "C" {

#include "ossfuzz_libbfio.h"
#include "ossfuzz_libevtx.h"

#if !defined( LIBEVTX_HAVE_BFIO )

/* Opens a file using a Basic File IO (bfio) handle
 * Returns 1 if successful or -1 on error
 */
LIBEVTX_EXTERN \
int libevtx_file_open_file_io_handle(
      libevtx_file_t *file,
      libbfio_handle_t *file_io_handle,
      int access_flags,
      libevtx_error_t **error );

#endif /* !defined( LIBEVTX_HAVE_BFIO ) */

/* Limits on the traversal and buffer sizes to bound the time a single input takes
 */
#define FUZZER_RECORD_LIMIT        4
#define FUZZER_STRING_LIMIT        8
#define FUZZER_STRING_SIZE_LIMIT   ( (size_t) 0x00100000 )

/* The ASCII codepages supported by libevtx_file_set_ascii_codepage
 */
static const int file_codepages[] = {
	LIBEVTX_CODEPAGE_ASCII,
	LIBEVTX_CODEPAGE_WINDOWS_874,
	LIBEVTX_CODEPAGE_WINDOWS_932,
	LIBEVTX_CODEPAGE_WINDOWS_936,
	LIBEVTX_CODEPAGE_WINDOWS_949,
	LIBEVTX_CODEPAGE_WINDOWS_950,
	LIBEVTX_CODEPAGE_WINDOWS_1250,
	LIBEVTX_CODEPAGE_WINDOWS_1251,
	LIBEVTX_CODEPAGE_WINDOWS_1252,
	LIBEVTX_CODEPAGE_WINDOWS_1253,
	LIBEVTX_CODEPAGE_WINDOWS_1254,
	LIBEVTX_CODEPAGE_WINDOWS_1255,
	LIBEVTX_CODEPAGE_WINDOWS_1256,
	LIBEVTX_CODEPAGE_WINDOWS_1257,
	LIBEVTX_CODEPAGE_WINDOWS_1258
};

static const size_t file_number_of_codepages =
	  sizeof( file_codepages ) / sizeof( file_codepages[ 0 ] );

/* Decodes the record using the typed accessors
 * Returns 1 if successful or -1 on error
 */
int record_decode_fuzzer(
      libevtx_record_t *record )
{
	uint8_t *utf8_string                   = NULL;
	uint16_t *utf16_string                 = NULL;
	uint8_t *data_buffer                   = NULL;
	off64_t offset                         = 0;
	size_t utf8_string_size                = 0;
	size_t utf16_string_size               = 0;
	size_t data_size                       = 0;
	uint64_t creation_time                 = 0;
	uint64_t identifier                    = 0;
	uint64_t written_time                  = 0;
	uint32_t event_identifier               = 0;
	uint32_t event_identifier_qualifiers    = 0;
	uint8_t event_level                    = 0;
	uint8_t event_version                  = 0;
	uint8_t value_byte                     = 0;
	int number_of_strings                  = 0;
	int string_index                       = 0;
	int result                             = 0;

	if( record == NULL )
	{
		return( -1 );
	}
	libevtx_record_get_offset(
	 record,
	 &offset,
	 NULL );

	libevtx_record_get_identifier(
	 record,
	 &identifier,
	 NULL );

	/* Note that not all record fields are present in a record
	 */
	libevtx_record_get_creation_time(
	 record,
	 &creation_time,
	 NULL );

	libevtx_record_get_written_time(
	 record,
	 &written_time,
	 NULL );

	libevtx_record_get_event_identifier(
	 record,
	 &event_identifier,
	 NULL );

	/* Note that not all record fields are present in a record
	 */
	libevtx_record_get_event_identifier_qualifiers(
	 record,
	 &event_identifier_qualifiers,
	 NULL );

	libevtx_record_get_event_version(
	 record,
	 &event_version,
	 NULL );

	libevtx_record_get_event_level(
	 record,
	 &event_level,
	 NULL );

	result = libevtx_record_get_utf8_provider_identifier_size(
	          record,
	          &utf8_string_size,
	          NULL );

	if( ( result == 1 )
	 && ( utf8_string_size > 0 )
	 && ( utf8_string_size <= FUZZER_STRING_SIZE_LIMIT ) )
	{
		utf8_string = (uint8_t *) malloc( utf8_string_size );

		if( utf8_string != NULL )
		{
			if( libevtx_record_get_utf8_provider_identifier(
			     record,
			     utf8_string,
			     utf8_string_size,
			     NULL ) == 1 )
			{
				value_byte ^= utf8_string[ 0 ];
			}
			free( utf8_string );
		}
	}
	/* The size is the number of UTF-16 characters,
	 * including the end of string character
	 */
	result = libevtx_record_get_utf16_provider_identifier_size(
	          record,
	          &utf16_string_size,
	          NULL );

	if( ( result == 1 )
	 && ( utf16_string_size > 0 )
	 && ( utf16_string_size <= FUZZER_STRING_SIZE_LIMIT ) )
	{
		utf16_string = (uint16_t *) malloc( utf16_string_size * sizeof( uint16_t ) );

		if( utf16_string != NULL )
		{
			if( libevtx_record_get_utf16_provider_identifier(
			     record,
			     utf16_string,
			     utf16_string_size,
			     NULL ) == 1 )
			{
				value_byte ^= (uint8_t) utf16_string[ 0 ];
			}
			free( utf16_string );
		}
	}
	result = libevtx_record_get_utf8_source_name_size(
	          record,
	          &utf8_string_size,
	          NULL );

	if( ( result == 1 )
	 && ( utf8_string_size > 0 )
	 && ( utf8_string_size <= FUZZER_STRING_SIZE_LIMIT ) )
	{
		utf8_string = (uint8_t *) malloc( utf8_string_size );

		if( utf8_string != NULL )
		{
			if( libevtx_record_get_utf8_source_name(
			     record,
			     utf8_string,
			     utf8_string_size,
			     NULL ) == 1 )
			{
				value_byte ^= utf8_string[ 0 ];
			}
			free( utf8_string );
		}
	}
	/* The size is the number of UTF-16 characters,
	 * including the end of string character
	 */
	result = libevtx_record_get_utf16_source_name_size(
	          record,
	          &utf16_string_size,
	          NULL );

	if( ( result == 1 )
	 && ( utf16_string_size > 0 )
	 && ( utf16_string_size <= FUZZER_STRING_SIZE_LIMIT ) )
	{
		utf16_string = (uint16_t *) malloc( utf16_string_size * sizeof( uint16_t ) );

		if( utf16_string != NULL )
		{
			if( libevtx_record_get_utf16_source_name(
			     record,
			     utf16_string,
			     utf16_string_size,
			     NULL ) == 1 )
			{
				value_byte ^= (uint8_t) utf16_string[ 0 ];
			}
			free( utf16_string );
		}
	}
	result = libevtx_record_get_utf8_channel_name_size(
	          record,
	          &utf8_string_size,
	          NULL );

	if( ( result == 1 )
	 && ( utf8_string_size > 0 )
	 && ( utf8_string_size <= FUZZER_STRING_SIZE_LIMIT ) )
	{
		utf8_string = (uint8_t *) malloc( utf8_string_size );

		if( utf8_string != NULL )
		{
			if( libevtx_record_get_utf8_channel_name(
			     record,
			     utf8_string,
			     utf8_string_size,
			     NULL ) == 1 )
			{
				value_byte ^= utf8_string[ 0 ];
			}
			free( utf8_string );
		}
	}
	/* The size is the number of UTF-16 characters,
	 * including the end of string character
	 */
	result = libevtx_record_get_utf16_channel_name_size(
	          record,
	          &utf16_string_size,
	          NULL );

	if( ( result == 1 )
	 && ( utf16_string_size > 0 )
	 && ( utf16_string_size <= FUZZER_STRING_SIZE_LIMIT ) )
	{
		utf16_string = (uint16_t *) malloc( utf16_string_size * sizeof( uint16_t ) );

		if( utf16_string != NULL )
		{
			if( libevtx_record_get_utf16_channel_name(
			     record,
			     utf16_string,
			     utf16_string_size,
			     NULL ) == 1 )
			{
				value_byte ^= (uint8_t) utf16_string[ 0 ];
			}
			free( utf16_string );
		}
	}
	result = libevtx_record_get_utf8_computer_name_size(
	          record,
	          &utf8_string_size,
	          NULL );

	if( ( result == 1 )
	 && ( utf8_string_size > 0 )
	 && ( utf8_string_size <= FUZZER_STRING_SIZE_LIMIT ) )
	{
		utf8_string = (uint8_t *) malloc( utf8_string_size );

		if( utf8_string != NULL )
		{
			if( libevtx_record_get_utf8_computer_name(
			     record,
			     utf8_string,
			     utf8_string_size,
			     NULL ) == 1 )
			{
				value_byte ^= utf8_string[ 0 ];
			}
			free( utf8_string );
		}
	}
	/* The size is the number of UTF-16 characters,
	 * including the end of string character
	 */
	result = libevtx_record_get_utf16_computer_name_size(
	          record,
	          &utf16_string_size,
	          NULL );

	if( ( result == 1 )
	 && ( utf16_string_size > 0 )
	 && ( utf16_string_size <= FUZZER_STRING_SIZE_LIMIT ) )
	{
		utf16_string = (uint16_t *) malloc( utf16_string_size * sizeof( uint16_t ) );

		if( utf16_string != NULL )
		{
			if( libevtx_record_get_utf16_computer_name(
			     record,
			     utf16_string,
			     utf16_string_size,
			     NULL ) == 1 )
			{
				value_byte ^= (uint8_t) utf16_string[ 0 ];
			}
			free( utf16_string );
		}
	}
	result = libevtx_record_get_utf8_user_security_identifier_size(
	          record,
	          &utf8_string_size,
	          NULL );

	if( ( result == 1 )
	 && ( utf8_string_size > 0 )
	 && ( utf8_string_size <= FUZZER_STRING_SIZE_LIMIT ) )
	{
		utf8_string = (uint8_t *) malloc( utf8_string_size );

		if( utf8_string != NULL )
		{
			if( libevtx_record_get_utf8_user_security_identifier(
			     record,
			     utf8_string,
			     utf8_string_size,
			     NULL ) == 1 )
			{
				value_byte ^= utf8_string[ 0 ];
			}
			free( utf8_string );
		}
	}
	/* The size is the number of UTF-16 characters,
	 * including the end of string character
	 */
	result = libevtx_record_get_utf16_user_security_identifier_size(
	          record,
	          &utf16_string_size,
	          NULL );

	if( ( result == 1 )
	 && ( utf16_string_size > 0 )
	 && ( utf16_string_size <= FUZZER_STRING_SIZE_LIMIT ) )
	{
		utf16_string = (uint16_t *) malloc( utf16_string_size * sizeof( uint16_t ) );

		if( utf16_string != NULL )
		{
			if( libevtx_record_get_utf16_user_security_identifier(
			     record,
			     utf16_string,
			     utf16_string_size,
			     NULL ) == 1 )
			{
				value_byte ^= (uint8_t) utf16_string[ 0 ];
			}
			free( utf16_string );
		}
	}
	result = libevtx_record_get_number_of_strings(
	          record,
	          &number_of_strings,
	          NULL );

	if( ( result == 1 )
	 && ( number_of_strings > 0 ) )
	{
		for( string_index = 0;
		     string_index < number_of_strings;
		     string_index++ )
		{
			if( string_index >= FUZZER_STRING_LIMIT )
			{
				break;
			}
			result = libevtx_record_get_utf8_string_size(
			          record,
			          string_index,
			          &utf8_string_size,
			          NULL );

			if( ( result == 1 )
			 && ( utf8_string_size > 0 )
			 && ( utf8_string_size <= FUZZER_STRING_SIZE_LIMIT ) )
			{
				utf8_string = (uint8_t *) malloc( utf8_string_size );

				if( utf8_string != NULL )
				{
					if( libevtx_record_get_utf8_string(
					     record,
					     string_index,
					     utf8_string,
					     utf8_string_size,
					     NULL ) == 1 )
					{
						value_byte ^= utf8_string[ 0 ];
					}
					free( utf8_string );
				}
			}
			/* The size is the number of UTF-16 characters,
			 * including the end of string character
			 */
			result = libevtx_record_get_utf16_string_size(
			          record,
			          string_index,
			          &utf16_string_size,
			          NULL );

			if( ( result == 1 )
			 && ( utf16_string_size > 0 )
			 && ( utf16_string_size <= FUZZER_STRING_SIZE_LIMIT ) )
			{
				utf16_string = (uint16_t *) malloc( utf16_string_size * sizeof( uint16_t ) );

				if( utf16_string != NULL )
				{
					if( libevtx_record_get_utf16_string(
					     record,
					     string_index,
					     utf16_string,
					     utf16_string_size,
					     NULL ) == 1 )
					{
						value_byte ^= (uint8_t) utf16_string[ 0 ];
					}
					free( utf16_string );
				}
			}
		}
	}
	/* Note that not all record fields are present in a record
	 */
	result = libevtx_record_get_data_size(
	          record,
	          &data_size,
	          NULL );

	if( ( result == 1 )
	 && ( data_size > 0 )
	 && ( data_size <= FUZZER_STRING_SIZE_LIMIT ) )
	{
		data_buffer = (uint8_t *) malloc( data_size );

		if( data_buffer != NULL )
		{
			if( libevtx_record_get_data(
			     record,
			     data_buffer,
			     data_size,
			     NULL ) == 1 )
			{
				value_byte ^= data_buffer[ 0 ];
			}
			free( data_buffer );
		}
	}
	result = libevtx_record_get_utf8_xml_string_size(
	          record,
	          &utf8_string_size,
	          NULL );

	if( ( result == 1 )
	 && ( utf8_string_size > 0 )
	 && ( utf8_string_size <= FUZZER_STRING_SIZE_LIMIT ) )
	{
		utf8_string = (uint8_t *) malloc( utf8_string_size );

		if( utf8_string != NULL )
		{
			if( libevtx_record_get_utf8_xml_string(
			     record,
			     utf8_string,
			     utf8_string_size,
			     NULL ) == 1 )
			{
				value_byte ^= utf8_string[ 0 ];
			}
			free( utf8_string );
		}
	}
	/* The size is the number of UTF-16 characters,
	 * including the end of string character
	 */
	result = libevtx_record_get_utf16_xml_string_size(
	          record,
	          &utf16_string_size,
	          NULL );

	if( ( result == 1 )
	 && ( utf16_string_size > 0 )
	 && ( utf16_string_size <= FUZZER_STRING_SIZE_LIMIT ) )
	{
		utf16_string = (uint16_t *) malloc( utf16_string_size * sizeof( uint16_t ) );

		if( utf16_string != NULL )
		{
			if( libevtx_record_get_utf16_xml_string(
			     record,
			     utf16_string,
			     utf16_string_size,
			     NULL ) == 1 )
			{
				value_byte ^= (uint8_t) utf16_string[ 0 ];
			}
			free( utf16_string );
		}
	}
	/* Use the decoded values as a sink to keep them live
	 */
	if( ( value_byte ^ (uint8_t) identifier ^ (uint8_t) offset ) != 0 )
	{
		/* No action required
		 */
	}
	return( 1 );
}

int LLVMFuzzerTestOneInput(
      const uint8_t *data,
      size_t size )
{
	libbfio_handle_t *file_io_handle = NULL;
	libevtx_file_t *file             = NULL;
	libevtx_record_t *record         = NULL;
	int number_of_records            = 0;
	int record_index                 = 0;
	int result                       = 0;

	if( libbfio_memory_range_initialize(
	     &file_io_handle,
	     NULL ) != 1 )
	{
		return( 0 );
	}
	if( libbfio_memory_range_set(
	     file_io_handle,
	     (uint8_t *) data,
	     size,
	     NULL ) != 1 )
	{
		goto on_error_libbfio;
	}
	if( libevtx_file_initialize(
	     &file,
	     NULL ) != 1 )
	{
		goto on_error_libbfio;
	}
	if( libevtx_file_open_file_io_handle(
	     file,
	     file_io_handle,
	     LIBEVTX_OPEN_READ,
	     NULL ) != 1 )
	{
		goto on_error_libevtx_file;
	}
	/* The codepage is selected from the fuzz data to steer the
	 * codepage conversion of the record values
	 */
	if( size > 0 )
	{
		libevtx_file_set_ascii_codepage(
		 file,
		 file_codepages[ data[ 0 ] % file_number_of_codepages ],
		 NULL );
	}
	result = libevtx_file_get_number_of_records(
	          file,
	          &number_of_records,
	          NULL );

	if( ( result == 1 )
	 && ( number_of_records > 0 ) )
	{
		for( record_index = 0;
		     record_index < number_of_records;
		     record_index++ )
		{
			if( record_index >= FUZZER_RECORD_LIMIT )
			{
				break;
			}
			record = NULL;

			if( libevtx_file_get_record_by_index(
			     file,
			     record_index,
			     &record,
			     NULL ) != 1 )
			{
				continue;
			}
			record_decode_fuzzer(
			 record );

			libevtx_record_free(
			 &record,
			 NULL );
		}
	}
	libevtx_file_close(
	 file,
	 NULL );

on_error_libevtx_file:
	libevtx_file_free(
	 &file,
	 NULL );

on_error_libbfio:
	libbfio_handle_free(
	 &file_io_handle,
	 NULL );

	return( 0 );
}

} /* extern "C" */
