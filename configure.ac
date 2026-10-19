# Copyright (C) 2006 International Business Machines.
# All Rights Reserved.
# This file is distributed under the Eclipse Public License.

#############################################################################
#                       Names and other basic things                        #
#############################################################################

AC_INIT([CoinMP],[devel],[https://github.com/coin-or/CoinMP/issues/new],[coin-or-coinmp],[https://github.com/coin-or/CoinMP])

AC_COPYRIGHT([
Copyright 2006 International Business Machines and others.
All Rights Reserved.
This file is part of the open source package CoinMP which is distributed
under the Eclipse Public License.])

# List one file in the package so that the configure script can test
# whether the package is actually there
AC_CONFIG_SRCDIR(src/CoinMP.h)

# Do some initialization work (version numbers, change prefix default, ...)
AC_COIN_INITIALIZE

#############################################################################
#                         Standard build tool stuff                         #
#############################################################################

# Get the name of the C and C++ compilers and appropriate compiler options.
# CoinMP has C and C++ sources, and the example is in C.
AC_COIN_PROG_CC
AC_COIN_PROG_CXX

# This is mainly a C++ package, set the language accordingly.
AC_LANG_PUSH(C++)

# Initialize libtool
AC_COIN_PROG_LIBTOOL

# set RPATH_FLAGS to the compiler link flags required to hardcode location
# of the shared objects (expanded_libdir is set somewhere in configure before)
# (used in the example)
AC_COIN_RPATH_FLAGS([$expanded_libdir])

#############################################################################
#                              COIN-OR components                           #
#############################################################################

# CoinMP uses Cbc, Cgl, Clp, Osi, and CoinUtils directly (one .pc file per
# check, as some pkg-config versions do not accept several modules in one
# argument).
m4_foreach_w([myvar],[CoinUtils Osi Clp OsiClp Cgl Cbc],
  [ AC_COIN_CHK_PKG(myvar,[CoinMPLib],m4_case(myvar,[OsiClp],[osi-clp]))
    if test $coin_has_[]m4_tolower(myvar) != yes ; then
      AC_MSG_ERROR([Required package myvar is not available.])
    fi
  ])

##############################################################################
#                   Finishing up by writing all the output                   #
##############################################################################

AC_COIN_FINALIZE_FLAGS([CoinMPLib])

# Here list all the files that configure should create (except for the
# configuration header file)
AC_CONFIG_FILES([Makefile
                 src/Makefile
                 test/Makefile
                 examples/Makefile
                 coinmp.pc])

# Here put the location and name of the configuration header file
AC_CONFIG_HEADERS([src/config.h src/config_coinmp.h])

# Finally, we let configure write all the output...
AC_COIN_FINALIZE
