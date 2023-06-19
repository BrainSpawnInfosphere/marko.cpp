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

add_executable(test_marko
    test_bc.cpp
    test_marko.cpp)

target_link_libraries(test_marko PRIVATE
    marko
    GTest::gtest_main
)
# target_link_libraries(${MAIN} PRIVATE marko)

include(GoogleTest)
gtest_discover_tests(test_marko)