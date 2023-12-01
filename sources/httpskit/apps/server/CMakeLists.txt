set(SRC
    main.cpp
)

add_executable(server
    ${SRC}
)

target_link_libraries(server
    PUBLIC
    ${3RDPARTY_LIB}
    ${HTTPSKIT_LIB}
)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE})

