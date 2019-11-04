cmake_minimum_required(VERSION 2.8)
project( testLoopFunction )

find_package( OpenCV REQUIRED )
find_package( Lua REQUIRED )
#${LUA_INCLUDE_DIR}
#${LUA_LIBRARIES}

#=============================================================================
# - Find the AprilTag Library
#=============================================================================
# This module defines
#   AprilTag_INCLUDE_DIR, where to find TagDetector.h, etc.
#   AprilTag_LIB, libraries to link against to use AprilTag.
#   AprilTag_FOUND, libraries to link against to use AprilTag.
#
#=============================================================================

SET (AprilTag_FOUND 0)
FIND_PATH (AprilTag_INCLUDE_DIR
	NAMES apriltag.h
	PATH_SUFFIXES apriltag
	DOC "The AprilTag include directory"
)

FIND_LIBRARY (AprilTag_LIB
	NAMES apriltag
	DOC "The AprilTag shared library"
)

#=============================================================================

INCLUDE (FindPackageHandleStandardArgs)
	FIND_PACKAGE_HANDLE_STANDARD_ARGS (AprilTag
	FOUND_VAR AprilTag_FOUND
	REQUIRED_VARS AprilTag_LIB AprilTag_INCLUDE_DIR)

IF (NOT AprilTag_FOUND)
	MESSAGE (WARNING "The AprilTag Library has not been found!")
ENDIF (NOT AprilTag_FOUND)

#======= ARGoS Package ============

# Find the ARGoS package, make sure to save the ARGoS prefix
find_package(PkgConfig)
pkg_check_modules(ARGOS REQUIRED argos3_simulator)
set(ARGOS_PREFIX ${ARGOS_PREFIX} CACHE INTERNAL "")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ARGOS_PREFIX}/share/argos3/cmake)

link_directories(${ARGOS_LIBRARY_DIRS})

include_directories(    
	${OpenCV_INCLUDE_DIRS} 
	${AprilTag_INCLUDE_DIR} 
	${LUA_INCLUDE_DIR}
)

add_library(testLoopFunction SHARED
		   		testLoopFunction.cpp
				testLoopFunction.h
                LEDprojection.cpp
			)
MESSAGE(
${LUA_LIBRARIES}
)

target_link_libraries( testLoopFunction
${OpenCV_LIBS} 
${AprilTag_LIB} 
${LUA_LIBRARIES}
argos3core_simulator
argos3plugin_simulator_entities
)

