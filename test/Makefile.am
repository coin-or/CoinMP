# Copyright (C) 2006 International Business Machines and others.
# All Rights Reserved.
# This file is distributed under the Eclipse Public License.

# Author:  Andreas Waechter           IBM    2006-04-13

########################################################################
#                             unitTest                                 #
########################################################################

noinst_PROGRAMS = unitTest

unitTest_SOURCES = unitTest.cpp

unitTest_LDADD = ../src/libCoinMP.la $(COINMPLIB_LFLAGS)

AM_CPPFLAGS = -I$(srcdir)/../src -I../src $(COINMPLIB_CFLAGS)

test: unitTest$(EXEEXT)
	./unitTest$(EXEEXT)

.PHONY: test
