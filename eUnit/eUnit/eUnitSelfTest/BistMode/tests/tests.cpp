/**
 * Copyright (C) 2020 Intel Corporation
 * SPDX-License-Identifier: Apache-2.0
 * @author: Sebastian Balz
 */

#include <stdint.h>
extern uint8_t evaluateGPIO();
extern uint8_t funktionWithDep();
extern uint8_t error_code;

#include "../../../eUnit.h"


DISABLED_TEST(DISABLED_Test,test1){
	printOverGdb("it is possibel to disable generatet test by setting the group prefex to DISABLED");
	ASSERT_TRUE(0);
}
