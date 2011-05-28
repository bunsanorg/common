project(bunsan_common)

cmake_minimum_required(VERSION 2.8)

# BOOST
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
find_package (Boost COMPONENTS system filesystem serialization iostreams unit_test_framework program_options REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
#set(Boost_LIBRARIES boost_system boost_filesystem boost_serialization boost_iostreams boost_unit_test_framework boost_program_options)
#include_directories(/usr/local/include)
#link_directories(/usr/local/lib)
message("We have to check pthread library on win* platform")
#find_package (Threads)
#target_link_libraries (${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT} ${Boost_LIBRARIES})
#target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES} pthread)

add_definitions(-std=c++0x -pthread -g)

aux_source_directory(. srcs)

include_directories(.)

# platform_defined
if (UNIX)
	message(STATUS "unix found, using impl/unix")
	aux_source_directory(impl/unix impl_srcs)
elseif(APPLE)
	message(STATUS "apple found, using impl/apple")
	aux_source_directory(impl/apple impl_srcs)
elseif(WIN32)
	message(STATUS "win32 found, using impl/win32")
	aux_source_directory(impl/win32 impl_srcs)
else()
	message(SEND_ERROR "Unknown platform")
endif()

add_library(${PROJECT_NAME} SHARED ${srcs} ${impl_srcs})

