project(bunsan_common)

include(${CMAKE_SOURCE_DIR}/system-config.cmake OPTIONAL)

cmake_minimum_required(VERSION 2.8)

# This project is common dependency.
# Other bunsan projects should assume that
# this module is installed system-wide.
include(${CMAKE_SOURCE_DIR}/BunsanCommon.cmake)

# BOOST
bunsan_use_boost(system filesystem serialization unit_test_framework thread)

# FIXME OS-specified (*nix)
bunsan_use(dl)

bunsan_add_shared_library(${PROJECT_NAME}
    src/tempfile.cpp
    src/error.cpp
    src/system_error.cpp

    src/runtime/demangle.cpp
    src/runtime/stacktrace.cpp

    src/filesystem/operations.cpp
)

bunsan_install_headers()
bunsan_install_targets(${PROJECT_NAME})
install(FILES BunsanCommon.cmake DESTINATION ${CMAKE_ROOT}/Modules)

bunsan_include_tests()
