dnl Function to detect if libevtx dependencies are available
AC_DEFUN([AX_LIBEVTX_CHECK_LOCAL],
 [dnl Headers included in libevtx/libevtx_xml_tag.h
 AC_CHECK_HEADERS([wctype.h])

 dnl Functions used in libevtx/libevtx_xml_tag.h
 AC_CHECK_FUNCS([towupper])

 AS_IF(
  [test "x$ac_cv_func_towupper" != xyes],
  [AC_MSG_FAILURE(
   [Missing function: towupper],
   [1])
  ])

 dnl Check for internationalization functions in libevtx/libevtx_i18n.c 
 AC_CHECK_FUNCS([bindtextdomain])
 ])

