/**
 * Copyright (C) 2020 Intel Corporation
 * SPDX-License-Identifier: AGPL-3.0-or-later
 * @author: Sebastian Balz
 */


#ifndef GDBMANIPULATOR_TESTER_H
#define GDBMANIPULATOR_TESTER_H

#include <iostream>

#include "AbstractRunMode.h"
#include "tester/TestAnalyser.h"
#include "comChain/ComChain.h"
#include "TestMode.h"
#include <sys/time.h>

using namespace std;
extern string targetResetFunction;

/**
 * GDB-Input Stream.
 * Send data to the gdb's input stream and controll the target.
 */
class testMode: public abstractRunMode {
protected:

    amountOfInfo testLevel;
    xmlGenerator *xmlGenerator1;
    string xmlFile;
    string optOutTests;
    string optInTests;

    string buffToSmall();
    bool targetIsinited = false;
    /**
     * execute all test until the function "message to Debuger "called with the value finished all test
     * and evaluate all test results
     */
    void executeAllTest();

    testAnalyser *analyser;

    /**
     * add an Mock
     * date read from the targets stack from the calling function
     *      (how has called the message function)
     */
    void addMock();

    /**
   * addn an Stub
   * date read from the targets stack from the calling function
   *      (how has called the message function)
   */
    void addStub();

    /**
     * request to delete all Mocks
     */
    void deleteMock();

    /**
     * the gdbMessage requieres action with the targets buffer
     */
    virtual void gdbMessagesToDebugger(stopFrame *pFrame);

    /**
     * jump out of the current function and return the frame of calle of the current function
     */
    stopFrame *jumpOutFunction();

    string disabledTest = "";
    /**
     * Check if the target has marked test as disabled
     */
    void checkForDisabledTests();

public:

    /**
     * Tester.
     * instance that drives the test.
     * @param com communication chain that defines the different communication levels like serial->GDB->None
     * @param testSpeed amount of information that gets transmittet from the target to the GDBManipulator the higher the testlevel the slower is the test execution
     * @param XMLfile if != "" an result xml file gets created at the given location
     * @param printTolCli if true the result gets printet to the cli
     */
    explicit testMode(comChain *com, amountOfInfo testSpeed, string optInTest = "",string optOutTest = "", string XMLfile = "",
                      bool printTolCli = true);

    /**
    * run.
    * This function blocks until all test results are evaluated or an error occurs.
    */
    bool executeMode() override;

    string analyseResult() override;

    void modeFailed() override;

};


#endif //GDBMANIPULATOR_TESTER_H
