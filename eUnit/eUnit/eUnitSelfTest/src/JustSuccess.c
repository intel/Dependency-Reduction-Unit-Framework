/**
 * Copyright (C) 2020 Intel Corporation
 * SPDX-License-Identifier: Apache-2.0
 * @author: Sebastian Balz
 */
#include "main.h"


extern volatile amountOfInfo test_amount_of_stored_data;

extern volatile uint32_t test_result_index ;
extern volatile amountOfInfo test_amount_of_stored_data;
extern uint8_t test_result_buffer[SIZE_OF_RESULT_BUFFER];

void cleanUP(){
	MessageToDebugger(bufferIsFull);
}

void JustSuccess1(){
	cleanUP();
	test_amount_of_stored_data = amountOf_justSuccess;
	CompareInt(0,0,4,0,true,1);
	testCaseSuccess(1);
	CU_ASSERT_EQUAL(test_result_buffer[0],0b1);
}
void JustSuccess2(){
	cleanUP();
	test_amount_of_stored_data = amountOf_justSuccess;
	CompareInt(0,0,4,0,true,1);
	testCaseSuccess(1);
	CompareInt(1,1,4,0,true,1);
	testCaseSuccess(1);

	CU_ASSERT_EQUAL(test_result_buffer[0],0b101);
}
void JustSuccess3(){
	cleanUP();
	test_amount_of_stored_data = amountOf_justSuccess;
	CompareInt(0,0,4,0,true,1);
	CompareInt(1,1,4,0,true,2);
	CompareInt(2,2,4,0,true,3);
	CompareInt(3,3,4,0,true,4);
	CompareInt(4,4,4,0,true,5);
	testCaseSuccess(1);
	testCaseSuccess(1);
	testCaseSuccess(1);
	testCaseSuccess(1);
	testCaseSuccess(1);
	CU_ASSERT_EQUAL(test_result_buffer[0],0b1010101);
	CU_ASSERT_EQUAL(test_result_buffer[1],0b1);
}

void JustSuccess4(){
	cleanUP();
	test_amount_of_stored_data = amountOf_justSuccess;
	CompareInt(0,1,4,0,true,1);
	CompareInt(1,1,4,0,true,2);
	testCaseSuccess(1);
	testCaseSuccess(1);
	testCaseSuccess(1);

	CompareInt(2,2,4,0,true,3);
	CompareInt(3,3,4,0,true,4);
	CompareInt(4,4,4,0,true,5);
	testCaseSuccess(1);

	CU_ASSERT_EQUAL(test_result_buffer[0],0b1010110);
	CU_ASSERT_EQUAL(test_result_buffer[1],0b1);
}

void JustSuccess5(){
	cleanUP();
	test_amount_of_stored_data = amountOf_justSuccess;
	CompareInt(1,1,4,0,true,1);
	CompareInt(1,1,4,0,true,2);
	CompareInt(2,2,4,0,true,3);
	CompareInt(3,3,4,0,true,4);
	testCaseSuccess(1);
	testCaseSuccess(1);
	testCaseSuccess(1);
	testCaseSuccess(1);

	CompareInt(4,3,4,0,true,5);
	CU_ASSERT_EQUAL(test_result_buffer[0],0b1010101);
	CU_ASSERT_EQUAL(test_result_buffer[1],0b10);
}

void JustSuccessAddName(){
	cleanUP();
	addTestName("abc",3);
	addTestClass("def",3);
	test_amount_of_stored_data = amountOf_justSuccess;
	CompareInt(1,1,4,0,true,1);
	CompareInt(1,1,4,0,true,2);
	CompareInt(2,2,4,0,true,3);
	CompareInt(3,3,4,0,true,4);
	testCaseSuccess(1);
	testCaseSuccess(1);
	testCaseSuccess(1);
	testCaseSuccess(1);
	CompareInt(4,3,4,0,true,5);
	CU_ASSERT_EQUAL(test_result_buffer[0],0b1010101);
	CU_ASSERT_EQUAL(test_result_buffer[1],0b10);
}


void byteCompare1(){
	cleanUP();
	test_amount_of_stored_data = amountOf_justSuccess;
	int abc[] = {1,2,3};
	int def[] = {1,2,3};
	testCaseSuccess(1);

	byteComp(abc,12,def,12,1);
	CU_ASSERT_EQUAL(test_result_buffer[0],0b01);
}

void byteCompare2(){
	cleanUP();
	test_amount_of_stored_data = amountOf_justSuccess;
	int abc[] = {1,2,3};
	int def[] = {1,2,4};
	byteComp(abc,12,def,12,1);
	CU_ASSERT_EQUAL(test_result_buffer[0],0b10);
}

void byteCompare3(){
	cleanUP();
	test_amount_of_stored_data = amountOf_justSuccess;
	int abc[] = {1,2,3};
	int def[] = {1,2,4};
	byteComp(abc,12,abc,12,1);
	testCaseSuccess(1);
	byteComp(abc,12,def,12,1);
	CU_ASSERT_EQUAL(test_result_buffer[0],0b1001);
}



int initCompInt(){
	return 0;
}

CU_pSuite Group_JustSuccess(CU_pSuite pSuite){
	pSuite = CU_add_suite("JustSuccess",initCompInt,initCompInt);
   if (NULL == pSuite) {
	  CU_cleanup_registry();
	  exit( CU_get_error());
   }

   if ((NULL == CU_add_test(pSuite, "JustSuccess1", JustSuccess1) )
   		   ||(NULL == CU_add_test(pSuite, "JustSuccess2", JustSuccess2) )
		   ||(NULL == CU_add_test(pSuite, "JustSuccess3", JustSuccess3) )
		   ||(NULL == CU_add_test(pSuite, "JustSuccess4", JustSuccess4) )
		   ||(NULL == CU_add_test(pSuite, "JustSuccess5", JustSuccess5) )
		   ||(NULL == CU_add_test(pSuite, "JustSuccessAddName", JustSuccessAddName) )
		   ||(NULL == CU_add_test(pSuite, "byteCompare1", byteCompare1) )
		   ||(NULL == CU_add_test(pSuite, "byteCompare2", byteCompare2) )
		   ||(NULL == CU_add_test(pSuite, "byteCompare3", byteCompare3) )

      )
	  {
		 CU_cleanup_registry();
		 exit(CU_get_error());
	  }
   return pSuite;
}
