/**
 * Copyright (C) 2020 Intel Corporation
 * SPDX-License-Identifier: Apache-2.0
 * @author: Sebastian Balz
 */



#include "ComChain.h"

void comChain::failSilent(bool shallFailSilent){
	tryToFailSilent = shallFailSilent;
}

bool comChain::setVariableTo(string variable, string newValue) {
    if (nextInstance != nullptr) {
        return nextInstance->setVariableTo(variable, newValue);
    }
    Log::log("ComChain has reached its last chain for \"setVariableTo\"", CriticError, logFilter::COM_CHAIN);
    return false;
}

string comChain::evaluateExpression(string expression) {
    if (nextInstance != nullptr) {
        return nextInstance->evaluateExpression(expression);
    }
    Log::log("ComChain has reached its last chain for  \"evaluateExpression\"", CriticError, logFilter::COM_CHAIN);
    return "";
}

void comChain::insertBreakpoint(string when) {
    if (nextInstance != nullptr) {
        return nextInstance->insertBreakpoint(when);
    }
    Log::log("ComChain has reached its last chain for  \"insertBreakpoint\"", CriticError, logFilter::COM_CHAIN);
}

void comChain::deleteAllBreakpoints() {
    if (nextInstance != nullptr) {
        return nextInstance->deleteAllBreakpoints();
    }
    Log::log("ComChain has reached its last chain for  \"deleteAllBreakpoints\"", CriticError, logFilter::COM_CHAIN);
}

void comChain::setProgramCounterTo(string function) {
    if (nextInstance != nullptr) {
        return nextInstance->setProgramCounterTo(function);
    }
    Log::log("ComChain has reached its last chain for  \"setProgramCounterTo\"", CriticError, logFilter::COM_CHAIN);

}

stopFrame *comChain::jumpOutOfCurrentFunction(string returnValue) {
    if (nextInstance != nullptr) {
        return nextInstance->jumpOutOfCurrentFunction(returnValue);
    }
    Log::log("ComChain has reached its last chain for  \"jumpOutOfCurrentFunctiob\"", CriticError, logFilter::COM_CHAIN);
    return nullptr;
}

bool comChain::initSystem() {
    if (nextInstance != nullptr) {
        return nextInstance->initSystem();
    }
    Log::log("ComChain has reached its last chain for  \"initSystem\"", CriticError, logFilter::COM_CHAIN);
    return false;
}

void comChain::setTestLevel(amountOfInfo testLevel) {
    if (nextInstance != nullptr) {
        return nextInstance->setTestLevel(testLevel);
    }
    Log::log("ComChain has reached its last chain for  \"setTestLevel\"", CriticError, logFilter::COM_CHAIN);
    return;

}

void comChain::continueAfterMessage(bool waitUntilNextBreakpoint) {
    if (nextInstance != nullptr) {
        return nextInstance->continueAfterMessage(waitUntilNextBreakpoint);
    }
    Log::log("ComChain has reached its last chain for  \"continueAfterBreakpoint\"", CriticError, logFilter::COM_CHAIN);
    return;
}

void comChain::continueAfterReturnFromFunction(bool waitUntilNextBreakpoint){
	 if (nextInstance != nullptr) {
	        return nextInstance->continueAfterReturnFromFunction(waitUntilNextBreakpoint);
	    }
	    Log::log("ComChain has reached its last chain for  \"continueAfterReturnFromFunction\"", CriticError, logFilter::COM_CHAIN);
	    return;
}

void comChain::continueAfterMockStub(bool waitUntilNextBreakpoint) {
    if (nextInstance != nullptr) {
        return nextInstance->continueAfterMockStub(waitUntilNextBreakpoint);
    }
    Log::log("ComChain has reached its last chain for  \"continueAfterMockStub\"", CriticError, logFilter::COM_CHAIN);
    return;
}

memoryDump *comChain::getResultData() {
    if (nextInstance != nullptr) {
        return nextInstance->getResultData();
    }
    Log::log("ComChain has reached its last chain for \"continueAfterMockStub\"", CriticError, logFilter::COM_CHAIN);
    return nullptr;
}

void comChain::addStub(string when, string then) {
    if (nextInstance != nullptr) {
        return nextInstance->addStub(when, then);
    }
    Log::log("ComChain has reached its last chain for \"addStub\"", CriticError, logFilter::COM_CHAIN);
}


void comChain::addMock(string when, string mockFunction) {
    if (nextInstance != nullptr) {
        return nextInstance->addMock(when, mockFunction);
    }
    Log::log("ComChain has reached its last chain for \"addMock\"", CriticError, logFilter::COM_CHAIN);
}


bool comChain::reduceDependency(string when) {
    if (nextInstance != nullptr) {
        return nextInstance->reduceDependency(when);
    }
    Log::log("ComChain has reached its last chain for \"reduceDependency\"", CriticError, logFilter::COM_CHAIN);
    return false;
}

void comChain::deleteAllDependencies() {
    if (nextInstance != nullptr) {
        return nextInstance->deleteAllDependencies();
    }
    Log::log("ComChain has reached its last chain for \"deleteAllDependencies\"", CriticError, logFilter::COM_CHAIN);

}


bool comChain::renewEUnitBreakpoints(){
	 if (nextInstance != nullptr) {
		return nextInstance->renewEUnitBreakpoints();
	}
	Log::log("ComChain has reached its last chain for \"renewEUnitBreakpoints\"", CriticError, logFilter::COM_CHAIN);
 return false;
}

bool comChain::executionHasStopped() {
    if (nextInstance != nullptr) {
        return nextInstance->executionHasStopped();
    }
    Log::log("comChain has reached its last chain for executionHasStopped", CriticError, logFilter::COM_CHAIN);
    return false;
}

stopFrame *comChain::getExecutionStopFrame() {
    if (nextInstance != nullptr) {
        return nextInstance->getExecutionStopFrame();
    }
    Log::log("comChain has reached its last chain for getExecutionStopFrame", CriticError, logFilter::COM_CHAIN);
    return nullptr;
}

void comChain::closeComChain() {
    if (nextInstance != nullptr) {
        return nextInstance->closeComChain();
    }
    Log::log("comChain has reached its last chain for closeComCHain", CriticError, logFilter::COM_CHAIN);
    return;
}

bool comChain::executeCommand(string command) {
    if (nextInstance != nullptr) {
        return nextInstance->executeCommand(command);
    }
    Log::log("comChain is unable to execute the command  "+ command,CriticError);
    return false;
}

bool comChain::startTestMode() {
    if (nextInstance != nullptr) {
        return nextInstance->startTestMode();
    }
    Log::log("comChain has reached its last chain for startTestMode", CriticError, logFilter::COM_CHAIN);
    return false;
}

void comChain::interruptTarget(){
	if (nextInstance != nullptr) {
	        return nextInstance->interruptTarget();
	}
	Log::log("comChain has reached its last chain for interruptTarget", CriticError, logFilter::COM_CHAIN);
}



    void comChain::setTargetBreakMode(bool enable){
    	if (nextInstance != nullptr) {
			return nextInstance->setTargetBreakMode(enable);
		}
		Log::log("comChain has reached its last chain for setTargetBreakMode", CriticError, logFilter::COM_CHAIN);
    }


    void comChain::detachTarget(bool terminateTarget){
    	if (nextInstance != nullptr) {
			return nextInstance->detachTarget(terminateTarget);
		}
		Log::log("comChain has reached its last chain for detachTarget", CriticError, logFilter::COM_CHAIN);
    }

uint8_t comChain::getTargetVersion() {
    return targetVersion;
}




