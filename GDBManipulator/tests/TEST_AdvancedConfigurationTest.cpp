//
// Created by basto on 1/1/19.
//

#include <gtest/gtest.h>

#include "argvParser.h"

using namespace std;
string advtt;
bool advtt2;

int advCall2(int i, char *buff[]) {
    i++;
    advtt = buff[i];
    return i;
}

int advCall1(int i, char *buff[]) {
    advtt2 = true;
    return i + 1;
}

TEST(advancedConfiguration, CheckRequiredConfigFail1) {
    argvParser parser("tests");
    parser.addArg("t", "tt", "tests", advCall1)->required();
    char *arg[] = {"program", ""};
    ASSERT_FALSE(parser.analyseArgv(2, arg));
}

TEST(advancedConfiguration, CheckRequiredConfigFail2) {
    argvParser parser("tests");
    parser.addArg("t", "tt", "tests", advCall1)->required()->asFile();
    char *arg[] = {"program", ""};
    ASSERT_FALSE(parser.analyseArgv(2, arg));
}

TEST(advancedConfiguration, CheckRequiredConfigFail3) {
    argvParser parser("tests");
    parser.addArg("t", "tt", "tests", advCall1)->asFile()->required();
    char *arg[] = {"program", ""};
    ASSERT_FALSE(parser.analyseArgv(2, arg));
}

TEST(advancedConfiguration, CheckRequiredConfigFail4) {
    argvParser parser("tests");
    parser.addArg("t", "tt", "tests", advCall1)->numberOfParameter(1)->required();
    char *arg[] = {"program", ""};
    ASSERT_FALSE(parser.analyseArgv(2, arg));
}

TEST(advancedConfiguration, CheckRequiredConfigFail5) {
    argvParser parser("tests");
    parser.addArg("t", "tt", "tests", advCall1)->numberOfParameter(0)->required();
    char *arg[] = {"program", ""};
    ASSERT_FALSE(parser.analyseArgv(2, arg));
}

TEST(advancedConfiguration, CheckRequiredConfigFail6) {
    argvParser parser("tests");
    parser.addArg("t", "tt", "tests", advCall1)->numberOfParameter(-1)->required();
    char *arg[] = {"program", ""};
    ASSERT_FALSE(parser.analyseArgv(2, arg));
}

TEST(advancedConfiguration, CheckRequiredConfigFail7) {
    argvParser parser("tests");
    parser.addArg("t", "tt", "tests", advCall1)->numberOfParameter(2)->required();
    char *arg[] = {"program", ""};
    ASSERT_FALSE(parser.analyseArgv(2, arg));
}

TEST(advancedConfiguration, CheckNumberOfParameter1) {
    argvParser parser("tests");
    parser.addArg("t", "tt", "tests", advCall1)->numberOfParameter(2);
    char *arg[] = {"program", ""};
    ASSERT_FALSE(parser.analyseArgv(2, arg));
}

TEST(advancedConfiguration, CheckNumberOfParameter2) {
    argvParser parser("tests");
    parser.addArg("t", "tt", "tests", advCall1)->numberOfParameter(1);
    char *arg[] = {"program", ""};
    ASSERT_FALSE(parser.analyseArgv(2, arg));
}

TEST(advancedConfiguration, CheckNumberOfParameter3) {
    argvParser parser("tests");
    parser.addArg("t", "tt", "tests", advCall1)->numberOfParameter(3);
    char *arg[] = {"program", ""};
    ASSERT_FALSE(parser.analyseArgv(2, arg));
}

TEST(advancedConfiguration, Checkparam) {
    argvParser parser("tests");
    parser.addArg("t", "tt", "tests", advCall1)->allowedParameter(1, "abc")->required();
    char *arg[] = {"program", ""};
    ASSERT_FALSE(parser.analyseArgv(2, arg));
}

TEST(advancedConfiguration, Checkparam2) {
    argvParser parser("tests");
    parser.addArg("t", "tt", "tests", advCall1)->required()->allowedParameter(1, "abc");
    char *arg[] = {"program", ""};
    ASSERT_FALSE(parser.analyseArgv(2, arg));
}

TEST(advancedConfiguration, Checkparam3) {
    argvParser parser("tests");
    advtt2 = false;
    parser.addArg("t", "tt", "tests", advCall1)->required()->allowedParameter(1, "abc");
    char *arg[] = {"program", "t", "abc"};
    ASSERT_TRUE(parser.analyseArgv(3, arg));
    ASSERT_TRUE(advtt2);
}

TEST(advancedConfiguration, Checkparam4) {
    argvParser parser("tests");
    advtt2 = false;
    parser.addArg("t", "tt", "tests", advCall1)->required()->allowedParameter(1, "abc");
    char *arg[] = {"program", "t", "abcc"};
    ASSERT_FALSE(parser.analyseArgv(3, arg));
    ASSERT_FALSE(advtt2);
}

TEST(advancedConfiguration, Checkparam5) {
    argvParser parser("tests");
    advtt2 = false;
    parser.addArg("t", "tt", "tests", advCall1)->required()->allowedParameter(1, "abc");
    char *arg[] = {"program", "t", "ab"};
    ASSERT_FALSE(parser.analyseArgv(3, arg));
    ASSERT_FALSE(advtt2);
}

TEST(advancedConfiguration, Checkparam6) {
    argvParser parser("tests");
    advtt2 = false;
    parser.addArg("t", "tt", "tests", advCall1)->required()->allowedParameter(1, "abc");
    char *arg[] = {"program", "t", "abc "};
    ASSERT_FALSE(parser.analyseArgv(3, arg));
    ASSERT_FALSE(advtt2);
}
