cmake_minimum_required (VERSION 3.0)
cmake_policy (VERSION 3.0)

project (openGL-Learning)

set(CMAKE_CXX_STANDARD 17) # this does nothing for MSVC, use target_compile_options below
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMakeModules")

set (CMAKE_CXX_FLAGS "-Wall -g")

link_directories(${CMAKE_SOURCE_DIR}/lib)

find_package(GLM REQUIRED)
message(STATUS "GLM included at ${GLM_INCLUDE_DIR}")
find_package(GLFW3 REQUIRED)
message(STATUS "GLFW3 included at ${GLFW3_INCLUDE_DIR}, :${GLFW_LIBRARY}")
find_package(ASSIMP REQUIRED)
message(STATUS "ASSIMP included at ${ASSIMP_INCLUDE_DIR}")
find_package(Freetype REQUIRED)
message(STATUS "Found Freetype in ${FREETYPE_INCLUDE_DIRS}")

INCLUDE_DIRECTORIES(/System/Library/Frameworks)
FIND_LIBRARY(COCOA_LIBRARY Cocoa)
FIND_LIBRARY(OpenGL_LIBRARY OpenGL)
FIND_LIBRARY(IOKit_LIBRARY IOKit)
FIND_LIBRARY(CoreVideo_LIBRARY CoreVideo)
MARK_AS_ADVANCED(COCOA_LIBRARY OpenGL_LIBRARY)
set(APPLE_LIBS ${COCOA_LIBRARY} ${IOKit_LIBRARY} ${OpenGL_LIBRARY} ${CoreVideo_LIBRARY})
set(APPLE_LIBS ${APPLE_LIBS} ${GLFW3_LIBRARY} ${ASSIMP_LIBRARY} ${FREETYPE_LIBRARIES})
set(LIBS ${LIBS} ${APPLE_LIBS})
MESSAGE(MESSAGE "Using ${CMAKE_CXX_COMPILER}")
MESSAGE(MESSAGE "Using ${CMAKE_C_COMPILER}")

configure_file(configuration/root_dir.h.in configuration/root_directory.h)
include_directories(${CMAKE_BINARY_DIR}/configuration)

add_library(STB_IMAGE "${CMAKE_SOURCE_DIR}/src/stb_image.cpp")
add_library(GLAD "src/glad.c")
set(LIBS ${LIBS} STB_IMAGE GLAD) 

add_executable(${PROJECT_NAME} "${CMAKE_SOURCE_DIR}/src/main.cpp")
target_link_libraries(${PROJECT_NAME} ${LIBS})

include_directories(${GLM_INCLUDE_DIR} ${GLFW3_INCLUDE_DIR} ${ASSIMP_INCLUDE_DIR} ${FREETYPE_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/includes)