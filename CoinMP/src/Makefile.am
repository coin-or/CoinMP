# Copyright (C) 2006 International Business Machines and others.
# All Rights Reserved.
# This file is distributed under the Common Public License.

# Author:  Andreas Waechter           IBM    2006-04-13

########################################################################
#                               libCoinMP                              #
########################################################################

# Name of the library compiled in this directory.
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
libCoinMP_la_LIBADD = $(COINMPLIB_LFLAGS)

# This is for libtool
libCoinMP_la_LDFLAGS = $(LT_LDFLAGS)

# Here list all include flags.
AM_CPPFLAGS = $(COINMPLIB_CFLAGS)

########################################################################
#                Headers that need to be installed                     #
########################################################################

# Here list all the header files that are required by a user of the library,
# and that therefore should be installed in 'include/coin-or'
includecoindir = $(includedir)/coin-or
includecoin_HEADERS = CoinMP.h

install-exec-local:
	$(install_sh_DATA) config_coinmp.h $(DESTDIR)$(includecoindir)/CoinMPConfig.h

uninstall-local:
	rm -f $(DESTDIR)$(includecoindir)/CoinMPConfig.h
