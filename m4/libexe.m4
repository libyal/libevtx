dnl Functions for libexe
dnl
dnl Version: 20120501

dnl Function to detect if libexe is available
dnl ac_libexe_dummy is used to prevent AC_CHECK_LIB adding unnecessary -l<library> arguments
AC_DEFUN([AX_LIBEXE_CHECK_LIB],
 [dnl Check if parameters were provided
 AS_IF(
  [test "x$ac_cv_with_libexe" != x && test "x$ac_cv_with_libexe" != xno && test "x$ac_cv_with_libexe" != xauto-detect],
  [AS_IF(
   [test -d "$ac_cv_with_libexe"],
   [CFLAGS="$CFLAGS -I${ac_cv_with_libexe}/include"
   LDFLAGS="$LDFLAGS -L${ac_cv_with_libexe}/lib"],
   [AC_MSG_WARN([no such directory: $ac_cv_with_libexe])
   ])
  ])

 AS_IF(
  [test "x$ac_cv_with_libexe" = xno],
  [ac_cv_libexe=no],
  [dnl Check for a pkg-config file
  AS_IF(
   [test "x$cross_compiling" != "xyes" && test "x$PKGCONFIG" != "x"],
   [PKG_CHECK_MODULES(
    [libexe],
    [libexe >= 20120405],
    [ac_cv_libexe=yes],
    [ac_cv_libexe=no])
   ])

  AS_IF(
   [test "x$ac_cv_libexe" = xyes],
   [ac_cv_libexe_CPPFLAGS="$pkg_cv_libexe_CFLAGS"
   ac_cv_libexe_LIBADD="$pkg_cv_libexe_LIBS"],
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
   
    dnl TODO add functions
 
    ac_cv_libexe_LIBADD="-lexe"
    ])
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
  [ac_cv_libexe_CPPFLAGS="-I../libexe";
  ac_cv_libexe_LIBADD="../libexe/libexe.la";

  ac_cv_libexe=local

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
