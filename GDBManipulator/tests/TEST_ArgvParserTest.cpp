//
// Created by basto on 4/15/18.
//

#include <gtest/gtest.h>

#include "argvParser.h"

string port_ = "";
bool d = false;

int callBackPortTestMeth(int i, char *buff[]) {
    i++;
    port_ = buff[i];
    return i;
}

int callBackD(int i, char *buff[]) {
    d = true;
    return i;
}

TEST(argvParser, bufferToSmall) {
    argvParser parser("tests");
    parser.addArg("t", "tt", "tests", nullptr)->numberOfParameter(1);
    char *arg[] = {"program", NULL};
    ASSERT_FALSE(parser.analyseArgv(2, arg));
}

TEST(argvParser, CheckRequiredConfigFail1) {
    argvParser parser("tests");
    parser.addArg("t", "tt", "tests", callBackD)->required();
    char *arg[] = {"program",};
    ASSERT_FALSE(parser.analyseArgv(1, arg));
}

TEST(argvParser, CheckRequiredConfigFail2) {
    argvParser parser("tests");
    parser.addArg("t", "tt", "tests", callBackD)->required();
    parser.addArg("x", "xx", "tests", callBackD);
    char *arg[] = {"program", "x"};
    ASSERT_FALSE(parser.analyseArgv(2, arg));

}

TEST(argvParser, CheckRequiredConfigFail3) {
    argvParser parser("tests");
    parser.addArg("t", "tt", "tests", callBackD)->required();
    parser.addArg("x", "xx", "tests", callBackD)->required();
    char *arg[] = {"program", "x"};
    ASSERT_FALSE(parser.analyseArgv(2, arg));
}

TEST(argvParser, CheckRequiredConfigFail4) {
    argvParser parser("tests");
    parser.addArg("t", "tt", "tests", callBackD)->required();
    parser.addArg("x", "xx", "tests", callBackD)->required();
    char *arg[] = {"program", "t"};
    ASSERT_FALSE(parser.analyseArgv(2, arg));
}

TEST(argvParser, CheckRequiredConfigFail5) {
    argvParser parser("tests");
    parser.addArg("t", "tt", "tests", callBackD);
    parser.addArg("x", "xx", "tests", callBackD)->required();
    char *arg[] = {"program", "t"};
    ASSERT_FALSE(parser.analyseArgv(2, arg));
}

TEST(argvParser, CheckRequiredConfigFail6) {
    argvParser parser("tests");
    parser.addArg("t", "tt", "tests", callBackD)->required();
    parser.addArg("x", "xx", "tests", callBackD)->required();
    char *arg[] = {"program", "t", "xx"};
    ASSERT_TRUE(parser.analyseArgv(3, arg));
}

TEST(argvParser, CheckRequiredConfigFail7) {
    argvParser parser("tests");
    parser.addArg("t", "tt", "tests", callBackD)->required();
    parser.addArg("x", "xx", "tests", callBackD)->required();
    parser.addArg("a", "aa", "tests", callBackD);
    char *arg[] = {"program", "t", "xx"};
    ASSERT_TRUE(parser.analyseArgv(3, arg));
}

TEST(argvParser, CheckRequiredConfigFail8) {
    argvParser parser("tests");
    parser.addArg("t", "tt", "tests", callBackD)->required();
    parser.addArg("x", "xx", "tests", callBackD)->required();
    parser.addArg("a", "aa", "tests", callBackD)->required();
    char *arg[] = {"program", "t", "xx", "aa"};
    ASSERT_TRUE(parser.analyseArgv(4, arg));
}

TEST(argvParser, emptyConfig_testHelpMessage) {
    argvParser parser("empty conf1","",true);
    ASSERT_EQ("empty conf1\n\n\nUsage:\n", parser.getHelpMessage());
}

TEST(argvParser, testHelpMessage) {
    argvParser parser("empty conf");
    ASSERT_TRUE(parser.addArg("a", "aaa", "helpMessage", nullptr));
    ASSERT_EQ("empty conf\n\n\nUsage:\n     <a>         <aaa>                        : helpMessage\n",
              parser.getHelpMessage());
}

