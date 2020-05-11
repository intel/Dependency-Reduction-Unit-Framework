# Copyright (C) 2020 Intel Corporation
# SPDX-License-Identifier: AGPL-3.0-or-later
#
#  CONTENT
#    common make rules for linux
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

CXXFLAGS +=		$(addprefix -I,$(APP_INCS)) -std=c++11 -g3 -Og
_APP_OBJS1 = 	$(patsubst %.cc,%.o,$(patsubst %.cpp,%.o,$(APP_SRCS)))
_APP_OBJS = 	$(notdir $(_APP_OBJS1))
VPATH =			$(sort $(dir $(_APP_OBJS1)))

all:: $(APP)

clean::
	rm -f $(APP) $(_APP_OBJS)
	rm -f *.bash
	rm -f *.log

run:	all
	./$(APP)

$(APP): $(_APP_OBJS)
	g++ -g3 -o $@ $^ $(addprefix -l,$(APP_SLIB))

debug::
	@echo APP_SRCS = $(APP_SRCS)
	@echo _APP_OBJS = $(_APP_OBJS)
	@echo VPATH=$(VPATH)
