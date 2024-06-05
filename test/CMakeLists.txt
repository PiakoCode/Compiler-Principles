include(GoogleTest)


file(GLOB_RECURSE HEADERS "${PROJECT_SOURCE_DIR}/include/*.h" "${PROJECT_SOURCE_DIR}/include/*.hpp" "${PROJECT_SOURCE_DIR}/include/*.cpp")

file(GLOB_RECURSE TEST_SOURCES "${PROJECT_SOURCE_DIR}/test/*.cpp")

foreach(test_source ${TEST_SOURCES})
    get_filename_component(test_filename ${test_source} NAME)
    string(REPLACE ".cpp" "" test_name ${test_filename})
    message(${test_filename})
    message(${test_name})
    add_executable(${test_name} ${test_source} ${HEADERS})

    # target_link_libraries(bench GTest::gtest GTest::gtest_main benchmark::benchmark)
    # gtest_discover_tests(${test_name})
endforeach(test_source)