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
	@LIBBFIO_CPPFLAGS@

if HAVE_PYTHON
TESTS_PYEVTX  = \
	test_pyevtx_set_ascii_codepage.sh \
	test_pyevtx_open_close.sh
endif

TESTS = \
	test_set_ascii_codepage.sh \
	test_open_close.sh \
	test_evtxinfo.sh \
	test_evtxexport.sh \
	test_evtxexport_xml.sh \
	$(TESTS_PYEVTX)

check_SCRIPTS = \
	pyevtx_test_open_close.py \
	pyevtx_test_set_ascii_codepage.py \
	test_open_close.sh \
	test_pyevtx_open_close.sh \
	test_pyevtx_set_ascii_codepage.sh \
	test_evtxexport.sh \
	test_evtxexport_xml.sh \
	test_evtxinfo.sh \
	test_runner.sh \
	test_set_ascii_codepage.sh

EXTRA_DIST = \
	$(check_SCRIPTS)

check_PROGRAMS =  \
	evtx_test_open_close \
	evtx_test_set_ascii_codepage

evtx_test_open_close_SOURCES = \
	evtx_test_libcerror.h \
	evtx_test_libevtx.h \
	evtx_test_libcstring.h \
	evtx_test_open_close.c

evtx_test_open_close_LDADD = \
	../libevtx/libevtx.la
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
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
	../libevtx/libevtx.la
	@LIBCLOCALE_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@LIBCSTRING_LIBADD@

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f Makefile

