cmake_minimum_required(VERSION 3.29)
project(C_Implementation C)

set(CMAKE_C_STANDARD 23)

add_executable(C_Implementation
        graph.c
        dijkstra.c
        floyd_warshall_roy.c
        graph_generator.c
        experiment.c
        main.c
)

target_compile_options(C_Implementation PRIVATE -O2)