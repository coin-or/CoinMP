# Copyright (C) 2006 International Business Machines and others.
# All Rights Reserved.
# This file is distributed under the Common Public License.

## $Id$

# Author:  Andreas Waechter           IBM    2006-04-13

AUTOMAKE_OPTIONS = foreign

########################################################################
#                          Subdirectories                              #
########################################################################

SUBDIRS = src

# We don't want to compile the test subdirectory, unless the test target is
# specified.  But we need to list it as subdirectory to make sure that it is
# included in the tarball

if ALWAYS_FALSE
  SUBDIRS += test
endif

########################################################################
#             Additional files to be included in tarball               #
########################################################################

# Here we need include all files that are not mentioned in other Makefiles

EXTRA_DIST = \
	examples/Makefile \
	examples/example.cpp

########################################################################
#                           Extra Targets                              #
########################################################################

test: all
	cd test; make test

unitTest: test

clean-local:
	if test -r test/Makefile; then cd test; make clean; fi

distclean-local:
	if test -r test/Makefile; then cd test; make distclean; fi

.PHONY: test unitTest

########################################################################
#                  Installation of the addlibs file                    #
#                  Installation of the examples dir                    #
########################################################################

addlibsfile = coinmp_addlibs.txt
addlibsdir = $(prefix)/share/doc/coin/$(PACKAGE_NAME)

examplefiles = $(srcdir)/examples/Makefile $(srcdir)/examples/example.cpp
examplesdir = $(prefix)/examples/$(PACKAGE_NAME)

install-exec-local:
	$(install_sh_DATA) $(addlibsfile) $(DESTDIR)$(addlibsdir)/$(addlibsfile)
	$(install_sh_DATA) $(examplefiles) $(DESTDIR)$(examplesdir)

uninstall-local:
	rm -f $(DESTDIR)$(addlibsdir)/$(addlibsfile)
	rm -f $(DESTDIR)$(examplesdir)

########################################################################
#                         Maintainer Stuff                             #
########################################################################

# Files that are generated and should be cleaned with make distclean
DISTCLEANFILES =

include ../BuildTools/Makemain.inc
