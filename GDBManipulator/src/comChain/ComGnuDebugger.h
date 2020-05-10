/**
 * Copyright (C) 2020 Intel Corporation
 * SPDX-License-Identifier: Apache-2.0
 * @author: Sebastian Balz
 */


#ifndef GDBMANIPULATOR_COM_GNU_DEBUGGER_H
#define GDBMANIPULATOR_COM_GNU_DEBUGGER_H


#include "ComChain.h"
#include "comChain/decoder/NameLessPipe.h"
#include "tester/TargetMock.h"
#include "comChain/decoder/GdbInterface.h"
#include <utility>

class com_gnu_debugger : public comChain {
    string setAmount = "test_amount_of_stored_data";
    string setDataAmountJustSuccess = "amountOf_justSuccess";
    string setDataAmountTestNames = "amountOf_testNames";
    string setDataAmountTestResults = "amountOf_testResults";
    string setDataAmountTestLine = "amountOf_lineNumbers";

    string buffToSmall();


protected:
    bool isOpen = false;
    bool sendCommand(string command, bool waitForDone, bool waitForStopped);

    nameLessPipe *gdbIn;
    gdbDecoder *decoder;
    gdbOutputStream *gdbout;
    bool createStatistic{};

    // enable mi2
    string miOn = "-gdb-set mi-async on";
    // continue target execution
    string cont = "-exec-continue";

    string readRes = "-data-read-memory-bytes test_result_buffer "; // + string integer of size
    int targetBufferSize = 0;// needed for statistic

    targetMock *mocks = new targetMock();
    void getTargetDefines();


public:

    //com_gnu_debugger(nameLessPipe *gdbIn, gdbOutputStream *gdbOut, gdbDecoder *decoder, bool createStatistic = false) : comChain(nullptr),
    //          decoder(decoder), gdbIn(gdbIn), gdbout(gdbOut),createStatistic(createStatistic) {};

    com_gnu_debugger(string elfFileLoc_, string gdbLoc_, string gdbServer = "", int gdb_Port_ = 61234);

    bool setVariableTo(string variable, string newValue) override;

    string evaluateExpression(string expression) override;

    void insertBreakpoint(string when) override;

    void deleteAllBreakpoints() override;

    void setProgramCounterTo(string fucntion) override;

    stopFrame *jumpOutOfCurrentFunction(string returnValue) override;

    bool initSystem() override;

    void setTestLevel(amountOfInfo testLevel) override;

    void continueAfterMessage(bool waitUntilNextBreakpoint) override;

    void continueAfterMockStub(bool waitUntilNextBreakpoint) override;

    void continueAfterReturnFromFunction(bool waitUntilNextBreakpoint) override;

    memoryDump *getResultData() override;

    void addStub(string when, string expression) override;

    void addMock(string when, string mockFunction) override;

    bool reduceDependency(string when) override;

    void deleteAllDependencies() override;
    bool renewEUnitBreakpoints() override;
    bool executionHasStopped() override;

    stopFrame *getExecutionStopFrame() override;

    void closeComChain() override;

    bool executeCommand(string command) override;
    bool startTestMode() override;
    void interruptTarget()override;

   void setTargetBreakMode(bool enable)override;

   void detachTarget(bool terminateTarget = false)override;
   void failSilent(bool shallFailSilent) override;
};


#endif //GDBMANIPULATOR_COM_GNU_DEBUGGER_H
