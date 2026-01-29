cmake_minimum_required(VERSION 3.10)

project(pipeline_push_1000_frames CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(pipeline_push_1000_frames
    main.cpp

    ../../src/core/Buffer.cpp
    ../../src/core/Frame.cpp
    ../../src/core/PixelFormat.cpp

    ../../src/pipeline/StageThread.cpp
    ../../src/pipeline/Pipeline.cpp
)

target_include_directories(pipeline_push_1000_frames PRIVATE
    ../../include
)
