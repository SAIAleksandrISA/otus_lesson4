cmake_minimum_required(VERSION 3.5)

project(print_ip  VERSION 1.0.0)
add_executable(print_ip print_ip.cpp)

set_target_properties(print_ip PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

if (MSVC)
    target_compile_options(print_ip PRIVATE
        /W4
    )
else ()
    target_compile_options(print_ip PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
endif()

if (MSVC)
    target_compile_options(print_ip PRIVATE /W4)
else ()
    target_compile_options(print_ip PRIVATE -Wall -Wextra -pedantic -Werror)
endif()

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/version.h.in" 
    "${CMAKE_BINARY_DIR}/version.h"          
)
target_include_directories(print_ip PRIVATE "${CMAKE_BINARY_DIR}")