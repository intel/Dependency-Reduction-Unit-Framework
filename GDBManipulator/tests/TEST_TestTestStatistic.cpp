//
// Created by basto on 10/30/18.
//

#include <gtest/gtest.h>
#include "tester/TestStatistic.h"
#include "Utils.h"

TEST(TestStatistic, one_MemoryRead) {
    testStatistic::resetStatistic();
    testStatistic::startMemRead();
    usleep(1000);
    testStatistic::endMemRead(10, 10);
    testStatistic::getMemReads();
    string s = testStatistic::getMemReads();
    bool eq = utils::startsWith(&s, "   all memory reads: \n\t 0 : \tread 10 bytes \tfrom 10 \t in");
    ASSERT_TRUE(eq);
}

TEST(TestStatistic, readMemoryTwice) {
    testStatistic::resetStatistic();
    testStatistic::startMemRead();
    usleep(1100);
    testStatistic::endMemRead(10, 10);
    testStatistic::startMemRead();
    usleep(1100);
    testStatistic::endMemRead(10, 1);

    string s = testStatistic::getMemReads();
    bool eq = utils::startsWith(&s, "   all memory reads: \n\t 0 : \tread 10 bytes \tfrom 10 \t in ");
    ASSERT_TRUE(eq);

}

TEST(TestStatistic, noReads) {
    testStatistic::resetStatistic();
    testStatistic::startMemWrite();
    usleep(1000);
    testStatistic::endMemWrite(10);

    ASSERT_EQ("   all memory reads: ",
              testStatistic::getMemReads());
}

TEST(TestStatistic, readMemoryTwiceWithNoDelay) {
    testStatistic::resetStatistic();
    testStatistic::startMemRead();
    testStatistic::endMemRead(10, 10);
    testStatistic::startMemRead();
    testStatistic::endMemRead(10, 1);

    string s = testStatistic::getMemReads();
    bool eq = utils::startsWith(&s,
                                "   all memory reads: \n\t 0 : \tread 10 bytes \tfrom 10 \t in 0 ms\n\t 1 : \tread 1 bytes \tfrom 10 \t in");
    ASSERT_TRUE(eq);
}

TEST(TestStatistic, noMemWrites) {
    testStatistic::resetStatistic();
    testStatistic::startMemRead();
    usleep(1000);
    testStatistic::endMemRead(1, 10);
    ASSERT_EQ("   all memory writes: ",
              testStatistic::getMemWrites());
}

TEST(TestStatistic, writeMemoryRead) {
    testStatistic::resetStatistic();
    testStatistic::startMemWrite();
    usleep(1000);
    testStatistic::endMemWrite(10);
    string s = testStatistic::getMemWrites();
    bool eq = utils::startsWith(&s, "   all memory writes: \n\t 0 : \twrite 10 bytes \tin");
    ASSERT_TRUE(eq);
}

TEST(TestStatistic, writeMemoryTwice) {
    testStatistic::resetStatistic();
    testStatistic::startMemWrite();
    testStatistic::endMemWrite(10);
    testStatistic::startMemWrite();
    usleep(1100);
    testStatistic::endMemWrite(1);

    string s = testStatistic::getMemWrites();
    bool eq = utils::startsWith(&s, "   all memory writes: \n\t 0 : \twrite 10 bytes \tin ");
    ASSERT_TRUE(eq);
}

TEST(TestStatistic, writeMemoryTwiceWithNoDelay) {
    testStatistic::resetStatistic();
    testStatistic::startMemWrite();
    testStatistic::endMemWrite(10);
    testStatistic::startMemWrite();
    testStatistic::endMemWrite(1);


    string s = testStatistic::getMemWrites();
    bool eq = utils::startsWith(&s, "   all memory writes: \n\t 0 : \twrite 10 bytes \tin ");
    ASSERT_TRUE(eq);
}


TEST(TestStatistic, mocksANdStrubs_noStubs) {
    testStatistic::resetStatistic();
    ASSERT_EQ("   all mock and Stubs per test group: ", testStatistic::getMocksAndStubs());
}

TEST(TestStatistic, mocksANdStrubs_oneStub1) {
    testStatistic::resetStatistic();
    testStatistic::setTestGroupName("g1");
    testStatistic::addStub();
    ASSERT_EQ("   all mock and Stubs per test group: \n\t group: g1 \t :  Stubs: 1  Mocks: 0  deletions: 0",
              testStatistic::getMocksAndStubs());
}

TEST(TestStatistic, mocksAndStrubs_oneStub2) {
    testStatistic::resetStatistic();
    testStatistic::addStub();
    testStatistic::setTestGroupName("g1");
    ASSERT_EQ("   all mock and Stubs per test group: \n\t group: g1 \t :  Stubs: 1  Mocks: 0  deletions: 0",
              testStatistic::getMocksAndStubs());
}

TEST(TestStatistic, mocksAndStrubs_oneMock1) {
    testStatistic::resetStatistic();
    testStatistic::setTestGroupName("g1");
    testStatistic::addMock();
    ASSERT_EQ("   all mock and Stubs per test group: \n\t group: g1 \t :  Stubs: 0  Mocks: 1  deletions: 0",
              testStatistic::getMocksAndStubs());
}

TEST(TestStatistic, mocksAndStrubs_oneMock2) {
    testStatistic::resetStatistic();
    testStatistic::addMock();
    testStatistic::setTestGroupName("g1");
    ASSERT_EQ("   all mock and Stubs per test group: \n\t group: g1 \t :  Stubs: 0  Mocks: 1  deletions: 0",
              testStatistic::getMocksAndStubs());
}

TEST(TestStatistic, mocksANdStrubs_oneDl1) {
    testStatistic::resetStatistic();
    testStatistic::setTestGroupName("g1");
    testStatistic::addMockDel();
    ASSERT_EQ("   all mock and Stubs per test group: \n\t group: g1 \t :  Stubs: 0  Mocks: 0  deletions: 1",
              testStatistic::getMocksAndStubs());
}

TEST(TestStatistic, mocksAndStrubs_oneDel2) {
    testStatistic::resetStatistic();
    testStatistic::addMockDel();
    testStatistic::setTestGroupName("g1");
    ASSERT_EQ("   all mock and Stubs per test group: \n\t group: g1 \t :  Stubs: 0  Mocks: 0  deletions: 1",
              testStatistic::getMocksAndStubs());
}

TEST(TestStatistic, mocksAndStrubs_twoGroups) {
    testStatistic::resetStatistic();
    testStatistic::addMockDel();
    testStatistic::setTestGroupName("g1");
    testStatistic::setTestGroupName("g2");
    testStatistic::addStub();
    ASSERT_EQ(
            "   all mock and Stubs per test group: \n\t group: g1 \t :  Stubs: 0  Mocks: 0  deletions: 1\n\t group: g2 \t :  Stubs: 1  Mocks: 0  deletions: 0",
            testStatistic::getMocksAndStubs());
}

TEST(TestStatistic, mocksAndStrubstGroups) {
    testStatistic::resetStatistic();
    testStatistic::addMockDel();
    testStatistic::setTestGroupName("g1");
    testStatistic::setTestGroupName("g2");
    testStatistic::addStub();
    testStatistic::setTestGroupName("g3");
    testStatistic::addMock();
    ASSERT_EQ(
            "   all mock and Stubs per test group: \n\t group: g1 \t :  Stubs: 0  Mocks: 0  deletions: 1\n\t group: g2 \t :  Stubs: 1  Mocks: 0  deletions: 0\n\t group: g3 \t :  Stubs: 0  Mocks: 1  deletions: 0",
            testStatistic::getMocksAndStubs());
}




