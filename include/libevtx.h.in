/*
 * Library to access the Windows XML Event Log (EVTX) format
 *
 * Copyright (C) 2011-2017, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This software is free software: you can redistribute it and/or modisoy
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

#if !defined( _LIBEVTX_H )
#define _LIBEVTX_H

#include <libevtx/codepage.h>
#include <libevtx/definitions.h>
#include <libevtx/error.h>
#include <libevtx/extern.h>
#include <libevtx/features.h>
#include <libevtx/types.h>

#include <stdio.h>

#if defined( LIBEVTX_HAVE_BFIO )
#include <libbfio.h>
#endif

#if defined( __cplusplus )
extern "C" {
#endif

/* -------------------------------------------------------------------------
 * Support functions
 * ------------------------------------------------------------------------- */

/* Returns the library version
 */
LIBEVTX_EXTERN \
const char *libevtx_get_version(
             void );

/* Returns the access flags for reading
 */
LIBEVTX_EXTERN \
int libevtx_get_access_flags_read(
     void );

/* Retrieves the narrow system string codepage
 * A value of 0 represents no codepage, UTF-8 encoding is used instead
 * Returns 1 if successful or -1 on error
 */
LIBEVTX_EXTERN \
int libevtx_get_codepage(
     int *codepage,
     libevtx_error_t **error );

/* Sets the narrow system string codepage
 * A value of 0 represents no codepage, UTF-8 encoding is used instead
 * Returns 1 if successful or -1 on error
 */
LIBEVTX_EXTERN \
int libevtx_set_codepage(
     int codepage,
     libevtx_error_t **error );

/* Determines if a file contains an EVTX file signature
 * Returns 1 if true, 0 if not or -1 on error
 */
LIBEVTX_EXTERN \
int libevtx_check_file_signature(
     const char *filename,
     libevtx_error_t **error );

#if defined( LIBEVTX_HAVE_WIDE_CHARACTER_TYPE )

/* Determines if a file contains an EVTX file signature
 * Returns 1 if true, 0 if not or -1 on error
 */
LIBEVTX_EXTERN \
int libevtx_check_file_signature_wide(
     const wchar_t *filename,
     libevtx_error_t **error );

#endif /* defined( LIBEVTX_HAVE_WIDE_CHARACTER_TYPE ) */

#if defined( LIBEVTX_HAVE_BFIO )

/* Determines if a file contains an EVTX file signature using a Basic File IO (bfio) handle
 * Returns 1 if true, 0 if not or -1 on error
 */
LIBEVTX_EXTERN \
int libevtx_check_file_signature_file_io_handle(
     libbfio_handle_t *bfio_handle,
     libevtx_error_t **error );

#endif /* defined( LIBEVTX_HAVE_BFIO ) */

/* -------------------------------------------------------------------------
 * Notify functions
 * ------------------------------------------------------------------------- */

/* Sets the verbose notification
 */
LIBEVTX_EXTERN \
void libevtx_notify_set_verbose(
      int verbose );

/* Sets the notification stream
 * Returns 1 if successful or -1 on error
 */
LIBEVTX_EXTERN \
int libevtx_notify_set_stream(
     FILE *stream,
     libevtx_error_t **error );

/* Opens the notification stream using a filename
 * The stream is opened in append mode
 * Returns 1 if successful or -1 on error
 */
LIBEVTX_EXTERN \
int libevtx_notify_stream_open(
     const char *filename,
     libevtx_error_t **error );

/* Closes the notification stream if opened using a filename
 * Returns 0 if successful or -1 on error
 */
LIBEVTX_EXTERN \
int libevtx_notify_stream_close(
     libevtx_error_t **error );

/* -------------------------------------------------------------------------
 * Error functions
 * ------------------------------------------------------------------------- */

/* Frees an error
 */
LIBEVTX_EXTERN \
void libevtx_error_free(
      libevtx_error_t **error );

/* Prints a descriptive string of the error to the stream
 * Returns the number of printed characters if successful or -1 on error
 */
LIBEVTX_EXTERN \
int libevtx_error_fprint(
     libevtx_error_t *error,
     FILE *stream );

