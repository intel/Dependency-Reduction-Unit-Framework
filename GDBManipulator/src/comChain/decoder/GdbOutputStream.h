/**
 * Copyright (C) 2020 Intel Corporation
 * SPDX-License-Identifier: Apache-2.0
 * @author: Sebastian Balz
 */


#ifndef GDBMANIPULATOR_GDBOUTPUT_H
#define GDBMANIPULATOR_GDBOUTPUT_H


#include <vector>
#include "GdbDecoder.h"
#include "GdbStreams.h"


#include "NameLessPipe.h"

/**
 * GDB-Output Stream.
 * read the gdb's stdOut and decode it
 */
class gdbOutputStream : public gdbStreams {

    vector<stopFrame *> stopFrames;
    gdbDecoder *decoder;
    nameLessPipe *nameLessPipe1;
public:
    explicit gdbOutputStream(gdbDecoder *gdbDecoder, nameLessPipe *nlp) {
        decoder = gdbDecoder;
        nameLessPipe1 = nlp;
    }

    ~gdbOutputStream() {
        delete stdout;
    };

    void run() override;


    bool hasStopFrames();

    bool hasNextValue();

    string getLastValue();

    gdbDecoder *getdecoder();

    stopFrame *getLatestStopFrame();


};


#endif //GDBMANIPULATOR_GDBOUTPUT_H
