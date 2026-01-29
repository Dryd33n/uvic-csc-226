cmake_minimum_required(VERSION 3.29)
project(C_implementation C)

set(CMAKE_C_STANDARD 11)

add_executable(C_implementation main.c
        linear_probing.c
        chaining.c
        batch_runner.c
)
