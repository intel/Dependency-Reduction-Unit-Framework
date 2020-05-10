//
// Created by basto on 6/29/18.
//

#include "gtest/gtest.h"
#include "tester/TargetMock.h"


TEST(Mocks, addMockName) {
    targetMock *m = new targetMock();
    m->addMock("test1", "abc");
    ASSERT_TRUE(m->hasMocks());
    ASSERT_TRUE(m->hasMock("test1"));
    ASSERT_EQ(m->getThen("test1"), "abc");
}

TEST(Mocks, addMockName2) {
    targetMock *m = new targetMock();
    m->addMock("test1", "abc");
    m->addMock("test12", "abcd");
    ASSERT_TRUE(m->hasMocks());
    ASSERT_TRUE(m->hasMock("test1"));
    ASSERT_TRUE(m->hasMock("test12"));
    ASSERT_EQ(m->getThen("test1"), "abc");
    ASSERT_EQ(m->getThen("test12"), "abcd");
}

TEST(Mocks, deleteMockName) {
    targetMock *m = new targetMock();
    m->addMock("test1", "abc");
    m->addMock("test12", "abcd");
    ASSERT_TRUE(m->hasMocks());
    ASSERT_TRUE(m->hasMock("test1"));
    ASSERT_TRUE(m->hasMock("test12"));
    ASSERT_EQ(m->getThen("test1"), "abc");
    ASSERT_EQ(m->getThen("test12"), "abcd");
    m->deleteAllMocks();
    ASSERT_FALSE(m->hasMocks());
    ASSERT_FALSE(m->hasMock("test1"));
    ASSERT_FALSE(m->hasMock("test12"));
}

