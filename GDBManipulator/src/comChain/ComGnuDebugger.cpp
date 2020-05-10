/**
 * Copyright (C) 2020 Intel Corporation
 * SPDX-License-Identifier: Apache-2.0
 * @author: Sebastian Balz
 */

#include "ComGnuDebugger.h"
#include "main.h"

bool com_gnu_debugger::sendCommand(string command, bool waitForDone, bool waitForStopped) {
    testStatistic::startTransmit();
    decoder->setLastCommand(commandState::waitForResult);
    Log::log("Sending command:  \"" + command + "\"  to the gdb", Message, COM_CHAIN);
    if(! isOpen){
    	Log::log("failed to send command because the chain is not open",Message);
    	return false;
    }
    string sendcommand = command +"\n";
    ////////////////////////////
    //cout << "\t<<-- " <<command<<endl;
    ///////////////////////////
    decoder->setDebuggerState(debuggerState::debuggerStateResetet);
    decoder->setLastCommand(commandState::waitForResult);

    //gdbIn->write(tmp, static_cast<int>(command.length()));
    const char *buff = sendcommand.c_str();
    gdbIn->writeToSTD_IN((char *) buff, sendcommand.length());
    if (waitForStopped) {
        Log::log("Sending command: and wait until the gdb mode is \"stopped\" ... ", Debug, COM_CHAIN);

        while (decoder->getDebuggerState() != debuggerState::debuggerHasStopped) {
            if (decoder->getLastCommand() == commandState::debuggerError) {
               testStatistic::endTransmit();
             if(tryToFailSilent)
             {
      					Log::log("Failed to send the gdb command = " + command, Error, COM_CHAIN);
      					utils::cleanUpAndExit(-8);
      				}else{
      					Log::log("Failed to send the gdb command = " + command, Message, COM_CHAIN);
      					return false;
      				}
            }
            usleep(100);
        }
        Log::log("Sending command: debugger has stopped", Info, COM_CHAIN);
    }
    if (waitForDone) {
        Log::log("Wait until the debugger has executed the command ...", Debug, COM_CHAIN);
        while (decoder->getLastCommand() != commandState::debuggerDone) {
            usleep(100);
            if (decoder->getLastCommand() == commandState::debuggerError) {
                testStatistic::endTransmit();
            if(tryToFailSilent)
            {
    					Log::log("Failed to send the gdb command = " + command, Error, COM_CHAIN);
    					utils::cleanUpAndExit(-8);
    				}else{
    					Log::log("Failed to send the gdb command = " + command, Message, COM_CHAIN);
    					return false;
    				}
            }
        }
        Log::log("Sending command was success full", Info, COM_CHAIN);
    }
    string tmp = decoder->getLastCommand()+"\"";
    Log::log("Sending command got new command state \"" + tmp, Debug, COM_CHAIN);
    testStatistic::endTransmit();
    usleep(1000); // give some time to analyse the next buff
    Log::log("Sending command: "+command+"  was successful", Message, COM_CHAIN);
    return true;
}


bool com_gnu_debugger::setVariableTo(string variable, string newValue) {
    Log::log("Set Target variable \"" + variable + "\" to \"" + newValue+"\"", Info, logFilter::COM_CHAIN);
    testStatistic::startMemWrite();
    string toSend = "set " + variable + " = " + newValue;
    bool out = sendCommand(toSend, true, false);
    testStatistic::endMemWrite(1);
    return out;
}

string com_gnu_debugger::evaluateExpression(string expression) {
    testStatistic::startMemRead();

    sendCommand("-data-evaluate-expression " + expression, true, false);
    testStatistic::endMemRead(-1, -1);
    if (gdbout->hasNextValue()) {
        return gdbout->getLastValue();
    } else {
        Log::log("Failed to evaluate expression \"" + expression+"\"", CriticError, logFilter::COM_CHAIN);
        return "";
    }
}

void com_gnu_debugger::getTargetDefines(){
	Log::log("get targets defines",Info,logFilter::COM_CHAIN);
	 int s = atoi(evaluateExpression("EUNIT_TARGET_MODE").c_str());

	 if((s & 0b1) > 0) // EUNIT_TESTMODE Flag Set
	 {
		 Log::log("Target has defined EUNIT_TESTMODE",Message,logFilter::COM_CHAIN);
		 targetIsInTestMode = true;
	 }
	 if((s & 0b10) >0) // EUNIT_BISTMODE Flag Set
	 {
	 		 Log::log("Target has defined EUNIT_BISTMODE",Message,logFilter::COM_CHAIN);
	 		 targetIsInBistMode = true;
	 }
	 if((s & 0b100) >0) // EUNIT_LONGJUMP Flag set
	 {
		 Log::log("Target has defined EUNIT_LONGJUMP",Message,logFilter::COM_CHAIN);
		 targetIsInBistMode = true;
	 }
	 if((s & 0b1000) >0) // EUNIT_USE_SOFTWARTE_BREAK_POINTs
	 {
		 Log::log("Target has defined EUNIT_USE_SOFTWARTE_BREAK_POINTs",Message,logFilter::COM_CHAIN);
		 targetUsesesSWBreakpoints = true;
	 }
	 else
     {
         Log::log("Target does NOT use Software Breakpoints",Message,logFilter::COM_CHAIN);
     }
     targetVersion = s >>5;
	 Log::log("eUnit used Version 0" + to_string(targetVersion),Message);
}

