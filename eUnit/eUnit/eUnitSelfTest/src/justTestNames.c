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
extern void cleanUP();

void TN_JustSuccess1(){
	cleanUP();
	test_amount_of_stored_data = amountOf_testNames;
	CompareInt(0,0,4,0,true,1);
	testCaseSuccess(1);
	CU_ASSERT_EQUAL(test_result_buffer[0],0b1);
}
void TN_JustSuccess2(){
	cleanUP();
	test_amount_of_stored_data = amountOf_testNames;
	CompareInt(0,0,4,0,true,1);
	CompareInt(1,1,4,0,true,1);
	testCaseSuccess(1);
	testCaseSuccess(1);
	CU_ASSERT_EQUAL(test_result_buffer[0],0b101);
}
void TN_JustSuccess3(){
	cleanUP();
	test_amount_of_stored_data = amountOf_testNames;
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

void TN_JustSuccess4(){
	cleanUP();
	test_amount_of_stored_data = amountOf_testNames;
	CompareInt(0,1,4,0,true,1);
	CompareInt(1,1,4,0,true,2);
	CompareInt(2,2,4,0,true,3);
	CompareInt(3,3,4,0,true,4);
	CompareInt(4,4,4,0,true,5);
	testCaseSuccess(1);
	testCaseSuccess(1);
	testCaseSuccess(1);
	testCaseSuccess(1);
	CU_ASSERT_EQUAL(test_result_buffer[0],0b1010110);
	CU_ASSERT_EQUAL(test_result_buffer[1],0b1);
}

void TN_JustSuccess5(){
	cleanUP();
	test_amount_of_stored_data = amountOf_testNames;
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

void TN_JustSuccessAddName(){
	cleanUP();
	addTestName("abc",3);
	addTestClass("def",3);
	test_amount_of_stored_data = amountOf_testNames;
	CompareInt(1,1,4,0,true,1);
	testCaseSuccess(1);
	CompareInt(1,1,4,0,true,2);
	testCaseSuccess(1);
	CompareInt(2,2,4,0,true,3);
	testCaseSuccess(1);
	CompareInt(3,3,4,0,true,4);
	testCaseSuccess(1);
	CompareInt(4,3,4,0,true,5);

	CU_ASSERT_EQUAL(test_result_buffer[0],0b11);
	CU_ASSERT_EQUAL(test_result_buffer[1],0);
	CU_ASSERT_EQUAL(test_result_buffer[2],3);
	CU_ASSERT_EQUAL(test_result_buffer[3],0);
	CU_ASSERT_EQUAL(test_result_buffer[4],0);
	CU_ASSERT_EQUAL(test_result_buffer[5],0);
	CU_ASSERT_EQUAL(test_result_buffer[6],'a');
	CU_ASSERT_EQUAL(test_result_buffer[7],'b');
	CU_ASSERT_EQUAL(test_result_buffer[8],'c');
	CU_ASSERT_EQUAL(test_result_buffer[9],0b11);
	CU_ASSERT_EQUAL(test_result_buffer[10],0b10);
	CU_ASSERT_EQUAL(test_result_buffer[11],3);
	CU_ASSERT_EQUAL(test_result_buffer[12],0);
	CU_ASSERT_EQUAL(test_result_buffer[13],0);
	CU_ASSERT_EQUAL(test_result_buffer[14],0);
	CU_ASSERT_EQUAL(test_result_buffer[15],'d');
	CU_ASSERT_EQUAL(test_result_buffer[16],'e');
	CU_ASSERT_EQUAL(test_result_buffer[17],'f');
	CU_ASSERT_EQUAL(test_result_buffer[18],0b1010101);
	CU_ASSERT_EQUAL(test_result_buffer[19],0b10);
	CU_ASSERT_EQUAL(test_result_buffer[20],0);
}


void TN_byteCompare1(){
	cleanUP();
	test_amount_of_stored_data = amountOf_testNames;
	int abc[] = {1,2,3};
	int def[] = {1,2,3};
	byteComp(abc,12,def,12,1);
	testCaseSuccess(1);
	CU_ASSERT_EQUAL(test_result_buffer[0],0b01);
}

void TN_byteCompare2(){
	cleanUP();
	test_amount_of_stored_data = amountOf_testNames;
	int abc[] = {1,2,3};
	int def[] = {1,2,4};
	byteComp(abc,12,def,12,1);
	CU_ASSERT_EQUAL(test_result_buffer[0],0b10);
}

void TN_byteCompare3(){
	cleanUP();
	test_amount_of_stored_data = amountOf_testNames;
	int abc[] = {1,2,3};
	int def[] = {1,2,4};
	byteComp(abc,12,abc,12,1);
	testCaseSuccess(1);
	byteComp(abc,12,def,12,1);
	CU_ASSERT_EQUAL(test_result_buffer[0],0b1001);
}



int TN_initCompInt(){
	return 0;
}

CU_pSuite Group_TestNames(CU_pSuite pSuite){
	pSuite = CU_add_suite("TestNames",TN_initCompInt,TN_initCompInt);
   if (NULL == pSuite) {
	  CU_cleanup_registry();
	  exit( CU_get_error());
   }

   if ((NULL == CU_add_test(pSuite, "JustSuccess1", TN_JustSuccess1) )
   		   ||(NULL == CU_add_test(pSuite, "JustSuccess 2", TN_JustSuccess2) )
		   ||(NULL == CU_add_test(pSuite, "JustSuccess 3", TN_JustSuccess3) )
		   ||(NULL == CU_add_test(pSuite, "JustSuccess 4", TN_JustSuccess4) )
		   ||(NULL == CU_add_test(pSuite, "JustSuccess 5", TN_JustSuccess5) )
		   ||(NULL == CU_add_test(pSuite, "add test name which call not be in buffer", TN_JustSuccessAddName) )
		   ||(NULL == CU_add_test(pSuite, "byteCompare 1", TN_byteCompare1) )
		   ||(NULL == CU_add_test(pSuite, "byteCompare 2", TN_byteCompare2) )
		   ||(NULL == CU_add_test(pSuite, "byteCompare 3", TN_byteCompare3) )

      )
	  {
		 CU_cleanup_registry();
		 exit(CU_get_error());
	  }
   return pSuite;
}
