cmake_minimum_required(VERSION 3.16)
project(neditor VERSION 0.0.1)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pipe")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -rdynamic -fno-omit-frame-pointer -fno-optimize-sibling-calls -fno-inline")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

add_subdirectory(3rdparty)

file(GLOB_RECURSE SRCS neditor/*.cc)
add_executable(neditor ${SRCS})
target_link_libraries(neditor PUBLIC imgui nlohmann_json::nlohmann_json)
target_include_directories(neditor PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

