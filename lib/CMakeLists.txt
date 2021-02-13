add_library(XPathReader basic_xpath_reader.cc)
target_include_directories( XPathReader PUBLIC
						   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
						   $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/../include>
						   $<INSTALL_INTERFACE:include> )
target_link_libraries(XPathReader LibXml2::LibXml2)
						   
add_library(xmlWriter xml_simplewriter.cc attribute.cc xml_struct_writer.cc)
target_include_directories( xmlWriter PUBLIC
						   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
						   $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/../include>
						   $<INSTALL_INTERFACE:include> )
target_link_libraries(xmlWriter LibXml2::LibXml2)


install(TARGETS XPathReader xmlWriter
		EXPORT XPathReaderTargets
		ARCHIVE DESTINATION lib
		LIBRARY DESTINATION lib
		RUNTIME DESTINATION bin
		INCLUDES DESTINATION include)
  
install(EXPORT XPathReaderTargets
		FILE XPathReaderTargets.cmake
		NAMESPACE XPathReader::
		DESTINATION lib/cmake/XPathReader)  
		

add_library(XPathReader::xmlWriter ALIAS xmlWriter)
add_library(XPathReader::XPathReader ALIAS XPathReader)		