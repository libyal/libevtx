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
	test_set_ascii_codepage.sh \
	test_evtxinfo.sh \
	test_evtxexport.sh \
	test_evtxexport_xml.sh \
	$(TESTS_PYEVTX)

check_SCRIPTS = \
	pyevtx_test_get_version.py \
	pyevtx_test_open_close.py \
	pyevtx_test_set_ascii_codepage.py \
	test_api_functions.sh \
	test_pyevtx_set_ascii_codepage.sh \
	test_python_functions.sh \
	test_evtxexport.sh \
	test_evtxexport_xml.sh \
	test_evtxinfo.sh \
	test_runner.sh \
	test_set_ascii_codepage.sh

EXTRA_DIST = \
	$(check_SCRIPTS)

check_PROGRAMS = \
	evtx_test_get_version \
	evtx_test_open_close \
	evtx_test_set_ascii_codepage

evtx_test_get_version_SOURCES = \
	evtx_test_get_version.c \
	evtx_test_libcstring.h \
	evtx_test_libevtx.h \
	evtx_test_unused.h

evtx_test_get_version_LDADD = \
	../libevtx/libevtx.la \
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
	../libevtx/libevtx.la \
	@LIBCDATA_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@LIBCSTRING_LIBADD@

evtx_test_set_ascii_codepage_SOURCES = \
	evtx_test_libcerror.h \
	evtx_test_libclocale.h \
	evtx_test_libevtx.h \
	evtx_test_libcstring.h \
	evtx_test_set_ascii_codepage.c

evtx_test_set_ascii_codepage_LDADD = \
	../libevtx/libevtx.la \
	@LIBCLOCALE_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@LIBCSTRING_LIBADD@

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f Makefile

