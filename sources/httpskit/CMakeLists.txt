set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")

add_subdirectory(apps)
add_subdirectory(libs)

foreach(file ${CMAKE_CURRENT_SOURCE_DIR}/resources/certificates)
    file(COPY ${file} DESTINATION ${PATH_BIN})
endforeach()
