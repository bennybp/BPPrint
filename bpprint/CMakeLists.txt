# Additional compilation flags
set(BPPRINT_EXTRA_CXX_FLAGS "" CACHE BOOL "Additional compilations flags for the BPPrint library (semicolon-separated)")
set(BPPRINT_BUILD_SHARED FALSE CACHE BOOL "Build a shared library, rather than a static library")

# Set up the library type. This variable is passed to add_library
set(BPPRINT_BUILD_TYPE "STATIC")
if(BPPRINT_BUILD_SHARED)
  set(BPPRINT_BUILD_TYPE "SHARED")
endif()


# Set the compile options
if("${CMAKE_CXX_COMPILER_ID}" MATCHES "Intel")
    list(APPEND BPPRINT_CXX_FLAGS "-std=c++11")
    list(APPEND BPPRINT_CXX_FLAGS "-w3")

    #   383  : value copied to temporary, reference to temporary used
    #   981  : operands are evaluated in unspecified order
    #  1418  : external function definition with no prior declaration
    list(APPEND BPPRINT_CXX_FLAGS "-wd383")
    list(APPEND BPPRINT_CXX_FLAGS "-wd981")
    list(APPEND BPPRINT_CXX_FLAGS "-wd1418")
elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
    list(APPEND BPPRINT_CXX_FLAGS "-std=c++11")
    list(APPEND BPPRINT_CXX_FLAGS "-Wall;-Wextra;-pedantic")
elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    list(APPEND BPPRINT_CXX_FLAGS "-std=c++11")
    list(APPEND BPPRINT_CXX_FLAGS "-Wall;-Wextra;-pedantic")

    # for heavy-duty testing
    list(APPEND BPPRINT_CXX_FLAGS "-Weverything")
    list(APPEND BPPRINT_CXX_FLAGS "-Wno-c++98-compat")
    list(APPEND BPPRINT_CXX_FLAGS "-Wno-c++98-compat-pedantic")
    list(APPEND BPPRINT_CXX_FLAGS "-Wno-implicit-fallthrough")
    list(APPEND BPPRINT_CXX_FLAGS "-Wno-padded")
    list(APPEND BPPRINT_CXX_FLAGS "-Wno-weak-vtables")
    list(APPEND BPPRINT_CXX_FLAGS "-Wno-covered-switch-default")
    list(APPEND BPPRINT_CXX_FLAGS "-Wno-format-nonliteral")
endif()
  

# This is the main library
add_library(bpprint ${BPPRINT_BUILD_TYPE}
                   Printf_wrap.cpp 
                   Format.cpp  
           )

# Include the main source directory (my parent) as an include directory
target_include_directories(bpprint PRIVATE ${CMAKE_SOURCE_DIR})

# Add the extra flags to the actual build flags
list(APPEND BPPRINT_CXX_FLAGS ${BPPRINT_EXTRA_CXX_FLAGS})

target_compile_options(bpprint PRIVATE ${BPPRINT_CXX_FLAGS})


# Where to install the library
install(TARGETS bpprint
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)

# Install the headers
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} DESTINATION include FILES_MATCHING PATTERN "*.hpp")

