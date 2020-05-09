/**
 * Copyright (C) 2020 Intel Corporation
 * SPDX-License-Identifier: Apache-2.0
 * @author: Sebastian Balz
 */
#include <stdio.h>
#include "eUnit.h"

extern int disabledTest;
typedef struct result{
	int success;
	int failed;
}result;

result testResults;
int add5(int a){
    return a+5;
}
/**
 * function that gets on each succeeded test invoked
 */
void succes(){
	testResults.success++;
}
/**
 * function that gets on each failed test invoked
 */
void fail(TestResult *res){
	printf("callback: test failed");
	testResults.failed ++;
    int *is = (res->is);
    int *should = (res->should);
    if(res->testTypeFlags & typeCompEqual){
        printf("\t\texpected %d to be equal to %d \n",*is,*should);
    }else{
        printf("\t\texpected %d not to be equal to %d \n",*is,*should);
    }

}
void init(){
	printf("init callback \n");
}
void end(bool s){
if(s)
	printf("\ncallback at end Test succeed",s);
else
	printf("\ncallback at end Test failed",s);
}

int main() {
    
    addPrintFunction(printf);
    addcallBackOnEachFail(fail);
    addcallBackOnTestExit(end);
    addcallBackOnTestInit(init);
    addcallBackonEachSuccess(succes);
    invokeAllTest();
    
    return 0;
}
