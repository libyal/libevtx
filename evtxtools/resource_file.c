/*
 * Resource file
 *
 * Copyright (c) 2011-2012, Joachim Metz <joachim.metz@gmail.com>
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
#include <memory.h>
#include <types.h>

#include "evtxtools_libcerror.h"
#include "evtxtools_libcstring.h"
#include "evtxtools_libexe.h"
#include "evtxtools_libwrc.h"
#include "resource_file.h"

/* Initializes the resource file
 * Returns 1 if successful or -1 on error
 */
int resource_file_initialize(
     resource_file_t **resource_file,
     libcerror_error_t **error )
{
	static char *function = "resource_file_initialize";

	if( resource_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid resource file.",
		 function );

		return( -1 );
	}
	if( *resource_file != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid resource file value already set.",
		 function );

		return( -1 );
	}
	*resource_file = memory_allocate_structure(
	                  resource_file_t );

	if( *resource_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create resource file.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *resource_file,
	     0,
	     sizeof( resource_file_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear resource file.",
		 function );

		memory_free(
		 *resource_file );

		*resource_file = NULL;

		return( -1 );
	}
	if( libexe_file_initialize(
	     &( ( *resource_file )->exe_file ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize EXE file.",
		 function );

		goto on_error;
	}
	if( libwrc_stream_initialize(
	     &( ( *resource_file )->resource_stream ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize resource stream.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *resource_file != NULL )
	{
		if( ( *resource_file )->resource_stream != NULL )
		{
			libwrc_stream_free(
			 &( ( *resource_file )->resource_stream ),
			 NULL );
		}
		if( ( *resource_file )->exe_file != NULL )
		{
			libexe_file_free(
			 &( ( *resource_file )->exe_file ),
			 NULL );
		}
		memory_free(
		 *resource_file );

		*resource_file = NULL;
	}
	return( -1 );
}

/* Frees the resource file and its elements
 * Returns 1 if successful or -1 on error
 */
int resource_file_free(
     resource_file_t **resource_file,
     libcerror_error_t **error )
{
	static char *function = "resource_file_free";
	int result            = 1;

	if( resource_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid resource file.",
		 function );

		return( -1 );
	}
	if( *resource_file != NULL )
	{
		if( ( *resource_file )->is_open != 0 )
		{
			if( resource_file_close(
			     *resource_file,
			     error ) != 0 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_CLOSE_FAILED,
				 "%s: unable to close resource file.",
				 function );

				result = -1;
			}
		}
		if( libwrc_stream_free(
		     &( ( *resource_file )->resource_stream ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free resource stream.",
			 function );

			result = -1;
		}
		if( libexe_file_free(
		     &( ( *resource_file )->exe_file ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free EXE file.",
			 function );

			result = -1;
		}
		if( ( *resource_file )->name != NULL )
		{
			memory_free(
			 ( *resource_file )->name );
		}
		memory_free(
		 *resource_file );

		*resource_file = NULL;
	}
	return( result );
}

/* Opens the resource file
 * Returns 1 if successful or -1 on error
 */
int resource_file_open(
     resource_file_t *resource_file,
     const libcstring_system_character_t *filename,
     libcerror_error_t **error )
{
	static char *function    = "resource_file_open";
	uint32_t virtual_address = 0;

	if( resource_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid resource file.",
		 function );

		return( -1 );
	}
	if( resource_file->is_open != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid resource file already open.",
		 function );

		return( -1 );
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	if( libexe_file_open_wide(
	     resource_file->exe_file,
	     filename,
	     LIBEXE_OPEN_READ,
	     error ) != 1 )
#else
	if( libexe_file_open(
	     resource_file->exe_file,
	     filename,
	     LIBEXE_OPEN_READ,
	     error ) != 1 )
#endif
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open EXE file.",
		 function );

		goto on_error;
	}
	if( libexe_file_get_section_by_name(
	     resource_file->exe_file,
	     ".rsrc",
	     5,
	     &( resource_file->resource_section ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve resource section.",
		 function );

		goto on_error;
	}
	if( libexe_section_get_virtual_address(
	     resource_file->resource_section,
	     &virtual_address,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve resource section virtual adress.",
		 function );

		goto on_error;
	}
	if( libexe_section_get_data_file_io_handle(
	     resource_file->resource_section,
	     &( resource_file->resource_section_file_io_handle ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve resource section file IO handle.",
		 function );

		goto on_error;
	}
	if( libwrc_stream_set_virtual_address(
	     resource_file->resource_stream,
	     virtual_address,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set resource stream virtual adress.",
		 function );

		goto on_error;
	}
	if( libwrc_stream_open_file_io_handle(
	     resource_file->resource_stream,
	     resource_file->resource_section_file_io_handle,
	     LIBWRC_OPEN_READ,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open resource stream.",
		 function );

		goto on_error;
	}
	resource_file->is_open = 1;

	return( 1 );

on_error:
	if( resource_file->resource_section_file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &( resource_file->resource_section_file_io_handle ),
		 NULL );
	}
	if( resource_file->resource_section != NULL )
	{
		libexe_section_free(
		 &( resource_file->resource_section ),
		 NULL );
	}
	libexe_file_close(
	 resource_file->exe_file,
	 NULL );

	return( -1 );
}

