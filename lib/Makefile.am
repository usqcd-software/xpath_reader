
INCFLAGS=-I@top_srcdir@/include

AM_CXXFLAGS = $(INCFLAGS) @CXXFLAGS@ @QDPXX_CXXFLAGS@ @LIBXML2_CXXFLAGS@
VPATH = @top_srcdir@/lib
VPATH += @top_srcdir@/include

HDRS = basic_xpath_reader.h tcomplex.h array.h attribute.h

lib_LIBRARIES = libXPathReader.a

libXPathReader_a_SOURCES = basic_xpath_reader.cc attribute.cc
libXPathReader_a_DEPENDENCIES = $(HDRS)
