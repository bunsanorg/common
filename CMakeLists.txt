project(bunsan_common)

cmake_minimum_required(VERSION 2.8)

find_package(XMLRPC REQUIRED c++2 libwww-client abyss-server)

# BOOST
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
find_package(Boost COMPONENTS system filesystem serialization iostreams unit_test_framework program_options REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

message(AUTHOR_WARNING "We have to check pthread library on win* platform")
message(AUTHOR_WARNING "We have to provide support for different compilers")

add_definitions(-std=c++0x -pthread -g)

aux_source_directory(src srcs)

include_directories(include ${XMLRPC_INCLUDE_DIRS})

add_library(${PROJECT_NAME} SHARED ${srcs})
target_link_libraries(${PROJECT_NAME} zmq ${Boost_LIBRARIES} ${XMLRPC_LIBRARIES})

# install

install(DIRECTORY include DESTINATION .)
install(TARGETS ${PROJECT_NAME} DESTINATION lib)

# testing

enable_testing()
add_subdirectory(tests ${PROJECT_BINARY_DIR}/tests)

