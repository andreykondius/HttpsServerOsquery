set(FORWARDMANAGER_LIB forwardmanager_lib)
set(TLSSERVER_LIB tlsserver_lib)
set(AGENTSMANAGER_LIB agetsmanager_lib)

add_subdirectory(tlsServer)
add_subdirectory(agentsManager)
add_subdirectory(forwardManager)

add_library(${HTTPSKIT_LIB} INTERFACE)

target_link_libraries(${HTTPSKIT_LIB} INTERFACE
   ${FORWARDMANAGER_LIB}
)

target_include_directories(${3RDPARTY_LIB} INTERFACE
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    agentsManager
    forwardManager
    tlsServer
)


