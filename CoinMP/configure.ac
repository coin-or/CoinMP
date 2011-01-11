# Copyright (C) 2006 International Business Machines.
# All Rights Reserved.
# This file is distributed under the Common Public License.

## $Id$

# Author:  Andreas Waechter            IBM    2006-04-13

#############################################################################
#                       Names and other basic things                        #
#############################################################################

AC_PREREQ(2.59)

AC_INIT([CoinMP],[1.6stable],[coin-discuss@list.coin-or.org])

AC_COPYRIGHT([
Copyright 2006 International Business Machines and others.
All Rights Reserved.
This file is part of the open source package Coin which is distributed
under the Common Public License.])

# List one file in the package so that the configure script can test
# whether the package is actually there
AC_CONFIG_SRCDIR(src/CoinMP.h)

# Where should everything be installed by default?  Here, we want it
# to be installed directly in 'bin', 'lib', 'include' subdirectories
# of the directory where configure is run.  The default would be
# /usr/local.
AC_PREFIX_DEFAULT([`pwd`])

#############################################################################
#                         Standard build tool stuff                         #
#############################################################################

# Get the system type
AC_CANONICAL_BUILD

# If this project depends on external projects, the Externals file in
# the source root directory contains definition of where to find those
# externals.  The following macro ensures that those externals are
# retrieved by svn if they are not there yet.
AC_COIN_PROJECTDIR_INIT

# Check if user wants to produce debugging code
AC_COIN_DEBUG_COMPILE

# Get the name of the C++ compiler and appropriate compiler options
AC_COIN_PROG_CXX

# Initialize automake and libtool
AC_COIN_INIT_AUTO_TOOLS

#############################################################################
#                              COIN components                              #
#############################################################################

#AC_COIN_CHECK_PACKAGE(CoinDepend, [cgl osi coinutils], [CbcLib CbcGeneric])
#if test $coin_has_coindepend != yes ; then
#  AC_MSG_ERROR([One or more required packages CoinUtils, Osi and Cgl are not available.])
#fi

AC_COIN_CHECK_PACKAGE(CoinUtils, [coinutils], [CbcLib CbcGeneric])
if test $coin_has_coinutils != yes ; then
  AC_MSG_ERROR("required package CoinUtils Unavailable.")
fi

AC_COIN_CHECK_PACKAGE(Cgl, [cgl], [CbcLib CbcGeneric])
if test $coin_has_cgl != yes ; then
  AC_MSG_ERROR("required package Cgl Unavailable.")
fi

AC_COIN_CHECK_PACKAGE(Osi, [osi], [CbcLib CbcGeneric])
if test $coin_has_osi != yes ; then
  AC_MSG_ERROR("required package Osi Unavailable.")
fi

AC_COIN_CHECK_PACKAGE(Clp, [osi-clp], [CbcLib CbcGeneric])
if test $coin_has_clp != yes ; then
  AC_MSG_ERROR("required package Clp Unavailable.")
fi

AC_COIN_CHECK_PACKAGE(Cbc, [cbc], [CoinMP])
if test $coin_has_cbc != yes; then
  AC_MSG_ERROR("Required package Cbc unavailable.")
fi 

AC_COIN_CHECK_PACKAGE(Dylp, [osi-dylp], [CoinMP])
AC_COIN_CHECK_PACKAGE(SYMPHONY, [osi-sym], [CoinMP])
AC_COIN_CHECK_PACKAGE(Vol, [osi-vol], [CoinMP])

#AC_COIN_HAS_PROJECT(CoinMP)

# Decide which one is the default solver
#AC_MSG_CHECKING(for CBC default solver)
#AC_ARG_WITH([cbc-default-solver],
#            AC_HELP_STRING([--with-cbc-default-solver],
#                           [specify default solver for Cbc in lower case letters (if not given, clp is assumed)]),
#            [cbc_default_solver=$with_val],
#            [cbc_default_solver=clp])
#AC_MSG_RESULT($cbc_default_solver)

#case $cbc_default_solver in
#  clp)
#    AC_DEFINE([CBC_DEFAULT_SOLVER],"clp",
#              [Define to the name of the default solver to be used in CBC in small letters])
#    ;;
#  dylp)
#    AC_DEFINE([CBC_DEFAULT_SOLVER],"dylp")
#    ;;
#  *)
#    AC_MSG_ERROR([Unknown solver $cbc_default_solver chosen as CBC's default solver])
#    ;;
#esac



#############################################################################
#                            Third party solvers                            #
#############################################################################

# Check which third party solvers are available ToDo!
AC_COIN_CHECK_PACKAGE(Cpx, [osi-cpx], [CoinMP])
AC_COIN_CHECK_PACKAGE(Fmp, [osi-fmp], [CoinMP])
AC_COIN_CHECK_PACKAGE(Glpk, [osi-glpk], [CoinMP])
AC_COIN_CHECK_PACKAGE(Msk, [osi-msk], [CoinMP])
AC_COIN_CHECK_PACKAGE(Spx, [osi-spx], [CoinMP])
AC_COIN_CHECK_PACKAGE(Xpr, [osi-xpr], [CoinMP])

#############################################################################
#                       Other third party software                          #
#############################################################################

# Ampl Solver library
AC_COIN_CHECK_PACKAGE(ASL, [coinasl], [CoinMP])

##############################################################################
#                   Finishing up by writing all the output                   #
##############################################################################

# Here list all the files that configure should create (except for the
# configuration header file)
AC_CONFIG_FILES([Makefile
                 src/Makefile
                 test/Makefile
		 coinmp_addlibs.txt])

# Here put the location and name of the configuration header file
AC_CONFIG_HEADER([inc/config_coinmp.h])

# Finally, we let configure write all the output...
AC_COIN_FINALIZE
