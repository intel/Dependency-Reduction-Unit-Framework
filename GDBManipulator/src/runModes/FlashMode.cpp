/**
 * Copyright (C) 2020 Intel Corporation
 * SPDX-License-Identifier: AGPL-3.0-or-later
 * @author: Sebastian Balz
 */

#include "FlashMode.h"

bool flashMode::executeMode() {
	FlashSuccess =  com->initSystem() & com->executeCommand("-exec-continue");
	return FlashSuccess;
}

string flashMode::analyseResult() {
	string out;
	if(FlashSuccess)
		out = "Flash done";
	else
		out = "Flash failed";
	return out;
}
