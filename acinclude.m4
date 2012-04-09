dnl Function to detect if libevtx dependencies are available
AC_DEFUN([AX_LIBEVTX_CHECK_LOCAL],
 [dnl Check for internationalization functions in libevtx/libevtx_i18n.c 
 AC_CHECK_FUNCS([bindtextdomain])
 ])

