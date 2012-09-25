AC_INIT([libcollections], [0.1], [manvscode@gmail.com], [libcollections], [http://www.shrewdcreative.com/])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])

AM_SILENT_RULES([yes])
AC_PROG_CC
AC_PROG_CC_C99
AC_C_INLINE

AC_HEADER_STDC


dnlCFLAGS=" -Wall -pedantic -g -ggdb -pg -O0 -D_DEBUG_VECTOR -DUNICODE"
CFLAGS=" -Wall -pedantic -O3 -DUNICODE"
LDFLAGS=""


LT_INIT([shared static])

AC_PROG_LIBTOOL
AM_PROG_LIBTOOL
AC_PROG_RANLIB


AC_ARG_ENABLE([allocators],
    AS_HELP_STRING([--disable-allocators], [Disable exposed allocators]))

AS_IF([test "x$enable_allocators" != "xno"], [
	CFLAGS="$CFLAGS -DUSE_ALLOCATORS"
	AC_DEFINE(USE_ALLOCATORS, 1, [Expose memory allocation and deallocation.])
])


AC_CONFIG_HEADERS([src/config.h])

AC_PROG_INSTALL

echo " ------------------------------ "
echo ""
echo "  CFLAGS = $CFLAGS"
echo " LDFLAGS = $LDFLAGS"
echo ""
echo " ------------------------------ "

AC_CONFIG_FILES([
	Makefile
	src/Makefile
	tests/Makefile
])

AC_OUTPUT
