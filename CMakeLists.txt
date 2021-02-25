cmake_minimum_required( VERSION 3.1 )

project( rendering )

find_package( OpenGL REQUIRED )

include_directories( ${OPENGL_INCLUDE_DIRS} )

set( GLFW_BUILD_DOCS OFF CACHE BOOL  "GLFW lib only" )
set( GLFW_INSTALL OFF CACHE BOOL  "GLFW lib only" )

add_subdirectory( glfw )

#add_subdirectory( glew )

file(GLOB SRC "src/*.c")

add_executable( rendering WIN32 ${SRC} )
target_link_libraries( rendering ${OPENGL_LIBRARIES} glfw GLEW )
