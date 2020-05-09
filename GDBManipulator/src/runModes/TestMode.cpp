/**
 * Copyright (C) 2020 Intel Corporation
 * SPDX-License-Identifier: Apache-2.0
 * @author: Sebastian Balz
 */


#include "TestMode.h"
#include <thread>
#include "main.h"
extern bool createStatistic;
extern int targetTimeout;
using namespace std::chrono;
static system_clock::time_point lastReset = chrono::high_resolution_clock::now();
bool isTimeout = false;


/**
 * reset timer for timeout.
 */
void resetTimer(){
	Log::log("reset timeout counter",Info);
	lastReset = chrono::high_resolution_clock::now();
}
/**
 * background thread that checks all 10 ms if the function resetTimer has not been called in the last durationMS
 * this this the case this function sets isTimeout and interrupt the target execution.
 * The execute all test function can check whether there was a timeout and add a timeout assert and
 * jump out of the current function
 */
void timerThread(int durationMS,comChain * com){
	//TODO sending two commands (interrupt and continueAfterX) has the effect, that both commands return if one is done
	// current fix is that executeAllTest could no find a frame and waits 10 ms
	// a better solution might be hierarchically sending commands or find a way that just the affected command returns
	resetTimer();
	Log::log("init timeout counter with " + to_string(durationMS) + " ms",Message);
	while(1){
		usleep(10000); // 10 ms
		system_clock::time_point now = chrono::high_resolution_clock::now();
		if(std::chrono::duration_cast<std::chrono::milliseconds>(now - lastReset).count() > durationMS){
			isTimeout = true;
			resetTimer();

			Log::log("timeout counter has reached " + to_string(durationMS) + " ms since last reset --> Interrupt target",Message);
			com->interruptTarget();
			Log::log("send interrupt done",Info);

		}

	}
}

/**
 *
 *  -exec-return    // step out
 *  wenn func="addResult"
 *      -data-evaluate-expression res  // wertet ergebnis aus
 *
 */
string testMode::buffToSmall()
{
    string s;
    stopFrame *frame = jumpOutFunction();
	if (frame->function == "addResult")
	{
        s = com->evaluateExpression("*res");
	}
	else if (frame->function == "addStringToBuffer")
	{
		if (frame->args.at(2)->value == "typeOfString_testClass")
		{
            s = ("buffer to small and got class name");
		}
		else if (frame->args.at(2)->value == "typeOfString_testName")
		{
            s = ("buffer to small and got class name");
		}
		else
		{
            s = ("buffer to small and got File name");
        }
    }
	Log::log("Buffer to short evaluated : \"" + s+"\"", Message);
    return s;
}

void testMode::executeAllTest()
{
    // until unexpected function or all Test finished
    stopFrame *frame = nullptr;
	struct timeval timeStart;
	struct timeval timeEnd;
    bool testNotFinYet; // backup for the while
    gettimeofday(&timeStart, NULL);
	do
	{

        testNotFinYet = false;
		if (com->executionHasStopped())
		{
			resetTimer();
			if (frame != nullptr)
			{
                delete frame; // delete old frame
            }
            frame = com->getExecutionStopFrame();
            if(frame == nullptr){
            	analyser->addError();
            	Log::log("received null pointer frame",Error);
            	return;
            }
			Log::log("Stopped at frame : " + frame->function, Info);
			if (frame->function == "MessageToDebugger")
			{
				Log::log("Got new MessageToDebugger " + frame->args.at(0)->value, Debug);
                gdbMessagesToDebugger(frame);
			}
			else if(frame->function == "detachTarget"){
					com->deleteAllDependencies();
					com->setTargetBreakMode(false);
					com->jumpOutOfCurrentFunction("");
					com->executeCommand("-target-detach");
					Log::log("Terminate Connection to Target",Message);
					analyser->addError();
					cout << analyser->generateStringResult()<<endl<<endl;
					utils::cleanUpAndExit(-11);
			}
			else if(isTimeout){
				isTimeout = false;
				//bool CompareInt(uint32_t is, uint32_t should, uint8_t size, testTypeFlags type,
				//uint8_t eq, uint32_t line_)
				com->evaluateExpression("CompareInt(0,1,1,16,1,-1)");

				com->jumpOutOfCurrentFunction("");
				testNotFinYet = true;
				com->continueAfterMockStub(true);
			}
			else
			{ // Mocks/ Stubs
				if (com->reduceDependency(frame->function))
				{
                    testNotFinYet = true;
				}
				else
				{
					if(frame == 0){
						Log::log("null pointer", Error);
						return;

					}
					Log::log("Debugger has stopped in function \"" + frame->function + "\"", Error);
                }
                com->continueAfterMockStub(true);
            }

		}
		else
		{
			Log::log("expect that execution has stopped with an frame, but there was no frame", Message);
			usleep(100000);// 10 ms
			testNotFinYet = true;
        }

	}while ((frame != nullptr && (frame->function == "MessageToDebugger") && (frame->args.at(0)->value != "finishAllTest")) || (testNotFinYet));
	Log::log("Finished test execution", Message);
    // generate xml File if xml generator != null pointer
	if (analyser->getXmlGenerator() != nullptr)
	{
		Log::log("Generate XML-file" , Message);
        auto end = std::chrono::system_clock::now();
        std::time_t end_time = std::chrono::system_clock::to_time_t(end);
		gettimeofday(&timeEnd, NULL);
        long timeMs = (timeEnd.tv_sec - timeStart.tv_sec) * 1000 + (timeEnd.tv_usec - timeStart.tv_usec) / 1000;
        analyser->getXmlGenerator()->buildXml(analyser->getErrors(), std::ctime(&end_time), timeMs);
    }

}

