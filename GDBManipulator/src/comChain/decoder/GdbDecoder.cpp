/**
 * Copyright (C) 2020 Intel Corporation
 * SPDX-License-Identifier: Apache-2.0
 * @author: Sebastian Balz
 */

#include "logging.h"
#include <cstring>
#include "GdbDecoder.h"
#include "Utils.h"
#include "LogFileFilter.h"

/**
 * current Syncron state
 */
decodeAdditionalInfo gdbDecoder::decodeAsyncMessage(string line, asycnOutput *data) {

    data->hasSignal = false;
    data->hasReason = false;
    data->hasStoppedThreads = false;
    data->hasFrame = false;

    decodeAdditionalInfo info = decodeAdditionalInfo::noteEvaluated;
    while (!line.empty() && line != "\n" && line != "\r" && line != "") {
        if (utils::startsWith(&line, ",") || utils::startsWith(&line, "\"")) {
            line.erase(0, 1);
            //return decodeAsyncMessage(line, data);
        } else if (utils::startsWith(&line, "*stopped")) {
            data->mode = debuggerHasStopped;
            setDebuggerState(debuggerHasStopped);
            line.erase(0, sizeof("*stopped"));
            info = decodeAdditionalInfo::stopped;
        } else if (utils::startsWith(&line, "*running")) {
            data->mode = debuggerIsRunning;
            setDebuggerState(debuggerIsRunning);
            line.erase(0, sizeof("*running"));
            info = decodeAdditionalInfo::running;
        } else if (utils::startsWith(&line, "stopFrame={") || utils::startsWith(&line, "frame")) {
            if (data->stoppedFrame != nullptr)
                delete data->stoppedFrame;
            data->hasFrame = true;
            data->stoppedFrame = decodeFrame(&line);
            if (data->stoppedFrame == nullptr)
                info = decodeAdditionalInfo::failedToEvaluate;
        } else if (utils::startsWith(&line, "thread-id=\"")) {
            line.erase(0, sizeof("thread-id="));
            string idAsString = line.substr(0, line.find('"'));
            line.erase(0, line.find('"') + 2);
            data->threadID = static_cast<int>(strtol(idAsString.c_str(), nullptr, 10));
        } else if (utils::startsWith(&line, "stopped-threads=\"")) {
            line.erase(0, line.find('"') + 1);
            data->hasStoppedThreads = true;
            data->stoppedThreads = line.substr(0, line.find('"'));
            line.erase(0, line.find('"') + 1);
        } else if (utils::startsWith(&line, "reason=\"")) {
            data->hasReason = true;
            line.erase(0, line.find('"') + 1);
            data->reason = line.substr(0, line.find('"'));
            line.erase(0, line.find('"') + 2);
        } else if (utils::startsWith(&line, "signal-name")) {
            data->hasSignal = true;
            line.erase(0, line.find('"') + 1);
            data->signal = line.substr(0, line.find('"'));
            line.erase(0, line.find('"') + 2);
        } else if (utils::startsWith(&line, "signal-mean")) {
            line.erase(0, line.find('"') + 1);
            data->signalMeaning = line.substr(0, line.find('"'));
            line.erase(0, line.find('"') + 2);
        } else if ((utils::startsWith(&line, "disp"))) {
            line.erase(0, line.find('"') + 2);
            line.erase(0, line.find('"') + 2);
        } else if ((utils::startsWith(&line, "bkptno"))) {
            data->isBreakpoint = true;
            line.erase(0, line.find('"') + 4);
        } else if ((utils::startsWith(&line, "core"))) {
            line.erase(0, line.find('"') + 4);
        }
            // ERROR STATE
        else {
            if (utils::startsWith(&line, ",")) {
                return decodeAsyncMessage(line, data);
            } else {
                Log::log("Decoding testMode message parser has reached an undefined state : [" + line + "]", CriticError,
                         logFilter::gdbMI2decoder);
                info = decodeAdditionalInfo::failedToEvaluate;

            }

        }


    }
    return info;
}

/**
 * Status
 * ^done
 * ^done,threads=[{id="1",target-id="Thread <main>",frame={level="0",addr="0x080008e5",func="??",args=[]},state="stopped"}],current-thread-id="1"
 * ^done,bkpt={number="1",type="breakpoint",disp="keep",enabled="y",addr="0x08000844",func="main",file="../src/main.c",fullname="/home/basto/Atollic/workspace/EmbeddedTesting/EmbeddedTesting/src/main.c",line="67",thread-groups=["i1"],times="0",original-location="main"}

 *
 * "^done,value=\"{testStatus = testFailed, testType = typeCompInt32, sizeShould = 4, sizeIs = 4, should = 0x2001ff80, is = 0x2001ff84, line = 39}\""
 *
 * ^error,msg="Undefined MI command: xx",code="undefined-command"
 * ^running
 */
