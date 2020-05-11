/**
 * Copyright (C) 2020 Intel Corporation
 * SPDX-License-Identifier: AGPL-3.0-or-later
 * @author: Sebastian Balz
 */


#include <iostream>h
#include "argvParser.h"
#include <csignal>
#include "runModes/AbstractRunMode.h"
#include "ArgumentCallBackFunctions.h"
#include "runModes/TestMode.h"
#include "runModes/FlashMode.h"
#include "runModes/RunMode.h"
#include "comChain/ComArmNoneEabiGdb.h"
#include "main.h"

extern void cleanUpAndExit(int code);

extern bool createStatistic;

void catchCTRLC();
void executeScript(string location,comChain * com);
abstractRunMode *mode;
int main(int argc, char *argv[]) {
    catchCTRLC();

    // logging
    Log::advancedConf()->pintLogSrc(true);
    Log::setLogLevel(Error, DebugL3);
    Log::advancedConf()->pintLogSrc(true);
    Log::setLogFileName(".gdbMann.log");

    // init all parameters
    argvParser *p = initProgramArguments();
    // analyze the given parameters
    if (!p->analyseArgv(argc, argv)) {
        p->printHelpMessage(cliHighlighting);
        return -5;
    }
    logArgumentStatus();
    defineEunitVersions(); // set up eUnit Versions check
    // clean up on  SIGINT SIGABRT SIGTERM

    Log::log("Generate comChain", Info);
    utils::sharedMemoryInit();
    char toShare[] = "false\0";
    utils::sharedMemoryWrite(toShare, 5);
    comChain *com;
    if (runOnTarget) {
        // build gdb arm none eabi chain
        com_arm_none_eabi_gdb::arm_non_eabi_conf conf(elfFile, callServer, callClient, targetResetFunction, gdbPort,
                                                      flashTar, createStatistic);
        com = new com_arm_none_eabi_gdb(conf);
    } else {
        com = new com_gnu_debugger(elfFile, callClient);
    }
    Log::log("Generate testMode", Info);
    // build testMode

        if(executionMode == flashModeSetting){
        	mode = new flashMode(com);
        }
        else if(executionMode == runModeSetting )
        {
        	mode = new runMode(com,testLevel,true);
        }else{
        	mode = new testMode(com, testLevel,enabledTestGroups, disabedTestGroups, xmlFile, true);
        }

    Log::log("Start testMode", Info);
    // run test and wait until all test have been executed or an error occurs
    bool ModeWasNotSuccessFull = !mode->executeMode();
    // generate test result string
    Log::log("Generate result", Info);
    string s = mode->analyseResult();
    Log::log(s, UserInfo, logFilter::GDB_Interface);

    if(gdbScriptAfterExecution != "") {
        Log::log("Run user defined script at " + gdbScriptAfterExecution,Message);
        com->executeCommand("-exec-interrupt");
        executeScript(gdbScriptAfterExecution,com);
    }
    // print statistic
    if (createStatistic) {
        Log::log(testStatistic::generateStatistic(), UserInfo, logFilter::GDB_Interface);
        Log::log("Generate test statistic", Info);
    }
    com->failSilent(true);
  
    if(executionMode == runModeSetting || executionMode == flashModeSetting){
        com->interruptTarget();
        sleep(1);
    	com->detachTarget(false);
    }else{
        com->detachTarget(true);
    }
    usleep(10000);
    // clean exit
    if(ModeWasNotSuccessFull && failByFailedTests )
    	utils::cleanUpAndExit(-1);
    else
    	utils::cleanUpAndExit(0);
    // wait for clean exit
    while (1) {
        sleep(50);
    }
}


/**
 * catch :
 *      - SIGINT
 *      - SIGABRT
 *      - SIGTERM
 *  and make an clean exit (kill all known child's )
 */
void catchCTRLC() {

    signal(SIGINT,utils::cleanUpAndExit);
    signal(SIGABRT,utils::cleanUpAndExit);
    signal(SIGTERM,utils::cleanUpAndExit);
}

void executeScript(string location,comChain * com){
    ifstream *f = new ifstream();
    f->open("gdbScript");
    if(f->is_open()) {
        char s[2048] = "";

        while (f->peek() >= 0) {
            f->getline(s, 2048);
            string c = string(s);
            com->executeCommand(string(s));

        }
       // Log::log("executed gdb script, detach target from gdb",Message);
       // char toShare[] = "false\0";
       // utils::sharedMemoryWrite(toShare, 5); // activate output decoder
       // com->executeCommand(" -target-disconnect");
        Log::log("Script execution done",Message);
    }else{
        Log::log("Failed to open gdb Script at " + location,Error);
    }
}
