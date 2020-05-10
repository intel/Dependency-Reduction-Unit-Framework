/**
 * Copyright (C) 2020 Intel Corporation
 * SPDX-License-Identifier: Apache-2.0
 * @author: Sebastian Balz
 */

#include <stdint.h>
extern uint8_t evaluateGPIO();
extern uint8_t funktionWithDep();
extern uint8_t error_code;

#include "eUnit.h"

TEST(evaluateGPIO,HW0){
	whenStub(funktionWithDep," 0");
	ASSERT_EQ(0,evaluateGPIO());
	deleteAllMocks();
}

TEST(evaluateGPIO,HW1){
	whenStub(funktionWithDep," 1");
	ASSERT_EQ(1,evaluateGPIO());
}

TEST(evaluateGPIO,HW254){
	whenStub(funktionWithDep," 254");
	ASSERT_EQ(1,evaluateGPIO());
	deleteAllMocks();
}

TEST(evaluateGPIO,HW255){
	error_code = 0;
	whenStub(funktionWithDep," 255");
	ASSERT_EQ((uint8_t)-1,evaluateGPIO());
	deleteAllMocks();
}

uint8_t mockFunktion(){
	error_code = 1;
	return -1;
}
TEST(evaluateGPIO,SetErrorCodeTo1){
	whenMock(funktionWithDep,mockFunktion);
	ASSERT_EQ((uint8_t)-2,evaluateGPIO());
	deleteAllMocks();
}

TEST(testThatNeedsquitLong,longTest){
	printOverGdb("this Test need some time to execute. It is possibel the define a timeout by using gdbMann with <--timeout <ms>\n ");
	funktionWithDep();
	ASSERT_TRUE(1);
}

DISABLED_TEST(DISABLED_Test,test1){
	printOverGdb("it is possibel to disable generatet test by setting the group prefex to DISABLED");
	ASSERT_TRUE(0);
}
