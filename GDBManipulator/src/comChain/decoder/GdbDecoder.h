/**
 * Copyright (C) 2020 Intel Corporation
 * SPDX-License-Identifier: Apache-2.0
 * @author: Sebastian Balz
 */


#ifndef GDBMANIPULATOR_GDBSTATES_H
#define GDBMANIPULATOR_GDBSTATES_H

#include <iostream>
#include <vector>
#include <mutex>

#include "DecodeState.h"
#include "FrameDecoder.h"
#include "tester/MemoryDump.h"
#include "ValueDecoder.h"


using namespace std;


/**
 * asycnOutput.
 * elements of an async message output
 * not each element is used in a async output
 */
typedef struct asycnOutput {
    debuggerState mode = debuggerState::debuggerStateResetet;
    bool hasReason = false;
    string reason = "";
    bool hasSignal = false;
    string signal = "";
    string signalMeaning = "";
    int threadID;
    bool hasFrame = false;
    stopFrame *stoppedFrame = nullptr;
    bool hasStoppedThreads = false;
    string stoppedThreads = "";
    bool isBreakpoint = false;

    ~asycnOutput() { delete stoppedFrame; }

} asycnOutput;


/**
 * gdb Decode.
 * this class is a gateway to methods which decodes the gdb output(stdout) and keep  track of
 * the gdb states.
 *
 */
class gdbDecoder : public valueDecoder, public frameDecoder, public decoderState {

    vector<memoryDump *> storedMemoryDumps;

    /**
     * decodeSyncMessage.
     * decode the syncron output
     * @param line line to analyse
     * @return decoded status
     */
    decodeAdditionalInfo decodeSyncMessage(string line, stopFrame **frame);

    /**
     * evaluateDone.
     * decoding the Sycronus ^done result
     * @param line  line to decode
     * @return  decodeAdditionalInfo
     */
    decodeAdditionalInfo evaluateDone(string *line, stopFrame **frame);


    /**
     * decodeAsyncMessage.
     * decode the async Output
     * @param line line to decode
     * @param data pointer to a initiated struct where the data gets stored
     * @return true on success
     */
    decodeAdditionalInfo decodeAsyncMessage(string line, asycnOutput *data);

public:

    ~gdbDecoder() {
        //TODO
        // for(int i = 0; i<storedMemoryDumps.size();i++){
        //     delete storedMemoryDumps.at(i);
        // }
        storedMemoryDumps.clear();
    }

    /**
     * Analyse one Line.
     * Decode the output of the gdb/mi
     * it expected a complete line from the gdb/mi interface
     * @param line data line from the gdb
     * @param data struct where the ansync messages can get stored. If the message is not a async message the struct does not get changed
     * @return analysed state
     */
    decodingStatus analyseLine(string line, asycnOutput *data);

    /**
     * get the next dump.
     * return the next memory dump if one does exist
     * the dump belongs now to you
     * @return mem dump
     */
    memoryDump *getNextDump();

    /**
     * hasNextDump.
     * are there some dumps
     * @return true if there are some dumps
     */
    bool hasNextDump();

};

std::ostream &operator<<(std::ostream &strm, const asycnOutput &a);

std::ostream &operator<<(std::ostream &strm, const debuggerState &a);


#endif //GDBMANIPULATOR_GDBSTATES_H
