dnl Checks for libexe required headers and functions
dnl
dnl Version: 20211231

dnl Function to detect if libexe is available
dnl ac_libexe_dummy is used to prevent AC_CHECK_LIB adding unnecessary -l<library> arguments
AC_DEFUN([AX_LIBEXE_CHECK_LIB],
  [AS_IF(
    [test "x$ac_cv_enable_shared_libs" = xno || test "x$ac_cv_with_libexe" = xno],
    [ac_cv_libexe=no],
    [ac_cv_libexe=check
    dnl Check if the directory provided as parameter exists
    AS_IF(
      [test "x$ac_cv_with_libexe" != x && test "x$ac_cv_with_libexe" != xauto-detect],
      [AS_IF(
        [test -d "$ac_cv_with_libexe"],
        [CFLAGS="$CFLAGS -I${ac_cv_with_libexe}/include"
        LDFLAGS="$LDFLAGS -L${ac_cv_with_libexe}/lib"],
        [AC_MSG_FAILURE(
          [no such directory: $ac_cv_with_libexe],
          [1])
        ])
      ],
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
        [dnl Check for the individual functions
        ac_cv_libexe=yes

        AC_CHECK_LIB(
          exe,
          libexe_get_version,
          [ac_cv_libexe_dummy=yes],
          [ac_cv_libexe=no])

        dnl File functions
        AC_CHECK_LIB(
          exe,
          libexe_file_initialize,
          [ac_cv_libexe_dummy=yes],
          [ac_cv_libexe=no])
        AC_CHECK_LIB(
          exe,
          libexe_file_free,
          [ac_cv_libexe_dummy=yes],
          [ac_cv_libexe=no])
        AC_CHECK_LIB(
          exe,
          libexe_file_signal_abort,
          [ac_cv_libexe_dummy=yes],
          [ac_cv_libexe=no])

        AC_CHECK_LIB(
          exe,
          libexe_file_open,
          [ac_cv_libexe_dummy=yes],
          [ac_cv_libexe=no])
        AC_CHECK_LIB(
          exe,
          libexe_file_close,
          [ac_cv_libexe_dummy=yes],
          [ac_cv_libexe=no])

        AS_IF(
          [test "x$ac_cv_enable_wide_character_type" != xno],
          [AC_CHECK_LIB(
            exe,
            libexe_file_open,
            [ac_cv_libexe_dummy=yes],
            [ac_cv_libexe=no])
          ])

        dnl TODO add functions

        ac_cv_libexe_LIBADD="-lexe"])
      ])
    AS_IF(
      [test "x$ac_cv_with_libexe" != x && test "x$ac_cv_with_libexe" != xauto-detect && test "x$ac_cv_libexe" != xyes],
      [AC_MSG_FAILURE(
        [unable to find supported libexe in directory: $ac_cv_with_libexe],
        [1])
      ])
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
  [ac_cv_libexe_CPPFLAGS="-I../libexe";
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
