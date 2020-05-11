/**
 * Copyright (C) 2020 Intel Corporation
 * SPDX-License-Identifier: AGPL-3.0-or-later
 * @author: Sebastian Balz
 */

#ifndef EUNIT_H_
#define EUNIT_H_
#include <stdint.h>
#ifdef __cplusplus
extern "C"
{
#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////
// eUnit Configuration
// 		Main Configurations
//		 * EUNIT_TESTMODE
//		 * EUNIT_BISTMODE
//		 * EUNIT_USE_SOFTWARTE_BREAK_POINTs
//		 * EUNIT_LONGJUMP
//		Minor Configurations
//			* SIZE_OF_RESULT_BUFFER
//			* SIZE_OF_TEST_FILTER
/////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Test Mode.
 * Is this defines set to <1> the eUnit framework requires interaction with gdbman, as it uses
 * SW breakpoints for communication
 * This has following effects:
 * 		- add:
 * 		 	* test result buffer <SIZE_OF_RESULT_BUFFER>
 * 		 	* opt in and opt out filter <2 XS IZE_OF_TEST_FILTER>
 * 		- if  <disableBreakMode()> has not been called before invoking
 * 			  <invokeAllTest()> this framework blocks in <MessageToDebugger> and requires the gdb
 * 			  to increase the program counter by 2
 *
 */
#ifndef EUNIT_TESTMODE
#define EUNIT_TESTMODE			1
#endif

/**
 * Build in Self Test Mode
 * enables the user to define callback function for
 * 	* print test result 	<eUnit_SetPrintFunction()>
 * 	* on fail/success/init/end
 * 	no drawbacks besides a little overhead for the callbacks
 * */
#ifndef EUNIT_BISTMODE
#define EUNIT_BISTMODE 			1
#endif

/**
 * use software Breakpoints.
 * Is this define == 1 the eUnit-framework uses Software breakpoints.
 * Otherwise the framework rely on the gdbmanipulator to set hardware breakpoints
 * 	Hardware breakpoints     embedded devices to only have a limited amount of hw- breakpoints
 * 		-->  less mocks and stubs that can be defined simultaneously
 * 	Software breakpoints 	 assembly command to stop the execution
 * 		--> application stops even if the gdbManipulator is not connected
 */
#define EUNIT_USE_SOFTWARTE_BREAK_POINTs 	1

/**
 * Use long jump to return from failed ASSERT
 * This macro intends to return from fail tests  even if they have nested functions calls
 * if EUNIT_LONGJUMP == 1 the Assert macro expect that the test file defines
 * the variable  "static jmp_buf env" as longJump variable
 * 		If one Assert Failes the assert returns the Macro with
 * 				"longjmp (env,1)"
 * 		The test case have to start with
 * 			 val = setjmp (env);
 *	 	 	 if(val) {
 *				return;
 *			 }
 *
 * Is this not the case the Assert Macro returns just with a "return;"
 */
#define EUNIT_LONGJUMP			1

/**
 * do not write the file name to the buffer
 */
#define WRITE_FILE_NAME        	1

/**
 * do not write the class name to the buffer
 */
#define WRITE_CLASS_NAME    	1

/**
 * do not write the test name to the buffer
 */
#define WRITE_TEST_NAME        	1

// The following configurations do only have effect if the EUNIT_TESTMODE is active
#if (EUNIT_TESTMODE == 1)
/**
 * Size of Data puffer.
 * The greater the faster is the test execution but the memory usage increase (of cause :) )
 * it is possible to set the buffer to 0.
 * In that case each data transmit in each test results an gdb interrupt that needs some time to get executed
 *
 * To reduce the buffer usage decrease the testlevel.
 */
#define SIZE_OF_RESULT_BUFFER 256

/**
 * Size of each test filter string
 */
#define SIZE_OF_TEST_FILTER 			124

/////////////////////////////////////////////////////////////////////////////////////////////////////
//   END Configuration
/////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // TEST Mode


#define eUnit_Version 1
/**
 * Message To Debugger.
 * The communication with gdbManipulator is done by the blocking function <MessageToDebugger>.
 * This enum defines the reason why this particular communication is required.
 */
typedef enum messageToDebugger {
	bufferIsFull = 1, 	// the test result buffer is full
	finishAllTest,		// finished all tests and end test mode
	bufferToSmall,// test result buffer is to small. The data has to be read directly
	startTesting,		// start test mode
	addMock,// add mock. Function name and mock is defined in the function below
	deleteMocks,// delete all mocks and subs by removing all hardware breakpoints
	addStub	// add mock. Function name and mock is defined in the function below

} messageToDebugger;

/**
 * test type flags.
 * Information about the test case that is stored in the buffer
 * int8|16|32|char|byte sig/using array
 * This informations is needed to reduce the information about the size of the test result (if type is known)
 * and the kind of comparison
 */
typedef enum testTypeFlags {
	typeNotDef = 0,
	typeCompInt8 = 1,
	typeCompInt16 = 2,
	typeCompInt32 = 3,
	typeCompChar = 4,
	typeCompByte = 5,
	typeCompBool = 6,//1<<3
	typeCompEqual = 1 << 5,
	typeCompUnigned = 1 << 6,
	typeCompArray = 1 << 7
} testTypeFlags;

/**
 * Amount of data that gets stored in the buffer
 */
typedef enum amountOfInfo {
	amountOf_justSuccess = 0, // binary information about the success or failure
	amountOf_testNames = 1, // information about success and the name of the test and its class
	amountOf_testResults = 2, // success, class/test names + shall and is value if failed
	amountOf_lineNumbers = 3 // succes, class/Test names, result and source location and line number of Assertion that failed
} amountOfInfo;

/**
 * Kinf of data in the buffer
 */
typedef enum typeOfData {
	testNotRun = 0,
	testSucceed,            // size data is
	testFailed,            // size dataShould  size dataIS
	testName            // size data
} typeOfData;

typedef struct TestResult {
	typeOfData testStatus;
	testTypeFlags testTypeFlags;
	uint32_t sizeShould, sizeIs;
	void *should;
	void *is;
	uint32_t line;

} TestResult;


#ifndef __cplusplus
typedef enum {
	false, true
} bool;
#endif

/**
 * invoke All Test.
 * call all test groups and init testing by calling
 * 	MessageToDebugger(startTesting);
 *	and at the end
 *	MessageToDebugger(finishAllTest);
 *	@return returns the number of failed tests
 */
unsigned int invokeAllTest();

/**
 * Remove all mocks and stubs.
 * Remove all mocks and stubs by removing all hardware breakpoints
 */
void deleteAllMocks();

/**
 * Add stubs.
 * Add stub by adding an hardware breakpoints and return a defined value if this breakpoint gets hit
 */
void whenStub(void *ifCalled, char *thenDo);

/**
 * Add mock.
 * Add mock by adding an hardware breakpoints and jump to the defined function if the breakpoint gets hit
 */
void whenMock(void *ifCalled, void *thenDo);

/**
 * Disable Break Mode.
 * Is the Test_Mode defined the the framework is going to stop in the function <MessageToDebugger>
 * by calling this function before invoking all tests the framework does not block anymore
 * Use this in case of self testing without gbd
 */
void disableBreakMode();

/**
 * Message to Debugger.
 * This function is the heart of the eUnit framework. Is handles the communication with the gdbManipulator
 * the reduce dependencies and to transfer the test results
 * This function blocks if
 * 	EUNIT_TESTMODE == 1
 * 	AND disableBreakMode has not been called
 */
void __attribute__((used)) MessageToDebugger(messageToDebugger message);

/**
 * Test case success.
 * to reduce the buffer usage just one successful assert get added to the buffer for each succeed test case
 * By calling this function the one successful assert gets added and determine he end of the test case
 */
void testCaseSuccess(bool success);

/**
 * print over GDB.
 * print an NULL terminating string over the gdb interface if the
 * test level is greater than justSuccess
 * volatile to not get optimized out
 */
void __attribute__((used)) printOverGdb(char *);

/**
 * Shall execute this test group.
 * Check if group has been opt out or opt in.
 * This function checks if one group has been opt out
 */
bool shallExecuteTestGroup(char* groupName);

/**
 * Add the test class name.
 * Adds the test class name to the buffer if the testlevel is greater testNames
 */
void addTestClass(char *name, uint32_t size);

/**
 * Add the test file name.
 * Adds the test file name to the buffer if the testlevel is greater lineNumbers
 */
void addTestFileName(char *name, uint32_t size);

/**
 * Add the test name.
 * Adds the test name to the buffer if the testlevel is greater testNames
 */
void addTestName(char *name, uint32_t size);

/**
 * basic compare algorithm for all kind of int value
 */
bool CompareInt(int32_t is, int32_t should, uint8_t size, testTypeFlags type,
		uint8_t eq, uint32_t line_);

/**
 * basic compare algorithm for all kind of memory
 */
bool byteComp(void *is, uint32_t sizeIs, void *should, uint32_t sizeShould,
		uint32_t line_);
/**
 * basic compare algorithm for string
 */
bool eUnitStrComp(char *is, uint32_t sizeIs, char *should, uint32_t sizeShould,
		uint32_t line_);

/**
 * define callback function that can be used to print the
 * test result to the serial interface
 */
void addPrintFunction(int (*pPrint)(const char *fmt, ...));

/**
 * define a callback function that gets called at each failed Assert
 * @param TestResult  result of the failed test
 */
void addcallBackOnEachFail(void (*callBackOnEachFail)(TestResult *));
/**
 * define a callback function that gets called on each successful Assert
 */
void addcallBackonEachSuccess(void (*callBackonEachSuccess)(void));
/**
 * define a callback function that gets called while initialize the system
 */
void addcallBackOnTestInit(void (*callBackOnTestInit)(void));
/**
 * define callback function that gets called on test end
 * it gets  true if all test had been successful and false if at least on test has failed
 */
void addcallBackOnTestExit(void (*callBackOnTestExit)(bool success));

/**
 * detach the target.
 * This function intents to break the connection to the gdbManipulaor if it is connected
 * In eUNit_Test mode and breakMode == true this function blocks until the connection has been closed.
 */
void  __attribute__((used)) detachTarget();


extern const uint8_t EUNIT_TARGET_MODE;

#if(EUNIT_LONGJUMP == 1)
	// each test has to define and register long jump
	#define RETURN_FROM_ASSERT longjmp (env,1);
#else
	#define RETURN_FROM_ASSERT return 1;
#endif

#define ASSERT_EQ(should, is)     if(!CompareInt(is, should, sizeof(should), typeCompEqual, true, __LINE__)){  RETURN_FROM_ASSERT;}
#define ASSERT_NEQ(should, is)    if(!CompareInt(is, should, sizeof(should), typeCompEqual, false, __LINE__)){ RETURN_FROM_ASSERT;}
#define Assert_EQ_Mem(is, sizeIs, should, sizeShould) 	if(testSuccess){if(!byteComp(is,sizeIs,should, sizeShould,__LINE__)){ RETURN_FROM_ASSERT;}
#define ASSERT_NE(should, is)    if(!CompareInt(is, should, sizeof(should), typeCompEqual, false, __LINE__)){ RETURN_FROM_ASSERT;}
#define EXPECT_TRUE(is)  	 	 {testSuccess &= CompareInt(is, 1, 1, typeCompBool, 1, __LINE__); }
#define EXPECT_FALSE(is) 	 	 {testSuccess &=CompareInt(is, 1, 1, typeCompBool, 0, __LINE__);  }
#define ASSERT_TRUE(is) 	 	 if(!CompareInt(is, 1, 1, typeCompBool, 1, __LINE__)){longjmp (env,1); }
#define ASSERT_FALSE(is) 	 	 if(!CompareInt(is, 1, 1, typeCompBool, 0, __LINE__)){longjmp (env,1); }
#define ASSERT_STREQ(should, is) if(!eUnitStrComp((char*)should,strlen(should),(char*)is,strlen(is),__LINE__)){RETURN_FROM_ASSERT;}

#ifdef __cplusplus
}
#endif
#endif //
