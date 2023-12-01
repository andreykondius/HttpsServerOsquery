# Данный файл настройки предназначен для включения в проект opensource-библиотек третьей стороны

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(clickhouse-cpp)

add_library(${3RDPARTY_LIB} INTERFACE
)

target_link_libraries(${3RDPARTY_LIB} INTERFACE
    clickhouse-cpp-lib
    ${CMAKE_CURRENT_SOURCE_DIR}/json/json11.cpp
)

target_include_directories(${3RDPARTY_LIB} INTERFACE
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    clickhouse-cpp
    clickhouse-cpp/contrib/absl
    json
)
