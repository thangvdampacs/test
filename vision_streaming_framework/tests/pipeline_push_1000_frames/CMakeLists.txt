cmake_minimum_required(VERSION 3.10)

project(pipeline_push_1000_frames CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(pipeline_push_1000_frames
    main.cpp
)

target_link_libraries(pipeline_push_1000_frames
    vf_core
    vf_pipeline
    vf_dag
)

