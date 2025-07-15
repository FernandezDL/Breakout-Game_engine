cmake_minimum_required(VERSION 3.10)

project(GAME VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED true)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_COMPILE_COMMANDS ON)

add_subdirectory("D:/UVG/Game_engine/raylib" 
                 "D:/UVG/Game_engine/raylib/build")
include_directories("D:/UVG/Game_engine/raylib/src")

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/src/*.cpp")

add_executable(${PROJECT_NAME} src/main.cpp ${SOURCES})

target_include_directories(${PROJECT_NAME}
  PRIVATE ${PROJECT_SOURCE_DIR}/include
  PUBLIC ${PROJECT_SOURCE/DIR}/src
)

target_link_libraries(${PROJECT_NAME}
  raylib
)


