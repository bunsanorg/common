# compiler
if(${CMAKE_COMPILER_IS_GNUCXX})
    set(gxx_flags "-rdynamic -std=c++11 -Wall -Wextra -Wno-multichar")
    if(${UNIX})
        set(gxx_flags "-pthread ${gxx_flags}")
    endif()
    set(CMAKE_CXX_FLAGS_DEBUG "-g ${gxx_flags}")
    set(CMAKE_CXX_FLAGS_RELEASE "-O2 ${gxx_flags}")
endif()

if(${CMAKE_COMPILER_IS_GNUCC})
    set(gcc_flags "-rdynamic -std=c11 -Wall -Wextra")
    if(${UNIX})
        set(gcc_flags "-pthread ${gcc_flags}")
    endif()
    set(CMAKE_C_FLAGS_DEBUG "-g ${gcc_flags}")
    set(CMAKE_C_FLAGS_RELEASE "-O2 ${gcc_flags}")
endif()

include(${CMAKE_SOURCE_DIR}/user-config.cmake OPTIONAL)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build, options are: Debug Release" FORCE)
endif()

if(NOT DEFINED ENABLE_TESTS)
    set(ENABLE_TESTS ON CACHE BOOL "Do you want to enable testing?" FORCE)
endif()

# binary targets
function(add_binary_targets bin_targets_)
    set(bin_targets)

    foreach(bin ${ARGN})
        set(trgt ${PROJECT_NAME}_${bin})
        list(APPEND bin_targets ${trgt})
        set(binprefix src/bin/${bin})
        if(EXISTS ${CMAKE_SOURCE_DIR}/${binprefix}.cpp)
            add_executable(${trgt} ${binprefix}.cpp)
        elseif(EXISTS ${CMAKE_SOURCE_DIR}/${binprefix}.c)
            add_executable(${trgt} ${binprefix}.c)
        else()
            message(SEND_ERROR "Source does not exist for target ${trgt}")
        endif()
        target_link_libraries(${trgt} ${PROJECT_NAME} ${libraries})
    endforeach()

    set(${bin_targets_} ${bin_targets} PARENT_SCOPE)
endfunction()

# testing
macro(bunsan_include_tests)
    if(${ENABLE_TESTS})
        enable_testing()
        add_subdirectory(tests ${CMAKE_CURRENT_BINARY_DIR}/tests)
    endif()
endmacro()

macro(bunsan_tests_project)
    project(${PROJECT_NAME}_tests)

    cmake_minimum_required(VERSION 2.8)

    message("tests were included")

    aux_source_directory(. test_srcs)

    add_definitions(-DBOOST_TEST_DYN_LINK)

    set(test_env
        "BUNSAN_SOURCE_DIR=${CMAKE_SOURCE_DIR}"
        "BUNSAN_BINARY_DIR=${CMAKE_BINARY_DIR}"
    )

    foreach(src ${test_srcs})
        string(REGEX REPLACE "^.*/([^/]+)\\.cpp$" "\\1" trgt ${src})
        add_executable(test_${trgt} ${src})
        target_link_libraries(test_${trgt} ${CMAKE_PROJECT_NAME} ${libraries})
        add_test(${trgt} test_${trgt})
        set_tests_properties(${trgt} PROPERTIES ENVIRONMENT "${test_env}")
    endforeach()

    if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/resources)
        add_subdirectory(resources ${CMAKE_CURRENT_BINARY_DIR}/resources)
    endif()
endmacro()

# installation
macro(bunsan_install_targets)
    install(TARGETS ${ARGN}
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)
endmacro()
