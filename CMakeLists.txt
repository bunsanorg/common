cmake_minimum_required(VERSION 3.0)

include(${CMAKE_SOURCE_DIR}/system-config.cmake OPTIONAL)

project(bunsan_common)

find_package(BunsanCMake CONFIG REQUIRED)

bunsan_add_library(${PROJECT_NAME}
    src/tempfile.cpp
    src/error.cpp
    src/categorized_error.cpp
    src/system_error.cpp

    src/application/application.cpp
    src/application/global_registry.cpp

    src/runtime/demangle.cpp
    src/runtime/stacktrace.cpp

    src/filesystem/error.cpp
    src/filesystem/operations.cpp
)
bunsan_use_threads(${PROJECT_NAME})
bunsan_use_boost(${PROJECT_NAME}
    system
    filesystem
    log
    log_setup
    program_options
    serialization
    thread
)
if(${UNIX})
    target_link_libraries(${PROJECT_NAME} PRIVATE dl)
elseif(${WIN32})
    if(${MSVC})
        target_link_libraries(${PROJECT_NAME} PRIVATE dbghelp)
    else()
        target_link_libraries(${PROJECT_NAME} PRIVATE psapi)
    endif()
endif()

bunsan_install_headers()
bunsan_install_targets(${PROJECT_NAME})
bunsan_install_project()

bunsan_include_tests()
