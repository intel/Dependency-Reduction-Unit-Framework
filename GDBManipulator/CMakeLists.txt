# Copyright (C) 2020 Intel Corporation
# SPDX-License-Identifier: Apache-2.0
cmake_minimum_required(VERSION 3.2)
project(gdbMan)
set(CMAKE_CXX_STANDARD 11)

add_subdirectory(src)
# include libs
## CPP-Argument-Parser
add_subdirectory(lib/CPP-Argument-Parser)
include_directories(lib/CPP-Argument-Parser/include)
## Simplelogging
add_subdirectory(lib/SimpleLogging)
include_directories(lib/SimpleLogging/include)

include_directories(src)

message(${src_files})
add_executable(gdbMann ${src_files}  src/main.cpp src/main.h )

#link libs
target_link_libraries(gdbMann pthread SimpleLoggingLib CPPArgvParsLib)


find_package(GTest)
if (GTest_FOUND)
    message(" -- gtest found at " ${GTEST_INCLUDE_DIRS})
    include_directories(${GTEST_INCLUDE_DIRS})
    add_subdirectory(tests)
    add_executable(gdbMannSelfTest ${src_files} ${test_files})
    target_link_libraries(gdbMannSelfTest ${GTEST_LIBRARIES} pthread CPPArgvParsLib SimpleLoggingLib)

else ()
    message(WARNING "to enable self test install gtest")
endif ()


