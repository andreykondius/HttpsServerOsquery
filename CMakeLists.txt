cmake_minimum_required(VERSION 3.5)

project(httpskit VERSION 1.0 LANGUAGES CXX)

SET(PATH_BIN ${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE})
set(3RDPARTY_LIB 3rdparty_lib)
set(HTTPSKIT_LIB httpskit_lib)
add_subdirectory(sources)