void com_gnu_debugger::insertBreakpoint(string when) {
    sendCommand("-break-insert " + when, true, false); // add breakpoint
    testStatistic::addGDBExec();
}

void com_gnu_debugger::deleteAllBreakpoints() {
	Log::log("remove all breakpoints",Info);
    sendCommand("-break-delete", true, false);
    testStatistic::addGDBExec();
    renewEUnitBreakpoints();
}

void com_gnu_debugger::setProgramCounterTo(string fucntion) {
    sendCommand("set $pc = " + fucntion, true, false);
    testStatistic::addGDBExec();
}

stopFrame *com_gnu_debugger::jumpOutOfCurrentFunction(string returnValue) {
    stopFrame *frame = nullptr;
    sendCommand("-exec-return " + returnValue, true, true); // return immediately
    testStatistic::addGDBExec();
    if (gdbout->hasStopFrames()) {
        frame = gdbout->getLatestStopFrame();
    } else {
        Log::log("The execution has currently no stop frame", Error);

    }
    return frame;
}

bool com_gnu_debugger::initSystem() {
    Log::log("Initiate \"Gnu Debugger\"... ", Message, logFilter::COM_CHAIN);
    testStatistic::addGDBExec();
    isOpen = true;
    sendCommand(miOn, true, false);
    getTargetDefines();
    if (createStatistic) {
        targetBufferSize = stoi(evaluateExpression("sizeof(test_result_buffer)"), 0);
        testStatistic::addGDBExec();
    }
    return true;
}

bool com_gnu_debugger::startTestMode() {
	 if(!targetIsInTestMode){
		 Log::log("The target has not defined #EUNIT_TESTMODE with == 1\r\n "
				 "The current mode expects the target to stop in \"MessageToDebugger\". Is this Flag not set this will "
                 "not happen (or if the break mode has been disabled).\r\n"
				 "  Please use \"--runMode runMode\" or set the define to 1 and reflash the target with \"-f\" \r\n"
                 "  Another solution might be to check whether the target has been compiled with the debug option\r\n"  ,Error);
		 return false;
	 }
	testStatistic::addGDBExec();

    Log::log("Start target execution", Info, logFilter::COM_CHAIN);
    renewEUnitBreakpoints();
   sendCommand("run", false, true);

   // wait until Test stating
   // set amount of test result data
   if (!gdbout->hasStopFrames()) {
       Log::log("Expected that the target has been stopped in function : \"MessageToDebugger\" with the argument \"StartTest\" but has no stop frame", Error);
       return false;
   }
   stopFrame *frame = gdbout->getLatestStopFrame();
       if (frame->function != "MessageToDebugger" || frame->args.at(0)->value != "startTesting") {
           Log::log("Expected that the target has been stoped in function : \"MessageToDebugger\" with the argument \"StartTest\" but has stopped in :\""
                    + frame->function + "\" with \"" + frame->args.at(0)->value + "\"", Error, logFilter::GDB_Input_Streams);
           return false;
       }
   Log::log("Target has stopped as expected with startTesting", Info);

    return true; //redefine abc
}

void com_gnu_debugger::setTestLevel(amountOfInfo testLevel) {
    switch (testLevel) {
        case amountOfInfo::amountOf_justSuccess :
            setVariableTo(setAmount, setDataAmountJustSuccess);
            break;
        case amountOfInfo::amountOf_testNames   :
            setVariableTo(setAmount, setDataAmountTestNames);
            break;
        case amountOfInfo::amountOf_testResults :
            setVariableTo(setAmount, setDataAmountTestResults);
            break;
        case amountOfInfo::amountOf_lineNumbers :
            setVariableTo(setAmount, setDataAmountTestLine);
            break;
    }
}

void com_gnu_debugger::continueAfterMessage(bool waitUntilNextBreakpoint) {

    sendCommand(cont, true, waitUntilNextBreakpoint);
    testStatistic::addGDBExec();

}

void com_gnu_debugger::continueAfterMockStub(bool waitUntilNextBreakpoint) {
    sendCommand(cont, true, waitUntilNextBreakpoint);
    testStatistic::addGDBExec();
}

void com_gnu_debugger::continueAfterReturnFromFunction(bool waitUntilNextBreakpoint){
	continueAfterMessage(waitUntilNextBreakpoint);
}
memoryDump *com_gnu_debugger::getResultData() {

	if(targetIsInTestMode){ // read the result just if the result is not opt out
		//cout << "get index"<<endl;
		string sizeOfResults = evaluateExpression("test_result_index"); // get buffer index
		// get results with size of index
		if (atoi(sizeOfResults.c_str()) > 0) {
			testStatistic::startMemRead();
			sendCommand(readRes + sizeOfResults, true, false);
			testStatistic::endMemRead(targetBufferSize, atoi(sizeOfResults.c_str()));
			if (decoder->hasNextDump()) {
				memoryDump *d = decoder->getNextDump();
				return d;
			} else {
				Log::log("Failed to evaluate the targets buffer", Error);
			}
		}
	}
    return nullptr;
}

