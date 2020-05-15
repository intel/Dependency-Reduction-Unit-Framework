/**
 * Copyright (C) 2020 Intel Corporation
 * SPDX-License-Identifier: AGPL-3.0-or-later
 * @author: Sebastian Balz
 */

#include "../../../eUnit.h"


TEST(TEST_SOULD_SUCCEED_ASSERT, ASSERT_EQ){
	// uint8_t
	ASSERT_EQ(0,0);
	ASSERT_EQ(1,1);
	ASSERT_EQ(-1,-1);
	// uint32_t
	ASSERT_EQ(100000,100000);
	ASSERT_EQ(-100000,-100000);
}

TEST(TEST_SOULD_SUCCEED_ASSERT, ASSERT_NEQ){
	// uint8_t
	ASSERT_NEQ(42,0);
	ASSERT_NEQ(1,-1);
	ASSERT_NEQ(-1,0);
	ASSERT_NEQ(0,-1);
	// uint32_t
	ASSERT_NEQ(-100000,100000);
	ASSERT_NEQ(100000,-100000);
}
TEST(TEST_SOULD_SUCCEED_ASSERT, ASSERT_NE){
	// uint8_t
	ASSERT_NE(42,0);
	ASSERT_NE(1,-1);
	ASSERT_NE(-1,0);
	ASSERT_NE(0,-1);
	// uint32_t
	ASSERT_NE(-100000,100000);
	ASSERT_NE(100000,-100000);
}

TEST(TEST_SOULD_SUCCEED_ASSERT, ASSERT_TRUE){

	ASSERT_TRUE(1);
	ASSERT_TRUE(2);
	ASSERT_TRUE(99);
}
TEST(TEST_SOULD_SUCCEED_ASSERT, ASSERT_FALSE){
	ASSERT_FALSE(0);
	ASSERT_FALSE(-1);
	ASSERT_FALSE(-99);
}

char buff[] = "test string";
TEST(TEST_SOULD_SUCCEED_ASSERT,ASSERT_STREQ){
	ASSERT_STREQ("test string","test string");
	ASSERT_STREQ(buff,buff);
	ASSERT_STREQ(buff,"test string");
	ASSERT_STREQ("test string",buff);
}

TEST(TEST_SOULD_FAIL_ASSERT, ASSERT_EQ1){
	// uint8_t
	ASSERT_EQ(0,1);
	printOverGdb("should not get printed!");
	ASSERT_EQ(1,1);
}
TEST(TEST_SOULD_FAIL_ASSERT, ASSERT_EQ2){
	// uint8_t
	ASSERT_EQ(1,0);
	printOverGdb("should not get printed!");
	ASSERT_EQ(1,1);
}


TEST(TEST_SOULD_FAIL_ASSERT, ASSERT_NEQ1){
	// uint8_t
	ASSERT_NEQ(1,1);
	printOverGdb("should not get printed!");
	ASSERT_EQ(1,1);
}
TEST(TEST_SOULD_FAIL_ASSERT, ASSERT_NEQ2){
	// uint8_t
	ASSERT_NEQ(0,0);
	printOverGdb("should not get printed!");
	ASSERT_EQ(1,1);
}

TEST(TEST_SOULD_FAIL_ASSERT, ASSERT_TRUE){
	ASSERT_TRUE(0);
	printOverGdb("should not get printed!");
	ASSERT_EQ(1,1);
}

TEST(TEST_SOULD_FAIL_ASSERT, ASSERT_TRUE2){
	ASSERT_TRUE(0);
	printOverGdb("should not get printed!");
	ASSERT_EQ(1,1);
}

TEST(TEST_SOULD_FAIL_ASSERT, ASSERT_FALSE1){
	ASSERT_FALSE(1);
	printOverGdb("should not get printed!");
	ASSERT_EQ(1,1);
}
TEST(TEST_SOULD_FAIL_ASSERT, ASSERT_FALSE2){
	ASSERT_FALSE(99);
	printOverGdb("should not get printed!");
	ASSERT_EQ(1,1);
}


