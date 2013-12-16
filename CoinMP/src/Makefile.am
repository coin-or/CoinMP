# Copyright (C) 2006 International Business Machines and others.
# All Rights Reserved.
# This file is distributed under the Common Public License.

## $Id$

# Author:  Andreas Waechter           IBM    2006-04-13

AUTOMAKE_OPTIONS = foreign

########################################################################
#                               libCoinMP                              #
########################################################################

# Name of the library compiled in this directory.  We want it to be installed
# in the 'lib' directory
lib_LTLIBRARIES = libCoinMP.la

# List all source files for this library, including headers
libCoinMP_la_SOURCES = \
	CoinMP.cpp CoinMP.h \
	CoinProblem.c CoinProblem.h \
	CoinResult.c CoinResult.h \
	CoinSolver.c CoinSolver.h \
	CoinOption.c CoinOption.h \
	CoinCbc.cpp CoinCbc.h \
	CoinMPConfig.h

# List all additionally required libraries
if DEPENDENCY_LINKING
libCoinMP_la_LIBADD = $(COINMP_LIBS)
endif

# This is for libtool (on Windows)
libCoinMP_la_LDFLAGS = $(LT_LDFLAGS)

########################################################################
#                            Additional flags                          #
########################################################################

# Here list all include flags, relative to this "srcdir" directory.  This
# "cygpath" stuff is necessary to compile with native compilers on Windows.
# "top_srcdir" refers to the basic directory for the main package that is
# being compiled.
AM_CPPFLAGS = $(COINMP_CFLAGS)

# This line is necessary to allow VPATH compilation
DEFAULT_INCLUDES = -I. -I`$(CYGPATH_W) $(srcdir)` -I.

########################################################################
#                Headers that need to be installed                     #
########################################################################

# Here list all the header files that are required by a user of the library,
# and that therefore should be installed in 'install'
includecoindir = $(includedir)/coin
includecoin_HEADERS = CoinMP.h 

#######################################################################
# Create the Config.h file that has all public defines and install it #
#######################################################################

install-exec-local:
	$(install_sh_DATA) config_coinmp.h $(DESTDIR)$(includecoindir)/CoinMPConfig.h

uninstall-local:
	rm -f $(DESTDIR)$(includecoindir)/CoinMPConfig.h

