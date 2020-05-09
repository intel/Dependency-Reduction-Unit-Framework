/**
 * Copyright (C) 2020 Intel Corporation
 * SPDX-License-Identifier: Apache-2.0
 * @author: Sebastian Balz
 */


#ifndef GDBMANIPULATOR_VALUEDECODER_H
#define GDBMANIPULATOR_VALUEDECODER_H


#include <string>
#include <vector>
#include <mutex>

#include "DecodeState.h"


using namespace std;

/**
 * Value Decoder.
 * This class decodes Requested values from e.g an variable.
 *
 */
class valueDecoder {
    vector<string> storedValues;
    mutex valueMutex;

protected:
    /**
     * decode Value.
     * decode the value part of the line and store it in an buffer.
     * @param line line to evaluate, expect that the line starts with <value>
     * @return additional decoding info
     */
    decodeAdditionalInfo decodeValue(string *line);

public:
    /**
     * hasNextValue.
     * check if values have been stored
     * @return bool if there values not jet requested
     */
    bool hasNextValue();

    /**
     * getLatestValue.
     * return the last recently seen value and remote it from the buffer
     * @return
     */
    string getLatestValue();
};


#endif //GDBMANIPULATOR_VALUEDECODER_H
