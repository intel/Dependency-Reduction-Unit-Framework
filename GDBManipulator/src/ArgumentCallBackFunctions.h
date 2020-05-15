/**
 * Copyright (C) 2020 Intel Corporation
 * SPDX-License-Identifier: AGPL-3.0-or-later
 * @author: Sebastian Balz
 */


#ifndef GDBMANIPULATOR_ARGUMENTCALLBACKFUNCTIONS_H
#define GDBMANIPULATOR_ARGUMENTCALLBACKFUNCTIONS_H


#include "argvParser.h"
#include "tester/TestAnalyser.h"

extern string elfFile;
extern string callServer;
extern string callClient;
extern string xmlFile;
extern int gdbPort;
extern bool flashTar;
extern amountOfInfo testLevel;
extern bool runOnTarget;
extern bool cliHighlighting;
extern string gdbScriptAfterExecution;
extern string disabedTestGroups;
extern string disabedTestGroups;
extern string enabledTestGroups;
enum runModeType{
    testModeSetting,
	runModeSetting,
	flashModeSetting
};
extern runModeType executionMode;
extern bool failByFailedTests;


/**
 * define all program arguments.
 */
argvParser *initProgramArguments();
/**
 * log the status of all arguments
 */
void logArgumentStatus();


std::ostream &operator<<(std::ostream &strm, const runModeType &a);



#endif //GDBMANIPULATOR_ARGUMENTCALLBACKFUNCTIONS_H
