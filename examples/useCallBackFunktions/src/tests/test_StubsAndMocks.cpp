/**
 * Copyright (C) 2020 Intel Corporation
 * SPDX-License-Identifier: AGPL-3.0-or-later
 * @author: Sebastian Balz
 */
#include "toTest/testObjekt.h"
#include "eUnit.h"

uint8_t newFunktionWithoutDep(){
	printOverGdb("called from Mock");
	return 2;
}
TEST(reduceDependencys,UseStubs){
	// return direct a 1 when funktionWithDep gets called
	whenStub(funktionWithDep,"1");
	ASSERT_EQ(evaluateGPIO(),1);
}

TEST(reduceDependencys,UseMocks){
	// execute newFunktionWithoutDep instead of funktionWithDep
	whenMock(funktionWithDep,newFunktionWithoutDep);
	ASSERT_EQ(evaluateGPIO(),1);
}

