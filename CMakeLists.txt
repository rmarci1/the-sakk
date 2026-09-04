cmake_minimum_required(VERSION 3.20)

project(sakk
    VERSION 1.0
    LANGUAGES C
)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_compile_options(
    -Wall
    -Wextra
    -Wpedantic
)

add_executable(sakk
    src/main.c
    src/board.c
    src/game.c
    src/pieces.c
    src/input.c
    src/move.c
)

target_include_directories(sakk
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)