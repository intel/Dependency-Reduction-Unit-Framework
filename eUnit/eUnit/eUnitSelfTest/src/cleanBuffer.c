/**
 * Copyright (C) 2020 Intel Corporation
 * SPDX-License-Identifier: AGPL-3.0-or-later
 * @author: Sebastian Balz
 */
#include "main.h"
extern uint8_t test_result_buffer[];
extern volatile uint32_t test_result_index;
extern uint8_t bitShift;
void cleanBufferAfterBuffrISFull(){

	bitShift = 5;
	CU_ASSERT_TRUE(SIZE_OF_RESULT_BUFFER > 0);
	// Fill Buffer
	for(int i = 0; i< SIZE_OF_RESULT_BUFFER; i++){
		test_result_buffer[i] = i+1;
	}
	test_result_index = SIZE_OF_RESULT_BUFFER;
	MessageToDebugger(bufferIsFull);
	// Expect buffer to be empty
	for(int i = 0; i<SIZE_OF_RESULT_BUFFER; i++){
		CU_ASSERT_EQUAL(test_result_buffer[i],0);
	}
	CU_ASSERT_FALSE(bitShift);
	CU_ASSERT_EQUAL(test_result_index,0);
}
void cleanBufferAfterBufferToSmall(){

	bitShift = 5;
	CU_ASSERT_TRUE(SIZE_OF_RESULT_BUFFER > 0);
	// Fill Buffer
	for(int i = 0; i< SIZE_OF_RESULT_BUFFER; i++){
		test_result_buffer[i] = i+1;
	}
	test_result_index = SIZE_OF_RESULT_BUFFER;
	MessageToDebugger(bufferToSmall);
	// Expect buffer to be empty
	for(int i = 0; i<SIZE_OF_RESULT_BUFFER; i++){
		CU_ASSERT_EQUAL(test_result_buffer[i],0);
	}
	CU_ASSERT_FALSE(bitShift);
	CU_ASSERT_EQUAL(test_result_index,0);
}

void cleanNotBuffer(){
	bitShift = 5;
	CU_ASSERT_TRUE(SIZE_OF_RESULT_BUFFER > 0);
	// Fill Buffer
	for(int i = 0; i< SIZE_OF_RESULT_BUFFER && i<255; i++){
		test_result_buffer[i] = i+1;
	}
	test_result_index = SIZE_OF_RESULT_BUFFER;
	MessageToDebugger(startTesting );
	MessageToDebugger(addMock );
	MessageToDebugger(deleteMocks );
	MessageToDebugger(addStub );
	// Expect buffer not to be empty
	for(int i = 0; i<SIZE_OF_RESULT_BUFFER && i<255; i++){
		CU_ASSERT_EQUAL(test_result_buffer[i],i+1);
	}
	CU_ASSERT_EQUAL(bitShift,5);
	CU_ASSERT_EQUAL(test_result_index,SIZE_OF_RESULT_BUFFER);

}


int initBuffer(){
	return 0;
}

CU_pSuite Group_Buffer(CU_pSuite pSuite){
	pSuite = CU_add_suite("CleanBuffer",initBuffer,initBuffer);
   if (NULL == pSuite) {
	  CU_cleanup_registry();
	  exit( CU_get_error());
   }

   if ((NULL == CU_add_test(pSuite, "clean buffer after buffer is full", cleanBufferAfterBuffrISFull) )
		   ||(NULL == CU_add_test(pSuite, "clean buffer after buffer to small", cleanBufferAfterBufferToSmall) )
		   ||(NULL == CU_add_test(pSuite, "Buffer shall not get cleaned with those messages", cleanNotBuffer) )
   )
	  {
		 CU_cleanup_registry();
		 exit(CU_get_error());
	  }
   return pSuite;
}
