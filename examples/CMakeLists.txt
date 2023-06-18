
LIST(APPEND APPS
    rr
    simple
    msg
    ps
    regex
)

foreach(MAIN ${APPS})
    add_executable(${MAIN} ${MAIN}.cpp)
    target_link_libraries(${MAIN} PRIVATE marko)
endforeach()
