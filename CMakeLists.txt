cmake_minimum_required(VERSION 3.10)
project(El-Farol-cpp)

file(GLOB SourceFiles "src/main.cxx" "src/sources/*.cxx")
add_executable(El-Farol-cpp ${SourceFiles})

include_directories("src/headers")