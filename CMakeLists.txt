cmake_minimum_required(VERSION 3.3)

set(PROJECT_NAME task3)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")
set(BIN_DIR ${CMAKE_HOME_DIRECTORY}/bin)
set(LIB_DIR ${CMAKE_HOME_DIRECTORY}/lib)
file(MAKE_DIRECTORY ${BIN_DIR})

project(${PROJECT_NAME})
include_directories(${LIB_DIR}/glm)
link_directories(${LIB_DIR})
set(SOURCE_FILES
        src/Triangles/Branch.cpp
        src/Triangles/Branch.h
        src/Triangles/Fragment.frag
        src/Triangles/Leaf.cpp
        src/Triangles/Leaf.h
        src/Triangles/main.cpp
        src/Triangles/MyObject.cpp
        src/Triangles/MyObject.h
        src/Triangles/shader.cpp
        src/Triangles/shader.h
        src/Triangles/shaderhelper.h
        src/Triangles/shaderprogram.cpp
        src/Triangles/shaderprogram.h
        src/Triangles/tree.cpp
        src/Triangles/tree.h)

add_executable(task3 ${SOURCE_FILES})

target_link_libraries(task3 -lGLEW -lGLU -lGL -lglut)
# change build directory to "BUILD_DIR"
set_target_properties(task3 PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR})

