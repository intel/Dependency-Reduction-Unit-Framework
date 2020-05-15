/**
 * Copyright (C) 2020 Intel Corporation
 * SPDX-License-Identifier: AGPL-3.0-or-later
 * @author: Sebastian Balz
 */

#ifndef GDBMANIPULATOR_STOPFRAME_H
#define GDBMANIPULATOR_STOPFRAME_H

#include <string>
#include <vector>

using namespace std;


/*!
 * Function Arguments.
 * Each argument has ha name can have value and/or an meaning
 */
struct functionArgument {
    string name;        /// argument name
    string value;       /// armument value
    string meaning;     /// argument meaning
};

/**
 * Stop-Frame.
 * the stop frame is position where the execution of the target code has stopped.
 */
struct stopFrame {
    unsigned long addr;  /// address of the stopped function
    string function;       /// name of the stopped function
    vector<functionArgument *> args; /// list of arguments
    ~stopFrame() {
        for (unsigned int i = 0; i < args.size(); i++) {
            delete args.at(i);
        }
        args.clear();

    };

};


#endif //GDBMANIPULATOR_STOPFRAME_H
