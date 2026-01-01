cmake_minimum_required(VERSION 3.10)
project(OrionCore)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED)

add_executable(orion_core main.cpp)
target_link_libraries(orion_core PRIVATE ${OpenCV_LIBS})
