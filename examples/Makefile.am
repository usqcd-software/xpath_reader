
INCFLAGS=-I@top_srcdir@/include

AM_CXXFLAGS= $(INCFLAGS) @CXXFLAGS@ @LIBXML2_CXXFLAGS@


AM_LDFLAGS=-L@top_builddir@/lib

LDADD = -lXPathReader -lxmlWriter @LIBS@ @LIBXML2_LIBS@ 

check_PROGRAMS=basic_xpath_reader_test  basic_xpath_reader_test2 \
	xpath_reader_test simple_writer_test simple_schema_writer_test \
	schema_writer_test struct_writer_test

basic_xpath_reader_test_SOURCES=basic_xpath_reader_test.cc
basic_xpath_reader_test_DEPENDENCIES = build_lib

basic_xpath_reader_test2_SOURCES=basic_xpath_reader_test2.cc
basic_xpath_reader_test2_DEPENDENCIES = build_lib

xpath_reader_test_SOURCES=xpath_reader_test.cc
xpath_reader_test_DEPENDENCIES= build_lib

simple_writer_test_SOURCES=simple_writer_test.cc
simple_writer_test_DEPENDENCIES = build_lib

simple_schema_writer_test_SOURCES=simple_schema_writer_test.cc
simple_schema_writer_test_DEPENDENCIES=build_lib

schema_writer_test_SOURCES=schema_writer_test.cc
schema_writer_test_DEPENDENCIES=build_lib

struct_writer_test_SOURCES=struct_writer_test.cc
struct_writer_test_DEPENDENCIES=build_lib

.PHONY: build_lib
build_lib:
	cd @top_builddir@/lib ; $(MAKE)
