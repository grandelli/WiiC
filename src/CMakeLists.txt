CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(WiiC)

# Preliminary stuff
IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	SET(LINUX 1)
	ADD_DEFINITIONS(-DLINUX)
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules/")

SET(BUILD_SHARED_LIBS ON CACHE BOOL "Build shared libs")
SET(CMAKE_BUILD_TYPE "Release" CACHE STRING "Release")
SET(CMAKE_CXX_FLAGS_DEBUG "-g -Wall")
SET(CMAKE_CXX_FLAGS_RELEASE "-O3")

# Third-party libraries
FIND_PACKAGE(Bluetooth REQUIRED)

FIND_PACKAGE(OpenCV QUIET)
IF(OpenCV_FOUND)
	MESSAGE(STATUS "Found OpenCV:")
	MESSAGE(STATUS "   - Include Dir: ${OpenCV_INCLUDE_DIRS}")
	MESSAGE(STATUS "   - Libs: ${OpenCV_LIBS}")
	MESSAGE(STATUS "   - Version: ${OpenCV_VERSION}")
	IF("${OpenCV_VERSION}" STRLESS "2.2.0")
		ADD_DEFINITIONS(-DCV_OLD)
	ENDIF("${OpenCV_VERSION}" STRLESS "2.2.0")
ELSE(OpenCV_FOUND)
	MESSAGE(STATUS "OpenCV not found but not required. WiiC-ML will not be compiled.")
ENDIF(OpenCV_FOUND)

# Source code
ADD_SUBDIRECTORY(wiic)
ADD_SUBDIRECTORY(wiicpp)
ADD_SUBDIRECTORY(ml)
ADD_SUBDIRECTORY(bin)

# Config dir
MAKE_DIRECTORY($ENV{HOME}/.wiic)

# Documentation
SET(DOXYFILE_OUTPUT_DIR "${CMAKE_SOURCE_DIR}/../doc")
include(UseDoxygen)

# Uninstall Target
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)

add_custom_target(uninstall COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)