/* Prints a descriptive string of the error to the string
 * The end-of-string character is not included in the return value
 * Returns the number of printed characters if successful or -1 on error
 */
LIBEVTX_EXTERN \
int libevtx_error_sprint(
     libevtx_error_t *error,
     char *string,
     size_t size );

/* Prints a backtrace of the error to the stream
 * Returns the number of printed characters if successful or -1 on error
 */
LIBEVTX_EXTERN \
int libevtx_error_backtrace_fprint(
     libevtx_error_t *error,
     FILE *stream );

/* Prints a backtrace of the error to the string
 * The end-of-string character is not included in the return value
 * Returns the number of printed characters if successful or -1 on error
 */
LIBEVTX_EXTERN \
int libevtx_error_backtrace_sprint(
     libevtx_error_t *error,
     char *string,
     size_t size );

/* -------------------------------------------------------------------------
 * File functions
 * ------------------------------------------------------------------------- */

/* Creates a file
 * Make sure the value file is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
LIBEVTX_EXTERN \
int libevtx_file_initialize(
     libevtx_file_t **file,
     libevtx_error_t **error );

/* Frees a file
 * Returns 1 if successful or -1 on error
 */
LIBEVTX_EXTERN \
int libevtx_file_free(
     libevtx_file_t **file,
     libevtx_error_t **error );

/* Signals a file to abort its current activity
 * Returns 1 if successful or -1 on error
 */
LIBEVTX_EXTERN \
int libevtx_file_signal_abort(
     libevtx_file_t *file,
     libevtx_error_t **error );

/* Opens a file
 * Returns 1 if successful or -1 on error
 */
LIBEVTX_EXTERN \
int libevtx_file_open(
     libevtx_file_t *file,
     const char *filename,
     int access_flags,
     libevtx_error_t **error );

#if defined( LIBEVTX_HAVE_WIDE_CHARACTER_TYPE )

/* Opens a file
 * Returns 1 if successful or -1 on error
 */
LIBEVTX_EXTERN \
int libevtx_file_open_wide(
     libevtx_file_t *file,
     const wchar_t *filename,
     int access_flags,
     libevtx_error_t **error );

#endif /* defined( LIBEVTX_HAVE_WIDE_CHARACTER_TYPE ) */

#if defined( LIBEVTX_HAVE_BFIO )

/* Opens a file using a Basic File IO (bfio) handle
 * Returns 1 if successful or -1 on error
 */
LIBEVTX_EXTERN \
int libevtx_file_open_file_io_handle(
     libevtx_file_t *file,
     libbfio_handle_t *file_io_handle,
     int access_flags,
     libevtx_error_t **error );

#endif /* defined( LIBEVTX_HAVE_BFIO ) */

/* Closes a file
 * Returns 0 if successful or -1 on error
 */
LIBEVTX_EXTERN \
int libevtx_file_close(
     libevtx_file_t *file,
     libevtx_error_t **error );

/* Determine if the file corrupted
 * Returns 1 if corrupted, 0 if not or -1 on error
 */
LIBEVTX_EXTERN \
int libevtx_file_is_corrupted(
     libevtx_file_t *file,
     libevtx_error_t **error );

/* Retrieves the file ASCII codepage
 * Returns 1 if successful or -1 on error
 */
LIBEVTX_EXTERN \
int libevtx_file_get_ascii_codepage(
     libevtx_file_t *file,
     int *ascii_codepage,
     libevtx_error_t **error );

/* Sets the file ASCII codepage
 * Returns 1 if successful or -1 on error
 */
LIBEVTX_EXTERN \
int libevtx_file_set_ascii_codepage(
     libevtx_file_t *file,
     int ascii_codepage,
     libevtx_error_t **error );

/* Retrieves the format version
 * Returns 1 if successful or -1 on error
 */
LIBEVTX_EXTERN \
int libevtx_file_get_format_version(
     libevtx_file_t *file,
     uint16_t *major_version,
     uint16_t *minor_version,
     libevtx_error_t **error );

