# Copyright (C) 2020 Intel Corporation
# SPDX-License-Identifier: Apache-2.0
#
#  CONTENT
#    simple Makefile to build all source for GdbManipulator and GdbManipulatorTest
#
#  AUTHOR
#    Sebastian Balz
#
#  DATE
#    2018-03-13
#
#  LANGUAGE
#    make
#


all::
	make -C gdbMann
	make -C gdbMannTest

test :
	make -C gdbMannTest
	./gdbMannTest/gdbMannTest

clean ::
	make clean -C gdbMann
	make clean -C gdbMannTest

run ::
	make -C gdbMann
	./gdbMann/gdbMann


