include(GoogleTest)

# find_package(benchmark REQUIRED)
file(GLOB_RECURSE HEADERS "${PROJECT_SOURCE_DIR}/src/include/*.h")
file(GLOB_RECURSE TEST_SOURCES "${PROJECT_SOURCE_DIR}/test/*.cpp")

foreach(test_source ${TEST_SOURCES})
    get_filename_component(test_filename ${test_source} NAME)
    string(REPLACE ".cpp" "" test_name ${test_filename})

    add_executable(${test_name} ${test_source})

    # target_link_libraries(bench GTest::gtest GTest::gtest_main benchmark::benchmark)
    # gtest_discover_tests(${test_name})
endforeach(test_source)