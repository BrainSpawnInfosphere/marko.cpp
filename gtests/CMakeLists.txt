# GTest -----------------
include(FetchContent)
FetchContent_Declare(gtest
  GIT_REPOSITORY "https://github.com/google/googletest"
  GIT_TAG "origin/main"
  SOURCE_DIR "${CMAKE_BINARY_DIR}/_deps/gtest"
)
# set(EXAMPLES OFF CACHE INTERNAL "Dont build examples")
FetchContent_MakeAvailable(gtest)
if(gtest_POPULATED)
    message(STATUS "=> Found gtest")
else()
    message(STATUS "*** Didn't find gtest")
endif()

enable_testing()

add_executable(
  test_marko
  test_marko.cpp
)
target_link_libraries(
    test_marko
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(test_marko)