cmake_minimum_required(VERSION 3.1.3)
project(BPPrint CXX)

# Enable testing?
enable_testing()

# Options to set
# By default, use C++14 compilation
set(CMAKE_CXX_EXTENSIONS False CACHE BOOL "Enable/Disable compiler-specific C++ extensions")
set(CMAKE_CXX_STANDARD 14 CACHE STRING "Which C++ standard to use")
set(BPPRINT_BUILD_SHARED FALSE CACHE BOOL "Build a shared library, rather than a static library")
set(BPPRINT_BUILD_PIC FALSE CACHE BOOL "Build the static library with PIC enabled")

# CMake doesn't support Intel CXX standard until cmake 3.6
if("${CMAKE_CXX_COMPILER_ID}" MATCHES "Intel")
  if("${CMAKE_VERSION}" VERSION_LESS "3.6")
      add_compile_options(-std=c++${CMAKE_CXX_STANDARD})
  endif()
endif()


# Set up the library type. This variable is passed to add_library
set(BPPRINT_BUILD_TYPE "STATIC")
if(BPPRINT_BUILD_SHARED)
  set(BPPRINT_BUILD_TYPE "SHARED")
endif()


# Set the compile options
if("${CMAKE_CXX_COMPILER_ID}" MATCHES "Intel")
    add_compile_options(-w3)

    #   383  : value copied to temporary, reference to temporary used
    #   981  : operands are evaluated in unspecified order
    #  1418  : external function definition with no prior declaration
    add_compile_options(-wd383)
    add_compile_options(-wd981)
    add_compile_options(-wd1418)
elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
    add_compile_options(-Wall;-Wextra;-pedantic)
elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    add_compile_options(-Wall;-Wextra;-pedantic)
    add_compile_options(-Wno-format-nonliteral)

    # for heavy-duty testing
    add_compile_options(-Weverything)
    add_compile_options(-Wno-c++98-compat)
    add_compile_options(-Wno-c++98-compat-pedantic)
    add_compile_options(-Wno-implicit-fallthrough)
    add_compile_options(-Wno-padded)
    add_compile_options(-Wno-weak-vtables)
    add_compile_options(-Wno-covered-switch-default)
endif()

# The main subdirectory with the bpprint library
add_subdirectory(bpprint)

# Directory with some tests
add_subdirectory(test)


# Create and install the config file
configure_file("bpprintConfig.cmake.in" "bpprintConfig.cmake" @ONLY)
install(FILES ${CMAKE_BINARY_DIR}/bpprintConfig.cmake DESTINATION lib/cmake/bpprint) 
