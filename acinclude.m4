dnl Function to detect if libluksde dependencies are available
AC_DEFUN([AX_LIBLUKSDE_CHECK_LOCAL],
 [dnl Check for internationalization functions in libluksde/libluksde_i18n.c 
 AC_CHECK_FUNCS([bindtextdomain])
 ])

