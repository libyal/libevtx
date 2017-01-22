/*
 * Common input functions for the evtxtools
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

#if !defined( _EVTXINPUT_H )
#define _EVTXINPUT_H

#include <common.h>
#include <types.h>

#include "evtxtools_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

enum EVTXTOOLS_EVENT_LOG_TYPES
{
	EVTXTOOLS_EVENT_LOG_TYPE_UNKNOWN,
	EVTXTOOLS_EVENT_LOG_TYPE_APPLICATION,
	EVTXTOOLS_EVENT_LOG_TYPE_DFS_REPLICATION,
	EVTXTOOLS_EVENT_LOG_TYPE_HARDWARE_EVENTS,
	EVTXTOOLS_EVENT_LOG_TYPE_INTERNET_EXPLORER,
	EVTXTOOLS_EVENT_LOG_TYPE_KEY_MANAGEMENT_SERVICE,
	EVTXTOOLS_EVENT_LOG_TYPE_MEDIA_CENTER,
	EVTXTOOLS_EVENT_LOG_TYPE_SECURITY,
	EVTXTOOLS_EVENT_LOG_TYPE_SYSTEM
};

int evtxinput_determine_event_log_type(
     const system_character_t *string,
     int *event_log_type,
     libcerror_error_t **error );

int evtxinput_determine_event_log_type_from_filename(
     const system_character_t *filename,
     int *event_log_type,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _EVTXINPUT_H ) */

