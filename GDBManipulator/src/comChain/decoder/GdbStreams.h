/**
 * Copyright (C) 2020 Intel Corporation
 * SPDX-License-Identifier: Apache-2.0
 * @author: Sebastian Balz
 */


#ifndef GDBMANIPULATOR_GDBSTREAMS_H
#define GDBMANIPULATOR_GDBSTREAMS_H


class gdbStreams {
public:

    /**
     * run in a loop until the pipe has closed or a unexpected error occurs
     */
    virtual void run() = 0;
    virtual ~gdbStreams(){};

};


#endif //GDBMANIPULATOR_GDBSTREAMS_H
