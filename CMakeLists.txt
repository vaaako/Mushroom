# CMAKE CONFIG #
cmake_minimum_required(VERSION 3.20)
project(LunarBat)

set(CMAKE_CXX_STANDARD 20)

# Set Clang as the C++ compiler
set(CMAKE_CXX_COMPILER "clang++")

# COMPILING CONFIG #
# Disable optimizations, enable additional warnings, and treat warnings as error-Wsign-conversions
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -g -fdiagnostics-color=always -Wextra -Wall -Wuninitialized -Winit-self -Wfloat-equal -Wundef -Wpointer-arith -Wcast-align -Wstrict-overflow=5 -Wwrite-strings -Wcast-qual -Wswitch-default -Wconversion -Wunreachable-code")


# Find and link against the editline library
find_library(READLINE_LIBRARY readline)
if(READLINE_LIBRARY)
	message(STATUS "Found readline library: ${READLINE_LIBRARY}")
else()
	message(FATAL_ERROR "editline library not found")
endif()


# Collect source files
file(GLOB_RECURSE SOURCE_FILES src/*.cpp src/**/*.cpp)

# Create the executable
add_executable(mushroom ${SOURCE_FILES})

# Append each library you want to link
list(APPEND LDLIBS editline)

# Link against the editline library
target_link_libraries(mushroom ${READLINE_LIBRARY})


# cmake -S . -B build
# cmake --build build
# ./build/your_defined_name
