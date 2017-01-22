/*
 * Library chunk type test program
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
#include <file_stream.h>
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "evtx_test_libcerror.h"
#include "evtx_test_libevtx.h"
#include "evtx_test_macros.h"
#include "evtx_test_memory.h"
#include "evtx_test_unused.h"

#include "../libevtx/libevtx_chunk.h"

#if defined( __GNUC__ )

/* Tests the libevtx_chunk_initialize function
 * Returns 1 if successful or 0 if not
 */
int evtx_test_chunk_initialize(
     void )
{
	libcerror_error_t *error        = NULL;
	libevtx_chunk_t *chunk          = NULL;
	int result                      = 0;

#if defined( HAVE_EVTX_TEST_MEMORY )
	int number_of_malloc_fail_tests = 1;
	int number_of_memset_fail_tests = 1;
	int test_number                 = 0;
#endif

	/* Test regular cases
	 */
	result = libevtx_chunk_initialize(
	          &chunk,
	          &error );

	EVTX_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        EVTX_TEST_ASSERT_IS_NOT_NULL(
         "chunk",
         chunk );

        EVTX_TEST_ASSERT_IS_NULL(
         "error",
         error );

	result = libevtx_chunk_free(
	          &chunk,
	          &error );

	EVTX_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        EVTX_TEST_ASSERT_IS_NULL(
         "chunk",
         chunk );

        EVTX_TEST_ASSERT_IS_NULL(
         "error",
         error );

	/* Test error cases
	 */
	result = libevtx_chunk_initialize(
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

	chunk = (libevtx_chunk_t *) 0x12345678UL;

	result = libevtx_chunk_initialize(
	          &chunk,
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

	chunk = NULL;

#if defined( HAVE_EVTX_TEST_MEMORY )

	for( test_number = 0;
	     test_number < number_of_malloc_fail_tests;
	     test_number++ )
	{
		/* Test libevtx_chunk_initialize with malloc failing
		 */
		evtx_test_malloc_attempts_before_fail = test_number;

		result = libevtx_chunk_initialize(
		          &chunk,
		          &error );

		if( evtx_test_malloc_attempts_before_fail != -1 )
		{
			evtx_test_malloc_attempts_before_fail = -1;

			if( chunk != NULL )
			{
				libevtx_chunk_free(
				 &chunk,
				 NULL );
			}
		}
		else
		{
			EVTX_TEST_ASSERT_EQUAL_INT(
			 "result",
			 result,
			 -1 );

			EVTX_TEST_ASSERT_IS_NULL(
			 "chunk",
			 chunk );

			EVTX_TEST_ASSERT_IS_NOT_NULL(
			 "error",
			 error );

			libcerror_error_free(
			 &error );
		}
	}
	for( test_number = 0;
	     test_number < number_of_memset_fail_tests;
	     test_number++ )
	{
		/* Test libevtx_chunk_initialize with memset failing
		 */
		evtx_test_memset_attempts_before_fail = test_number;

		result = libevtx_chunk_initialize(
		          &chunk,
		          &error );

		if( evtx_test_memset_attempts_before_fail != -1 )
		{
			evtx_test_memset_attempts_before_fail = -1;

			if( chunk != NULL )
			{
				libevtx_chunk_free(
				 &chunk,
				 NULL );
			}
		}
		else
		{
			EVTX_TEST_ASSERT_EQUAL_INT(
			 "result",
			 result,
			 -1 );

			EVTX_TEST_ASSERT_IS_NULL(
			 "chunk",
			 chunk );

			EVTX_TEST_ASSERT_IS_NOT_NULL(
			 "error",
			 error );

			libcerror_error_free(
			 &error );
		}
	}
#endif /* defined( HAVE_EVTX_TEST_MEMORY ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( chunk != NULL )
	{
		libevtx_chunk_free(
		 &chunk,
		 NULL );
	}
	return( 0 );
}

/* Tests the libevtx_chunk_free function
 * Returns 1 if successful or 0 if not
 */
int evtx_test_chunk_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libevtx_chunk_free(
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

/* Tests the libevtx_chunk_get_number_of_records function
 * Returns 1 if successful or 0 if not
 */
int evtx_test_chunk_get_number_of_records(
     void )
{
	libcerror_error_t *error     = NULL;
	libevtx_chunk_t *chunk       = NULL;
	uint16_t number_of_records   = 0;
	int number_of_records_is_set = 0;
	int result                   = 0;

	/* Initialize test
	 */
	result = libevtx_chunk_initialize(
	          &chunk,
	          &error );

	EVTX_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVTX_TEST_ASSERT_IS_NOT_NULL(
	 "chunk",
	 chunk );

	EVTX_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libevtx_chunk_get_number_of_records(
	          chunk,
	          &number_of_records,
	          &error );

	EVTX_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EVTX_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	number_of_records_is_set = result;

	/* Test error cases
	 */
	result = libevtx_chunk_get_number_of_records(
	          NULL,
	          &number_of_records,
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

	if( number_of_records_is_set != 0 )
	{
		result = libevtx_chunk_get_number_of_records(
		          chunk,
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
	}
	/* Clean up
	 */
	result = libevtx_chunk_free(
	          &chunk,
	          &error );

	EVTX_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVTX_TEST_ASSERT_IS_NULL(
	 "chunk",
	 chunk );

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
	if( chunk != NULL )
	{
		libevtx_chunk_free(
		 &chunk,
		 NULL );
	}
	return( 0 );
}

/* Tests the libevtx_chunk_get_number_of_recovered_records function
 * Returns 1 if successful or 0 if not
 */
int evtx_test_chunk_get_number_of_recovered_records(
     void )
{
	libcerror_error_t *error               = NULL;
	libevtx_chunk_t *chunk                 = NULL;
	uint16_t number_of_recovered_records   = 0;
	int number_of_recovered_records_is_set = 0;
	int result                             = 0;

	/* Initialize test
	 */
	result = libevtx_chunk_initialize(
	          &chunk,
	          &error );

	EVTX_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVTX_TEST_ASSERT_IS_NOT_NULL(
	 "chunk",
	 chunk );

	EVTX_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libevtx_chunk_get_number_of_recovered_records(
	          chunk,
	          &number_of_recovered_records,
	          &error );

	EVTX_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EVTX_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	number_of_recovered_records_is_set = result;

	/* Test error cases
	 */
	result = libevtx_chunk_get_number_of_recovered_records(
	          NULL,
	          &number_of_recovered_records,
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

	if( number_of_recovered_records_is_set != 0 )
	{
		result = libevtx_chunk_get_number_of_recovered_records(
		          chunk,
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
	}
	/* Clean up
	 */
	result = libevtx_chunk_free(
	          &chunk,
	          &error );

	EVTX_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVTX_TEST_ASSERT_IS_NULL(
	 "chunk",
	 chunk );

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
	if( chunk != NULL )
	{
		libevtx_chunk_free(
		 &chunk,
		 NULL );
	}
	return( 0 );
}

#endif /* defined( __GNUC__ ) */

/* The main program
 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
int wmain(
     int argc EVTX_TEST_ATTRIBUTE_UNUSED,
     wchar_t * const argv[] EVTX_TEST_ATTRIBUTE_UNUSED )
#else
int main(
     int argc EVTX_TEST_ATTRIBUTE_UNUSED,
     char * const argv[] EVTX_TEST_ATTRIBUTE_UNUSED )
#endif
{
	EVTX_TEST_UNREFERENCED_PARAMETER( argc )
	EVTX_TEST_UNREFERENCED_PARAMETER( argv )

#if defined( __GNUC__ )

	EVTX_TEST_RUN(
	 "libevtx_chunk_initialize",
	 evtx_test_chunk_initialize );

	EVTX_TEST_RUN(
	 "libevtx_chunk_free",
	 evtx_test_chunk_free );

	/* TODO: add tests for libevtx_chunk_read */

	EVTX_TEST_RUN(
	 "libevtx_chunk_get_number_of_records",
	 evtx_test_chunk_get_number_of_records );

	/* TODO: add tests for libevtx_chunk_get_record */

	EVTX_TEST_RUN(
	 "libevtx_chunk_get_number_of_recovered_records",
	 evtx_test_chunk_get_number_of_recovered_records );

	/* TODO: add tests for libevtx_chunk_get_recovered_record */

#endif /* defined( __GNUC__ ) */

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

