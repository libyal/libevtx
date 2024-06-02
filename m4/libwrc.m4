dnl Checks for libwrc required headers and functions
dnl
dnl Version: 20240601

dnl Function to detect if libwrc is available
dnl ac_libwrc_dummy is used to prevent AC_CHECK_LIB adding unnecessary -l<library> arguments
AC_DEFUN([AX_LIBWRC_CHECK_LIB],
  [AS_IF(
    [test "x$ac_cv_enable_shared_libs" = xno || test "x$ac_cv_with_libwrc" = xno],
    [ac_cv_libwrc=no],
    [ac_cv_libwrc=check
    dnl Check if the directory provided as parameter exists
    dnl For both --with-libwrc which returns "yes" and --with-libwrc= which returns ""
    dnl treat them as auto-detection.
    AS_IF(
      [test "x$ac_cv_with_libwrc" != x && test "x$ac_cv_with_libwrc" != xauto-detect && test "x$ac_cv_with_libwrc" != xyes],
      [AX_CHECK_LIB_DIRECTORY_EXISTS([libwrc])],
      [dnl Check for a pkg-config file
      AS_IF(
        [test "x$cross_compiling" != "xyes" && test "x$PKGCONFIG" != "x"],
        [PKG_CHECK_MODULES(
          [libwrc],
          [libwrc >= 20211231],
          [ac_cv_libwrc=yes],
          [ac_cv_libwrc=check])
        ])
      AS_IF(
        [test "x$ac_cv_libwrc" = xyes],
        [ac_cv_libwrc_CPPFLAGS="$pkg_cv_libwrc_CFLAGS"
        ac_cv_libwrc_LIBADD="$pkg_cv_libwrc_LIBS"])
      ])

    AS_IF(
      [test "x$ac_cv_libwrc" = xcheck],
      [dnl Check for headers
      AC_CHECK_HEADERS([libwrc.h])

      AS_IF(
        [test "x$ac_cv_header_libwrc_h" = xno],
        [ac_cv_libwrc=no],
        [ac_cv_libwrc=yes

        AX_CHECK_LIB_FUNCTIONS(
          [libwrc],
          [wrc],
          [[libwrc_get_version],
           [libwrc_stream_initialize],
           [libwrc_stream_free],
           [libwrc_stream_signal_abort],
           [libwrc_stream_open],
           [libwrc_stream_close],
           [libwrc_stream_set_ascii_codepage],
           [libwrc_stream_get_virtual_address],
           [libwrc_stream_set_virtual_address],
           [libwrc_stream_get_number_of_resources],
           [libwrc_stream_get_resource_by_index],
           [libwrc_stream_get_resource_by_identifier],
           [libwrc_stream_get_resource_by_type],
           [libwrc_stream_get_resource_by_utf8_name],
           [libwrc_stream_get_resource_by_utf16_name],
           [libwrc_resource_free],
           [libwrc_resource_get_identifier],
           [libwrc_resource_get_utf8_name_size],
           [libwrc_resource_get_utf8_name],
           [libwrc_resource_get_utf16_name_size],
           [libwrc_resource_get_utf16_name],
           [libwrc_resource_get_type],
           [libwrc_resource_get_number_of_languages],
           [libwrc_resource_get_language_identifier],
           [libwrc_resource_get_number_of_items],
           [libwrc_resource_get_item_by_index],
           [libwrc_resource_item_free],
           [libwrc_resource_item_get_identifier],
           [libwrc_resource_item_get_utf8_name_size],
           [libwrc_resource_item_get_utf8_name],
           [libwrc_resource_item_get_utf16_name_size],
           [libwrc_resource_item_get_utf16_name],
           [libwrc_resource_item_read_buffer],
           [libwrc_resource_item_read_buffer_at_offset],
           [libwrc_resource_item_seek_offset],
           [libwrc_resource_item_get_offset],
           [libwrc_resource_item_get_size],
           [libwrc_resource_item_get_number_of_sub_items],
           [libwrc_resource_item_get_sub_item_by_index],
           [libwrc_message_table_resource_initialize],
           [libwrc_message_table_resource_free],
           [libwrc_message_table_resource_read],
           [libwrc_message_table_resource_get_number_of_messages],
           [libwrc_message_table_resource_get_identifier],
           [libwrc_message_table_resource_get_index_by_identifier],
           [libwrc_message_table_resource_get_utf8_string_size],
           [libwrc_message_table_resource_get_utf8_string],
           [libwrc_message_table_resource_get_utf16_string_size],
           [libwrc_message_table_resource_get_utf16_string],
           [libwrc_mui_resource_initialize],
           [libwrc_mui_resource_free],
           [libwrc_mui_resource_read],
           [libwrc_mui_resource_get_file_type],
           [libwrc_mui_resource_get_utf8_main_name_size],
           [libwrc_mui_resource_get_utf8_main_name],
           [libwrc_mui_resource_get_utf16_main_name_size],
           [libwrc_mui_resource_get_utf16_main_name],
           [libwrc_mui_resource_get_utf8_mui_name_size],
           [libwrc_mui_resource_get_utf8_mui_name],
           [libwrc_mui_resource_get_utf16_mui_name_size],
           [libwrc_mui_resource_get_utf16_mui_name],
           [libwrc_mui_resource_get_utf8_language_size],
           [libwrc_mui_resource_get_utf8_language],
           [libwrc_mui_resource_get_utf16_language_size],
           [libwrc_mui_resource_get_utf16_language],
           [libwrc_mui_resource_get_utf8_fallback_language_size],
           [libwrc_mui_resource_get_utf8_fallback_language],
           [libwrc_mui_resource_get_utf16_fallback_language_size],
           [libwrc_mui_resource_get_utf16_fallback_language]])

        AS_IF(
          [test "x$ac_cv_enable_wide_character_type" != xno],
          [AX_CHECK_LIB_FUNCTIONS(
            [libwrc],
            [wrc],
            [[libwrc_stream_open_wide]])
          ])

        dnl TODO add functions

        ac_cv_libwrc_LIBADD="-lwrc"])
      ])

    AX_CHECK_LIB_DIRECTORY_MSG_ON_FAILURE([libwrc])
    ])

  AS_IF(
    [test "x$ac_cv_libwrc" = xyes],
    [AC_DEFINE(
      [HAVE_LIBWRC],
      [1],
      [Define to 1 if you have the `wrc' library (-lwrc).])
    ])

  AS_IF(
    [test "x$ac_cv_libwrc" = xyes],
    [AC_SUBST(
      [HAVE_LIBWRC],
      [1]) ],
    [AC_SUBST(
      [HAVE_LIBWRC],
      [0])
    ])
  ])

dnl Function to detect if libwrc dependencies are available
AC_DEFUN([AX_LIBWRC_CHECK_LOCAL],
  [ac_cv_libwrc_CPPFLAGS="-I../libwrc -I\$(top_srcdir)/libwrc";
  ac_cv_libwrc_LIBADD="../libwrc/libwrc.la";

  ac_cv_libwrc=local
  ])

dnl Function to detect how to enable libwrc
AC_DEFUN([AX_LIBWRC_CHECK_ENABLE],
  [AX_COMMON_ARG_WITH(
    [libwrc],
    [libwrc],
    [search for libwrc in includedir and libdir or in the specified DIR, or no if to use local version],
    [auto-detect],
    [DIR])

  dnl Check for a shared library version
  AX_LIBWRC_CHECK_LIB

  dnl Check if the dependencies for the local library version
  AS_IF(
    [test "x$ac_cv_libwrc" != xyes],
    [AX_LIBWRC_CHECK_LOCAL

  AC_DEFINE(
    [HAVE_LOCAL_LIBWRC],
    [1],
    [Define to 1 if the local version of libwrc is used.])
  AC_SUBST(
    [HAVE_LOCAL_LIBWRC],
    [1])
  ])

  AM_CONDITIONAL(
    [HAVE_LOCAL_LIBWRC],
    [test "x$ac_cv_libwrc" = xlocal])
  AS_IF(
    [test "x$ac_cv_libwrc_CPPFLAGS" != "x"],
    [AC_SUBST(
      [LIBWRC_CPPFLAGS],
      [$ac_cv_libwrc_CPPFLAGS])
    ])
  AS_IF(
    [test "x$ac_cv_libwrc_LIBADD" != "x"],
    [AC_SUBST(
      [LIBWRC_LIBADD],
      [$ac_cv_libwrc_LIBADD])
    ])

  AS_IF(
    [test "x$ac_cv_libwrc" = xyes],
    [AC_SUBST(
      [ax_libwrc_pc_libs_private],
      [-lwrc])
    ])

  AS_IF(
    [test "x$ac_cv_libwrc" = xyes],
    [AC_SUBST(
      [ax_libwrc_spec_requires],
      [libwrc])
    AC_SUBST(
      [ax_libwrc_spec_build_requires],
      [libwrc-devel])
    ])
  ])
