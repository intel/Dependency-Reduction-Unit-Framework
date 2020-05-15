/**
 * Copyright (C) 2020 Intel Corporation
 * SPDX-License-Identifier: AGPL-3.0-or-later
 * @author: Sebastian Balz
 */

#ifndef GDBMANIPULATOR_TARGETMOCK_H
#define GDBMANIPULATOR_TARGETMOCK_H


#include <string>
#include <sstream>
#include <vector>
#include "logging.h"


using namespace std;

class targetMock {

    typedef struct mock {
        string when;
        string then;

        mock(string when_, string then_) : when(string(when_)), then(string(then_)) {};
    } mock;
    const string addBreak = "-break-insert "; // + function
    const string returnVal = "-exec-return "; // + value

    vector<mock *> *toMock;

public:
    targetMock() {
        toMock = new vector<mock *>();

    }

    void addMock(string when, string then);

    bool hasMock(long int addrWhen);

    bool hasMock(string when_);

    bool hasMocks();

    string getThen(long whenadd_);

    string getThen(string when_);

    void deleteAllMocks();

};


#endif //GDBMANIPULATOR_TARGETMOCK_H
