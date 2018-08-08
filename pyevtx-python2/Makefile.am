# Note that we cannot use: AUTOMAKE_OPTIONS = subdir-objects
# subdir-objects will compile the source files to a single version of Python.
# Since subdir-objects is being deprecated we copy the source files instead.
am__installdirs = "$(DESTDIR)$(pyexecdir2)"
pyexecdir = $(pyexecdir2)

if HAVE_PYTHON2
AM_CFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCERROR_CPPFLAGS@ \
	@LIBCDATA_CPPFLAGS@ \
	@LIBCLOCALE_CPPFLAGS@ \
	@LIBCSPLIT_CPPFLAGS@ \
	@LIBUNA_CPPFLAGS@ \
	@LIBCFILE_CPPFLAGS@ \
	@LIBCPATH_CPPFLAGS@ \
	@LIBBFIO_CPPFLAGS@ \
	@LIBEVTX_DLL_IMPORT@

am_pyevtx_la_rpath = -rpath $(pyexecdir2)

BUILT_SOURCES = \
	pyevtx.c pyevtx.h \
	pyevtx_codepage.c pyevtx_codepage.h \
	pyevtx_datetime.c pyevtx_datetime.h \
	pyevtx_error.c pyevtx_error.h \
	pyevtx_event_levels.c pyevtx_event_levels.h \
	pyevtx_file.c pyevtx_file.h \
	pyevtx_file_flags.c pyevtx_file_flags.h \
	pyevtx_file_object_io_handle.c pyevtx_file_object_io_handle.h \
	pyevtx_integer.c pyevtx_integer.h \
	pyevtx_libbfio.h \
	pyevtx_libcerror.h \
	pyevtx_libclocale.h \
	pyevtx_libevtx.h \
	pyevtx_python.h \
	pyevtx_record.c pyevtx_record.h \
	pyevtx_records.c pyevtx_records.h \
	pyevtx_strings.c pyevtx_strings.h \
	pyevtx_unused.h

pyexec_LTLIBRARIES = pyevtx.la

nodist_pyevtx_la_SOURCES = $(BUILT_SOURCES)

pyevtx_la_LIBADD = \
	@LIBCERROR_LIBADD@ \
	../libevtx/libevtx.la \
	@LIBCDATA_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBBFIO_LIBADD@

pyevtx_la_CPPFLAGS = $(PYTHON2_CPPFLAGS)
pyevtx_la_LDFLAGS  = -module -avoid-version $(PYTHON2_LDFLAGS)

$(BUILT_SOURCES):
	/bin/cp -f $(top_srcdir)/pyevtx/$@ $@

endif

MAINTAINERCLEANFILES = \
	Makefile.in

clean-local:
	/bin/rm -f pyevtx*.[ch]

distclean: clean
	/bin/rm -f Makefile

