# Testing of BPPrint

add_executable(test_bpprint test_bpprint.cpp)
target_include_directories(test_bpprint PRIVATE ${CMAKE_SOURCE_DIR})
target_link_libraries(test_bpprint PRIVATE bpprint)

add_test(NAME run_test_bpprint COMMAND test_bpprint)
