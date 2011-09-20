/*
 * Common output functions for the evtxtools
 *
 * Copyright (c) 2011, Joachim Metz <jbmetz@users.sourceforge.net>
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

#if !defined( _EVTXOUTPUT_H )
#define _EVTXOUTPUT_H

#include <common.h>
#include <types.h>

#include <libsystem.h>

#if defined( __cplusplus )
extern "C" {
#endif

void evtxoutput_copyright_fprint(
      FILE *stream );

void evtxoutput_version_fprint(
      FILE *stream,
      const char *program );

void evtxoutput_version_detailed_fprint(
      FILE *stream,
      const char *program );

#if defined( __cplusplus )
}
#endif

#endif

