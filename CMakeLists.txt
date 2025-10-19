cmake_minimum_required(VERSION 3.13)
project(FurcateTEE C)

find_package(OpenEnclave CONFIG REQUIRED)

add_subdirectory(enclave)
add_subdirectory(host)

# Testing
enable_testing()
add_subdirectory(tests)
