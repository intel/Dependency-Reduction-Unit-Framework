/**
 * Copyright (C) 2020 Intel Corporation
 * SPDX-License-Identifier: AGPL-3.0-or-later
 * @author: Sebastian Balz
 */

#include "RunMode.h"

void runMode::gdbMessagesToDebugger(stopFrame *frame)
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
                cout << analyser->generateStringResult()<<endl<<endl;
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
		}else if(frame->args.at(0)->value == "startTesting"){
			string tmp =  analyser->getAmountOfResultData()+"\"";
			Log::log("Set \"amountOfInfo\" to \""+tmp, Info, logFilter::GDB_Input_Streams);
			com->setTestLevel(testLevel);
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

        }
        com->continueAfterMessage(true);
		Log::log ("Continue test execution", Info);
	}
	else
	{
		Log::log("Has been called with wrong function " + frame->function, Error);
    }

}

void runMode::executeAllTest()
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
			if (frame != nullptr)
			{
                delete frame;
            }
            frame = com->getExecutionStopFrame();
			Log::log("Stopped at frame : " + frame->function, Info);

			if (frame->function == "MessageToDebugger")
			{
				Log::log("Got new MessageToDebugger " + frame->args.at(0)->value, Debug);
                gdbMessagesToDebugger(frame);
			}else if(frame->function == "printOverGdb"){
				string message = frame->args.at(0)->meaning;
				if(message.size()>= 3 && message.at(message.size()-1) == 'n' && message.at(message.size()-2) == '\\' )
						cout << message.substr(0,message.size()-3 )<<endl;
				else{
					cout << message;
					cout << flush;
				}

				testNotFinYet = true;
				com->executeCommand("-exec-return"); // jump over the addToBuffer function
				com->continueAfterMockStub(true);
			} else if(frame->function == "detachTarget"){
				com->deleteAllDependencies();
				com->setTargetBreakMode(false);
				com->executeCommand("-target-detach");
				Log::log("Terminate Connection to Target",Message);
				analyser->addError();
				cout << analyser->generateStringResult()<<endl<<endl;
				utils::cleanUpAndExit(-11);
			}
			else
			{ // Mocks/ Stubs
				if (com->reduceDependency(frame->function))
				{
                    testNotFinYet = true;
				}
				else
				{
					Log::log("Debugger has stopped in function \"" + frame->function + "\"", Error);
                }
                com->continueAfterMockStub(true);
            }

		}
		else
		{
			Log::log("\"ExecuteAllTest\" did not found a new frame", Error);
        }

	}while ((frame->function == "MessageToDebugger") || (testNotFinYet));
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

bool runMode::executeMode(){
	  testStatistic::startTesting();
		Log::log("Initiate com-chain ... ", UserInfo);
		if (!(com->initSystem()))
		{
			Log::log("Failed to initiate com-chain ", Error);
			com->closeComChain();
			analyser->addError();
			return false;
	    }
		Log::log("Initiate com-chain done", UserInfo);
		com->renewEUnitBreakpoints();
		targetIsinited = true;
		Log::log("Start runMode ... ", Message);
	    com->continueAfterMessage(true);
	    executeAllTest();
	    Log::log("End runMode", UserInfo);
	    testStatistic::endTesting();
	    return true;
}




