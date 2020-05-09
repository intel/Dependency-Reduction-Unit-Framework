/**
 * Copyright (C) 2020 Intel Corporation
 * SPDX-License-Identifier: Apache-2.0
 * @author: Sebastian Balz
 */


#ifndef GDBMANIPULATOR_GDBERRORSTREAM_H
#define GDBMANIPULATOR_GDBERRORSTREAM_H


#include "GdbDecoder.h"
#include "GdbStreams.h"


#include "NameLessPipe.h"

/**
 * GDB-Error Output.
 * reads the gdb's error stream and print it as error
 */
class gdbErrorStream {

    nameLessPipe *_stderr;
    gdbDecoder *decode;
public:
    explicit gdbErrorStream(gdbDecoder *gdbDecoder, nameLessPipe *stderr_) {
        _stderr = stderr_;
        decode = gdbDecoder;
    };

    void run();

};


#endif //GDBMANIPULATOR_GDBERRORSTREAM_H
