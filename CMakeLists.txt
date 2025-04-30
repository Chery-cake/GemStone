cmake_minimum_required(VERSION 3.30)
project(GemStone LANGUAGES CXX C)  # C for possible lexer generators

set(CMAKE_C_STANDARD 23)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)  # For tooling integration

# Platform-specific settings
if(WIN32)
    add_definitions(-DCOMPILER_WINDOWS)
else()
    add_definitions(-DCOMPILER_POSIX)
endif()

# Build options
option(BUILD_WITH_LLVM "Use LLVM for code generation" OFF)
option(ENABLE_DEBUG_OUTPUT "Enable compiler debug output" OFF)
option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

if(ENABLE_DEBUG_OUTPUT)
    add_definitions(-DDEBUG_OUTPUT)
endif()

if(BUILD_WITH_LLVM)
    find_package(LLVM REQUIRED CONFIG)
    add_definitions(-DUSE_LLVM)
    include_directories(${LLVM_INCLUDE_DIRS})
    link_directories(${LLVM_LIBRARY_DIR})
endif()

if(BUILD_SHARED_LIBS)
    set(LIBTYPE SHARED)
else()
    set(LIBTYPE STATIC)
endif()

# Set output directories\
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Add subprojects in dependency order
add_subdirectory(subprojects/scanner) # Lexical Analyzer
# add_subdirectory(subprojects/parser) # Syntax Analyzer
# add_subdirectory(subprojects/semantic) # Semantic Analysis

# add_subdirectory(subprojects/icg) # Intermediate Code Generation
# add_subdirectory(subprojects/tcg) # Target Code Generation

# add_subdirectory(subprojects/symbols) # Symbol Table
# add_subdirectory(subprojects/error) # Error Handling
add_subdirectory(subprojects/variables) # Global Variables
add_subdirectory(subprojects/cli) # Command Line Interface
