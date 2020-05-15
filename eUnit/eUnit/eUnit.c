/**
 * Copyright (C) 2020 Intel Corporation
 * SPDX-License-Identifier: AGPL-3.0-or-later
 * @author: Sebastian Balz
 */

#ifdef __cplusplus
extern "C"
{
#endif

#include <string.h>
#include "eUnit.h"

/**
 * Kind of String that gets stored in buffer
 */
typedef enum typeOfString
{
	typeOfString_testName,
	typeOfString_testFile,
	typeOfString_testClass,
	typeOfString_gdbMessage

} typeOfString;

/**
 * week default implementation
 */
unsigned int __attribute__((weak)) invokeAllTest()
{
	MessageToDebugger(startTesting);
	CompareInt(1, -1, 1, typeCompEqual, true, -1);
	MessageToDebugger(finishAllTest);
	detachTarget();
	printOverGdb("");
	return 1;
}

/**
 * register mock dependency
 */
void whenMock(void *ifCalled, void *thenDo);

/**
 * register stub dependency
 */
void whenStub(void *ifCalled, char *thenDo);

/**
 * delete all dependencies
 */
void deleteAllMocks();

/**
 * to disable the breakpoint in case of messages.
 * Use this in case of an Testmode without the GDBManipulator or selftest
 * Note: if no messages gets transmitted is is not any more possible to (un)register mocks/stubs
 */
void disableBreakMode();
/**
 * add the name of the current test name
 */
void addTestName(char *name, uint32_t size);
/**
 * add the current class name
 */
void addTestClass(char *name, uint32_t size);
/**
 * add the current test file name
 */
void addTestFileName(char *name, uint32_t size);

/**
 * try to reduce gdb communication
 */
bool hasNewMocks = false;

/**
 * number of disabled tests
 */
volatile int disabledTest = 0;

/**
 * to disable the breakpoint in case of messages.
 * Use this in case of an Testmode without the GDBManipulator or for selftest propose
 */
bool breaksMode = EUNIT_TESTMODE;

/**
 * Informations about defines.
 * This variable is used to inform the gdbManipulator about the eUnit configuration
 */
const uint8_t __attribute__((used)) EUNIT_TARGET_MODE = (EUNIT_TESTMODE<<0) | (EUNIT_BISTMODE << 1) | (EUNIT_LONGJUMP << 2)
		| (EUNIT_USE_SOFTWARTE_BREAK_POINTs<<3) | ((eUnit_Version & 0b111) << 5);

#if(eUnit_Version >= 8)
#error To increase the version count set "EUNIT_TARGET_MODE" to uint16_t
#endif

#if (EUNIT_BISTMODE == 1)
static int (*eUnitPrint)(const char *fmt, ...) = 0;
static int failedTests = 0;
static char * currentTestName = 0;
static char * currentGroupName = 0;
static void (*callBackOnEachFail)(TestResult * res);
static void (*callBackOnEachSuccess)(void);
static void (*callBackOnTestInit)(void);
static void (*callBackOnTestExit)(bool success);
void addResult(TestResult *res);
#endif

uint8_t byteCom(void *a, void *b, int32_t size)
{
	for (int i = 0; i < size; i++)
	{
		if (*((uint8_t *) a + i) != *((uint8_t *) b + i))
			return 0;
	}
	return size >= 0; // just 1 if size is not negativ
}

#if EUNIT_TESTMODE == 1

void addResult(TestResult *res);

/**
 * current index in the buffer
 */
volatile int32_t test_result_index = 0;

/*
 * Test Result Buffer.
 * Codierung : sizeIs, isData, sizeShould, should
 */
uint8_t volatile test_result_buffer[SIZE_OF_RESULT_BUFFER + 1] =
{	0,};

/*
 * if just the test result shall be written to he result buffer
 * the amount of data can be reducedby writing 4 results to one byte
 */
uint8_t bitShift = 0;

/**
 * amount of data that gets stored in the buffer
 */
volatile amountOfInfo test_amount_of_stored_data = amountOf_testResults;

char optOutTestGroups[SIZE_OF_TEST_FILTER] =
{	0,};
char optInTestGroups[SIZE_OF_TEST_FILTER] =
{	0,};


/**
 * check whether the size of the test case is known
 */
bool isTestSizeKnown(testTypeFlags type)
{
	type &= 0b1111;
	return type == typeCompInt32 || type == typeCompInt8
	|| type == typeCompInt16;
}

/**
 * calculate the required size in the test result buffer
 */
static uint32_t calcResultLength(TestResult *res)
{
	unsigned int length = 0;
	length += 1;

	if (res->testStatus == testFailed
			&& test_amount_of_stored_data >= amountOf_testNames)
	{
		length += 1;
		if (!isTestSizeKnown(res->testTypeFlags)) // sizeIS
		length += 4;
		length += res->sizeShould;
		if (!isTestSizeKnown(res->testTypeFlags))// Size should
		length += 4;
		length += res->sizeIs;
		length += res->sizeShould;
		if (test_amount_of_stored_data == amountOf_lineNumbers)
		{
			length += 4;
		}
	}

	return length;
}

/**
 * copy succeeded test.
 */
static void coppySuccess(TestResult *res)
{
	if (bitShift == 0 && test_result_index >= SIZE_OF_RESULT_BUFFER)
	{
		MessageToDebugger(bufferIsFull);
	}
	if (bitShift == 0)
	{
		test_result_index++;
	}
	test_result_buffer[test_result_index - 1] |= (res->testStatus & 0b11) << bitShift;
	bitShift += 2;

	bitShift &= 0b111; // wrap arount by << 8
}

/**
 * success : 1 b = testStatus
 * failed : 1 b = testStatus  , 1b =
 */
static void coppyResult(TestResult *res)
{
	if (res->testStatus != testNotRun)
	{
		// coppy Result head
		bool testSizeIsKnown = isTestSizeKnown(res->testTypeFlags);

		if (res->testStatus == testSucceed)
		coppySuccess(res);
		else if (res->testStatus == testFailed)
		{ // Test Failed
			bitShift = 0;
			*(test_result_buffer + test_result_index) = res->testStatus;// Type of data an its status
			test_result_index += 1;//sizeof(typeOfData);
			// copy line number as int_32 if required
			if (test_amount_of_stored_data == amountOf_lineNumbers)
			{
				*((int *) (test_result_buffer + test_result_index)) = res->line;
				test_result_index += 4;
			}
			// copy test type flags (8-32 bit int , array, ...)
			*(test_result_buffer + test_result_index) = res->testTypeFlags;
			test_result_index += 1;

			// if type not specified copy isSize
			if (testSizeIsKnown == false)
			{
				*(test_result_buffer + test_result_index) = res->sizeIs;
				test_result_index += 4;
			}
			// copy is value
			memcpy(test_result_buffer + test_result_index, res->is,
					res->sizeIs);
			test_result_index += res->sizeIs;

			// if type not specified copy shouldSize
			if (!testSizeIsKnown)
			{
				*(test_result_buffer + test_result_index) = res->sizeShould;
				test_result_index += 4;
			}
			memcpy(test_result_buffer + test_result_index, res->should,
					res->sizeShould);
			test_result_index += res->sizeShould;
		}
	}
}

/**
 * add the test result to the test result buffer depending on the test level
 */
static void coppyResultToBuffer(TestResult *res)
{
	switch (test_amount_of_stored_data)
	{
		case amountOf_justSuccess:
		coppySuccess(res);
		break;
		case amountOf_testNames:
		coppySuccess(res);
		break;
		case amountOf_testResults:
		coppyResult(res);
		break;
		case amountOf_lineNumbers:
		coppyResult(res);
		break;
	}
}



/**
 * Check whether a single filter do apply to a single group.
 *  This function does modify the groupName by changing <*> chars to \NULL
 * @param groupName group name to check    does get modified
 * @param Filter filter to apply to the group name
 * @return
 */
bool doesSingleFilterApplyToGroup(char *groupName, char *Filter)
{
	char tmpFilter[strlen(Filter) + 1];
	strcpy(tmpFilter, Filter);
	char * pch = strtok(tmpFilter, "*"); // search for wildcard
	bool hasMatch = false;
	char * lastToken = tmpFilter;
	char *newIndex = strstr(groupName, pch);// check if the goupe name contains the token
	if (Filter[0] != '*')// check if the first filter char == wildcard then the first token must match to the beginning of the group name
	{
		if (newIndex != groupName)
		return false;
	}
	while (pch != NULL && newIndex != NULL) // while the filter or the search word has tokens left
	{
		lastToken = pch;
		char * tmp = strstr(newIndex, pch);
		if (tmp >= newIndex) // toke
		{
			newIndex = tmp;
			hasMatch = true;
		}
		else
		{
			return false;
		}
		pch = strtok(NULL, "*");
	}
	if (Filter[strlen(Filter) - 1] != '*')
	{ // check if the Filter end with wildcard
		if (strcmp(groupName + strlen(groupName) - strlen(lastToken), lastToken)
				!= 0)
		{
			hasMatch = false;
		}
	}
	return hasMatch;

}

bool doesFilterApplyToGroup(char* groupName, char* filter)
{
	char buffFilter[strlen(filter) + 1];
	char buffGroup[strlen(groupName) + 1];
	strcpy(buffFilter, filter);
	char * pch = strtok(buffFilter, ":"); // split filter
	while (pch != NULL)
	{
		strcpy(buffGroup, groupName);
		if (doesSingleFilterApplyToGroup(buffGroup, pch))
		{
			//printf("\nfilter \"%s\"  does apply to \"%s\"\n",buffGroup,pch);
			return true;
		}
		char * tmp = pch + strlen(pch) + 1;
		pch = strtok(tmp, ":"); // split filter
	}
	return false;
}

#endif  // TEST Mode

void addResult(TestResult *res)
{
#if (EUNIT_TESTMODE == 1)

	uint32_t length = calcResultLength(res);

	if (length >= SIZE_OF_RESULT_BUFFER)
	{ // buffer to small
		MessageToDebugger(bufferToSmall);
	}
	else if (length >= sizeof(test_result_buffer) - test_result_index)
	{ // buffer full
		MessageToDebugger(bufferIsFull);
		coppyResultToBuffer(res);
	}
	else
	{ // space in buffer left
		coppyResultToBuffer(res);
	} // do nothing if buffer is to small because the result gets direct evaluated
#endif
#if (EUNIT_BISTMODE == 1)
	if (eUnitPrint != 0)
	{
		if (res->testStatus == testSucceed)
		{
			eUnitPrint("[       OK ]\n");
		}
		else
		{
			eUnitPrint("[     FAILD]\n");
		}
	}
#endif
}

void addcallBackOnEachFail(void (*callBackOnEachFail_)(TestResult *))
{
#if (EUNIT_BISTMODE == 1)
	callBackOnEachFail = callBackOnEachFail_;
#endif
}
void addcallBackonEachSuccess(void (*callBackonEachSuccess_)(void))
{
#if (EUNIT_BISTMODE == 1)
	callBackOnEachSuccess = callBackonEachSuccess_;
#endif
}
void addcallBackOnTestInit(void (*callBackOnTestInit_)(void))
{
#if (EUNIT_BISTMODE == 1)
	callBackOnTestInit = callBackOnTestInit_;
#endif
}
void addcallBackOnTestExit(void (*callBackOnTestExit_)(bool success))
{
#if (EUNIT_BISTMODE == 1)
	callBackOnTestExit = callBackOnTestExit_;
#endif
}
bool eUnitStrComp(char *is, uint32_t sizeIs, char *should, uint32_t sizeShould,
		uint32_t line_)
{
	return byteComp(is, sizeIs, should, sizeShould, line_);
}
/**
 *  byteOfData | typeOfString | 4Bsize | content  sizeX |
 */
void addStringToBuffer(char *name, uint32_t size, typeOfString stringType)
{
#if (EUNIT_TESTMODE == 1)
	if (((stringType == typeOfString_testName
							|| stringType == typeOfString_testClass
							|| stringType == typeOfString_gdbMessage) // test name or Class
					&& (test_amount_of_stored_data >= amountOf_testNames))// and name/class is requested
			|| (test_amount_of_stored_data >= amountOf_lineNumbers))
	{ // or line number is requested

		bitShift = 0;
		int sizeOfData = 5;// type of data + uInt32

		// add additional flag in detect a file name
		if (test_amount_of_stored_data >= amountOf_lineNumbers)
		sizeOfData += 1;

		if (size + sizeOfData > sizeof(test_result_buffer))
		{
			MessageToDebugger(bufferToSmall);
			bitShift = 0;
			return;
		}
		else if (size + test_result_index + sizeOfData
				> sizeof(test_result_buffer))
		{
			MessageToDebugger(bufferIsFull);
			bitShift = 0;
		}

		// write kind of data type to buffer
		// if not justTEstSuccess indicate that the next byte is the size and the
		//rest of this byte is no just "notRun"
		// if no JustSucces has the bitshift no effect
		uint8_t hasBitShift = bitShift != 0;// = 1 if bitshift
		// if bitshift is greater 0 the index has been moved to the next position
		test_result_index -= hasBitShift;
		*(test_result_buffer + test_result_index) |= testName << bitShift;
		test_result_index += 1;//sizeof(typeOfData); // problem with ucunit gcc compiler
		bitShift = 0;

		test_result_buffer[test_result_index] = stringType;
		test_result_index++;

		// size
		*(test_result_buffer + test_result_index) = size;
		test_result_index += 4;//sizeof(unsigned int);

		// write n
		memcpy(test_result_buffer + test_result_index, name, size);
		test_result_index += size;
	}
#endif
#if (EUNIT_BISTMODE == 1)
	if (eUnitPrint != 0)
	{
		if (stringType == typeOfString_testClass)
		{
			if (currentGroupName != 0)
			eUnitPrint("[----------]\n\n");
			currentGroupName = name;
			eUnitPrint("[----------]%s\n", name);
		}
		else if (stringType == typeOfString_testName)
		{
			currentTestName = name;
		}
		else if (stringType == typeOfString_gdbMessage)
		{
			eUnitPrint(name);
		}
	}

#endif

}

void testCaseSuccess(bool success)
{
	if (success)
	{
		TestResult res;
		res.testStatus = testSucceed;
#if (EUNIT_TESTMODE == 1) || (EUNIT_BISTMODE == 1)
		addResult(&res);
#endif
#if (EUNIT_BISTMODE == 1)
		if(callBackOnEachSuccess != 0){
			callBackOnEachSuccess();
		}
#endif
	}
}

bool shallExecuteTestGroup(char *groupName)
{

	// disabled Test
	bool disabledT = true;
	bool enabledT = true;
#if (EUNIT_TESTMODE == 1)
	if (optOutTestGroups[0] != '\0')
	disabledT = !doesFilterApplyToGroup(groupName, optOutTestGroups);
	if (optInTestGroups[0] != '\0')
	{
		enabledT = false; // if test are enabled the all not enabled test are disabled
		enabledT = doesFilterApplyToGroup(groupName, optInTestGroups);
	}
#endif //TEST Mode
	return enabledT & disabledT;
}

void addTestName(char *name, uint32_t size)
{
	addStringToBuffer(name, size, typeOfString_testName);
#if (EUNIT_BISTMODE == 1)
	if (eUnitPrint != 0)
	{
		eUnitPrint("[ RUN      ] %s.%s\n", currentGroupName, currentTestName);
	}
#endif

}

void addTestClass(char *name, uint32_t size)
{
	addStringToBuffer(name, size, typeOfString_testClass);

}

void addTestFileName(char *name, uint32_t size)
{
	addStringToBuffer(name, size, typeOfString_testFile);
}

void whenMock(void *ifCalled, void *thenDo)
{
#if (EUNIT_TESTMODE == 1)
	if (!breaksMode)
	{
#endif
		printOverGdb("Mocks are not supported in this mode\n");
#if (EUNIT_TESTMODE == 1)
	}
	hasNewMocks = true;
	MessageToDebugger(addMock);
#endif

}

void whenStub(void *ifCalled, char *thenDo)
{
#if (EUNIT_TESTMODE == 1)
	if (!breaksMode)
	{
#endif
	printOverGdb("Stubs are not supported in this mode\n");
#if (EUNIT_TESTMODE == 1)
}
#endif
	hasNewMocks = true;
	MessageToDebugger(addStub);
}

void deleteAllMocks()
{
	if (hasNewMocks == true)
	{
		MessageToDebugger(deleteMocks);
		hasNewMocks = false;
	}
}

void addPrintFunction(int (*pPrint)(const char *fmt, ...))
{
#if (EUNIT_BISTMODE == 1)
	eUnitPrint = pPrint;
#endif
}

void disableBreakMode()
{
#if (EUNIT_BISTMODE == 1) || EUNIT_TESTMODE == 1
	breaksMode = false;
#endif
}

bool CompareInt(int32_t is, int32_t should, uint8_t size, testTypeFlags type,
		uint8_t eq, uint32_t line_)
{
	if(type == typeCompBool)
	{
		if(is >0)
			is = 1; // set True values to 1 to compare them
		type = typeCompInt8;
	}
	TestResult res;
	res.is = &is;
	res.sizeShould = size;
	res.sizeIs = size;

	if (size == 1)
		type |= typeCompInt8;
	else if (size == 2)
		type |= typeCompInt16;
	else if (size == 4)
		type |= typeCompInt32;
	res.testTypeFlags = type;
	res.should = &should;
	res.line = line_;
	if (eq)
	{
		if (is == should)
			res.testStatus = testSucceed;
		else
			res.testStatus = testFailed;
	}
	else
	{
		if (is != should)
			res.testStatus = testSucceed;
		else
			res.testStatus = testFailed;
	}
	if (res.testStatus != testSucceed) // copy just if test failed
	{
#if (EUNIT_BISTMODE == 1)
		// call Back
		if (callBackOnEachSuccess != 0 && res.testStatus == testSucceed)
		callBackOnEachSuccess();
		if (callBackOnEachFail != 0 && res.testStatus == testFailed)
		callBackOnEachFail(&res);
		if (res.testStatus == testFailed)
		failedTests++;
#endif // BIST Mode
#if (EUNIT_TESTMODE == 1) || (EUNIT_BISTMODE == 1)
		addResult(&res);
#endif // Test Mode
	}
	return res.testStatus == testSucceed;
}

bool byteComp(void *is, uint32_t sizeIs, void *should, uint32_t sizeShould,
		uint32_t line_)
{
	TestResult res;
	res.line = line_;
	res.testTypeFlags = typeCompArray | typeCompByte;
	res.testStatus = testFailed;
	res.is = is;
	res.sizeIs = sizeIs;
	res.should = should;
	res.sizeShould = sizeShould;
	if (sizeShould != sizeIs)
		res.testStatus = testFailed;
	else if (byteCom(is, should, sizeIs) == 0)
		res.testStatus = testFailed;
	else
		res.testStatus = testSucceed;
	if (res.testStatus != testSucceed)
	{
#if (EUNIT_BISTMODE == 1)
		if (callBackOnEachSuccess != 0 && res.testStatus == testSucceed)
		callBackOnEachSuccess();
		if (callBackOnEachFail != 0 && res.testStatus == testFailed)
		callBackOnEachFail(&res);
#endif
#if (EUNIT_TESTMODE == 1) || (EUNIT_BISTMODE == 1)
		addResult(&res);
#endif
	}
	return res.testStatus == testSucceed;
}

void printOverGdb(char * buff)
{
#if (EUNIT_TESTMODE == 1) || (EUNIT_BISTMODE == 1)   // just if gdbMan is enabled
	addStringToBuffer(buff, strlen(buff) + 1, typeOfString_gdbMessage);
#endif
}

// https://sourceforge.net/p/predef/wiki/Architectures/
void MessageToDebugger(messageToDebugger message)
{
#if (EUNIT_TESTMODE == 1)
#if (EUNIT_USE_SOFTWARTE_BREAK_POINTs == 1)
	if (breaksMode)// break program
	{
#ifdef __arm__
		asm("bkpt");
#endif // ARM
#ifdef __linux__
		asm("int $3");
#endif // LINUX
	}
#endif // EUNIT_USE_SOFTWARTE_BREAK_POINTs

	if (message == bufferIsFull || message == bufferToSmall|| message == finishAllTest )
	{
		for (int i = 0; i < test_result_index + 1; i++)
		{
			test_result_buffer[i] = 0;
		}
		test_result_index = 0;
		bitShift = 0;
	}
#endif //TEST Mode
#if (EUNIT_BISTMODE == 1)
	if (message == finishAllTest && (eUnitPrint != 0))
	{
		eUnitPrint("[----------]\n[==========]\n");
		if (failedTests > 0)
		eUnitPrint("[   FAILD  ]");
		else
		eUnitPrint("[  SUCCESS ]");
	}
	// call Back
	if (message == startTesting && callBackOnTestInit != 0)
	callBackOnTestInit();
	if (message == finishAllTest && callBackOnTestExit != 0)
	callBackOnTestExit((failedTests == 0));
#endif // BIST Mode

}
__attribute  ((used)) void detachTarget(){
#if (EUNIT_TESTMODE == 1)
#if (EUNIT_USE_SOFTWARTE_BREAK_POINTs == 1)
	if (breaksMode)
	{
#ifdef __arm__
		asm("bkpt");
		// break programm
#endif // ARM
#ifdef __linux__

		asm("int $3");
#endif // LINUX
	}
#endif // (EUNIT_USE_SOFTWARTE_BREAK_POINTs == 1)

#endif // EUNIT_TESTMODE
	breaksMode = false; // disable breakmode incaseof EUNIT_TESTMODE == 1
}

#ifdef __cplusplus
}
#endif
