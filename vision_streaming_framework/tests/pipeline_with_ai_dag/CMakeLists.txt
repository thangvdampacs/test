add_executable(pipeline_with_ai_dag
    main.cpp
)

target_link_libraries(pipeline_with_ai_dag
    vf_core
    vf_pipeline
    vf_dag
    vf_stages
)

