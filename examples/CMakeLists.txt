
LIST(APPEND APPS
    # rr
    simple
    msg
    ps
)

foreach(MAIN ${APPS})
    add_executable(${MAIN} ${MAIN}.cpp)
    target_link_libraries(${MAIN} PRIVATE marko)
endforeach()
