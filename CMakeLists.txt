cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 11)

project(meme)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/thirdparty/SQLiteCpp)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/thirdparty/cxxopts)

include_directories(include)

include_directories(
    ${CMAKE_CURRENT_LIST_DIR}/thirdparty/SQLiteCpp/include
)

file(GLOB SOURCES "src/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME}
    SQLiteCpp
    cxxopts
    sqlite3
    pthread
    dl
)