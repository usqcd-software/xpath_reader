
INCFLAGS=-I@top_srcdir@/include

AM_CXXFLAGS = $(INCFLAGS) @CXXFLAGS@ @LIBXML2_CXXFLAGS@
VPATH = @top_srcdir@/lib
VPATH += @top_srcdir@/include

TYPE_HDRS = tcomplex.h array.h
READER_HDRS = basic_xpath_reader.h document.h $(TYPE_HDRS)
WRITER_HDRS = xml_simplewriter.h xml_writer.h attribute.h $(TYPE_HDRS)

lib_LIBRARIES = libXPathReader.a libxmlWriter.a

libXPathReader_a_SOURCES = basic_xpath_reader.cc
libXPathReader_a_DEPENDENCIES = $(READER_HDRS)

libxmlWriter_a_SOURCES = xml_simplewriter.cc attribute.cc xml_struct_writer.cc
libxmlWriter_a_DEPENDENCIES = $(WRITER_HDRS)
