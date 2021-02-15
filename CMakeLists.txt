cmake_minimum_required (VERSION 3.12)

get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(${PROJECT_NAME})

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -lm")

# GLEW
find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})
link_libraries(${GLEW_LIBRARIES})

# GLFW
find_package(glfw3 REQUIRED)

# OPENGL
find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})

# MY CODE
file(GLOB SOURCES "src/*.c")

add_executable(${PROJECT_NAME} ${SOURCES})

# GLSL SOURCES
add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/src/glsl/ $<TARGET_FILE_DIR:${PROJECT_NAME}>)
# ASSETS
add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/assets/ $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets/)

target_link_libraries(${PROJECT_NAME} ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES} glfw)
