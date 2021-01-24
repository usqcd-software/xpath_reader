add_library(XPathReader basic_xpath_reader.cc)
target_include_directories( XPathReader PUBLIC
						   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
						   $<INSTALL_INTERFACE:include> )
target_link_libraries(XPathReader LibXml2::LibXml2)
						   
add_library(xmlWriter   xml_simplewriter.cc attribute.cc xml_struct_writer.cc)
target_include_directories( xmlWriter PUBLIC
						   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
						   $<INSTALL_INTERFACE:include> )
target_link_libraries(xmlWriter LibXml2::LibXml2)

install(TARGETS XPathReader xmlWriter
		EXPORT XPathReader
		LIBRARY DESTINATION lib
		ARCHIVE DESTINATION lib
)
		