decodeAdditionalInfo gdbDecoder::decodeSyncMessage(string line, stopFrame **frame) {

    decodeAdditionalInfo info;
    debuggerState gdbState = getDebuggerState();
    commandState lastCommand;
    // running, stopped
    if (utils::startsWith(&line, "^running")) {
        setDebuggerState(debuggerState::debuggerIsRunning);
        info = decodeAdditionalInfo::running;
        lastCommand = commandState::debuggerDone;
        gdbState = debuggerState::debuggerIsRunning;
    } else if (utils::startsWith(&line, "^done")) {
        info = evaluateDone(&line, frame);
        lastCommand = commandState::debuggerDone;
        gdbState = debuggerState::debuggerHasStopped;

    } else if (utils::startsWith(&line, "^stopped")) {
        setDebuggerState(debuggerState::debuggerHasStopped);
        lastCommand = commandState::debuggerDone;
        info = decodeAdditionalInfo::stopped;
        gdbState = debuggerState::debuggerHasStopped;
    } else if (utils::startsWith(&line, "^error")) {
    	   string s = string(utils::sharedMemoryRead());
			if (!utils::startsWith(&s, "true")) {
				Log::log("got gdb Error message : " + line, Error, logFilter::GDB_Output_Streams);
				info = decodeAdditionalInfo::error;
				lastCommand = commandState::debuggerError;
			}
    } else {
        lastCommand = commandState::debuggerError;
        info = decodeAdditionalInfo::failedToEvaluate;
        Log::log("Failed to decode \"SyncMessage\"", Error);
    }

    setLastCommand(lastCommand);
    setDebuggerState(gdbState);
    return info;
}


/**
 * sync : mem,
 * @param line
 * @param data
 * @return
 */
decodingStatus gdbDecoder::analyseLine(string line, asycnOutput *data) {
    decodingStatus status;
    if (line != "") {
        // remove \r on windows systems
#ifdef _WIN32
        if (line.at(line.length() - 1) == '\r') {
            line = line.substr(0, line.length() - 1);
        }
#endif

        data->hasFrame = false;
        data->hasStoppedThreads = false;
        // async Message
        if (line.at(0) == '*') {
            Log::log("AnalyseLine detect ansync output", DebugL3, logFilter::gdbMI2decoder);
            status.kind = decodeKindeOfDecodedLine::decodedAsyncOut;
            status.info = decodeAsyncMessage(line, data);
        }
            // Sync Message
        else if (line.at(0) == '^') {
            Log::log("AnalyseLine detect sync output", DebugL3, logFilter::gdbMI2decoder);
            status.kind = decodeKindeOfDecodedLine::decodeSyncronOut;
            // data->stoppedFrame = new stopFrame();
            status.info = decodeSyncMessage(line, &data->stoppedFrame);
            data->hasFrame = data->stoppedFrame != nullptr;
        }
            // Sync echo
        else if (line.at(0) == '&') { // echo command
            status.kind = decodeKindeOfDecodedLine::decodedMessage;
            Log::log("AnalyseLine detect Echo output", DebugL3, logFilter::gdbMI2decoder);
            if (line.find("No such file or directory") != std::string::npos) {
                Log::log("No Such File : " + line, CriticError);
                utils::cleanUpAndExit(-4);
            } else if (line.find("Remote connection closed") != string::npos) {
                Log::log("GDBClient has lost connection to GDBServer", CriticError);
                utils::cleanUpAndExit(-4);
            }
        }
            // Sync record
        else if (line.at(0) == '~') { // stream record
            status.kind = decodeKindeOfDecodedLine::decodedMessage;
            Log::log("AnalyseLine detect stream record -> ignore it", DebugL3, logFilter::gdbMI2decoder);
        } else if (line.at(0) == '=') {
            status.kind = decodeKindeOfDecodedLine::decodedMessage;
            Log::log("AnalyseLine detect stream activity (=) -> ignore it", DebugL3, logFilter::gdbMI2decoder);
        } else if (utils::startsWith(&line, "(testMode)")) {
            status.kind = decodeKindeOfDecodedLine::decodedMessage;
            Log::log("AnalyseLine detect testMode std output (testMode)", DebugL3, logFilter::gdbMI2decoder);
        } else if (utils::startsWith(&line, "+")) {
            status.kind = decodeKindeOfDecodedLine::decodedMessage;
            Log::log("AnalyseLine detect testMode std output (testMode)", DebugL3, logFilter::gdbMI2decoder);
        } else if (utils::startsWith(&line, "@")) {
            status.kind = decodeKindeOfDecodedLine::decodedMessage;
        } else if (utils::startsWith(&line, "(gdb)")) {
            status.kind = decodeKindeOfDecodedLine::decodedMessage;
        } else {
            // check if come should be closed to prevent external error messages
            string s = string(utils::sharedMemoryRead());
            if (!utils::startsWith(&s, "true")) {
                Log::log("AnalyseLine : undefined Message : [" + line + "]", Error,
                         logFilter::gdbMI2decoder);
            }else{
                sleep(1);
                Log::log("Close communication because of undefined communication state. See Error log for further informations",CriticError);
                utils::cleanUpAndExit(-11);
            }
        }
        if (status.info == decodeAdditionalInfo::failedToEvaluate) {
            Log::log("Failed to decode line \"" + line+"\"", CriticError);
        }
    }
    return status;
}