/* Closes the resource file
 * Returns the 0 if succesful or -1 on error
 */
int resource_file_close(
     resource_file_t *resource_file,
     libcerror_error_t **error )
{
	static char *function = "resource_file_close";
	int result            = 0;

	if( resource_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid resource file.",
		 function );

		return( -1 );
	}
	if( resource_file->is_open != 0 )
	{
		if( libwrc_stream_close(
		     resource_file->resource_stream,
		     error ) != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_CLOSE_FAILED,
			 "%s: unable to close resource stream.",
			 function );

			result = -1;
		}
		if( resource_file->resource_section_file_io_handle != NULL )
		{
			if( libbfio_handle_free(
			     &( resource_file->resource_section_file_io_handle ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free resource section file IO handle.",
				 function );

				result = -1;
			}
		}
		if( resource_file->resource_section != NULL )
		{
			if( libexe_section_free(
			     &( resource_file->resource_section ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free resource section.",
				 function );

				result = -1;
			}
		}
		if( libexe_file_close(
		     resource_file->exe_file,
		     error ) != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_CLOSE_FAILED,
			 "%s: unable to close EXE file.",
			 function );

			result = -1;
		}
		resource_file->is_open = 0;
	}
	return( result );
}

/* Sets the name
 * Returns 1 if successful or -1 error
 */
int resource_file_set_name(
     resource_file_t *resource_file,
     const libcstring_system_character_t *name,
     size_t name_length,
     libcerror_error_t **error )
{
	static char *function = "resource_file_set_name";

	if( resource_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid resource file.",
		 function );

		return( -1 );
	}
	if( resource_file->is_open != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid resource file already open.",
		 function );

		return( -1 );
	}
	if( name == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid name.",
		 function );

		return( -1 );
	}
	if( name_length > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid name length value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( resource_file->name != NULL )
	{
		memory_free(
		 resource_file->name );

		resource_file->name = NULL;
	}
	resource_file->name_size = name_length + 1;

	resource_file->name = libcstring_system_string_allocate(
			      resource_file->name_size );

	if( resource_file->name == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create name.",
		 function );

		goto on_error;
	}
	if( libcstring_system_string_copy(
	     resource_file->name,
	     name,
	     name_length ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy name.",
		 function );

		goto on_error;
	}
	( resource_file->name )[ name_length ] = 0;

	return( 1 );

on_error:
	if( resource_file->name != NULL )
	{
		memory_free(
		 resource_file->name );

		resource_file->name = NULL;
	}
	resource_file->name_size = 0;

	return( -1 );
}

