# Copyright (C) 2020 Intel Corporation
# SPDX-License-Identifier: AGPL-3.0-or-later
#
#  CONTENT
#    simple Makefile to build all source for GdbManipulator
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

APP =		gdbMann
APP_SRCS =	../../main.cpp $(shell find \
			../../src \
			../../lib/SimpleLogging/src \
			../../lib/CPP-Argument-Parser/src\
			-name "*.cpp")
APP_INCS =	../../lib/SimpleLogging/include \
			../../lib/CPP-Argument-Parser/include \
			../../src
		
APP_SLIB =	pthread

include		../common/linux.mk

