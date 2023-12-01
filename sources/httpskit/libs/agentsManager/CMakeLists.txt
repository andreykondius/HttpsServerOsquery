add_library(${AGENTSMANAGER_LIB} STATIC
    agentsmanager.cpp
    agentsmanager.h
)

target_link_libraries(${AGENTSMANAGER_LIB}
    PUBLIC
    ${3RDPARTY_LIB}
    ${TLSSERVER_LIB}
)

