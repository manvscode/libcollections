AC_PREREQ(2.67)
AC_INIT([collections], [1.0.0], [manvscode@gmail.com], [collections], [https://joemarrero.com/])
AM_INIT_AUTOMAKE([1.13 foreign silent-rules dist-zip -Wall -Werror])

AC_CONFIG_MACRO_DIR([m4])

AM_SILENT_RULES([yes])
AC_CONFIG_HEADERS([src/libcollections-config.h])

AC_PROG_CC
AC_PROG_CC_STDC
AC_PROG_CC_C99
AC_HEADER_STDC
AC_C_INLINE
AC_TYPE_SIZE_T

AH_TOP([
#ifndef _LIBCOLLECTIONS_CONFIG_H_
#define _LIBCOLLECTIONS_CONFIG_H_
])
AH_BOTTOM([
#endif /* _LIBCOLLECTIONS_CONFIG_H_ */
])

# AC_CANONICAL_HOST is needed to access the 'host_os' variable
AC_CANONICAL_HOST

AM_PROG_AR


DFLAGS="-DUNICODE"
#CFLAGS="-std=c99 -Wall -pedantic -fsanitize=undefined -g -ggdb -O0 -D_DEBUG_VECTOR -DUNICODE"
#CFLAGS="$DFLAGS $CFLAGS"
#LDFLAGS="$LDFLAGS"


build_linux=no
build_windows=no
build_mac=no

# Detect the target system
case "${host_os}" in
    linux*)
        build_linux=yes
        ;;
    cygwin*|mingw*)
        build_windows=yes
        ;;
    darwin*)
        build_mac=yes
        ;;
    *)
        AC_MSG_ERROR(["OS $host_os is not supported"])
        ;;
esac

# Pass the conditionals to automake
AM_CONDITIONAL([LINUX], [test "$build_linux" = "yes"])
AM_CONDITIONAL([WINDOWS], [test "$build_windows" = "yes"])
AM_CONDITIONAL([OSX], [test "$build_mac" = "yes"])

LT_INIT([shared static win32-dll])

AS_IF([test "$build_linux" = "yes"], [
	AC_MSG_NOTICE([Building for Linux.])
],[test "$build_windows" = "yes"],[
	AC_MSG_NOTICE([Building for Microsoft Windows.])
	LDFLAGS="$LDFLAGS -no-undefined"
],[
	AC_MSG_NOTICE([Building for MacOS X.])
])

AC_PROG_LIBTOOL
AM_PROG_LIBTOOL

# -------------------------------------------------
AC_C_RESTRICT
AC_CHECK_FUNCS([ftruncate])
AC_CHECK_FUNCS([gettimeofday])
AC_CHECK_FUNCS([memmove])
AC_CHECK_FUNCS([memset])
AC_CHECK_FUNCS([pow])
AC_CHECK_FUNCS([setlocale])
AC_CHECK_FUNCS([strdup])
AC_CHECK_HEADERS([fcntl.h])
AC_CHECK_HEADERS([limits.h])
AC_CHECK_HEADERS([locale.h])
AC_CHECK_HEADERS([stddef.h])
AC_CHECK_HEADERS([stdint.h])
AC_CHECK_HEADERS([stdlib.h])
AC_CHECK_HEADERS([string.h])
AC_CHECK_HEADERS([sys/time.h])
AC_CHECK_HEADERS([unistd.h])
AC_CHECK_HEADERS([wchar.h])
AC_CHECK_HEADERS([wctype.h])
# -------------------------------------------------
AC_ARG_ENABLE([examples],
	[AS_HELP_STRING([--enable-examples], [Enable building the example programs.])],
	[:],
	[enable_examples=no])

AM_CONDITIONAL([ENABLE_EXAMPLES], [test "$enable_examples" = "yes"])
# -------------------------------------------------

AC_PROG_INSTALL

echo "------------------------------------------------------------------------------"
if [test -z "$CFLAGS"]; then
	echo "  CFLAGS: Not set"
else
	echo "  CFLAGS: $CFLAGS"
fi
if [test -z "$LDFLAGS"]; then
	echo " LDFLAGS: Not set"
else
	echo " LDFLAGS: $LDFLAGS"
fi
echo "------------------------------------------------------------------------------"

AC_CONFIG_FILES([
	Makefile
	src/Makefile
	examples/Makefile
	collections.pc
	collections-static.pc
])

AC_OUTPUT
