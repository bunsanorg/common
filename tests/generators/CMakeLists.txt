project(${PROJECT_NAME}_generators)

bunsan_add_executable(test_generator_stream_enum stream_enum.cpp)
target_link_libraries(test_generator_stream_enum PRIVATE bunsan_common)
bunsan_targets_finish_setup(test_generator_stream_enum)
