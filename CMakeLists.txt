cmake_minimum_required(VERSION 3.6)
project(Game_Of_Life_2_1)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "-lglut -lGLU -lGL -lm")

set(SOURCE_FILES main.cpp)
add_executable(Game_Of_Life_2_1 ${SOURCE_FILES})