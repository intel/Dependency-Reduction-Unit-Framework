/*
*****************************************************************************

  File        : main.cpp

  The MIT License (MIT)
  Copyright (c) 2019 STMicroelectronics

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.

*****************************************************************************
*/
#define EUNIT_BISTMODE 1 // Use Self Test Mode
#include<stdio.h>
#include "eUnit.h";

typedef struct result{
	int success;
	int failed;
}result;

result testResults;
/**
 * function that gets on each succeeded test invoked
 */
void succes(){
	testResults.success++;
	printf("test: success\r\n");
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
	printf("init tests \n");
}
void end(bool s){
	if(s)
		printf("\ncallback at end Test succeed",s);
	else
		printf("\ncallback at end Test failed",s);

	printf("\r\nresults: %d failed \t %d succeed\r\n",testResults.failed,testResults.success);
}

int main(void){
	// Note: define EUNIT_BISTMODE 1
	printf("Callback example\r\n");
	disableBreakMode(); // No break points
    addcallBackOnEachFail(fail);
    addcallBackOnTestExit(end);
    addcallBackOnTestInit(init);
    addcallBackonEachSuccess(succes);
	int n;
	if((n=invokeAllTest()) == 0){
		printf("\r\nno test failed\r\n");
	}else{
		printf("\r\n%d test failed\r\n",n);
	}

	return 0;
}
