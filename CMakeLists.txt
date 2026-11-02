cmake_minimum_required(VERSION 3.21)
project(cp LANGUAGES C)

add_executable(entry entry.c)

set_target_properties(entry PROPERTIES
    C_STANDARD 23
    C_STANDARD_REQUIRED YES
    C_EXTENSIONS NO
)

# Keep compiler-generated code independent of the hosted C runtime.
target_compile_options(entry PRIVATE
    -ffreestanding
    -fno-stack-protector
    -Wall
    -Wextra
    "$<$<CONFIG:Debug>:-O0>"
)

# Supply our own startup code and use Linux syscalls directly.
target_link_options(entry PRIVATE
    -nostdlib
    -static
    "LINKER:-e,_start"
)
