cmake_minimum_required(VERSION 3.16)

project(LibraryManager
    VERSION 1.0
    LANGUAGES C CXX
)

# --------------------------------------------------------------------
# Global configuration
# --------------------------------------------------------------------

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Export compile_commands.json (clangd, lsp, etc)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Default build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "" FORCE)
endif()

# --------------------------------------------------------------------
# Options
# --------------------------------------------------------------------

option(ENABLE_TESTING    "Enable unit tests with Google Test" ON)
option(ENABLE_SANITIZERS "Enable Address/UB Sanitizers (Debug only)" ON)
option(ENABLE_WARNINGS   "Enable extra compiler warnings" ON)

# --------------------------------------------------------------------
# Helper Functions (Professional CMake Style)
# --------------------------------------------------------------------

function(enable_project_warnings target)
    if(ENABLE_WARNINGS)
        target_compile_options(${target} PRIVATE
            -Wall
            -Wextra
            -Wpedantic
            -Wshadow
            -Wconversion
            -Wnull-dereference
        )
    endif()
endfunction()

function(enable_sanitizers target)
    if(ENABLE_SANITIZERS AND CMAKE_BUILD_TYPE STREQUAL "Debug")
        message(STATUS "Sanitizers enabled (Debug): ${target}")
        target_compile_options(${target} PRIVATE
            -fsanitize=address,undefined
            -fno-omit-frame-pointer
            -O1
            -g
        )
        target_link_options(${target} PRIVATE
            -fsanitize=address,undefined
        )
    elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
        message(STATUS "Sanitizers disabled (Release): ${target}")
    endif()
endfunction()

# --------------------------------------------------------------------
# Libraries
# --------------------------------------------------------------------

add_library(core
    src/core/book.c
    src/core/author.c
    src/core/genre.c
    src/core/library.c
)

target_include_directories(core
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include/core
        ${PROJECT_SOURCE_DIR}/include/utils
)

enable_project_warnings(core)
enable_sanitizers(core)

# ---- utils ----------------------------------------------------------

add_library(utils
    src/utils/log.c
)

target_include_directories(utils
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include/utils
)

enable_project_warnings(utils)
enable_sanitizers(utils)

# --------------------------------------------------------------------
# Executable
# --------------------------------------------------------------------

add_executable(test_app
    src/main.c
)

target_link_libraries(test_app
    PRIVATE
        core
        utils
)

enable_project_warnings(test_app)
enable_sanitizers(test_app)

# --------------------------------------------------------------------
# Testing with Google Test
# --------------------------------------------------------------------

if(ENABLE_TESTING)
    enable_testing()
    add_subdirectory(tests)
else()
    message(STATUS "Unit tests disabled")
endif()

# --------------------------------------------------------------------
# Build Info
# --------------------------------------------------------------------

message(STATUS "")
message(STATUS "====== Library Manager Configuration ======")
message(STATUS "Build type:       ${CMAKE_BUILD_TYPE}")
message(STATUS "Sanitizers:       ${ENABLE_SANITIZERS} (Debug only)")
message(STATUS "Tests:            ${ENABLE_TESTING}")
message(STATUS "Warnings:         ${ENABLE_WARNINGS}")
message(STATUS "==========================================")
message(STATUS "")