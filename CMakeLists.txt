cmake_minimum_required(VERSION 3.5)

#set(simu_Lua on)
add_subdirectory(../simuGL/src
                 ${CMAKE_CURRENT_BINARY_DIR}/simuGL
)

# opencv
find_package( OpenCV REQUIRED )

# apriltag
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

INCLUDE (FindPackageHandleStandardArgs)
	FIND_PACKAGE_HANDLE_STANDARD_ARGS (AprilTag
	FOUND_VAR AprilTag_FOUND
	REQUIRED_VARS AprilTag_LIB AprilTag_INCLUDE_DIR)

IF (NOT AprilTag_FOUND)
	MESSAGE (WARNING "The AprilTag Library has not been found!")
ENDIF (NOT AprilTag_FOUND)
#   AprilTag_INCLUDE_DIR, where to find TagDetector.h, etc.
#   AprilTag_LIB, libraries to link against to use AprilTag.

set(CMAKE_BUILD_TYPE Debug)

add_definitions(
	${simu_definitions}
)

add_executable(main 
	${simu_cpps}
	MainLoop.cpp
	solve_square.c
)

include_directories( 
	${simu_include_dirs}
	${AprilTag_INCLUDE_DIR}
)

target_link_libraries(main 	
	${simu_link_libraries}
	${OpenCV_LIBS}
	${AprilTag_LIB}
)
