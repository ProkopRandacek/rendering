cmake_minimum_required (VERSION 3.12)

get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(${PROJECT_NAME})

# GLEW
find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})
link_libraries(${GLEW_LIBRARIES})

# GLUT
find_package(GLUT REQUIRED)
include_directories(${GLUT_INCLUDE_DIR})

# OPENGL
find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})

# MY CODE
file(GLOB SOURCES "src/*.c")
file(GLOB SHADERS "shader/*.glsl")
source_group("Shaders" FILES ${SHADERS})

add_executable(${PROJECT_NAME} ${SOURCES} ${SHADERS})

target_link_libraries(${PROJECT_NAME} ${GLEW_LIBRARIES} ${GLUT_LIBRARIES} ${OPENGL_LIBRARIES})
