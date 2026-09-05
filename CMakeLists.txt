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

add_executable(chess
    src/main.c
    src/board.c
    src/game.c
    src/piece.c
    src/ui.c
    src/move.c
    src/list.c
)

target_include_directories(chess
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)
set_target_properties(chess PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}"
)