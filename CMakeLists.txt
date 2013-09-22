project(bunsan_common)

include(${CMAKE_SOURCE_DIR}/system-config.cmake OPTIONAL)

cmake_minimum_required(VERSION 2.8)

include(BunsanCommon)

bunsan_use_boost(system filesystem serialization)

if(${UNIX})
    bunsan_use(dl)
elseif(${WIN32})
    if(${MSVC})
        bunsan_use(dbghelp)
    else()
        bunsan_use(psapi)
    endif()
endif()

bunsan_add_shared_library(${PROJECT_NAME}
    src/tempfile.cpp
    src/error.cpp
    src/categorized_error.cpp
    src/system_error.cpp

    src/runtime/demangle.cpp
    src/runtime/stacktrace.cpp

    src/filesystem/error.cpp
    src/filesystem/operations.cpp
)

bunsan_install_headers()
bunsan_install_targets(${PROJECT_NAME})

bunsan_include_tests()
