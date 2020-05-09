/**
 * Copyright (C) 2020 Intel Corporation
 * SPDX-License-Identifier: Apache-2.0
 * @author: Sebastian Balz
 */


#include "ComArmNoneEabiGdb.h"


bool com_arm_none_eabi_gdb::flashTarget() {
    Log::log("Flash Target", Message, logFilter::COM_CHAIN);
    testStatistic::startFlash();
    sendCommand("load", true, false);
    testStatistic::endFlash();
    Log::log("Flash done", Message, logFilter::COM_CHAIN);
    return true;
}

bool com_arm_none_eabi_gdb::initSystem() {
    if ((conf.gdbClient == "") | (conf.elfFileLoc == "")) {
        Log::log("Aarm none eabi gdb requires an gdbClient and an elf file ", CriticError, logFilter::COM_CHAIN);
        return false;
    }
    isOpen = true;
    Log::log("Arm none eabi testMode-debugger init system", Info, logFilter::COM_CHAIN);
    testStatistic::addGDBExec();
    sendCommand(miOn, true, false);
    Log::log("Connect to target on port: " + to_string(conf.gdb_serverPort), Message, logFilter::COM_CHAIN);
    testStatistic::addGDBExec();
    if (!sendCommand(connectToTarget + to_string(conf.gdb_serverPort), true, true)) {
        Log::log("Failed to send command. Might it be possible that no GDB_Server is running on port " +
                 to_string(conf.gdb_serverPort) + "?", CriticError, logFilter::COM_CHAIN);
        isOpen = false;
        return false;
    }
    Log::log("Connected to remote target ", Message, logFilter::COM_CHAIN);
    stopFrame *stopFrame1;
    if (!gdbout->hasStopFrames()) {
        Log::log("Failed to connect to target", Error, logFilter::COM_CHAIN);
        isOpen = false;
        return false;
    } else if (conf.flash) {
        sendCommand("monitor flash set_parallelism_mode 2", true, false);
        Log::log("Flash target ... ", UserInfo, logFilter::COM_CHAIN);
        flashTarget();
        setProgramCounterTo(conf.resetHandler);
        Log::log("Flash done", UserInfo, logFilter::COM_CHAIN);
        stopFrame1 = gdbout->getLatestStopFrame(); // pop from list

        delete stopFrame1;
    }
    getTargetDefines();
    return true;
}

void com_arm_none_eabi_gdb::continueAfterMessage(bool waitUntilNextBreakpoint) {
	if(targetIsInTestMode || !targetUsesesSWBreakpoints){
		sendCommand(contAfterBreak, true, false);
	}
    testStatistic::addGDBExec();
    com_gnu_debugger::continueAfterMessage(waitUntilNextBreakpoint);
}

void com_arm_none_eabi_gdb::continueAfterReturnFromFunction(bool waitUntilNextBreakpoint) {
	com_gnu_debugger::continueAfterMessage(waitUntilNextBreakpoint);
}

bool com_arm_none_eabi_gdb::startTestMode() {
	 if(!targetIsInTestMode){
		 Log::log("The target has not defined \"#EUNIT_TESTMODE\" with \"1\"\r\n "
				 "is this not the case the \"eUnit\"-Framework is inactive. \r\n "
                 "Please set the define to <1> and recompile the framework and flash the target with <-f>\r\n "
                 "Alternative it is possible to use the \"gdbMann::runMode\" <--runMode runMode> to use the optional "
                 "BIST-Mode or to display the \"printOverGdb\" messages. ",UserInfo);
		 return false;
	 }
    renewEUnitBreakpoints();
	sendCommand(cont, false, true);
    stopFrame *stopFrame1;

    if (conf.createTestStatistic) {
        targetBufferSize = stoi(evaluateExpression("sizeof(test_result_buffer)"), 0);
        testStatistic::addGDBExec();
    }
    Log::log("Start target execution", Info, logFilter::COM_CHAIN);
   // sendCommand(cont, false, true);
    usleep(1000);
    // wait until Test stating
    // set amount of test result data
    if (!gdbout->hasStopFrames()) {
        Log::log("Expected function : Message to Debugger with StartTest has a frame", Error, logFilter::COM_CHAIN);
        return false;
    }
    stopFrame1 = gdbout->getLatestStopFrame();
    if (stopFrame1->function != "MessageToDebugger" || stopFrame1->args.at(0)->value != "startTesting") {

        Log::log("Expected function : Message to Debugger with StartTest but was :"
                 + stopFrame1->function, Error, logFilter::GDB_Input_Streams);
        return false;
    }
    Log::log("Target has stopped with \"startTesting\"", Message);
    delete stopFrame1;
    return true;
}
