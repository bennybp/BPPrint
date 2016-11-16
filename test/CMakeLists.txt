# Testing of BPPrint

# Set the compile options
if("${CMAKE_CXX_COMPILER_ID}" MATCHES "Intel")
    list(APPEND BPPRINT_TEST_CXX_FLAGS "-std=c++11")
    list(APPEND BPPRINT_TEST_CXX_FLAGS "-w3")

    #   383  : value copied to temporary, reference to temporary used
    #   981  : operands are evaluated in unspecified order
    #  1418  : external function definition with no prior declaration
    list(APPEND BPPRINT_TEST_CXX_FLAGS "-wd383")
    list(APPEND BPPRINT_TEST_CXX_FLAGS "-wd981")
    list(APPEND BPPRINT_TEST_CXX_FLAGS "-wd1418")
elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
    list(APPEND BPPRINT_TEST_CXX_FLAGS "-std=c++11")
    list(APPEND BPPRINT_TEST_CXX_FLAGS "-Wall;-Wextra;-pedantic")
elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    list(APPEND BPPRINT_TEST_CXX_FLAGS "-std=c++11")
    list(APPEND BPPRINT_TEST_CXX_FLAGS "-Wall;-Wextra;-pedantic")

    # for heavy-duty testing
    list(APPEND BPPRINT_TEST_CXX_FLAGS "-Weverything")
    list(APPEND BPPRINT_TEST_CXX_FLAGS "-Wno-c++98-compat")
    list(APPEND BPPRINT_TEST_CXX_FLAGS "-Wno-c++98-compat-pedantic")
    list(APPEND BPPRINT_TEST_CXX_FLAGS "-Wno-padded")
    list(APPEND BPPRINT_TEST_CXX_FLAGS "-Wno-format-nonliteral")
endif()


add_executable(test_bpprint test_bpprint.cpp)
target_include_directories(test_bpprint PRIVATE ${CMAKE_SOURCE_DIR})
target_compile_options(test_bpprint PRIVATE ${BPPRINT_TEST_CXX_FLAGS} ${BPPRINT_EXTRA_CXX_FLAGS})
target_link_libraries(test_bpprint PRIVATE bpprint)

add_test(NAME run_test_bpprint COMMAND test_bpprint)
