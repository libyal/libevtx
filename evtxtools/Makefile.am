AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCSTRING_CPPFLAGS@ \
	@LIBCERROR_CPPFLAGS@ \
	@LIBCLOCALE_CPPFLAGS@ \
	@LIBCNOTIFY_CPPFLAGS@ \
	@LIBUNA_CPPFLAGS@ \
	@LIBCFILE_CPPFLAGS@ \
	@LIBCPATH_CPPFLAGS@ \
	@LIBBFIO_CPPFLAGS@ \
	@LIBFDATETIME_CPPFLAGS@ \
	@LIBFGUID_CPPFLAGS@ \
	@LIBFVALUE_CPPFLAGS@ \
	@LIBFWNT_CPPFLAGS@ \
	@LIBCSYSTEM_CPPFLAGS@ \
	@MEMWATCH_CPPFLAGS@ \
	@LIBEVTX_DLL_IMPORT@

AM_LDFLAGS = @STATIC_LDFLAGS@

bin_PROGRAMS = \
	evtxexport \
	evtxinfo

evtxexport_SOURCES = \
	evtxexport.c \
	evtxoutput.c evtxoutput.h \
	evtxtools_libcerror.h \
	evtxtools_libclocale.h \
	evtxtools_libcnotify.h \
	evtxtools_libcsystem.h \
	evtxtools_libcstring.h \
	evtxtools_libevtx.h \
	evtxtools_libfdatetime.h \
	evtxtools_libfguid.h \
	evtxtools_libuna.h \
	export_handle.c export_handle.h \
	log_handle.c log_handle.h

evtxexport_LDADD  = \
	@LIBCSTRING_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	../libevtx/libevtx.la \
	@LIBFDATETIME_LIBADD@ \
	@LIBFGUID_LIBADD@ \
	@LIBFVALUE_LIBADD@ \
	@LIBFWNT_LIBADD@ \
	@LIBCSYSTEM_LIBADD@ \
	@LIBINTL@ \
	@MEMWATCH_LIBADD@

evtxinfo_SOURCES = \
	evtxinfo.c \
	evtxoutput.c evtxoutput.h \
	evtxtools_libcerror.h \
	evtxtools_libclocale.h \
	evtxtools_libcnotify.h \
	evtxtools_libcstring.h \
	evtxtools_libcsystem.h \
	evtxtools_libevtx.h \
	evtxtools_libfdatetime.h \
	evtxtools_libfguid.h \
	evtxtools_libuna.h \
	info_handle.c info_handle.h

evtxinfo_LDADD  = \
	@LIBCSTRING_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	../libevtx/libevtx.la \
	@LIBCSYSTEM_LIBADD@ \
	@LIBINTL@ \
	@MEMWATCH_LIBADD@

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f Makefile

splint:
	@echo "Running splint on evtxexport ..." 
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(evtxexport_SOURCES) 
	@echo "Running splint on evtxinfo ..." 
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(evtxinfo_SOURCES) 

