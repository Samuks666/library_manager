# Fetch Google Test
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Test executables
add_executable(test_library
    test_library.cpp
)

add_executable(test_book
    test_book.cpp
)

add_executable(test_genre
    test_genre.cpp
)

add_executable(test_author
    test_author.cpp
)

# Link libraries and configure each test
foreach(test test_library test_book test_genre test_author)
    # Link with core and utils libraries
    target_link_libraries(${test}
        PRIVATE
            gtest_main
            core
            utils
    )
    
    # Include directories from parent project
    target_include_directories(${test}
        PRIVATE
            ${PROJECT_SOURCE_DIR}/include
    )
    
    # C++ standard
    set_target_properties(${test} PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
    )
    
    # Apply same diagnostics as main app
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        if(ENABLE_SANITIZERS)
            target_compile_options(${test} PRIVATE
                -fsanitize=address,undefined
                -fno-omit-frame-pointer
                -O1
            )
            target_link_options(${test} PRIVATE
                -fsanitize=address,undefined
            )
        endif()
    endif()
    
    # Warnings
    target_compile_options(${test} PRIVATE
        -Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wnull-dereference
    )
    
    # Register test
    add_test(
        NAME ${test}
        COMMAND ${test}
        WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
    )
endforeach()

message(STATUS "Unit tests configured (Google Test)")
