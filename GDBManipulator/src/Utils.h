/**
 * Copyright (C) 2020 Intel Corporation
 * SPDX-License-Identifier: Apache-2.0
 * @author: Sebastian Balz
 */

#ifndef GDBMANIPULATOR_UTILS_H
#define GDBMANIPULATOR_UTILS_H

#include <string>

#include "comChain/decoder/NameLessPipe.h"

using namespace std;

/**
 * helper functions.
 * this function are used over the whole project, or have the goal the reduce the dependence to other
 * library(to reduce the programms food print)
 */
class utils {
public:
    /**
        * check if the string "start" starts with "with".
        * replace the boost::stats_with()
        * @param start check if THIS string starts with "with"
        * @param with check that the "start" string starts with THIS string
        * @return the "start" string starts with "with" == TRUE
        */
    static bool startsWith(string *start, string with);

    static void cleanUpAndExit(int code);;
    static void cleanUpAndExit(int code, int additionalInfo);

    static void addChildProzess(processHander pid);

    static void deleteChildProcess(processHander pid);

    static void sharedMemoryInit();

    static void sharedMemoryWrite(char *buff, int size);

    static char *sharedMemoryRead();

    static void killProcess(processHander pid);
    static void failSave();

    static void alarmThread(int code);
};


#endif //GDBMANIPULATOR_UTILS_H
