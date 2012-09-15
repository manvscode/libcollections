AC_INIT([libcollections], [0.1], [manvscode@gmail.com], [libcollections], [http://www.shrewdcreative.com/])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])

AM_SILENT_RULES([yes])
AC_PROG_CC
AC_PROG_CC_C99
AC_C_INLINE

AC_HEADER_STDC


dnl CFLAGS=" -Wall -pedantic -g -ggdb -pg -O0 -D_DEBUG_VECTOR -DUSE_ALLOCATORS -DUNICODE"
CFLAGS=" -Wall -pedantic -O3 -DUNICODE"
LDFLAGS=""

AC_PROG_RANLIB


dnl AC_CONFIG_HEADERS([src/config.h])

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
