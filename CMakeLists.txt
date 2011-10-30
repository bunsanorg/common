project(bunsan_common)

cmake_minimum_required(VERSION 2.8)

set(gcc_flags -g -Wall -Wextra)

if (NOT DEFINED network)
	message(STATUS "Using network library part")
	set(network ON)
endif()

if (${network})
	find_package(XMLRPC REQUIRED c++2 libwww-client abyss-server)
	set(network_libraries ${XMLRPC_LIBRARIES} zmq)
	set(network_includes ${XMLRPC_INCLUDE_DIRS})
	if (${CMAKE_COMPILER_IS_GNUCC})
		add_definitions(-std=c++0x -pthread ${gcc_flags})
	endif()
else()
	if (${CMAKE_COMPILER_IS_GNUCC})
		add_definitions(-std=c++0x ${gcc_flags})
	endif()
endif()

# BOOST
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
find_package(Boost COMPONENTS system filesystem serialization iostreams unit_test_framework program_options REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

aux_source_directory(src srcs)
aux_source_directory(src/process process_srcs)

if (${network})
	aux_source_directory(src/network network_srcs)
endif()

include_directories(include ${network_includes})

add_library(${PROJECT_NAME} SHARED ${srcs} ${process_srcs} ${network_srcs})
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES} ${network_libraries})

# install

install(DIRECTORY include DESTINATION .)
install(TARGETS ${PROJECT_NAME} DESTINATION lib)

# testing

enable_testing()
add_subdirectory(tests ${PROJECT_BINARY_DIR}/tests)
if (${network})
	add_subdirectory(tests/network ${PROJECT_BINARY_DIR}/tests/network)
endif()

