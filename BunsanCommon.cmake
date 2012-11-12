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

if(EXISTS ${CMAKE_SOURCE_DIR}/include)
    include_directories(include)
endif()

# libraries
macro(bunsan_use)
    list(APPEND libraries ${ARGN})
endmacro()

macro(bunsan_use_boost)
    set(Boost_USE_STATIC_LIBS OFF)
    set(Boost_USE_MULTITHREADED ON)
    find_package(Boost COMPONENTS ${ARGN} REQUIRED)
    include_directories(${Boost_INCLUDE_DIRS})
    bunsan_use(${Boost_LIBRARIES})
endmacro()

macro(bunsan_use_xmlrpc)
    find_package(XMLRPC REQUIRED ${ARGN})
    include_directories(${XMLRPC_INCLUDE_DIRS})
    bunsan_use(${XMLRPC_LIBRARIES})
endmacro()

macro(bunsan_use_bunsan)
    foreach(lib ${ARGN})
        bunsan_use(bunsan_${lib})
    endforeach()
endmacro()

# binary targets
function(bunsan_add_executable target)
    add_executable(${target} ${ARGN})
    target_link_libraries(${target} ${libraries})
endfunction()

function(bunsan_add_shared_library target)
    add_library(${target} SHARED ${ARGN})
    target_link_libraries(${target} ${libraries})
endfunction()

function(bunsan_add_cli_targets targets_)
    set(cli_targets)

    foreach(cli ${ARGN})
        set(trgt ${PROJECT_NAME}_${cli})
        list(APPEND cli_targets ${trgt})
        set(cliprefix src/bin/${cli})
        file(GLOB srcs src/bin/${cli}.cpp)
        bunsan_add_executable(${trgt} ${srcs})
    endforeach()

    set(${cli_targets_} ${cli_targets} PARENT_SCOPE)
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

macro(bunsan_install_headers)
    install(DIRECTORY include DESTINATION .)
endmacro()