/* Retrieves the flags
 * Returns 1 if successful or -1 on error
 */
LIBEVTX_EXTERN \
int libevtx_file_get_flags(
     libevtx_file_t *file,
     uint32_t *flags,
     libevtx_error_t **error );

/* Retrieves the number of records
 * Returns 1 if successful or -1 on error
 */
LIBEVTX_EXTERN \
int libevtx_file_get_number_of_records(
     libevtx_file_t *file,
     int *number_of_records,
     libevtx_error_t **error );

/* Retrieves a specific record
 * Returns 1 if successful or -1 on error
 */
LIBEVTX_EXTERN \
int libevtx_file_get_record(
     libevtx_file_t *file,
     int record_index,
     libevtx_record_t **record,
     libevtx_error_t **error );

/* Retrieves the number of recovered records
 * Returns 1 if successful or -1 on error
 */
LIBEVTX_EXTERN \
int libevtx_file_get_number_of_recovered_records(
     libevtx_file_t *file,
     int *number_of_records,
     libevtx_error_t **error );

/* Retrieves a specific recovered record
 * Returns 1 if successful or -1 on error
 */
LIBEVTX_EXTERN \
int libevtx_file_get_recovered_record(
     libevtx_file_t *file,
     int record_index,
     libevtx_record_t **record,
     libevtx_error_t **error );

/* -------------------------------------------------------------------------
 * File functions - deprecated
 * ------------------------------------------------------------------------- */

/* Retrieves the version
 *
 * This function deprecated use libevtx_file_get_format_version instead
 *
 * Returns 1 if successful or -1 on error
 */
LIBEVTX_DEPRECATED \
LIBEVTX_EXTERN \
int libevtx_file_get_version(
     libevtx_file_t *file,
     uint16_t *major_version,
     uint16_t *minor_version,
     libevtx_error_t **error );

/* -------------------------------------------------------------------------
 * Record functions
 * ------------------------------------------------------------------------- */

/* Frees a record
 * Returns 1 if successful or -1 on error
 */
LIBEVTX_EXTERN \
int libevtx_record_free(
     libevtx_record_t **record,
     libevtx_error_t **error );

/* Retrieves the offset
 * Returns 1 if successful or -1 on error
 */
LIBEVTX_EXTERN \
int libevtx_record_get_offset(
     libevtx_record_t *record,
     off64_t *offset,
     libevtx_error_t **error );

/* Retrieves the identifier (record number)
 * Returns 1 if successful or -1 on error
 */
LIBEVTX_EXTERN \
int libevtx_record_get_identifier(
     libevtx_record_t *record,
     uint64_t *identifier,
     libevtx_error_t **error );

/* Retrieves the 64-bit FILETIME value containing the written time
 * Returns 1 if successful or -1 on error
 */
LIBEVTX_EXTERN \
int libevtx_record_get_written_time(
     libevtx_record_t *record,
     uint64_t *filetime,
     libevtx_error_t **error );

/* Retrieves the event identifier
 * Returns 1 if successful or -1 on error
 */
LIBEVTX_EXTERN \
int libevtx_record_get_event_identifier(
     libevtx_record_t *record,
     uint32_t *event_identifier,
     libevtx_error_t **error );

/* Retrieves the event identifier qualifiers
 * Returns 1 if successful, 0 if not available or -1 on error
 */
LIBEVTX_EXTERN \
int libevtx_record_get_event_identifier_qualifiers(
     libevtx_record_t *record,
     uint32_t *event_identifier_qualifiers,
     libevtx_error_t **error );

/* Retrieves the event level
 * Returns 1 if successful or -1 on error
 */
LIBEVTX_EXTERN \
int libevtx_record_get_event_level(
     libevtx_record_t *record,
     uint8_t *event_level,
     libevtx_error_t **error );

