AC_INIT([libcollections], [0.1], [manvscode@gmail.com], [libcollections], [http://www.shrewdcreative.com/])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])

AM_SILENT_RULES([yes])
AC_PROG_CC
AC_PROG_CC_C99
AC_C_INLINE

AC_HEADER_STDC


CFLAGS=" -Wall -pedantic -g -ggdb -pg -O0 -D_DEBUG_VECTOR -DUNICODE"
dnlCFLAGS=" -Wall -pedantic -O3 -DUNICODE"
LDFLAGS=""

AC_ARG_ENABLE([shared],
	AS_HELP_STRING([--enable-shared], [Build shared library]))

LT_INIT
AC_PROG_LIBTOOL

#AS_IF([test "x$enable_shared" = "xyes"], [
#LT_INIT
#AC_PROG_LIBTOOL
#])

#AS_IF([test "x$enable_shared" = "xno"], [
#LT_INIT
#AC_PROG_RANLIB
#])

AM_CONDITIONAL(SHARED_LIB, test "x$enable_shared" = "xyes")
AM_CONDITIONAL(STATIC_LIB, test "x$enable_shared" = "xno")



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
