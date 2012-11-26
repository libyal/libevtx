AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCSTRING_CPPFLAGS@ \
	@LIBCERROR_CPPFLAGS@ \
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
	@LIBEXE_CPPFLAGS@ \
	@LIBREGF_CPPFLAGS@ \
	@LIBWRC_CPPFLAGS@ \
	@LIBCDIRECTORY_CPPFLAGS@ \
	@LIBCSYSTEM_CPPFLAGS@ \
	@LIBEVTX_DLL_IMPORT@

AM_LDFLAGS = @STATIC_LDFLAGS@

bin_PROGRAMS = \
	evtxexport \
	evtxinfo

evtxexport_SOURCES = \
	evtxexport.c \
	evtxinput.c evtxinput.h \
	evtxoutput.c evtxoutput.h \
	evtxtools_libcdirectory.h \
	evtxtools_libcerror.h \
	evtxtools_libclocale.h \
	evtxtools_libcnotify.h \
	evtxtools_libcpath.h \
	evtxtools_libcsplit.h \
	evtxtools_libcstring.h \
	evtxtools_libcsystem.h \
	evtxtools_libevtx.h \
	evtxtools_libfcache.h \
	evtxtools_libfdatetime.h \
	evtxtools_libfguid.h \
	evtxtools_libexe.h \
	evtxtools_libregf.h \
	evtxtools_libuna.h \
	evtxtools_libwrc.h \
	export_handle.c export_handle.h \
	log_handle.c log_handle.h \
	message_handle.c message_handle.h \
	message_string.c message_string.h \
	path_handle.c path_handle.h \
	registry_file.c registry_file.h \
	resource_file.c resource_file.h

evtxexport_LDADD = \
	@LIBCSYSTEM_LIBADD@ \
	@LIBREGF_LIBADD@ \
	@LIBWRC_LIBADD@ \
	@LIBEXE_LIBADD@ \
	@LIBFVALUE_LIBADD@ \
	@LIBFWNT_LIBADD@ \
	@LIBFWEVT_LIBADD@ \
	@LIBFGUID_LIBADD@ \
	@LIBFDATETIME_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCDIRECTORY_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	../libevtx/libevtx.la \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@LIBCSTRING_LIBADD@ \
	@LIBINTL@

evtxinfo_SOURCES = \
	evtxinfo.c \
	evtxinput.c evtxinput.h \
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

evtxinfo_LDADD = \
	@LIBCSYSTEM_LIBADD@ \
	../libevtx/libevtx.la \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@LIBCSTRING_LIBADD@ \
	@LIBINTL@

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f Makefile

splint:
	@echo "Running splint on evtxexport ..."
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(evtxexport_SOURCES)
	@echo "Running splint on evtxinfo ..."
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(evtxinfo_SOURCES)

