AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCERROR_CPPFLAGS@ \
	@LIBCDATA_CPPFLAGS@ \
	@LIBCTHREADS_CPPFLAGS@ \
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
	@PTHREAD_CPPFLAGS@ \
	@LIBEVTX_DLL_IMPORT@

AM_LDFLAGS = @STATIC_LDFLAGS@

bin_PROGRAMS = \
	evtxexport \
	evtxinfo

evtxexport_SOURCES = \
	evtxexport.c \
	evtxinput.c evtxinput.h \
	evtxtools_getopt.c evtxtools_getopt.h \
	evtxtools_i18n.h \
	evtxtools_libbfio.h \
	evtxtools_libcdirectory.h \
	evtxtools_libcerror.h \
	evtxtools_libclocale.h \
	evtxtools_libcnotify.h \
	evtxtools_libcpath.h \
	evtxtools_libcsplit.h \
	evtxtools_libevtx.h \
	evtxtools_libfcache.h \
	evtxtools_libfdatetime.h \
	evtxtools_libfguid.h \
	evtxtools_libfvalue.h \
	evtxtools_libfwnt.h \
	evtxtools_libexe.h \
	evtxtools_libregf.h \
	evtxtools_libuna.h \
	evtxtools_libwrc.h \
	evtxtools_output.c evtxtools_output.h \
	evtxtools_signal.c evtxtools_signal.h \
	evtxtools_system_split_string.h \
	evtxtools_unused.h \
	evtxtools_wide_string.c evtxtools_wide_string.h \
	export_handle.c export_handle.h \
	log_handle.c log_handle.h \
	message_handle.c message_handle.h \
	message_string.c message_string.h \
	path_handle.c path_handle.h \
	registry_file.c registry_file.h \
	resource_file.c resource_file.h

evtxexport_LDADD = \
	@LIBREGF_LIBADD@ \
	@LIBWRC_LIBADD@ \
	@LIBEXE_LIBADD@ \
	@LIBFVALUE_LIBADD@ \
	@LIBFWNT_LIBADD@ \
	@LIBFWEVT_LIBADD@ \
	@LIBFGUID_LIBADD@ \
	@LIBFDATETIME_LIBADD@ \
	@LIBFDATA_LIBADD@ \
	@LIBFCACHE_LIBADD@ \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBCDIRECTORY_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libevtx/libevtx.la \
	@LIBCTHREADS_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@LIBINTL@ \
	@PTHREAD_LIBADD@

evtxinfo_SOURCES = \
	evtxinfo.c \
	evtxinput.c evtxinput.h \
	evtxtools_getopt.c evtxtools_getopt.h \
	evtxtools_i18n.h \
	evtxtools_libcerror.h \
	evtxtools_libclocale.h \
	evtxtools_libcnotify.h \
	evtxtools_libevtx.h \
	evtxtools_libfdatetime.h \
	evtxtools_libfguid.h \
	evtxtools_libfvalue.h \
	evtxtools_libfwnt.h \
	evtxtools_libuna.h \
	evtxtools_output.c evtxtools_output.h \
	evtxtools_signal.c evtxtools_signal.h \
	evtxtools_wide_string.c evtxtools_wide_string.h \
	evtxtools_unused.h \
	info_handle.c info_handle.h

evtxinfo_LDADD = \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	../libevtx/libevtx.la \
	@LIBCTHREADS_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@LIBINTL@ \
	@PTHREAD_LIBADD@

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f Makefile

splint:
	@echo "Running splint on evtxexport ..."
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(evtxexport_SOURCES)
	@echo "Running splint on evtxinfo ..."
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(evtxinfo_SOURCES)

