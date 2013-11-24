# Copyright (C) 2006 International Business Machines.
# All Rights Reserved.
# This file is distributed under the Common Public License.

## $Id$

# Author:  Andreas Waechter            IBM    2006-04-13

#############################################################################
#                       Names and other basic things                        #
#############################################################################

AC_PREREQ(2.59)

AC_INIT([CoinMP],[1.7.4],[coinmp@list.coin-or.org])

AC_COPYRIGHT([
Copyright 2006 International Business Machines and others.
All Rights Reserved.
This file is part of the open source package CoinMP which is distributed
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
AC_COIN_PROJECTDIR_INIT(CoinMP,8:4:7)

# Check if user wants to produce debugging code
AC_COIN_DEBUG_COMPILE(CoinMP)

# Get the name of the C++ compiler and appropriate compiler options
AC_COIN_PROG_CXX

# Initialize automake and libtool
AC_COIN_INIT_AUTO_TOOLS

#############################################################################
#                            COIN-OR components                             #
#############################################################################

# Required packages.
# We need Clp, Cbc, and the OSI interface to CLP to build CoinCbc.cpp.

AC_COIN_CHECK_PACKAGE(Cbc, [cbc clp osi-clp], [CoinMP])
if test $coin_has_cbc != yes; then
  AC_MSG_ERROR("Required package Cbc unavailable.")
fi 

##############################################################################
#                   Finishing up by writing all the output                   #
##############################################################################

# Here list all the files that configure should create (except for the
# configuration header file)
AC_CONFIG_FILES([Makefile
                 src/Makefile
                 test/Makefile
                 examples/Makefile
                 coinmp.pc
                 coinmp-uninstalled.pc])

# Here put the location and name of the configuration header file
AC_CONFIG_HEADER([src/config.h src/config_coinmp.h])

# Finally, we let configure write all the output...
AC_COIN_FINALIZE
