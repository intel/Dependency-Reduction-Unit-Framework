/**
 * Copyright (C) 2020 Intel Corporation
 * SPDX-License-Identifier: AGPL-3.0-or-later
 * @author: Sebastian Balz
 */


#ifndef GDBMANIPULATOR_FLASHMODE_H
#define GDBMANIPULATOR_FLASHMODE_H


#include "AbstractRunMode.h"



class flashMode: public abstractRunMode {
	bool FlashSuccess = false;
public:
	flashMode(comChain *com):abstractRunMode(com){

    };

    bool executeMode() override;

    string analyseResult() override;


};


#endif //GDBMANIPULATOR_FLASHMODE_H