TEST(argvParser, testHelpMessageTwoArgs) {
    argvParser parser("empty conf");
    ASSERT_TRUE(parser.addArg("a", "aaa", "helpMessage", nullptr));
    ASSERT_TRUE(parser.addArg("b", "bbb", "helpMessage2", nullptr));
    ASSERT_EQ(
            "empty conf\n\n\nUsage:\n     <a>         <aaa>                        : helpMessage\n     <b>         <bbb>                        : helpMessage2\n",
            parser.getHelpMessage());
}


TEST(argvParser, argWithCallBackAndNoMachingArg) {
    argvParser parser("empty conf");
    char *arg[] = {"program", "-p", "61234", "-d"};
    parser.addArg("", "", "", nullptr);
    ASSERT_FALSE(parser.analyseArgv(4, arg));

}

TEST(argvParser, argWithCallBackAndNoMachingArg2) {
    argvParser parser("empty conf");
    char *arg[] = {"program", "-p", "61234", "-d"};
    parser.addArg("-x", "-x", "xxx", nullptr);
    ASSERT_FALSE(parser.analyseArgv(4, arg));

}

TEST(argvParser, argWithCallBackAndNoMachingArg3) {
    char *arg[] = {"program", "-port", "61234", "-dd", "blub"};
    argvParser parser("empty conf");
    ASSERT_TRUE(parser.addArg("-p", "-port", "add Port", callBackPortTestMeth));
    ASSERT_TRUE(parser.addArg("-d", "-dd", "add d", callBackD));
    port_ = "";
    d = false;
    ASSERT_FALSE(parser.analyseArgv(5, arg));
    ASSERT_EQ(port_, "61234");
    ASSERT_TRUE(d);

}

TEST(argvParser, argWithCallBackShOrt) {
    char *arg[] = {"program", "-p", "61234", "-d"};
    argvParser parser("empty conf");
    parser.addArg("-p", "-port", "add Port", callBackPortTestMeth);
    parser.addArg("-d", "-dd", "add d", callBackD);
    port_ = "";
    d = false;
    ASSERT_TRUE(parser.analyseArgv(4, arg));
    ASSERT_EQ(port_, "61234");
    ASSERT_TRUE(d);

}

TEST(argvParser, argWithCallBackLong) {
    char *arg[] = {"program", "-port", "61234", "-dd"};
    argvParser parser("empty conf");
    parser.addArg("-p", "-port", "add Port", callBackPortTestMeth);
    parser.addArg("-d", "-dd", "add d", callBackD);
    ASSERT_TRUE(parser.analyseArgv(4, arg));
    ASSERT_EQ(port_, "61234");
    ASSERT_TRUE(d);

}

TEST(argvParser, NoArgumentsPassed) {
    char *arg[] = {"program", "a"};
    port_ = "61234";
    d = true;
    argvParser parser("empty conf");
    parser.addArg("-p", "-port", "add Port", callBackPortTestMeth);
    parser.addArg("-d", "-dd", "add d", callBackD);
    ASSERT_TRUE(parser.analyseArgv(1, arg));
    ASSERT_EQ(port_, "61234");
    ASSERT_TRUE(d);

}

TEST(argvParser, ArgumentIsMoreThanDefined) {
    char *arg[] = {"program", "abcd"};
    argvParser parser("tests");
    parser.addArg("abc", "abc", "add something", nullptr);
    ASSERT_FALSE(parser.analyseArgv(2, arg));

}


TEST(argvParser, addShortArgTwice) {
    char *arg[] = {"program", "a"};
    argvParser parser("empty conf");
    ASSERT_TRUE(parser.addArg("-p", "-abc", "add Port", callBackPortTestMeth));
    ASSERT_FALSE(parser.addArg("-p", "-dd", "add d", callBackD));
}

TEST(argvParser, addLongArgTwice1) {
    char *arg[] = {"program", "a"};
    argvParser parser("empty conf");
    ASSERT_TRUE(parser.addArg("-xx", "-abc", "add Port", callBackPortTestMeth));
    ASSERT_FALSE(parser.addArg("-p", "-abc", "add d", callBackD));
}

TEST(argvParser, addLongArgumentEqToExistingShortArg) {
    char *arg[] = {"program", "-a"};
    argvParser parser("empty conf");
    ASSERT_TRUE(parser.addArg("-abc", "-xx", "add Port", callBackPortTestMeth));
    ASSERT_FALSE(parser.addArg("-p", "-abc", "add d", callBackD));
}