/* Retrieves the size of the UTF-8 encoded provider identifier
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
LIBEVTX_EXTERN \
int libevtx_record_get_utf8_provider_identifier_size(
     libevtx_record_t *record,
     size_t *utf8_string_size,
     libevtx_error_t **error );

/* Retrieves the UTF-8 encoded provider identifier
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
LIBEVTX_EXTERN \
int libevtx_record_get_utf8_provider_identifier(
     libevtx_record_t *record,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libevtx_error_t **error );

/* Retrieves the size of the UTF-16 encoded provider identifier
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
LIBEVTX_EXTERN \
int libevtx_record_get_utf16_provider_identifier_size(
     libevtx_record_t *record,
     size_t *utf16_string_size,
     libevtx_error_t **error );

/* Retrieves the UTF-16 encoded provider identifier
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
LIBEVTX_EXTERN \
int libevtx_record_get_utf16_provider_identifier(
     libevtx_record_t *record,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libevtx_error_t **error );

/* Retrieves the size of the UTF-8 encoded source name
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
LIBEVTX_EXTERN \
int libevtx_record_get_utf8_source_name_size(
     libevtx_record_t *record,
     size_t *utf8_string_size,
     libevtx_error_t **error );

/* Retrieves the UTF-8 encoded source name
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
LIBEVTX_EXTERN \
int libevtx_record_get_utf8_source_name(
     libevtx_record_t *record,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libevtx_error_t **error );

/* Retrieves the size of the UTF-16 encoded source name
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
LIBEVTX_EXTERN \
int libevtx_record_get_utf16_source_name_size(
     libevtx_record_t *record,
     size_t *utf16_string_size,
     libevtx_error_t **error );

/* Retrieves the UTF-16 encoded source name
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
LIBEVTX_EXTERN \
int libevtx_record_get_utf16_source_name(
     libevtx_record_t *record,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libevtx_error_t **error );

/* Retrieves the size of the UTF-8 encoded computer name
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
LIBEVTX_EXTERN \
int libevtx_record_get_utf8_computer_name_size(
     libevtx_record_t *record,
     size_t *utf8_string_size,
     libevtx_error_t **error );

/* Retrieves the UTF-8 encoded computer name
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
LIBEVTX_EXTERN \
int libevtx_record_get_utf8_computer_name(
     libevtx_record_t *record,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libevtx_error_t **error );

/* Retrieves the size of the UTF-16 encoded computer name
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
LIBEVTX_EXTERN \
int libevtx_record_get_utf16_computer_name_size(
     libevtx_record_t *record,
     size_t *utf16_string_size,
     libevtx_error_t **error );

/* Retrieves the UTF-16 encoded computer name
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
LIBEVTX_EXTERN \
int libevtx_record_get_utf16_computer_name(
     libevtx_record_t *record,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libevtx_error_t **error );

/* Retrieves the size of the UTF-8 encoded user security identifier (SID)
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
LIBEVTX_EXTERN \
int libevtx_record_get_utf8_user_security_identifier_size(
     libevtx_record_t *record,
     size_t *utf8_string_size,
     libevtx_error_t **error );

/* Retrieves the UTF-8 encoded user security identifier (SID)
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
LIBEVTX_EXTERN \
int libevtx_record_get_utf8_user_security_identifier(
     libevtx_record_t *record,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libevtx_error_t **error );

/* Retrieves the size of the UTF-16 encoded user security identifier (SID)
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
LIBEVTX_EXTERN \
int libevtx_record_get_utf16_user_security_identifier_size(
     libevtx_record_t *record,
     size_t *utf16_string_size,
     libevtx_error_t **error );

/* Retrieves the UTF-16 encoded user security identifier (SID)
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
LIBEVTX_EXTERN \
int libevtx_record_get_utf16_user_security_identifier(
     libevtx_record_t *record,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libevtx_error_t **error );

/* Parses the record data with a template definition
 * This function needs to be called before accessing the strings otherwise
 * the record data will be parsed without a template definition by default
 * Returns 1 if successful, 0 if data could not be parsed or -1 on error
 */
LIBEVTX_EXTERN \
int libevtx_record_parse_data_with_template_definition(
     libevtx_record_t *record,
     libevtx_template_definition_t *template_definition,
     libevtx_error_t **error );

/* Retrieves the number of strings
 * Returns 1 if successful or -1 on error
 */
