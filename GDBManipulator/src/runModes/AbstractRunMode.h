/**
 * Copyright (C) 2020 Intel Corporation
 * SPDX-License-Identifier: AGPL-3.0-or-later
 * @author: Sebastian Balz
 */

#ifndef GDBMANIPULATOR_RUNMODE_H
#define GDBMANIPULATOR_RUNMODE_H

#include "comChain/ComChain.h"


class abstractRunMode{
protected:
    comChain *com;

public:
    explicit abstractRunMode(comChain *com):com(com){

    }
    /**
     * execute the mode.
     * @param return  true on success
     */
    virtual bool executeMode() = 0;
    /**
     * Analyze the result of the mode execution.
     * This might be the evaluation of the rest result
     */
    virtual string analyseResult() = 0;

    /**
     * External trigger to terminalte the mode and try to make an clean exit.
     * This might be the "last" try to evaluate the targets result buffer
     */
    virtual void modeFailed(){Log::log("using (empty)default fail mode function",Info)};
};
#endif //GDBMANIPULATOR_RUNMODE_H
