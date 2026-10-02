cmake_minimum_required(VERSION 4.3)
project(challenge1_sc C)

set(CMAKE_C_STANDARD 23)

add_executable(challenge1_sc main.c)

target_link_libraries(challenge1_sc curl cjson)

