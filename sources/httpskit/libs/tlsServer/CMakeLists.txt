add_library(${TLSSERVER_LIB} STATIC
    tlsserver.cpp
    tlsserver.h
    tlssession.h
    tlssession.cpp
    serializer.h
    serializer.cpp
)

FIND_PACKAGE (OpenSSL REQUIRED)
if (NOT OpenSSL-NOTFOUND)
    MESSAGE(STATUS "OpenSSL found")
endif()

target_link_libraries(${TLSSERVER_LIB}
    PUBLIC
    ${3RDPARTY_LIB}
    ${AGENTSMANAGER_LIB}
    OpenSSL::SSL
)

target_include_directories(${TLSSERVER_LIB} PUBLIC
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    clickhouse-cpp
    clickhouse-cpp/contrib/absl
    json
)
