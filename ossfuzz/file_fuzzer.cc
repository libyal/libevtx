/*
 * OSS-Fuzz target for libevtx file type
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

int LLVMFuzzerTestOneInput(
      const uint8_t *data,
      size_t size )
{
	libbfio_handle_t *file_io_handle = NULL;
	libevtx_file_t *file             = NULL;
	libevtx_record_t *record         = NULL;
	uint16_t major_version           = 0;
	uint16_t minor_version           = 0;
	uint32_t flags                   = 0;
	int ascii_codepage               = 0;
	int number_of_records            = 0;
	int number_of_recovered_records  = 0;
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

		libevtx_file_get_ascii_codepage(
		 file,
		 &ascii_codepage,
		 NULL );
	}
	libevtx_file_get_format_version(
	 file,
	 &major_version,
	 &minor_version,
	 NULL );

	libevtx_file_get_flags(
	 file,
	 &flags,
	 NULL );

	libevtx_file_is_corrupted(
	 file,
	 NULL );

	result = libevtx_file_get_number_of_records(
	          file,
	          &number_of_records,
	          NULL );

	if( ( result == 1 )
	 && ( number_of_records > 0 ) )
	{
		record = NULL;

		if( libevtx_file_get_record_by_index(
		     file,
		     0,
		     &record,
		     NULL ) == 1 )
		{
			libevtx_record_free(
			 &record,
			 NULL );
		}
	}
	result = libevtx_file_get_number_of_recovered_records(
	          file,
	          &number_of_recovered_records,
	          NULL );

	if( ( result == 1 )
	 && ( number_of_recovered_records > 0 ) )
	{
		record = NULL;

		if( libevtx_file_get_recovered_record_by_index(
		     file,
		     0,
		     &record,
		     NULL ) == 1 )
		{
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
