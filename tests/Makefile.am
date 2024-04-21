AUTOMAKE_OPTIONS = subdir-objects

AM_CPPFLAGS = \
	-I../include -I$(top_srcdir)/include \
	-I../common -I$(top_srcdir)/common \
	@LIBCERROR_CPPFLAGS@ \
	@LIBCTHREADS_CPPFLAGS@ \
	@LIBCDATA_CPPFLAGS@ \
	@LIBCLOCALE_CPPFLAGS@ \
	@LIBCNOTIFY_CPPFLAGS@ \
	@LIBCSPLIT_CPPFLAGS@ \
	@LIBUNA_CPPFLAGS@ \
	@LIBCDIRECTORY_CPPFLAGS@ \
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
	@PTHREAD_CPPFLAGS@ \
	@LIBEVTX_DLL_IMPORT@

if HAVE_PYTHON_TESTS
TESTS_PYEVTX = \
	test_python_module.sh
endif

TESTS = \
	test_library.sh \
	test_tools.sh \
	test_evtxinfo.sh \
	test_evtxexport.sh \
	$(TESTS_PYEVTX)

check_SCRIPTS = \
	pyevtx_test_file.py \
	pyevtx_test_support.py \
	test_evtxexport.sh \
	test_evtxinfo.sh \
	test_library.sh \
	test_manpage.sh \
	test_python_module.sh \
	test_runner.sh \
	test_tools.sh

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
	evtx_test_template_definition \
	evtx_test_tools_info_handle \
	evtx_test_tools_message_handle \
	evtx_test_tools_message_string \
	evtx_test_tools_output \
	evtx_test_tools_path_handle \
	evtx_test_tools_registry_file \
	evtx_test_tools_resource_file \
	evtx_test_tools_signal

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

evtx_test_tools_info_handle_SOURCES = \
	../evtxtools/evtxinput.c ../evtxtools/evtxinput.h \
	../evtxtools/info_handle.c ../evtxtools/info_handle.h \
	evtx_test_libcerror.h \
	evtx_test_macros.h \
	evtx_test_memory.c evtx_test_memory.h \
	evtx_test_tools_info_handle.c \
	evtx_test_unused.h

evtx_test_tools_info_handle_LDADD = \
	@LIBCLOCALE_LIBADD@ \
	../libevtx/libevtx.la \
	@LIBCERROR_LIBADD@

evtx_test_tools_message_handle_SOURCES = \
	../evtxtools/message_handle.c ../evtxtools/message_handle.h \
	../evtxtools/message_string.c ../evtxtools/message_string.h \
	../evtxtools/resource_file.c ../evtxtools/resource_file.h \
	../evtxtools/path_handle.c ../evtxtools/path_handle.h \
	../evtxtools/registry_file.c ../evtxtools/registry_file.h \
	evtx_test_libcerror.h \
	evtx_test_macros.h \
	evtx_test_memory.c evtx_test_memory.h \
	evtx_test_tools_message_handle.c \
	evtx_test_unused.h

evtx_test_tools_message_handle_LDADD = \
	@LIBWRC_LIBADD@ \
	@LIBREGF_LIBADD@ \
	@LIBEXE_LIBADD@ \
	@LIBFWEVT_LIBADD@ \
	@LIBFVALUE_LIBADD@ \
	@LIBFWNT_LIBADD@ \
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
	@LIBCERROR_LIBADD@

evtx_test_tools_message_string_SOURCES = \
	../evtxtools/message_string.c ../evtxtools/message_string.h \
	evtx_test_libcerror.h \
	evtx_test_macros.h \
	evtx_test_memory.c evtx_test_memory.h \
	evtx_test_tools_message_string.c \
	evtx_test_unused.h

evtx_test_tools_message_string_LDADD = \
	@LIBWRC_LIBADD@ \
	@LIBEXE_LIBADD@ \
	@LIBFWEVT_LIBADD@ \
	@LIBFVALUE_LIBADD@ \
	@LIBFWNT_LIBADD@ \
	@LIBFGUID_LIBADD@ \
	@LIBFDATETIME_LIBADD@ \
	@LIBFDATA_LIBADD@ \
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

evtx_test_tools_output_SOURCES = \
	../evtxtools/evtxtools_output.c ../evtxtools/evtxtools_output.h \
	evtx_test_libcerror.h \
	evtx_test_macros.h \
	evtx_test_tools_output.c \
	evtx_test_unused.h

evtx_test_tools_output_LDADD = \
	../libevtx/libevtx.la \
	@LIBCERROR_LIBADD@

evtx_test_tools_path_handle_SOURCES = \
	../evtxtools/path_handle.c ../evtxtools/path_handle.h \
	evtx_test_libcerror.h \
	evtx_test_macros.h \
	evtx_test_memory.c evtx_test_memory.h \
	evtx_test_tools_path_handle.c \
	evtx_test_unused.h

evtx_test_tools_path_handle_LDADD = \
	@LIBCDIRECTORY_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libevtx/libevtx.la \
	@LIBCERROR_LIBADD@

evtx_test_tools_registry_file_SOURCES = \
	../evtxtools/registry_file.c ../evtxtools/registry_file.h \
	evtx_test_libcerror.h \
	evtx_test_macros.h \
	evtx_test_memory.c evtx_test_memory.h \
	evtx_test_tools_registry_file.c \
	evtx_test_unused.h

evtx_test_tools_registry_file_LDADD = \
	@LIBREGF_LIBADD@ \
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
	@LIBCERROR_LIBADD@

evtx_test_tools_resource_file_SOURCES = \
	../evtxtools/message_string.c ../evtxtools/message_string.h \
	../evtxtools/resource_file.c ../evtxtools/resource_file.h \
	evtx_test_libcerror.h \
	evtx_test_macros.h \
	evtx_test_memory.c evtx_test_memory.h \
	evtx_test_tools_resource_file.c \
	evtx_test_unused.h

evtx_test_tools_resource_file_LDADD = \
	@LIBWRC_LIBADD@ \
	@LIBEXE_LIBADD@ \
	@LIBFWEVT_LIBADD@ \
	@LIBFVALUE_LIBADD@ \
	@LIBFWNT_LIBADD@ \
	@LIBFGUID_LIBADD@ \
	@LIBFDATETIME_LIBADD@ \
	@LIBFDATA_LIBADD@ \
	@LIBFCACHE_LIBADD@ \
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

evtx_test_tools_signal_SOURCES = \
	../evtxtools/evtxtools_signal.c ../evtxtools/evtxtools_signal.h \
	evtx_test_libcerror.h \
	evtx_test_macros.h \
	evtx_test_tools_signal.c \
	evtx_test_unused.h

evtx_test_tools_signal_LDADD = \
	../libevtx/libevtx.la \
	@LIBCERROR_LIBADD@

DISTCLEANFILES = \
	Makefile \
	Makefile.in \
	notify_stream.log

