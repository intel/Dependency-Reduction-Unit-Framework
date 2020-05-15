/**
 * Copyright (C) 2020 Intel Corporation
 * SPDX-License-Identifier: AGPL-3.0-or-later
 * @author: Sebastian Balz
 */


#include "ValueDecoder.h"

#include "logging.h"
#include "LogFileFilter.h"
#include "Utils.h"

decodeAdditionalInfo valueDecoder::decodeValue(string *line) {
    if (!utils::startsWith(line, "value")) {
        Log::log("Decoding additional info failed to decode value \"" + *line+"\"", Error, logFilter::gdbMI2decoder);
        return decodeAdditionalInfo::failedToEvaluate;
    }
    line->erase(0, line->find('"') + 1);
    string value = line->substr(0, line->find('\"'));
    string value1 = line->substr(0, line->find('\'') - 1);
    if (value1.size() < value.size())
        value = value1;
    valueMutex.lock();
    storedValues.push_back(value);
    valueMutex.unlock();
    line->erase(0, line->find('"') + 1);
    return decodeAdditionalInfo::doneWithValue;

}

bool valueDecoder::hasNextValue() {
    valueMutex.lock();
    bool tmp = !storedValues.empty();
    valueMutex.unlock();
    return tmp;

}

string valueDecoder::getLatestValue() {
    string l;

    if (hasNextValue()) {
        valueMutex.lock();
        l = storedValues.front();
        storedValues.erase(storedValues.begin());
        valueMutex.unlock();
    } else
        Log::log("getLatestValue() has been called but has no values", Error, logFilter::gdbMI2decoder);
    return l;
}
