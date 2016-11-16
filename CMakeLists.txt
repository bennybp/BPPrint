cmake_minimum_required(VERSION 3.0.2)
project(BPPrint CXX)

# Enable testing?
enable_testing()

# The main subdirectory with the bpprint library
add_subdirectory(bpprint)

# Directory with some tests
add_subdirectory(test)


# Create and install the config file
configure_file("bpprintConfig.cmake.in" "bpprintConfig.cmake" @ONLY)
install(FILES ${CMAKE_BINARY_DIR}/bpprintConfig.cmake DESTINATION lib/cmake/bpprint) 
