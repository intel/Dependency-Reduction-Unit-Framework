/**
 * Copyright (C) 2020 Intel Corporation
 * SPDX-License-Identifier: AGPL-3.0-or-later
 * @author: Sebastian Balz
 */

#ifndef GDBMANIPULATOR_TESTSTATISTIC_H
#define GDBMANIPULATOR_TESTSTATISTIC_H

#include <chrono>
#include <string>
#include <vector>

using namespace std;

class testStatistic {


public:

    static void resetStatistic();

    static void startTesting();

    static void endTesting();

    static void addMockDel();

    static void startMemRead();

    static void endMemRead(int bufferSize, int readSize);

    static void startMemWrite();

    static void endMemWrite(int writeSize);

    static void addMock();

    static void addStub();

    static void setTestGroupName(string name);

    static void startFlash();

    static void endFlash();

    static void startTransmit();

    static void endTransmit();

    static void addGDBExec();

    static string getMemReads();

    static string getMemWrites();

    static string getMocksAndStubs();

    static string generateStatistic();
};


#endif //GDBMANIPULATOR_TESTSTATISTIC_H
