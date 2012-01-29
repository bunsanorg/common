project(bunsan_common)

cmake_minimum_required(VERSION 2.8)

if(${CMAKE_COMPILER_IS_GNUCXX})
	set(gxx_flags "-std=c++0x -Wall -Wextra")
	if(${UNIX})
		set(gxx_flags "-pthread ${gxx_flags}")
	endif()
	set(CMAKE_CXX_FLAGS_DEBUG "-g ${gxx_flags}")
	set(CMAKE_CXX_FLAGS_RELEASE "-O2 ${gxx_flags}")
endif()

include(${CMAKE_SOURCE_DIR}/user-config.cmake OPTIONAL)

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build, options are: Debug Release" FORCE)
endif()

if(NOT DEFINED ENABLE_TESTS)
	set(ENABLE_TESTS ON CACHE BOOL "Do you want to enable testing?" FORCE)
endif()

if(NOT DEFINED ENABLE_NETWORK)
	set(ENABLE_NETWORK ON CACHE BOOL "Do you want to build network library part? This option requires xmlrpc-c and zmq libraries." FORCE)
endif()

if(${ENABLE_NETWORK})
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

aux_source_directory(src srcs)

aux_source_directory(src/process process_srcs)
list(APPEND srcs ${process_srcs})

if(${ENABLE_NETWORK})
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

if(${ENABLE_TESTS})
	enable_testing()
	add_subdirectory(tests ${PROJECT_BINARY_DIR}/tests)
	if(${ENABLE_NETWORK})
		add_subdirectory(tests/network ${PROJECT_BINARY_DIR}/tests/network)
	endif()
endif()

