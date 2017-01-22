/*
 * Library record type test program
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

#include "../libevtx/libevtx_record.h"

/* Tests the libevtx_record_free function
 * Returns 1 if successful or 0 if not
 */
int evtx_test_record_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libevtx_record_free(
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

#if defined( __GNUC__ )

	/* TODO: add tests for libevtx_record_initialize */

#endif /* defined( __GNUC__ ) */

	EVTX_TEST_RUN(
	 "libevtx_record_free",
	 evtx_test_record_free );

#if defined( __GNUC__ )

	/* TODO: add tests for libevtx_record_get_offset */

	/* TODO: add tests for libevtx_record_get_identifier */

	/* TODO: add tests for libevtx_record_get_written_time */

	/* TODO: add tests for libevtx_record_get_event_identifier */

	/* TODO: add tests for libevtx_record_get_event_identifier_qualifiers */

	/* TODO: add tests for libevtx_record_get_event_level */

	/* TODO: add tests for libevtx_record_get_utf8_provider_identifier_size */

	/* TODO: add tests for libevtx_record_get_utf8_provider_identifier */

	/* TODO: add tests for libevtx_record_get_utf16_provider_identifier_size */

	/* TODO: add tests for libevtx_record_get_utf16_provider_identifier */

	/* TODO: add tests for libevtx_record_get_utf8_source_name_size */

	/* TODO: add tests for libevtx_record_get_utf8_source_name */

	/* TODO: add tests for libevtx_record_get_utf16_source_name_size */

	/* TODO: add tests for libevtx_record_get_utf16_source_name */

	/* TODO: add tests for libevtx_record_get_utf8_computer_name_size */

	/* TODO: add tests for libevtx_record_get_utf8_computer_name */

	/* TODO: add tests for libevtx_record_get_utf16_computer_name_size */

	/* TODO: add tests for libevtx_record_get_utf16_computer_name */

	/* TODO: add tests for libevtx_record_get_utf8_user_security_identifier_size */

	/* TODO: add tests for libevtx_record_get_utf8_user_security_identifier */

	/* TODO: add tests for libevtx_record_get_utf16_user_security_identifier_size */

	/* TODO: add tests for libevtx_record_get_utf16_user_security_identifier */

	/* TODO: add tests for libevtx_record_parse_data_with_template_definition */

	/* TODO: add tests for libevtx_record_get_number_of_strings */

	/* TODO: add tests for libevtx_record_get_utf8_string_size */

	/* TODO: add tests for libevtx_record_get_utf8_string */

	/* TODO: add tests for libevtx_record_get_utf16_string_size */

	/* TODO: add tests for libevtx_record_get_utf16_string */

	/* TODO: add tests for libevtx_record_get_data_size */

	/* TODO: add tests for libevtx_record_get_data */

	/* TODO: add tests for libevtx_record_get_utf8_xml_string_size */

	/* TODO: add tests for libevtx_record_get_utf8_xml_string */

	/* TODO: add tests for libevtx_record_get_utf16_xml_string_size */

	/* TODO: add tests for libevtx_record_get_utf16_xml_string */

#endif /* defined( __GNUC__ ) */

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

