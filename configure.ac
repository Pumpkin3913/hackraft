AC_INIT([hackcraft], [1.0])
AM_INIT_AUTOMAKE

AC_CONFIG_MACRO_DIR([m4]) 
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([ Makefile ])

AC_ARG_ENABLE(debug, [  --enable-debug Enable debugging], [CXXFLAGS='-O0 -g -DDEBUG=1'], [])

CXX=clang
AC_PROG_CXX
AX_CXX_COMPILE_STDCXX_14

AX_PROG_LUA([5.3], )
AX_LUA_HEADERS(, [AC_MSG_ERROR([Cannot find LUA headers])])
AX_LUA_LIBS(, [AC_MSG_ERROR([Cannot find LUA libraries])])

AC_OUTPUT