/**
 * ^done,value="94 '^'"
 * ^done,memory=[{begin="0x20000104",offset="0x00000000",end="0x20000162",contents="00..0"}]
 * ^done
 *
 * "^done,value=\"{testStatus = testFailed, testType = typeCompInt32, sizeShould = 4, sizeIs = 4, should = 0x2001ff80, is = 0x2001ff84, line = 39}\""
 */


decodeAdditionalInfo gdbDecoder::evaluateDone(string *line, stopFrame **frame) {
    Log::log("evaluate Done Line ", DebugL2);
    decodeAdditionalInfo info = decodeAdditionalInfo::noteEvaluated;
    memoryDump *dump;
    if (utils::startsWith(line, "^done")) {
        info = decodeAdditionalInfo::done;
        line->erase(0, sizeof("^done"));
        while (!line->empty() && *line != "\n" && *line != "\r") {
            if (utils::startsWith(line, "value")) {
                info = decodeValue(line);
            } else if (utils::startsWith(line, "memory")) {
                dump = new memoryDump(line);
                if (dump->decodeDump()) {
                    info = decodeAdditionalInfo::doneWithMemory;
                    storedMemoryDumps.push_back(dump);
                } else {
                    info = decodeAdditionalInfo::failedToDecodeMem;
                    delete dump;
                }
                line->erase(0, line->find(']') + 1);

            } else if (utils::startsWith(line, "bkpt")) {
                info = decodeAdditionalInfo::done; // nothing to do hear
                line->erase(0, line->find('}') + 1);
            } else if (utils::startsWith(line, "frame")) {
                info = decodeAdditionalInfo::stopped; // nothing to do hear
                *frame = decodeFrame(line);
                setLastCommand(commandState::debuggerDone);

            } else {
                string s = string(utils::sharedMemoryRead());
                if (!utils::startsWith(&s, "true")) {
                    Log::log("evaluateDone : unknown ^done :" + *line, CriticError, logFilter::gdbMI2decoder);
                    info = decodeAdditionalInfo::failedToEvaluate;
                    return info;
                }
            }
        }
    }
    return info;

}

memoryDump *gdbDecoder::getNextDump() {
    Log::log("getNextDump ", DebugL2, logFilter::gdbMI2decoder);
    memoryDump *d = nullptr;
    if (hasNextDump()) {
        d = storedMemoryDumps.front();
        storedMemoryDumps.erase(storedMemoryDumps.begin());
    } else {
        Log::log("tried to get mem dump but there was none", Error, logFilter::gdbMI2decoder);
    }
    return d;
}

bool gdbDecoder::hasNextDump() {
    return !storedMemoryDumps.empty();
}

std::ostream &operator<<(std::ostream &strm, const asycnOutput &a) {
    strm << "async-output : { ";
    strm << " current mode : " << a.mode << " ";
    if (a.hasReason)
        strm << " reason: " + a.reason << " ";
    if (a.hasSignal)
        strm << " signal : " + a.signal << " ";
    if (a.hasFrame)
        strm << *a.stoppedFrame << " ";
    if (a.hasStoppedThreads)
        strm << " stopped : " << a.stoppedThreads << " ";
    return strm << " }";
}


