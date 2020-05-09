/*
 * TEST_TestChain.cpp
 *
 *  Created on: Jun 11, 2019
 *      Author: sbalz
 */
#ifndef TEST_TESTCHAIN_H
#define TEST_TESTCHAIN_H
#include "comChain/ComChain.h"
#include "comChain/decoder/NameLessPipe.h"
#include "tester/TargetMock.h"
#include "comChain/decoder/GdbInterface.h"
#include "runModes/AbstractRunMode.h"


class testChain: public comChain {
	//com_gnu_debugger(nameLessPipe *gdbIn, gdbOutputStream *gdbOut, gdbDecoder *decoder, bool createStatistic = false) : comChain(nullptr),
	    //          decoder(decoder), gdbIn(gdbIn), gdbout(gdbOut),createStatistic(createStatistic) {};
public:
		testChain() : comChain(nullptr) {

	    };

	    bool setVariableTo(string variable, string newValue) override{

	    }

	    string evaluateExpression(string expression) override{

	    }

	    void insertBreakpoint(string when) override{

	    }

	    void deleteAllBreakpoints() override{

	    }

	    void setProgramCounterTo(string fucntion) override{

	    }

	    stopFrame *jumpOutOfCurrentFunction(string returnValue) override{

	    }

	    bool initSystem() override{

	    }

	    void setTestLevel(amountOfInfo testLevel) override{

	    }

	    void continueAfterMessage(bool waitUntilNextBreakpoint) override{

	    }

	    void continueAfterMockStub(bool waitUntilNextBreakpoint) override{

	    }

	    void continueAfterReturnFromFunction(bool waitUntilNextBreakpoint) override{

	    }

	    memoryDump *getResultData() override{

	    }

	    void addStub(string when, string expression) override{

	    }

	    void addMock(string when, string mockFunction) override{

	    }

	    bool reduceDependency(string when) override{

	    }

	    void deleteAllDependencies() override{

	    }

	    bool executionHasStopped() override{

	    }

	    stopFrame *getExecutionStopFrame() override{

	    }

	    void closeComChain() override{

	    }

	    bool executeCommand(string command) override{

	    }
	    bool startTestMode() override{

	    }
	    void interruptTarget()override{

	    }

	   void setTargetBreakMode(bool enable)override{

	   }

	   void detachTarget(bool terminateTarget = false)override{

	   }
};

#endif // TEST_TESTCHAIN_H


