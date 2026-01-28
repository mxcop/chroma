cmake_minimum_required(VERSION 3.15)
project(colorspaces)

# C++ standard version
set(CMAKE_CXX_STANDARD 17 CACHE STRING "" FORCE)

# Executable target
add_executable(colorspaces)
set_target_properties(colorspaces PROPERTIES OUTPUT_NAME ${CMAKE_PROJECT_NAME})

# Add compiler (debug & release) definitions
target_compile_definitions(colorspaces PRIVATE $<$<CONFIG:Debug>:DEBUG=1>)
target_compile_definitions(colorspaces PRIVATE $<$<CONFIG:Release>:NDEBUG=1>)

# Include directories & pre-compiled header
target_include_directories(colorspaces PRIVATE "src/")

# Glob all source files
file(GLOB_RECURSE src "src/*.cc")
target_sources(colorspaces PRIVATE ${src})
