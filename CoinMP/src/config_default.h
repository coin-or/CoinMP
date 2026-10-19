/* this needs to come before the include of config_coinmp_default.h */
#ifndef COINMPLIB_EXPORT
#if defined(_WIN32) && defined(DLL_EXPORT)
#define COINMPLIB_EXPORT __declspec(dllexport)
#else
#define COINMPLIB_EXPORT
#endif
#endif

/* include the public project specific macros */
#include "config_coinmp_default.h"
