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


void assert_LineNumbers(){
	cleanUP();
	test_amount_of_stored_data = amountOf_lineNumbers;
	uint8_t a = 55;
	uint16_t b = 300;
	uint32_t c = 1000;
	CompareInt(a,55,1,typeCompEqual |typeCompInt8 ,true,1);
	testCaseSuccess(1);

	CompareInt(c,1000,4,typeCompEqual |typeCompInt8,true,2);
	testCaseSuccess(1);
	CU_ASSERT_EQUAL(test_result_buffer[0],0b0101)
}


void assertFail1_LineNumbers(){
	cleanUP();
	test_amount_of_stored_data = amountOf_lineNumbers;
	uint8_t a = 55;
	uint16_t b = 300;
	uint32_t c = 1000;
	CompareInt(a,55,1,typeCompEqual |typeCompInt8 ,true,1);
	testCaseSuccess(1);
	CompareInt(999,c,4,typeCompEqual |typeCompInt32,true,43);
	CompareInt(c,1000,4,typeCompEqual|typeCompInt32,true,2);
	CompareInt(a,55,1,typeCompEqual|typeCompInt8,true,1);
	testCaseSuccess(1);
	testCaseSuccess(1);
	CU_ASSERT_EQUAL(test_result_buffer[0],0b01)
	CU_ASSERT_EQUAL(test_result_buffer[1],0b10)
	CU_ASSERT_EQUAL(*(uint32_t *) (test_result_buffer+2),43);
	CU_ASSERT_EQUAL(test_result_buffer[6],0b100011)
	CU_ASSERT_EQUAL(*(uint32_t *) (test_result_buffer+7),999);
	CU_ASSERT_EQUAL(*(uint32_t *) (test_result_buffer+11),1000);
	CU_ASSERT_EQUAL(test_result_buffer[15],0b101);
	CU_ASSERT_EQUAL(test_result_buffer[16],0);
}

void resultNameWithLine(){
	cleanUP();
	addTestName("abc",3);
	addTestClass("def",3);
	test_amount_of_stored_data = amountOf_lineNumbers;
	CompareInt(1,1,4,typeCompInt32,true,1);
	testCaseSuccess(1);
	CompareInt(1,1,4,typeCompInt32,true,2);
	testCaseSuccess(1);
	CompareInt(2,2,4,typeCompInt32,true,3);
	testCaseSuccess(1);
	CompareInt(3,3,4,typeCompInt32,true,4);
	testCaseSuccess(1);
	CompareInt(4,3,4,typeCompInt32,false,5);
	testCaseSuccess(1);
	//printf("\n");
	//for(int i = 0; i<25;i++){
	//    printf(" %d : %d  | %c \n",i, test_result_buffer[i],test_result_buffer[i]);
	//}
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
	CU_ASSERT_EQUAL(test_result_buffer[19],0b1);
	CU_ASSERT_EQUAL(test_result_buffer[20],0);
}



int initTestLines(){
	return 0;
}

CU_pSuite Group_TestLines(CU_pSuite pSuite){
	pSuite = CU_add_suite("JustResult and Asserts",initTestLines,initTestLines);
   if (NULL == pSuite) {
	  CU_cleanup_registry();
	  exit( CU_get_error());
   }

   if ((NULL == CU_add_test(pSuite, "check assert", assert_LineNumbers))
		   || (NULL == CU_add_test(pSuite, "assertFail1 withLineNumbers", assertFail1_LineNumbers))
		   || (NULL == CU_add_test(pSuite, "add test name and test group", resultNameWithLine))

		   )
	  {
		 CU_cleanup_registry();
		 exit(CU_get_error());
	  }
   return pSuite;
}

