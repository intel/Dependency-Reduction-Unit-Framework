/**
 * Copyright (C) 2020 Intel Corporation
 * SPDX-License-Identifier: Apache-2.0
 * @author: Sebastian Balz
 */

#include "MemoryDump.h"

#include "logging.h"
#include "LogFileFilter.h"


bool memoryDump::decodeDump() {
    Log::log("Decode decodeDump", DebugL3);
    if (!decoded) {
        // decode addr range
        string startHex, endHex, offsetHex;
        long start, end;
        decLine->erase(0, decLine->find('"') + 1);
        startHex = decLine->substr(0, decLine->find('"'));
        decLine->erase(0, decLine->find("offset"));
        decLine->erase(0, decLine->find('"') + 1);
        offsetHex = decLine->substr(0, decLine->find('"'));
        decLine->erase(0, decLine->find("end"));
        decLine->erase(0, decLine->find('"') + 1);
        endHex = decLine->substr(0, decLine->find('"'));
        decLine->erase(0, decLine->find("contents"));
        decLine->erase(0, decLine->find('"') + 1);


        start = strtoul(startHex.c_str(), nullptr, 16);
        start += strtoul(offsetHex.c_str(), nullptr, 16);
        end = strtoul(endHex.c_str(), nullptr, 16);
        Log::log("Read memory dump from " + startHex + " to " + endHex, DebugL2, logFilter::gdbMI2decoder);
        sizeOfDump = (unsigned int) (end - start);
        if (decLine->length() != sizeOfDump * 2 + 3) {
            Log::log("Size of memory dump does not match with with given size", Error,
                     logFilter::gdbMI2decoder);
            return false;
        }
        data = new uint8_t[sizeOfDump];
        // bytes are stored as asci --> no memcpy
        for (unsigned int i = 0; i < sizeOfDump; i++) {
            string byte = decLine->substr(i * 2, 2);
            data[i] = (uint8_t) strtoul(byte.c_str(), nullptr, 16);

        }
        decoded = true;
    }
    return true;
}


memoryDump::memoryDump(string *decodedLine) {
    decLine = decodedLine;
}

uint8_t *memoryDump::getDump() {
    return data;
}

memoryDump::~memoryDump() {

    delete[] data;
}

int memoryDump::getSize() {
    return sizeOfDump;
}

void memoryDump::injectDump(uint8_t *injectData_) {
    data = injectData_;

}
