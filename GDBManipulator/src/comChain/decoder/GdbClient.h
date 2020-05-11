/**
 * Copyright (C) 2020 Intel Corporation
 * SPDX-License-Identifier: AGPL-3.0-or-later
 * @author: Sebastian Balz
 */

#ifndef GDBMANIPULATOR_GDBDECODERCONTAINER_H
#define GDBMANIPULATOR_GDBDECODERCONTAINER_H


#include "GdbDecoder.h"
#include "GdbErrorStream.h"
#include "GdbOutputStream.h"
#include "tester/TestAnalyser.h"


/**
 * this class has the goal to create all entity's for one gdb Client
 */
class gdbClient {

    /**
     * shared decoder
     */
    gdbDecoder *decode;
    gdbErrorStream *errorStream;
    gdbOutputStream *outStream;
    gdbDecoder *decoder;

    string stdOutLoc;


    string stdErrLoc;
    nameLessPipe *nlp;
    string xmlOutFile;
    bool flashAtBoot;
    int gdbPort;

public:
    gdbClient(gdbDecoder *decoder_);

    ~gdbClient();

    gdbErrorStream *getErrorStream() const;

    gdbOutputStream *getOutStream() const;


    /**
     * returns the connection to GDB-Clients STD_Out/Err/In
     */
    nameLessPipe *getNameLessPipe();


    /**
     * start the gdb client with redirected stdErr/IN/Out and connect
     * pipeReader/Writer to those pipes
     * @return success
     */
    bool open();


};


#endif //GDBMANIPULATOR_GDBDECODERCONTAINER_H
