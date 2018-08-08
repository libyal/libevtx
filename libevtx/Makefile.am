AM_CPPFLAGS = \
	-DLOCALEDIR=\"$(datadir)/locale\" \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCERROR_CPPFLAGS@ \
	@LIBCTHREADS_CPPFLAGS@ \
	@LIBCDATA_CPPFLAGS@ \
	@LIBCLOCALE_CPPFLAGS@ \
	@LIBCNOTIFY_CPPFLAGS@ \
	@LIBCSPLIT_CPPFLAGS@ \
	@LIBUNA_CPPFLAGS@ \
	@LIBCFILE_CPPFLAGS@ \
	@LIBCPATH_CPPFLAGS@ \
	@LIBBFIO_CPPFLAGS@ \
	@LIBFCACHE_CPPFLAGS@ \
	@LIBFDATA_CPPFLAGS@ \
	@LIBFDATETIME_CPPFLAGS@ \
	@LIBFGUID_CPPFLAGS@ \
	@LIBFVALUE_CPPFLAGS@ \
	@LIBFWEVT_CPPFLAGS@ \
	@LIBFWNT_CPPFLAGS@ \
	@PTHREAD_CPPFLAGS@ \
	@LIBEVTX_DLL_EXPORT@

lib_LTLIBRARIES = libevtx.la

libevtx_la_SOURCES = \
	evtx_chunk.h \
	evtx_event_record.h \
	evtx_file_header.h \
	libevtx.c \
	libevtx_byte_stream.c libevtx_byte_stream.h \
	libevtx_checksum.c libevtx_checksum.h \
	libevtx_chunk.c libevtx_chunk.h \
	libevtx_chunks_table.c libevtx_chunks_table.h \
	libevtx_codepage.c libevtx_codepage.h \
	libevtx_debug.c libevtx_debug.h \
	libevtx_definitions.h \
	libevtx_error.c libevtx_error.h \
	libevtx_extern.h \
	libevtx_file.c libevtx_file.h \
	libevtx_i18n.c libevtx_i18n.h \
	libevtx_io_handle.c libevtx_io_handle.h \
	libevtx_legacy.c libevtx_legacy.h \
	libevtx_libbfio.h \
	libevtx_libcdata.h \
	libevtx_libcerror.h \
	libevtx_libclocale.h \
	libevtx_libcnotify.h \
	libevtx_libfcache.h \
	libevtx_libfdata.h \
	libevtx_libfdatetime.h \
	libevtx_libfguid.h \
	libevtx_libfvalue.h \
	libevtx_libfwevt.h \
	libevtx_libuna.h \
	libevtx_notify.c libevtx_notify.h \
	libevtx_record.c libevtx_record.h \
	libevtx_record_values.c libevtx_record_values.h \
	libevtx_support.c libevtx_support.h \
	libevtx_template_definition.c libevtx_template_definition.h \
	libevtx_types.h \
	libevtx_unused.h

libevtx_la_LIBADD = \
	@LIBCERROR_LIBADD@ \
	@LIBCTHREADS_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBBFIO_LIBADD@ \
	@LIBFCACHE_LIBADD@ \
	@LIBFDATA_LIBADD@ \
	@LIBFDATETIME_LIBADD@ \
	@LIBFGUID_LIBADD@ \
	@LIBFWEVT_LIBADD@ \
	@LIBFWNT_LIBADD@ \
	@LIBFVALUE_LIBADD@ \
	@PTHREAD_LIBADD@

libevtx_la_LDFLAGS = -no-undefined -version-info 1:0:0

EXTRA_DIST = \
	libevtx_definitions.h.in \
	libevtx.rc \
	libevtx.rc.in

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f libevtx_definitions.h
	/bin/rm -f libevtx.rc
	/bin/rm -f Makefile

splint:
	@echo "Running splint on libevtx ..."
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(libevtx_la_SOURCES)

