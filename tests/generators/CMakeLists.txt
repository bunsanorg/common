project(${PROJECT_NAME}_generators)

bunsan_use_bunsan(common)
bunsan_use_boost(system filesystem)
bunsan_add_executable(test_generator_stream_enum stream_enum.cpp)
