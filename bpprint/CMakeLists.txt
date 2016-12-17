# This is the main library
add_library(bpprint ${BPPRINT_BUILD_TYPE}
                   Printf_wrap.cpp 
                   Format.cpp  
           )

# Include the main source directory (my parent) as an include directory
target_include_directories(bpprint PRIVATE ${CMAKE_SOURCE_DIR})

# Enable position-independent code?
# Should be safe to enable for shared as well
if(BPPRINT_BUILD_PIC)
    set_property(TARGET bpprint PROPERTY POSITION_INDEPENDENT_CODE True)
endif()

# Where to install the library
install(TARGETS bpprint
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)

# Install the headers
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} DESTINATION include FILES_MATCHING PATTERN "*.hpp")

