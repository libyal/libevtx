/*
 * Tools path_handle type test program
 *
 * Copyright (C) 2011-2023, Joachim Metz <joachim.metz@gmail.com>
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
#include <memory.h>
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "evtx_test_libcerror.h"
#include "evtx_test_macros.h"
#include "evtx_test_memory.h"
#include "evtx_test_unused.h"

#include "../evtxtools/path_handle.h"

/* Tests the path_handle_initialize function
 * Returns 1 if successful or 0 if not
 */
int evtx_test_tools_path_handle_initialize(
     void )
{
	libcerror_error_t *error        = NULL;
	path_handle_t *path_handle      = NULL;
	int result                      = 0;

#if defined( HAVE_EVTX_TEST_MEMORY )
	int number_of_malloc_fail_tests = 1;
	int number_of_memset_fail_tests = 1;
	int test_number                 = 0;
#endif

	/* Test regular cases
	 */
	result = path_handle_initialize(
	          &path_handle,
	          &error );

	EVTX_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVTX_TEST_ASSERT_IS_NOT_NULL(
	 "path_handle",
	 path_handle );

	EVTX_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = path_handle_free(
	          &path_handle,
	          &error );

	EVTX_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVTX_TEST_ASSERT_IS_NULL(
	 "path_handle",
	 path_handle );

	EVTX_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = path_handle_initialize(
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

	path_handle = (path_handle_t *) 0x12345678UL;

	result = path_handle_initialize(
	          &path_handle,
	          &error );

	path_handle = NULL;

	EVTX_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EVTX_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

#if defined( HAVE_EVTX_TEST_MEMORY )

	for( test_number = 0;
	     test_number < number_of_malloc_fail_tests;
	     test_number++ )
	{
		/* Test path_handle_initialize with malloc failing
		 */
		evtx_test_malloc_attempts_before_fail = test_number;

		result = path_handle_initialize(
		          &path_handle,
		          &error );

		if( evtx_test_malloc_attempts_before_fail != -1 )
		{
			evtx_test_malloc_attempts_before_fail = -1;

			if( path_handle != NULL )
			{
				path_handle_free(
				 &path_handle,
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
			 "path_handle",
			 path_handle );

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
		/* Test path_handle_initialize with memset failing
		 */
		evtx_test_memset_attempts_before_fail = test_number;

		result = path_handle_initialize(
		          &path_handle,
		          &error );

		if( evtx_test_memset_attempts_before_fail != -1 )
		{
			evtx_test_memset_attempts_before_fail = -1;

			if( path_handle != NULL )
			{
				path_handle_free(
				 &path_handle,
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
			 "path_handle",
			 path_handle );

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
	if( path_handle != NULL )
	{
		path_handle_free(
		 &path_handle,
		 NULL );
	}
	return( 0 );
}

/* Tests the path_handle_free function
 * Returns 1 if successful or 0 if not
 */
int evtx_test_tools_path_handle_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = path_handle_free(
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

	EVTX_TEST_RUN(
	 "path_handle_initialize",
	 evtx_test_tools_path_handle_initialize );

	EVTX_TEST_RUN(
	 "path_handle_free",
	 evtx_test_tools_path_handle_free );

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

