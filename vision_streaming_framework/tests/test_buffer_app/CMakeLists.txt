cmake_minimum_required(VERSION 3.10)

project(test_buffer_app CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(test_buffer_app
    main.cpp
)

target_link_libraries(test_buffer_app
    vf_core
)
