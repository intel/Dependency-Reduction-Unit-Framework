/**
 * Copyright (C) 2020 Intel Corporation
 * SPDX-License-Identifier: Apache-2.0
 * @author: Sebastian Balz
 */

#ifndef GDBMANIPULATOR_DECODESTATE_H
#define GDBMANIPULATOR_DECODESTATE_H


#include <string>
#include <iostream>
#include <mutex>

using namespace std;
/**
 * Additional decoding info.
 * additional infos to the current decoding state
 */
enum decodeAdditionalInfo {
    noteEvaluated = 0,
    done,
    failedToEvaluate,
    error,
    doneWithValue,
    doneWithMemory,
    failedToDecodeMem,
    stopped,
    running
};

/**
 * kind of the decoded line.
 */
enum decodeKindeOfDecodedLine {
    decodingNotEvaluated,
    decodingFailed, // Decoding failed
    decodedAsyncOut,
    decodedMessage,
    decodeSyncronOut
};

/**
 * Debugger state.
 * current running mode of the debugger
 */
enum debuggerState {
    debuggerHasStopped,
    debuggerIsRunning,
    debuggerStateResetet // use this if you like to reset if you like to wait for a state
};

/**
 * Command state.
 * state of the last executed command
 * if you like to poll on this state reset the state before with
 * setDebuggerState(commandState::waitForResult)
 */
enum commandState {
    waitForResult,
    debuggerDone,
    debuggerError // GDB/mi error message

};


/**
 * status of the decoded line.
 */
struct decodingStatus {
    decodeKindeOfDecodedLine kind;
    decodeAdditionalInfo info;

    decodingStatus() {
        kind = decodeKindeOfDecodedLine::decodingNotEvaluated;
        info = decodeAdditionalInfo::noteEvaluated;
    }
};

/**
 * Decoding State.
 * this Class represend the state of the Deconding.
 * It contains  as getter the state of:
 *      -   the execution of the last command
 *      -   the current target state (running/stopped)
 * and as definitions the decoding state of the current linke
 */
class decoderState {
    mutex stateMutex;
    mutex commandMutex;
    debuggerState debuggerState1 = debuggerState::debuggerHasStopped;
    commandState lastCommand1 = commandState::waitForResult;

public:
    /**
     * getDebuggerState.
     * the debug state is the currend mode of the Debugger it could be running or stopped.
     * @return courend state
     */
    debuggerState getDebuggerState();

    /**
     * setLastCommand.
     * if the state of the execution has changed external (like execute anther command)
     * you can set this via this method
     * @param set current command state
     */
    void setLastCommand(commandState set);

    /**
     * setDebuggerState.
     * set the debugger state.
     * Use this method if you liek to wait for a new state.
     * @param state
     */
    void setDebuggerState(debuggerState state);

    /**
     * getLastCommand.
     * returns the current command state
     * @return current state
     */
    commandState getLastCommand();

};

std::ostream &operator<<(std::ostream &strm, const debuggerState &a);

std::ostream &operator<<(std::ostream &strm, const decodeKindeOfDecodedLine &a);

std::ostream &operator<<(std::ostream &strm, const commandState &a);

#endif //GDBMANIPULATOR_DECODESTATE_H
