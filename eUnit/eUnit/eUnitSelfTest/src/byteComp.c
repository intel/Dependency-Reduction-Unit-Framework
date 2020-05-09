/**
 * Copyright (C) 2020 Intel Corporation
 * SPDX-License-Identifier: Apache-2.0
 * @author: Sebastian Balz
 */
#include "main.h"
extern uint8_t byteCom(void *a, void *b, int32_t size);


void comapareSelf(){
	uint8_t abc[5] = {1,2,3,4,5};
	CU_ASSERT_EQUAL(byteCom(abc,abc,5),1);
}

void comapareEql(){
	uint8_t abc[5] = {1,2,3,4,5};
	uint8_t def[5] = {1,2,3,4,5};
	CU_ASSERT_EQUAL(byteCom(abc,def,5),1);
}
void comapareEql2(){
	uint8_t abc[6] = {1,2,3,4,5,6};
	uint8_t def[5] = {1,2,3,4,5};
	CU_ASSERT_EQUAL(byteCom(abc,def,5),1);
}
void comapareEql3(){
	uint8_t abc[6] = {1,2,3,4,5,6};
	uint8_t def[5] = {1,2,3,4,5};
	CU_ASSERT_EQUAL(byteCom(def,abc,5),1);
}

void compareFail(){
	uint8_t abc[5] = {1,2,3,4,6};
	uint8_t def[5] = {1,2,3,4,5};
	CU_ASSERT_EQUAL(byteCom(def,abc,5),0);
	CU_ASSERT_EQUAL(byteCom(abc,def,5),0);
}
void compareFail2(){
	uint8_t abc[5] = {1,2,3,4,6};
	uint8_t def[5] = {2,2,3,4,6};
	CU_ASSERT_EQUAL(byteCom(def,abc,5),0);
		CU_ASSERT_EQUAL(byteCom(abc,def,5),0);
}

void compareNegIndex(){
	uint8_t abc[5] = {1,2,3,4,6};
	uint8_t def[5] = {1,2,3,4,6};
	CU_ASSERT_EQUAL(byteCom(def,abc,-1),0);
		CU_ASSERT_EQUAL(byteCom(abc,def,-1),0);
}
int initByteComp(){
	return 0;
}

CU_pSuite Group_ByteComp(CU_pSuite pSuite){
	pSuite = CU_add_suite("ByteCompare",initByteComp,initByteComp);
   if (NULL == pSuite) {
	  CU_cleanup_registry();
	  exit( CU_get_error());
   }

   if ((NULL == CU_add_test(pSuite, "comapareSelf", comapareSelf) )
   		   ||(NULL == CU_add_test(pSuite, "comapareEql", comapareEql) )
   		   ||(NULL == CU_add_test(pSuite, "comapareEql2", comapareEql2) )
		   ||(NULL == CU_add_test(pSuite, "comapareEql3", comapareEql3) )
		   ||(NULL == CU_add_test(pSuite, "compareFail2", compareFail2) )
		   ||(NULL == CU_add_test(pSuite, "compareFail", compareFail) )
		   ||(NULL == CU_add_test(pSuite, "compareNegIndex", compareNegIndex) )
      )
	  {
		 CU_cleanup_registry();
		 exit(CU_get_error());
	  }
   return pSuite;
}
