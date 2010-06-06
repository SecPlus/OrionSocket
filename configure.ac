# Configure Script
# --

AC_PREREQ([2.63])
AC_INIT([orionsocket], [0.1], [tiago4orion@gmail.com])
AM_INIT_AUTOMAKE
AC_CONFIG_SRCDIR([src/socket.c])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC

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
