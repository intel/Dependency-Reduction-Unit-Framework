/**
 * Copyright (C) 2020 Intel Corporation
 * SPDX-License-Identifier: Apache-2.0
 * @author: Sebastian Balz
 */


#include "DecodeState.h"

#include "logging.h"
#include "LogFileFilter.h"


void decoderState::setLastCommand(commandState set) {
    Log::log("Set the current command state from \""+to_string(lastCommand1)+"\" to \"" + to_string(set)+"\"", DebugL2, logFilter::gdbMI2decoder);
    commandMutex.lock();
    lastCommand1 = set;
    commandMutex.unlock();
}

commandState decoderState::getLastCommand() {

    commandState t;
    commandMutex.lock();
    t = lastCommand1;
    commandMutex.unlock();
    return t;
}


debuggerState decoderState::getDebuggerState() {
    debuggerState s;
    stateMutex.lock();
    s = debuggerState1;
    stateMutex.unlock();
    return s;
}


void decoderState::setDebuggerState(debuggerState state_) {
    Log::log("Set debugger state to \"" + to_string(state_)+"\"", DebugL2, logFilter::gdbMI2decoder);
    stateMutex.lock();
    debuggerState1 = state_;
    stateMutex.unlock();

}


std::ostream &operator<<(std::ostream &strm, const commandState &a) {
    if (a == waitForResult)
        strm << "{wait for result} ";
    else if (a == commandState::debuggerDone)
        strm << "{done} ";
    else
        strm << "{error} ";
    return strm;
}

std::ostream &operator<<(std::ostream &strm, const decodeKindeOfDecodedLine &a) {
    string s;
    switch (a) {
        case decodingFailed   :
            s = "decodingFailed ";
            break;
        case decodedAsyncOut  :
            s = "decodedAsyncOut ";
            break;
        case decodedMessage   :
            s = "decodedMessage ";
            break;
        case decodeSyncronOut  :
            s = "decodeSyncronOut ";
            break;

        default:
            s = "undefined decode Status ";
            break;
    }
    return strm << s;
}

std::ostream &operator<<(std::ostream &strm, const debuggerState &a){
	  string s;
	switch (a){
	case debuggerHasStopped:
		s = "debuggerHasStopped"; break;
	case    debuggerIsRunning:
		s = "debuggerIsRunning"; break;
	case    debuggerStateResetet:
		s = "debuggerStateResetet"; break;

	}
	return strm<<s;
}
