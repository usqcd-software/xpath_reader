cmake_minimum_required(VERSION 3.15)
project(XPathReader VERSION 2.0 
                    DESCRIPTION "XPath Reader for use by QDP++"
                    LANGUAGES CXX)
                    
# Basic Includes
include( GNUInstallDirs)
include( CMakePackageConfigHelpers)
 
# Find LibXML2 
find_package(LibXml2 REQUIRED)
 
# Configure Lib and Example
add_subdirectory(lib)
add_subdirectory(examples)
 
# install the headers 
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include
    	DESTINATION ${CMAKE_INSTALL_PREFIX})
 		
# write the file exporting the targets
install(EXPORT XPathReader
        FILE XPathReaderTargets.cmake
        NAMESPACE XPathReader::
        DESTINATION lib/cmake/XPathReader
) 
 
# Create a Package Config file.
configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
  "${CMAKE_CURRENT_BINARY_DIR}/XPathReaderConfig.cmake"
  INSTALL_DESTINATION lib/cmake/XPathReader
)

install(FILES
          "${CMAKE_CURRENT_BINARY_DIR}/XPathReaderConfig.cmake"
        DESTINATION lib/cmake/XPathReader
) 
         
 