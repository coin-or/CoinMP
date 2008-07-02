CoinDir  := ../..
#CoinMPDir := ..


CXX      := g++
CXXFLAGS := -g

# Wherever the headers are installed
IncDir	:= $(CoinDir)/include/coin

# Wherever the libs are installed
LIBDIRS := $(CoinDir)/lib

LIBS	:= -lCoinMP
LIBS 	+= -lCbc
LIBS 	+= -lCgl
LIBS 	+= -lOsi
LIBS 	+= -lOsiClp
LIBS 	+= -lClp
LIBS 	+= -lCoinUtils
LIBS	+= `cat $(CoinDir)/share/doc/coin/CoinMP/coinmp_addlibs.txt`
LIBS	+= `cat $(CoinDir)/share/doc/coin/Cbc/cbc_addlibs.txt`
LIBS	+= `cat $(CoinDir)/share/doc/coin/Cgl/cgl_addlibs.txt`
LIBS	+= `cat $(CoinDir)/share/doc/coin/Osi/osi_addlibs.txt`
LIBS	+= `cat $(CoinDir)/share/doc/coin/Clp/clp_addlibs.txt`
LIBS	+= `cat $(CoinDir)/share/doc/coin/CoinUtils/coinutils_addlibs.txt`


###############################################################################

SHLINKPREFIX = -Wl,-rpath,
CXXFLAGS += $(addprefix -I,${IncDir})

LDFLAGS := $(addprefix -L,$(LIBDIRS))
LDFLAGS += $(addprefix $(SHLINKPREFIX),$(LIBDIRS))
LDFLAGS	+= $(LIBS)

###############################################################################

.DELETE_ON_ERROR:

.PHONY: default

default: all

###############################################################################

all :	example

example : example.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o example example.cpp

clean:	
	@rm example

###############################################################################
