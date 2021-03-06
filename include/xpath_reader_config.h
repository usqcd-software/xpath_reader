#ifndef XPATH_READER_CONFIG_H
#define XPATH_READER_CONFIG_H

/* Undef the unwanted from the environment -- eg the compiler command line */
#undef PACKAGE
#undef PACKAGE_BUGREPORT
#undef PACKAGE_NAME
#undef PACKAGE_STRING
#undef PACKAGE_TARNAME
#undef PACKAGE_VERSION
#undef VERSION

/* Include the stuff generated by autoconf */
#include "xpath_reader_config_internal.h"

/* Prefix everything with XPATH_READER_ */
static const char* const XPATH_READER_PACKAGE(PACKAGE);
static const char* const XPATH_READER_PACKAGE_BUGREPORT(PACKAGE_BUGREPORT);
static const char* const XPATH_READER_PACKAGE_NAME(PACKAGE_NAME);
static const char* const XPATH_READER_PACKAGE_STRING(PACKAGE_STRING);
static const char* const XPATH_READER_PACKAGE_TARNAME(PACKAGE_TARNAME);
static const char* const XPATH_READER_PACKAGE_VERSION(PACKAGE_VERSION);
static const char* const XPATH_READER_VERSION(VERSION);


/* Undef the unwanted */
#undef PACKAGE
#undef PACKAGE_BUGREPORT
#undef PACKAGE_NAME
#undef PACKAGE_STRING
#undef PACKAGE_TARNAME
#undef PACKAGE_VERSION
#undef VERSION
#endif

