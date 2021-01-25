cmake_minimum_required(VERSION 3.15)
project(XPathReader VERSION 1.4.0 
                    DESCRIPTION "XPath Reader for use by QDP++"
                    LANGUAGES CXX)
                    
# Basic Includes
include( GNUInstallDirs)
include( CMakePackageConfigHelpers)
 
# Find LibXML2 
find_package(LibXml2 REQUIRED)
 
# Create xpath_reader_config_internal.h
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/include/xpath_reader_config_internal.h.cmake.in
			   ${CMAKE_CURRENT_BINARY_DIR}/include/xpath_reader_config_internal.h)
			   
# Mark file for installation
install( FILES ${CMAKE_CURRENT_BINARY_DIR}/include/xpath_reader_config_internal.h DESTINATION include)

 
# Configure Lib and Example
add_subdirectory(lib)
add_subdirectory(examples)
 
# install the headers 
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include
    	DESTINATION ${CMAKE_INSTALL_PREFIX}
    	FILES_MATCHING PATTERN "*.h")
 		
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
       
# generate the version file for the config file
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/XPathReaderConfigVersion.cmake"
  VERSION "${XPathReader_VERSION_MAJOR}.${XPathReader_VERSION_MINOR}"
  COMPATIBILITY AnyNewerVersion
)

# install it
install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/XPathReaderConfigVersion.cmake
  DESTINATION lib/cmake/XPathReader
)

# export package. Does nothing by default, but
# can have uses in specially set up configurations
export(PACKAGE XPathReader)  
 