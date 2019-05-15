cmake_minimum_required(VERSION 3.5)

project(debugGL)

#############################################################
##   system detect
#############################################################
MESSAGE(STATUS "operation system is ${CMAKE_SYSTEM}")  
	  
IF (CMAKE_SYSTEM_NAME MATCHES "Linux")  
	MESSAGE(STATUS "current platform: Linux ")  
	#add_definitions(-Dlinux)

ELSEIF (CMAKE_SYSTEM_NAME MATCHES "Windows")  
	MESSAGE(STATUS "current platform: Windows")  
	add_definitions(-Dwindows)
	SET(CMAKE_EXE_LINKER_FLAGS "/SAFESEH:NO")

ELSEIF (CMAKE_SYSTEM_NAME MATCHES "FreeBSD")  
	MESSAGE(STATUS "current platform: FreeBSD")  

ELSEIF (CMAKE_SYSTEM_NAME MATCHES "Darwin")  
	MESSAGE(STATUS "current platform: Darwin(mac?)")  
	add_definitions(-Dmac)

ELSE ()  
	MESSAGE(STATUS "other platform: ${CMAKE_SYSTEM_NAME}")  
ENDIF (CMAKE_SYSTEM_NAME MATCHES "Linux")  
#############################################################
##   system detect end
#############################################################


#############################################################
##   add opengl
#############################################################
IF (CMAKE_SYSTEM_NAME MATCHES "Windows")
#my windows does not need to find package
ELSE()
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
ENDIF (CMAKE_SYSTEM_NAME MATCHES "Windows")
#MESSAGE(STATUS "opengl lib: ${OPENGL_LIBRARIES}")  
#MESSAGE(STATUS "glut lib: ${GLUT_LIBRARY}")  
#############################################################

#############################################################
##   add lua
#############################################################
find_package(Lua REQUIRED)
#MESSAGE(STATUS "lua lib: ${LUA_LIBRARIES}")  
#MESSAGE(STATUS "lua inc: ${LUA_INCLUDE_DIR}")  
#${LUA_INCLUDE_DIR}
#${LUA_LIBRARIES}
#############################################################

#----------------------------------------------------------#
# opencv
find_package( OpenCV REQUIRED )

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

INCLUDE (FindPackageHandleStandardArgs)
	FIND_PACKAGE_HANDLE_STANDARD_ARGS (AprilTag
	FOUND_VAR AprilTag_FOUND
	REQUIRED_VARS AprilTag_LIB AprilTag_INCLUDE_DIR)

IF (NOT AprilTag_FOUND)
	MESSAGE (WARNING "The AprilTag Library has not been found!")
ENDIF (NOT AprilTag_FOUND)
#=============================================================================

include_directories(${OPENGL_INCLUDE_DIRS} 
			${GLUT_INCLUDE_DIRS}
			${LUA_INCLUDE_DIR}
			src/GLTools
			src/CVector
			src/CParticle
			src/Robot
			src/CWorld
			src/CellularAutomaton
            ${OpenCV_INCLUDE_DIRS}
            ${AprilTag_INCLUDE_DIR}
		)

set(CMAKE_BUILD_TYPE Debug)

add_executable(main src/GLTools/MainGL.cpp
					src/MainLoop.cpp
					src/GLTools/DrawFunction.cpp

					src/CVector/Vector3.cpp
					src/CVector/Matrix3.cpp
					src/CVector/Quaternion.cpp

					src/CParticle/QParticle.cpp	
					src/CParticle/Object.cpp	
					src/CParticle/Box.cpp	
					src/CParticle/Sphere.cpp	
					src/CParticle/Cylinder.cpp
					src/CParticle/Convexhull.cpp
					src/CParticle/DBox.cpp	

					src/Robot/Manipulator.cpp	

					src/CWorld/ChessWorld.cpp	
					src/CellularAutomaton/CellularAutomaton.cpp
			  )



target_link_libraries(main 	${OpenCV_LIBS}
                            ${OPENGL_LIBRARIES} 
							${GLUT_LIBRARY}
							${LUA_LIBRARIES}                    
                            ${AprilTag_LIB}
		)
