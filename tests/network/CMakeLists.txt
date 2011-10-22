project(bunsan_common_network_tests)

cmake_minimum_required(VERSION 2.8)

message("network tests was included")

aux_source_directory(. test_srcs)

foreach(src ${test_srcs})
	string(REGEX REPLACE "^.*/([^/]+)\\.cpp$" "\\1" trgt ${src})
	add_executable(${trgt} ${src})
	target_link_libraries(${trgt} bunsan_common)
	add_test(${trgt} ${trgt})
endforeach()

