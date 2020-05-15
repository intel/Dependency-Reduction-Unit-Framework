# Copyright (C) 2020 Intel Corporation
# SPDX-License-Identifier: AGPL-3.0-or-later
#
#  CONTENT
#    simple Makefile to build all source for GdbManipulator
#
#  AUTHOR
#    Sebastian Balz  based on Norbert Stoeffler version
#
#  DATE
#    2018-07-28
#
#  LANGUAGE
#    make
#
CFLAGS += -g 
CXXFLAGS +=	$(addprefix -I,$(APP_INCS)) 
_APP_OBJS1 = 	$(patsubst %.c,%.o,$(APP_SRCS))
_APP_OBJS = 	$(notdir $(_APP_OBJS1))
VPATH =			$(sort $(dir $(_APP_OBJS1)))

all:: $(APP)

clean::
	rm -rf build/*
	

run:	all
	./build/$(APP)
	
test:	all
	./build/$(APP)
	
	
$(APP): $(_APP_OBJS)
	mkdir -p build 
	gcc -w -O0  -o build/$@ $^ ${CFLAGS} $(addprefix -l,$(APP_SLIB)) -I "../ ../.."
	mv *.o build/

debug::
	@echo APP_SRCS = $(APP_SRCS)
	@echo _APP_OBJS = $(_APP_OBJS)
	@echo VPATH=$(VPATH)
