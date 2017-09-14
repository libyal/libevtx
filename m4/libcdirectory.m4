dnl Checks for libcdirectory or required headers and functions
dnl
dnl Version: 20170904

dnl Function to detect if libcdirectory is available
dnl ac_libcdirectory_dummy is used to prevent AC_CHECK_LIB adding unnecessary -l<library> arguments
AC_DEFUN([AX_LIBCDIRECTORY_CHECK_LIB],
  [dnl Check if parameters were provided
  AS_IF(
    [test "x$ac_cv_with_libcdirectory" != x && test "x$ac_cv_with_libcdirectory" != xno && test "x$ac_cv_with_libcdirectory" != xauto-detect],
    [AS_IF(
      [test -d "$ac_cv_with_libcdirectory"],
      [CFLAGS="$CFLAGS -I${ac_cv_with_libcdirectory}/include"
      LDFLAGS="$LDFLAGS -L${ac_cv_with_libcdirectory}/lib"],
      [AC_MSG_WARN([no such directory: $ac_cv_with_libcdirectory])
      ])
    ])

  AS_IF(
    [test "x$ac_cv_with_libcdirectory" = xno],
    [ac_cv_libcdirectory=no],
    [dnl Check for a pkg-config file
    AS_IF(
      [test "x$cross_compiling" != "xyes" && test "x$PKGCONFIG" != "x"],
      [PKG_CHECK_MODULES(
        [libcdirectory],
        [libcdirectory >= 20120423],
        [ac_cv_libcdirectory=yes],
        [ac_cv_libcdirectory=check])
      ])

    AS_IF(
      [test "x$ac_cv_libcdirectory" = xyes && test "x$ac_cv_enable_wide_character_type" != xno],
      [AC_CACHE_CHECK(
       [whether libcdirectory/features.h defines LIBCDIRECTORY_HAVE_WIDE_CHARACTER_TYPE as 1],
       [ac_cv_header_libcdirectory_features_h_have_wide_character_type],
       [AC_LANG_PUSH(C)
       AC_COMPILE_IFELSE(
         [AC_LANG_PROGRAM(
           [[#include <libcdirectory/features.h>]],
           [[#if !defined( LIBCDIRECTORY_HAVE_WIDE_CHARACTER_TYPE ) || ( LIBCDIRECTORY_HAVE_WIDE_CHARACTER_TYPE != 1 )
#error LIBCDIRECTORY_HAVE_WIDE_CHARACTER_TYPE not defined
##endif]] )],
         [ac_cv_header_libcdirectory_features_h_have_wide_character_type=yes],
         [ac_cv_header_libcdirectory_features_h_have_wide_character_type=no])
       AC_LANG_POP(C)],
       [ac_cv_header_libcdirectory_features_h_have_wide_character_type=no])

      AS_IF(
        [test "x$ac_cv_header_libcdirectory_features_h_have_wide_character_type" = xno],
        [ac_cv_libcdirectory=no])
    ])

    AS_IF(
      [test "x$ac_cv_libcdirectory" = xyes],
      [ac_cv_libcdirectory_CPPFLAGS="$pkg_cv_libcdirectory_CFLAGS"
      ac_cv_libcdirectory_LIBADD="$pkg_cv_libcdirectory_LIBS"])

    AS_IF(
      [test "x$ac_cv_libcdirectory" = xcheck],
      [dnl Check for headers
      AC_CHECK_HEADERS([libcdirectory.h])

      AS_IF(
        [test "x$ac_cv_header_libcdirectory_h" = xno],
        [ac_cv_libcdirectory=no],
        [dnl Check for the individual functions
        ac_cv_libcdirectory=yes

        AC_CHECK_LIB(
          cdirectory,
          libcdirectory_get_version,
          [ac_cv_libcdirectory_dummy=yes],
          [ac_cv_libcdirectory=no])

        dnl Directory functions
        AC_CHECK_LIB(
          cdirectory,
          libcdirectory_directory_initialize,
          [ac_cv_libcdirectory_dummy=yes],
          [ac_cv_libcdirectory=no])
        AC_CHECK_LIB(
          cdirectory,
          libcdirectory_directory_free,
          [ac_cv_libcdirectory_dummy=yes],
          [ac_cv_libcdirectory=no])

        AC_CHECK_LIB(
          cdirectory,
          libcdirectory_directory_open,
          [ac_cv_libcdirectory_dummy=yes],
          [ac_cv_libcdirectory=no])
        AC_CHECK_LIB(
          cdirectory,
          libcdirectory_directory_close,
          [ac_cv_libcdirectory_dummy=yes],
          [ac_cv_libcdirectory=no])

        AC_CHECK_LIB(
          cdirectory,
          libcdirectory_directory_read_entry,
          [ac_cv_libcdirectory_dummy=yes],
          [ac_cv_libcdirectory=no])
        AC_CHECK_LIB(
          cdirectory,
          libcdirectory_directory_has_entry,
          [ac_cv_libcdirectory_dummy=yes],
          [ac_cv_libcdirectory=no])

        AS_IF(
          [test "x$ac_cv_enable_wide_character_type" != xno],
          [AC_CHECK_LIB(
            cdirectory,
            libcdirectory_directory_open_wide,
            [ac_cv_libcdirectory_dummy=yes],
            [ac_cv_libcdirectory=no])
          AC_CHECK_LIB(
            cdirectory,
            libcdirectory_directory_has_entry_wide,
            [ac_cv_libcdirectory_dummy=yes],
            [ac_cv_libcdirectory=no])
          ])

        dnl Directory entry functions
        AC_CHECK_LIB(
          cdirectory,
          libcdirectory_directory_entry_initialize,
          [ac_cv_libcdirectory_dummy=yes],
          [ac_cv_libcdirectory=no])
        AC_CHECK_LIB(
          cdirectory,
          libcdirectory_directory_entry_free,
          [ac_cv_libcdirectory_dummy=yes],
          [ac_cv_libcdirectory=no])

        AC_CHECK_LIB(
          cdirectory,
          libcdirectory_directory_entry_get_type,
          [ac_cv_libcdirectory_dummy=yes],
          [ac_cv_libcdirectory=no])
        AC_CHECK_LIB(
          cdirectory,
          libcdirectory_directory_entry_get_name,
          [ac_cv_libcdirectory_dummy=yes],
          [ac_cv_libcdirectory=no])

        AS_IF(
          [test "x$ac_cv_enable_wide_character_type" != xno],
          [AC_CHECK_LIB(
            cdirectory,
            libcdirectory_directory_entry_get_name_wide,
            [ac_cv_libcdirectory_dummy=yes],
            [ac_cv_libcdirectory=no])
          ])

        ac_cv_libcdirectory_LIBADD="-lcdirectory"
        ])
      ])
    ])

  AS_IF(
    [test "x$ac_cv_libcdirectory" = xyes],
    [AC_DEFINE(
      [HAVE_LIBCDIRECTORY],
      [1],
      [Define to 1 if you have the `cdirectory' library (-lcdirectory).])
    ])

  AS_IF(
    [test "x$ac_cv_libcdirectory" = xyes],
    [AC_SUBST(
      [HAVE_LIBCDIRECTORY],
      [1]) ],
    [AC_SUBST(
      [HAVE_LIBCDIRECTORY],
      [0])
    ])
  ])

dnl Function to detect if libcdirectory dependencies are available
AC_DEFUN([AX_LIBCDIRECTORY_CHECK_LOCAL],
  [AS_IF(
    [test "x$ac_cv_enable_winapi" = xno],
    [dnl Headers included in libcdirectory/libcdirectory_directory.h
    AC_CHECK_HEADERS([dirent.h errno.h sys/stat.h])

    dnl Directory functions used in libcdirectory/libcdirectory_directory.h
    AC_CHECK_FUNCS([closedir opendir readdir_r])

    AS_IF(
      [test "x$ac_cv_func_closedir" != xyes],
      [AC_MSG_FAILURE(
        [Missing functions: closedir],
        [1])
      ])

    AS_IF(
      [test "x$ac_cv_func_opendir" != xyes],
      [AC_MSG_FAILURE(
        [Missing functions: opendir],
        [1])
      ])

    AS_IF(
      [test "x$ac_cv_func_readdir_r" != xyes],
      [AC_MSG_FAILURE(
        [Missing functions: readdir_r],
        [1])
      ])
    ])

  ac_cv_libcdirectory_CPPFLAGS="-I../libcdirectory";
  ac_cv_libcdirectory_LIBADD="../libcdirectory/libcdirectory.la";

  ac_cv_libcdirectory=local
  ])

dnl Function to detect how to enable libcdirectory
AC_DEFUN([AX_LIBCDIRECTORY_CHECK_ENABLE],
  [AX_COMMON_ARG_WITH(
    [libcdirectory],
    [libcdirectory],
    [search for libcdirectory in includedir and libdir or in the specified DIR, or no if to use local version],
    [auto-detect],
    [DIR])

  dnl Check for a shared library version
  AX_LIBCDIRECTORY_CHECK_LIB

  dnl Check if the dependencies for the local library version
  AS_IF(
    [test "x$ac_cv_libcdirectory" != xyes],
    [AX_LIBCDIRECTORY_CHECK_LOCAL

    AC_DEFINE(
      [HAVE_LOCAL_LIBCDIRECTORY],
      [1],
      [Define to 1 if the local version of libcdirectory is used.])
    AC_SUBST(
      [HAVE_LOCAL_LIBCDIRECTORY],
      [1])
    ])

  AM_CONDITIONAL(
    [HAVE_LOCAL_LIBCDIRECTORY],
    [test "x$ac_cv_libcdirectory" = xlocal])
  AS_IF(
    [test "x$ac_cv_libcdirectory_CPPFLAGS" != "x"],
    [AC_SUBST(
      [LIBCDIRECTORY_CPPFLAGS],
      [$ac_cv_libcdirectory_CPPFLAGS])
    ])
  AS_IF(
    [test "x$ac_cv_libcdirectory_LIBADD" != "x"],
    [AC_SUBST(
      [LIBCDIRECTORY_LIBADD],
      [$ac_cv_libcdirectory_LIBADD])
    ])

  AS_IF(
    [test "x$ac_cv_libcdirectory" = xyes],
    [AC_SUBST(
      [ax_libcdirectory_pc_libs_private],
      [-lstring])
    ])

  AS_IF(
    [test "x$ac_cv_libcdirectory" = xyes],
    [AC_SUBST(
      [ax_libcdirectory_spec_requires],
      [libcdirectory])
    AC_SUBST(
      [ax_libcdirectory_spec_build_requires],
      [libcdirectory-devel])
    ])
  ])

