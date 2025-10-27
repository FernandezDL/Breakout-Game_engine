cmake_minimum_required(VERSION 3.10)

project(GAME VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# --- raylib (ajusta la ruta si es necesario) ---
add_subdirectory("D:/UVG/Game_engine/raylib" "D:/UVG/Game_engine/raylib/build")
include_directories("D:/UVG/Game_engine/raylib/src")

# --- EnTT (header-only) ---
include(FetchContent)
FetchContent_Declare(
  entt
  GIT_REPOSITORY https://github.com/skypjack/entt.git
  GIT_TAG v3.13.2
)
FetchContent_MakeAvailable(entt)

# --- Fuentes ---
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/src/*.cpp")
add_executable(${PROJECT_NAME} ${SOURCES})

# Includes del proyecto + fallback explícito a single_include de EnTT
target_include_directories(${PROJECT_NAME}
  PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    "${entt_SOURCE_DIR}/single_include"
)

# Enlace
target_link_libraries(${PROJECT_NAME}
  raylib
  EnTT::EnTT
)
