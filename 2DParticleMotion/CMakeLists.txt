cmake_minimum_required(VERSION 3.16)
project(ReturnToZeroSimulation)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Core)

add_executable(return_to_zero
    main.cpp
    simulation.cpp
    simulation.hpp
)

target_link_libraries(return_to_zero
    Qt6::Core
)

# Копируем config.json в папку сборки (если есть)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.json 
               ${CMAKE_CURRENT_BINARY_DIR}/config.json COPYONLY)