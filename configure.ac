dnl Process this file with autoconf to produce a configure script.
dnl Created by Anjuta application wizard.

AC_INIT(libnids, 0.9)

AM_INIT_AUTOMAKE(AC_PACKAGE_NAME, AC_PACKAGE_VERSION)
AM_CONFIG_HEADER(config.h)
AM_MAINTAINER_MODE

AC_ISC_POSIX
AC_PROG_CC
AM_PROG_CC_STDC
AC_HEADER_STDC






AM_PROG_LIBTOOL


AC_MSG_CHECKING([how to link PROJ.4 library])

AC_ARG_WITH(static_proj4,[  --with-static-proj4=ARG Compile with PROJ.4 statically (ARG=no or path)],,)

if test "x$with_static_proj4" = "xno" -o "x$with_static_proj4" = "x"; then

  PROJ_STATIC=no
  AC_MSG_RESULT([link dynamically.])

else

  PROJ_STATIC=yes
  AC_MSG_RESULT([link statically.])

  ORIG_LIBS="$LIBS"
  PROJ_LIB="-lproj"

  if test "x$with_static_proj4" = "xyes" ; then
    LIBS="$PROJ_LIB $ORIG_LIBS"
    AC_CHECK_LIB(proj,pj_init,PROJ_STATIC=yes,PROJ_STATIC=no,)
  else
  
    LIBS="-L$with_static_proj4/src $PROJ_LIB $ORIG_LIBS"
    AC_CHECK_LIB(proj,pj_init,PROJ_STATIC=yes,PROJ_STATIC=no,)

    if test "$PROJ_STATIC" = "no"; then
      dnl Disable previous value cached by the previous AC_CHECK_LIB test :
      dnl AC_CHECK_LIB caches the result value for a couple (function, library)
      dnl in a variable named "ac_cv_lib_<library name>_<function name>".
      dnl In order to try to locate the library in different locations, using
      dnl different values of LIBS, we have to 'unset' this variable after
      dnl each negative test.

      unset ac_cv_lib_proj_pj_init

      LIBS="-L$with_static_proj4/lib $PROJ_LIB $ORIG_LIBS"
      AC_CHECK_LIB(proj,pj_init,PROJ_STATIC=yes,PROJ_STATIC=no,)
    fi

    if test "$PROJ_STATIC" = "no"; then
      unset ac_cv_lib_proj_pj_init
      LIBS="-L$with_static_proj4/src/.libs $PROJ_LIB $ORIG_LIBS"
      AC_CHECK_LIB(proj,pj_init,PROJ_STATIC=yes,PROJ_STATIC=no,)
    fi

    if test "$PROJ_STATIC" = "no"; then
      unset ac_cv_lib_proj_pj_init
      LIBS="-L$with_static_proj4 $PROJ_LIB $ORIG_LIBS"
      AC_CHECK_LIB(proj,pj_init,PROJ_STATIC=yes,PROJ_STATIC=no,)
    fi

    if test "$PROJ_STATIC" = "no"; then
      unset ac_cv_lib_proj_pj_init
      LIBS="$PROJ_LIB $ORIG_LIBS"
      AC_CHECK_LIB(proj,pj_init,PROJ_STATIC=yes,PROJ_STATIC=no,)
    fi

  fi

  if test "$PROJ_STATIC" = "no"; then
    LIBS="$ORIG_LIBS"
  fi

  if test -r "$with_static_proj4/src/proj_api.h" ; then
    PROJ_INCLUDE="-I$with_static_proj4/src"
  elif test -r "$with_static_proj4/include/proj_api.h" ; then
    PROJ_INCLUDE="-I$with_static_proj4/include"
  elif test -r "$with_static_proj4/proj_api.h" ; then
    PROJ_INCLUDE="-I$with_static_proj4"
  fi
  
fi

AC_SUBST(PROJ_STATIC)
AC_SUBST(PROJ_INCLUDE)




AC_OUTPUT([
Makefile
apps/Makefile
etc/Makefile
include/Makefile
kml/Makefile
scripts/Makefile
share/Makefile
src/Makefile
])
