AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	-I$(top_srcdir)/libcstring \
	-I$(top_srcdir)/liberror \
	-I$(top_srcdir)/libsystem \
	@LIBUNA_CPPFLAGS@ \
	@LIBBFIO_CPPFLAGS@ \
	@LIBFDATETIME_CPPFLAGS@ \
	@LIBFGUID_CPPFLAGS@ \
	@LIBFVALUE_CPPFLAGS@ \
	@LIBFWNT_CPPFLAGS@ \
	@LIBFMAPI_CPPFLAGS@ \
	@MEMWATCH_CPPFLAGS@ \
	@LIBEVTX_DLL_IMPORT@

AM_LDFLAGS = @STATIC_LDFLAGS@

bin_PROGRAMS = \
	evtxexport \
	evtxinfo

evtxexport_SOURCES = \
	evtxexport.c \
	evtxinput.c evtxinput.h \
	evtxoutput.c evtxoutput.h \
	evtxtools_libevtx.h \
	evtxtools_libfdatetime.h \
	evtxtools_libfguid.h \
	evtxtools_libuna.h \
	export.c export.h \
	export_handle.c export_handle.h \
	log_handle.c log_handle.h

evtxexport_LDADD  = \
	../libcstring/libcstring.la \
	../liberror/liberror.la \
	../libevtx/libevtx.la \
	../libsystem/libsystem.la \
	@LIBUNA_LIBADD@ \
	@LIBFDATETIME_LIBADD@ \
	@LIBFGUID_LIBADD@ \
	@LIBFVALUE_LIBADD@ \
	@LIBFWNT_LIBADD@ \
	@LIBFMAPI_LIBADD@ \
	@LIBINTL@ \
	@MEMWATCH_LIBADD@

evtxinfo_SOURCES = \
	evtxinfo.c \
	evtxinput.c evtxinput.h \
	evtxoutput.c evtxoutput.h \
	evtxtools_libevtx.h \
	evtxtools_libfdatetime.h \
	evtxtools_libfguid.h \
	evtxtools_libuna.h \
	info_handle.c info_handle.h

evtxinfo_LDADD  = \
	../libcstring/libcstring.la \
	../liberror/liberror.la \
	../libevtx/libevtx.la \
	../libsystem/libsystem.la \
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

