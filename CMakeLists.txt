project(bunsan_common)

cmake_minimum_required(VERSION 2.8)

include(${CMAKE_SOURCE_DIR}/user-config.cmake OPTIONAL)

if(NOT DEFINED network)
	message(STATUS "Using network library part")
	set(network ON)
endif()

if(${network})
	find_package(XMLRPC REQUIRED c++2 libwww-client abyss-server)
	list(APPEND includes ${XMLRPC_INCLUDE_DIRS})
	list(APPEND libraries ${XMLRPC_LIBRARIES} zmq)
endif()

# BOOST
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
find_package(Boost COMPONENTS system filesystem serialization iostreams unit_test_framework program_options REQUIRED)
list(APPEND includes ${Boost_INCLUDE_DIRS})
list(APPEND libraries ${Boost_LIBRARIES})

if(${CMAKE_COMPILER_IS_GNUCC})
	set(gcc_flags -g -Wall -Wextra)
	if(${MINGW})
		# MinGW has no pthread support
		add_definitions(-std=c++0x ${gcc_flags})
	else()
		add_definitions(-std=c++0x -pthread ${gcc_flags})
	endif()
endif()

aux_source_directory(src srcs)

aux_source_directory(src/process process_srcs)
list(APPEND srcs ${process_srcs})

if(${network})
	aux_source_directory(src/network network_srcs)
	list(APPEND srcs ${network_srcs})
endif()

include_directories(include ${includes})

add_library(${PROJECT_NAME} SHARED ${srcs})
target_link_libraries(${PROJECT_NAME} ${libraries})

# install

install(DIRECTORY include DESTINATION .)
install(TARGETS ${PROJECT_NAME}
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib)

# testing

enable_testing()
add_subdirectory(tests ${PROJECT_BINARY_DIR}/tests)
if(${network})
	add_subdirectory(tests/network ${PROJECT_BINARY_DIR}/tests/network)
endif()

