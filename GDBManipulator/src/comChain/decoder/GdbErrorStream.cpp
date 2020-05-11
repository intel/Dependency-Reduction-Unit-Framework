/**
 * Copyright (C) 2020 Intel Corporation
 * SPDX-License-Identifier: AGPL-3.0-or-later
 * @author: Sebastian Balz
 */

#include "GdbErrorStream.h"

#include "LogFileFilter.h"
#include "Utils.h"

void gdbErrorStream::run() {
    usleep(100);
    while (true) {
        string s = _stderr->readFromSTD_ERR_Line();
        if (s == "") {
            Log::log("Stderr pipe closed", Error, logFilter::GDB_Error_Streams);
            return;
        }
        Log::log("HandleStdERR: got Error :" + s, Error, logFilter::GDB_Error_Streams);
        utils::cleanUpAndExit(-13);
    }
}
