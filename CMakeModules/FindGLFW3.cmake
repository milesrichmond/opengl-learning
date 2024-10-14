set( _glfw3_HEADER_SEARCH_DIRS
"/usr/include"
"/usr/local/include"
"${CMAKE_SOURCE_DIR}/includes"
"/opt/homebrew/Cellar/glfw/3.4/include"
)
set( _glfw3_LIB_SEARCH_DIRS
"/usr/lib"
"/usr/local/lib"
"${CMAKE_SOURCE_DIR}/lib"
"/opt/homebrew/Cellar/glfw/3.4/lib"
)

FIND_PATH(GLFW3_INCLUDE_DIR "GLFW/glfw3.h" PATHS ${_glfw3_HEADER_SEARCH_DIRS})

FIND_LIBRARY(GLFW3_LIBRARY NAMES glfw3 glfw PATHS ${_glfw3_LIB_SEARCH_DIRS})
MESSAGE(STATUS "Found GLFW3: ${GLFW3_LIBRARY}")
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GLFW3 DEFAULT_MSG GLFW3_LIBRARY GLFW3_INCLUDE_DIR)