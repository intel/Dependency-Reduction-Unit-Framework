//
// Created by basto on 10/16/18.
//
#include <gtest/gtest.h>

#include "argvParser.h"

using namespace std;

string t1, t2, t3;
bool a1, a2, a3;

int callBack1(int i, char *buff[]) {
    i++;
    t1 = buff[i];
    i++;
    t2 = buff[i];
    return i;
}

int callBack2(int i, char *buff[]) {
    i++;
    t3 = buff[i];
    return i;
}

int callBack3(int i, char *buff[]) {
    a1 = true;
    return i;
}

TEST(argvParser, systemtest1) {
    argvParser parser("test");
    parser.addArg("a", "aa", "tests", callBack1)->required();
    parser.addArg("b", "bb", "tests", callBack2)->required();

    char *arg[] = {"program", "../tests/TEST_configFiles/config1.txt", "../tests/TEST_configFiles/config2.txt"};

    ASSERT_FALSE(parser.analyseArgv(3, arg));
    ASSERT_EQ(t1, "test");
    ASSERT_EQ(t2, "x y z ");

}

TEST(argvParser, systemtest2) {
    argvParser parser("tests");
    parser.addArg("a", "aa", "tests", callBack1)->required();
    parser.addArg("b", "bb", "tests", callBack2);

    char *arg[] = {"program", "../tests/TEST_configFiles/config1.txt"};

    ASSERT_TRUE(parser.analyseArgv(2, arg));
    ASSERT_EQ(t1, "test");
    ASSERT_EQ(t2, "x y z ");

}

TEST(argvParser, systemtest3) {
    a1 = false;
    argvParser parser("test");
    parser.addArg("a", "aa", "tests", callBack1)->required();
    parser.addArg("b", "bb", "tests", callBack2);
    parser.addArg("t1", "tt1", "tests", callBack3)->required();
    parser.addArg("t2", "tt2", "tests", callBack3)->required();
    parser.addArg("t3", "tt3", "tests", callBack3)->required();

    char *arg[] = {"program", "../tests/TEST_configFiles/config1.txt", "../tests/TEST_configFiles/config2.txt"};

    ASSERT_TRUE(parser.analyseArgv(3, arg));
    ASSERT_TRUE(a1);
    ASSERT_EQ(t1, "test");
    ASSERT_EQ(t2, "x y z ");

}

TEST(argvParser, systemtest4) {
    a1 = false;
    argvParser parser("test");
    parser.addArg("a", "aa", "tests", callBack1)->required();
    parser.addArg("b", "bb", "tests", callBack2);
    parser.addArg("t1", "tt1", "tests", callBack3)->required();
    parser.addArg("t2", "tt2", "tests", callBack3)->required();
    parser.addArg("t3", "tt3", "tests", callBack3)->required();

    char *arg[] = {"program", "a", "b", "c", "../tests/TEST_configFiles/config1.txt", "../tests/TEST_configFiles/config2.txt"};

    ASSERT_TRUE(parser.analyseArgv(6, arg));
    ASSERT_TRUE(a1);
    ASSERT_EQ(t1, "test");
    ASSERT_EQ(t2, "x y z ");

}
