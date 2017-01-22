/*
 * Extracts items from a Windows XML Event Viewer Log (EVTX) file
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
#include <system_string.h>
#include <types.h>

#if defined( HAVE_UNISTD_H )
#include <unistd.h>
#endif

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "evtxtools_getopt.h"
#include "evtxtools_libcerror.h"
#include "evtxtools_libclocale.h"
#include "evtxtools_libcnotify.h"
#include "evtxtools_libevtx.h"
#include "evtxtools_output.h"
#include "evtxtools_signal.h"
#include "evtxtools_unused.h"
#include "export_handle.h"
#include "log_handle.h"

export_handle_t *evtxexport_export_handle = NULL;
int evtxexport_abort                      = 0;

/* Prints the executable usage information
 */
void usage_fprint(
      FILE *stream )
{
	if( stream == NULL )
	{
		return;
	}
	fprintf( stream, "Use evtxexport to export items stored in a Windows XML Event Viewer\n"
	                 "Log (EVTX) file.\n\n" );

	fprintf( stream, "Usage: evtxexport [ -c codepage ] [ -f format ] [ -l log_file ]\n"
	                 "                  [ -m mode ] [ -p resource_files_path ]\n"
	                 "                  [ -r registy_files_path ] [ -s system_file ]\n"
	                 "                  [ -S software_file ] [ -t event_log_type ]\n"
	                 "                  [ -hTvV ] source\n\n" );


	fprintf( stream, "\tsource: the source file\n\n" );

	fprintf( stream, "\t-c:     codepage of ASCII strings, options: ascii, windows-874,\n"
	                 "\t        windows-932, windows-936, windows-949, windows-950,\n"
	                 "\t        windows-1250, windows-1251, windows-1252 (default),\n"
	                 "\t        windows-1253, windows-1254, windows-1255, windows-1256\n"
	                 "\t        windows-1257 or windows-1258\n" );
	fprintf( stream, "\t-f:     output format, options: xml, text (default)\n" );
	fprintf( stream, "\t-h:     shows this help\n" );
	fprintf( stream, "\t-l:     logs information about the exported items\n" );
	fprintf( stream, "\t-m:     export mode, option: all, items (default), recovered\n"
	                 "\t        'all' exports the (allocated) items and recovered items,\n"
	                 "\t        'items' exports the (allocated) items and 'recovered' exports\n"
	                 "\t        the recovered items\n" );
	fprintf( stream, "\t-p:     search PATH for the resource files\n" );
	fprintf( stream, "\t-r:     name of the directory containing the SOFTWARE and SYSTEM\n"
	                 "\t        (Windows) Registry file\n" );
	fprintf( stream, "\t-s:     filename of the SYSTEM (Windows) Registry file.\n"
	                 "\t        This option overrides the path provided by -r\n" );
	fprintf( stream, "\t-S:     filename of the SOFTWARE (Windows) Registry file.\n"
	                 "\t        This option overrides the path provided by -r\n" );
	fprintf( stream, "\t-t:     event log type, options: application, security, system\n"
	                 "\t        if not specified the event log type is determined based\n"
	                 "\t        on the filename.\n" );
	fprintf( stream, "\t-T:     use event template definitions to parse the event record data\n" );
	fprintf( stream, "\t-v:     verbose output to stderr\n" );
	fprintf( stream, "\t-V:     print version\n" );
}

/* Signal handler for evtxexport
 */
void evtxexport_signal_handler(
      evtxtools_signal_t signal EVTXTOOLS_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	static char *function    = "evtxexport_signal_handler";

	EVTXTOOLS_UNREFERENCED_PARAMETER( signal )

	evtxexport_abort = 1;

	if( evtxexport_export_handle != NULL )
	{
		if( export_handle_signal_abort(
		     evtxexport_export_handle,
		     &error ) != 1 )
		{
			libcnotify_printf(
			 "%s: unable to signal export handle to abort.\n",
			 function );

			libcnotify_print_error_backtrace(
			 error );
			libcerror_error_free(
			 &error );
		}
	}
	/* Force stdin to close otherwise any function reading it will remain blocked
	 */
#if defined( WINAPI ) && !defined( __CYGWIN__ )
	if( _close(
	     0 ) != 0 )
#else
	if( close(
	     0 ) != 0 )
#endif
	{
		libcnotify_printf(
		 "%s: unable to close stdin.\n",
		 function );
	}
}

