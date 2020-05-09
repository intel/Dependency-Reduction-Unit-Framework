//
// Created by basto on 12/15/18.
//

#include <gtest/gtest.h>

#include "argvParser.h"

string port_x = "";
bool x = false;

int callBackPortTestMeth1(int i, char *buff[]) {
    i++;
    port_x = buff[i];
    return i;
}

int callBackD1(int i, char *buff[]) {
    x = true;
    return i;
}

TEST(DefinedNumerOfArgsr, numberOfARgsShould1is0) {
    port_x = "";
    argvParser parser("tests");
    parser.addArg("p", "port", "adjust the network port", callBackPortTestMeth1)->numberOfParameter(1);
    char *arg[] = {"program", "p",};
    ASSERT_FALSE(parser.analyseArgv(2, arg));
}

TEST(DefinedNumerOfArgsr, numberOfARgsShould1is1) {
    port_x = "";
    argvParser parser("tests");
    parser.addArg("p", "port", "tests", callBackPortTestMeth1)->numberOfParameter(1);
    char *arg[] = {"program", "p", "abc",};
    ASSERT_TRUE(parser.analyseArgv(3, arg));
    ASSERT_EQ(port_x, "abc");
}

TEST(DefinedNumerOfArgsr, numberOfARgsShould2is1) {
    port_x = "";
    argvParser parser("tests");
    parser.addArg("p", "port", "tests", callBackPortTestMeth1)->numberOfParameter(2);
    char *arg[] = {"program", "p", "abc",};
    ASSERT_FALSE(parser.analyseArgv(3, arg));
}

TEST(DefinedNumerOfArgsr, numberOfARgsShould3is1) {
    port_x = "";
    argvParser parser("tests");
    parser.addArg("p", "port", "tests", callBackPortTestMeth1)->numberOfParameter(3);
    char *arg[] = {"program", "p", "abc",};
    ASSERT_FALSE(parser.analyseArgv(3, arg));
}

TEST(DefinedNumerOfArgsr, numberOfARgsShould0is0) {
    port_x = "";
    x = false;
    argvParser parser("tests");
    parser.addArg("p", "port", "tests", callBackD1)->numberOfParameter(0);
    char *arg[] = {"program", "p", "abc",};
    ASSERT_FALSE(parser.analyseArgv(3, arg));
    ASSERT_TRUE(x);
}