void testMode::checkForDisabledTests()
{
    disabledTest = com->evaluateExpression(" disabledTest");
	if (disabledTest == "")
	{
		Log::log("Failed to evaluate expression \"disabledTest\"", CriticError);
        com->closeComChain();
        utils::cleanUpAndExit(-10);
	}
	else if (!disabledTest.find("0") == 0)
	{
		disabledTest = "You have " + disabledTest + " disabled tests";
		Log::log(disabledTest, Message);
    } else
        disabledTest = "";
}

void testMode::addStub()
{
	Log::log("Add stub", Message);
    stopFrame *stop = jumpOutFunction();
    string tmp = stop->args.at(0)->value;
    string when = tmp.substr(tmp.find("<") + 1,
                             tmp.size() - tmp.find("<") - 2); // cut from "0x8000f9d <return55>" the return55
    string then = "-exec-return " + stop->args.at(1)->meaning;
    com->addStub(when, stop->args.at(1)->meaning);
}

void testMode::addMock()
{
	Log::log("Add mock", Message);
    stopFrame *stop = jumpOutFunction();
    string tmp = stop->args.at(0)->value;
    string when = tmp.substr(tmp.find("<") + 1,
                             tmp.size() - tmp.find("<") - 2); // cut from "0x8000f9d <return55>" the return55
    tmp = stop->args.at(1)->value;
    com->addMock(when, tmp.substr(0, tmp.find("<")));

}

void testMode::deleteMock()
{
	Log::log("Delete all dependencies", Message);
    com->deleteAllDependencies();
}

stopFrame *testMode::jumpOutFunction()
{
    return com->jumpOutOfCurrentFunction("");
}

void testMode::modeFailed(){
	if(targetIsinited){
		Log::log("\n/////////////////////////////////\n"
				"try to close com Chain and to rescue the current buffer\n "
				"  if this operation fails this application exit within 3 sec\n"
				"/////////////////////////////////\n",UserInfo);
		com->failSilent(true);
		analyser->addError();

        com->interruptTarget();
        usleep(100000);
        com->deleteAllDependencies();
        com->setTargetBreakMode(false);
		memoryDump *d = com->getResultData();
		if (d != nullptr)
		{
			analyser->analyse(d);
			cout << analyser->generateStringResult()<<endl<<endl;
		}
		com->detachTarget(true);
		usleep(100000);
	}
	com->closeComChain();

}


