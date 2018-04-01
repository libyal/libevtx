AM_CPPFLAGS = \
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
	@LIBEVTX_DLL_IMPORT@

if HAVE_PYTHON_TESTS
TESTS_PYEVTX = \
	test_python_module.sh
endif

TESTS = \
	test_library.sh \
	test_evtxinfo.sh \
	test_evtxexport.sh \
	test_evtxexport_xml.sh \
	$(TESTS_PYEVTX)

check_SCRIPTS = \
	pyevtx_test_file.py \
	pyevtx_test_support.py \
	test_evtxexport.sh \
	test_evtxexport_xml.sh \
	test_evtxinfo.sh \
	test_library.sh \
	test_python_module.sh \
	test_runner.sh

EXTRA_DIST = \
	$(check_SCRIPTS)

check_PROGRAMS = \
	evtx_test_chunk \
	evtx_test_chunks_table \
	evtx_test_error \
	evtx_test_file \
	evtx_test_io_handle \
	evtx_test_notify \
	evtx_test_record \
	evtx_test_record_values \
	evtx_test_support \
	evtx_test_template_definition

evtx_test_chunk_SOURCES = \
	evtx_test_chunk.c \
	evtx_test_libcerror.h \
	evtx_test_libevtx.h \
	evtx_test_macros.h \
	evtx_test_memory.c evtx_test_memory.h \
	evtx_test_unused.h

evtx_test_chunk_LDADD = \
	../libevtx/libevtx.la \
	@LIBCERROR_LIBADD@

evtx_test_chunks_table_SOURCES = \
	evtx_test_chunks_table.c \
	evtx_test_libcerror.h \
	evtx_test_libevtx.h \
	evtx_test_macros.h \
	evtx_test_memory.c evtx_test_memory.h \
	evtx_test_unused.h

evtx_test_chunks_table_LDADD = \
	../libevtx/libevtx.la \
	@LIBCERROR_LIBADD@

evtx_test_error_SOURCES = \
	evtx_test_error.c \
	evtx_test_libevtx.h \
	evtx_test_macros.h \
	evtx_test_unused.h

evtx_test_error_LDADD = \
	../libevtx/libevtx.la

evtx_test_file_SOURCES = \
	evtx_test_file.c \
	evtx_test_functions.c evtx_test_functions.h \
	evtx_test_getopt.c evtx_test_getopt.h \
	evtx_test_libbfio.h \
	evtx_test_libcerror.h \
	evtx_test_libclocale.h \
	evtx_test_libcnotify.h \
	evtx_test_libevtx.h \
	evtx_test_libuna.h \
	evtx_test_macros.h \
	evtx_test_memory.c evtx_test_memory.h \
	evtx_test_unused.h

evtx_test_file_LDADD = \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libevtx/libevtx.la \
	@LIBCTHREADS_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@PTHREAD_LIBADD@

evtx_test_io_handle_SOURCES = \
	evtx_test_io_handle.c \
	evtx_test_libcerror.h \
	evtx_test_libevtx.h \
	evtx_test_macros.h \
	evtx_test_memory.c evtx_test_memory.h \
	evtx_test_unused.h

evtx_test_io_handle_LDADD = \
	../libevtx/libevtx.la \
	@LIBCERROR_LIBADD@

evtx_test_notify_SOURCES = \
	evtx_test_libcerror.h \
	evtx_test_libevtx.h \
	evtx_test_macros.h \
	evtx_test_notify.c \
	evtx_test_unused.h

evtx_test_notify_LDADD = \
	../libevtx/libevtx.la \
	@LIBCERROR_LIBADD@

evtx_test_record_SOURCES = \
	evtx_test_libcerror.h \
	evtx_test_libevtx.h \
	evtx_test_macros.h \
	evtx_test_memory.c evtx_test_memory.h \
	evtx_test_record.c \
	evtx_test_unused.h

evtx_test_record_LDADD = \
	../libevtx/libevtx.la \
	@LIBCERROR_LIBADD@

evtx_test_record_values_SOURCES = \
	evtx_test_libcerror.h \
	evtx_test_libevtx.h \
	evtx_test_macros.h \
	evtx_test_memory.c evtx_test_memory.h \
	evtx_test_record_values.c \
	evtx_test_unused.h

evtx_test_record_values_LDADD = \
	../libevtx/libevtx.la \
	@LIBCERROR_LIBADD@

evtx_test_support_SOURCES = \
	evtx_test_functions.c evtx_test_functions.h \
	evtx_test_getopt.c evtx_test_getopt.h \
	evtx_test_libbfio.h \
	evtx_test_libcerror.h \
	evtx_test_libclocale.h \
	evtx_test_libcnotify.h \
	evtx_test_libevtx.h \
	evtx_test_libuna.h \
	evtx_test_macros.h \
	evtx_test_memory.c evtx_test_memory.h \
	evtx_test_support.c \
	evtx_test_unused.h

evtx_test_support_LDADD = \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libevtx/libevtx.la \
	@LIBCERROR_LIBADD@

evtx_test_template_definition_SOURCES = \
	evtx_test_libcerror.h \
	evtx_test_libevtx.h \
	evtx_test_macros.h \
	evtx_test_memory.c evtx_test_memory.h \
	evtx_test_template_definition.c \
	evtx_test_unused.h

evtx_test_template_definition_LDADD = \
	../libevtx/libevtx.la \
	@LIBCERROR_LIBADD@

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f Makefile

