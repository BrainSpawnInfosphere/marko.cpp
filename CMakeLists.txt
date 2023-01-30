cmake_minimum_required(VERSION 3.14.0)
project(marko VERSION 2022.7.1.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

cmake_host_system_information(RESULT HOST QUERY HOSTNAME)
cmake_host_system_information(RESULT OSN QUERY OS_NAME)
cmake_host_system_information(RESULT OS_VERSION QUERY OS_RELEASE)
cmake_host_system_information(RESULT PROC QUERY PROCESSOR_DESCRIPTION)

message(STATUS "-------------------------------------")
message(STATUS "  Project: ${PROJECT_NAME}")
message(STATUS "  Version: ${CMAKE_PROJECT_VERSION}")
message(STATUS "  C++ ${CMAKE_CXX_STANDARD}")
message(STATUS "-------------------------------------")
message(STATUS " ${HOST}")
message(STATUS " ${OSN}: ${OS_VERSION}")
message(STATUS " ${PROC}")
message(STATUS "-------------------------------------")


# Library =====================================================================
add_library(${PROJECT_NAME} SHARED $<TARGET_OBJECTS:objlib>)
add_library(${PROJECT_NAME}-static STATIC $<TARGET_OBJECTS:objlib>)
set_target_properties(${PROJECT_NAME}-static PROPERTIES OUTPUT_NAME ${PROJECT_NAME})

# if building STATIC library, an unimportant but annoying error appears about
# no symbols
SET(CMAKE_C_ARCHIVE_CREATE   "<CMAKE_AR> Scr <TARGET> <LINK_FLAGS> <OBJECTS>")
SET(CMAKE_CXX_ARCHIVE_CREATE "<CMAKE_AR> Scr <TARGET> <LINK_FLAGS> <OBJECTS>")
SET(CMAKE_C_ARCHIVE_FINISH   "<CMAKE_RANLIB> -no_warning_for_no_symbols -c <TARGET>")
SET(CMAKE_CXX_ARCHIVE_FINISH "<CMAKE_RANLIB> -no_warning_for_no_symbols -c <TARGET>")

message(STATUS ">> ${PROJECT_SOURCE_DIR}")

# makeing an object allows us to build both shared and static
# w/o having to build library twice ... I really don't make
# use of this here :)
add_library(objlib OBJECT
    src/ascii.cpp
    # src/bsocket.cpp
    # src/mcsocket.cpp
    src/udpsocket.cpp
    src/socket.cpp
    include/marko/udpsocket.hpp
)
target_include_directories(objlib
    PUBLIC
        # ${CMAKE_SOURCE_DIR}/include
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include> # marko.cpp/include
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include> # marko.cpp/include
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include> # marko.cpp/include

        $<INSTALL_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        # $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/include> # /Users/kevin/tmp/cpm/build
        # $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include> # /Users/kevin/tmp/cpm/build/_deps/marko-src
)
# target_precompile_headers(objlib
#   PUBLIC
#     include/marko/udpsocket.hpp
# )
# target_compile_features(objlib PUBLIC cxx_std_20 ) # better?
# set_target_properties(${PROJECT_NAME}-static
#     PROPERTIES STATIC_LIBRARY_FLAGS "-no_warning_for_no_symbols"
# )
# set_property(
#     TARGET ${PROJECT_NAME}-static
#     APPEND
#     PROPERTY STATIC_LIBRARY_FLAGS "-no_warning_for_no_symbols"
# )

# Add an alias so that library can be used inside the build tree, e.g. when testing
# add_library(Marko::marko ALIAS ${PROJECT_NAME})

# Examples ====================================================================
option(EXAMPLES "Examples" ON)
message(STATUS "Building examples is ${EXAMPLES}")
if (EXAMPLES)
    add_subdirectory(examples)
endif()

# Tests =======================
option(BUILD_TESTS "Tests" ON)
message(STATUS "Building tests is ${BUILD_TESTS}")
if (BUILD_TESTS)
    add_subdirectory(tests)
endif()

# Install =====================================================================
# set(MARKO_BASE_PATH "/opt/gecko")
# set(CMAKE_INSTALL_PREFIX ${MARKO_BASE_PATH})
# set(MARKO_BIN     "${CMAKE_INSTALL_PREFIX}/bin")
# set(MARKO_LIB     "${CMAKE_INSTALL_PREFIX}/lib")
# set(MARKO_CMAKE   "${CMAKE_INSTALL_PREFIX}/lib/cmake")
# set(MARKO_INCLUDE "${CMAKE_INSTALL_PREFIX}/include/marko")

# set(CMAKE_INSTALL_PREFIX "/opt/gecko")
# include("cmake/template.cmake")
