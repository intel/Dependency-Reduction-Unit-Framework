/**
 * Copyright (C) 2020 Intel Corporation
 * SPDX-License-Identifier: AGPL-3.0-or-later
 * @author: Sebastian Balz
 */

#include "testSuite/testSuite.h"
#include "testSuite/asserts.h"

TEST(classSuccess,NEQ_SUCCESS){
  	uint8_t u1 = 1;
	uint32_t u2 = -1;
	ASSERT_NEQ(0,1);
	ASSERT_NEQ(1,0);
	ASSERT_NEQ(-1,0);
	ASSERT_NEQ(-1,1);
	ASSERT_NEQ(u1,u2);
}

TEST(classSuccess,EQ_SUCCESS){
  	uint8_t u1 = 1;
	uint32_t u2 = -1;
	uint32_t u3 = 1;
	ASSERT_EQ(0,0);
	ASSERT_EQ(1,1);
	ASSERT_EQ(-1,-1);
	ASSERT_EQ(u1,u1);
	ASSERT_EQ(u2,u2);
	ASSERT_EQ(u1,u3);
}

TEST(classFail,NEQ_FAIL){
	uint8_t u1 = 1;
	uint32_t u2 = -1;
	uint32_t u3 = 1;
	
	ASSERT_NEQ(0,0);
	ASSERT_NEQ(1,1);
	ASSERT_NEQ(-1,-1);
	ASSERT_NEQ(u1,u1);
	ASSERT_NEQ(u2,u2);
	ASSERT_NEQ(u1,u3);
}





