project(bunsan_common)

include(${CMAKE_SOURCE_DIR}/system-config.cmake OPTIONAL)

cmake_minimum_required(VERSION 2.8)

# This project is common dependency.
# Other bunsan projects should assume that
# this module is installed system-wide.
include(${CMAKE_SOURCE_DIR}/BunsanCommon.cmake)

include_directories(include)

# BOOST
bunsan_use_boost(system filesystem serialization unit_test_framework thread)

add_library(${PROJECT_NAME} SHARED
    src/tempfile.cpp
    src/error.cpp
    src/system_error.cpp
    src/util.cpp

    src/process/error.cpp
    src/process/execute.cpp
    src/process/aliases.cpp

    src/interprocess/sync/file_lock.cpp

    src/filesystem/operations.cpp
)
target_link_libraries(${PROJECT_NAME} ${libraries})

bunsan_install_targets(${PROJECT_NAME})
install(FILES BunsanCommon.cmake DESTINATION ${CMAKE_ROOT}/Modules)

bunsan_include_tests()
