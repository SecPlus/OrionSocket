# Configure Script
# --

AC_INIT([orionsocket], [1.0.5], [tiago4orion@gmail.com])
AM_INIT_AUTOMAKE
LT_INIT
AC_CONFIG_SRCDIR([src/socket.c])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])
# Checks for programs.
AC_PROG_CC

AC_ARG_ENABLE([debug],[AS_HELP_STRING([--enable-debug],
                    [Print on the screen some debugging information. @<:@default=no@:>@])],
            [with_debug=$enableval], [with_debug=no])

if test "x$with_debug" == "xyes"
then
    AC_DEFINE([ORIONSOCKET_DEBUG], [1], [Print debug information])
fi
# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([arpa/inet.h netdb.h netinet/in.h stdint.h stdlib.h string.h sys/socket.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([bzero gethostbyname inet_ntoa memset socket])

AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT
