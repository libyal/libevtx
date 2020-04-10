/*
 * Library support functions test program
 *
 * Copyright (C) 2011-2020, Joachim Metz <joachim.metz@gmail.com>
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

#include <common.h>
#include <file_stream.h>
#include <narrow_string.h>
#include <system_string.h>
#include <types.h>
#include <wide_string.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "evtx_test_functions.h"
#include "evtx_test_getopt.h"
#include "evtx_test_libbfio.h"
#include "evtx_test_libcerror.h"
#include "evtx_test_libevtx.h"
#include "evtx_test_macros.h"
#include "evtx_test_memory.h"
#include "evtx_test_unused.h"

#if !defined( LIBEVTX_HAVE_BFIO )

LIBEVTX_EXTERN \
int libevtx_check_file_signature_file_io_handle(
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error );

#endif /* !defined( LIBEVTX_HAVE_BFIO ) */

/* Tests the libevtx_get_version function
 * Returns 1 if successful or 0 if not
 */
int evtx_test_get_version(
     void )
{
	const char *version_string = NULL;
	int result                 = 0;

	version_string = libevtx_get_version();

	result = narrow_string_compare(
	          version_string,
	          LIBEVTX_VERSION_STRING,
	          9 );

	EVTX_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	return( 1 );

on_error:
	return( 0 );
}

/* Tests the libevtx_get_access_flags_read function
 * Returns 1 if successful or 0 if not
 */
int evtx_test_get_access_flags_read(
     void )
{
	int access_flags = 0;

	access_flags = libevtx_get_access_flags_read();

	EVTX_TEST_ASSERT_EQUAL_INT(
	 "access_flags",
	 access_flags,
	 LIBEVTX_ACCESS_FLAG_READ );

	return( 1 );

on_error:
	return( 0 );
}

/* Tests the libevtx_get_codepage function
 * Returns 1 if successful or 0 if not
 */
