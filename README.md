# CoinMP 1.8

[![A COIN-OR Project](https://coin-or.github.io/coin-or-badge.png)](https://www.coin-or.org)

Projects such as this one are maintained by a small group of volunteers under
the auspices of the non-profit [COIN-OR Foundation](https://www.coin-or.org)
and we need your help! Please consider [sponsoring our
activities](https://github.com/sponsors/coin-or) or [volunteering](mailto:volunteer@coin-or.org) to help!

[![Latest Release](https://img.shields.io/github/v/release/coin-or/CoinMP?sort=semver)](https://github.com/coin-or/CoinMP/releases)

_This file is auto-generated from [config.yml](.coin-or/config.yml) using the 
[generate_readme](.coin-or/generate_readme) script.
To make changes, please edit [config.yml](.coin-or/config.yml) or the generation scripts
[here](.coin-or/generate_readme) and [here](https://github.com/coin-or/coinbrew/blob/master/scripts/generate_readme)._

CoinMP is a C-API interface library that supports most of the
functionality of the CLP (Coin LP), CBC (Coin Branch-and-Cut), and CGL (Cut
Generation Library) projects. It is built as a shared library (`libCoinMP`,
or `CoinMP.dll` on Windows) that can be used from other C/C++ projects and
applications. The header file `CoinMP/src/CoinMP.h` contains the prototypes of all
functions of the library.

CoinMP is written in C++ and is released as open source under the [Eclipse Public License 2.0](http://www.opensource.org/licenses/EPL-2.0).

It is distributed under the auspices of the [COIN-OR Foundation](https://www.coin-or.org).

The CoinMP development site is https://github.com/coin-or/CoinMP.

## CURRENT BUILD STATUS

[![Windows Builds](https://github.com/coin-or/CoinMP/actions/workflows/windows-ci.yml/badge.svg?branch=stable/1.8)](https://github.com/coin-or/CoinMP/actions/workflows/windows-ci.yml?query=branch%3Astable/1.8)

[![Linux and MacOS Builds](https://github.com/coin-or/CoinMP/actions/workflows/linux-ci.yml/badge.svg?branch=stable/1.8)](https://github.com/coin-or/CoinMP/actions/workflows/linux-ci.yml?query=branch%3Astable/1.8)

## DOWNLOAD

What follows is a quick start guide for obtaining or building
CoinMP on common platforms. More detailed information is
available [here](https://coin-or.github.io/user_introduction.html).

### Docker image

There is a Docker image that provides CoinMP, as well as other projects
in the [COIN-OR Optimization
Suite](https://github.com/coin-or/COIN-OR-OptimizationSuite) [here](https://hub.docker.com/repository/docker/coinor/coin-or-optimization-suite)

### Binaries

For newer releases, binaries will be made available as assets attached to
releases in Github
[here](https://github.com/coin-or/CoinMP/releases). Older binaries
are archived as part of CoinMP
[here](https://www.coin-or.org/download/binary/CoinMP).

 * *Linux* (see https://repology.org/project/coinmp/versions for a complete listing): 
   * arch:
     ```
     $ sudo pacman -S  coin-or-mp
     ```
   * Debian/Ubuntu:
     ```
     $ sudo apt-get install  coinor-libcoinmp-dev
     ```
   * freebsd:
     ```
     $ sudo pkg install math/coinmp
     ```
 * *Windows*: The easiest way to get CoinMP on Windows is to download an archive as described above.

* *conda* (cross-platform):
     ```
     $ conda install -c conda-forge coinmp
     ```

Due to license incompatibilities, pre-compiled binaries lack some 
functionality. If binaries are not available for your platform for the latest 
version and you would like to request them to be built and posted, feel free 
to open a discussion on Github. 

### Source

Source code can be obtained either by

 * Downloading a snapshot of the source code for the latest release version of CoinMP from the
 [releases](https://github.com/coin-or/CoinMP/releases) page,
 * Cloning this repository from [Github](https://github.com/coin-or/CoinMP), or 
 * Using the [coinbrew](https://github.com/coin-or/coinbrew) script to get the project and all dependencies (recommended, see below).   

### Dependencies

CoinMP has a number of dependencies, which are detailed in
[config.yml](.coin-or/config.yml). Dependencies on other COIN-OR projects are
automatically downloaded when obtaining the source with `coinbrew`. For some
of the remaining third-party dependencies, automatic download scripts and
build wrappers are provided (and will also be automatically run for required
and recommended dependencies), while other libraries that are aeasy to obtain
must be installed using an appropriate package manager (or may come with your
OS by default). 

## BUILDING from source

These quick start instructions assume you are in a bash shell. 

### Using `coinbrew`

To download and build CoinMP from source, execute the 
following on the command line. 
```
wget https://raw.githubusercontent.com/coin-or/coinbrew/master/coinbrew
chmod u+x coinbrew
./coinbrew fetch CoinMP@1.8
./coinbrew build CoinMP
```
For more detailed instructions on coinbrew, see https://coin-or.github.io/coinbrew.
The `coinbrew` script will fetch the additional projects specified in the Dependencies section of [config.yml](.coin-or/config.yml).

### Without `coinbrew` (Expert users)

 * Download the source code, e.g., by cloning the git repo https://github.com/coin-or/CoinMP
 * Download and install the source code for the dependencies listed in [config.yml](.coin-or/config.yml)
 * Build the code as follows (make sure to set PKG_CONFIG_PTH to install directory for dependencies).

```
./configure -C
make
make test
make install
```

### Example

The `CoinMP/examples` directory has a C program that solves a few small problems
with CoinMP. After building and installing CoinMP, it can be built by running
`make` in the corresponding subdirectory of the build directory, e.g.,
```
cd build/CoinMP/1.8/examples
make
./example
```


## Project Links

 * [Code of Conduct](https://www.coin-or.org/code-of-conduct/)
 * [COIN-OR Web Site](http://www.coin-or.org/)
 * [COIN-OR general discussion forum](https://github.com/orgs/coin-or/discussions)
 * [CoinMP Discussion forum](https://github.com/coin-or/CoinMP/discussions)
 * [Report a bug](https://github.com/coin-or/CoinMP/issues/new)
 * [Doxygen generated documentation](http://coin-or.github.io/CoinMP/Doxygen)

