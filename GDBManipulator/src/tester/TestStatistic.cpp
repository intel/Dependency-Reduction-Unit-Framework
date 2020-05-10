/**
 * Copyright (C) 2020 Intel Corporation
 * SPDX-License-Identifier: Apache-2.0
 * @author: Sebastian Balz
 */

#include "TestStatistic.h"

/**
    * dependencies of each test Group
    */
struct dependencies {
    int mocks, stubs, deletions;
    string groupName;

    dependencies(string testName, int mock_ = 0, int stub_ = 0, int del_ = 0) : mocks(mock_), stubs(stub_),
                                                                                deletions(del_), groupName(testName) {

    };
};

struct memoryRead {
    std::chrono::steady_clock::time_point timeStampStart;
    std::chrono::steady_clock::time_point timeStampEnd;
    int read, readFrom;
};
bool tarFlashed = false;
std::chrono::steady_clock::time_point timeInGDBTMP;
long timeInGDB;

std::chrono::steady_clock::time_point startFlash_;
std::chrono::steady_clock::time_point endFlash_;
std::chrono::steady_clock::time_point startTime;
std::chrono::steady_clock::time_point endTime;

vector<dependencies *> *dependenciesPerGroup = new vector<dependencies *>();
dependencies *currentDep = new dependencies("");
vector<memoryRead *> *totalMemoryReads = new vector<memoryRead *>();
vector<memoryRead *> *totalMemoryWrites = new vector<memoryRead *>();
memoryRead *currentMemoryReadAction = new memoryRead();
memoryRead *currentMemoryWriteAction = new memoryRead();

int gDBCommands = 0;

void testStatistic::resetStatistic() {
    dependenciesPerGroup = new vector<dependencies *>();
    currentDep = new dependencies("");
    totalMemoryReads = new vector<memoryRead *>();
    totalMemoryWrites = new vector<memoryRead *>();
    currentMemoryReadAction = new memoryRead();
    currentMemoryWriteAction = new memoryRead();
}

void testStatistic::startTesting() {
    startTime = std::chrono::steady_clock::now();
    endTime = startTime; // if endTesting does not get called   or for testing purpose
}


void testStatistic::endTesting() {
    endTime = std::chrono::steady_clock::now();
}

void testStatistic::addMockDel() {
    currentDep->deletions++;
}

void testStatistic::startMemRead() {
    currentMemoryReadAction->timeStampStart = std::chrono::steady_clock::now();
}

void testStatistic::endMemRead(int bufferSize, int readSize) {
    currentMemoryReadAction->timeStampEnd = std::chrono::steady_clock::now();
    currentMemoryReadAction->read = readSize;
    currentMemoryReadAction->readFrom = bufferSize;
    totalMemoryReads->push_back(currentMemoryReadAction);
    currentMemoryReadAction = new memoryRead();

}

void testStatistic::startMemWrite() {
    currentMemoryWriteAction->timeStampStart = std::chrono::steady_clock::now();
}

void testStatistic::endMemWrite(int writeSize) {
    currentMemoryWriteAction->timeStampEnd = std::chrono::steady_clock::now();
    currentMemoryWriteAction->read = writeSize;
    totalMemoryWrites->push_back(currentMemoryWriteAction);

    currentMemoryWriteAction = new memoryRead();
}

void testStatistic::addMock() {
    currentDep->mocks++;
}

void testStatistic::addStub() {
    currentDep->stubs++;
}

void testStatistic::setTestGroupName(string name) {
    if (currentDep->groupName != "") {
        dependenciesPerGroup->push_back(currentDep);
        currentDep = new dependencies(name);
    } else {
        currentDep->groupName = name;
    }
}

string testStatistic::getMemReads() {
    string out = "   all memory reads: ";
    for (unsigned int i = 0; i < totalMemoryReads->size(); i++) {
        memoryRead *r = totalMemoryReads->at(i);
        std::chrono::milliseconds elapsed_time = chrono::duration_cast<chrono::milliseconds>(
                r->timeStampEnd - r->timeStampStart);
        out += "\n\t " + to_string(i) + " : \tread " + to_string(r->read) + " bytes \tfrom " + to_string(r->readFrom) +
               " \t in " + to_string(elapsed_time.count()) + " ms";
    }
    return out;
}


string testStatistic::getMemWrites() {
    string out = "   all memory writes: ";
    for (unsigned int i = 0; i < totalMemoryWrites->size(); i++) {
        memoryRead *r = totalMemoryWrites->at(i);
        std::chrono::milliseconds elapsed_time = chrono::duration_cast<chrono::milliseconds>(
                r->timeStampEnd - r->timeStampStart);
        out += "\n\t " + to_string(i) + " : \twrite " + to_string(r->read) + " bytes \tin " +
               to_string(elapsed_time.count()) + " ms";
    }
    return out;
}

string testStatistic::getMocksAndStubs() {
    if (currentDep->mocks > 0 || currentDep->stubs > 0 || currentDep->deletions > 0 || currentDep->groupName != "") {
        dependenciesPerGroup->push_back(currentDep);
    }
    string out = "   all mock and Stubs per test group: ";
    for (unsigned int i = 0; i < dependenciesPerGroup->size(); i++) {
        dependencies *dep = dependenciesPerGroup->at(i);
        out += "\n\t group: " + dep->groupName + " \t :  Stubs: " + to_string(dep->stubs) + "  Mocks: "
               + to_string(dep->mocks) + "  deletions: " + to_string(dep->deletions);
    }
    return out;
}

string testStatistic::generateStatistic() {
    string out = "##################### \n\n Test Statistic \n\n";
    out += testStatistic::getMemReads() + "\n\n" + testStatistic::getMemWrites() + "\n\n" +
           testStatistic::getMocksAndStubs() + "\n\n";
    std::chrono::milliseconds elapsed_time = chrono::duration_cast<chrono::milliseconds>(endTime - startTime);
    if (tarFlashed) {
        std::chrono::milliseconds elapsed_time = chrono::duration_cast<chrono::milliseconds>(endFlash_ - startFlash_);
        out += "  flashed target in :   \t" + to_string(elapsed_time.count()) + "\n";
    }
    out += "  executed GDB commands :\t" + to_string(gDBCommands) + "\n";
    out += "  total communication time : \t" + to_string(timeInGDB / 1000) + " ms\n";
    out += "  total elapsed time :   \t" + to_string(elapsed_time.count()) + " ms \n";
    out += "#####################\n";
    return out + "\n";
}

void testStatistic::startFlash() {
    tarFlashed = true;
    startFlash_ = std::chrono::steady_clock::now();
    endFlash_ = startFlash_; // if endTesting does not get called   or for testing purpose
}

void testStatistic::endFlash() {
    endFlash_ = std::chrono::steady_clock::now();

}

void testStatistic::addGDBExec() {
    gDBCommands++;
}

void testStatistic::endTransmit() {

    timeInGDB += (chrono::duration_cast<chrono::microseconds>(std::chrono::steady_clock::now() - timeInGDBTMP)).count();
}

void testStatistic::startTransmit() {
    timeInGDBTMP = std::chrono::steady_clock::now();
}

