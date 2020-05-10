/**
 * Copyright (C) 2020 Intel Corporation
 * SPDX-License-Identifier: Apache-2.0
 * @author: Sebastian Balz
 */


#ifndef GDBMANIPULATOR_LOGFILEFILTER_H
#define GDBMANIPULATOR_LOGFILEFILTER_H


/**
 * points of interest for SimpleLogging
 */
namespace logFilter {
    const unsigned int GDB_Server = 1 << 0;
    const unsigned int GDB_Client = 1 << 1;
    const unsigned int Test_Analyser = 1 << 2;
    const unsigned int GDB_Input_Streams = 1 << 4;
    const unsigned int GDB_Output_Streams = 1 << 5;
    const unsigned int GDB_Error_Streams = 1 << 6;
    const unsigned int gdbMI2decoder = 1 << 7;
    const unsigned int GDB_Interface = 1 << 8;
    const unsigned int COM_CHAIN = 1 << 9;


    namespace configuredFilter {
        const unsigned int GDB_Steams = GDB_Input_Streams | GDB_Output_Streams | GDB_Error_Streams;
        const unsigned int GDB_ALL = GDB_Server | GDB_Client | GDB_Steams | GDB_Interface | gdbMI2decoder;
        const unsigned int Test_Analyser = 1 << 2;
    }
};

#endif //GDBMANIPULATOR_LOGFILEFILTER_H
