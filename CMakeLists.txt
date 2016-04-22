cmake_minimum_required(VERSION 3.5)

project(STLViewer)
#### External Project Defs:
include(ExternalProject)

find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW glfw3)

if(GLFW_FOUND)
    include_directories(${GLFW_INLUDE_DIRS})
else(GLFW_FOUND)
    ExternalProject_Add(
            GFLW
            GIT_REPOSITORY "https://github.com/glfw/glfw"
            INSTALL_COMMAND ""
            UPDATE_COMMAND ""
    )
endif(GLFW_FOUND)


pkg_search_module(GLM glm)
if(GLM_FOUND)
    include_directories(${GlM_INLUDE_DIRS})
else(GLM_FOUND)
    ExternalProject_Add(
            GLM
            GIT_REPOSITORY "https://github.com/g-truc/glm"
            INSTALL_COMMAND ""
            UPDATE_COMMAND ""
    )

endif(GLM_FOUND)

find_package(GLEW)
if (GLEW_FOUND)
    include_directories(${GLEW_INCLUDE_DIRS})
    link_libraries(${GLEW_LIBRARIES})
else(GLEW_FOUND)
    ExternalProject_Add(
            GLEW
            GIT_REPOSITORY "https://github.com/nigels-com/glew.git"
            INSTALL_COMMAND ""
            UPDATE_COMMAND ""
    )
    include_directories(${GLEW_INCLUDE_DIRS})
    link_libraries(${GLEW_LIBRARIES})
endif()

find_package(OpenGL REQUIRED)

#ready to build:
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -g")
set(SOURCE_FILES main.cpp STLData.cpp STLData.h window_GL.cpp window_GL.h renderer.cpp renderer.h Camera.h GLError.cpp GLError.h)
add_executable(STLViewer ${SOURCE_FILES})
add_custom_command(TARGET STLViewer PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/shaders $<TARGET_FILE_DIR:STLViewer>/shaders)

target_link_libraries(STLViewer ${GLFW_LIBRARIES} ${GLEW_LIBRARIES} GL)
