# Copyright (C) 2020 Intel Corporation
# SPDX-License-Identifier: Apache-2.0
#
#  CONTENT
#    simple Makefile to build the tests for gdbMann
#
#  AUTHOR
#    Norbert Stoeffler
#
#  DATE
#    2018-03-10
#
#  LANGUAGE
#    make
#

APP =		gdbMannTest
APP_SRCS =	$(shell find \
			../../src/ \
			../../tests  \
			../../lib/SimpleLogging/src \
			../../lib/CPP-Argument-Parser/src\
			-name "*.cpp" ! -name "main.cpp")
APP_INCS =	../../lib/SimpleLogging/include \
			../../lib/CPP-Argument-Parser/include \
			../../src
			
APP_SLIB =	gtest \
			gtest_main \
			pthread 

include		../common/linux.mk

