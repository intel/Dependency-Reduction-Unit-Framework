/**
 * Copyright (C) 2020 Intel Corporation
 * SPDX-License-Identifier: Apache-2.0
 * @author: Sebastian Balz
 */

#include <CUnit/CUnit.h>
#include <stdint.h>
#include "../../eUnit.h"


extern volatile amountOfInfo test_amount_of_stored_data;

extern volatile uint32_t test_result_index ;
extern volatile amountOfInfo test_amount_of_stored_data;
extern uint8_t test_result_buffer[SIZE_OF_RESULT_BUFFER];

extern void cleanUP();


void assert(){
	cleanUP();
	test_amount_of_stored_data = amountOf_testResults;
	uint8_t a = 55;
	uint16_t b = 300;
	uint32_t c = 1000;
	CompareInt(a,55,1, typeCompEqual, true,1);
	CompareInt(c,1000,4, typeCompEqual, true,2);
	testCaseSuccess(1);
	testCaseSuccess(1);
	CU_ASSERT_EQUAL(test_result_buffer[0],0b0101)
}


void addFileName(){
	cleanUP();
	test_amount_of_stored_data = amountOf_testResults;
	uint8_t a = 55;
	uint16_t b = 300;
	uint32_t c = 1000;
	CompareInt(a,55,1, typeCompEqual, true,1);
	testCaseSuccess(1);
	addTestFileName("fileName",8);
	CompareInt(c,1000,4, typeCompEqual, true,2);
	testCaseSuccess(1);
	CU_ASSERT_EQUAL(test_result_buffer[0],0b0101)
}


void assertFail1(){
	cleanUP();
	test_amount_of_stored_data = amountOf_testResults;
	uint8_t a = 55;
	uint16_t b = 300;
	uint32_t c = 1000;
	CompareInt(a,55,1, typeCompEqual|typeCompInt8, true,1);
	testCaseSuccess(1);
	CompareInt(999,c,4, typeCompEqual | typeCompInt32, true,2);
	CompareInt(1000,c,4, typeCompEqual| typeCompInt32, true,2);
	testCaseSuccess(1);
	CompareInt(a,55,1, typeCompEqual |typeCompInt8, true,1);
	testCaseSuccess(1);

	CU_ASSERT_EQUAL(test_result_buffer[0],0b01)
	CU_ASSERT_EQUAL(test_result_buffer[1],0b10)
	CU_ASSERT_EQUAL(test_result_buffer[2],0b100011)
	CU_ASSERT_EQUAL(*(uint32_t *) (test_result_buffer+3),999);
	CU_ASSERT_EQUAL(*(uint32_t *) (test_result_buffer+7),1000);
	CU_ASSERT_EQUAL(test_result_buffer[11],0b101);
}
void resultName(){
	cleanUP();
	addTestName("abc",3);
	addTestClass("def",3);
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

int initJustResult(){
	return 0;
}

CU_pSuite Group_JustResult(CU_pSuite pSuite){
	pSuite = CU_add_suite("JustResult and Asserts",initJustResult,initJustResult);
   if (NULL == pSuite) {
	  CU_cleanup_registry();
	  exit( CU_get_error());
   }

   if ((NULL == CU_add_test(pSuite, "assert", assert))
		   || (NULL == CU_add_test(pSuite, "assertFail1", assertFail1))
		   || (NULL == CU_add_test(pSuite, "add testname and testgroup  to  buffer", resultName))
		   || (NULL == CU_add_test(pSuite, "add file name to buffer", addFileName))
		   )
	  {
		 CU_cleanup_registry();
		 exit(CU_get_error());
	  }
   return pSuite;
}

