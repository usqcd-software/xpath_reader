cmake_minimum_required(VERSION 3.17)
PROJECT(XPathReaderExamples VERSION 1.0.0
	LANGUAGES CXX)

get_directory_property(hasParent PARENT_DIRECTORY)
if(NOT hasParent)
	find_package(XPathReader REQUIRED)
endif()

macro(add_example EXECNAME)
    # Make the executable
    add_executable(${EXECNAME} ${EXECNAME}.cc)

    # Add on the libs, transitively
    # add libxml2 libx
    target_link_libraries(${EXECNAME} XPathReader::XPathReader XPathReader::xmlWriter)
	install( TARGETS ${EXECNAME} DESTINATION bin )
endmacro()

add_example(print_xpath)
add_example(print_nodeset)
add_example(replace_xpath)
add_example(basic_xpath_reader_test)
add_example(basic_xpath_reader_test2)
add_example(basic_xpath_reader_test3)
add_example(xpath_reader_test)
add_example(xpath_reader_bool_test)
add_example(simple_writer_test)
add_example(simple_schema_writer_test)
add_example(schema_writer_test)
add_example(struct_writer_test)

install(FILES foo.xml schema_test-v0.5.xml
	    DESTINATION share)