TEST(argvParser, requiredArgsNotSearchedJet) {
    char *arg[] = {"program", "-a"};
    argvParser parser("empty conf");
    ASSERT_TRUE(parser.addArg("-a", "-aa", "aa", nullptr)->required());
    ASSERT_FALSE(parser.foundAllRequierdArgs());

}

TEST(argvParser, requiredArgsNotSearchedButNotAllFound) {
    char *arg[] = {"program", "-a"};
    argvParser parser("empty conf");
    ASSERT_TRUE(parser.addArg("-a", "-aa", "aa", callBackD)->required());
    ASSERT_TRUE(parser.addArg("-b", "-bb", "bb", callBackD)->required());
    ASSERT_FALSE(parser.analyseArgv(2, arg));
    ASSERT_FALSE(parser.foundAllRequierdArgs());

}

TEST(argvParser, requiredArgsNotSearchedButNotAllFoundOther) {
    char *arg[] = {"program", "-b"};
    argvParser parser("empty conf");
    ASSERT_TRUE(parser.addArg("-a", "-aa", "aa", callBackD)->required());
    ASSERT_TRUE(parser.addArg("-b", "-bb", "bb", callBackD));
    ASSERT_FALSE(parser.analyseArgv(2, arg));
    ASSERT_FALSE(parser.foundAllRequierdArgs());

}

TEST(argvParser, requiredArgsNotSearchedButNotAllFoundCorrect) {
    char *arg[] = {"program", "-a"};
    argvParser parser("empty conf");
    ASSERT_TRUE(parser.addArg("-a", "-aa", "aa", callBackD)->required());
    ASSERT_TRUE(parser.addArg("-b", "-bb", "bb", callBackD));
    ASSERT_TRUE(parser.analyseArgv(2, arg));
    ASSERT_TRUE(parser.foundAllRequierdArgs());

}


TEST(readConfigFromFileAndCallBack, test1) {
    char *arg[] = {"program", "tests/TEST_configFiles/config6"};
    argvParser parser("empty conf");
    ASSERT_TRUE(parser.addArg("-a", "-aa", "aa", callBackD)->required());
    ASSERT_TRUE(parser.addArg("-b", "-bb", "bb", callBackD));
    ASSERT_FALSE(parser.analyseArgv(2, arg));
    ASSERT_FALSE(parser.foundAllRequierdArgs());
}


//123  b

TEST(readConfigFromFileAndCallBack, test2) {
    char *arg[] = {"program", "tests/TEST_configFiles/config7"};
    argvParser parser("empty conf");
    port_ = "";
    ASSERT_TRUE(parser.addArg("-p", "-pp", "p", callBackD)->required());
    ASSERT_TRUE(parser.addArg("-b", "-bb", "bb", callBackD)->required());
    ASSERT_FALSE(parser.analyseArgv(2, arg));
    ASSERT_FALSE(parser.foundAllRequierdArgs());
}

TEST(readConfigFromFileAndCallBack, test3) {
    char *arg[] = {"program", "../tests/TEST_configFiles/config7"};
    argvParser parser("empty conf");
    port_ = "";
    ASSERT_TRUE(parser.addArg("-p", "-pp", "p", callBackPortTestMeth)->required());
    ASSERT_TRUE(parser.addArg("-b", "-bb", "bb", callBackD));
    ASSERT_TRUE(parser.analyseArgv(2, arg));
    ASSERT_TRUE(parser.foundAllRequierdArgs());
    ASSERT_EQ(port_, "123  b");
}

TEST(readConfigFromFileAndCallBack, test4) {
    char *arg[] = {"program", "-p", "xx", "../tests/TEST_configFiles/config7"};
    argvParser parser("empty conf");
    port_ = "";
    ASSERT_TRUE(parser.addArg("-p", "-pp", "p", callBackPortTestMeth)->required());
    ASSERT_TRUE(parser.addArg("-b", "-bb", "bb", callBackD));
    ASSERT_TRUE(parser.analyseArgv(4, arg));
    ASSERT_TRUE(parser.foundAllRequierdArgs());
    ASSERT_EQ(port_, "123  b");
}