void com_gnu_debugger::addStub(string when, string expression) {

    string then = "-exec-return " + expression;
    mocks->addMock(when, then);
    Log::log("Add stub when :\"" + when + "\" then: \"" + then+"\"", Message);
    insertBreakpoint("*"+when);
    testStatistic::addStub();
}

void com_gnu_debugger::addMock(string when, string mockFunction) {
    string then = "set $pc = " + mockFunction;
    mocks->addMock(when, then);
    Log::log("Add mock when :\"" + when + "\" then: \"" + mockFunction+"\"", Message);
    insertBreakpoint("*"+when);
    testStatistic::addMock();
}

bool com_gnu_debugger::reduceDependency(string when) {
    if (mocks->hasMock(when)) {
        Log::log("Target has stopped to reduce dependency \"" +when+"\"", Message);
        string toDo = mocks->getThen(when);
        if (sendCommand(toDo, true, false)) {
            return true;
        }
        Log::log("Failed to execute mock \"" +when+"\"", CriticError);
    }
    return false;
}

void com_gnu_debugger::deleteAllDependencies() {
    Log::log("Delete all mock and stub breakpoints", Message);
    if (mocks->hasMocks()) { // reset breakpoints just if the are some set
        mocks->deleteAllMocks();
        deleteAllBreakpoints();
        testStatistic::addMockDel();
    }

}

bool com_gnu_debugger::renewEUnitBreakpoints(){
	if(!targetUsesesSWBreakpoints)
	{
	    featureReturn ret;
	     if((ret = eUnitFeatureChecker->checkFeature("UseSoftwareBreakpoints",targetVersion)) != FeatureSupported) {
	         Log::log("eUnit does not Support UseSoftwareBreakpoints in this Version. Failed with " +to_string(ret),CriticError);
             utils::cleanUpAndExit(-14);
         }
         insertBreakpoint("detachTarget");
         if(!targetIsInTestMode) // test Mode Flage NOT set
         {
             insertBreakpoint("printOverGdb"); // no test results
         }
         else // TEST Mode Flag SET
         {
             insertBreakpoint("MessageToDebugger");
         }

	}
	if(!targetIsInTestMode){
        insertBreakpoint("printOverGdb");
	}
	return true;
}

bool com_gnu_debugger::executionHasStopped() {
    return gdbout->hasStopFrames();
}

stopFrame *com_gnu_debugger::getExecutionStopFrame() {
    return gdbout->getLatestStopFrame();
}

com_gnu_debugger::com_gnu_debugger(string elfFileLoc_, string gdbLoc_, string gdbServer, int gdb_Port_) : comChain(nullptr)
{
    Log::log("Create com-chain \"ComGnuDebugger\"", Info);
    decoder = new gdbDecoder();
    gdbInterface *inp = new gdbInterface(
            std::move(elfFileLoc_), std::move(gdbLoc_), gdbServer, gdb_Port_, decoder);
    inp->open();
    gdbout = inp->getClient()->getOutStream();
    gdbIn = inp->getClient()->getNameLessPipe();
}

void com_gnu_debugger::closeComChain() {
	isOpen = false;
    Log::log("Close comChain \"ComGnuDebugger\"", Message);
    //sendCommand("-exec-interrupt", true, false);
    char toShare[] = "true";
    utils::sharedMemoryWrite(toShare, 4); // close com chains
    sendCommand("quit", false, false);
    if (nextInstance != nullptr) {
        nextInstance->closeComChain();
    }
    Log::log("ComChain has been closed", Info);
}

bool com_gnu_debugger::executeCommand(string command) {
    if(command != "") {
        if (!sendCommand(command, true, false)) {
            return nextInstance->executeCommand(command);
        }
    }
    return true;
}



void com_gnu_debugger::interruptTarget() {
	 Log::log("Interrupt Target", Message, logFilter::COM_CHAIN);
	 sendCommand("interrupt", true, true);
}

void com_gnu_debugger::setTargetBreakMode(bool enable){
    if(targetIsInTestMode) {
        Log::log("set Break Mode: " + to_string(enable), Message);
        setVariableTo("breaksMode", to_string(enable));
    }
}

void com_gnu_debugger::detachTarget(bool terminateTarget){
 if(terminateTarget){
	 char toShare[] = "true"; // disconnect result in a socket error in the gdb-Server
	 utils::sharedMemoryWrite(toShare, 4); // close com chains
	 sendCommand("-target-disconnect",true,false);
 }else{
	 sendCommand("-target-detach",true,false);
 }
}

void com_gnu_debugger::failSilent(bool shallFailSilent) {
   if(nextInstance != nullptr){
       nextInstance->failSilent(shallFailSilent);
   }
   if(shallFailSilent)
       utils::sharedMemoryWrite("true",4);
}


