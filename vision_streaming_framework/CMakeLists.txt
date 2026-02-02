cmake_minimum_required(VERSION 3.10)
project(vision_framework CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ===== include path =====
include_directories(
    ${PROJECT_SOURCE_DIR}/include
)

# ===== core library =====
add_library(vf_core
    src/core/Buffer.cpp
    src/core/Frame.cpp
    src/core/PixelFormat.cpp
)

# ===== pipeline library =====
add_library(vf_pipeline
    src/pipeline/StageThread.cpp
    src/pipeline/Pipeline.cpp
)

# ===== dag library =====
add_library(vf_dag
    src/dag/DagResult.cpp
    src/dag/MergeNode.cpp
    src/dag/Scheduler.cpp
    src/dag/Graph.cpp
)

# ===== stages library =====
add_library(vf_stages
    src/stages/AIStage.cpp
)

target_link_libraries(vf_stages
    vf_core
    vf_pipeline
    vf_dag
)

# ===== examples =====
add_subdirectory(tests)

