/*
 *  Simple example of a CUnit unit test.
 *
 *  This program (crudely) demonstrates a very simple "black box"
 *  test of the standard library functions fprintf() and fread().
 *  It uses suite initialization and cleanup functions to open
 *  and close a common temporary file used by the test functions.
 *  The test functions then write to and read from the temporary
 *  file in the course of testing the library functions.
 *
 *  The 2 test functions are added to a single CUnit suite, and
 *  then run using the CUnit Basic interface.  The output of the
 *  program (on CUnit version 2.0-2) is:
 *
 *           CUnit : A Unit testing framework for C.
 *           http://cunit.sourceforge.net/
 *
 *       Suite: Suite_1
 *         Test: test of fprintf() ... passed
 *         Test: test of fread() ... passed
 *
 *       --Run Summary: Type      Total     Ran  Passed  Failed
 *                      suites        1       1     n/a       0
 *                      tests         2       2       2       0
 *                      asserts       5       5       5       0
 */
#define EUNIT_NO_GTEST

#include <stdio.h>
#include <string.h>
#include "CUnit/Basic.h"


CU_pSuite callGroups(CU_pSuite pSuite){
	pSuite = Group_ShallNotFail(pSuite);
	pSuite = Group_Buffer(pSuite);
	pSuite = Group_ByteComp(pSuite);
	pSuite = Group_JustSuccess(pSuite);
	pSuite = Group_TestNames(pSuite);
	pSuite = Group_JustResult(pSuite);
	pSuite = Group_TestLines(pSuite);
	pSuite = Group_OptOut(pSuite);
	return pSuite;
}

/* The main() function for setting up and running the tests.
 * Returns a CUE_SUCCESS on successful running, another
 * CUnit error code on failure.
 */
int main()
{
	disableBreakMode();
   CU_pSuite pSuite = NULL;

   /* initialize the CUnit test registry */
   if (CUE_SUCCESS != CU_initialize_registry())
      return CU_get_error();

   pSuite = callGroups(pSuite);
   /* Run all tests using the CUnit Basic interface */
   CU_basic_set_mode(CU_BRM_VERBOSE);
   CU_basic_run_tests();
  
  if(CU_get_number_of_tests_failed() >0){
	return -1;
  }else if(CU_get_error() >0)
	return -1;
  else
	return 0;
   return CU_get_error();
}
