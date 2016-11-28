cmake_minimum_required(VERSION 3.5)



project(STLViewer)
#### External Project Defs:
include(ExternalProject)
set(EXTERNAL_PREFIX "${CMAKE_CURRENT_BINARY_DIR}/externals")
set(EXTERNAL_INSTALL_DIR "${CMAKE_CURRENT_BINARY_DIR}/local")
set(EXTERNAL_CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_DIR})

set_property(DIRECTORY PROPERTY EP_BASE ${EXTERNAL_PREFIX})

find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW glfw3)
if(GLFW_FOUND)
    include_directories(${glfw3_INLUDE_DIRS})
    link_libraries(${glfw3_LIBRARIES})
else(GLFW_FOUND)
    ExternalProject_Add(
            GLFW
            GIT_REPOSITORY "https://github.com/glfw/glfw"
            CMAKE_ARGS ${EXTERNAL_CMAKE_ARGS}
    )
    include_directories(${EXTERNAL_INSTALL_DIR}/include/GLFW)
    set(GLFW_LIBRARIES ${EXTERNAL_INSTALL_DIR}/lib/libglfw3.a)
endif(GLFW_FOUND)


#pkg_search_module(GLM glm)
find_package(glm)
if(glm_FOUND)
    include_directories(${GLM_INLUDE_DIRS})
else(glm_FOUND)
    ExternalProject_Add(
            GLM
            GIT_REPOSITORY "https://github.com/g-truc/glm"
#            UPDATE_COMMAND ""
#            INSTALL_DIR "${CMAKE_CURRENT_BINARY_DIR}/lib"
#            BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/lib
#            CMAKE_ARGS -DCMAKE_INSTALL=${CMAKE_CURRENT_BINARY_DIR}/lib
            CMAKE_ARGS ${EXTERNAL_CMAKE_ARGS}
    )
    include_directories(${EXTERNAL_INSTALL_DIR}/include/glm)
endif(glm_FOUND)

find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})
link_libraries(${GLEW_LIBRARIES})

find_package(OpenGL REQUIRED)

#ready to build:
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -g")
set(SOURCE_FILES STLData.cpp STLData.h window_GL.cpp window_GL.h renderer.cpp renderer.h Camera.h GLError.cpp GLError.h)
#Build the lib
add_library(STLViewerLib ${SOURCE_FILES})

add_dependencies(STLViewerLib GLFW GLM)

target_link_libraries(STLViewerLib ${GLFW_LIBRARIES} ${GLM_LIBRARIES} ${GLEW_LIBRARIES} GL)


#Build the executable
add_executable(STLViewer ${SOURCE_FILES} main.cpp shaders.h)

add_dependencies(STLViewer GLFW GLM)

target_link_libraries(STLViewer ${GLFW_LIBRARIES} ${GLM_LIBRARIES} ${GLEW_LIBRARIES} GL)

