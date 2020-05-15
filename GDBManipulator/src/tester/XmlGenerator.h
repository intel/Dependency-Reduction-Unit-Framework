/**
 * Copyright (C) 2020 Intel Corporation
 * SPDX-License-Identifier: AGPL-3.0-or-later
 * @author: Sebastian Balz
 */

#ifndef GDBMANIPULATOR_XMLGENERATOR_H
#define GDBMANIPULATOR_XMLGENERATOR_H

#include <ostream>
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

class xmlGenerator {
    /*
     <testcase name="echoSTDIN" status="run" time="0.001" classname="nameLessPipeWithFork">
      <failure message="/home/basto/git/EmbeddedTesting/GDBManipulator/test/nameLessPipeTest.cpp:43&#x0A;      Expected: s&#x0A;      Which is: -1&#x0A;To be equal to: sizeof(exep)&#x0A;      Which is: 19" type=""><![CDATA[/home/basto/git/EmbeddedTesting/GDBManipulator/test/nameLessPipeTest.cpp:43
      Expected: s
      Which is: -1
To be equal to: sizeof(exep)
      Which is: 19]]></failure>
    </testcase>
     */
    //<testcase name="Decode_Several_Messages" status="run" time="0" classname="GDB_Message_Async_And_Sync_out" />

    //<testsuite name="GDB_Message_Async_And_Sync_out" tests="2" failures="0" disabled="0" errors="0" time="0">

    // <testsuites tests="40" failures="2" disabled="0" errors="0" timestamp="2018-04-19T22:34:20" time="0.01" name="AllTests">


    string testCases;
    string outFile;
    int testRunClass = 0, testFailedClass = 0, testFailedTotal = 0, testRunTotal = 0;
    string className;
    const string xmlHeader = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    string output;

    void closeClass();

public:

    /**
     * stores test output in xml file like gtest
     * @param output if the output is an empty string no xml file gets written but the xml string gets build
     */
    xmlGenerator(string output = "test_detail.xml") {
        if (output != "")
            outFile = output;
        className = "class_name_not_defined";

    }

    /**
     * finish the xml file and write it to test xmlFile
     * until this function gets called the fileSystem stays unchanged
     * @param errors  number of error while test-evaluation
     * @param timeStamp current timestamp
     * @param runtime test runtime
     * @return string with also gets written to the xml file (e.g for test analyse)
     */
    string buildXml(int errors, string timeStamp = "noTimeStamp", float runtime = 0);

    /**
     * add a Test Case
     * @param testClass
     */
    void addTestClass(string testClass);

    /**
     * if failMessage != "" the test has Failed
     * @param name
     * @param failedMessage
     */
    void addTestCase(string name, string failedMessage = "");

};


#endif //GDBMANIPULATOR_XMLGENERATOR_H
