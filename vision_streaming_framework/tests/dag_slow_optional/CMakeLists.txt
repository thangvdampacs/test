cmake_minimum_required(VERSION 3.10)

project(dag_slow_optional CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(dag_slow_optional
    main.cpp

    ../../src/dag/MergeNode.cpp
    ../../src/dag/Scheduler.cpp
    ../../src/dag/Graph.cpp
)

target_include_directories(dag_slow_optional PRIVATE
    ../../include
)
