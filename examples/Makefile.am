
INCFLAGS=-I@top_srcdir@/include

AM_CXXFLAGS= $(INCFLAGS) @CXXFLAGS@ @QDPXX_CXXFLAGS@ @LIBXML2_CXXFLAGS@


AM_LDFLAGS=-L@top_builddir@/lib @QDPXX_LDFLAGS@

LDADD = -lXPathReader @LIBS@ @QDPXX_LIBS@ @LIBXML2_LIBS@ 

check_PROGRAMS=basic_xpath_reader_test  basic_xpath_reader_test2 \
	xpath_reader_test


basic_xpath_reader_test_SOURCES=basic_xpath_reader_test.cc
basic_xpath_reader_test_DEPENDENCIES = build_lib

basic_xpath_reader_test2_SOURCES=basic_xpath_reader_test2.cc
basic_xpath_reader_test2_DEPENDENCIES = build_lib

xpath_reader_test_SOURCES=xpath_reader_test.cc
xpath_reader_test_DEPENDENCIES= build_lib

#basic_xpath_writer_test_SOURCES=basic_xpath_writer_test.cc
#basic_xpath_writer_test_DEPENDENCIES = build_lib

.PHONY: build_lib
build_lib:
	cd @top_builddir@/lib ; $(MAKE)
