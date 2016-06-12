################################################################################
## qDecoder - http://www.qdecoder.org
##
## Copyright (c) 2000-2012 Seungyoung Kim.
## All rights reserved.
##
## Redistribution and use in source and binary forms, with or without
## modification, are permitted provided that the following conditions are met:
##
## 1. Redistributions of source code must retain the above copyright notice,
##    this list of conditions and the following disclaimer.
## 2. Redistributions in binary form must reproduce the above copyright notice,
##    this list of conditions and the following disclaimer in the documentation
##    and/or other materials provided with the distribution.
##
## THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
## AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
## IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
## ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
## LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
## CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
## SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
## INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
## CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
## ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
## POSSIBILITY OF SUCH DAMAGE.
################################################################################
## $Id: configure.ac 636 2012-05-07 23:40:43Z seungyoung.kim $
################################################################################

AC_COPYRIGHT([
==============================================================================
 qDecoder - http://www.qdecoder.org

 Copyright (c) 2000-2012 Seungyoung Kim.
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 1. Redistributions of source code must retain the above copyright notice,
    this list of conditions and the following disclaimer.
 2. Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
==============================================================================
])

## Internal functions
AC_DEFUN([Q_ARG_ENABLE], [
	AC_ARG_ENABLE([$1],[AS_HELP_STRING([--enable-$1], [$2])],,[enableval=no])
	if test "$enableval" = yes; then
		AC_MSG_NOTICE(['$1' feature is enabled])
		CPPFLAGS="$CPPFLAGS $3"
	fi
])

AC_DEFUN([Q_ARG_DISABLE], [
	AC_ARG_ENABLE([$1],[AS_HELP_STRING([--disable-$1], [$2])],,[enableval=yes])
	if test "$enableval" = no; then
		AC_MSG_NOTICE(['$1' feature is disabled])
		CPPFLAGS="$CPPFLAGS $3"
	fi
])

## Initialize
AC_INIT([qDecoder], [12 RELEASE], [http://www.qdecoder.org/])
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADER([config.h])
AC_CONFIG_FILES([Makefile src/qdecoder.pc src/Makefile examples/Makefile])

## Set path
PATH="$PATH:/bin:/sbin:/usr/bin:/usr/sbin:/usr/local/bin:/usr/local/sbin"
CPPFLAGS="$CPPFLAGS -I/usr/include -I/usr/local/include -I./ -D_GNU_SOURCE"
LDFLAGS="$LDFLAGS -L/usr/lib -L/usr/local/lib"

## Set autoconf setting
#AC_CANONICAL_TARGET
AC_PREFIX_DEFAULT([/usr/local])
#AC_PRESERVE_HELP_ORDER

## Checks for programs.
AC_PROG_CC
if test $ac_compiler_gnu = yes; then
	CFLAGS="-Wall -fPIC"
#else
#	AC_MSG_FAILURE([GCC is required.])
fi
AC_PROG_CC_C99
if test $ac_cv_prog_cc_c99 = no; then
	AC_MSG_FAILURE([Compiler does not support C99 mode.])
fi

AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_RANLIB

AC_PATH_PROG(AR, ar)
AC_PATH_PROG(CHMOD, chmod)
AC_PATH_PROG(LD, ld)
AC_PATH_PROG(RM, rm)
AC_PATH_PROG(MKDIR, mkdir)
AC_PATH_PROG(RMDIR, rmdir)

## Checks for header files.
AC_HEADER_STDC
AC_HEADER_STDBOOL
AC_HEADER_DIRENT
#AC_CHECK_HEADER([sys/sendfile.h])

## Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T
AC_TYPE_OFF_T

## Checks for libraries.

## Checks for library functions.
#AC_CHECK_FUNCS([socket sendfile])

AC_ARG_ENABLE([fastcgi],[AS_HELP_STRING([--enable-fastcgi=/FASTCGI_INCLUDE_DIR_PATH/], [enable FastCGI supports])],[enableval=yes],[enableval=no])
if test "$enableval" = yes; then
	AC_CHECK_FILE([$enable_fastcgi/fcgi_stdio.h],[enableval=yes],[enableval=no])
	if test "$enableval" = yes; then
		AC_MSG_NOTICE(['fastcgi' feature is enabled])
		CPPFLAGS="$CPPFLAGS -DENABLE_FASTCGI -I$enable_fastcgi"
		LIBS="$LIBS -lfcgi"
	else
		AC_MSG_FAILURE([can't find 'fcgi_stdio.h' header file in '$enable_fastcgi' directory. can't enable fastcgi support.])
	fi
fi

Q_ARG_ENABLE([debug], [enable debugging output (development mode)], [-DBUILD_DEBUG])
if test "$enableval" = yes; then
	CFLAGS="$CFLAGS -g"
else
	CFLAGS="$CFLAGS -g"
fi

AC_MSG_NOTICE([CFLAGS $CFLAGS])
AC_MSG_NOTICE([CPPFLAGS $CPPFLAGS])
AC_MSG_NOTICE([LIBS $LIBS])

## Create Makefile
AC_OUTPUT