/* The main program
 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
int wmain( int argc, wchar_t * const argv[] )
#else
int main( int argc, char * const argv[] )
#endif
{
	libcerror_error_t *error                              = NULL;
	log_handle_t *log_handle                              = NULL;
	system_character_t *option_ascii_codepage             = NULL;
	system_character_t *option_event_log_type             = NULL;
	system_character_t *option_export_format              = NULL;
	system_character_t *option_export_mode                = NULL;
	system_character_t *option_log_filename               = NULL;
	system_character_t *option_resource_files_path        = NULL;
	system_character_t *option_preferred_language         = NULL;
	system_character_t *option_registry_directory_name    = NULL;
	system_character_t *option_software_registry_filename = NULL;
	system_character_t *option_system_registry_filename   = NULL;
	system_character_t *source                            = NULL;
	char *program                                         = "evtxexport";
	system_integer_t option                               = 0;
	int result                                            = 0;
	int use_template_definition                           = 0;
	int verbose                                           = 0;

	libcnotify_stream_set(
	 stderr,
	 NULL );
	libcnotify_verbose_set(
	 1 );

	if( libclocale_initialize(
	     "evtxtools",
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize locale values.\n" );

		goto on_error;
	}
	if( evtxtools_output_initialize(
	     _IONBF,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize output settings.\n" );

		goto on_error;
	}
	evtxoutput_version_fprint(
	 stdout,
	 program );

	while( ( option = evtxtools_getopt(
	                   argc,
	                   argv,
	                   _SYSTEM_STRING( "c:f:hl:m:p:r:s:S:t:TvV" ) ) ) != (system_integer_t) -1 )
	{
		switch( option )
		{
			case (system_integer_t) '?':
			default:
				fprintf(
				 stderr,
				 "Invalid argument: %" PRIs_SYSTEM "\n",
				 argv[ optind - 1 ] );

				usage_fprint(
				 stdout );

				return( EXIT_FAILURE );

			case (system_integer_t) 'c':
				option_ascii_codepage = optarg;

				break;

			case (system_integer_t) 'f':
				option_export_format = optarg;

				break;

			case (system_integer_t) 'h':
				usage_fprint(
				 stdout );

				return( EXIT_SUCCESS );

			case (system_integer_t) 'l':
				option_log_filename = optarg;

				break;

			case (system_integer_t) 'm':
				option_export_mode = optarg;

				break;

			case (system_integer_t) 'p':
				option_resource_files_path = optarg;

				break;

			case (system_integer_t) 'r':
				option_registry_directory_name = optarg;

				break;

			case (system_integer_t) 's':
				option_system_registry_filename = optarg;

				break;

			case (system_integer_t) 'S':
				option_software_registry_filename = optarg;

				break;

			case (system_integer_t) 't':
				option_event_log_type = optarg;

				break;

			case (system_integer_t) 'T':
				use_template_definition = 1;

				break;

			case (system_integer_t) 'v':
				verbose = 1;

				break;

			case (system_integer_t) 'V':
				evtxoutput_copyright_fprint(
				 stdout );

				return( EXIT_SUCCESS );
		}
	}
	if( optind == argc )
	{
		fprintf(
		 stderr,
		 "Missing source file.\n" );

		usage_fprint(
		 stdout );

		return( EXIT_FAILURE );
	}
	source = argv[ optind ];

	libcnotify_verbose_set(
	 verbose );
	libevtx_notify_set_stream(
	 stderr,
	 NULL );
	libevtx_notify_set_verbose(
	 verbose );

	if( log_handle_initialize(
	     &log_handle,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize log handle.\n" );

		goto on_error;
	}
	if( export_handle_initialize(
	     &evtxexport_export_handle,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize export handle.\n" );

		goto on_error;
	}
	if( option_ascii_codepage != NULL )
	{
		result = export_handle_set_ascii_codepage(
		          evtxexport_export_handle,
		          option_ascii_codepage,
		          &error );

		if( result == -1 )
		{
			fprintf(
			 stderr,
			 "Unable to set ASCII codepage in export handle.\n" );

			goto on_error;
		}
		else if( result == 0 )
		{
			fprintf(
			 stderr,
			 "Unsupported ASCII codepage defaulting to: windows-1252.\n" );
		}
	}
	if( option_event_log_type != NULL )
	{
		result = export_handle_set_event_log_type(
		          evtxexport_export_handle,
		          option_event_log_type,
		          &error );

		if( result == -1 )
		{
			fprintf(
			 stderr,
			 "Unable to set event log type in export handle.\n" );

			goto on_error;
		}
	}
	if( option_export_format != NULL )
	{
		result = export_handle_set_export_format(
			  evtxexport_export_handle,
			  option_export_format,
			  &error );

		if( result == -1 )
		{
			fprintf(
			 stderr,
			 "Unable to set export format.\n" );

			goto on_error;
		}
		else if( result == 0 )
		{
			fprintf(
			 stderr,
			 "Unsupported export format defaulting to: text.\n" );
		}
	}
	if( option_export_mode != NULL )
	{
		result = export_handle_set_export_mode(
			  evtxexport_export_handle,
			  option_export_mode,
			  &error );

		if( result == -1 )
		{
			fprintf(
			 stderr,
			 "Unable to set export mode.\n" );

			goto on_error;
		}
		else if( result == 0 )
		{
			fprintf(
			 stderr,
			 "Unsupported export mode defaulting to: items.\n" );
		}
	}
	if( ( option_event_log_type == NULL )
	 || ( result == 0 ) )
	{
		result = export_handle_set_event_log_type_from_filename(
			  evtxexport_export_handle,
			  source,
			  &error );

		if( result == -1 )
		{
			fprintf(
			 stderr,
			 "Unable to set event log type from filename in export handle.\n" );

			goto on_error;
		}
	}
	if( option_resource_files_path != NULL )
	{
		if( export_handle_set_resource_files_path(
		     evtxexport_export_handle,
		     option_resource_files_path,
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to set resource files path in export handle.\n" );

			goto on_error;
		}
	}
	if( option_software_registry_filename != NULL )
	{
		if( export_handle_set_software_registry_filename(
		     evtxexport_export_handle,
		     option_software_registry_filename,
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to set software registry filename in export handle.\n" );

			goto on_error;
		}
	}
	if( option_system_registry_filename != NULL )
	{
		if( export_handle_set_system_registry_filename(
		     evtxexport_export_handle,
		     option_system_registry_filename,
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to set system registry filename in export handle.\n" );

			goto on_error;
		}
	}
	if( option_registry_directory_name != NULL )
	{
		if( export_handle_set_registry_directory_name(
		     evtxexport_export_handle,
		     option_registry_directory_name,
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to set registry directory name in export handle.\n" );

			goto on_error;
		}
	}
	if( option_preferred_language != NULL )
	{
/* TODO set preferred language identifier from input */
		if( export_handle_set_preferred_language_identifier(
		     evtxexport_export_handle,
		     0x0409,
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to set preferred language identifier in export handle.\n" );

			goto on_error;
		}
	}
	evtxexport_export_handle->use_template_definition = use_template_definition;

	if( log_handle_open(
	     log_handle,
	     option_log_filename,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to open log file: %" PRIs_SYSTEM ".\n",
		 option_log_filename );

		goto on_error;
	}
	if( export_handle_open_input(
	     evtxexport_export_handle,
	     source,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to open: %" PRIs_SYSTEM ".\n",
		 source );

		goto on_error;
	}
	result = export_handle_export_file(
	          evtxexport_export_handle,
	          log_handle,
	          &error );

	if( result == -1 )
	{
		fprintf(
		 stderr,
		 "Unable to export file.\n" );

		goto on_error;
	}
	if( export_handle_close_input(
	     evtxexport_export_handle,
	     &error ) != 0 )
	{
		fprintf(
		 stderr,
		 "Unable to close export handle.\n" );

		goto on_error;
	}
	if( export_handle_free(
	     &evtxexport_export_handle,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to free export handle.\n" );

		goto on_error;
	}
	if( log_handle_close(
	     log_handle,
	     &error ) != 0 )
	{
		fprintf(
		 stderr,
		 "Unable to close log handle.\n" );

		goto on_error;
	}
	if( log_handle_free(
	     &log_handle,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to free log handle.\n" );

		goto on_error;
	}
	if( result == 0 )
	{
		fprintf(
		 stdout,
		 "No records to export.\n" );
	}
	return( EXIT_SUCCESS );

on_error:
	if( error != NULL )
	{
		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );
	}
	if( evtxexport_export_handle != NULL )
	{
		export_handle_free(
		 &evtxexport_export_handle,
		 NULL );
	}
	if( log_handle != NULL )
	{
		log_handle_free(
		 &log_handle,
		 NULL );
	}
	return( EXIT_FAILURE );
}

