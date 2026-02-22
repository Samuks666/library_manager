cmake_minimum_required(VERSION 3.16)

project(LibraryManager C)

set(CMAKE_C_STANDARD 11)

add_library(core
    src/core/book.c
    src/core/author.c
    src/core/library.c
)

target_include_directories(core
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)

add_executable(test_app
    src/main.c
)

target_link_libraries(test_app core)
