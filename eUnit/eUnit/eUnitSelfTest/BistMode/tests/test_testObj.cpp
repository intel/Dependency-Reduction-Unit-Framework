/**
 * Copyright (C) 2020 Intel Corporation
 * SPDX-License-Identifier: AGPL-3.0-or-later
 * @author: Sebastian Balz
 */
#include "../testObjekt.h"
#include "../../../eUnit.h"

TEST(reduceDependencys,not_Possible_In_HostObly_Mode__PleaseUseTheGDBMAN_instead){
	whenStub(funktionWithDep,"1");
	ASSERT_EQ(evaluateGPIO(),1);
}


