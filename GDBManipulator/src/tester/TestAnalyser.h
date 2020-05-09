/**
 * Copyright (C) 2020 Intel Corporation
 * SPDX-License-Identifier: Apache-2.0
 * @author: Sebastian Balz
 */

#ifndef GDBMANIPULATOR_TESTANALYSER_H
#define GDBMANIPULATOR_TESTANALYSER_H


#include <ctime>
#include <chrono>

#include "MemoryDump.h"
#include "comChain/decoder/StopFrame.h"
#include "XmlGenerator.h"

/**
 * Tpy of test data.
 *
 */
typedef enum typeOfData {
    testNotRun = 0,         /// test not run
    testSucceed,            /// size data is
    testFailed,             /// size dataShould  size dataIS
    testName                /// size data

} typeOfData;

/**
 * different types amount of data.
 * The more data gets transfered the slower is the test execution
 */
typedef enum amountOfInfo {
    amountOf_justSuccess,       /// coppy just success to buffer
    amountOf_testNames,         /// add test and class names
    amountOf_testResults,       /// add result
    amountOf_lineNumbers        /// add line number of failed asserts

} amountOfInfo;

/**
 * Type of string.
 * Defines which kind of string lays in the buffer
 */
typedef enum typeOfString {
    typeOfString_testName,
    typeOfString_testFile,
    typeOfString_testClass,
	typeOfString_gdbMessage

} typeOfString;

/**
 * Analyse the Test-Buffer.
 * This class receive an buffer an analyse it depending to the amount of test data.
 */
class testAnalyser {
    const string RUNTEST = "[ RUN      ]";
    const string TESTOK = "[       OK ]";
    const string TESTFAIL = "[  FAILED  ]";
    amountOfInfo amountOfResultData;
    int testPerGroup = 0;
    /**
     * Type of test.
     */
    typedef enum testTypeFlags {
        typeNotDef = 0,
        typeCompInt8 = 1,
        typeCompInt16 = 2,
        typeCompInt32 = 3,
        typeCompChar = 4,
        typeCompByte = 5,
        typeCompBool = 6,//1<<3
		typeTimeOut = 1<<4,
        typeCompEqual = 1 << 5,
        typeCompUnsigned = 1 << 6,
        typeCompArray = 1 << 7
    } testTypeFlags;

    /**
     * Analyse Data.
     * Use if amount of data is greater than Test-Result
     * @param dump  data du analyse
     * @return no error accure d
     */
    bool analyseResults(memoryDump *dump);

    /**
     * Analyse Data.
     * Use if amount of data is less  than Test-Result
     * @param dump  data du analyse
     * @return no error ocurred
     */
    bool analyseSuccess(memoryDump *dump);

    /**
     * Print test data to CLI.
     * if true the test result gets printed to the cli
     */
    bool printLiveToCli;

    /**
     * current string to print
     */
    string resultString;
    /**
     * success full tests
     */
    unsigned int succeed = 0;
    /**
     * errors while testing
     */
    unsigned int errors = 0;
    /**
     * failed tests
     */
    unsigned int failed = 0;

    unsigned int disabledTest = 0;


    string currentTestName = "not defined test ";
    string currentFileName = "not defined file";
    string currentClassName = "not defined class";

    /**
     * analyse the results of a failed test.
     * this method decode one single test Result for a failed test
     * @param data result buffer
     * @param i index of the result start
     * @return  string with human readable result
     */
    string analyseFailedResult(uint8_t *data, int *i);

    /**
     * analyse two result arrays in the  buffer.
     * an make them human readable
     * @param startIs
     * @param sizeIs
     * @param startShould
     * @param sizeShould
     * @return
     */
    string arrayAnalyse(uint8_t *startIs, int sizeIs, uint8_t *startShould, int sizeShould);

    xmlGenerator *xml;
public:
    amountOfInfo getAmountOfResultData() const;


    /**
     * new testanalyser.
     *
     * @param amountInfo defines how much data is expected to lay in buffer
     * @param xmlGen
     * @param printLiveToCli_
     */
    testAnalyser(amountOfInfo amountInfo = amountOf_justSuccess, xmlGenerator *xmlGen = nullptr,
                 bool printLiveToCli_ = true) {
        printLiveToCli = printLiveToCli_;
        amountOfResultData = amountInfo;
        resultString = "[==========] \n[----------] \n[----------] ";

        xml = xmlGen;
    };

    ~testAnalyser() {

    };

    /**
     * analyse the dump.
     * analyse the dump and print its meaning to the cli
     * @param dump mem-dump to analyse
     * @return if any failer happens with the GDB! false
     */
    bool analyse(memoryDump *dump);

    /**
     * analyse the result direct if e.g the buffer is to short
     */
    bool analyseDirect(string directData);

    /**
     * generate the output of the rest result
     * if printLiveToCli_ = true the output does not contain the rest results
     * @return result-string
     */
    string generateStringResult();


    /**
     * number of succeeded tests
     * @return
     */
    unsigned int getSucceedTests() const;

    /**
     * number of failed tests
     */
    unsigned int getFailedTests() const;

    /**
     * if the error counter is greater than 0 i means a deconding error has occurred.
     * the the result could be corrupted!.
     * @return error count
     */
    unsigned int getErrors() const;


    /**
     * return an XML-Generator obj
     * @return
     */
    xmlGenerator *getXmlGenerator();

    /**
     * increase the error counter by one.
     */
    void addError();

    void setDisabledTests(int disabledTest_){
    	disabledTest = disabledTest_;
    }
};


#endif //GDBMANIPULATOR_TESTANALYSER_H
