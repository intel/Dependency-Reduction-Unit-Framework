/**
 * Copyright (C) 2020 Intel Corporation
 * SPDX-License-Identifier: AGPL-3.0-or-later
 * @author: Sebastian Balz
 */
#include "main.h"


extern char optOutTestGroups[SIZE_OF_TEST_FILTER];
extern char optInTestGroups[SIZE_OF_TEST_FILTER];



void setOptOutFilter(char * toSet){
	int i = 0;
	while(toSet[i] != 0){
		optOutTestGroups[i] = toSet[i];
		i++;
	}
	optOutTestGroups[i] = 0;
	optOutTestGroups[i+1] = 0;
}

void setOptInFilter(char * toSet){
	int i = 0;
	while(toSet[i] != '\0'){
		optInTestGroups[i] = toSet[i];
		i++;
	}
	optInTestGroups[i] = 0;
	optInTestGroups[i+1] = 0;
}
void noOptOutDef(){
	setOptOutFilter("");
	setOptInFilter("");
	CU_ASSERT_TRUE(shallExecuteTestGroup(""));
	CU_ASSERT_TRUE(shallExecuteTestGroup("myGroup"));

}
void noOptOutNoMatch(){
	setOptOutFilter("my:Group");
	CU_ASSERT_TRUE(shallExecuteTestGroup(""));
	CU_ASSERT_TRUE(shallExecuteTestGroup("myGroup"));
	CU_ASSERT_STRING_EQUAL(optOutTestGroups,"my:Group");
}
void noOptOutNoMatch2(){
	setOptOutFilter("my:Group*");
	CU_ASSERT_TRUE(shallExecuteTestGroup(""));
	CU_ASSERT_TRUE(shallExecuteTestGroup("myGroup"));
	CU_ASSERT_STRING_EQUAL(optOutTestGroups,"my:Group*");
}
void noOptOutNoMatch3(){
	setOptOutFilter("*my:Group*");
	CU_ASSERT_TRUE(shallExecuteTestGroup(""));
	CU_ASSERT_TRUE(shallExecuteTestGroup("myGroup"));
	CU_ASSERT_STRING_EQUAL(optOutTestGroups,"*my:Group*");
}

void noOptOutMatch1(){
	setOptOutFilter("*my:Group*");
	CU_ASSERT_FALSE(shallExecuteTestGroup("my"));
	CU_ASSERT_TRUE(shallExecuteTestGroup("abcMy"));
	CU_ASSERT_FALSE(shallExecuteTestGroup("abcmy"));
}
void noOptOutMatch2(){
	setOptOutFilter("*my:Group*");

	CU_ASSERT_FALSE(shallExecuteTestGroup("Group"));
	CU_ASSERT_TRUE(shallExecuteTestGroup("group"));
	CU_ASSERT_FALSE(shallExecuteTestGroup("GroupABC"));
}

void noOptInNoMatch1(){
	setOptInFilter("my");
	setOptOutFilter("");
	CU_ASSERT_FALSE(shallExecuteTestGroup("Group"));
	CU_ASSERT_FALSE(shallExecuteTestGroup("group"));
	CU_ASSERT_FALSE(shallExecuteTestGroup("GroupABC"));
}

void noOptInNoMatch2(){
	setOptInFilter("my");
	setOptOutFilter("");
	CU_ASSERT_FALSE(shallExecuteTestGroup("Group"));
	CU_ASSERT_FALSE(shallExecuteTestGroup("group"));
	CU_ASSERT_FALSE(shallExecuteTestGroup("Groupmy"));
	CU_ASSERT_FALSE(shallExecuteTestGroup("myGroup"));
}
void noOptInNoMatch3(){
	setOptInFilter("my:Group");
	setOptOutFilter("");
	CU_ASSERT_TRUE(shallExecuteTestGroup("Group"));
	CU_ASSERT_FALSE(shallExecuteTestGroup("group"));
	CU_ASSERT_FALSE(shallExecuteTestGroup("Groupmy"));
	CU_ASSERT_FALSE(shallExecuteTestGroup("myGroup"));
}
void noOptInNoMatch4(){
	setOptInFilter("my:Group:x*y");
	setOptOutFilter("");
	CU_ASSERT_TRUE(shallExecuteTestGroup("Group"));
	CU_ASSERT_FALSE(shallExecuteTestGroup("group"));
	CU_ASSERT_FALSE(shallExecuteTestGroup("Groupmy"));
	CU_ASSERT_FALSE(shallExecuteTestGroup("myGroup"));
	CU_ASSERT_TRUE(shallExecuteTestGroup("xasdsady"));
	//CU_ASSERT_STRING_EQUAL(enable,"my:Group:x*y");
}

void noOptInMatch1(){
	setOptInFilter("*my");
	setOptOutFilter("");
	CU_ASSERT_FALSE(shallExecuteTestGroup("Group"));
	CU_ASSERT_FALSE(shallExecuteTestGroup("group"));
	CU_ASSERT_FALSE(shallExecuteTestGroup("GroupABC"));
	CU_ASSERT_TRUE(shallExecuteTestGroup("Groupmy"));
	//CU_ASSERT_STRING_EQUAL(enable,"*my");
}
void noOptInMatch2(){
	setOptInFilter( "my*");
	setOptOutFilter("");
	CU_ASSERT_FALSE(shallExecuteTestGroup("Group"));
	CU_ASSERT_FALSE(shallExecuteTestGroup("group"));
	CU_ASSERT_FALSE(shallExecuteTestGroup("GroupABC"));
	CU_ASSERT_TRUE(shallExecuteTestGroup("myGroup"));
	//CU_ASSERT_STRING_EQUAL(enable,"my*");
}

int initopt(){
	return 0;
}

CU_pSuite Group_OptOut(CU_pSuite pSuite){
	pSuite = CU_add_suite("OptOut",initopt,initopt);
   if (NULL == pSuite) {
	  CU_cleanup_registry();
	  exit( CU_get_error());
   }

   if (      (NULL == CU_add_test(pSuite, "noOptOutDef", noOptOutDef) )
		   ||(NULL == CU_add_test(pSuite, "noOptOutNoMatch", noOptOutNoMatch) )
		   ||(NULL == CU_add_test(pSuite, "noOptOutNoMatch", noOptOutNoMatch2) )
		   ||(NULL == CU_add_test(pSuite, "noOptOutNoMatch", noOptOutNoMatch3) )
		   ||(NULL == CU_add_test(pSuite, "noOptOutMatch", noOptOutMatch1) )
		   ||(NULL == CU_add_test(pSuite, "noOptOutMatch", noOptOutMatch2) )

		   ||(NULL == CU_add_test(pSuite, "noOptInNoMatch1", noOptInNoMatch1) )
		   ||(NULL == CU_add_test(pSuite, "noOptInNoMatch2", noOptInNoMatch2) )
		   ||(NULL == CU_add_test(pSuite, "noOptInNoMatch3", noOptInNoMatch3) )
		   ||(NULL == CU_add_test(pSuite, "noOptInNoMatch4", noOptInNoMatch4) )

		   ||(NULL == CU_add_test(pSuite, "noOptInMatch1", noOptInMatch1) )
		   ||(NULL == CU_add_test(pSuite, "noOptInMatch2", noOptInMatch2) )





	      )
	  {
		 CU_cleanup_registry();
		 exit(CU_get_error());
	  }
   return pSuite;
}
