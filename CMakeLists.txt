cmake_minimum_required(VERSION 3.21)
project(cp LANGUAGES C)

# Select optimization independently of debug information.
set(CP_OPTIMIZATION_LEVEL "0" CACHE STRING "Clang optimization level: 0, 1, 2, or 3")
set_property(CACHE CP_OPTIMIZATION_LEVEL PROPERTY STRINGS 0 1 2 3)
if(NOT "${CP_OPTIMIZATION_LEVEL}" MATCHES "^[0-3]$")
    message(FATAL_ERROR "CP_OPTIMIZATION_LEVEL must be 0, 1, 2, or 3")
endif()

add_executable(entry
    entry.c
    memory.c
    int_math.c
    sys_call.c
    exit.c
)

set_target_properties(entry PROPERTIES
    C_STANDARD 23
    C_STANDARD_REQUIRED YES
    C_EXTENSIONS NO
)

# Check direct header dependencies with the installed IWYU tool.
option(CP_ENABLE_IWYU "Run include-what-you-use during compilation" ON)
if(CP_ENABLE_IWYU)
    find_program(CP_IWYU_EXECUTABLE
        NAMES include-what-you-use
        HINTS "${CMAKE_CURRENT_SOURCE_DIR}/.toolchain/bin"
        REQUIRED
    )
    set_property(TARGET entry PROPERTY
        C_INCLUDE_WHAT_YOU_USE "${CP_IWYU_EXECUTABLE};-Xiwyu;--no_internal_mappings"
    )
endif()

# Enable CHECK only for CMake Debug builds, independently of optimization.
target_compile_definitions(entry PRIVATE
    "$<$<CONFIG:Debug>:CP_DEBUG=1>"
)

# Keep compiler-generated code independent of the hosted C runtime.
target_compile_options(entry PRIVATE
    -ffreestanding
    -fno-stack-protector
    -Wall
    -Wextra
    "-O${CP_OPTIMIZATION_LEVEL}"
)

# Supply our own startup code and use Linux syscalls directly.
target_link_options(entry PRIVATE
    -nostdlib
    -static
    "LINKER:-e,_start"
)
