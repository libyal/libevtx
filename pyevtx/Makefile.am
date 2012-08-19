if HAVE_PYTHON
AM_CFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCSTRING_CPPFLAGS@ \
	@LIBCERROR_CPPFLAGS@ \
	@LIBCLOCALE_CPPFLAGS@ \
	@LIBUNA_CPPFLAGS@ \
	@LIBCFILE_CPPFLAGS@ \
	@LIBCPATH_CPPFLAGS@ \
	@LIBBFIO_CPPFLAGS@ \
	@MEMWATCH_CPPFLAGS@

pyexec_LTLIBRARIES = pyevtx.la

pyevtx_la_SOURCES = \
	pyevtx.c pyevtx.h \
	pyevtx_codepage.c pyevtx_codepage.h \
	pyevtx_datetime.c pyevtx_datetime.h \
	pyevtx_file.c pyevtx_file.h \
	pyevtx_file_object_io_handle.c pyevtx_file_object_io_handle.h \
	pyevtx_libbfio.h \
	pyevtx_libcerror.h \
	pyevtx_libclocale.h \
	pyevtx_libcstring.h \
	pyevtx_libevtx.h \
	pyevtx_python.h \
	pyevtx_record.c pyevtx_record.h \
	pyevtx_records.c pyevtx_records.h

pyevtx_la_LIBADD = \
	@LIBCSTRING_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	../libevtx/libevtx.la \
	@LIBCLOCALE_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBBFIO_LIBADD@

pyevtx_la_CPPFLAGS = $(PYTHON_CPPFLAGS)
pyevtx_la_LDFLAGS  = -module -avoid-version $(PYTHON_LDFLAGS)

endif

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f Makefile

