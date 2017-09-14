dnl Functions for libfwevt
dnl
dnl Version: 20170908

dnl Function to detect if libfwevt is available
dnl ac_libfwevt_dummy is used to prevent AC_CHECK_LIB adding unnecessary -l<library> arguments
AC_DEFUN([AX_LIBFWEVT_CHECK_LIB],
  [dnl Check if parameters were provided
  AS_IF(
    [test "x$ac_cv_with_libfwevt" != x && test "x$ac_cv_with_libfwevt" != xno && test "x$ac_cv_with_libfwevt" != xauto-detect],
    [AS_IF(
      [test -d "$ac_cv_with_libfwevt"],
      [CFLAGS="$CFLAGS -I${ac_cv_with_libfwevt}/include"
      LDFLAGS="$LDFLAGS -L${ac_cv_with_libfwevt}/lib"],
      [AC_MSG_WARN([no such directory: $ac_cv_with_libfwevt])
      ])
    ])

  AS_IF(
    [test "x$ac_cv_with_libfwevt" = xno],
    [ac_cv_libfwevt=no],
    [dnl Check for a pkg-config file
    AS_IF(
      [test "x$cross_compiling" != "xyes" && test "x$PKGCONFIG" != "x"],
      [PKG_CHECK_MODULES(
        [libfwevt],
        [libfwevt >= 20160103],
        [ac_cv_libfwevt=yes],
        [ac_cv_libfwevt=no])
      ])

    AS_IF(
      [test "x$ac_cv_libfwevt" = xyes],
      [ac_cv_libfwevt_CPPFLAGS="$pkg_cv_libfwevt_CFLAGS"
      ac_cv_libfwevt_LIBADD="$pkg_cv_libfwevt_LIBS"],
      [dnl Check for headers
      AC_CHECK_HEADERS([libfwevt.h])

    AS_IF(
        [test "x$ac_cv_header_libfwevt_h" = xno],
        [ac_cv_libfwevt=no],
        [dnl Check for the individual functions
        ac_cv_libfwevt=yes

        AC_CHECK_LIB(
          fwevt,
          libfwevt_get_version,
          [ac_cv_libfwevt_dummy=yes],
          [ac_cv_libfwevt=no])

        dnl Channel functions
        AC_CHECK_LIB(
          fwevt,
          libfwevt_channel_initialize,
          [ac_cv_libfwevt_dummy=yes],
          [ac_cv_libfwevt=no])
        AC_CHECK_LIB(
          fwevt,
          libfwevt_channel_free,
          [ac_cv_libfwevt_dummy=yes],
          [ac_cv_libfwevt=no])

        AC_CHECK_LIB(
          fwevt,
          libfwevt_channel_read,
          [ac_cv_libfwevt_dummy=yes],
          [ac_cv_libfwevt=no])

        dnl Event functions
        AC_CHECK_LIB(
          fwevt,
          libfwevt_event_initialize,
          [ac_cv_libfwevt_dummy=yes],
          [ac_cv_libfwevt=no])
        AC_CHECK_LIB(
          fwevt,
          libfwevt_event_free,
          [ac_cv_libfwevt_dummy=yes],
          [ac_cv_libfwevt=no])

        AC_CHECK_LIB(
          fwevt,
          libfwevt_event_read,
          [ac_cv_libfwevt_dummy=yes],
          [ac_cv_libfwevt=no])
        AC_CHECK_LIB(
          fwevt,
          libfwevt_event_get_identifier,
          [ac_cv_libfwevt_dummy=yes],
          [ac_cv_libfwevt=no])
        AC_CHECK_LIB(
          fwevt,
          libfwevt_event_get_message_identifier,
          [ac_cv_libfwevt_dummy=yes],
          [ac_cv_libfwevt=no])
        AC_CHECK_LIB(
          fwevt,
          libfwevt_event_get_template_offset,
          [ac_cv_libfwevt_dummy=yes],
          [ac_cv_libfwevt=no])

        dnl Keyword functions
        AC_CHECK_LIB(
          fwevt,
          libfwevt_keyword_initialize,
          [ac_cv_libfwevt_dummy=yes],
          [ac_cv_libfwevt=no])
        AC_CHECK_LIB(
          fwevt,
          libfwevt_keyword_free,
          [ac_cv_libfwevt_dummy=yes],
          [ac_cv_libfwevt=no])

        AC_CHECK_LIB(
          fwevt,
          libfwevt_keyword_read,
          [ac_cv_libfwevt_dummy=yes],
          [ac_cv_libfwevt=no])

        dnl Level functions
        AC_CHECK_LIB(
          fwevt,
          libfwevt_level_initialize,
          [ac_cv_libfwevt_dummy=yes],
          [ac_cv_libfwevt=no])
        AC_CHECK_LIB(
          fwevt,
          libfwevt_level_free,
          [ac_cv_libfwevt_dummy=yes],
          [ac_cv_libfwevt=no])

        AC_CHECK_LIB(
          fwevt,
          libfwevt_level_read,
          [ac_cv_libfwevt_dummy=yes],
          [ac_cv_libfwevt=no])

        dnl Manifest functions
        AC_CHECK_LIB(
          fwevt,
          libfwevt_manifest_initialize,
          [ac_cv_libfwevt_dummy=yes],
          [ac_cv_libfwevt=no])
        AC_CHECK_LIB(
          fwevt,
          libfwevt_manifest_free,
          [ac_cv_libfwevt_dummy=yes],
          [ac_cv_libfwevt=no])

        AC_CHECK_LIB(
          fwevt,
          libfwevt_manifest_read,
          [ac_cv_libfwevt_dummy=yes],
          [ac_cv_libfwevt=no])
        AC_CHECK_LIB(
          fwevt,
          libfwevt_manifest_get_number_of_providers,
          [ac_cv_libfwevt_dummy=yes],
          [ac_cv_libfwevt=no])
        AC_CHECK_LIB(
          fwevt,
          libfwevt_manifest_get_provider,
          [ac_cv_libfwevt_dummy=yes],
          [ac_cv_libfwevt=no])
        AC_CHECK_LIB(
          fwevt,
          libfwevt_manifest_get_provider_by_identifier,
          [ac_cv_libfwevt_dummy=yes],
          [ac_cv_libfwevt=no])

        dnl Map functions
        AC_CHECK_LIB(
          fwevt,
          libfwevt_map_initialize,
          [ac_cv_libfwevt_dummy=yes],
          [ac_cv_libfwevt=no])
        AC_CHECK_LIB(
          fwevt,
          libfwevt_map_free,
          [ac_cv_libfwevt_dummy=yes],
          [ac_cv_libfwevt=no])

        AC_CHECK_LIB(
          fwevt,
          libfwevt_map_read,
          [ac_cv_libfwevt_dummy=yes],
          [ac_cv_libfwevt=no])

        dnl Opcode functions
        AC_CHECK_LIB(
          fwevt,
          libfwevt_opcode_initialize,
          [ac_cv_libfwevt_dummy=yes],
          [ac_cv_libfwevt=no])
        AC_CHECK_LIB(
          fwevt,
          libfwevt_opcode_free,
          [ac_cv_libfwevt_dummy=yes],
          [ac_cv_libfwevt=no])

        AC_CHECK_LIB(
          fwevt,
          libfwevt_opcode_read,
          [ac_cv_libfwevt_dummy=yes],
          [ac_cv_libfwevt=no])

        dnl Provider functions
        AC_CHECK_LIB(
          fwevt,
          libfwevt_provider_initialize,
          [ac_cv_libfwevt_dummy=yes],
          [ac_cv_libfwevt=no])
        AC_CHECK_LIB(
          fwevt,
          libfwevt_provider_free,
          [ac_cv_libfwevt_dummy=yes],
          [ac_cv_libfwevt=no])

        AC_CHECK_LIB(
          fwevt,
          libfwevt_provider_read,
          [ac_cv_libfwevt_dummy=yes],
          [ac_cv_libfwevt=no])
        AC_CHECK_LIB(
          fwevt,
          libfwevt_provider_read_channels,
          [ac_cv_libfwevt_dummy=yes],
          [ac_cv_libfwevt=no])
        AC_CHECK_LIB(
          fwevt,
          libfwevt_provider_read_events,
          [ac_cv_libfwevt_dummy=yes],
          [ac_cv_libfwevt=no])
        AC_CHECK_LIB(
          fwevt,
          libfwevt_provider_read_keywords,
          [ac_cv_libfwevt_dummy=yes],
          [ac_cv_libfwevt=no])
        AC_CHECK_LIB(
          fwevt,
          libfwevt_provider_read_levels,
          [ac_cv_libfwevt_dummy=yes],
          [ac_cv_libfwevt=no])
        AC_CHECK_LIB(
          fwevt,
          libfwevt_provider_read_maps,
          [ac_cv_libfwevt_dummy=yes],
          [ac_cv_libfwevt=no])
        AC_CHECK_LIB(
          fwevt,
          libfwevt_provider_read_opcodes,
          [ac_cv_libfwevt_dummy=yes],
          [ac_cv_libfwevt=no])
        AC_CHECK_LIB(
          fwevt,
          libfwevt_provider_read_tasks,
          [ac_cv_libfwevt_dummy=yes],
          [ac_cv_libfwevt=no])
        AC_CHECK_LIB(
          fwevt,
          libfwevt_provider_read_templates,
          [ac_cv_libfwevt_dummy=yes],
          [ac_cv_libfwevt=no])

        AC_CHECK_LIB(
          fwevt,
          libfwevt_provider_compare_identifier,
          [ac_cv_libfwevt_dummy=yes],
          [ac_cv_libfwevt=no])
        AC_CHECK_LIB(
          fwevt,
          libfwevt_provider_get_number_of_channels,
          [ac_cv_libfwevt_dummy=yes],
          [ac_cv_libfwevt=no])
        AC_CHECK_LIB(
          fwevt,
          libfwevt_provider_get_channel,
          [ac_cv_libfwevt_dummy=yes],
          [ac_cv_libfwevt=no])
        AC_CHECK_LIB(
          fwevt,
          libfwevt_provider_get_number_of_events,
          [ac_cv_libfwevt_dummy=yes],
          [ac_cv_libfwevt=no])
        AC_CHECK_LIB(
          fwevt,
          libfwevt_provider_get_event,
          [ac_cv_libfwevt_dummy=yes],
          [ac_cv_libfwevt=no])
        AC_CHECK_LIB(
          fwevt,
          libfwevt_provider_get_event_by_identifier,
          [ac_cv_libfwevt_dummy=yes],
          [ac_cv_libfwevt=no])
        AC_CHECK_LIB(
          fwevt,
          libfwevt_provider_get_number_of_keywords,
          [ac_cv_libfwevt_dummy=yes],
          [ac_cv_libfwevt=no])
        AC_CHECK_LIB(
          fwevt,
          libfwevt_provider_get_keyword,
          [ac_cv_libfwevt_dummy=yes],
          [ac_cv_libfwevt=no])
        AC_CHECK_LIB(
          fwevt,
          libfwevt_provider_get_number_of_levels,
          [ac_cv_libfwevt_dummy=yes],
          [ac_cv_libfwevt=no])
        AC_CHECK_LIB(
          fwevt,
          libfwevt_provider_get_level,
          [ac_cv_libfwevt_dummy=yes],
          [ac_cv_libfwevt=no])
        AC_CHECK_LIB(
          fwevt,
          libfwevt_provider_get_number_of_maps,
          [ac_cv_libfwevt_dummy=yes],
          [ac_cv_libfwevt=no])
        AC_CHECK_LIB(
          fwevt,
          libfwevt_provider_get_map,
          [ac_cv_libfwevt_dummy=yes],
          [ac_cv_libfwevt=no])
        AC_CHECK_LIB(
          fwevt,
          libfwevt_provider_get_number_of_opcodes,
          [ac_cv_libfwevt_dummy=yes],
          [ac_cv_libfwevt=no])
        AC_CHECK_LIB(
          fwevt,
          libfwevt_provider_get_opcode,
          [ac_cv_libfwevt_dummy=yes],
          [ac_cv_libfwevt=no])
        AC_CHECK_LIB(
          fwevt,
          libfwevt_provider_get_number_of_tasks,
          [ac_cv_libfwevt_dummy=yes],
          [ac_cv_libfwevt=no])
        AC_CHECK_LIB(
          fwevt,
          libfwevt_provider_get_task,
          [ac_cv_libfwevt_dummy=yes],
          [ac_cv_libfwevt=no])
        AC_CHECK_LIB(
          fwevt,
          libfwevt_provider_get_number_of_templates,
          [ac_cv_libfwevt_dummy=yes],
          [ac_cv_libfwevt=no])
        AC_CHECK_LIB(
          fwevt,
          libfwevt_provider_get_template,
          [ac_cv_libfwevt_dummy=yes],
          [ac_cv_libfwevt=no])
        AC_CHECK_LIB(
          fwevt,
          libfwevt_provider_get_template_by_offset,
          [ac_cv_libfwevt_dummy=yes],
          [ac_cv_libfwevt=no])

        dnl Task functions
        AC_CHECK_LIB(
          fwevt,
          libfwevt_task_initialize,
          [ac_cv_libfwevt_dummy=yes],
          [ac_cv_libfwevt=no])
        AC_CHECK_LIB(
          fwevt,
          libfwevt_task_free,
          [ac_cv_libfwevt_dummy=yes],
          [ac_cv_libfwevt=no])

        AC_CHECK_LIB(
          fwevt,
          libfwevt_task_read,
          [ac_cv_libfwevt_dummy=yes],
          [ac_cv_libfwevt=no])

        dnl Template functions
        AC_CHECK_LIB(
          fwevt,
          libfwevt_template_initialize,
          [ac_cv_libfwevt_dummy=yes],
          [ac_cv_libfwevt=no])
        AC_CHECK_LIB(
          fwevt,
          libfwevt_template_free,
          [ac_cv_libfwevt_dummy=yes],
          [ac_cv_libfwevt=no])

        AC_CHECK_LIB(
          fwevt,
          libfwevt_template_read,
          [ac_cv_libfwevt_dummy=yes],
          [ac_cv_libfwevt=no])
        AC_CHECK_LIB(
          fwevt,
          libfwevt_template_read_xml_document,
          [ac_cv_libfwevt_dummy=yes],
          [ac_cv_libfwevt=no])

        AC_CHECK_LIB(
          fwevt,
          libfwevt_template_set_ascii_codepage,
          [ac_cv_libfwevt_dummy=yes],
          [ac_cv_libfwevt=no])
        AC_CHECK_LIB(
          fwevt,
          libfwevt_template_get_data,
          [ac_cv_libfwevt_dummy=yes],
          [ac_cv_libfwevt=no])
        AC_CHECK_LIB(
          fwevt,
          libfwevt_template_set_data
          [ac_cv_libfwevt_dummy=yes],
          [ac_cv_libfwevt=no])
        AC_CHECK_LIB(
          fwevt,
          libfwevt_template_get_offset
          [ac_cv_libfwevt_dummy=yes],
          [ac_cv_libfwevt=no])
        AC_CHECK_LIB(
          fwevt,
          libfwevt_template_set_offset
          [ac_cv_libfwevt_dummy=yes],
          [ac_cv_libfwevt=no])
        AC_CHECK_LIB(
          fwevt,
          libfwevt_template_get_size
          [ac_cv_libfwevt_dummy=yes],
          [ac_cv_libfwevt=no])

        dnl XML document functions
        AC_CHECK_LIB(
          fwevt,
          libfwevt_xml_document_initialize,
          [ac_cv_libfwevt_dummy=yes],
          [ac_cv_libfwevt=no])
        AC_CHECK_LIB(
          fwevt,
          libfwevt_xml_document_free,
          [ac_cv_libfwevt_dummy=yes],
          [ac_cv_libfwevt=no])
        AC_CHECK_LIB(
          fwevt,
          libfwevt_xml_document_clone,
          [ac_cv_libfwevt_dummy=yes],
          [ac_cv_libfwevt=no])

        AC_CHECK_LIB(
          fwevt,
          libfwevt_xml_document_get_root_xml_tag,
          [ac_cv_libfwevt_dummy=yes],
          [ac_cv_libfwevt=no])
        AC_CHECK_LIB(
          fwevt,
          libfwevt_xml_document_read,
          [ac_cv_libfwevt_dummy=yes],
          [ac_cv_libfwevt=no])
        AC_CHECK_LIB(
          fwevt,
          libfwevt_xml_document_get_utf8_xml_string_size,
          [ac_cv_libfwevt_dummy=yes],
          [ac_cv_libfwevt=no])
        AC_CHECK_LIB(
          fwevt,
          libfwevt_xml_document_get_utf8_xml_string,
          [ac_cv_libfwevt_dummy=yes],
          [ac_cv_libfwevt=no])
        AC_CHECK_LIB(
          fwevt,
          libfwevt_xml_document_get_utf16_xml_string_size,
          [ac_cv_libfwevt_dummy=yes],
          [ac_cv_libfwevt=no])
        AC_CHECK_LIB(
          fwevt,
          libfwevt_xml_document_get_utf16_xml_string,
          [ac_cv_libfwevt_dummy=yes],
          [ac_cv_libfwevt=no])

        AS_IF(
          [test "x$ac_cv_enable_debug_output" != xno ],
          [AC_CHECK_LIB(
            fwevt,
            libfwevt_xml_document_debug_print,
            [ac_cv_libfwevt_dummy=yes],
            [ac_cv_libfwevt=no])
        ])

        dnl XML tag functions

        AC_CHECK_LIB(
          fwevt,
          libfwevt_xml_tag_get_utf8_name_size,
          [ac_cv_libfwevt_dummy=yes],
          [ac_cv_libfwevt=no])
        AC_CHECK_LIB(
          fwevt,
          libfwevt_xml_tag_get_utf8_name,
          [ac_cv_libfwevt_dummy=yes],
          [ac_cv_libfwevt=no])
        AC_CHECK_LIB(
          fwevt,
          libfwevt_xml_tag_get_utf16_name_size,
          [ac_cv_libfwevt_dummy=yes],
          [ac_cv_libfwevt=no])
        AC_CHECK_LIB(
          fwevt,
          libfwevt_xml_tag_get_utf16_name,
          [ac_cv_libfwevt_dummy=yes],
          [ac_cv_libfwevt=no])
        AC_CHECK_LIB(
          fwevt,
          libfwevt_xml_tag_get_utf8_value_size,
          [ac_cv_libfwevt_dummy=yes],
          [ac_cv_libfwevt=no])
        AC_CHECK_LIB(
          fwevt,
          libfwevt_xml_tag_get_utf8_value,
          [ac_cv_libfwevt_dummy=yes],
          [ac_cv_libfwevt=no])
        AC_CHECK_LIB(
          fwevt,
          libfwevt_xml_tag_get_utf16_value_size,
          [ac_cv_libfwevt_dummy=yes],
          [ac_cv_libfwevt=no])
        AC_CHECK_LIB(
          fwevt,
          libfwevt_xml_tag_get_utf16_value,
          [ac_cv_libfwevt_dummy=yes],
          [ac_cv_libfwevt=no])

        AC_CHECK_LIB(
          fwevt,
          libfwevt_xml_tag_get_number_of_attributes,
          [ac_cv_libfwevt_dummy=yes],
          [ac_cv_libfwevt=no])
        AC_CHECK_LIB(
          fwevt,
          libfwevt_xml_tag_get_attribute_by_index,
          [ac_cv_libfwevt_dummy=yes],
          [ac_cv_libfwevt=no])
        AC_CHECK_LIB(
          fwevt,
          libfwevt_xml_tag_get_attribute_by_utf8_name,
          [ac_cv_libfwevt_dummy=yes],
          [ac_cv_libfwevt=no])
        AC_CHECK_LIB(
          fwevt,
          libfwevt_xml_tag_get_attribute_by_utf16_name,
          [ac_cv_libfwevt_dummy=yes],
          [ac_cv_libfwevt=no])

        AC_CHECK_LIB(
          fwevt,
          libfwevt_xml_tag_get_number_of_elements,
          [ac_cv_libfwevt_dummy=yes],
          [ac_cv_libfwevt=no])
        AC_CHECK_LIB(
          fwevt,
          libfwevt_xml_tag_get_element_by_index,
          [ac_cv_libfwevt_dummy=yes],
          [ac_cv_libfwevt=no])
        AC_CHECK_LIB(
          fwevt,
          libfwevt_xml_tag_get_element_by_utf8_name,
          [ac_cv_libfwevt_dummy=yes],
          [ac_cv_libfwevt=no])
        AC_CHECK_LIB(
          fwevt,
          libfwevt_xml_tag_get_element_by_utf16_name,
          [ac_cv_libfwevt_dummy=yes],
          [ac_cv_libfwevt=no])

        AC_CHECK_LIB(
          fwevt,
          libfwevt_xml_tag_get_flags,
          [ac_cv_libfwevt_dummy=yes],
          [ac_cv_libfwevt=no])

        ac_cv_libfwevt_LIBADD="-lfwevt"
        ])
      ])
    ])

  AS_IF(
    [test "x$ac_cv_libfwevt" = xyes],
    [AC_DEFINE(
      [HAVE_LIBFWEVT],
      [1],
      [Define to 1 if you have the `fwevt' library (-lfwevt).])
    ])

  AS_IF(
    [test "x$ac_cv_libfwevt" = xyes],
    [AC_SUBST(
      [HAVE_LIBFWEVT],
      [1]) ],
    [AC_SUBST(
      [HAVE_LIBFWEVT],
      [0])
    ])
  ])

dnl Function to detect if libfwevt dependencies are available
AC_DEFUN([AX_LIBFWEVT_CHECK_LOCAL],
  [dnl No additional checks.

  ac_cv_libfwevt_CPPFLAGS="-I../libfwevt";
  ac_cv_libfwevt_LIBADD="../libfwevt/libfwevt.la";

  ac_cv_libfwevt=local
  ])

dnl Function to detect how to enable libfwevt
AC_DEFUN([AX_LIBFWEVT_CHECK_ENABLE],
  [AX_COMMON_ARG_WITH(
    [libfwevt],
    [libfwevt],
    [search for libfwevt in includedir and libdir or in the specified DIR, or no if to use local version],
    [auto-detect],
    [DIR])

  dnl Check for a shared library version
  AX_LIBFWEVT_CHECK_LIB

  dnl Check if the dependencies for the local library version
  AS_IF(
    [test "x$ac_cv_libfwevt" != xyes],
    [AX_LIBFWEVT_CHECK_LOCAL

    AC_DEFINE(
      [HAVE_LOCAL_LIBFWEVT],
      [1],
      [Define to 1 if the local version of libfwevt is used.])
    AC_SUBST(
      [HAVE_LOCAL_LIBFWEVT],
      [1])
    ])

  AM_CONDITIONAL(
    [HAVE_LOCAL_LIBFWEVT],
    [test "x$ac_cv_libfwevt" = xlocal])
  AS_IF(
    [test "x$ac_cv_libfwevt_CPPFLAGS" != "x"],
    [AC_SUBST(
      [LIBFWEVT_CPPFLAGS],
      [$ac_cv_libfwevt_CPPFLAGS])
    ])
  AS_IF(
    [test "x$ac_cv_libfwevt_LIBADD" != "x"],
    [AC_SUBST(
      [LIBFWEVT_LIBADD],
      [$ac_cv_libfwevt_LIBADD])
    ])

  AS_IF(
    [test "x$ac_cv_libfwevt" = xyes],
    [AC_SUBST(
      [ax_libfwevt_pc_libs_private],
      [-lfwevt])
    ])

  AS_IF(
    [test "x$ac_cv_libfwevt" = xyes],
    [AC_SUBST(
      [ax_libfwevt_spec_requires],
      [libfwevt])
    AC_SUBST(
      [ax_libfwevt_spec_build_requires],
      [libfwevt-devel])
    ])
  ])

