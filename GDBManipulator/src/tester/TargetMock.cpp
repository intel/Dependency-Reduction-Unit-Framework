/**
 * Copyright (C) 2020 Intel Corporation
 * SPDX-License-Identifier: Apache-2.0
 * @author: Sebastian Balz
 */

#include "TargetMock.h"

bool targetMock::hasMock(long int addrWhen) {
    std::stringstream stream;
    stream << std::hex << addrWhen;
    std::string result(stream.str());
    return hasMock(result);

}

string targetMock::getThen(string when_) {
    for (unsigned int i = 0; i < toMock->size(); i++) {
        if (toMock->at(i)->when == when_)
            return toMock->at(i)->then;
    }
    return "";
}

bool targetMock::hasMocks() {
    return toMock->size() > 0;
}

void targetMock::deleteAllMocks() {
    delete toMock;
    toMock = new vector<mock *>();
}

string targetMock::getThen(long whenadd_) {
    std::stringstream stream;
    stream << std::hex << whenadd_;
    std::string result(stream.str());
    return getThen(result);
}

bool targetMock::hasMock(string when_) {
    for (unsigned int i = 0; i < toMock->size(); i++) {
        if (toMock->at(i)->when == when_)
            return true;
    }
    return false;
}

void targetMock::addMock(string when, string then) {

    mock *m = new mock(when, then);
    toMock->push_back(m);

}
