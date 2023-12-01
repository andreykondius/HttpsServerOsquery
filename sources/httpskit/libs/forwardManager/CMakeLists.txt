add_library(${FORWARDMANAGER_LIB}
    forwardmanager.cpp
    forwardmanager.h
)

target_link_libraries(${FORWARDMANAGER_LIB}
    PUBLIC
    ${3RDPARTY_LIB}
    ${TLSSERVER_LIB}
)