bool testMode::executeMode() {
    testStatistic::startTesting();
	Log::log("Initiate com-chain ... ", UserInfo);
	if (!(com->initSystem() && com->startTestMode()))
	{ // shall run until Message To Debugger
		com->closeComChain();
		analyser->addError();
        return false;
    }else
    {
    	while(com->executionHasStopped())
    	{
    		com->getExecutionStopFrame();
    	}
    }
	com->renewEUnitBreakpoints();
	targetIsinited = true;
	string tmp =  analyser->getAmountOfResultData()+"\"";
	Log::log("Set \"amountOfInfo\" to \""+tmp, Info, logFilter::GDB_Input_Streams);
    com->setTestLevel(testLevel);
    if(optInTests != "") {
		if(eUnitFeatureChecker->checkFeature("RunTimeTestFiler",com->getTargetVersion()) != FeatureSupported) {
            Log::log("eUnit does not Support RunTimeTestFiler in this Version. Failed with " + to_string(ret),CriticError);
            utils::cleanUpAndExit(-14);
        }
    	unsigned int size = atoi(com->evaluateExpression("sizeof(optInTestGroups)").c_str());
    	if(size < optInTests.length()){
			Log::log("Failed to set \"optInFilter\". Size of target buffer is " + to_string(size)+  "  with the current buffer is " + to_string(optOutTests.length()),Error);
			analyser->addError();
		}else{
			Log::log("Set optInTestGroups Filter to " +optInTests,Message );
			optInTests = "\""+optInTests+"\"";
			com->setVariableTo("optInTestGroups", optInTests);
		}
    }
    if(optOutTests != "") {
        featureReturn ret;
		if((ret = eUnitFeatureChecker->checkFeature("RunTimeTestFiler",com->getTargetVersion())) != FeatureSupported) {
            Log::log("eUnit does not Support RunTimeTestFiler in this Version. Failed with " + to_string(ret),CriticError);
            utils::cleanUpAndExit(-14);
        }
    	unsigned int size = atoi(com->evaluateExpression("sizeof(optOutTestGroups)").c_str());
    	if(size < optOutTests.length()){
    		Log::log("Failed to set optOutFilter  size of target buffer is " + to_string(size) +  "  with the current buffer is " + to_string(optOutTests.length()),Error);
			analyser->addError();
    	}else{
    		Log::log("Set optInTestGroups Filter to " + optOutTests,Message );
			optOutTests = "\""+optOutTests+"\"";
			com->setVariableTo("optOutTestGroups", optOutTests);
    	}
    }
    Log::log("Start test execution ... ",UserInfo);

    Log::log("Execute all tests ... ", Message);
    if(targetTimeout > 0)
    {
    	Log::log("set target timeout to "+  to_string(targetTimeout),Message);
    	new thread(timerThread,targetTimeout,com); // enable timer to add timeout assets
    }
    com->continueAfterMessage(true);

    executeAllTest();
    Log::log("Executed all tests", Message);
    testStatistic::endTesting();
    return analyser->getErrors() == 0 && analyser->getFailedTests() == 0;
}


testMode::testMode(comChain *com, amountOfInfo testSpeed,  string optInTest,string optOutTest, string XMLfile, bool printTolCli)
        : abstractRunMode(com), testLevel(testSpeed), optOutTests(optOutTest),optInTests(optInTest){
    xmlGenerator1 = nullptr;
    if (XMLfile != "")
        xmlGenerator1 = new xmlGenerator(XMLfile);

    analyser = new testAnalyser(testSpeed, xmlGenerator1, printTolCli);

}

void testMode::gdbMessagesToDebugger(stopFrame *frame)
{
	if (frame->function == "MessageToDebugger")
	{
		Log::log ("Stopped in \"MessageToDebugger\" with: the argument \""+ frame->args.at(0)->value+ "\"", Message);
        if ((frame->args.at(0)->value == "bufferIsFull" || frame->args.at(0)->value == "finishAllTest" ||
						frame->args.at(0)->value == "bufferToSmall"))
		{
            memoryDump *d = com->getResultData();
			if (d != nullptr)
			{
                analyser->analyse(d);
			}
			else
			{
				Log::log("Read result buffer", Message);
                analyser->analyseDirect(buffToSmall());
            }
			if (frame->args.at(0)->value == "finishAllTest")
			{
                checkForDisabledTests();
            }
			if(frame->args.at(0)->value == "bufferToSmall")
			{
            	com->executeCommand("p memset(test_result_buffer, 0, test_result_index)");
            	com->setVariableTo("test_result_index","0");
				analyser->analyseDirect(buffToSmall());
				com->continueAfterReturnFromFunction(true);
				return;
            }
            // reset results and index
            // cont exec
		}
		else if (frame->args.at(0)->value == "addMock")
		{
            addMock();
		}
		else if (frame->args.at(0)->value == "deleteMocks")
		{
            deleteMock();
		}
		else if (frame->args.at(0)->value == "addStub")
		{
            addStub();
		}
		else
		{
			Log::log("got unexpected \"MessateToDebugger\": \"" + frame->args.at(0)->value +
					"\"\nPossible reason: target has been reseted", Error);
            analyser->addError();
            utils::cleanUpAndExit(-10);
        }
        com->continueAfterMessage(frame->args.at(0)->value != "finishAllTest");
		Log::log ("Continue test execution", Info);
	}
	else
	{
		Log::log("Has been called with wrong function " + frame->function, Error);
    }

}

string testMode::analyseResult()
{
  return  analyser->generateStringResult() + "\n"+ disabledTest;
}

