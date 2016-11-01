AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCSTRING_CPPFLAGS@ \
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
	@LIBCSYSTEM_CPPFLAGS@ \
	@LIBEVTX_DLL_IMPORT@

if HAVE_PYTHON_TESTS
TESTS_PYEVTX = \
	test_python_functions.sh \
	test_pyevtx_set_ascii_codepage.sh
endif

TESTS = \
	test_api_functions.sh \
	test_api_types.sh \
	test_evtxinfo.sh \
	test_evtxexport.sh \
	test_evtxexport_xml.sh \
	$(TESTS_PYEVTX)

check_SCRIPTS = \
	pyevtx_test_get_version.py \
	pyevtx_test_open_close.py \
	pyevtx_test_set_ascii_codepage.py \
	test_api_functions.sh \
	test_api_types.sh \
	test_evtxexport.sh \
	test_evtxexport_xml.sh \
	test_evtxinfo.sh \
	test_pyevtx_set_ascii_codepage.sh \
	test_python_functions.sh \
	test_runner.sh

EXTRA_DIST = \
	$(check_SCRIPTS)

check_PROGRAMS = \
	evtx_test_error \
	evtx_test_file \
	evtx_test_open_close \
	evtx_test_record \
	evtx_test_support

evtx_test_error_SOURCES = \
	evtx_test_error.c \
	evtx_test_libcstring.h \
	evtx_test_libevtx.h \
	evtx_test_macros.h \
	evtx_test_unused.h

evtx_test_error_LDADD = \
	../libevtx/libevtx.la \
	@LIBCSTRING_LIBADD@

evtx_test_file_SOURCES = \
	evtx_test_file.c \
	evtx_test_libcerror.h \
	evtx_test_libclocale.h \
	evtx_test_libcstring.h \
	evtx_test_libcsystem.h \
	evtx_test_libevtx.h \
	evtx_test_libuna.h \
	evtx_test_memory.c evtx_test_memory.h \
	evtx_test_unused.h

evtx_test_file_LDADD = \
	@LIBCSYSTEM_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libevtx/libevtx.la \
	@LIBCERROR_LIBADD@ \
	@LIBCSTRING_LIBADD@

evtx_test_open_close_SOURCES = \
	evtx_test_libcerror.h \
	evtx_test_libcstring.h \
	evtx_test_libcsystem.h \
	evtx_test_libevtx.h \
	evtx_test_open_close.c

evtx_test_open_close_LDADD = \
	@LIBCSYSTEM_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libevtx/libevtx.la \
	@LIBCERROR_LIBADD@ \
	@LIBCSTRING_LIBADD@

evtx_test_record_SOURCES = \
	evtx_test_libcerror.h \
	evtx_test_libclocale.h \
	evtx_test_libcstring.h \
	evtx_test_libcsystem.h \
	evtx_test_libevtx.h \
	evtx_test_libuna.h \
	evtx_test_memory.c evtx_test_memory.h \
	evtx_test_record.c \
	evtx_test_unused.h

evtx_test_record_LDADD = \
	@LIBCSYSTEM_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libevtx/libevtx.la \
	@LIBCERROR_LIBADD@ \
	@LIBCSTRING_LIBADD@

evtx_test_support_SOURCES = \
	evtx_test_libcerror.h \
	evtx_test_libclocale.h \
	evtx_test_libcstring.h \
	evtx_test_libevtx.h \
	evtx_test_libuna.h \
	evtx_test_macros.h \
	evtx_test_support.c \
	evtx_test_unused.h

evtx_test_support_LDADD = \
	@LIBCSYSTEM_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libevtx/libevtx.la \
	@LIBCERROR_LIBADD@ \
	@LIBCSTRING_LIBADD@

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f Makefile

