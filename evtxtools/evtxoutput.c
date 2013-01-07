/*
 * Common output functions for the evtxtools
 *
 * Copyright (c) 2011-2013, Joachim Metz <joachim.metz@gmail.com>
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
#include <memory.h>
#include <types.h>

#if defined( HAVE_LOCAL_LIBBFIO )
#include <libbfio_definitions.h>
#elif defined( HAVE_LIBBFIO_H )
#include <libbfio.h>
#endif

#if defined( HAVE_LOCAL_LIBFVALUE )
#include <libfvalue_definitions.h>
#elif defined( HAVE_LIBFVALUE_H )
#include <libfvalue.h>
#endif

#if defined( HAVE_LOCAL_LIBFWNT )
#include <libfwnt_definitions.h>
#elif defined( HAVE_LIBFWNT_H )
#include <libfwnt.h>
#endif

#include "evtxoutput.h"
#include "evtxtools_libcerror.h"
#include "evtxtools_libclocale.h"
#include "evtxtools_libcstring.h"
#include "evtxtools_libcsystem.h"
#include "evtxtools_libevtx.h"
#include "evtxtools_libfdatetime.h"
#include "evtxtools_libfguid.h"
#include "evtxtools_libuna.h"

/* Prints the copyright information
 */
void evtxoutput_copyright_fprint(
      FILE *stream )
{
	if( stream == NULL )
	{
		return;
	}
	/* TRANSLATORS: This is a proper name.
	 */
	fprintf(
	 stream,
	 _( "Copyright (c) 2011-2013, %s.\n" ),
	 _( "Joachim Metz" ) );

	fprintf(
	 stream,
	 _( "This is free software; see the source for copying conditions. There is NO\n"
	    "warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n" ) );

	/* TRANSLATORS: The placeholder indicates the bug-reporting address
	 * for this package.  Please add _another line_ saying
	 * "Report translation bugs to <...>\n" with the address for translation
	 * bugs (typically your translation team's web or email address).
	 */
	fprintf(
	 stream,
	 _( "Report bugs to <%s>.\n" ),
	 PACKAGE_BUGREPORT );
}

/* Prints the (basic) version information
 */
void evtxoutput_version_fprint(
      FILE *stream,
      const char *program )
{
	if( stream == NULL )
	{
		return;
	}
	if( program == NULL )
	{
		return;
	}
        fprintf(
	 stream,
	 "%s %s\n\n",
         program,
	 LIBEVTX_VERSION_STRING );
}

/* Prints the detailed version information
 */
void evtxoutput_version_detailed_fprint(
      FILE *stream,
      const char *program )
{
	if( stream == NULL )
	{
		return;
	}
	if( program == NULL )
	{
		return;
	}
        fprintf(
	 stream,
	 "%s %s (libevtx %s",
         program,
	 LIBEVTX_VERSION_STRING,
	 LIBEVTX_VERSION_STRING );

	fprintf(
	 stream,
	 ", libuna %s",
	 LIBUNA_VERSION_STRING );

	fprintf(
	 stream,
	 ", libbfio %s",
	 LIBBFIO_VERSION_STRING );

/* TODO libfcache, libfdata */

	fprintf(
	 stream,
	 ", libfdatetime %s",
	 LIBFDATETIME_VERSION_STRING );

	fprintf(
	 stream,
	 ", libfguid %s",
	 LIBFGUID_VERSION_STRING );

	fprintf(
	 stream,
	 ", libfvalue %s",
	 LIBFVALUE_VERSION_STRING );

	fprintf(
	 stream,
	 ", libfwnt %s",
	 LIBFWNT_VERSION_STRING );

        fprintf(
	 stream,
	 ")\n\n" );
}

