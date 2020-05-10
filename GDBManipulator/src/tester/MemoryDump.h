/**
 * Copyright (C) 2020 Intel Corporation
 * SPDX-License-Identifier: Apache-2.0
 * @author: Sebastian Balz
 */


#ifndef GDBMANIPULATOR_MEMORYDUMP_H
#define GDBMANIPULATOR_MEMORYDUMP_H


#include <string>

using namespace std;

/**
 * Memory Dumps.
 * this class stores memory dump from the target e.g from the result buffer.
 */
class memoryDump {


    unsigned int sizeOfDump;
    uint8_t *data = nullptr;
    string *decLine;
    bool decoded = false;

    /**
     * inject a memory dump.
     * just just for Testing!!
     */
    void injectDump(uint8_t *injectData_);

public:
    /**
     * memory dump received by gdb
     */
    memoryDump(string *decodedLine);

    /**
     * memory dump already as row data available
     * @param data  row data
     */
    memoryDump(uint8_t *data) : data(data) {
        decoded = true;
    }

    /**
     * get Dump.
     * get the decoded data bytes
     * is empty if not jet correctly decoded it returns null
     * @return dump array or null
     */
    virtual uint8_t *getDump();


    /**
     * decode the dump.
     * decode the dump message and store the data in an buffer.
     * once decoded further decoding do not have affect.
     * @return true on success or true if already decoded
     */
    virtual bool decodeDump();

    virtual ~memoryDump();

    /**
     * get size.
     * returns the size of the decoded frame
     * @return
     */
    virtual int getSize();


};


#endif //GDBMANIPULATOR_MEMORYDUMP_H
