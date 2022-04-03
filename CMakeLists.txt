cmake_minimum_required(VERSION 3.0.0)
project(rougetcod VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)

add_executable(rougetcod src/main.cpp src/Actor.cpp src/Engine.cpp src/Map.cpp src/bsp.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

target_include_directories(rougetcod PRIVATE external/libtcod/include)

target_link_libraries(rougetcod C:/Dev/rougetcod/external/libtcod/lib/libtcod.lib)
