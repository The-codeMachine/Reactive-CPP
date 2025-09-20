cmake_minimum_required(VERSION 3.15)
project(noxmailCLI)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(main ${SOURCES})