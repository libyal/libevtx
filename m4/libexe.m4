dnl Checks for libexe required headers and functions
dnl
dnl Version: 20240601

dnl Function to detect if libexe is available
dnl ac_libexe_dummy is used to prevent AC_CHECK_LIB adding unnecessary -l<library> arguments
AC_DEFUN([AX_LIBEXE_CHECK_LIB],
  [AS_IF(
    [test "x$ac_cv_enable_shared_libs" = xno || test "x$ac_cv_with_libexe" = xno],
    [ac_cv_libexe=no],
    [ac_cv_libexe=check
    dnl Check if the directory provided as parameter exists
    dnl For both --with-libexe which returns "yes" and --with-libexe= which returns ""
    dnl treat them as auto-detection.
    AS_IF(
      [test "x$ac_cv_with_libexe" != x && test "x$ac_cv_with_libexe" != xauto-detect && test "x$ac_cv_with_libexe" != xyes],
      [AX_CHECK_LIB_DIRECTORY_EXISTS([libexe])],
      [dnl Check for a pkg-config file
      AS_IF(
        [test "x$cross_compiling" != "xyes" && test "x$PKGCONFIG" != "x"],
        [PKG_CHECK_MODULES(
          [libexe],
          [libexe >= 20120405],
          [ac_cv_libexe=yes],
          [ac_cv_libexe=check])
        ])
      AS_IF(
        [test "x$ac_cv_libexe" = xyes && test "x$ac_cv_enable_wide_character_type" != xno],
        [AC_CACHE_CHECK(
         [whether libexe/features.h defines LIBEXE_HAVE_WIDE_CHARACTER_TYPE as 1],
         [ac_cv_header_libexe_features_h_have_wide_character_type],
         [AC_LANG_PUSH(C)
         AC_COMPILE_IFELSE(
           [AC_LANG_PROGRAM(
             [[#include <libexe/features.h>]],
             [[#if !defined( LIBEXE_HAVE_WIDE_CHARACTER_TYPE ) || ( LIBEXE_HAVE_WIDE_CHARACTER_TYPE != 1 )
#error LIBEXE_HAVE_WIDE_CHARACTER_TYPE not defined
#endif]] )],
           [ac_cv_header_libexe_features_h_have_wide_character_type=yes],
           [ac_cv_header_libexe_features_h_have_wide_character_type=no])
         AC_LANG_POP(C)],
         [ac_cv_header_libexe_features_h_have_wide_character_type=no])

        AS_IF(
          [test "x$ac_cv_header_libexe_features_h_have_wide_character_type" = xno],
          [ac_cv_libexe=no])
        ])
      AS_IF(
        [test "x$ac_cv_libexe" = xyes],
        [ac_cv_libexe_CPPFLAGS="$pkg_cv_libexe_CFLAGS"
        ac_cv_libexe_LIBADD="$pkg_cv_libexe_LIBS"])
      ])

    AS_IF(
      [test "x$ac_cv_libexe" = xcheck],
      [dnl Check for headers
      AC_CHECK_HEADERS([libexe.h])

      AS_IF(
        [test "x$ac_cv_header_libexe_h" = xno],
        [ac_cv_libexe=no],
        [ac_cv_libexe=yes

        AX_CHECK_LIB_FUNCTIONS(
          [libexe],
          [exe],
          [[libexe_get_version],
           [libexe_file_initialize],
           [libexe_file_free],
           [libexe_file_signal_abort],
           [libexe_file_open],
           [libexe_file_close]])

        AS_IF(
          [test "x$ac_cv_enable_wide_character_type" != xno],
          [AX_CHECK_LIB_FUNCTIONS(
            [libexe],
            [exe],
            [[libexe_file_open_wide]])
          ])

        dnl TODO add functions

        ac_cv_libexe_LIBADD="-lexe"])
      ])

    AX_CHECK_LIB_DIRECTORY_MSG_ON_FAILURE([libexe])
    ])

  AS_IF(
    [test "x$ac_cv_libexe" = xyes],
    [AC_DEFINE(
      [HAVE_LIBEXE],
      [1],
      [Define to 1 if you have the `exe' library (-lexe).])
    ])

  AS_IF(
    [test "x$ac_cv_libexe" = xyes],
    [AC_SUBST(
      [HAVE_LIBEXE],
      [1]) ],
    [AC_SUBST(
      [HAVE_LIBEXE],
      [0])
    ])
  ])

dnl Function to detect if libexe dependencies are available
AC_DEFUN([AX_LIBEXE_CHECK_LOCAL],
  [ac_cv_libexe_CPPFLAGS="-I../libexe -I\$(top_srcdir)/libexe";
  ac_cv_libexe_LIBADD="../libexe/libexe.la";

  ac_cv_libexe=local
  ])

dnl Function to detect how to enable libexe
AC_DEFUN([AX_LIBEXE_CHECK_ENABLE],
  [AX_COMMON_ARG_WITH(
    [libexe],
    [libexe],
    [search for libexe in includedir and libdir or in the specified DIR, or no if to use local version],
    [auto-detect],
    [DIR])

  dnl Check for a shared library version
  AX_LIBEXE_CHECK_LIB

  dnl Check if the dependencies for the local library version
  AS_IF(
    [test "x$ac_cv_libexe" != xyes],
    [AX_LIBEXE_CHECK_LOCAL

  AC_DEFINE(
    [HAVE_LOCAL_LIBEXE],
    [1],
    [Define to 1 if the local version of libexe is used.])
  AC_SUBST(
    [HAVE_LOCAL_LIBEXE],
    [1])
  ])

  AM_CONDITIONAL(
    [HAVE_LOCAL_LIBEXE],
    [test "x$ac_cv_libexe" = xlocal])
  AS_IF(
    [test "x$ac_cv_libexe_CPPFLAGS" != "x"],
    [AC_SUBST(
      [LIBEXE_CPPFLAGS],
      [$ac_cv_libexe_CPPFLAGS])
    ])
  AS_IF(
    [test "x$ac_cv_libexe_LIBADD" != "x"],
    [AC_SUBST(
      [LIBEXE_LIBADD],
      [$ac_cv_libexe_LIBADD])
    ])

  AS_IF(
    [test "x$ac_cv_libexe" = xyes],
    [AC_SUBST(
      [ax_libexe_pc_libs_private],
      [-lexe])
    ])

  AS_IF(
    [test "x$ac_cv_libexe" = xyes],
    [AC_SUBST(
      [ax_libexe_spec_requires],
      [libexe])
    AC_SUBST(
      [ax_libexe_spec_build_requires],
      [libexe-devel])
    ])
  ])
