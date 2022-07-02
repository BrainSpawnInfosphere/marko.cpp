
# add_executable(request request.cpp)
# target_link_libraries(request marko)
#
# add_executable(reply reply.cpp)
# target_link_libraries(reply marko)
#
# add_executable(rr rr.cpp)
# target_link_libraries(rr marko)
# target_include_directories(rr PUBLIC ${CMAKE_SOURCE_DIR}/include)
#
# add_executable(ps ps.cpp)
# target_link_libraries(ps marko)

LIST(APPEND APPS
    rr
    ps
)

foreach(MAIN ${APPS})
    add_executable(${MAIN} ${MAIN}.cpp)
    target_link_libraries(${MAIN} PRIVATE marko)
    target_include_directories(${MAIN} PRIVATE ${CMAKE_SOURCE_DIR}/include)
endforeach()
