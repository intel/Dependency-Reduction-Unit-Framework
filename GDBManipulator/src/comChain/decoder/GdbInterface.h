/**
 * Copyright (C) 2020 Intel Corporation
 * SPDX-License-Identifier: AGPL-3.0-or-later
 * @author: Sebastian Balz
 */


#ifndef GDBMANIPULATOR_GDBINTERFACE_H
#define GDBMANIPULATOR_GDBINTERFACE_H

#include "GdbClient.h"


/**
 * this class hat the goal to run the gdb-server and gdb-Client
 */
class gdbInterface {

    gdbClient *client;
    string elfFileLoc;
    string gdbLoc;
    int gdbPort;
public:


    /**
     * starts the Gdb-Server.
     * @param elfFileLoc_ location of the elf file
     * @param gdbLoc_ command to call the gdb client
     */
    gdbInterface(string elfFileLoc_, string gdbLoc_, string gdbServer,
                 int gdb_Port_ = 61234, gdbDecoder *decoder = new gdbDecoder());

    /**
     * start the gdb client with redirected stdErr/IN/Out and connect
     * pipeReader/Writer to those pipes
     * @return success
     */
    bool open();

    ~gdbInterface() {
        delete client;
    }

    gdbClient *getClient();;


};


#endif //GDBMANIPULATOR_GDBINTERFACE_H
