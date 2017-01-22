/*
 * Library record_values type test program
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

#include "../libevtx/libevtx_record_values.h"

#if defined( __GNUC__ )

/* Tests the libevtx_record_values_initialize function
 * Returns 1 if successful or 0 if not
 */
int evtx_test_record_values_initialize(
     void )
{
	libcerror_error_t *error               = NULL;
	libevtx_record_values_t *record_values = NULL;
	int result                             = 0;

#if defined( HAVE_EVTX_TEST_MEMORY )
	int number_of_malloc_fail_tests        = 1;
	int number_of_memset_fail_tests        = 1;
	int test_number                        = 0;
#endif

	/* Test regular cases
	 */
	result = libevtx_record_values_initialize(
	          &record_values,
	          &error );

	EVTX_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        EVTX_TEST_ASSERT_IS_NOT_NULL(
         "record_values",
         record_values );

        EVTX_TEST_ASSERT_IS_NULL(
         "error",
         error );

	result = libevtx_record_values_free(
	          &record_values,
	          &error );

	EVTX_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        EVTX_TEST_ASSERT_IS_NULL(
         "record_values",
         record_values );

        EVTX_TEST_ASSERT_IS_NULL(
         "error",
         error );

	/* Test error cases
	 */
	result = libevtx_record_values_initialize(
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

	record_values = (libevtx_record_values_t *) 0x12345678UL;

	result = libevtx_record_values_initialize(
	          &record_values,
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

	record_values = NULL;

#if defined( HAVE_EVTX_TEST_MEMORY )

	for( test_number = 0;
	     test_number < number_of_malloc_fail_tests;
	     test_number++ )
	{
		/* Test libevtx_record_values_initialize with malloc failing
		 */
		evtx_test_malloc_attempts_before_fail = test_number;

		result = libevtx_record_values_initialize(
		          &record_values,
		          &error );

		if( evtx_test_malloc_attempts_before_fail != -1 )
		{
			evtx_test_malloc_attempts_before_fail = -1;

			if( record_values != NULL )
			{
				libevtx_record_values_free(
				 &record_values,
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
			 "record_values",
			 record_values );

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
		/* Test libevtx_record_values_initialize with memset failing
		 */
		evtx_test_memset_attempts_before_fail = test_number;

		result = libevtx_record_values_initialize(
		          &record_values,
		          &error );

		if( evtx_test_memset_attempts_before_fail != -1 )
		{
			evtx_test_memset_attempts_before_fail = -1;

			if( record_values != NULL )
			{
				libevtx_record_values_free(
				 &record_values,
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
			 "record_values",
			 record_values );

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
	if( record_values != NULL )
	{
		libevtx_record_values_free(
		 &record_values,
		 NULL );
	}
	return( 0 );
}

/* Tests the libevtx_record_values_free function
 * Returns 1 if successful or 0 if not
 */
int evtx_test_record_values_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libevtx_record_values_free(
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

/* Tests the libevtx_record_values_clone function
 * Returns 1 if successful or 0 if not
 */
int evtx_test_record_values_clone(
     void )
{
	libcerror_error_t *error                           = NULL;
	libevtx_record_values_t *destination_record_values = NULL;
	libevtx_record_values_t *source_record_values      = NULL;
	int result                                         = 0;

	/* Initialize test
	 */
	result = libevtx_record_values_initialize(
	          &source_record_values,
	          &error );

	EVTX_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        EVTX_TEST_ASSERT_IS_NOT_NULL(
         "source_record_values",
         source_record_values );

        EVTX_TEST_ASSERT_IS_NULL(
         "error",
         error );

	/* Test regular cases
	 */
	result = libevtx_record_values_clone(
	          &destination_record_values,
	          source_record_values,
	          &error );

	EVTX_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        EVTX_TEST_ASSERT_IS_NOT_NULL(
         "destination_record_values",
         destination_record_values );

        EVTX_TEST_ASSERT_IS_NULL(
         "error",
         error );

	result = libevtx_record_values_free(
	          &destination_record_values,
	          &error );

	EVTX_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        EVTX_TEST_ASSERT_IS_NULL(
         "destination_record_values",
         destination_record_values );

        EVTX_TEST_ASSERT_IS_NULL(
         "error",
         error );

	result = libevtx_record_values_clone(
	          &destination_record_values,
	          NULL,
	          &error );

	EVTX_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        EVTX_TEST_ASSERT_IS_NULL(
         "destination_record_values",
         destination_record_values );

        EVTX_TEST_ASSERT_IS_NULL(
         "error",
         error );

	/* Test error cases
	 */
	result = libevtx_record_values_clone(
	          NULL,
	          source_record_values,
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
	result = libevtx_record_values_free(
	          &source_record_values,
	          &error );

	EVTX_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        EVTX_TEST_ASSERT_IS_NULL(
         "source_record_values",
         source_record_values );

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
	if( destination_record_values != NULL )
	{
		libevtx_record_values_free(
		 &destination_record_values,
		 NULL );
	}
	if( source_record_values != NULL )
	{
		libevtx_record_values_free(
		 &source_record_values,
		 NULL );
	}
	return( 0 );
}

/* Tests the libevtx_record_values_get_event_identifier function
 * Returns 1 if successful or 0 if not
 */
int evtx_test_record_values_get_event_identifier(
     void )
{
	libcerror_error_t *error               = NULL;
	libevtx_record_values_t *record_values = NULL;
	uint32_t event_identifier              = 0;
	int event_identifier_is_set            = 0;
	int result                             = 0;

	/* Initialize test
	 */
	result = libevtx_record_values_initialize(
	          &record_values,
	          &error );

	EVTX_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVTX_TEST_ASSERT_IS_NOT_NULL(
	 "record_values",
	 record_values );

	EVTX_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libevtx_record_values_get_event_identifier(
	          record_values,
	          &event_identifier,
	          &error );

	EVTX_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EVTX_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	event_identifier_is_set = result;

	/* Test error cases
	 */
	result = libevtx_record_values_get_event_identifier(
	          NULL,
	          &event_identifier,
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

	if( event_identifier_is_set != 0 )
	{
		result = libevtx_record_values_get_event_identifier(
		          record_values,
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
	result = libevtx_record_values_free(
	          &record_values,
	          &error );

	EVTX_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVTX_TEST_ASSERT_IS_NULL(
	 "record_values",
	 record_values );

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
	if( record_values != NULL )
	{
		libevtx_record_values_free(
		 &record_values,
		 NULL );
	}
	return( 0 );
}

/* Tests the libevtx_record_values_get_event_identifier_qualifiers function
 * Returns 1 if successful or 0 if not
 */
int evtx_test_record_values_get_event_identifier_qualifiers(
     void )
{
	libcerror_error_t *error               = NULL;
	libevtx_record_values_t *record_values = NULL;
	uint32_t event_identifier_qualifiers   = 0;
	int event_identifier_qualifiers_is_set = 0;
	int result                             = 0;

	/* Initialize test
	 */
	result = libevtx_record_values_initialize(
	          &record_values,
	          &error );

	EVTX_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVTX_TEST_ASSERT_IS_NOT_NULL(
	 "record_values",
	 record_values );

	EVTX_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libevtx_record_values_get_event_identifier_qualifiers(
	          record_values,
	          &event_identifier_qualifiers,
	          &error );

	EVTX_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EVTX_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	event_identifier_qualifiers_is_set = result;

	/* Test error cases
	 */
	result = libevtx_record_values_get_event_identifier_qualifiers(
	          NULL,
	          &event_identifier_qualifiers,
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

	if( event_identifier_qualifiers_is_set != 0 )
	{
		result = libevtx_record_values_get_event_identifier_qualifiers(
		          record_values,
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
	result = libevtx_record_values_free(
	          &record_values,
	          &error );

	EVTX_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVTX_TEST_ASSERT_IS_NULL(
	 "record_values",
	 record_values );

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
	if( record_values != NULL )
	{
		libevtx_record_values_free(
		 &record_values,
		 NULL );
	}
	return( 0 );
}

/* Tests the libevtx_record_values_get_event_level function
 * Returns 1 if successful or 0 if not
 */
int evtx_test_record_values_get_event_level(
     void )
{
	libcerror_error_t *error               = NULL;
	libevtx_record_values_t *record_values = NULL;
	uint8_t event_level                    = 0;
	int event_level_is_set                 = 0;
	int result                             = 0;

	/* Initialize test
	 */
	result = libevtx_record_values_initialize(
	          &record_values,
	          &error );

	EVTX_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVTX_TEST_ASSERT_IS_NOT_NULL(
	 "record_values",
	 record_values );

	EVTX_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libevtx_record_values_get_event_level(
	          record_values,
	          &event_level,
	          &error );

	EVTX_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EVTX_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	event_level_is_set = result;

	/* Test error cases
	 */
	result = libevtx_record_values_get_event_level(
	          NULL,
	          &event_level,
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

	if( event_level_is_set != 0 )
	{
		result = libevtx_record_values_get_event_level(
		          record_values,
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
	result = libevtx_record_values_free(
	          &record_values,
	          &error );

	EVTX_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVTX_TEST_ASSERT_IS_NULL(
	 "record_values",
	 record_values );

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
	if( record_values != NULL )
	{
		libevtx_record_values_free(
		 &record_values,
		 NULL );
	}
	return( 0 );
}

/* Tests the libevtx_record_values_get_utf8_provider_identifier_size function
 * Returns 1 if successful or 0 if not
 */
int evtx_test_record_values_get_utf8_provider_identifier_size(
     void )
{
	libcerror_error_t *error                 = NULL;
	libevtx_record_values_t *record_values   = NULL;
	size_t utf8_provider_identifier_size     = 0;
	int result                               = 0;
	int utf8_provider_identifier_size_is_set = 0;

	/* Initialize test
	 */
	result = libevtx_record_values_initialize(
	          &record_values,
	          &error );

	EVTX_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVTX_TEST_ASSERT_IS_NOT_NULL(
	 "record_values",
	 record_values );

	EVTX_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libevtx_record_values_get_utf8_provider_identifier_size(
	          record_values,
	          &utf8_provider_identifier_size,
	          &error );

	EVTX_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EVTX_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	utf8_provider_identifier_size_is_set = result;

	/* Test error cases
	 */
	result = libevtx_record_values_get_utf8_provider_identifier_size(
	          NULL,
	          &utf8_provider_identifier_size,
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

	if( utf8_provider_identifier_size_is_set != 0 )
	{
		result = libevtx_record_values_get_utf8_provider_identifier_size(
		          record_values,
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
	result = libevtx_record_values_free(
	          &record_values,
	          &error );

	EVTX_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVTX_TEST_ASSERT_IS_NULL(
	 "record_values",
	 record_values );

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
	if( record_values != NULL )
	{
		libevtx_record_values_free(
		 &record_values,
		 NULL );
	}
	return( 0 );
}

/* Tests the libevtx_record_values_get_utf8_provider_identifier function
 * Returns 1 if successful or 0 if not
 */
int evtx_test_record_values_get_utf8_provider_identifier(
     void )
{
	uint8_t utf8_provider_identifier[ 512 ];

	libcerror_error_t *error               = NULL;
	libevtx_record_values_t *record_values = NULL;
	int result                             = 0;
	int utf8_provider_identifier_is_set    = 0;

	/* Initialize test
	 */
	result = libevtx_record_values_initialize(
	          &record_values,
	          &error );

	EVTX_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVTX_TEST_ASSERT_IS_NOT_NULL(
	 "record_values",
	 record_values );

	EVTX_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libevtx_record_values_get_utf8_provider_identifier(
	          record_values,
	          utf8_provider_identifier,
	          512,
	          &error );

	EVTX_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EVTX_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	utf8_provider_identifier_is_set = result;

	/* Test error cases
	 */
	result = libevtx_record_values_get_utf8_provider_identifier(
	          NULL,
	          utf8_provider_identifier,
	          512,
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

	if( utf8_provider_identifier_is_set != 0 )
	{
		result = libevtx_record_values_get_utf8_provider_identifier(
		          record_values,
		          NULL,
		          512,
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

		result = libevtx_record_values_get_utf8_provider_identifier(
		          record_values,
		          utf8_provider_identifier,
		          0,
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

		result = libevtx_record_values_get_utf8_provider_identifier(
		          record_values,
		          utf8_provider_identifier,
		          (size_t) SSIZE_MAX + 1,
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
	result = libevtx_record_values_free(
	          &record_values,
	          &error );

	EVTX_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVTX_TEST_ASSERT_IS_NULL(
	 "record_values",
	 record_values );

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
	if( record_values != NULL )
	{
		libevtx_record_values_free(
		 &record_values,
		 NULL );
	}
	return( 0 );
}

/* Tests the libevtx_record_values_get_utf16_provider_identifier_size function
 * Returns 1 if successful or 0 if not
 */
int evtx_test_record_values_get_utf16_provider_identifier_size(
     void )
{
	libcerror_error_t *error                  = NULL;
	libevtx_record_values_t *record_values    = NULL;
	size_t utf16_provider_identifier_size     = 0;
	int result                                = 0;
	int utf16_provider_identifier_size_is_set = 0;

	/* Initialize test
	 */
	result = libevtx_record_values_initialize(
	          &record_values,
	          &error );

	EVTX_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVTX_TEST_ASSERT_IS_NOT_NULL(
	 "record_values",
	 record_values );

	EVTX_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libevtx_record_values_get_utf16_provider_identifier_size(
	          record_values,
	          &utf16_provider_identifier_size,
	          &error );

	EVTX_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EVTX_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	utf16_provider_identifier_size_is_set = result;

	/* Test error cases
	 */
	result = libevtx_record_values_get_utf16_provider_identifier_size(
	          NULL,
	          &utf16_provider_identifier_size,
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

	if( utf16_provider_identifier_size_is_set != 0 )
	{
		result = libevtx_record_values_get_utf16_provider_identifier_size(
		          record_values,
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
	result = libevtx_record_values_free(
	          &record_values,
	          &error );

	EVTX_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVTX_TEST_ASSERT_IS_NULL(
	 "record_values",
	 record_values );

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
	if( record_values != NULL )
	{
		libevtx_record_values_free(
		 &record_values,
		 NULL );
	}
	return( 0 );
}

/* Tests the libevtx_record_values_get_utf16_provider_identifier function
 * Returns 1 if successful or 0 if not
 */
int evtx_test_record_values_get_utf16_provider_identifier(
     void )
{
	uint16_t utf16_provider_identifier[ 512 ];

	libcerror_error_t *error               = NULL;
	libevtx_record_values_t *record_values = NULL;
	int result                             = 0;
	int utf16_provider_identifier_is_set   = 0;

	/* Initialize test
	 */
	result = libevtx_record_values_initialize(
	          &record_values,
	          &error );

	EVTX_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVTX_TEST_ASSERT_IS_NOT_NULL(
	 "record_values",
	 record_values );

	EVTX_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libevtx_record_values_get_utf16_provider_identifier(
	          record_values,
	          utf16_provider_identifier,
	          512,
	          &error );

	EVTX_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EVTX_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	utf16_provider_identifier_is_set = result;

	/* Test error cases
	 */
	result = libevtx_record_values_get_utf16_provider_identifier(
	          NULL,
	          utf16_provider_identifier,
	          512,
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

	if( utf16_provider_identifier_is_set != 0 )
	{
		result = libevtx_record_values_get_utf16_provider_identifier(
		          record_values,
		          NULL,
		          512,
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

		result = libevtx_record_values_get_utf16_provider_identifier(
		          record_values,
		          utf16_provider_identifier,
		          0,
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

		result = libevtx_record_values_get_utf16_provider_identifier(
		          record_values,
		          utf16_provider_identifier,
		          (size_t) SSIZE_MAX + 1,
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
	result = libevtx_record_values_free(
	          &record_values,
	          &error );

	EVTX_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVTX_TEST_ASSERT_IS_NULL(
	 "record_values",
	 record_values );

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
	if( record_values != NULL )
	{
		libevtx_record_values_free(
		 &record_values,
		 NULL );
	}
	return( 0 );
}

/* Tests the libevtx_record_values_get_utf8_source_name_size function
 * Returns 1 if successful or 0 if not
 */
int evtx_test_record_values_get_utf8_source_name_size(
     void )
{
	libcerror_error_t *error               = NULL;
	libevtx_record_values_t *record_values = NULL;
	size_t utf8_source_name_size           = 0;
	int result                             = 0;
	int utf8_source_name_size_is_set       = 0;

	/* Initialize test
	 */
	result = libevtx_record_values_initialize(
	          &record_values,
	          &error );

	EVTX_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVTX_TEST_ASSERT_IS_NOT_NULL(
	 "record_values",
	 record_values );

	EVTX_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libevtx_record_values_get_utf8_source_name_size(
	          record_values,
	          &utf8_source_name_size,
	          &error );

	EVTX_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EVTX_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	utf8_source_name_size_is_set = result;

	/* Test error cases
	 */
	result = libevtx_record_values_get_utf8_source_name_size(
	          NULL,
	          &utf8_source_name_size,
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

	if( utf8_source_name_size_is_set != 0 )
	{
		result = libevtx_record_values_get_utf8_source_name_size(
		          record_values,
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
	result = libevtx_record_values_free(
	          &record_values,
	          &error );

	EVTX_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVTX_TEST_ASSERT_IS_NULL(
	 "record_values",
	 record_values );

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
	if( record_values != NULL )
	{
		libevtx_record_values_free(
		 &record_values,
		 NULL );
	}
	return( 0 );
}

/* Tests the libevtx_record_values_get_utf8_source_name function
 * Returns 1 if successful or 0 if not
 */
int evtx_test_record_values_get_utf8_source_name(
     void )
{
	uint8_t utf8_source_name[ 512 ];

	libcerror_error_t *error               = NULL;
	libevtx_record_values_t *record_values = NULL;
	int result                             = 0;
	int utf8_source_name_is_set            = 0;

	/* Initialize test
	 */
	result = libevtx_record_values_initialize(
	          &record_values,
	          &error );

	EVTX_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVTX_TEST_ASSERT_IS_NOT_NULL(
	 "record_values",
	 record_values );

	EVTX_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libevtx_record_values_get_utf8_source_name(
	          record_values,
	          utf8_source_name,
	          512,
	          &error );

	EVTX_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EVTX_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	utf8_source_name_is_set = result;

	/* Test error cases
	 */
	result = libevtx_record_values_get_utf8_source_name(
	          NULL,
	          utf8_source_name,
	          512,
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

	if( utf8_source_name_is_set != 0 )
	{
		result = libevtx_record_values_get_utf8_source_name(
		          record_values,
		          NULL,
		          512,
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

		result = libevtx_record_values_get_utf8_source_name(
		          record_values,
		          utf8_source_name,
		          0,
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

		result = libevtx_record_values_get_utf8_source_name(
		          record_values,
		          utf8_source_name,
		          (size_t) SSIZE_MAX + 1,
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
	result = libevtx_record_values_free(
	          &record_values,
	          &error );

	EVTX_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVTX_TEST_ASSERT_IS_NULL(
	 "record_values",
	 record_values );

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
	if( record_values != NULL )
	{
		libevtx_record_values_free(
		 &record_values,
		 NULL );
	}
	return( 0 );
}

/* Tests the libevtx_record_values_get_utf16_source_name_size function
 * Returns 1 if successful or 0 if not
 */
int evtx_test_record_values_get_utf16_source_name_size(
     void )
{
	libcerror_error_t *error               = NULL;
	libevtx_record_values_t *record_values = NULL;
	size_t utf16_source_name_size          = 0;
	int result                             = 0;
	int utf16_source_name_size_is_set      = 0;

	/* Initialize test
	 */
	result = libevtx_record_values_initialize(
	          &record_values,
	          &error );

	EVTX_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVTX_TEST_ASSERT_IS_NOT_NULL(
	 "record_values",
	 record_values );

	EVTX_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libevtx_record_values_get_utf16_source_name_size(
	          record_values,
	          &utf16_source_name_size,
	          &error );

	EVTX_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EVTX_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	utf16_source_name_size_is_set = result;

	/* Test error cases
	 */
	result = libevtx_record_values_get_utf16_source_name_size(
	          NULL,
	          &utf16_source_name_size,
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

	if( utf16_source_name_size_is_set != 0 )
	{
		result = libevtx_record_values_get_utf16_source_name_size(
		          record_values,
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
	result = libevtx_record_values_free(
	          &record_values,
	          &error );

	EVTX_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVTX_TEST_ASSERT_IS_NULL(
	 "record_values",
	 record_values );

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
	if( record_values != NULL )
	{
		libevtx_record_values_free(
		 &record_values,
		 NULL );
	}
	return( 0 );
}

/* Tests the libevtx_record_values_get_utf16_source_name function
 * Returns 1 if successful or 0 if not
 */
int evtx_test_record_values_get_utf16_source_name(
     void )
{
	uint16_t utf16_source_name[ 512 ];

	libcerror_error_t *error               = NULL;
	libevtx_record_values_t *record_values = NULL;
	int result                             = 0;
	int utf16_source_name_is_set           = 0;

	/* Initialize test
	 */
	result = libevtx_record_values_initialize(
	          &record_values,
	          &error );

	EVTX_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVTX_TEST_ASSERT_IS_NOT_NULL(
	 "record_values",
	 record_values );

	EVTX_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libevtx_record_values_get_utf16_source_name(
	          record_values,
	          utf16_source_name,
	          512,
	          &error );

	EVTX_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EVTX_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	utf16_source_name_is_set = result;

	/* Test error cases
	 */
	result = libevtx_record_values_get_utf16_source_name(
	          NULL,
	          utf16_source_name,
	          512,
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

	if( utf16_source_name_is_set != 0 )
	{
		result = libevtx_record_values_get_utf16_source_name(
		          record_values,
		          NULL,
		          512,
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

		result = libevtx_record_values_get_utf16_source_name(
		          record_values,
		          utf16_source_name,
		          0,
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

		result = libevtx_record_values_get_utf16_source_name(
		          record_values,
		          utf16_source_name,
		          (size_t) SSIZE_MAX + 1,
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
	result = libevtx_record_values_free(
	          &record_values,
	          &error );

	EVTX_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVTX_TEST_ASSERT_IS_NULL(
	 "record_values",
	 record_values );

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
	if( record_values != NULL )
	{
		libevtx_record_values_free(
		 &record_values,
		 NULL );
	}
	return( 0 );
}

/* Tests the libevtx_record_values_get_utf8_computer_name_size function
 * Returns 1 if successful or 0 if not
 */
int evtx_test_record_values_get_utf8_computer_name_size(
     void )
{
	libcerror_error_t *error               = NULL;
	libevtx_record_values_t *record_values = NULL;
	size_t utf8_computer_name_size         = 0;
	int result                             = 0;
	int utf8_computer_name_size_is_set     = 0;

	/* Initialize test
	 */
	result = libevtx_record_values_initialize(
	          &record_values,
	          &error );

	EVTX_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVTX_TEST_ASSERT_IS_NOT_NULL(
	 "record_values",
	 record_values );

	EVTX_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libevtx_record_values_get_utf8_computer_name_size(
	          record_values,
	          &utf8_computer_name_size,
	          &error );

	EVTX_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EVTX_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	utf8_computer_name_size_is_set = result;

	/* Test error cases
	 */
	result = libevtx_record_values_get_utf8_computer_name_size(
	          NULL,
	          &utf8_computer_name_size,
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

	if( utf8_computer_name_size_is_set != 0 )
	{
		result = libevtx_record_values_get_utf8_computer_name_size(
		          record_values,
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
	result = libevtx_record_values_free(
	          &record_values,
	          &error );

	EVTX_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVTX_TEST_ASSERT_IS_NULL(
	 "record_values",
	 record_values );

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
	if( record_values != NULL )
	{
		libevtx_record_values_free(
		 &record_values,
		 NULL );
	}
	return( 0 );
}

/* Tests the libevtx_record_values_get_utf8_computer_name function
 * Returns 1 if successful or 0 if not
 */
int evtx_test_record_values_get_utf8_computer_name(
     void )
{
	uint8_t utf8_computer_name[ 512 ];

	libcerror_error_t *error               = NULL;
	libevtx_record_values_t *record_values = NULL;
	int result                             = 0;
	int utf8_computer_name_is_set          = 0;

	/* Initialize test
	 */
	result = libevtx_record_values_initialize(
	          &record_values,
	          &error );

	EVTX_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVTX_TEST_ASSERT_IS_NOT_NULL(
	 "record_values",
	 record_values );

	EVTX_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libevtx_record_values_get_utf8_computer_name(
	          record_values,
	          utf8_computer_name,
	          512,
	          &error );

	EVTX_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EVTX_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	utf8_computer_name_is_set = result;

	/* Test error cases
	 */
	result = libevtx_record_values_get_utf8_computer_name(
	          NULL,
	          utf8_computer_name,
	          512,
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

	if( utf8_computer_name_is_set != 0 )
	{
		result = libevtx_record_values_get_utf8_computer_name(
		          record_values,
		          NULL,
		          512,
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

		result = libevtx_record_values_get_utf8_computer_name(
		          record_values,
		          utf8_computer_name,
		          0,
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

		result = libevtx_record_values_get_utf8_computer_name(
		          record_values,
		          utf8_computer_name,
		          (size_t) SSIZE_MAX + 1,
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
	result = libevtx_record_values_free(
	          &record_values,
	          &error );

	EVTX_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVTX_TEST_ASSERT_IS_NULL(
	 "record_values",
	 record_values );

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
	if( record_values != NULL )
	{
		libevtx_record_values_free(
		 &record_values,
		 NULL );
	}
	return( 0 );
}

/* Tests the libevtx_record_values_get_utf16_computer_name_size function
 * Returns 1 if successful or 0 if not
 */
int evtx_test_record_values_get_utf16_computer_name_size(
     void )
{
	libcerror_error_t *error               = NULL;
	libevtx_record_values_t *record_values = NULL;
	size_t utf16_computer_name_size        = 0;
	int result                             = 0;
	int utf16_computer_name_size_is_set    = 0;

	/* Initialize test
	 */
	result = libevtx_record_values_initialize(
	          &record_values,
	          &error );

	EVTX_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVTX_TEST_ASSERT_IS_NOT_NULL(
	 "record_values",
	 record_values );

	EVTX_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libevtx_record_values_get_utf16_computer_name_size(
	          record_values,
	          &utf16_computer_name_size,
	          &error );

	EVTX_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EVTX_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	utf16_computer_name_size_is_set = result;

	/* Test error cases
	 */
	result = libevtx_record_values_get_utf16_computer_name_size(
	          NULL,
	          &utf16_computer_name_size,
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

	if( utf16_computer_name_size_is_set != 0 )
	{
		result = libevtx_record_values_get_utf16_computer_name_size(
		          record_values,
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
	result = libevtx_record_values_free(
	          &record_values,
	          &error );

	EVTX_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVTX_TEST_ASSERT_IS_NULL(
	 "record_values",
	 record_values );

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
	if( record_values != NULL )
	{
		libevtx_record_values_free(
		 &record_values,
		 NULL );
	}
	return( 0 );
}

/* Tests the libevtx_record_values_get_utf16_computer_name function
 * Returns 1 if successful or 0 if not
 */
int evtx_test_record_values_get_utf16_computer_name(
     void )
{
	uint16_t utf16_computer_name[ 512 ];

	libcerror_error_t *error               = NULL;
	libevtx_record_values_t *record_values = NULL;
	int result                             = 0;
	int utf16_computer_name_is_set         = 0;

	/* Initialize test
	 */
	result = libevtx_record_values_initialize(
	          &record_values,
	          &error );

	EVTX_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVTX_TEST_ASSERT_IS_NOT_NULL(
	 "record_values",
	 record_values );

	EVTX_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libevtx_record_values_get_utf16_computer_name(
	          record_values,
	          utf16_computer_name,
	          512,
	          &error );

	EVTX_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EVTX_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	utf16_computer_name_is_set = result;

	/* Test error cases
	 */
	result = libevtx_record_values_get_utf16_computer_name(
	          NULL,
	          utf16_computer_name,
	          512,
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

	if( utf16_computer_name_is_set != 0 )
	{
		result = libevtx_record_values_get_utf16_computer_name(
		          record_values,
		          NULL,
		          512,
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

		result = libevtx_record_values_get_utf16_computer_name(
		          record_values,
		          utf16_computer_name,
		          0,
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

		result = libevtx_record_values_get_utf16_computer_name(
		          record_values,
		          utf16_computer_name,
		          (size_t) SSIZE_MAX + 1,
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
	result = libevtx_record_values_free(
	          &record_values,
	          &error );

	EVTX_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVTX_TEST_ASSERT_IS_NULL(
	 "record_values",
	 record_values );

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
	if( record_values != NULL )
	{
		libevtx_record_values_free(
		 &record_values,
		 NULL );
	}
	return( 0 );
}

/* Tests the libevtx_record_values_get_utf8_user_security_identifier_size function
 * Returns 1 if successful or 0 if not
 */
int evtx_test_record_values_get_utf8_user_security_identifier_size(
     void )
{
	libcerror_error_t *error                      = NULL;
	libevtx_record_values_t *record_values        = NULL;
	size_t utf8_user_security_identifier_size     = 0;
	int result                                    = 0;
	int utf8_user_security_identifier_size_is_set = 0;

	/* Initialize test
	 */
	result = libevtx_record_values_initialize(
	          &record_values,
	          &error );

	EVTX_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVTX_TEST_ASSERT_IS_NOT_NULL(
	 "record_values",
	 record_values );

	EVTX_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libevtx_record_values_get_utf8_user_security_identifier_size(
	          record_values,
	          &utf8_user_security_identifier_size,
	          &error );

	EVTX_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EVTX_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	utf8_user_security_identifier_size_is_set = result;

	/* Test error cases
	 */
	result = libevtx_record_values_get_utf8_user_security_identifier_size(
	          NULL,
	          &utf8_user_security_identifier_size,
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

	if( utf8_user_security_identifier_size_is_set != 0 )
	{
		result = libevtx_record_values_get_utf8_user_security_identifier_size(
		          record_values,
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
	result = libevtx_record_values_free(
	          &record_values,
	          &error );

	EVTX_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVTX_TEST_ASSERT_IS_NULL(
	 "record_values",
	 record_values );

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
	if( record_values != NULL )
	{
		libevtx_record_values_free(
		 &record_values,
		 NULL );
	}
	return( 0 );
}

/* Tests the libevtx_record_values_get_utf8_user_security_identifier function
 * Returns 1 if successful or 0 if not
 */
int evtx_test_record_values_get_utf8_user_security_identifier(
     void )
{
	uint8_t utf8_user_security_identifier[ 512 ];

	libcerror_error_t *error                 = NULL;
	libevtx_record_values_t *record_values   = NULL;
	int result                               = 0;
	int utf8_user_security_identifier_is_set = 0;

	/* Initialize test
	 */
	result = libevtx_record_values_initialize(
	          &record_values,
	          &error );

	EVTX_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVTX_TEST_ASSERT_IS_NOT_NULL(
	 "record_values",
	 record_values );

	EVTX_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libevtx_record_values_get_utf8_user_security_identifier(
	          record_values,
	          utf8_user_security_identifier,
	          512,
	          &error );

	EVTX_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EVTX_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	utf8_user_security_identifier_is_set = result;

	/* Test error cases
	 */
	result = libevtx_record_values_get_utf8_user_security_identifier(
	          NULL,
	          utf8_user_security_identifier,
	          512,
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

	if( utf8_user_security_identifier_is_set != 0 )
	{
		result = libevtx_record_values_get_utf8_user_security_identifier(
		          record_values,
		          NULL,
		          512,
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

		result = libevtx_record_values_get_utf8_user_security_identifier(
		          record_values,
		          utf8_user_security_identifier,
		          0,
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

		result = libevtx_record_values_get_utf8_user_security_identifier(
		          record_values,
		          utf8_user_security_identifier,
		          (size_t) SSIZE_MAX + 1,
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
	result = libevtx_record_values_free(
	          &record_values,
	          &error );

	EVTX_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVTX_TEST_ASSERT_IS_NULL(
	 "record_values",
	 record_values );

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
	if( record_values != NULL )
	{
		libevtx_record_values_free(
		 &record_values,
		 NULL );
	}
	return( 0 );
}

/* Tests the libevtx_record_values_get_utf16_user_security_identifier_size function
 * Returns 1 if successful or 0 if not
 */
int evtx_test_record_values_get_utf16_user_security_identifier_size(
     void )
{
	libcerror_error_t *error                       = NULL;
	libevtx_record_values_t *record_values         = NULL;
	size_t utf16_user_security_identifier_size     = 0;
	int result                                     = 0;
	int utf16_user_security_identifier_size_is_set = 0;

	/* Initialize test
	 */
	result = libevtx_record_values_initialize(
	          &record_values,
	          &error );

	EVTX_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVTX_TEST_ASSERT_IS_NOT_NULL(
	 "record_values",
	 record_values );

	EVTX_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libevtx_record_values_get_utf16_user_security_identifier_size(
	          record_values,
	          &utf16_user_security_identifier_size,
	          &error );

	EVTX_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EVTX_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	utf16_user_security_identifier_size_is_set = result;

	/* Test error cases
	 */
	result = libevtx_record_values_get_utf16_user_security_identifier_size(
	          NULL,
	          &utf16_user_security_identifier_size,
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

	if( utf16_user_security_identifier_size_is_set != 0 )
	{
		result = libevtx_record_values_get_utf16_user_security_identifier_size(
		          record_values,
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
	result = libevtx_record_values_free(
	          &record_values,
	          &error );

	EVTX_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVTX_TEST_ASSERT_IS_NULL(
	 "record_values",
	 record_values );

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
	if( record_values != NULL )
	{
		libevtx_record_values_free(
		 &record_values,
		 NULL );
	}
	return( 0 );
}

/* Tests the libevtx_record_values_get_utf16_user_security_identifier function
 * Returns 1 if successful or 0 if not
 */
int evtx_test_record_values_get_utf16_user_security_identifier(
     void )
{
	uint16_t utf16_user_security_identifier[ 512 ];

	libcerror_error_t *error                  = NULL;
	libevtx_record_values_t *record_values    = NULL;
	int result                                = 0;
	int utf16_user_security_identifier_is_set = 0;

	/* Initialize test
	 */
	result = libevtx_record_values_initialize(
	          &record_values,
	          &error );

	EVTX_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVTX_TEST_ASSERT_IS_NOT_NULL(
	 "record_values",
	 record_values );

	EVTX_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libevtx_record_values_get_utf16_user_security_identifier(
	          record_values,
	          utf16_user_security_identifier,
	          512,
	          &error );

	EVTX_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EVTX_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	utf16_user_security_identifier_is_set = result;

	/* Test error cases
	 */
	result = libevtx_record_values_get_utf16_user_security_identifier(
	          NULL,
	          utf16_user_security_identifier,
	          512,
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

	if( utf16_user_security_identifier_is_set != 0 )
	{
		result = libevtx_record_values_get_utf16_user_security_identifier(
		          record_values,
		          NULL,
		          512,
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

		result = libevtx_record_values_get_utf16_user_security_identifier(
		          record_values,
		          utf16_user_security_identifier,
		          0,
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

		result = libevtx_record_values_get_utf16_user_security_identifier(
		          record_values,
		          utf16_user_security_identifier,
		          (size_t) SSIZE_MAX + 1,
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
	result = libevtx_record_values_free(
	          &record_values,
	          &error );

	EVTX_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVTX_TEST_ASSERT_IS_NULL(
	 "record_values",
	 record_values );

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
	if( record_values != NULL )
	{
		libevtx_record_values_free(
		 &record_values,
		 NULL );
	}
	return( 0 );
}

/* Tests the libevtx_record_values_get_utf8_xml_string_size function
 * Returns 1 if successful or 0 if not
 */
int evtx_test_record_values_get_utf8_xml_string_size(
     void )
{
	libcerror_error_t *error               = NULL;
	libevtx_record_values_t *record_values = NULL;
	size_t utf8_xml_string_size            = 0;
	int result                             = 0;
	int utf8_xml_string_size_is_set        = 0;

	/* Initialize test
	 */
	result = libevtx_record_values_initialize(
	          &record_values,
	          &error );

	EVTX_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVTX_TEST_ASSERT_IS_NOT_NULL(
	 "record_values",
	 record_values );

	EVTX_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libevtx_record_values_get_utf8_xml_string_size(
	          record_values,
	          &utf8_xml_string_size,
	          &error );

	EVTX_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EVTX_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	utf8_xml_string_size_is_set = result;

	/* Test error cases
	 */
	result = libevtx_record_values_get_utf8_xml_string_size(
	          NULL,
	          &utf8_xml_string_size,
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

	if( utf8_xml_string_size_is_set != 0 )
	{
		result = libevtx_record_values_get_utf8_xml_string_size(
		          record_values,
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
	result = libevtx_record_values_free(
	          &record_values,
	          &error );

	EVTX_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVTX_TEST_ASSERT_IS_NULL(
	 "record_values",
	 record_values );

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
	if( record_values != NULL )
	{
		libevtx_record_values_free(
		 &record_values,
		 NULL );
	}
	return( 0 );
}

/* Tests the libevtx_record_values_get_utf8_xml_string function
 * Returns 1 if successful or 0 if not
 */
int evtx_test_record_values_get_utf8_xml_string(
     void )
{
	uint8_t utf8_xml_string[ 512 ];

	libcerror_error_t *error               = NULL;
	libevtx_record_values_t *record_values = NULL;
	int result                             = 0;
	int utf8_xml_string_is_set             = 0;

	/* Initialize test
	 */
	result = libevtx_record_values_initialize(
	          &record_values,
	          &error );

	EVTX_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVTX_TEST_ASSERT_IS_NOT_NULL(
	 "record_values",
	 record_values );

	EVTX_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libevtx_record_values_get_utf8_xml_string(
	          record_values,
	          utf8_xml_string,
	          512,
	          &error );

	EVTX_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EVTX_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	utf8_xml_string_is_set = result;

	/* Test error cases
	 */
	result = libevtx_record_values_get_utf8_xml_string(
	          NULL,
	          utf8_xml_string,
	          512,
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

	if( utf8_xml_string_is_set != 0 )
	{
		result = libevtx_record_values_get_utf8_xml_string(
		          record_values,
		          NULL,
		          512,
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

		result = libevtx_record_values_get_utf8_xml_string(
		          record_values,
		          utf8_xml_string,
		          0,
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

		result = libevtx_record_values_get_utf8_xml_string(
		          record_values,
		          utf8_xml_string,
		          (size_t) SSIZE_MAX + 1,
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
	result = libevtx_record_values_free(
	          &record_values,
	          &error );

	EVTX_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVTX_TEST_ASSERT_IS_NULL(
	 "record_values",
	 record_values );

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
	if( record_values != NULL )
	{
		libevtx_record_values_free(
		 &record_values,
		 NULL );
	}
	return( 0 );
}

/* Tests the libevtx_record_values_get_utf16_xml_string_size function
 * Returns 1 if successful or 0 if not
 */
int evtx_test_record_values_get_utf16_xml_string_size(
     void )
{
	libcerror_error_t *error               = NULL;
	libevtx_record_values_t *record_values = NULL;
	size_t utf16_xml_string_size           = 0;
	int result                             = 0;
	int utf16_xml_string_size_is_set       = 0;

	/* Initialize test
	 */
	result = libevtx_record_values_initialize(
	          &record_values,
	          &error );

	EVTX_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVTX_TEST_ASSERT_IS_NOT_NULL(
	 "record_values",
	 record_values );

	EVTX_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libevtx_record_values_get_utf16_xml_string_size(
	          record_values,
	          &utf16_xml_string_size,
	          &error );

	EVTX_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EVTX_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	utf16_xml_string_size_is_set = result;

	/* Test error cases
	 */
	result = libevtx_record_values_get_utf16_xml_string_size(
	          NULL,
	          &utf16_xml_string_size,
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

	if( utf16_xml_string_size_is_set != 0 )
	{
		result = libevtx_record_values_get_utf16_xml_string_size(
		          record_values,
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
	result = libevtx_record_values_free(
	          &record_values,
	          &error );

	EVTX_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVTX_TEST_ASSERT_IS_NULL(
	 "record_values",
	 record_values );

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
	if( record_values != NULL )
	{
		libevtx_record_values_free(
		 &record_values,
		 NULL );
	}
	return( 0 );
}

/* Tests the libevtx_record_values_get_utf16_xml_string function
 * Returns 1 if successful or 0 if not
 */
int evtx_test_record_values_get_utf16_xml_string(
     void )
{
	uint16_t utf16_xml_string[ 512 ];

	libcerror_error_t *error               = NULL;
	libevtx_record_values_t *record_values = NULL;
	int result                             = 0;
	int utf16_xml_string_is_set            = 0;

	/* Initialize test
	 */
	result = libevtx_record_values_initialize(
	          &record_values,
	          &error );

	EVTX_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVTX_TEST_ASSERT_IS_NOT_NULL(
	 "record_values",
	 record_values );

	EVTX_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libevtx_record_values_get_utf16_xml_string(
	          record_values,
	          utf16_xml_string,
	          512,
	          &error );

	EVTX_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EVTX_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	utf16_xml_string_is_set = result;

	/* Test error cases
	 */
	result = libevtx_record_values_get_utf16_xml_string(
	          NULL,
	          utf16_xml_string,
	          512,
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

	if( utf16_xml_string_is_set != 0 )
	{
		result = libevtx_record_values_get_utf16_xml_string(
		          record_values,
		          NULL,
		          512,
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

		result = libevtx_record_values_get_utf16_xml_string(
		          record_values,
		          utf16_xml_string,
		          0,
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

		result = libevtx_record_values_get_utf16_xml_string(
		          record_values,
		          utf16_xml_string,
		          (size_t) SSIZE_MAX + 1,
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
	result = libevtx_record_values_free(
	          &record_values,
	          &error );

	EVTX_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVTX_TEST_ASSERT_IS_NULL(
	 "record_values",
	 record_values );

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
	if( record_values != NULL )
	{
		libevtx_record_values_free(
		 &record_values,
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

#if defined( __GNUC__ ) && defined( TODO )

	EVTX_TEST_RUN(
	 "libevtx_record_values_initialize",
	 evtx_test_record_values_initialize );

	EVTX_TEST_RUN(
	 "libevtx_record_values_free",
	 evtx_test_record_values_free );

	EVTX_TEST_RUN(
	 "libevtx_record_values_clone",
	 evtx_test_record_values_clone );

	/* TODO: add tests for libevtx_record_values_read_header */

	/* TODO: add tests for libevtx_record_values_read_xml_document */

	EVTX_TEST_RUN(
	 "libevtx_record_values_get_event_identifier",
	 evtx_test_record_values_get_event_identifier );

	EVTX_TEST_RUN(
	 "libevtx_record_values_get_event_identifier_qualifiers",
	 evtx_test_record_values_get_event_identifier_qualifiers );

	EVTX_TEST_RUN(
	 "libevtx_record_values_get_event_level",
	 evtx_test_record_values_get_event_level );

	EVTX_TEST_RUN(
	 "libevtx_record_values_get_utf8_provider_identifier_size",
	 evtx_test_record_values_get_utf8_provider_identifier_size );

	EVTX_TEST_RUN(
	 "libevtx_record_values_get_utf8_provider_identifier",
	 evtx_test_record_values_get_utf8_provider_identifier );

	EVTX_TEST_RUN(
	 "libevtx_record_values_get_utf16_provider_identifier_size",
	 evtx_test_record_values_get_utf16_provider_identifier_size );

	EVTX_TEST_RUN(
	 "libevtx_record_values_get_utf16_provider_identifier",
	 evtx_test_record_values_get_utf16_provider_identifier );

	EVTX_TEST_RUN(
	 "libevtx_record_values_get_utf8_source_name_size",
	 evtx_test_record_values_get_utf8_source_name_size );

	EVTX_TEST_RUN(
	 "libevtx_record_values_get_utf8_source_name",
	 evtx_test_record_values_get_utf8_source_name );

	EVTX_TEST_RUN(
	 "libevtx_record_values_get_utf16_source_name_size",
	 evtx_test_record_values_get_utf16_source_name_size );

	EVTX_TEST_RUN(
	 "libevtx_record_values_get_utf16_source_name",
	 evtx_test_record_values_get_utf16_source_name );

	EVTX_TEST_RUN(
	 "libevtx_record_values_get_utf8_computer_name_size",
	 evtx_test_record_values_get_utf8_computer_name_size );

	EVTX_TEST_RUN(
	 "libevtx_record_values_get_utf8_computer_name",
	 evtx_test_record_values_get_utf8_computer_name );

	EVTX_TEST_RUN(
	 "libevtx_record_values_get_utf16_computer_name_size",
	 evtx_test_record_values_get_utf16_computer_name_size );

	EVTX_TEST_RUN(
	 "libevtx_record_values_get_utf16_computer_name",
	 evtx_test_record_values_get_utf16_computer_name );

	EVTX_TEST_RUN(
	 "libevtx_record_values_get_utf8_user_security_identifier_size",
	 evtx_test_record_values_get_utf8_user_security_identifier_size );

	EVTX_TEST_RUN(
	 "libevtx_record_values_get_utf8_user_security_identifier",
	 evtx_test_record_values_get_utf8_user_security_identifier );

	EVTX_TEST_RUN(
	 "libevtx_record_values_get_utf16_user_security_identifier_size",
	 evtx_test_record_values_get_utf16_user_security_identifier_size );

	EVTX_TEST_RUN(
	 "libevtx_record_values_get_utf16_user_security_identifier",
	 evtx_test_record_values_get_utf16_user_security_identifier );

	/* TODO: add tests for libevtx_record_values_parse_data */

	/* TODO: add tests for libevtx_record_values_get_number_of_strings */

	/* TODO: add tests for libevtx_record_values_get_utf8_string_size */

	/* TODO: add tests for libevtx_record_values_get_utf8_string */

	/* TODO: add tests for libevtx_record_values_get_utf16_string_size */

	/* TODO: add tests for libevtx_record_values_get_utf16_string */

	/* TODO: add tests for libevtx_record_values_get_data_size */

	/* TODO: add tests for libevtx_record_values_get_data */

	EVTX_TEST_RUN(
	 "libevtx_record_values_get_utf8_xml_string_size",
	 evtx_test_record_values_get_utf8_xml_string_size );

	EVTX_TEST_RUN(
	 "libevtx_record_values_get_utf8_xml_string",
	 evtx_test_record_values_get_utf8_xml_string );

	EVTX_TEST_RUN(
	 "libevtx_record_values_get_utf16_xml_string_size",
	 evtx_test_record_values_get_utf16_xml_string_size );

	EVTX_TEST_RUN(
	 "libevtx_record_values_get_utf16_xml_string",
	 evtx_test_record_values_get_utf16_xml_string );

#endif /* defined( __GNUC__ ) && defined( TODO ) */

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

