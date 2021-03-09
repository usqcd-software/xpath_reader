cmake_minimum_required(VERSION 3.17)
project(XPathReader VERSION 1.4.0 
                    DESCRIPTION "XPath Reader for use by QDP++"
                    LANGUAGES CXX)
                    
# Basic Includes
include( GNUInstallDirs)
include( CMakePackageConfigHelpers)

# Enable Address address and undefined behaviour sanitizers
option(XPathReader_ENABLE_SANITIZERS "Enable Address and Undefined Behaviour Sanitizers" OFF)

# Find LibXML2 
find_package(LibXml2 REQUIRED)
 
# Create xpath_reader_config_internal.h
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/include/xpath_reader_config_internal.h.cmake.in
			   ${CMAKE_CURRENT_BINARY_DIR}/include/xpath_reader_config_internal.h)
			   
# Mark file for installation
install( FILES ${CMAKE_CURRENT_BINARY_DIR}/include/xpath_reader_config_internal.h DESTINATION include)

# Deal with Sanitizer
if( XPathReader_ENABLE_SANITIZERS )
  if( ( CMAKE_CXX_COMPILER_ID MATCHES "Clang" AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 3.1)
     OR ( CMAKE_CXX_COMPILER_ID MATCHES "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 4.8) )
      message(STATUS "XPathReader: Setting Sanitizer options")
      set(XPathReader_SANITIZER_OPTS "-fsanitize=address,undefined")
  else()
    message(STATUS "Don't know how to enable sanitizers for ${CMAKE_CXX_COMPILER_ID}... Ignoring")
  endif()
endif()    

 
# Configure Lib and Example
add_subdirectory(lib)
add_subdirectory(examples)
 
# install the headers 
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include
	DESTINATION .
    	FILES_MATCHING PATTERN "*.h")
 

    
# generate the version file for the config file
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/XPathReaderConfigVersion.cmake"
  VERSION "${XPathReader_VERSION_MAJOR}.${XPathReader_VERSION_MINOR}"
  COMPATIBILITY AnyNewerVersion
)

# Create a Package Config file.
configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
  "${CMAKE_CURRENT_BINARY_DIR}/XPathReaderConfig.cmake"
  INSTALL_DESTINATION lib/cmake/XPathReader
)
   
# install it
install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/XPathReaderConfig.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/XPathReaderConfigVersion.cmake
  DESTINATION lib/cmake/XPathReader
)

# export package. Does nothing by default, but
# can have uses in specially set up configurations
export(PACKAGE XPathReader)  
 
