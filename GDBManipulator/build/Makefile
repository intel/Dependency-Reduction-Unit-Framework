# Copyright (C) 2020 Intel Corporation
# SPDX-License-Identifier: AGPL-3.0-or-later
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
	make -C  gdbMan
	make -s -C gdbManTest

test :
	make -s -C gdbManTest
	./gdbMannTest/gdbManTest

clean ::
	make clean -C gdbMan
	make clean -C gdbManTest

run ::
	make -C gdbMan
	./gdbMann/gdbMan


