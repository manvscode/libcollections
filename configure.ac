AC_INIT([libcollections], [0.1], [manvscode@gmail.com], [libcollections], [http://www.shrewdcreative.com/])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])

AM_SILENT_RULES([yes])
AC_CONFIG_HEADERS([src/libcollections-config.h])

AC_PROG_CC
AC_PROG_CC_C99
AC_C_INLINE
AC_HEADER_STDC


AH_TOP([
#ifndef _LIBCOLLECTIONS_CONFIG_H_
#define _LIBCOLLECTIONS_CONFIG_H_
])
AH_BOTTOM([
#endif /* _LIBCOLLECTIONS_CONFIG_H_ */
])

#CFLAGS=" -Wall -pedantic -pg -ggdb -pg -O0 -D_DEBUG_VECTOR -DUNICODE -L../lib/ -L."
CFLAGS=" -Wall -pedantic -pg -O2 -DUNICODE"
LDFLAGS=""


LT_INIT([shared static])

AC_PROG_LIBTOOL
AM_PROG_LIBTOOL
AC_PROG_RANLIB



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
