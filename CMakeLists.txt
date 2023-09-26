cmake_minimum_required(VERSION 3.23)

project(kode LANGUAGES CXX)


set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.2 COMPONENTS Quick REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Multimedia Gui Test Widgets)
find_package(Qt6 REQUIRED COMPONENTS Core)


add_subdirectory(utils)
add_subdirectory(ui)
