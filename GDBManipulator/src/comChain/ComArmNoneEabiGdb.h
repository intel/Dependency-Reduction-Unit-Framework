/**
 * Copyright (C) 2020 Intel Corporation
 * SPDX-License-Identifier: Apache-2.0
 * @author: Sebastian Balz
 */


#ifndef GDBMANIPULATOR_COM_GNUDEBUGGER_H
#define GDBMANIPULATOR_COM_GNUDEBUGGER_H


#include "comChain/ComChain.h"
#include "LogFileFilter.h"
#include "comChain/decoder/NameLessPipe.h"

#include "comChain/ComGnuDebugger.h"


class com_arm_none_eabi_gdb : public com_gnu_debugger {
public:
    typedef struct arm_non_eabi_conf {
        int gdb_serverPort;
        bool flash;
        string resetHandler = "Reset_Handler";
        bool createTestStatistic;
        string elfFileLoc;
        string gdbServer;
        string gdbClient;

        arm_non_eabi_conf(string elfFileLoc, string gdbServer = "", string gdbClient = "",
                          string resetHandler = "Reset_Handler",
                          int gdb_serverPort = 61234, bool flash = true, bool createTestStatistic = true) :
                gdb_serverPort(gdb_serverPort), flash(flash), resetHandler(resetHandler),
                createTestStatistic(createTestStatistic),
                elfFileLoc(elfFileLoc), gdbServer(gdbServer), gdbClient(gdbClient) {};
    } arm_non_eabi_conf;

private:

    bool flashTarget();

    arm_non_eabi_conf conf;
    // remote port
    string connectToTarget = "tar extended-remote:";
    // continue target execution
    string cont = "-exec-continue";
    // jump over  software breakpoint
    string contAfterBreak = "set $pc +=2";
public:


    explicit com_arm_none_eabi_gdb(arm_non_eabi_conf conf) :
            com_gnu_debugger(conf.elfFileLoc, conf.gdbClient, conf.gdbServer, conf.gdb_serverPort),
            conf(conf) {
        Log::log("create com-chain com_arm_none_eabi_gdb", Info);
    };

    bool initSystem() override;

    void continueAfterMessage(bool waitUntilNextBreakpoint) override;
    void continueAfterReturnFromFunction(bool waitUntilNextBreakpoint) override;
    bool startTestMode();
};

#endif //GDBMANIPULATOR_COM_GNUDEBUGGER_H
