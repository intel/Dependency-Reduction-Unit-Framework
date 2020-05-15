/**
 * Copyright (C) 2020 Intel Corporation
 * SPDX-License-Identifier: AGPL-3.0-or-later
 * @author: Sebastian Balz
 */
#include "main.h"


void testNotFail(){
		CU_ASSERT_EQUAL(0,0);
}

int initNotFail(){
	return 0;
}

CU_pSuite Group_ShallNotFail(CU_pSuite pSuite){
	pSuite = CU_add_suite("ShallNotFail",initNotFail,initNotFail);
   if (NULL == pSuite) {
	  CU_cleanup_registry();
	  exit( CU_get_error());
   }

   if ((NULL == CU_add_test(pSuite, "shall not fail  ", testNotFail)))
	  {
		 CU_cleanup_registry();
		 exit(CU_get_error());
	  }
   return pSuite;
}