LIBEVTX_EXTERN \
int libevtx_record_get_number_of_strings(
     libevtx_record_t *record,
     int *number_of_strings,
     libevtx_error_t **error );

/* Retrieves the size of a specific UTF-8 encoded string
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBEVTX_EXTERN \
int libevtx_record_get_utf8_string_size(
     libevtx_record_t *record,
     int string_index,
     size_t *utf8_string_size,
     libevtx_error_t **error );

/* Retrieves a specific UTF-8 encoded string
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBEVTX_EXTERN \
int libevtx_record_get_utf8_string(
     libevtx_record_t *record,
     int string_index,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libevtx_error_t **error );

/* Retrieves the size of a specific UTF-16 encoded string
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBEVTX_EXTERN \
int libevtx_record_get_utf16_string_size(
     libevtx_record_t *record,
     int string_index,
     size_t *utf16_string_size,
     libevtx_error_t **error );

/* Retrieves a specific UTF-16 encoded string
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBEVTX_EXTERN \
int libevtx_record_get_utf16_string(
     libevtx_record_t *record,
     int string_index,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libevtx_error_t **error );

/* Retrieves the size of the data
 * Returns 1 if successful, 0 if not available or -1 on error
 */
LIBEVTX_EXTERN \
int libevtx_record_get_data_size(
     libevtx_record_t *record,
     size_t *data_size,
     libevtx_error_t **error );

/* Retrieves the data
 * Returns 1 if successful, 0 if not available or -1 on error
 */
LIBEVTX_EXTERN \
int libevtx_record_get_data(
     libevtx_record_t *record,
     uint8_t *data,
     size_t data_size,
     libevtx_error_t **error );

/* Retrieves the size of the UTF-8 encoded XML string
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBEVTX_EXTERN \
int libevtx_record_get_utf8_xml_string_size(
     libevtx_record_t *record,
     size_t *utf8_string_size,
     libevtx_error_t **error );

/* Retrieves the UTF-8 encoded XML string
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBEVTX_EXTERN \
int libevtx_record_get_utf8_xml_string(
     libevtx_record_t *record,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libevtx_error_t **error );

/* Retrieves the size of the UTF-16 encoded XML string
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBEVTX_EXTERN \
int libevtx_record_get_utf16_xml_string_size(
     libevtx_record_t *record,
     size_t *utf16_string_size,
     libevtx_error_t **error );

/* Retrieves the UTF-16 encoded XML string
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBEVTX_EXTERN \
int libevtx_record_get_utf16_xml_string(
     libevtx_record_t *record,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libevtx_error_t **error );

/* -------------------------------------------------------------------------
 * Record functions - deprecated
 * ------------------------------------------------------------------------- */

/* Parses the record data
 * Returns 1 if successful, 0 if data could not be parsed or -1 on error
 *
 * This function deprecated use there is no need to call this function anymore.
 * If you want to parse the record data with a template definition use:
 * libevtx_record_parse_data_with_template_definition
 */
LIBEVTX_DEPRECATED \
LIBEVTX_EXTERN \
int libevtx_record_parse_data(
     libevtx_record_t *record,
     libevtx_template_definition_t *template_definition,
     libevtx_error_t **error );

/* -------------------------------------------------------------------------
 * Template definition functions
 * ------------------------------------------------------------------------- */

/* Creates a template definition
 * Make sure the value template_definition is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
LIBEVTX_EXTERN \
int libevtx_template_definition_initialize(
     libevtx_template_definition_t **template_definition,
     libevtx_error_t **error );

/* Frees a template definition
 * Returns 1 if successful or -1 on error
 */
LIBEVTX_EXTERN \
int libevtx_template_definition_free(
     libevtx_template_definition_t **template_definition,
     libevtx_error_t **error );

/* Sets the data
 * Returns 1 if successful or -1 on error
 */
LIBEVTX_EXTERN \
int libevtx_template_definition_set_data(
     libevtx_template_definition_t *template_definition,
     const uint8_t *data,
     size_t data_size,
     uint32_t data_offset,
     libevtx_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBEVTX_H ) */

