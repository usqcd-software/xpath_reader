AC_INIT(xpath_reader, 1.0, bj@ph.ed.ac.uk)
AC_CONFIG_AUX_DIR(config)
AC_PROG_CXX(g++)
AC_PROG_RANLIB()
AM_INIT_AUTOMAKE

dnl ---with-qdp=DIR
#AC_ARG_WITH(qdp,
#  AC_HELP_STRING(
#    [--with-qdp=DIR],
#    [Build QDP++ Apps on top of QDP++ where QDP++ is installed in DIR]
#  ),
#  [QDPXX_HOME="$with_qdp"]
#)
#
#if test "X${QDPXX_HOME}X" = "XX" ; then
#  AC_PATH_PROG(QDPXX_CONFIG, [qdp++-config], [])
#else
#  AC_PATH_PROG(QDPXX_CONFIG, [qdp++-config], [], [${QDPXX_HOME}/bin:${PATH}])
#fi
#
#if test "X${QDPXX_CONFIG}X" = "XX" ; then
#  AC_MSG_ERROR([QDP++ configuration program qdp++-config not found.])
#fi
#
#AC_MSG_NOTICE([Found QDP++ configuration program ${QDPXX_CONFIG}])
#AC_SUBST(QDPXX_CXXFLAGS, "`${QDPXX_CONFIG} --cxxflags`")
#AC_MSG_NOTICE([QDP++ compile flags: ${QDPXX_CXXFLAGS}])
#AC_SUBST(QDPXX_LDFLAGS,  "`${QDPXX_CONFIG} --ldflags`")
#AC_MSG_NOTICE([QDP++ linking flags: ${QDPXX_LDFLAGS}])
#AC_SUBST(QDPXX_LIBS,     "`${QDPXX_CONFIG} --libs`")
#AC_MSG_NOTICE([QDP++ libraries flags: ${QDPXX_LIBS}])
#AC_SUBST(QDPXX_ND,       "`${QDPXX_CONFIG} --Nd`")
#if test "X${QDPXX_ND}X" = "XX" ; then
#  AC_MSG_ERROR([Cannot determine QDP++ spacetime dimension])
#else
#  AC_MSG_NOTICE([QDP++ spacetime dimension: ${QDPXX_ND}])
#fi
#AC_SUBST(QDPXX_NC,       "`${QDPXX_CONFIG} --Nc`")
#AC_MSG_NOTICE([QDP++ number of colors: ${QDPXX_NC}])
#AC_SUBST(QDPXX_NS,       "`${QDPXX_CONFIG} --Ns`")
#if test "X${QDPXX_NS}X" = "XX" ; then
#  AC_MSG_ERROR([Cannot determine number of spin components in QDP++])
#else
#  AC_MSG_NOTICE([QDP++ number of spin components: ${QDPXX_NS}])
#fi
#
#dnl Try to compile a QDP++ program to check the --with options
#AC_MSG_CHECKING([if we can compile/link a simple QDP++ program])
#
#PAC_QDPXX_LINK_CXX_FUNC(
#  ${QDPXX_CXXFLAGS},
#  ${QDPXX_LDFLAGS},
#  ${QDPXX_LIBS},
#  ,
#  ,
#  [qdpxx_link_ok=yes],
#  [qdpxx_link_ok=no]
#)
#
#if test "X${qdpxx_link_ok}X" = "XyesX" ; then 
#  AC_MSG_RESULT(yes)
#else 
#  AC_MSG_RESULT(no)
#  AC_MSG_ERROR([Cannot compile/link a program with QDP++.
#    Use --with-qdp=<dir> to select a working version.])
#fi 

# these are dummies for now
AC_SUBST(QDPXX_CXXFLAGS)
AC_SUBST(QDPXX_LDFLAGS)
AC_SUBST(QDPXX_LIBS)


dnl now libxml2
AC_ARG_WITH(libxml2,
  AC_HELP_STRING(
    [--with-libxml2=DIR],
    [Build on top of libxml2 where libxml2 is installed in DIR]
  ),
  [LIBXML2_HOME="$with_libxml2"]
)

echo ${LIBXML2_HOME}

if test "X${LIBXML2_HOME}X" = "XX" ; then
  AC_PATH_PROG(LIBXML2_CONFIG, [xml2-config], [])
else
  AC_PATH_PROG(LIBXML2_CONFIG, [xml2-config], [], [${LIBXML2_HOME}/bin:${PATH}])
fi

if test "X${LIBXML2_CONFIG}X" = "XX" ; then
  AC_MSG_ERROR([libxml2 configuration program xml2-config not found.])
fi

AC_MSG_NOTICE([Found libxml2 configuration program ${LIBXML2XX_CONFIG}])
AC_SUBST(LIBXML2_CXXFLAGS, "`${LIBXML2_CONFIG} --cflags`")
AC_MSG_NOTICE([libxml2 compile flags: ${LIBXML2_CXXFLAGS}])
AC_SUBST(LIBXML2_LIBS,     "`${LIBXML2_CONFIG} --libs`")
AC_MSG_NOTICE([libxml2 libraries flags: ${LIBXML2_LIBS}])

dnl This function is defined in acinclude.m4
dnl it tries to try to compile the program 
AC_MSG_CHECKING([if we can compile/link a simple libxml2 program])
PAC_LIBXML2_LINK_CXX_FUNC(
  ${LIBXML2_CXXFLAGS},
  ${LIBXML2_LIBS},
  ,
  ,
  [libxml2_link_ok=yes],
  [libxml2_link_ok=no]
)

if test "X${libxml2_link_ok}X" = "XyesX" ; then 
  AC_MSG_RESULT(yes)
else 
  AC_MSG_RESULT(no)
  AC_MSG_ERROR([Cannot compile/link a program with libxml2.
    Use --with-libxml2=<dir> to select a working version.])
fi 

AC_CONFIG_FILES(Makefile)
AC_CONFIG_FILES(lib/Makefile)
AC_CONFIG_FILES(include/Makefile)
AC_CONFIG_FILES(examples/Makefile)
AC_OUTPUT
