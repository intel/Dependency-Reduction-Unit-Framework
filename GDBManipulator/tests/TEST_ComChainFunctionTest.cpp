//
// Created by lapi on 15/11/2018.
//


#include <gtest/gtest.h>
#include "comChain/ComChain.h"


int statusValue = 0;
bool tmp = false;
stopFrame *s;

class rootChain : public comChain {

public:
    rootChain() : comChain(nullptr) {};

    stopFrame *jumpOutOfCurrentFunction(string returnValue) {
        return s;
    }


    virtual bool initSystem() {
        statusValue = 1;
        return tmp;
    }
};


class ChainElement : public comChain {

public:
    ChainElement(comChain *next) : comChain(next) {

    }

    bool initSystem() {
        return true;
    }
};

TEST(comChainFuctionTest, accessToBaseClass) {
    comChain *c = new rootChain();
    tmp = true;
    ASSERT_FALSE(c->executionHasStopped());
    ASSERT_EQ(c->getResultData(), nullptr);
}

TEST(comChainFuctionTest, accessToRootClass) {
    comChain *c = new rootChain();
    tmp = false;
    ASSERT_FALSE(c->initSystem());
    tmp = true;
    ASSERT_TRUE(c->initSystem());
    ASSERT_EQ(c->getResultData(), nullptr);
}

TEST(comChainFuctionTest, accessToRootClass2) {
    comChain *c = new rootChain();
    tmp = false;
    ASSERT_FALSE(c->initSystem());
    tmp = true;
    s = new stopFrame();
    s->function = "test1";
    ASSERT_EQ(c->jumpOutOfCurrentFunction("")->function, "test1");
}


TEST(comChainFuctionTest, ChainWithTwoElements) {
    comChain *c1 = new rootChain();
    comChain *c = new ChainElement(c1);
    tmp = false;
    ASSERT_FALSE(c1->initSystem());
    ASSERT_TRUE(c->initSystem());
    tmp = true;
    ASSERT_TRUE(c1->initSystem());
    ASSERT_TRUE(c->initSystem());
    s = new stopFrame();
    s->function = "test1";
    ASSERT_EQ(c->jumpOutOfCurrentFunction("")->function, "test1");
}

