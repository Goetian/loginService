cmake_minimum_required(VERSION 3.13.4)

project(main)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(main -lpqxx -lpq)