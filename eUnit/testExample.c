/**
 * Copyright (C) 2020 Intel Corporation
 * SPDX-License-Identifier: Apache-2.0
 * @author: Sebastian Balz
 */

#include "testSuite/testSuite.h"
#include "testSuite/asserts.h"

extern int add5(int a);
extern int return55();

void test_NEQ_SUCCESS(){
	addTestName("NEQ_SUCCESS",sizeof("NEQ_SUCCESS"));
  	  	uint8_t u1 = 1;
		uint32_t u2 = -1;
		ASSERT_NEQ(0,1);
		ASSERT_NEQ(1,0);
		ASSERT_NEQ(-1,0);
		ASSERT_NEQ(-1,1);
		ASSERT_NEQ(u1,u2);
}

void test_EQ_SUCCESS(){
	addTestName("EQ_SUCCESS",sizeof("EQ_SUCCESS"));
	  	uint8_t u1 = 1;
		uint32_t u2 = -1;
		uint32_t u3 = 1;
		ASSERT_EQ(0,0);
		ASSERT_EQ(1,1);
		ASSERT_EQ(-1,-1);
		ASSERT_EQ(u1,u1);
		ASSERT_EQ(u2,u2);
		ASSERT_EQ(u1,u3);
}

void test_NEQ_FAIL(){
	addTestName("NEQ_FAIL",sizeof("NEQ_FAIL"));
	uint8_t u1 = 1;
	uint32_t u2 = -1;
	uint32_t u3 = 1;
	
		ASSERT_NEQ(0,0);
		ASSERT_NEQ(1,1);
		ASSERT_NEQ(-1,-1);
		ASSERT_NEQ(u1,u1);
		ASSERT_NEQ(u2,u2);
		ASSERT_NEQ(u1,u3);
}

void test_callGroupe_EQ_SUCCESS(){

	addTestFileName(__FILE__,sizeof(__FILE__));

	addTestClass("EQ_SUCCESS",sizeof("EQ_SUCCESS"));
	test_NEQ_SUCCESS();
	test_EQ_SUCCESS();

}
int returnX(int i){
	return 55;
}
int abc(){
	return 42;
}
void test_callGroupe_NEQ_FAIL(){
	addTestFileName(__FILE__,sizeof(__FILE__));
	whenMock(add5,returnX);
	whenStub(abc,"100");

		int l = add5(5);
		int c = abc();
	if(abc() == 100){
		deleteAllMocks();
		if(l == 55 && abc() == 42)
			addTestClass("MockSucces",sizeof("MockSucces"));
		else
			addTestClass("MockFailed",sizeof("MockFailed"));
	}
	else
		addTestClass("MockFailed",sizeof("MockFailed"));
	test_NEQ_FAIL();
}

void invokeAllTest(){ 
	MessageToDebugger(startTesting);
		test_callGroupe_EQ_SUCCESS();
		test_callGroupe_NEQ_FAIL();
	MessageToDebugger(finishAllTest);
}