int evtx_test_get_codepage(
     void )
{
	libcerror_error_t *error = NULL;
	int codepage             = 0;
	int result               = 0;

	result = libevtx_get_codepage(
	          &codepage,
	          &error );

	EVTX_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVTX_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libevtx_get_codepage(
	          NULL,
	          &error );

	EVTX_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EVTX_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libevtx_set_codepage function
 * Returns 1 if successful or 0 if not
 */
int evtx_test_set_codepage(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	result = libevtx_set_codepage(
	          0,
	          &error );

	EVTX_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVTX_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libevtx_set_codepage(
	          -1,
	          &error );

	EVTX_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EVTX_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libevtx_check_file_signature function
 * Returns 1 if successful or 0 if not
 */
int evtx_test_check_file_signature(
     const system_character_t *source )
{
	char narrow_source[ 256 ];

	libcerror_error_t *error = NULL;
	int result               = 0;

	if( source != NULL )
	{
		/* Initialize test
		 */
		result = evtx_test_get_narrow_source(
		          source,
		          narrow_source,
		          256,
		          &error );

		EVTX_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		EVTX_TEST_ASSERT_IS_NULL(
		 "error",
		 error );

		/* Test check file signature
		 */
		result = libevtx_check_file_signature(
		          narrow_source,
		          &error );

		EVTX_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		EVTX_TEST_ASSERT_IS_NULL(
		 "error",
		 error );
	}
	/* Test error cases
	 */
	result = libevtx_check_file_signature(
	          NULL,
	          &error );

	EVTX_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EVTX_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libevtx_check_file_signature(
	          "",
	          &error );

	EVTX_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EVTX_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	if( source != NULL )
	{
#if defined( HAVE_EVTX_TEST_MEMORY )

		/* Test libevtx_check_file_signature with malloc failing in libbfio_file_initialize
		 */
		evtx_test_malloc_attempts_before_fail = 0;

		result = libevtx_check_file_signature(
		          narrow_source,
		          &error );

		if( evtx_test_malloc_attempts_before_fail != -1 )
		{
			evtx_test_malloc_attempts_before_fail = -1;
		}
		else
		{
			EVTX_TEST_ASSERT_EQUAL_INT(
			 "result",
			 result,
			 -1 );

			EVTX_TEST_ASSERT_IS_NOT_NULL(
			 "error",
			 error );

			libcerror_error_free(
			 &error );
		}
#endif /* defined( HAVE_EVTX_TEST_MEMORY ) */
	}
	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

#if defined( HAVE_WIDE_CHARACTER_TYPE )

/* Tests the libevtx_check_file_signature_wide function
 * Returns 1 if successful or 0 if not
 */
int evtx_test_check_file_signature_wide(
     const system_character_t *source )
{
	wchar_t wide_source[ 256 ];

	libcerror_error_t *error = NULL;
	int result               = 0;

	if( source != NULL )
	{
		/* Initialize test
		 */
		result = evtx_test_get_wide_source(
		          source,
		          wide_source,
		          256,
		          &error );

		EVTX_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		EVTX_TEST_ASSERT_IS_NULL(
		 "error",
		 error );

		/* Test check file signature
		 */
		result = libevtx_check_file_signature_wide(
		          wide_source,
		          &error );

		EVTX_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		EVTX_TEST_ASSERT_IS_NULL(
		 "error",
		 error );
	}
	/* Test error cases
	 */
	result = libevtx_check_file_signature_wide(
	          NULL,
	          &error );

	EVTX_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EVTX_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libevtx_check_file_signature_wide(
	          L"",
	          &error );

	EVTX_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EVTX_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	if( source != NULL )
	{
#if defined( HAVE_EVTX_TEST_MEMORY )

		/* Test libevtx_check_file_signature_wide with malloc failing in libbfio_file_initialize
		 */
		evtx_test_malloc_attempts_before_fail = 0;

		result = libevtx_check_file_signature_wide(
		          wide_source,
		          &error );

		if( evtx_test_malloc_attempts_before_fail != -1 )
		{
			evtx_test_malloc_attempts_before_fail = -1;
		}
		else
		{
			EVTX_TEST_ASSERT_EQUAL_INT(
			 "result",
			 result,
			 -1 );

			EVTX_TEST_ASSERT_IS_NOT_NULL(
			 "error",
			 error );

			libcerror_error_free(
			 &error );
		}
#endif /* defined( HAVE_EVTX_TEST_MEMORY ) */
	}
	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

/* Tests the libevtx_check_file_signature_file_io_handle function
 * Returns 1 if successful or 0 if not
 */
int evtx_test_check_file_signature_file_io_handle(
     const system_character_t *source )
{
	uint8_t empty_block[ 8192 ];

	libbfio_handle_t *file_io_handle = NULL;
	libcerror_error_t *error         = NULL;
	void *memset_result              = NULL;
	size_t source_length             = 0;
	int result                       = 0;

	/* Initialize test
	 */
	memset_result = memory_set(
	                 empty_block,
	                 0,
	                 sizeof( uint8_t ) * 8192 );

	EVTX_TEST_ASSERT_IS_NOT_NULL(
	 "memset_result",
	 memset_result );

	if( source != NULL )
	{
		/* Initialize test
		 */
		result = libbfio_file_initialize(
		          &file_io_handle,
		          &error );

		EVTX_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		EVTX_TEST_ASSERT_IS_NOT_NULL(
		 "file_io_handle",
		 file_io_handle );

		EVTX_TEST_ASSERT_IS_NULL(
		 "error",
		 error );

		source_length = system_string_length(
		                 source );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libbfio_file_set_name_wide(
		          file_io_handle,
		          source,
		          source_length,
		          &error );
#else
		result = libbfio_file_set_name(
		          file_io_handle,
		          source,
		          source_length,
		          &error );
#endif
		EVTX_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		EVTX_TEST_ASSERT_IS_NULL(
		 "error",
		 error );

		result = libbfio_handle_open(
		          file_io_handle,
		          LIBBFIO_OPEN_READ,
		          &error );

		EVTX_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		EVTX_TEST_ASSERT_IS_NULL(
		 "error",
		 error );

		/* Test check file signature
		 */
		result = libevtx_check_file_signature_file_io_handle(
		          file_io_handle,
		          &error );

		EVTX_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		EVTX_TEST_ASSERT_IS_NULL(
		 "error",
		 error );
	}
	/* Test error cases
	 */
	result = libevtx_check_file_signature_file_io_handle(
	          NULL,
	          &error );

	EVTX_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EVTX_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	/* Clean up
	 */
	if( source != NULL )
	{
		result = libbfio_handle_close(
		          file_io_handle,
		          &error );

		EVTX_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 0 );

		EVTX_TEST_ASSERT_IS_NULL(
		 "error",
		 error );

		result = libbfio_handle_free(
		          &file_io_handle,
		          &error );

		EVTX_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		EVTX_TEST_ASSERT_IS_NULL(
		 "file_io_handle",
		 file_io_handle );

		EVTX_TEST_ASSERT_IS_NULL(
		 "error",
		 error );
	}
	/* Test check file signature with data too small
	 */
	result = evtx_test_open_file_io_handle(
	          &file_io_handle,
	          empty_block,
	          sizeof( uint8_t ) * 1,
	          &error );

	EVTX_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVTX_TEST_ASSERT_IS_NOT_NULL(
	 "file_io_handle",
	 file_io_handle );

	EVTX_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libevtx_check_file_signature_file_io_handle(
	          file_io_handle,
	          &error );

	EVTX_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EVTX_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = evtx_test_close_file_io_handle(
	          &file_io_handle,
	          &error );

	EVTX_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	EVTX_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test check file signature with empty block
	 */
	result = evtx_test_open_file_io_handle(
	          &file_io_handle,
	          empty_block,
	          sizeof( uint8_t ) * 8192,
	          &error );

	EVTX_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVTX_TEST_ASSERT_IS_NOT_NULL(
	 "file_io_handle",
	 file_io_handle );

	EVTX_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libevtx_check_file_signature_file_io_handle(
	          file_io_handle,
	          &error );

	EVTX_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	EVTX_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = evtx_test_close_file_io_handle(
	          &file_io_handle,
	          &error );

	EVTX_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	EVTX_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
	return( 0 );
}

/* The main program
 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
int wmain(
     int argc,
     wchar_t * const argv[] )
#else
int main(
     int argc,
     char * const argv[] )
#endif
{
	system_character_t *source = NULL;
	system_integer_t option    = 0;

	while( ( option = evtx_test_getopt(
	                   argc,
	                   argv,
	                   _SYSTEM_STRING( "" ) ) ) != (system_integer_t) -1 )
	{
		switch( option )
		{
			case (system_integer_t) '?':
			default:
				fprintf(
				 stderr,
				 "Invalid argument: %" PRIs_SYSTEM ".\n",
				 argv[ optind - 1 ] );

				return( EXIT_FAILURE );
		}
	}
	if( optind < argc )
	{
		source = argv[ optind ];
	}
	EVTX_TEST_RUN(
	 "libevtx_get_version",
	 evtx_test_get_version );

	EVTX_TEST_RUN(
	 "libevtx_get_access_flags_read",
	 evtx_test_get_access_flags_read );

	EVTX_TEST_RUN(
	 "libevtx_get_codepage",
	 evtx_test_get_codepage );

	EVTX_TEST_RUN(
	 "libevtx_set_codepage",
	 evtx_test_set_codepage );

#if !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 )

	EVTX_TEST_RUN_WITH_ARGS(
	 "libevtx_check_file_signature",
	 evtx_test_check_file_signature,
	 source );

#if defined( HAVE_WIDE_CHARACTER_TYPE )

	EVTX_TEST_RUN_WITH_ARGS(
	 "libevtx_check_file_signature_wide",
	 evtx_test_check_file_signature_wide,
	 source );

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

	EVTX_TEST_RUN_WITH_ARGS(
	 "libevtx_check_file_signature_file_io_handle",
	 evtx_test_check_file_signature_file_io_handle,
	 source );

#endif /* !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 ) */

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

