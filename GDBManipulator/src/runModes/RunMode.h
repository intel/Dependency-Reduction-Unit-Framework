/**
 * Copyright (C) 2020 Intel Corporation
 * SPDX-License-Identifier: Apache-2.0
 * @author: Sebastian Balz
 */


#ifndef SRC_RUNMODE_H_
#define SRC_RUNMODE_H_

#include "comChain/ComChain.h"
#include "TestMode.h"


class runMode : public testMode{

protected:
	void gdbMessagesToDebugger(stopFrame *frame);

    /**
     * execute all test until the function "message to Debuger "called with the value finished all test
     * und evaluate all test results
     */
    void executeAllTest();

public:
	runMode(comChain *com, amountOfInfo testSpeed,bool printTolCli = true) : testMode(com,testSpeed,"","","",printTolCli)
	{

    };

    bool executeMode();
};


#endif /* SRC_RUNMODE_H_ */
